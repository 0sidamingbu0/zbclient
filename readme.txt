sudo apt-get install libmicrohttpd-dev
sudo apt-get install libcurl4-gnutls-dev
sudo apt-get install libjson-c-dev

sudo cp curl/* /usr/include -r
sudo cp json/* /usr/include
sudo cp microhttpd/* /usr/include

gcc zbserver.c -o zbserver -lmicrohttpd -lwiringPi -lpthread
gcc zbclient.c -o zbclient -lwiringPi -lpthread -lcurl -ljson-c

//gcc zbclient.c -o zbclient -lwiringPi -lcurl -lpthread -ljson-c -lmicrohttpd
sudo cp json-c/* /usr/lib/

#build libmicrohttp.so
donwload src
cp to raspi
sudo apt-get install autoconf
sudo apt-get install litool
autoreconf -fi
./configure --disable-curl
sudo make
sudo make install
sudo cp /usr/local/lib/libmicrohttp.* /lib/arm-linux-gnueabihf/
regcc .c
ok

=====
sudo rm /usr/arm-linux-gnueabihf/libmicrohttpd.so
sudo cp libmicrohttpd.so.12.37.0 /usr/arm-linux-gnueabihf/
sudo ln -s libmicrohttpd.so.12.37.0 libmicrohttpd.so.12
sudo ln -s libmicrohttpd.so.12 libmicrohttpd.so

----
ls -l |grep libmicrohttpd
sudo find / -name libmicrohttpd*