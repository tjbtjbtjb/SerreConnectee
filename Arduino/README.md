
Compilation
-----------
Avec une version >= 1.5.0, en ligne de commande : 

/usr/local/arduino-1.8.5/arduino --upload --port /dev/ttyACM1 --board arduino:avr:mega:cpu=atmega2560 Temperature.ino 

--verify

Reset USB
---------
* Rechercher le numéro du port pour le device USB ACM

root@phy41:~# dmesg | grep tty
[    0.000000] console [tty0] enabled
[    0.644413] 00:04: ttyS0 at I/O 0x3f8 (irq = 4, base_baud = 115200) is a 16550A
[    0.664801] 00:05: ttyS1 at I/O 0x2f8 (irq = 3, base_baud = 115200) is a 16550A
[    4.556333] systemd[1]: Starting system-getty.slice.
[    4.556459] systemd[1]: Created slice system-getty.slice.
[ 4186.085690] cdc_acm 3-2:1.0: ttyACM0: USB ACM device
[ 7561.206124] cdc_acm 3-2:1.0: ttyACM1: USB ACM device
[ 7650.038117] cdc_acm 3-2:1.0: ttyACM1: USB ACM device

 * Puis lancer les commandes : 

root@phy41:~# echo '3-2:1.0' | tee /sys/bus/usb/drivers/cdc_acm/unbind 
3-2:1.0

root@phy41:~# echo '3-2:1.0' | tee /sys/bus/usb/drivers/cdc_acm/bind 
3-2:1.0

 * Vérification dans les messages du système : 

root@phy41:~# !dmes
dmesg | grep tty
[    0.000000] console [tty0] enabled
[    0.644413] 00:04: ttyS0 at I/O 0x3f8 (irq = 4, base_baud = 115200) is a 16550A
[    0.664801] 00:05: ttyS1 at I/O 0x2f8 (irq = 3, base_baud = 115200) is a 16550A
[    4.556333] systemd[1]: Starting system-getty.slice.
[    4.556459] systemd[1]: Created slice system-getty.slice.
[ 4186.085690] cdc_acm 3-2:1.0: ttyACM0: USB ACM device
[ 7561.206124] cdc_acm 3-2:1.0: ttyACM1: USB ACM device
[ 7650.038117] cdc_acm 3-2:1.0: ttyACM1: USB ACM device
[339740.292745] cdc_acm 3-2:1.0: ttyACM0: USB ACM device

 * Ici par exemple, on note qu’on est passé de ttyACM1 à ttyACM0 comme nom de port.

 * Le tour est jouė

Configuration actuelle
----------------------

 * Avec carte fille Groove
 * A0/A1 : Module temp/humidity
 * A4/A5 : Module light
 * A8/A9 : CO2
 * A12/A13 : O2
 * Pas de screen I2C connecté

