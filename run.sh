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


#Main script
main() {
    XPANDR_PATH=/media/691843bb-62d6-4423-a105-19c06af91a8c/
    ORIG_DIR=/media/games
    ORIG_DB=${ORIG_DIR}/custom.db

    #Needed for Esc Menu
    export PCSX_ESC_KEY=2

    #Kill main menu, just in case anything was locked or in use
    killall ui_menu

    #Show splash screen
    if [ -e "${XPANDR_PATH}/splash.jpg" ]; then
        "${XPANDR_PATH}/display_xpandr" "${XPANDR_PATH}/splash.jpg" &
    fi

    #Create, if needed, basic directory structure on USB drive
    mkdir -p "${ORIG_DIR}"
    mkdir -p /media/data/games
    mkdir -p /media/data/system

    #Extract/Bind Custom DB
    bndcp_psxdata "${ORIG_DB}" /gaadata/databases/regional.db

    #Bind USB drive game and data dirs, if they exist
    cd "${ORIG_DIR}"
    for D in *; do
        echo 0 > /sys/class/leds/red/brightness
        if [ -d "${D}" ]; then
            #Create dir in /gaadata, if needed
            if [ ! -d "/gaadata/${D}" ]; then
                mkdir -p "/gaadata/${D}"
            fi
            #Bind usb drive dir into /gaadata
            bndcp_psxdata "${ORIG_DIR}/${D}" "/gaadata/${D}"
            #Create dir in /data/AppData/sony/pcsx/, if needed
            if [ ! -d "/data/AppData/sony/pcsx/${D}/.pcsx" ]; then
                mkdir -p "/data/AppData/sony/pcsx/${D}/.pcsx"
            fi
            #Bind usb drive dir into /data/AppData/sony/pcsx/
            bndcp_psxdata "/media/data/games/${D}" "/data/AppData/sony/pcsx/${D}/.pcsx"
            cp "${ORIG_DIR}/${D}/pcsx.cfg" "/media/data/games/${D}/pcsx.cfg"
            #Edit database, if INI file is found
            manage_db "${ORIG_DIR}/${D}/Game.ini" "${ORIG_DB}" "${D}"
        fi
        echo 1 > /sys/class/leds/red/brightness
    done

    echo 0 > /sys/class/leds/red/brightness

    #Bind again database file after possible editing
    umount /gaadata/databases/regional.db
    bndcp_psxdata "${ORIG_DB}" /gaadata/databases/regional.db

    #Extract/Bind Preferences
    bndcp_psxdata /media/data/system/custom.pre /data/AppData/sony/ui/user.pre
    bndcp_psxdata /media/data/system/sonyapp-copylink /usr/sony/bin/sonyapp-copylink

    #Extract/bind pcsx binary
    bndcp_psxdata /media/data/system/pcsx /usr/sony/bin/pcsx

    #Extract/Bind Custom UI
    bndcp_psxdata /media/data/GR /usr/sony/share/data/images/GR

    #Copy Cheats DB, if found
    if [ -e "${ORIG_DIR}/cheatpops.db" ]; then
        touch /data/AppData/sony/pcsx/cheatpops.db
        bndcp_psxdata "${ORIG_DIR}/cheatpops.db" /data/AppData/sony/pcsx/cheatpops.db
    fi

    #Set udev rule for controllers
    bndcp_psxdata /media/data/system/20-joystick.rules /etc/udev/rules.d/20-joystick.rules
    udevadm control --reload-rules
    udevadm trigger

    #Sync usb drive
    echo 1 > /sys/class/leds/red/brightness
    sync
    echo 0 > /sys/class/leds/red/brightness

    #Remove splash screen
    killall display_xpandr

    #Access Esc Menu from Select + Triangle
    sleep 2s
    cd /data/AppData/sony/pcsx
    nohup /usr/sony/bin/ui_menu --power-off-enable &
    sync
}

#Functions

