#!/bin/sh

# BSD 2-Clause License
# 
# Copyright (c) 2018, kounch
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# 
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#Functions

#Create, if needed, and bind all game data dirs
set_psxdirs() {
    SRC_DIR=$1
    DST_DIR=$2
    PCSX_DIRS="cfg cheats memcards patches plugins screenshots sstates"
    for i in ${PCSX_DIRS}; do
        mkdir -p "${SRC_DIR}/${i}"
        mkdir -p "${DST_DIR}/${i}"
        bndcp_psxdata "${SRC_DIR}/${i}" "${DST_DIR}/${i}"
    done
}

#Extract and bind, data files of snapshot
set_psxfiles() {
    SRC_DIR=$1
    DST_DIR=$2
    PCSX_FILES="filename.txt.res filename.txt.bak"
    for i in ${PCSX_FILES}; do
        bndcp_psxdata "${SRC_DIR}/${i}" "${DST_DIR}/${i}"
    done
}

#Bind file. If not found on source, extract it
bndcp_psxdata() {
    SRC_FILE=$1
    DST_FILE=$2
    if [ -e "${DST_FILE}" ]; then
		if [ ! -e "${SRC_FILE}" ]; then
			cp -r "${DST_FILE}" "${SRC_FILE}"
		fi
		mount -o bind "${SRC_FILE}" "${DST_FILE}"
	fi
}


#Main script

#Notify start
echo 0 > /sys/class/leds/green/brightness
echo 1 > /sys/class/leds/red/brightness

#Wait for system and menu finish starting up, needed to have working sound
sleep 8s

#Needed for Esc Menu
export PCSX_ESC_KEY=2

#Kill main menu, just in case anything was locked or in use
killall ui_menu

#Create, if needed, basic directory structure on USB drive
mkdir -p /media/games
mkdir -p /media/data/system

#Bind USB drive game and data dirs, if they exist
cd /media/games
for D in *; do
    echo 0 > /sys/class/leds/red/brightness
    if [ -d "${D}" ]; then
    	#Create dir in /gaadata, if needed
    	if [ ! -d "/gaadata/${D}" ]; then
    		mkdir -p "/gaadata/${D}"
    	fi
        #Bind usb drive dir into /gaadata
        bndcp_psxdata "/media/games/${D}" "/gaadata/${D}"
        #Bind usb drive data dirs into AppData
        set_psxdirs "/media/data/games/${D}" "/data/AppData/sony/pcsx/${D}/.pcsx"
        #Bind usb drive data files into AppData
        set_psxfiles "/media/data/games/${D}" "/data/AppData/sony/pcsx/${D}/.pcsx"
        #Link game config 
        ln -s "/gaadata/${D}/pcsx.cfg" "/data/AppData/sony/pcsx/${D}/.pcsx/pcsx.cfg"
    fi
    echo 1 > /sys/class/leds/red/brightness
done

echo 0 > /sys/class/leds/red/brightness

#Extract/Bind Preferences
bndcp_psxdata /media/data/system/custom.pre /data/AppData/sony/ui/user.pre
bndcp_psxdata /media/data/system/sonyapp-copylink /usr/sony/bin/sonyapp-copylink

#Extract/Bind Custom DB
bndcp_psxdata /media/games/custom.db /gaadata/databases/regional.db

#Extract/Bind Custom UI
bndcp_psxdata /media/data/GR /usr/sony/share/data/images/GR

#Copy Cheats DB, if found
if [ -e /media/games/cheatpops.db ]; then
	touch /data/AppData/sony/pcsx/cheatpops.db
    bndcp_psxdata /media/games/cheatpops.db /data/AppData/sony/pcsx/cheatpops.db
fi

#Set udev rule for controllers
bndcp_psxdata /media/data/system/20-joystick.rules /etc/udev/rules.d/20-joystick.rules
udevadm control --reload-rules
udevadm trigger

#Sync usb drive
echo 1 > /sys/class/leds/red/brightness
sync
echo 0 > /sys/class/leds/red/brightness

#Access Esc Menu from Select + Triangle
sleep 2s
cd /data/AppData/sony/pcsx
nohup /usr/sony/bin/ui_menu --power-off-enable &
sync

echo 1 > /sys/class/leds/green/brightness

# sleep forever so the usb is never unmounted and Esc Menu works
while :; do sleep 10; done
