#=====================================================
# Author:        Zeegine Team
# Copyright:             
# Last modified: 2015-02-06 15:24
# Filename:		 install.sh
# Description:   .../Zeegine-Server/
#                实现一键配置ZeegineServer
#=====================================================

#sudo apt-get install cmake
#sudo apt-get install libboost-all-dev
#支持mongoose网页服务调用php的cgi脚本
#sudo apt-get install php5-cgi

sudo cp -a ../build/ZeegineServer /usr/bin
sudo cp -a ./linux/ZeegineServer.conf /etc

sudo mkdir /var/lib/ZeegineServer 
sudo mkdir /var/lib/ZeegineServer/configs 
sudo mkdir /var/lib/ZeegineServer/www 
sudo mkdir /var/lib/ZeegineServer/storage 
sudo mkdir /var/lib/ZeegineServer/languages
sudo mkdir /var/lib/ZeegineServer/storage/Snaps
sudo mkdir /var/lib/ZeegineServer/storage/video

sudo cp -a ./www/* /var/lib/ZeegineServer/www
sudo cp -a ./languages/* /var/lib/ZeegineServer/languages
sudo cp -a ./linux/prusai3.cfg /var/lib/ZeegineServer/configs
sudo cp -a ./linux/ZeegineServer /etc/init.d/
sudo chmod 777 /etc/init.d/ZeegineServer
sudo chmod 777 /var/lib/ZeegineServer/storage/*
sudo update-rc.d ZeegineServer defaults
