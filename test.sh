#!/bin/bash
trap "exit" INT TERM ERR
trap "kill 0" EXIT

make
./NoeudBloc 0x20000100 localhost 0x20000130 &
./NoeudBloc 0x20000130 localhost 0x20000100 &
./NoeudParticipant localhost 0x20000100 1 &

wait
