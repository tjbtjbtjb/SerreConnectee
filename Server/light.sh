#!/bin/bash
#
# Éclairage d'appoint piloté par la luminosité.
#   light.sh [arduino] [auto|off]
#
# auto (défaut, lancé par cron pendant la journée) :
#  - lumière éteinte et luminosité < LUX_ON          -> on allume ;
#  - lumière allumée depuis plus de TEST_EVERY s     -> test : on éteint, on attend SETTLE s,
#    on mesure ; si la luminosité naturelle >= LUX_KEEP_OFF on laisse éteint, sinon on rallume.
# off (fin de journée) : on éteint.
#
# Rien n'est fait si la serre est en mode manuel (AUTO = 0).
# L'état allumé/éteint est celui commandé par ce script (fichier d'état) : le capteur LIGHT lit
# l'entrée du panneau, il sert seulement de contrôle après chaque commande.
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
statefile=$statedir/light_$arduino   # "<0|1> <date du dernier test, epoch>"

log() { echo "$*"; logger -t serre-light "$arduino: $*"; }

setlight() {
  if [ -n "$DRYRUN" ]
  then
    echo "[dry-run] set light $1"
    return 0
  fi
  $setval light $1 || { log "ERREUR : set light $1 refusé"; exit 20; }
  echo "$1 $2" > $statefile
  sleep 7  # laisser expirer le cache du capteur LIGHT
  seen=`$getval light || echo err`
  test "$seen" != "err" && [ `awk 'BEGIN {print ('$seen'!='$1')}'` -eq 1 ] && \
    logger -t serre-light "$arduino: capteur LIGHT = $seen après set light $1 (entrée panneau != relais ?)"
  return 0
}

mkdir -p $statedir
exec 9> $statedir/light_$arduino.lock
flock -n 9 || exit 0   # une exécution précédente (test en cours) n'est pas terminée

AUTO=`$getval auto || echo err`
test "$AUTO" == "err" && echo "Incorrect values." >&2 && exit 10
if [ `awk 'BEGIN {print ('$AUTO'==0)}'` -eq 1 ]
then
  exit 0   # mode manuel : le panneau commande
fi

now=`date +%s`

if [ "$mode" == "off" ]
then
  log "fin de journée, extinction"
  setlight 0 $now
  exit 0
fi

if [ -r $statefile ]
then
  read LIGHT LASTTEST < $statefile
else
  LIGHT=`$getval light || echo err`
  LASTTEST=0
fi

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
    setlight 1 $now
  fi
elif [ $((now - LASTTEST)) -ge $TEST_EVERY ]
then
  log "test : extinction pendant $SETTLE s (luminosité lampe allumée : $LUX %)"
  setlight 0 $LASTTEST
  test -z "$DRYRUN" && sleep $SETTLE
  LUXNAT=`$getval lux || echo err`
  if [ "$LUXNAT" == "err" ]
  then
    log "mesure impossible pendant le test, rallumage"
    setlight 1 $now
  elif [ `awk 'BEGIN {print ('$LUXNAT'>='$LUX_KEEP_OFF')}'` -eq 1 ]
  then
    log "luminosité naturelle $LUXNAT % >= $LUX_KEEP_OFF %, lumière laissée éteinte"
    test -z "$DRYRUN" && echo "0 $now" > $statefile
  else
    log "luminosité naturelle $LUXNAT % < $LUX_KEEP_OFF %, rallumage"
    setlight 1 $now
  fi
fi