#Bind file. If not found on source, copy it
bndcp_psxdata() {
    SRC_FILE=$1
    DST_FILE=$2
    if [ -e "${DST_FILE}" ]; then
        if [ ! -e "${SRC_FILE}" ]; then
            cp -r "${DST_FILE}" "${SRC_FILE}"
            sync
        fi
        mount -o bind "${SRC_FILE}" "${DST_FILE}"
    fi
}

#Alter or expand database with data from INI file
manage_db() {
    INI_FILE=$1
    DB_FILE=$2
    GAME_ID=$3
    SQLITE_BIN=/media/691843bb-62d6-4423-a105-19c06af91a8c/sqlite3
    TMP_DB=/tmp/tmp_xpandr.db

    if [ -f "${INI_FILE}" ]; then
        cp -f "${DB_FILE}" "${TMP_DB}"

        dos2unix "${INI_FILE}" >/dev/null 2>&1
        G_TITLE=`grep 'Title=' "${INI_FILE}" | awk -F'=' '{print $2}' | sed 's/'"'"'/'"''"'/g'`
        G_PUBLISHER=`grep 'Publisher=' "${INI_FILE}" | awk -F'=' '{print $2}' | sed 's/'"'"'/'"''"'/g'`
        G_YEAR=`grep 'Year=' "${INI_FILE}" | awk -F'=' '{print $2}'`
        G_PLAYERS=`grep 'Players=' "${INI_FILE}" | awk -F'=' '{print $2}'`
        G_DISCS=`grep 'Discs=' "${INI_FILE}" | awk -F'=' '{print $2}' | sed 's/,/ /g'`

        G_ROW=`${SQLITE_BIN} "${TMP_DB}" "SELECT * FROM GAME WHERE GAME_ID=${GAME_ID};"`
        if [ -z "${G_ROW}" ]; then
            Q_TXT="INSERT INTO GAME (GAME_ID,GAME_TITLE_STRING,PUBLISHER_NAME"
            Q_TXT="${Q_TXT},RELEASE_YEAR,PLAYERS,RATING_IMAGE,GAME_MANUAL_QR_IMAGE"
            Q_TXT="${Q_TXT}) VALUES (${GAME_ID},'${G_TITLE}','${G_PUBLISHER}',${G_YEAR}"
            Q_TXT="${Q_TXT},${G_PLAYERS},'CERO_A','QR_Code_GM');"
        else
            Q_TXT="UPDATE GAME SET GAME_ID=${GAME_ID},GAME_TITLE_STRING='${G_TITLE}'"
            Q_TXT="${Q_TXT},PUBLISHER_NAME='${G_PUBLISHER}',RELEASE_YEAR=${G_YEAR}"
            Q_TXT="${Q_TXT},PLAYERS=${G_PLAYERS}"
            Q_TXT="${Q_TXT},RATING_IMAGE='CERO_A',GAME_MANUAL_QR_IMAGE='QR_Code_GM'"
            Q_TXT="${Q_TXT} WHERE GAME_ID=${GAME_ID};"
        fi
        SQL_QUERY=`${SQLITE_BIN} "${TMP_DB}" "${Q_TXT}"`
        
        Q_TXT="DELETE FROM DISC WHERE GAME_ID=${GAME_ID};"
        SQL_QUERY=`${SQLITE_BIN} "${TMP_DB}" "${Q_TXT}"`
        
        declare -i DISC_ID=1
        for R in $G_DISCS; do
            Q_TXT="INSERT INTO DISC (GAME_ID,DISC_NUMBER,BASENAME)"
            Q_TXT="${Q_TXT} VALUES (${GAME_ID},${DISC_ID},'${R}');"
            SQL_QUERY=`${SQLITE_BIN} "${TMP_DB}" "${Q_TXT}"`
            DISC_ID=$DISC_ID+1
        done

        mv -f "${TMP_DB}" "${DB_FILE}"
        sync
    fi
}

#Notify start
echo 0 > /sys/class/leds/green/brightness
echo 1 > /sys/class/leds/red/brightness

#Wait for system and menu finish starting up, needed to have working sound
sleep 8s

main

#Notify end
echo 1 > /sys/class/leds/green/brightness

# sleep forever so the usb is never unmounted and Esc Menu works
while :; do sleep 10; done
