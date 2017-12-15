GCC=x86_64-w64-mingw32-gcc

.PHONY: all 7zip zip clean

all: create-toyc.exe launch-toyc.exe

create-toyc.exe: create-toyc.c
	$(GCC) -Os -o create-toyc.exe create-toyc.c -lshlwapi
	strip create-toyc.exe

launch-toyc.exe: launch-toyc.c
	$(GCC) -nostdlib -Os -o launch-toyc.exe launch-toyc.c -lkernel32
	strip launch-toyc.exe

7zip: create-toyc.exe launch-toyc.exe setup.cmd
	rm -f wsl-toyc.zip
	7za a wsl-toyc.zip create-toyc.exe launch-toyc.exe setup.cmd

zip: create-toyc.exe launch-toyc.exe setup.cmd
	rm -f wsl-toyc.zip
	todos setup.cmd
	zip wsl-toyc.zip create-toyc.exe launch-toyc.exe setup.cmd

clean:
	rm -f *.exe *.zip

