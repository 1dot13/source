<p align="center">
  <img src="113.png" alt="Jagged Alliance 2 1.13">
</p>

# Jagged Alliance 2 v1.13 - Starter Documentation

> **THIS DOCUMENT IS A WORK IN PROGRESS.**

> This documentation is up to date as of 1.13 r8741.

This document is an attempt to create clear and structured starter documentation for people who want to start playing *Jagged Alliance 2 v1.13*, which will henceforth be referred to simply as *1.13*.

### Contents
- [0. Introduction](#0-introduction)
- [1. Installation](#1-installation)
- [2. Configuration](#2-configuration)
- [3. FAQs](#3-faqs)
- [4. Mods](#4-mods)
- [5. Glossary](#5-glossary)
- [6. Links](#6-links)
- [7. Community](#7-community)
- [8. Credits](#8-credits)
- [9. Contribute](#9-contribute)

### More information

- [Play Guide](play-guide.md)
- [Recommended settings](recommended-settings.md)
- [Development](development.md)

## 0. Introduction

### 0.1 Preamble
From years of hanging around in the [Bear's Pit community](http://thepit.ja-galaxy-forum.com/) we noticed there's a complete lack of explanation how to get started with this mod and everything surrounding it. Therefore, we are trying to create some concise and comprehensive documentation for use by both newcomers and veterans.

If you are completely new to Jagged Alliance 2, you can read [its manual](https://store.steampowered.com/manual/545210) which is still mostly relevant to 1.13. You can also find some manuals in `Docs\Manuals`, once you install 1.13. Or you can watch a let's play of 1.13 on YouTube such as [this excellent one by Moerges](https://www.youtube.com/playlist?list=PL1ewRmt6QNI5Wa7c4CQ-2UD4xMWzgmIdB).

### 0.2 What is *1.13*?
*Jagged Alliance 2 v1.13* is a modification for the *Jagged Alliance 2* game, with a host of new features and improvements, as well as externalizing a lot of data to make modding easier.

Lots of features have been added like new resolutions, improved AI, weather effects and new items. 1.13 does still contain the original maps and quests.

The 1.13 mod has been structured in such a way that it can be used as a baseline for creating new mods.

### 0.3 Features
Here are some of the high points:
- Custom resolutions (the original is locked at 640x480).
- Externalisation of hardcoded data and settings to XML and INI files, allowing for deep user customisation and modding.
- AI improvements (e.g. attacking from flanks and using cover)
- Suppressive fire mechanic
- Improved autofire: you can choose how many rounds to fire
- More detailed IMP (user-created mercenary) customisation, can create multiple IMPs
- Militia can be directly ordered on both the tactical and strategic map
- Weather effects (rain and thunder storms)
- New items
  - Literally hundreds of new guns
  - New armour such as ghillie suits
  - New ammo types, e.g. tracer, match, and cold (subsonic).
  - New LBE (Load Bearing Equipment, like vests, harnesses and holsters)
  - New weapon attachments ranging from optics to folding stocks to trigger groups.
- New Inventory System makes your mercs' inventories more granular and customisable
- New Weapon Attachment System allowing for up to nine attachment slots
- More new game options to fine-tune difficulty and/or playstyle
  - A new, very challenging difficulty level: INSANE.
- [Multiplayer](http://ja2v113.pbworks.com/w/page/4218359/Multiplayer)


[...and much, much more.](http://ja2v113.pbworks.com/w/page/4218338/Features)


## 1. Installation
To play 1.13 you will have to:

1. Acquire and install Jagged Alliance 2
2. Install the 1.13 mod
3. Apply fixes if running Windows 8+

### 1.1 Jagged Alliance 2
To be able to play 1.13 you will need to own the Jagged Alliance 2 game itself. There are several places where you can still get the game, for example [GOG](https://www.gog.com/game/jagged_alliance_2) and [Steam](https://store.steampowered.com/app/1620/Jagged_Alliance_2_Gold/).

After you got your hands on a copy of JA2 and want to go install it, we have a few tips for you to make sure everything runs smooth.

- Don't install the game to a default folder like `C:\Program Files` or `C:\Program Files (x86)`, instead make a folder like `C:\Games` (depending on your disk setup) and install the game there; Windows' UAC is known to interfere.
- After installing Jagged Alliance, make a backup of the folder so you dont have to do a reinstallation each time you want to start from scratch, so for example copy the `Jagged Alliance 2` folder in `C:\Games` to a backup copy, technically you can make endless copies of the JA2 folder for different 1.13 mod based installations, the naming of the folder doesn't really matter to be able to play 1.13.
- If you wonder how you will be able to use the Steam version of JA2, you can find it in the installation directory of Steam, usually in a place like `C:\Program Files\Steam\steamapps` you can make a copy of the Jagged Alliance 2 folder from there to a place like `C:\Games`.

### 1.2 The 1.13 mod
Once you have your clean JA2 installation ready to be turned into a clean 1.13 installation, there are a few routes you can go to achieve this.

- Install the latest SCI (Single Click Installer, something of a misnomer). This is the best way to go as these SCI packages are based upon the latest game developments but don't worry, they are very stable.
- Install the old so called "stable" release which is referred to as 7609. Some mods chose to stick to this version so check what your goal mod requires.
- Install 1.13 via the SVN route. This is an expert path because next to getting all the mod files from SVN you will have to compile the game's executable file from source code as well.

#### 1.2.1 Install the latest SCI 
This is the preferred way to go for people who want to start playing 1.13 as it will give you a good out of the box experience.

You can get the latest SCI file from [this OneDrive location](https://onedrive.live.com/?id=13A6926EAC52083!583&cid=013A6926EAC52083). If you sort the OneDrive file listing from new to old you should see the file with the highest revision number up on top: download this one.

Once you have downloaded the file you will need to extract its contents with a file archiver. There are different tools to use for this like [7-Zip](https://www.7-zip.org/ "7-Zip homepage"). Extract the file to the location where your target JA2 installation is located and choose to overwrite files if you get asked, so for this example we'll use `C:\Games\Jagged Alliance 2\`.

> **If you are not prompted to overwrite files, you did not extract to the correct directory.**

#### 1.2.2 Install release 7609
For the special cases with a requirement to run on release 7609 (namely mods), you can find it on the [official 1.13 wiki site](http://ja2v113.pbworks.com/w/page/4218334/Downloads).

#### 1.2.3 Install 1.13 from SVN
If you have a special reason to not use the SCI release, for example if you are actively helping to fix bugs in 1.13, it can be handy to get your gamedata and executable from source. This is however an advanced topic which is covered in more detail [here](development.md).

### 1.3 Fixes for newer Windows versions
Technically, there is no need to adjust anything to make 1.13 run well on Windows 7 and older. However there are a few things you will need to do to be able to play 1.13 well on the Windows 8+ versions, here's a list:

- Wine DLLs
- registry fix for 16-bit color mode
- running on one CPU thread

#### 1.3.1 Wine DLLs and registry fix
The Wine DLLs and registry fix can be found at `\Docs\Windows Compatibility Fixes\Windows 7-10 Fix.zip`. Make sure to read the included readme!

#### 1.3.2 Single CPU thread
To run the game on a single CPU thread, check out [these instructions](https://www.eightforums.com/threads/processor-affinity-set-for-applications-in-windows-8.24086/).

### 1.4 Launching the game
Run `ja2.exe`.


## 2. Configuration
> **Do not use the included INI editor. It is known to cause problems.** To edit INI and XML files, use a text editor such as as [Notepad++](https://notepad-plus-plus.org/).

> Wherever you see `\Data-1.13\...`, the `1.13` part should be replaced if playing a mod other than 1.13, e.g. `\Data-AIM\...` for AIMNAS.

Part of the beauty of the 1.13 mod is just how much control the player has to tailor the experience to their own tastes. This is done via a number of config files. The first place to look at is the INI files in the `\Data-1.13` directory (but not its subdirectories). You can check out [this page](recommended-settings.md) if you want some suggestions about configuring 1.13.

### 2.1 `ja2.ini`
Found in the root directory (i.e. aside `ja2.exe`). Manages global settings such as resolution and enabling windowed mode.
### 2.2 `JA2_Options.ini`
Found in the `\Data-1.13` directory. This is where you will find the bulk of the settings for 1.13.
### 2.3 `CTHConstants.ini`
Found in the `\Data-1.13` directory. This controls various aspects of the NCTH (new chance to hit) system.
### 2.4 XML Files
Found in the `\Data-1.13\TableData` directory. These files store all sorts of data, such as items, enemy drop tables, NPC inventories, mercenary data, etc. Some files of interest may include `\Items\Weapons.xml`, `DifficultySettings.xml` and `Vehicles.xml`.


## 3. FAQs

### 3.1 The game is running poorly on my Windows 8+ machine. How can I fix this? 
[Follow these instructions.](#13-fixes-for-newer-windows-versions)

### 3.2 The Drassen counterattack is too difficult!
In `JA2_Options.ini` you can set `TRIGGER_MASSIVE_ENEMY_COUNTERATTACK_AT_DRASSEN` to `FALSE` to disable it. <!-- Alternatively, git gud. [link to relevant section of playguide on defending Drassen] -->

### 3.3 My New Game settings don't have all the options I recall/have seen elsewhere. Where are they now?
In 1.13 r8610, some of the New Game settings were moved to `JA2_Options.ini`. Refer [here](play-guide.md#11-new-game-settings) and [here](http://thepit.ja-galaxy-forum.com/index.php?t=msg&th=23855) for more info.

### 3.4 What is the difference between the OCTH (Old Chance to Hit) and the NCTH (New Chance to Hit) system?
Put simply, NCTH aims to be more realistic by taking into account many more factors when computing the chance to hit. Some players prefer to use OCTH as it is generally more consistent and easier to understand. [This article](https://ja2v113ham.fandom.com/wiki/New_Chance_To_Hit) goes into great detail about how NCTH works.

### 3.5 How do I change the amount of cash I start with?
To change the amount of money you start with, open `\Data-1.13\TableData\DifficultySettings.xml` and edit the `<StartingCash>` value of the difficulty you want to play.

### 3.6 What if I just want to play the vanilla game at higher resolutions?
Take a look at [Jagged Alliance 2 Stracciatella](https://ja2-stracciatella.github.io/).

### 3.7 Can I play 1.13 on Linux or Mac OS X?
It can be played on Linux with [Wine](https://www.winehq.org/), and Mac OS X with [Wineskin](http://wineskin.urgesoftware.com/tiki-index.php).

Vanilla *Jagged Alliance 2* can be played on Mac and Linux with [Jagged Alliance 2 Stracciatella](https://ja2-stracciatella.github.io/).

### 3.8 My mercs are only moving backwards and/or certain hotkeys are not working!
When you Alt-Tab out of the game, sometimes the Alt key gets stuck. In-game, tap Alt a few times to fix it.


## 4. Mods
<!-- Consider moving this section to its own page -->
Whilst 1.13 is itself a mod, it also has its own mods! Each mod may require a specific version of 1.13 to work, often r7609 (aka "stable").

### 4.1 [AIMNAS](http://thepit.ja-galaxy-forum.com/index.php?t=msg&th=18815)
An item mod that adds over a thousand items, and supports *BigMaps*: a recreation of each sector to be much, much bigger. <!-- TODO: add image of BigMaps -->  
Currently requires at least r8163.  
[Download](https://github.com/aimnas/core)

### 4.2 [Urban Chaos 1.13](http://thepit.ja-galaxy-forum.com/index.php?t=tree&th=18716)
Integrates the *Urban Chaos* total conversion mod into the 1.13 platform.  
The latest release as of 15/05/2019, [Urban Chaos-1.13 Full Experimental 12 v4.6x 20190510](http://thepit.ja-galaxy-forum.com/index.php?t=msg&th=23934&goto=357359&#msg_357359), supports r8675.

### 4.3 [Vengeance 1.13 Reloaded](http://thepit.ja-galaxy-forum.com/index.php?t=msg&th=23256)
An unofficial sequel to JA2, including new maps and characters.  
This mod includes 1.13 so you do not need to worry about 1.13 version compatibility.  
[Download](http://thepit.ja-galaxy-forum.com/index.php?t=msg&th=23264)

### 4.4 [Arulco Revisited](http://thepit.ja-galaxy-forum.com/index.php?t=msg&th=19441)
Adds new sectors, changes or replaces every sector map in Arulco, adding new towns and SAM sites.  
[This release](http://kermi.pp.fi/JA_2/Mods_v1.13/Arulco_Revisited/v1.4/Stable_Release_2014/) supports r7609.

### 4.5 [Arulco Vacations](http://arulco.blogspot.com/)
A heavily tweaked version of 1.13.  
As of v1.11, Arulco Vacations is designed to be installed alongside r7609.

### 4.6 [7609+AI](http://thepit.ja-galaxy-forum.com/index.php?t=msg&goto=347454&#msg_347454)
An extensive rework of the game's AI.  
This is an alternate executable for 1.13 r7609 and is therefore only usable with r7609.  
[Download](https://drive.google.com/drive/folders/0B_PNaFvHBMdBYTh0NzBvc1c0Ym8)

### 4.7 Arulco Folding Stock (AFS)
*AFS* is a port of the features introduced in *Urban Chaos 1.13* and *Deidranna Lives 1.13* to regular 1.13. [List of features](http://thepit.ja-galaxy-forum.com/index.php?t=msg&th=21225&start=0&).  
The latest release as of 15/05/2019, [Arulco Folding Stock Full Experimental 14 v4.6x 20190510](http://thepit.ja-galaxy-forum.com/index.php?t=msg&th=23933&goto=357358&#msg_357358), supports r8675.


## 5. Glossary
- Arulco: the fictional nation where Jagged Alliance 2 takes place. [More info](https://jaggedalliance.fandom.com/wiki/Arulco)
- Bobby Ray's: an online gun shop available in Jagged Alliance 2. The site becomes available upon seizing Drassen airport. [More info](https://jaggedalliance.fandom.com/wiki/Bobby_Ray's_Guns_and_Things)
- IMP: a custom mercenary created by the player that has no upkeep cost. [More info](https://jaggedalliance.fandom.com/wiki/Institute_for_Mercenary_Profiling)
- Map screen / Strategic screen: the map screen displays the world map of Arulco in a square grid (called sectors) and the forces deployed by the enemy and the player
- Tactical screen: the tactical screen shows a sector from an isometric viewpoint. This is where you'll do your fighting.

## 6. Links
- [Item reference list by FurloSK](http://ja2.furlo.sk/)
- [The history of the Jagged Alliance series](https://ja2-stracciatella.github.io/history/)

## 7. Community
- [The Bear's Pit forums](http://thepit.ja-galaxy-forum.com/)
- [The Bear's Pit Discord](https://discord.gg/GqrVZUM)

## 8. Credits
- tais
- Yunotchi

## 9. Contribute
This documentation is far from complete. If it has made things easier for you to get started playing the 1.13 mod we hereby would like ask you to participate in extending the contents with anything you found missing.

[This documentation](https://github.com/firstcontributions/first-contributions/blob/master/README.md) should give you an insight on how you can participate via GitHub.
