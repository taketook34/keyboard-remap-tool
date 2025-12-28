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
(instructions will be later)
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

## Launch as systemd service
Will be soon ...