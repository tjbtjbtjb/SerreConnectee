Configuration système de greenhouse
===================================

Copie des fichiers de configuration nécessaires au monitoring de la serre, relevés le 2026-10-20 sur
`greenhouse.physique.univ-paris-diderot.fr` (Raspberry Pi, Debian 13). Ces fichiers ne sont **pas** lus
depuis le dépôt : en cas de modification sur le serveur, recopier ici, et inversement.

Aucun secret ne doit être commité ici (clés TLS, `grafana.db`, mots de passe).

| Fichier du dépôt | Emplacement sur greenhouse | Rôle |
|---|---|---|
| `collectd/collectd.conf` | `/etc/collectd/collectd.conf` | Plugin `exec` qui lance `Server/dumpArduinoSensors_v4.sh` (utilisateur `beau:dialout`), envoi vers InfluxDB via le plugin `network` (127.0.0.1:25826), `TypesDB` incluant `Server/collectd_config/greenhouse_types.db` |
| `influxdb/influxdb.conf` | `/etc/influxdb/influxdb.conf` | Entrée `[[collectd]]` (port 25826, base `collectd`, `typesdb = /etc/influxdb/types/`) |
| `grafana/grafana.ini` | `/etc/grafana/grafana.ini` | Grafana sur 127.0.0.1:3000, servi sous `/grafana` (`root_url`, `serve_from_sub_path`) |
| `grafana/provisioning/datasources/influxdb.yaml` | `/etc/grafana/provisioning/datasources/` | Source de données `InfluxDB-collectd` |
| `grafana/provisioning/dashboards/serre.yaml` | `/etc/grafana/provisioning/dashboards/` | Provisioning du dashboard « Serre connectée » depuis `/var/lib/grafana/dashboards/serre/` |
| `grafana/dashboards/greenhouse-monitoring.json` | base Grafana (uid `BDIu8trmk`) | Export de l'ancien dashboard « GreenHouse Monitoring » (import manuel) |
| `apache2/000-default.conf` | `/etc/apache2/sites-available/000-default.conf` | Redirection HTTP → HTTPS, proxy `/grafana`, certificat Let's Encrypt (certbot) |
| `www/assets/*.svg` | `/var/www/html/assets/` | Logo Université Paris Cité et QR code GitHub affichés par le dashboard |

Le dashboard principal est généré par `../grafana/build_dashboard.py` :

    python3 Server/grafana/build_dashboard.py > serre-connectee.json
    sudo install -o grafana -g grafana -m 644 serre-connectee.json /var/lib/grafana/dashboards/serre/

Points d'attention
------------------

* `greenhouse_types.db` existe en deux exemplaires : collectd lit celui du dépôt
  (`Server/collectd_config/`), InfluxDB lit une **copie** dans `/etc/influxdb/types/`. Après ajout d'un
  type, recopier le fichier et redémarrer InfluxDB.
* `apache2/000-default.conf` référence `/etc/letsencrypt/live/...` : sur une nouvelle machine, partir
  d'un certificat auto-signé puis lancer
  `sudo certbot --apache -d greenhouse.physique.univ-paris-diderot.fr --no-redirect`.
  Modules Apache requis : `ssl`, `rewrite`, `proxy`, `proxy_http`.
* Les fichiers `/etc/collectd/collectd.conf.d/{filters,thresholds}.conf` sont ceux du paquet Debian
  (entièrement commentés) et ne sont pas copiés.
* La crontab de `beau` est `../crontab_current.txt` (éclairage piloté par `../light.sh`) : `crontab Server/crontab_current.txt`.
