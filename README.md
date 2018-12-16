# psxc_xpandr
A hack to expand and/or replace virtually games on PlayStation Classic using a external USB drive.

Features:
* Can keep or virtually replace the original installed games
* Minimal modification of PlayStation Classic Storage
* Extracts automatically most of the needed files when used on a blank USB drive
* Optionally compatible to play with two controllers, using a USB hub
* Optional UI modifications support
* Optional cheats support via cheatpops.db


## Installation

Format a compatible USB drive using FAT32 and name it SONY.

Download the latest release file [from here](https://github.com/kounch/psc_xpandr/releases/latest). Unzip its contents and copy the folder `691843bb-62d6-4423-a105-19c06af91a8c` to the root of the drive.


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

These can be customized to add or replace virtually the games inside the PlayStation Classic.

### Customization

#### Add a new game
To add new games, open `games/custom.db` in an SQLite Editor of your choice, add new entries to the GAME table as follows:

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

Then, add corresponding entries per each disc for each game to the DISC table:

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

#### Replace an existing game
To replace an existing game, open `games/custom.db` in an SQLite Editor of your choice.

Modify the entry of the existing game in `GAME` and `DISC` table, keeping the format.

Then you have to add actual game files, go to games dir and create a new folder with the GAME_ID from previous steps.

#### Modify User Interface

Edit the games in data/GR directory as you wish. For example, to change the UI background, edit the `JP_US_BG.png` file.

#### Add support for second controller when using a USB hub

Edit the file `data/system/20-joystick.rules`, replacing its content with

    KERNEL=="js0",SUBSYSTEMS=="input",SYMLINK+="input/joystick0"
    KERNEL=="js1",SUBSYSTEMS=="input",SYMLINK+="input/joystick1"

#### Add support for more than 25 games

Edit the file `data/system/sonyapp-copylink`, changing the number 25 in the second line (`COUNT_MAX=25`) for a greater number.

### Add cheats

Copy or create a `cheatpops.db` file inside the directory `/games` of your USB drive.

### FAQ

#### Does this modify my console?

The script tries to keep to a minimum the number of files and directories modified in the internal storage of the console, mounting virtually most of the data from your USB drive. However, a minimal amount of data has to be created in order for it to work.

Files/directories that the script creates in the internal media:
* For any directory that you create in `/games`, if it doesn't exist, an empty directory with the same name is created in the internal media in `/gaadata`
* For any directory that you create in `/games`, if it doesn't exist, a directory with the same name is created in `/data/AppData/sony/pcsx/`, and the following empty directories are created inside:

      .pcsx/cfg
      .pcsx/cheats
      .pcsx/memcards
      .pcsx/patches
      .pcsx/plugins
      .pcsx/screenshots
      .pcsx/sstates

* For any directory that you create in `/games`, inside the directory with the same name in `/data/AppData/sony/pcsx/`, a symbolic link `.pcsx/pcsx.cfg` is created for the `pcsx.cfg` in your USB drive.

### Can it brick my console?

A lot of work has been made to try to make it as failsafe as possible, however, no warranties are made for any damage that you could make to your console by using it. Please, don't do if you if you are not sure of what you are doing.
