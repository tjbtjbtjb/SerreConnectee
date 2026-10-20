#!/bin/bash
#
# Éclairage d'appoint piloté par la luminosité.
#   light.sh [arduino] [auto|off]
#
# auto (défaut, lancé par cron pendant la journée) :
#  - lumière éteinte et luminosité < LUX_ON          -> on allume ;
#  - lumière allumée et dernier test > TEST_EVERY s  -> test : on éteint, on attend SETTLE s,
#    on mesure ; si la luminosité naturelle >= LUX_KEEP_OFF on laisse éteint, sinon on rallume.
# off (fin de journée) : on éteint.
#
# L'état de la lumière est lu sur le capteur LIGHT, qui suit le relais en mode automatique.
# Le capteur AUTO ne voit pas le sélecteur manuel/automatique : le mode manuel se détecte
# quand une commande n'est pas suivie par LIGHT ; le script s'arrête alors sans rien changer.
# DRYRUN=1 light.sh ... : lit les capteurs et décrit les actions sans les exécuter.

arduino=arduino7
test $# -ge 1 && arduino=$1
mode=${2:-auto}

LUX_ON=50          # % : allumer en dessous
LUX_KEEP_OFF=55    # % : pendant un test, rester éteint au-dessus (hystérésis)
TEST_EVERY=1800    # s : intervalle entre deux tests d'extinction
SETTLE=20          # s : attente après extinction (> 6 s de cache capteur du firmware)

ldir=`dirname $0`
getval="$ldir/getValue.sh $arduino "
setval="$ldir/setValue.sh $arduino "
statedir=$HOME/.local/state/serre
statefile=$statedir/light_$arduino   # date (epoch) du dernier allumage ou test

log() { echo "$*"; logger -t serre-light "$arduino: $*"; }

# setlight <0|1> : commande puis vérifie sur le capteur LIGHT ; sort si la commande est sans effet
setlight() {
  if [ -n "$DRYRUN" ]
  then
    echo "[dry-run] set light $1"
    return 0
  fi
  $setval light $1 || { log "ERREUR : set light $1 refusé"; exit 20; }
  sleep 7  # laisser expirer le cache du capteur LIGHT
  seen=`$getval light || echo err`
  if [ "$seen" == "err" ] || [ `awk 'BEGIN {print ('$seen'!='$1')}'` -eq 1 ]
  then
    log "set light $1 sans effet (LIGHT = $seen) : serre en mode manuel ?"
    exit 0
  fi
}

mkdir -p $statedir
exec 9> $statedir/light_$arduino.lock
flock -n 9 || exit 0   # une exécution précédente (test en cours) n'est pas terminée

now=`date +%s`

if [ "$mode" == "off" ]
then
  LIGHT=`$getval light || echo err`
  if [ "$LIGHT" != "0.000" ]
  then
    log "fin de journée, extinction"
    setlight 0
  fi
  exit 0
fi

LASTTEST=0
test -r $statefile && LASTTEST=`awk '{print $NF}' $statefile`

LIGHT=`$getval light || echo err`
LUX=`$getval lux || echo err`
if [ "$LUX" == "err" ] || [ "$LIGHT" == "err" ]
then
  echo "Incorrect values." >&2
  exit 10
fi

if [ `awk 'BEGIN {print ('$LIGHT'==0)}'` -eq 1 ]
then
  if [ `awk 'BEGIN {print ('$LUX'<'$LUX_ON')}'` -eq 1 ]
  then
    log "luminosité $LUX % < $LUX_ON %, allumage"
    setlight 1
    test -z "$DRYRUN" && echo $now > $statefile
  fi
elif [ $((now - LASTTEST)) -ge $((TEST_EVERY - 60)) ]   # marge : les lancements cron dérivent de quelques secondes
then
  log "test : extinction pendant $SETTLE s (luminosité lampe allumée : $LUX %)"
  setlight 0
  test -z "$DRYRUN" && sleep $SETTLE
  LUXNAT=`$getval lux || echo err`
  test -z "$DRYRUN" && echo $now > $statefile
  if [ "$LUXNAT" == "err" ]
  then
    log "mesure impossible pendant le test, rallumage"
    setlight 1
  elif [ `awk 'BEGIN {print ('$LUXNAT'>='$LUX_KEEP_OFF')}'` -eq 1 ]
  then
    log "luminosité naturelle $LUXNAT % >= $LUX_KEEP_OFF %, lumière laissée éteinte"
  else
    log "luminosité naturelle $LUXNAT % < $LUX_KEEP_OFF %, rallumage"
    setlight 1
  fi
fi
