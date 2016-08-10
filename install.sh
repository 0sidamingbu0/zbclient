#!/bin/sh
sudo rm /etc/supervisor/conf.d/*
sudo cp zbclient.conf /etc/supervisor/conf.d/
sudo rm /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so
sudo cp libmicrohttpd.so.12.37.0 /usr/lib/arm-linux-gnueabihf/
sudo ln -s /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so.12.37.0 /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so.12
sudo ln -s /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so.12 /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so
chmod 777 join reset zbclient
sudo cp zbclient /usr/bin
sudo cp pi3-miniuart-bt-overlay.dtb /boot/overlays/
sudo cp config.txt /boot/
sudo cp cmdline.txt /boot/
sudo systemctl disable hciuart
sudo cp hciuart.service /lib/systemd/system/
echo "done please reboot"
