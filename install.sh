#!/bin/sh
#build the program and copy it to proper folder
#gcc mult_adaptor.cpp -o card_reader_adaptor_bin -lwiringPi -lcurl
sudo make
sudo ps aux | grep card_reader_adaptor_bin | awk '{print $2}' | xargs kill -9

PID_FILE=/var/run/supervisord.pid

PID=$(cat "${PID_FILE}")
sudo kill -9 ${PID}
sudo chown root:root card_reader_adaptor_bin

sudo mkdir -p /opt/local/
sudo mkdir -p /opt/supervisor/conf.d/
sudo cp card_reader_adaptor_bin /opt/local/
sudo chmod +x /opt/local/card_reader_adaptor_bin

sudo rm *.cpp
sudo rm *.c
sudo rm *.h
sudo rm -rf .git

sudo rm /etc/supervisor/conf.d/card_reader_adaptor.conf
sudo cp card_reader_adaptor.conf /opt/supervisor/conf.d/
sudo ln -s /opt/supervisor/conf.d/card_reader_adaptor.conf /etc/supervisor/conf.d/card_reader_adaptor.conf
sudo /etc/init.d/supervisor restart
