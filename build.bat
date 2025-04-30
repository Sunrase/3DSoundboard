make
pause > nul
bannertool.exe makebanner -i banner.png -a audio.wav -o banner.bnr
bannertool.exe makesmdh -s "3DSoundBoard" -l "3DSoundBoard" -p "Sunrase" -i icon.png  -o icon.icn
makerom -f cia -o 3DSoundboard.cia -DAPP_ENCRYPTED=false -rsf Game.rsf -target t -exefslogo -elf 3DSoundboard.elf -icon icon.icn -banner banner.bnr
makerom -f cci -o 3DSoundboard.3ds -DAPP_ENCRYPTED=true -rsf Game.rsf -target t -exefslogo -elf 3DSoundboard.elf -icon icon.icn -banner banner.bnr
