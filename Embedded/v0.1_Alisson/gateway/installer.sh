#!/bin/bash
if [ "$(id -u)" != "0" ]; then
echo
echo "Voce deve executar este script como root! "
else
mkdir /etc/smartbee /var/log/smartbee
cd files/
cp leituras.txt json.json smartbeed.py start_daemon.sh /etc/smartbee
cp start_daemon.sh /etc/init.d
insserv -dv /etc/init.d/start_daemon.sh
cd /etc/init.d/start_daemon.sh
fi
