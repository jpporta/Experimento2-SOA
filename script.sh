#!/bin/bash
make
mkdir RES
clear
for ((z=0;z<=50;z=z+5))
{
	# Execucao das cargas
	for ((x=0;x<z;x++))
	{
		./pi &
	}
	# Tempo de espera para carga exigir cpu
	sleep 1
	# 10x Execucao do programa para tirar tempo medio
	for ((vezes=1;vezes<=10;vezes++))
	{
		./Experimento2 >> "RES/RESc${z}.csv"
		sleep 1
		echo "Teste ${vezes} com ${z} cargas COMPLETO"
	}

	if [ "${z}" -gt 0 ]
	then
		killall pi
	fi
}
rm Experimento2
rm pi
exit 0

