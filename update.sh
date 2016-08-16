#!/bin/sh
sudo supervisorctl stop zbclient
sudo cp zbclient.conf /etc/supervisor/conf.d/
sudo rm /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so
sudo rm /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so.12
sudo cp libmicrohttpd.so.12.37.0 /usr/lib/arm-linux-gnueabihf/
sudo ln -s /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so.12.37.0 /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so.12
sudo ln -s /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so.12 /usr/lib/arm-linux-gnueabihf/libmicrohttpd.so
chmod 777 join reset zbclient
sudo cp zbclient /usr/bin
sudo supervisorctl start zbclient
