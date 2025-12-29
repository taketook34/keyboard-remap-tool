## Description

After cleaning my PC I found out that some keyboard buttons aren't working so I created a tool that fix this problem

## Build 
Build
```
make
```
Clean
```
make clean
```
Install
```
sudo make install
sudo make uninstall # for deleting
```

## Launch
Before launch you need to know dev file of your keyboard

You use evtest
```
sudo dnf install evtest -y
sudo evtest
```

After this you need to create special conf file. Example of .conf file
```
KEY_LEFT KEY_BACKSPACE
KEY_W KEY_Q
```
Where second argument is new role of key in first argument. 
To check right defines of key in 
```
/usr/include/linux/input-event-codes.h
```
Launch
```
sudo ./keyboard-remap-tool /dev/input/event4  file.conf
```
First inline parameter - dev file
Second inline parameter - file with configuration

## Launch as systemd service
Will be soon ...