Installation du serveur de monitoring
=====================================

Procédure pour reconstruire la machine `greenhouse` : un Raspberry Pi relié en USB aux cartes Arduino Mega,
qui collecte les mesures et les affiche dans Grafana. Elle reprend l'installation relevée le 2026-10-20 :
Debian 13 (trixie) arm64, collectd 5.12, InfluxDB 1.6, Grafana 13, Apache 2.4 + PHP 8.4.

Chaîne de données : Arduino (USB série) → `talkArduino_v4.py` → `dumpArduinoSensors_v4.sh` (plugin exec de
collectd) → InfluxDB (base `collectd`) → Grafana, servi par Apache sous `/grafana`.

Les fichiers de configuration cités ci-dessous sont dans `Server/config/` (voir son README).

1. Système et utilisateur
-------------------------

Les scripts utilisent des chemins absolus en `/home/beau/SerreConnectee` : créer l'utilisateur `beau`, ou
adapter ces chemins (`Server/*.sh`, `Server/config/collectd/collectd.conf`).

    sudo usermod -aG dialout beau          # accès aux ports /dev/ttyACM*
    sudo apt install git python3-serial python-is-python3
    git clone https://github.com/tjbtjbtjb/SerreConnectee ~/SerreConnectee

`python-is-python3` est nécessaire : `talkArduino_v4.py` commence par `#!/usr/bin/env python`.

2. Paquets
----------

    sudo apt install collectd influxdb influxdb-client rrdtool \
                     apache2 libapache2-mod-php certbot python3-certbot-apache

Grafana vient du dépôt apt de Grafana Labs :

    sudo install -d -m 755 /etc/apt/keyrings
    wget -qO- https://apt.grafana.com/gpg.key | gpg --dearmor | sudo tee /etc/apt/keyrings/grafana.gpg >/dev/null
    echo "deb [signed-by=/etc/apt/keyrings/grafana.gpg] https://apt.grafana.com stable main" | \
        sudo tee /etc/apt/sources.list.d/grafana.list
    sudo apt update && sudo apt install grafana

3. Cartes Arduino
-----------------

`listMega.sh` reconnaît les cartes Arduino Mega (USB `2341`/`2a03`, modèle `0042`) par leur numéro de série,
qu'il traduit en nom (`arduino7`…) grâce à `Server/arduino_db.txt`. Pour une nouvelle carte, relever son
numéro et l'ajouter dans ce fichier :

    udevadm info --query property --name /dev/ttyACM0 | grep ID_SERIAL_SHORT
    ~/SerreConnectee/Server/listMega.sh      # doit afficher /dev/ttyACM0:arduinoN

Ouvrir le port série peut redémarrer la carte. `talkArduino_v4.py` et `serialConsole.py` l'évitent,
`serialConsole_reboot.py` non.

4. collectd et InfluxDB
-----------------------

    sudo cp Server/config/collectd/collectd.conf /etc/collectd/collectd.conf
    sudo cp Server/config/influxdb/influxdb.conf /etc/influxdb/influxdb.conf
    sudo install -d /etc/influxdb/types
    sudo cp /usr/share/collectd/types.db /etc/influxdb/types/all_types.db
    sudo cp Server/collectd_config/greenhouse_types.db /etc/influxdb/types/
    sudo systemctl restart influxdb collectd

collectd lit `greenhouse_types.db` directement dans le dépôt. InfluxDB en lit une copie : la refaire après
chaque modification du fichier.

Vérifications :

    influx -database collectd -execute 'show tag values from sensors_value with key = "host"'
    ls /var/lib/collectd/rrd/            # fichiers RRD, utilisés par CGP (étape 7)

5. Grafana
----------

    sudo cp Server/config/grafana/grafana.ini /etc/grafana/grafana.ini
    sudo cp Server/config/grafana/provisioning/datasources/influxdb.yaml /etc/grafana/provisioning/datasources/
    sudo cp Server/config/grafana/provisioning/dashboards/serre.yaml /etc/grafana/provisioning/dashboards/
    sudo install -d -o grafana -g grafana /var/lib/grafana/dashboards/serre
    python3 Server/grafana/build_dashboard.py > /tmp/serre-connectee.json
    sudo install -o grafana -g grafana -m 644 /tmp/serre-connectee.json /var/lib/grafana/dashboards/serre/
    sudo systemctl enable --now grafana-server

Au premier lancement, se connecter en `admin` et changer immédiatement le mot de passe ; ne jamais le
noter dans le dépôt. L'ancien dashboard (`Server/config/grafana/dashboards/greenhouse-monitoring.json`)
s'importe à la main (Dashboards → New → Import).

Le dashboard public (lien de partage sans connexion) se crée depuis l'interface : Share → Share externally.
Il ne fonctionne qu'avec un dashboard sans variable de template.

6. Apache et HTTPS
------------------

    sudo a2enmod ssl rewrite proxy proxy_http
    sudo install -d /var/www/html/assets
    sudo cp Server/config/www/assets/*.svg /var/www/html/assets/

`Server/config/apache2/000-default.conf` utilise le certificat Let's Encrypt, qui n'existe pas encore sur
une machine neuve. Partir du vhost avec les lignes `SSLCertificateFile`/`SSLCertificateKeyFile` du
certificat auto-signé de Debian (`/etc/ssl/certs/ssl-cert-snakeoil.pem`, `/etc/ssl/private/ssl-cert-snakeoil.key`)
et sans la ligne `Include /etc/letsencrypt/...`, puis :

    sudo systemctl reload apache2
    sudo certbot --apache -d greenhouse.physique.univ-paris-diderot.fr --no-redirect
    sudo certbot renew --dry-run

Let's Encrypt doit joindre le port 80 depuis Internet. Le renouvellement est assuré par `certbot.timer`.

7. CGP (optionnel)
------------------

Collectd Graph Panel affiche les fichiers RRD de collectd sous `https://<serveur>/cgp/` :

    sudo git clone https://github.com/pommi/CGP /var/www/html/cgp

Sa configuration (`conf/config.php`) pointe sur `/var/lib/collectd/rrd`, valeur par défaut.

8. Tâches planifiées (optionnel)
--------------------------------

`Server/crontab_current.txt` active l'éclairage piloté par la luminosité (`Server/light.sh`, de 8h30 à 17h30).
Installation : `crontab Server/crontab_current.txt`. Le fichier contient aussi, en commentaire, les anciennes
tâches (chauffage, ventilation, arrosage) : corriger les chemins en `/home/beau/SerreConnectee/...` et
l'arduino visé avant de les réactiver.

Vérification finale
-------------------

    systemctl is-active collectd influxdb grafana-server apache2
    curl -s https://greenhouse.physique.univ-paris-diderot.fr/grafana/api/health
