Jagged Alliance 2 Multiplayer w/ UI patch
=========================================

Credits
-------
JA2MP by:
Haydent
RoWa21

UI Patch by:
Zathras

About the UI patch
------------------
The UI patch is designed to improve the process of creating and joining games in JA2MP.
Instead of each player needing to manipulate thier .ini file, these options can now be set
from in-game hosting and joining screens. In Addition the process of finishing a game and restarting
has been streamlined.




How to Use
----------
1. Hosting the game
	a. Click "Multiplayer Game" on the main menu.
	b. Click "Host" on the Join screen.
	c. Configure Game Type and game options, click "Start".
	d. The game will switch to the map screen.
	e. Before the game is "Started" (laptop unlocked) :
			- you can change the starting sector by moving the dropzone in the airspace view
			- new players can connect to the game
			- players can change thier starting edge ( click the Button with the Compass ICON )
			- players can change thier team in the same manner ( click the Team Button )
	f. Host clicks "Start Game" to Unlock the laptop. At this point:
		  - the game settings are locked
		  - players can hire mercs (unless random mercs is selected in which case they are automatically hired for the players)
		  - players can click "Ready"
	g. When all players are ready, the game will commence.
	
2. Joining the Game
	a. Click "Multiplayer Game" on the main menu.
	b. Type in your player name.
	c. Type in the server ip address and port.
	d. Click "Join".
	e. The Game will switch to the map screen.
	f. While the game has not yet "Started" (laptop unlocked) :
			- players can change thier starting edge ( drop down by clicking IN the column next to thier name )
			- players can change thier team in the same manner ( IF TDM or CO-OP )
	g. Host clicks "Start Game" to Unlock the laptop. At this point:
		  - players can hire mercs (unless random mercs is selected in which case they are automatically hired for the players)
		  - players can click "Ready"
	h. When all players are ready, the game will commence.
	
3. Ending the Game
	When the battle is over in a sector, after a few seconds the Scoreboard will be displayed.
If you wish to play another game, press "Continue" to re-join (or re-host) the server. Otherwise
you can press "Cancel" to return to the main menu.

4. In-game chat
	Press the 'y' key to open ingame chat.

Changelog
---------
Build 20090319
	- Added hits, misses, accuracy to scoreboard
	- Added in-game chat with history
	- Added more options from the ini to the host screen, Use NIV and Enable civillians
	- Fixed Clients connecting at the same time controlling the wrong client
	- Fixed Team Deathmatch
	- Fixed Refreshing the player list after a player changes team / edge
	- AI is always on for Co-op now
	- There is now always a scoreboard in Co-op even when you lose.
	- scores on the Co-op scoreboard have been fixed for all deaths including bleeding
	- Start Game button now only appears for server
	- Fixed cleaning up game resources on disconnection
	- Disabled the old 1,2,3,4 keys for connecting / disconnecting
	- Fixed victory conditions for all game modes
	- Fixed players names getting copied into TeamTurnString each game resulting in multiple names
	- Fixed Assertion bug caused by the game sometimes calling SetCurrentWorldSector more than once before placement of mercs
	- Fixed a bug that was possible when merging items such as M14 and EBR stock on mapscreen.
	- Fixed bug with game starting while a player was in chat, closes chat window when game is starting
	- Fixed bug with game starting while a player was in the options screen, if the player leaves the mapscreen while ready, they are set to unready
	- Team and Spawn direction popups changed to use buttons to open/close them
	- Disabled players joining after the laptop has been unlocked, as they would be out of state with any hires

Build 20081224
	- Fixed players disconnecting mid-game
	- Added Host Options screen to allow in-game editing of the ini file options
	- Added Join screen for in-game entry of the server details
	- Added player list / server settings and ready / start button to the map screen
	- Added ability to change starting sector, starting edge and team once connected to the server,
		but before the laptop is unlocked.
	- Added ability to randomise starting edge and/or mercs of all players
	- Fix : Allow the game to "start" if less than the max number of players connected
	- Added Scoreboard at the end of a match to tidy up end of game and restarting. 


