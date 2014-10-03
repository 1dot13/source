      ______     ______     ______     ______     __   __     ______     ______                            
     /\  ___\   /\  ___\   /\  ___\   /\  ___\   /\ "-.\ \   /\  ___\   /\  ___\                           
     \ \  __\   \ \___  \  \ \___  \  \ \  __\   \ \ \-.  \  \ \ \____  \ \  __\                           
      \ \_____\  \/\_____\  \/\_____\  \ \_____\  \ \_\\"\_\  \ \_____\  \ \_____\                         
       \/_____/   \/_____/   \/_____/   \/_____/   \/_/ \/_/   \/_____/   \/_____/                         
  __  __   __     __   __   __   _____      ______   __  __   ______   __       __        
 /\_\_\_\ /\ \  _ \ \ /\ "-.\ \ /\  __-.   /\  ___\ /\ \_\ \ /\  ___\ /\ \     /\ \       
 \/_/\_\/_\ \ \/ ".\ \\ \ \-.  \\ \ \/\ \  \ \___  \\ \  __ \\ \  __\ \ \ \____\ \ \____  
   /\_\/\_\\ \__/".~\_\\ \_\\"\_\\ \____-   \/\_____\\ \_\ \_\\ \_____\\ \_____\\ \_____\ 
   \/_/\/_/ \/_/   \/_/ \/_/ \/_/ \/____/    \/_____/ \/_/\/_/ \/_____/ \/_____/ \/_____/ 
                                                                                                           

													2014 (c) StaticZ <staticz@uoquint.ru>
________________________________________________________________________________________


		CONTENT
		
		1.0 About ExShell
		2.0 Install and Running
		3.0 Requirements
		4.0 Perfomance
		5.0 Tested Games
		6.0 Change Log



-------------------
(1.0) About ExShell
-------------------

"Essence XWnd Shell" aka ExShell is hook based application for force running old 
games (or ant other full screen applications) in windowed mode with different 
graphic filters (for example upscalers like 2xSaI). ExShell also provide some other
abilities, like setting FPS limit or lowing CPU usage (see config for more details).



-------------------------
(2.0) Install and Running
-------------------------

For installing copy ExShell to any directory you whant and open exshell.ecd config
file in any text editor (recomended Notepad++, you can import to it syntax higlight
from *.xml file that are distributed with ExShell). To lunch your application you
have to set path to it and it's screen resolution. The application you whant to run 
must be set in fullscreen mode (ja2.ini: SCREEN_MODE_WINDOWED = 0), otherwise you 
get many problems and errors.
Check other options and see comments there for more details. 

Important exshell.ecd entries:
- Set $StartPEPath for profile [EXSProfile01] to the ja2.exe file (full path).
- Set $FScreenMode to the same resoultion like JA2 1.13 (see: ja2.ini -> SCREEN_RESOLUTION)
  -> Also set SCREEN_MODE_WINDOWED = 0 in ja2.ini
- Set $WindowFrame to the double size (if you use 2x upscaling filters) of your JA2 1.13 screen resolution
- Set $ImageFilter to your liking. For best image quality set the filter to #2022
  -> This means 2x upscaling the screen resolution and using xbra filter

Be carefully, while saving 
config file, check that you use UTF8 (without BOM) encoding. Otherwise ExShell will 
have problems with reading it.

To start the JA2 1.13 with the exShell tool, just run the exshell.exe

Additional informations can be found on the Bears Pit Forum:
http://www.ja-galaxy-forum.com/ubbthreads.php/topics/336350/1


------------------
(3.0) Requirements
------------------

ExShell supports only windows x86 applications, that are based on DirectDraw or GDI.

System requirements:
		
	Windows(R) XP/Vista/7
	DirectX 9.0
	Monitor, keybord, mouse, cheese, beer
	


----------------
(4.0) Perfomance
----------------

For compare speed of different filters, see table below. There values are average
number of processor ticks for proccesing each pixel. All values were got by filtering
screen 640x480 and dividing result for pixels count. As procesor ticks are very 
small values and exaclty value depends on many things, like current CPU loading
the result where get as avarage for several minutes.  
	 _______________________
	| Filter Type   | Speed |
	|---------------+-------|
	| None*			|	 <1	|
	| Simple		|	  5	|
	| Pixelate		|	 10	|
	| Scanlines		|	 10	|
	| ScanlinesTV	|	 17 |
	| AdMame		|	 17	|	
	| Super2xSaI	|	117	|
	| 2xSaI			|	121	|
	| HQ			|	680	|
	| LQ			|	685	|
	| XBR			|	696	|
	*---------------^-------*

	* According to exshell architecture "None filter" is real filter, that simply copy
	  screen from one memmory block to other. So infact it's speed more depends on RAM
	  speed then on CPU.  

As you can see ower winner is AdMame, it is very fast and give nice result, close to
smth similar for Super2xSaI.



------------------
(5.0) Tested Games
------------------

ExShell was tested for compatibility with this games:

	1. Jagged Alliance 2: Feeding Ground	[OK]
	2. Jagged Alliance 2: 1.13				[OK]
	3. Jagged Alliance 2: Metavira			[OK]



----------------
(6.0) Change Log
----------------


(2014.09.28) ver 0.01 rev 011  build 14271 
------------------------------------------

	* Intial realise
	
	
	
	