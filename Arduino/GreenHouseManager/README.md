GreenHouseManager
=================

Communication avec l'arduino au travers un protocole simple via le port série. Envoie de commandes et retour de l'arduino.
Les capteurs et actionneurs sont enregistrés dans la tâche de service de l'arduino via des chaînes de caractères. Ces chaînes
sont des identifiants <id> lors de l'envoi des commandes. 

Commandes acceptées
-------------------
* GET <id>  : Récupère une valeur du capteur <id>
* SET <id> <val> : Impose une valeur <val> à l'actionneur <id>
* RESET : Lance la procédure de softReset

Réponses de l'arduino
---------------------
* ACK <option> : L'arduino acquiesce après avoir reçu une commande. En cas de commande GET, il précise la valeur en jeu
* ERR : La commande est incomprise ou incohérente
