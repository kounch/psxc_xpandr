# psxc_xpandr
A hack to expand and/or replace virtually games on PlayStation Classic using a external USB drive.

Features:
* Can keep or virtually replace the original installed games
* Can add new games (loaded from USB drive instead or internal memory)
* Minimal modification of PlayStation Classic internal storage
* Extracts automatically most of the needed files when used on a blank USB drive
* Adds support for access to the console menu using simultaneously the buttons `Select` and `Triangle` of a controller
* Optionally compatible to play with two controllers, using a USB hub
* Optional UI modifications support
* Optional cheats support via cheatpops.db


## Installation

Format a compatible USB drive using FAT32 and name it SONY.

Download the latest release file [from here](https://github.com/kounch/psc_xpandr/releases/latest). Unzip its contents and copy the folder `691843bb-62d6-4423-a105-19c06af91a8c` to the root of the drive.

Place your game files in numbered directories inside a `games` folder in the root of the drive, as explained [later in this text](#GameINI).

## Use

### First Use

On first use with a blank USB drive, the following folders and files are copied from the internal media to the USB drive
```
/
|
+-data
|  +-GR
|  |  +-Acid_B_BTN.png
|  |  (...)
|  |  +-X_Btn_ICN.png
|  |
|  +-system
|    +-20-joystick.rules
|    +-custom.pre
|    +-sonyapp-copylink
|
+-games
    +-custom.db
```

These can be customized to change the UI, add support for a second controller when using a USB hub, etc. Keep reading for more instructions.

### Customization

#### <a name="GameINI"></a>Add a new game by using BIN/CUE files and a `Game.ini` file
In the `games` dir create a new folder with a numerical identifier (`GAME_ID`), which has to be unique. Use a number between 1 and 20 if you desire to virtually replace an existing game. Use a number greater than 20 if you want to add virtually a new game. For new games, do not make gaps in the `GAME_ID` numeration (i.e use 21,22,23... and not 21,25...)

Inside of that directory you need to put at least those files:
* `BASENAME.cue`: Disc CUE file (normally, use as `BASENAME` string the original disc identifier)
* `BASENAME.bin`: Disc BIN (use the same `BASENAME` string)
* `BASENAME.png`: PNG image with less than 512x521 pixels size (use the same `BASENAME`)
* `pcsx.cfg`: PCSX configuration file for the game (a sample file is available to download [here](https://raw.githubusercontent.com/kounch/psxc_xpandr/master/pcsx.cfg))
* `Game.ini`: Game description file with the following format:

```
[Game]
Discs=...
Title=...
Publisher=...
Players=...
Year=...
```

Where the different fields must be filled as follows:

| Field     | Description                                                           |
|-----------|-----------------------------------------------------------------------|
| Discs     | Name (in order) of the BIN/CUE files of the game, separated by commas |
| Title     | Game Title                                                            |
| Publisher | Game Publisher                                                        |
| Players   | Number of players (1 or 2)                                            |
| Year      | Game release year                                                     |

#### Modify User Interface

Edit the files in data/GR directory as you wish. For example, to change the UI background, edit the `JP_US_BG.png` file.

#### Add support for second controller when using a USB hub

Edit the file `data/system/20-joystick.rules`, replacing its content with

    KERNEL=="js0",SUBSYSTEMS=="input",SYMLINK+="input/joystick0"
    KERNEL=="js1",SUBSYSTEMS=="input",SYMLINK+="input/joystick1"

#### Adding support for more than 25 games

Edit the file `data/system/sonyapp-copylink`, changing the number 25 in the second line (`COUNT_MAX=25`) for a greater number.

### Adding cheats

Copy or create a `cheatpops.db` file inside the directory `/games` of your USB drive.

#### Add a new game by editing manually `custom.db`
If you prefer to add by yourself the games to the database, do not create a `Game.ini` file, open `games/custom.db` in an SQLite Editor of your choice, and insert new entries to the GAME table as follows:

| Field                | Content description                    |
|----------------------|----------------------------------------|
| GAME_ID              | Numerical identifier, has to be unique |
| GAME_TITLE_STRING    | Name of the game                       |
| PUBLISHER_NAME       | Name of the Publisher                  |
| RELEASE_YEAR         | Year the game was released             |
| PLAYERS              | Number of players                      |
| RATING_IMAGE         | Must be `CERO_A`                       |
| GAME_MANUAL_QR_IMAGE | Must be `QR_Code_GM`                   |
| LINK_GAME_ID         | Must be empty                          |

Then, insert corresponding entries per each disc for each game to the DISC table:

| Field        | Content description                                   |
|--------------|-------------------------------------------------------|
|  GAME_ID     | GAME_ID from the GAME table                           |
|  DISC_NUMBER | Disc number                                           |
|  BASENAME    | Name of the disc cue/bin file pair, without extension |

Finally, you have to add actual game files. In the `games` dir create a new folder with the `GAME_ID` from previous steps.
Inside of that directory you need to put at least those files:
* `BASENAME.cue`: Disc CUE file (use `BASENAME` value from the `DISC` table)
* `BASENAME.bin`: Disc BIN (use `BASENAME` value from the `DISC` table)
* `BASENAME.png`: PNG image with less than 512x521 pixels size (use `BASENAME` value from the `DISC` table)
* `pcsx.cfg`: PCSX configuration file for the game (a sample file is available to download [here](https://raw.githubusercontent.com/kounch/psxc_xpandr/master/pcsx.cfg))

#### Replace an existing game changing `custom.db`
To replace an existing game, open `games/custom.db` in an SQLite Editor of your choice.

Modify the entries for the existing game in `GAME` and `DISC` table, keeping the format.

Then go to games dir and create a new folder with the GAME_ID, and add all the necessary files.

## Credits

Based on:
* lolhack, a payload launcher created by [madmonkey](https://github.com/madmonkey1907)
* The original [gpghax by justMaku](https://github.com/justMaku/gpghax)
* Access Esc Menu from Select + Triangle on controller by andshrew
* PCSX ReARMed, PCSX port for ARM by [Notaz](https://notaz.gp2x.de) (info about cheatpops.db)
* Reddit's rubixcube6 explanation on [how to make custom themes](https://redd.it/a5g5kx)
* Reddit's NonyaDB explanation to [overcome 25 games limit](https://www.reddit.com/r/PlaystationClassic/comments/a44ka6/add_custom_games_on_usb_storage_with_gpghax/ebci4hg/)
* Neeraj Kumar's [instructions for cross-compiling to ARM](https://neerajcodes.wordpress.com/2017/08/29/toolchain-cross-compilation-using-crosstool-ng/)
* Vicente Hernando's [instructions to cross-compile SQLite3]()

## FAQ

### Does this modify my console?

The script tries to keep to a minimum the number of files and directories modified in the internal storage of the console, mounting virtually most of the data from your USB drive. However, a minimal amount of data has to be created in order for it to work.

Files/directories that the script creates in the internal media:
* For any directory that you create in `/games`, if it doesn't exist, an empty directory with the same name is created in the internal media in `/gaadata`
* For any directory that you create in `/games`, if it doesn't exist, a directory with the same name is created in `/data/AppData/sony/pcsx/`.
* For any directory that you create in `/games`, inside the directory with the same name in `/data/AppData/sony/pcsx/`, a symbolic link `.pcsx/pcsx.cfg` is created for the `pcsx.cfg` in your USB drive.
* If you provide a 'cheatpops.db' file, an empty one with the same name is created inside the directory `/data/AppData/sony/pcsx`.

### Can it brick my console?

A lot of work has been made to try to make it as failsafe as possible, however, no warranties are made or responsabilities taken for any damage that you could make to your console by using it. Please, don't do if you if you are not sure of what you are doing, and try another option like, for example, [BleemSync](https://github.com/pathartl/BleemSync).

### How does it work?

The file `LUPDATA.bin` inside the folder `691843bb-62d6-4423-a105-19c06af91a8` is a shell script (source available [here](https://github.com/kounch/psxc_xpandr)), encrypted and signed so that the internal update system of an original PlayStation Classic copies it to a temporary folder an executes its contents.
Then, the script stops the main console menu, mounts virtual and temporal files and folders from the USB stick, reconfigures temporarily the system, editing if necessary the database copy using the included sqlite3 binary, and launchs again the main menu software.
This way, if the PlayStation Classic is powered off, and then started again without the USB drive, everything should work as if no modifications were made.

## Copyright

Copyright (c) 2018, kounch
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
