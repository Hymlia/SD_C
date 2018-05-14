#!/bin/bash

make
./NoeudBloc 0x20000100 & #localhost 0x20000101 &
#./NoeudBloc 0x20000101 localhost 0x20000100 &
./NoeudParticipant localhost 0x20000100 1
