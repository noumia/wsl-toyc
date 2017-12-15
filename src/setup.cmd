@rem setup.cmd
@echo off

if "%1" == "" goto help

mkdir "%1"
if ERRORLEVEL 1 goto err

if not exist rootfs.tar.gz (
  echo place rootfs.tar.gz file
  goto err
)

move rootfs.tar.gz "%1"
move create-toyc.exe "%1"

"%1\create-toyc.exe"
if ERRORLEVEL 1 goto err

ren launch-toyc.exe "%1.exe"

exit /b 0

:help
echo usage: setup.cmd distro_name 
:err
exit /b 1
