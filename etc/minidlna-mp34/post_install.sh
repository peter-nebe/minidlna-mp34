#!/bin/sh
NAME=minidlna
useradd --system --user-group $NAME

mkdir --parents   /var/cache/minidlna-mp34
chown $NAME:$NAME /var/cache/minidlna-mp34
mkdir --parents   /run/minidlna
chown $NAME:$NAME /run/minidlna

systemctl enable minidlna-mp34
systemctl start minidlna-mp34
