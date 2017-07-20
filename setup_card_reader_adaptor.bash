#!/bin/bash

#build the program and copy it to proper folder
#gcc mult_adaptor.cpp -o card_reader_adaptor_bin -lwiringPi -lcurl


ps aux |grep /usr/bin/card_reader_adaptor_bin|awk '{print $2}'|xargs kill -9

PID_FILE=/var/run/supervisord.pid

PID=$(cat "${PID_FILE}")
kill -9 ${PID}
chown root:root card_reader_adaptor_bin
cp card_reader_adaptor_bin /usr/bin/
sudo chmod +x /usr/bin/card_reader_adaptor_bin

rm *.cpp
rm *.c
rm *.h
rm -rf .git

cp card_reader_adaptor.conf /etc/supervisor/conf.d/ 
/etc/init.d/supervisor restart 