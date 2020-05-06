#!/bin/sh
useradd --system --user-group minidlna

mkdir --parents /var/cache/minidlna-mp34
chown minidlna:minidlna /var/cache/minidlna-mp34
mkdir --parents /run/minidlna
chown minidlna:minidlna /run/minidlna

systemctl enable /usr/local/etc/systemd/system/minidlna-mp34.service
systemctl start minidlna-mp34
