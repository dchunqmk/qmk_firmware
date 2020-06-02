@echo off

cd %~dp0

bash -c "cd ../../ && export NRFSDK15_ROOT=~/Documents/Keyboards/nrf52/nRF5_SDK_15.0.0_a53641a && make dchun/aurora/rev1" || exit

cd ..\..\.build

set file=aurora.hex

%~dp0/uf2conv.py %file% -c -f 0xADA52840

echo Waiting for the UF2 drive. Press Reset twice...

:loop
if not exist e:\ goto :loop

timeout 1

copy flash.uf2 e:\
