#!/bin/sh

CMD=pgrep
PPROC=ZeegineServer
PPROC1=P2PTunnelServer
while true
do
    if [ -n "`$CMD $PPROC`" ]; then  
        echo "ZeegineServer is ok"
    else
        echo "ZeegineServer is killed"
        sudo service ZeegineServer restart
    fi
    if [ -n "`$CMD $PPROC1`" ]; then  
        echo "P2PTunnelServer is ok"
    else
        echo "P2PTunnelServer is killed"
        /home/pi/zeegine/p2p/boot_from_start.sh
    fi
    sleep 5
done
