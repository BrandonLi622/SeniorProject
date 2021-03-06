#!/bin/bash

rm ./servOutput
rm ./cliOutput
touch servOutput
touch cliOutput

vari=0
varj=0
#numServs=2
#numClis=4

function fun()
{
if [ "$#" -ne 3 ] ;
then echo 'Call as ./runProtocol.sh {numServers} {numClients} {slotLength}'; exit
fi

if [ "$1" -lt "1" ] ;
then echo 'Call as ./runProtocol.sh {numServers} {numClients} {slotLength}'; exit
fi

if [ "$2" -lt "1" ] ;
then echo 'Call as ./runProtocol.sh {numServers} {numClients} {slotLength}'; exit
fi

if [ "$3" -lt "1" ] ;
then echo 'Call as ./runProtocol.sh {numServers} {numClients} {slotLength}'; exit
fi

while [ "$vari" -lt "$1" ]
do
#Server-build-Desktop-Debug/Server $numServs $numClis &>> ./servOutput &
Server-build-Desktop-Debug/Server $1 $2 $3 &
vari=$(($vari+1))
done

while [ "$varj" -lt "$2" ]
do
Client-build-Desktop-Debug/Client $1 $2 $3 &
varj=$(($varj+1))
done
}

fun $1 $2 $3
