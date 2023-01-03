Console v1.5 Sep 2005
=====================
for Windows NT/2000/XP

Copyright (C) 2001-2005  Marko Bozikovic

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Send bug reports, bug fixes, enhancements, requests, flames, etc., and
I'll try to keep a version up to date.  I can be reached as follows:
   bozho@kset.org

Console icon is Copyright (C) 2001-2005 by Ante Vukorepa (orcinus@kset.org)


1. Requirements
===============
WinNT 4.0 SP4 or later (it won't work on Win9x/Me)
IE 4.0 (or later) installed.


2. File list
============

console.exe
FreeImage.dll
FreeImagePlus.dll
console.inf
console.xml
console_big.xml
console_small.xml
devel-changes.txt
Readme.txt
License.txt
Tab Completition.reg
FixedMedium*.fon
TerminalMedium14.fon


3. Installation
===============

Unzip Console.zip file somewhere :-)

If you'd like to have "Console here" in the Explorer right-click menu for
drives and directories, install console.inf file (right-click on console.inf
and choose "Install") Remember to do it in the Console directory!


4. Uninstall
============

If you have installed console.inf (see "Installation" section), run
"Add/Remove Programs" Control Panel applet and remove "Console Here
Extension".

Delete all the files.


5. Overview
===========

Console is a Win console window enhancement. It was inspired by eConsole
(http://www.informatik.uni-frankfurt.de/~corion), a great console-enhancement
program (unfortunately, it seems that it's no longer being developed)

Console features include configurable font, color, size, background image and
transparency (on Win2000 and later)

Console is *not* a shell. It's simply a nice-looking front end for a shell
of your choice (cmd.exe, 4NT, ksh, Cygwin bash, etc.)


6. Acknowledgements
===================

A big thanks to Ante Vukorepa for the program icon.
Also, big thanks to Alen Ladavac for help with painting optimizations.

Many thanks to code contributors:
- Francois Leblanc for mouse wheel messages patch
- Daniel Soto for adding multiple monitors support
- Dean Scarff for adding <inverse_shift> stuff
- Adam Hodson for adding flat scrollbar support
- Daniel Cheng for making Console window looking like a regular window when
  <taskbar_button> is set to 'tray' or 'hide'
- Rob Davenport for config reload prompt patch and initial comments in the
  sample console.xml file

Thanks also goes to all the people using and testing Console, reporting bugs
and asking for new stuff :-)


7. Included fonts
=================

The fonts included with Console are XFree86 'misc' fonts converted to TTF. I
added them here, since Windows could use nicer console fonts. If you wish to
use them, simply install them as you would any other TTF font (go to Control
Panel -> Fonts and follow instructions :-)

I guess these fonts are distributed under XFree86 licence, and if you think
I'm in violation of any licence terms by providing these fonts, please drop me
a line.


8. FreeImage library
====================

Console uses FreeImage library for background image loading and manipulations.
Console source distribution includes only FreeImage library, include and DLL
files. Complete source code for FreeImage can be obtained at:
http://freeimage.sourceforge.net


9. Configuration & usage
========================

9.1 Running Console
-------------------

The first thing you might want to set is your COMSPEC environment variable,
since that is the shell Console will start. Set it to your favorite shell
(cmd.exe, 4nt.exe)

Console command line syntax:
console.exe [<config_file>] [OPTIONS]

<config_file> is the path to the configuration file. If ommited, Console will
try to locate console.xml configuration file in console.exe directory. If the
path contains blanks, enclose it in double quotes.

Options:

-c <initial shell command>    - sets initial shell command
-t <window title>             - sets default window title
-r <config auto reload>       - sets default new configuration auto-load
                                flag

You can pass initial shell command by using "-c" switch. If the command string
contains blanks, enclose it in double quotes. If the command string itself
contains double quotes, put two double quotes for each double quote appearing
in the command string. E.g:
  console.exe console.xml -c "cd ""C:\Program Files"""

Note: If you use cmd.exe as your shell, you must use /k switch in your command
line string (4NT doesn't need the /k switch):
  console.exe console.xml -c "/k <command string>"

-t switch specifies the default Console window title. Using the 'title'
attribute in the config file will override this switch.

-r switch specifies the default new configuration auto-reload flag. Possible
values are: 'yes', 'no' and 'prompt'. 'prompt' is the default. Using
'reload_new_config' tag in the config file will override this switch.

9.2 Copy/Paste
--------------

To copy text, press and hold shift+left click (or simply left click if Console
is not mouse draggable) Drag to select text, and then left-click inside the
selection to copy. Left-click outside the selection clears it. If you set
copy_on_select tag to true, the selection will be copied immediately after
releasing the mouse button.

You can paste text into Console using shift+right click or middle click (you
might want to disable middle-button click support for Console in your mouse
drivers for better performance)

Note: text selection and copy work for fixed-width fonts *only*. Use Lucida
Console or the supplied fonts (if you have any nice looking free fixed width
fonts, feel free to share them :-)

9.3 International characters
----------------------------

Console fully supports Unicode characters, so it should be able to handle all
national characters, but there are some things to note:

The font you use in Console must include international characters (this one is
obvious, isn't it? :-) Supplied FixedMediumXX fonts don't include all of them.
I have found some FixedMedium Unicode fonts, but I wasn't able to convert them
from bdf to ttf format yet. Your best bet is using Lucida Console font
supplied with Windows.

After that, you have to configure the Windows console. Start Console and
unhide Win console. By default, the Win console uses the Raster Font that
doesn't have international characters. Open Win console properties and on the
Fonts tab change the font to Lucida Console. Click OK and on the next small
dialog select "Save properties for future windows with the same title" This
will make Win console windows started by Console use Lucida Console font that
has international characters.

9.4 Controlling Console from other applications
-----------------------------------------------

Console can be controlled from other applications that support sending
messages to application windows (e.g. PowerPro)

Console can handle messages for its menu commands sent from other
applications. The other application should send WM_COMMAND message (0x0111 in
hex, or 273 decimal) to the main Console window, with wParam set to command
ID, and lParam set to 0.

Command IDs (decimal values) are:

Show Readme.txt file...............40001
Show About dialog..................40002
Copy selected text.................40003
Paste from clipboard...............40004
Toggle 'always on top' status......40005
Toggle show/hide Windows console...40006
Edit configuration file............40008
Reload settings....................40009
Exit Console.......................40010

For selecting a configuration file, wParam should be between 1000 and 2000
(1000 being the first .XML file in the startup config file direcotry, files
sorted by filename, ascending)

9.5 Maximizing Console window
-----------------------------

Console window can be maximized by setting 'rows' and/or 'columns' of the
<size> tag to 'max'.

Maximum number of rows and/or columns is determined by both maximum Windows'
console dimensions and maximum Console window size. The smaller will be used
(since Windows console cannot be resized beyond desktop size)

If the size of your Console window is limited by Windows console maximum size,
you can choose a smaller font in Windows console properties.

Note that increasing the number of rows and/or columns and reloading the
settings can sometimes result in a corrupt output. I haven't found a solution
for this yet, but it seems that restarting Console solves the problem.


9.6 Misc stuff
--------------

Using left button double click you can toggle between z-order selected in the
configuration file and "always on top" z-order.

Right-click pops up a menu. You can copy/paste text from the menu, toggle
'always on top' and 'hide console' flags, work with configuration files. You
can switch configuration files by selecting a file from 'Select configuration
file' submenu (currently active configuration file is checked) and reloading
configuration. Configuration files are enumerated from the initial
configuration file's directory.

You can drag-n-drop a file or directory to Console. If several files are
dropped at once, they will be sent to Console, separated by spaces. Filenames
that contain spaces will be enclosed in double quotes.

The supplied .reg file is a registry file that will turn on file and directory
name completion for cmd.exe (thanks to Rob 'insanitee' for the idea) To
install it, just double-click on it, and click 'Yes'.

You can map console font colors to custom values (see configuration section)
Check out supplied console.xml for an example with default values.

Color indexes for default colors are:
00 - Black      08 - Gray (bright black)
01 - Blue       09 - Bright blue
02 - Green      10 - Bright green
03 - Cyan       11 - Bright cyan
04 - Red        12 - Bright red
05 - Magenta    13 - Bright magenta
06 - Yellow     14 - Bright yellow
07 - White      15 - Bright white


10. Configuration file
======================

The console configuration file is a simple xml file. Here's a general layout
of the configuration file:

<?xml version="1.0"?>
<console title="Da console" editor="metapad.exe" editor_params="/e %f">

   <font>
      <size>8</size>
      <color r="255" g="255" b="255" />
      <bold>false</bold>
      <italic>false</italic>
      <name>Lucida Console</name>
   </font>

   <position>
      <X>150</X>
      <Y>150</Y>
      <docked>bottom left</docked>
      <snap_distance>10</snap_distance>
      <z_order>regular</z_order>
   </position>

   <appearance>

      <hide_console>true</hide_console>

      <scrollbar>
         <style>flat</style>
         <color r="192" g="192" b="192" />
         <width>10</width>
         <button_height>10</button_height>
         <thumb_height>120</thumb_height>
      </scrollbar>

      <border>false</border>

      <inside_border>1</inside_border>

      <taskbar_button>normal</taskbar_button>

      <size rows="10" columns="80" />

      <transparency alpha="150">alpha</transparency>

      <background>
         <color r="10" g="30" b="30" />
         <tint  r="0" g="30" b="30" opacity="50" />
         <image style="resize" relative="true" extend="false">2.bmp</image>
      </background>

      <cursor>
         <color r="255" g="255" b="255" />
         <style>horizontal line</style>
      </cursor>

   </appearance>

   <behaviour>

      <mouse_drag>true</mouse_drag>

      <copy_on_select>true</copy_on_select>

      <inverse_shift>true</inverse_shift>

      <reload_new_config>prompt</reload_new_config>

   </behaviour>

</console>

If you ommit tags and/or attributes, default values will be used.

Here are the tags and attributes:

console
-------
Description: root tag
Parent:      none
Attributes:  - title -> sets the console window title (default: console)
             - change_refresh -> change refresh interval (in ms) for window
               repaint (default: 50) It controls how long window repainting is
               delayed after a change has been detected in the real console.
               The minimum value for this setting is 5. Note that small values
               will increase CPU usage when console contents changes
               rapidly (like dumping a large text file on the console)
             - refresh -> sets main refresh interval (in ms) for window
               repaint (default: 500) Console window is periodically refresh
               even if there are no changes detected. This is needed for
               proper operation of some DOS programs (eg, RAR). You can switch
               this off (by setting it to 0) to reduce CPU usage, but some DOS
               programs will not be displayed correctly (if at all :-)
             - shell -> sets shell program, overriding COMSPEC environment
               variable (if you change this while Console is running, you'll
               have to restart it for chages to take effect)
             - editor -> specifies text editor that will be used to edit the
               configuration file (notepad.exe by default); if the program is
               not in your PATH, you'll need to specify the full path
             - editor_params -> specifies additional parameters to be passed
               to a text editor used to edit configuration file and view the
               Readme file; %f in editor_params will be replaced by the
               configuration/readme file name; if there's no %f in the
               editor_params, the configuration/readme file name will be
               concatenated to the editor_params string; if there are spaces
               in the filename, place &quot; around %f (&quot;%f&quot;)
Data:        none

Note on 'change_refresh' and 'refresh' attributes: Console v1.4 introduced
some window painting improvements, increasing the speed and reducing CPU
usage. You might want to set these parameters much lower for better
performance (something like 5-10 for 'change_refresh' and 100-200 for
'refresh')

    <font>
    ------
    Description: Sets the font and its properties
    Parent:      console
    Attributes:  none
    Data:        none

        <size>
        ------
        Description: sets font size in points
        Parent:      font
        Attributes:  none
        Data:        font size in points (default: 8)

        <bold>
        ------
        Description: sets font's bold flag
        Parent:      font
        Attributes:  none
        Data:        true/false (default: false)

        <italic>
        --------
        Description: sets font's italic flag
        Parent:      font
        Attributes:  none
        Data:        true/false (default: false)

        <color>
        -------
        Description: sets font color
        Parent:      font
        Attributes:  - r -> 0-255, sets Red component (default: 0)
                     - g -> 0-255, sets Green component (default: 0)
                     - b -> 0-255, sets Blue component (default: 0)
        Data:        none
        Note:        if this tag is not present, Console will use font colors
                     from the real console (useful if your shell, like 4NT,
                     supports different coloring options)

        <colors>
        --------
        Description: groups font color mapping tags
        Parent:      font
        Attributes:  none
        Data:        none

            <color_XX>
            ----------
            Description: maps color No. XX to a new color
            Parent:      colors
            Attributes:  - r -> 0-255, sets Red component (default: 0)
                         - g -> 0-255, sets Green component (default: 0)
                         - b -> 0-255, sets Blue component (default: 0)
            Data:        none
            Note:        XX goes from 00 to 15 (you may ommit colors you don't
                         want to change) See 'Misc Stuff' section above for
                         color indexes

        <name>
        ------
        Description: sets font's name
        Parent:      font
        Attributes:  none
        Data:        font name (default: Lucida Console)


    <position>
    ----------
    Description: groups window position tags
    Parent:      console
    Attributes:  none
    Data:        none

        <x>
        ---
        Description: sets initial window X coordinate
        Parent:      position
        Attributes:  none
        Data:        X coordinate
        Note:        Set to -1 to let the system place the Console window at
                     startup

        <y>
        ---
        Description: sets initial window Y coordinate
        Parent:      position
        Attributes:  none
        Data:        Y coordinate
        Note:        Set to -1 to let the system place the Console window at
                     startup

        <docked>
        --------
        Description: sets initial window docking position
        Parent:      position
        Attributes:  none
        Data:        docked position: none, bottom left, bottom right, top
                     left, top right (default: none)

        <snap_distance>
        ---------------
        Description: sets window snap distance
        Parent:      position
        Attributes:  none
        Data:        snap distance in pixels; use -1 for no snapping (default:
                     10)

        <z_order>
        ---------
        Description: sets window z ordering
        Parent:      position
        Attributes:  none
        Data:        window z-order: regular, on top, on bottom (default:
                     regular)


    <appearance>
    ------------
    Description: groups window appearance tags
    Parent:      console
    Attributes:  none
    Data:        none

        <hide_console>
        --------------
        Description: sets real console window visibility
        Parent:      appearance
        Attributes:  none
        Data:        true/false (default: true)

        <hide_console_timeout>
        ----------------------
        Description: sets timeout for initial console hiding (during startup);
                     some shells need real console window visible during
                     startup in order to initialize properly
        Parent:      appearance
        Attributes:  none
        Data:        timeout in milliseconds (default: 0)

        <start_minimized>
        -----------------
        Description: sets initial 'minimized' status
        Parent:      appearance
        Attributes:  none
        Data:        true/false (default: true)

        <scrollbar>
        ----------------
        Description: groups scrollbar attributes (if visible)
        Parent:      appearance
        Attributes:  none
        Data:        none

            <color>
            -------
            Description: sets scrollbar background color
            Parent:      scrollbar
            Attributes:  - r -> 0-255, sets Red component (default: system
                           default)
                         - g -> 0-255, sets Green component (default: system
                           default)
                         - b -> 0-255, sets Blue component (default: system
                           default)
            Data:        none

            <style>
            -------
            Description: sets scrollbar style
            Parent:      scrollbar
            Attributes:  none
            Data:        scrollbar style: regular, flat, encarta (default:
                         regular)
            Note:        Other attributes will not be applied for regular
                         scrollbars. This is a limitation I have introduced
                         due to strange behaviour (bad redraws) I got on Win2k
                         SP4 with Windowblinds 4.x

            <width>
            -------
            Description: sets scrollbar width
            Parent:      scrollbar
            Attributes:  none
            Data:        scrollbar width in pixels (default: system default)

            <button_height>
            ---------------
            Description: sets scrollbar arrow button height
            Parent:      scrollbar
            Attributes:  none
            Data:        scrollbar arrow button height in pixels (default:
                         system default)

            <thumb_height>
            --------------
            Description: sets scrollbar thumb height
            Parent:      scrollbar
            Attributes:  none
            Data:        scrollbar thumb height in pixels (default: system
                         default)

        <border>
        --------
        Description: sets visibility of window border
        Parent:      appearance
        Attributes:  none
        Data:        none/regular/thin (default: none)

        <inside_border>
        ---------------
        Description: sets client area border in pixels (text is drawn a bit
                     further
                     from the window edge for a more 'relaxed' look)
        Parent:      appearance
        Attributes:  none
        Data:        inside border in pixels (default: 0)

        <taskbar_button>
        ----------------
        Description: controls Console taskbar button appearance
        Parent:      appearance
        Attributes:  none
        Data:        normal, hide, tray (default: normal)
        Note:        if you put Console icon in the sys tray, left-clicking on
                     it will bring the Console window to the foreground,
                     right-clicking will bring up the popup menu and left
                     double click will hide/show the Console window

        <icon>
        ------
        Description: sets custom program icon
        Parent:      appearance
        Attributes:  none
        Data:        icon filename (full path)

        <size>
        ------
        Description: sets number of console rows and columns
        Parent:      appearance
        Attributes:  - rows -> number of console rows or 'max' (default: 25)
                     - columns -> number of console columns or 'max' (default:
                       80)
                     - buffer_rows -> size of the text buffer (default: not
                       used)
        Data:        none

        <transparency>
        --------------
        Description: sets window transparency
        Parent:      appearance
        Attributes:  - alpha -> sets the level of transparency when alpha
                       transparency is used (0-255)
                     - inactive_alpha -> if > 0 sets the transparency of the
                       inactive window (useful for making Console window more
                       transparent while in background)
        Data:        transparency type: none, alpha, color key, fake (default:
                     none) - alpha and color key transparencies are available
                     on Win2000 and later only

        <background>
        ------------
        Description: groups background tags
        Parent:      appearance
        Attributes:  none
        Data:        none

            <color>
            -------
            Description: sets background color (this color is used as color
                         key if 'color key' transparency is used; set it to
                         your real console background if you use background
                         image)
            Parent:      background
            Attributes:  - r -> 0-255, sets Red component (default: 0)
                         - g -> 0-255, sets Green component (default: 0)
                         - b -> 0-255, sets Blue component (default: 0)
            Data:        none

            <tint>
            ------
            Description: sets tinting color and opacity
            Parent:      background
            Attributes:  - r -> 0-255, sets Red component (default: 0)
                         - g -> 0-255, sets Green component (default: 0)
                         - b -> 0-255, sets Blue component (default: 0)
                         - opacity -> 0-100, sets tinting opacity, in %
                           (default: 50)
            Data:        none
            Note:        Tinting can be applied only to background images and
                         fake transparency.

            <image>
            -------
            Description: sets background image
            Parent:      background
            Attributes:  - style -> resize/center/tile (default: resize)
                         - relative -> true/false (default: false)
                         - extend -> true/false (default: false); if set to
                           true, the background image will be stretched to the
                           entire virtual display covered by multiple monitors
            Data:        image filename (complete path)


        <cursor>
        --------
        Description: goups cursor settings
        Parent:      appearance
        Attributes:  none
        Data:        none

            <color>
            -------
            Description: sets cursor color
            Parent:      cursor
            Attributes:  - r -> 0-255, sets Red component (default: 0)
                         - g -> 0-255, sets Green component (default: 0)
                         - b -> 0-255, sets Blue component (default: 0)
            Data:        none

            <style>
            -------
            Description: sets cursor style
            Parent:      cursor
            Attributes:  none
            Data:        cursor style: none, XTerm, block, noblink block,
                         pulse block, bar, console, noblink line, horizontal
                         line, vertical line, rect, noblink rect, pulse rect,
                         fading block (default: console)

    <behavior>/<behaviour>
    ----------------------
    Description: groups behavior tags
    Parent:      console
    Attributes:  none
    Data:        none

        <mouse_drag>
        ------------
        Description: sets 'mouse draggability' of the window
        Parent:      behavior
        Attributes:  none
        Data:        true/false (default: true)

        <copy_on_select>
        ----------------
        Description: sets 'copy-on-select' flag
        Parent:      behavior
        Attributes:  none
        Data:        true/false (default: false)

        <inverse_shift>
        ---------------
        Description: inverts the use of shift for selecting/dragging (when
                     mose drag is enabled)
        Parent:      behavior
        Attributes:  none
        Data:        true/false (default: false)

        <reload_new_config>
        -------------------
        Description: controls new configuration auto-loading; if set,
                     overrides '-r' command-line option; otherwise, '-r'
                     option value is used (default: prompt)
        Parent:      behavior
        Attributes:  none
        Data:        yes/no/prompt (default: prompt)

        <disable_menu>
        --------------
        Description: disables Console popup menu for the main window and the
                     tray icon; the system menu is still available
        Parent:      behavior
        Attributes:  none
        Data:        true/false (default: false)

Have fun!


11. Known bugs and limitations
==============================

Console will NOT run under Win9x/Me. It runs on WinNT, Win2000 and WinXP.

If you use scrollable text buffer, and then resize the console by executing
something like "mode con lines=50", you will lose the text buffer. This is a
tradeoff, so that I could make programs like DOS RAR nad ncftp work properly
with the text buffer. I have no idea how to make *both* of these work, but
I'll keep on trying :-) You can get the text buffer back by reloading settings
(you will keep the screen output in the new buffer)

Paste does not work for some DOS programs (eg, QEdit, DOS RAR)

Copy has not been implemented for variable fonts yet (and probably never will)

There's a very rare crash when exiting some programs (happened to me when
exiting ncftp on two occassions) This is so rare, I wasn't able to trap it...

There's a strange window repaint bug when using some programs (e.g. less.exe
from GnuWin32 packet) This bug occurs only in release build of Console, and
not in debug version. I still haven't found the solution for this.

Another paint bug occurs with FTE editor from SourceForge: Console doesn't
show it. This one is also very puzzling...

Tray icon uses 256 colors, and it looks ugly if you don't use WinXP, since
Explorer under WinNT and Win2000 shows tray icons in 16 colors only. You can
download patched explorer.exe (shows tray icons in 256 colors) from this URL:
http://www.dr-hoiby.com/TrayIconIn256Color/index.html (I use it without
problems under Win2k SP2)

Increasing the number of rows and/or columns and reloading the settings can
sometimes result in a corrupt output. I haven't found a solution for this yet,
but it seems that restarting Console solves the problem.


12. ToDo
========

- fix bugs :-)
- add more stuff to the context menu
- ideas for new cursors are welcome


13. Version history
===================

v1.5    - removed debug output from Release build (forgot to do it sometime in
          the past)

        - removed "resize" attribute from <image> tag under <background>;
          added "style" and "extend" atributes

        - fixed fake transparency and relative backgrounds for multiple
          monitors (I hope :-)

        - a fix for failed transparency when using fake transparency; only the
          color you set as the background color will be treated as the
          background color and painted transparently (before, Console was
          trying to guess the current background color, but that didn't work
          reliably with some programs like vim that seem to change background
          color while painting their screen)

        - tweaked background creation a bit

        - improved startup time for tinted backgrounds

        - added <hide_console_timeout> tag to the <appearance> tag; it
          contorls how long the Win console window will be visible (but
          minimized) during shell startup; some shells (like 4NT) need Win
          console window visible during initialization

        - window position is not changed when reloading settings

        - you can set initial X or Y position to -1 to let the system place
          the Console window at startup

        - if Console title is set to "", " - " is no longer displayed at the
          beginning of the window title

        - added a few more cursor types: "noblink block", "pulse block",
          "rect", "noblink rect", "noblink line"

        - copied text now has \r\n at the end of each row

        - added <start_minimized> tag to <appearance> tag; if set to true,
          Console will start minimized

        - Minimize option is now available if Console has visible taskbar
          button (used to be disabled for some configurations)

        - added <console_color> tag under <background> element that specifies
          console background color; this color will be replaced with the
          <color> color; this enables you to keep Windows' console background
          black (the least problems) and set your own background color for
          Console window

        - rows and columns of <size> tag can be set to 'max' to maximize the
          Console window; see chapter 9.5 for details


v1.42   - reduced CPU usage for window repainting

        - proportional fonts work again (they got broken during optimizations)

        - mouse wheel scrolling looks better now

        - mouse whell messages are now redirected to console window (thanks to
          Francois Leblanc for submitting a patch)

        - fixed a small text selection bug (multiple selection rectangles
          could be drawn)

        - text can now be selected by left-click and drag if Console window
          is not mouse draggable

        - text can now be pasted by clicking the middle mouse button

        - added copy_on_select tag for X Windows style select/copy

        - fixed a text copy bug some people have reported

        - added a .reg file for activating file and directory name completion
          for cmd.exe (thanks to Rob 'insanitee' for the idea)

        - Console now supports Unicode characters

        - added the <inside_border> tag to <appearance> tag for setting client
          area border (text is drawn a bit further from the window edge for a
          more 'relaxed' look)

        - added a command line switch for console title (-t)

        - files and directories can now be drag-n-dropped to Console

        - Console window is sent to foreground at start now

        - finally fixed selection problems (they appeared when inside border
          size was greater than character width/height)

        - Console will now use cmd.exe as shell if no shell was specified in
          the config file and COMSPEC environment variable was not found

        - added support for custom program icons (<icon> tag under
          <appearance> tag)

        - console font colors can now be mapped to custom values

        - fixed "dead characters" handling (hopefully)

        - added support for non-truecolor bitmap backgrounds

        - new <behaviour> tag; moved <mouse_drag> and <copy_on_select> there,
          added <inverse_shift> tag that inverses SHIFT behaviour for
          selecting when mouse drag is enabled (thanks to Dean Scarff
          for contributing code)

        - Console window title set by -t command line option is no longer
          cleared after settings reload

        - added FreeImage library for image loading/manipulation stuff

        - added 'Edit configuration file' to the right-click menu. It will
          open a text editor of your choice (notepad by default)

        - added 'editor' attribute to <console> flag: used to specify text
          editor for the configuration file (default: notepad.exe)

        - fixed some GDI, handle and memory leaks

        - fixed a small clipboard bug (text copied from a console could not be
          pasted into the same console)

        - added flat scrollbar support (thanks to Adam Hodson for the code)

        - extended scrollbar supprot, added a new tag <scrollbar> for setting
          several scrollbar attributes

        - fixed a problem with custom icons (no custom icon was shown in the
          ALT-TAB menu)

        - Console window now looks like a regular window even when
          <taskbar_button> is set to 'hide' or 'tray'; clicking on the
          minimize button when Console shows tray icon minimizes the Console
          window to tray (single click on the tray icon to restore, double-
          clicking on the tray icon still works the same); Thanks to Daniel
          Cheng for the code

        - Console windows with hidden/tray taskbar option are no longer shown
          in the ALT-TAB menu (a bug introduced in b339)

        - added several things to the right-click menu: copy, paste,
          'always on top' toggle and configuration file selection submenu

        - fixed keyboard layout switching

        - cleaned up and reorganized the code a bit

        - added 'Readme' option to the menus; it opens the Readme.txt file

        - added support for behavior/behaviour tag spelling (no, I will not
          add alternate spellings for 'color' tags and attributes :-)

        - a user is prompted for configuration reload when changing a
          configuration file (thanks to Rob Davenport for the patch)

        - fixed a small handle leak introduced in b340 (in config file
          enumeration)

        - fixed a memory leak when variable-width fonts were used

        - tweaked cursor drawing a bit (this fixed a bug with fading block
          cursor and variable-width fonts)

        - added <reload_new_config> under <behavior> tag and '-r' command-line
          option to control configuration auto-loading (when a new
          configuration file is selected from the popup menu); supported
          options: 'yes', 'no' and 'prompt'; default is 'prompt'

        - fixed a bug with non-animated cursor painting

        - configuration files submenu is now populated at startup, so the list
          can be used by external programs; it is still refreshed when opening
          the popup menu

        - fixed a small bug with custom icons (if the icon file is not found,
          the default icons are loaded)

        - fixed a background image bug: if a background image is not found,
          Console will use solid background

        - added some comments to the sample console.xml file; thanks to Rob
          Davenport for the initial comments

        - added 'editor_params' attribute to <console> tag; it can be used to
          pass additional parameters to a text editor used to edit
          configuration and view the Readme file

        - both Console window and the hidden Windows console window have the
          same titlebar text now, so Console titlebar text just copies
          whatever Windows console titlebar shows (i.e., currently running
          command), except when Windows console titlebar text is completely
          changed (in that case Win console titlebar text is concatenated to
          the default Console title)

        - improved files drag'n'drop handling; if several files are dropped at
          once, they will be sent to Console, separated by spaces; also,
          filenames that contain spaces will be enclosed in double quotes

        - added <disable_menu> tag to <behavior> tag; it disables the popup
          menu for main window and tray icon, system menu is still available

        - improved text selection (fixed some small bugs as well)

        - if the Console title was changed in the config file and settings
          were reloaded, it got broken; that's fixed now

        - back to old window titles; Windows console window has "Console
          Command Window" title again; this simplifies things and fixes some
          bugs


v1.41   - fixed window repainting problem with a non-relative background image

        - fixed a painting problem with 'console' style cursor

        - fixed a crash after reloading settings when cursor style was changed
          to 'none' from something else

        - added multi-monitor support (without breaking WinNT compatibility, I
          hope :); many thanks to Daniel Soto (danisoto@users.sourceforge.net)
          for providing the code

        - minimize box is back! It got lost... Note that Console window will
          have minimize box only if its border is set to 'regular' and taskbar
          button to 'normal'

        - setting Console window title is now done only when a title text
          changes; this will, hopefully, fix a titlebar/taskbar flicker
          problem reported

        - improved copy code and fixed a little bug with 'jumpy' window when
          copying selection; note that copy behaviour is slightly different
          now: selection is copied when you release the left mouse button
          after clicking inside the selection


v1.40   - speed improvements

        - background painting bugfixes

        - improved cursor drawing (now the previous position is erased)

        - text selection got a bit screwed up due to painting optimizations,
          fixed

        - added change_refresh attribute to <console> tag; it controls how
          long window repainting is delayed after a change is detected in the
          real console

        - improved window title handling

        - fixed a small reload bug (window repainting)


v1.32   - fixed a console resizing bug (setting different number of columns)

        - improved screen refreshing (problematic ReadConsoleOutput function
          :-)

        - changed paste code a bit

        - better text selection redrawing (no more flicker)


v1.31   - a small alpha transparency painting bugfix


v1.30   - a small window sizing fix (with regular border)

        - window scrolling with background looks as it should now

        - changed 'stretch' attribute of 'image' node to 'resize'

        - added 'refresh' attribute to 'console' tag; it controls how often
          the console window redraws if there are no changes detected in the
          console; this timer is neccessary to ensure proper operation of some
          DOS programs (like RAR); this refresh timer can be switched off to
          reduce CPU usage, but some DOS programs will not be displayed
          correctly (if at all :-)

        - fixed Console crash if cursor style was set to 'none'

        - real console can have any background color; there was a problem with
          non-black console background and Console background image (please
          read instructions for background 'color' tag regarding background
          images)

        - added 'shell' attribute to 'console' node to override COMSPEC
          variable

        - improved window painting a bit

        - added background tinting for background images and fake transparency


v1.20   - console resizing bug fixed (occurs when running some DOS apps,
          like RAR)

        - added console title as an option

        - added font color as an option

        - changed font options' layout a bit

        - taskbar_hide tag changed to taskbar_button tag

        - you can put Console icon to sys tray now if you left-click on it
          will bring the Console window to the foreground, right-clicking will
          bring up the popup menu and left double click will hide/show the
          Console window

        - added a scrollable text buffer as an option (use buffer_rows
          attribute in size tag)

        - added paste (shift+right click)

        - added copy: shift+left click and drag to select, left click inside
          the selection to copy, left click outside of the selection to clear
          it

        - current command is now displayed in the Console main window (if the
          titlebar is visible, of course :-)

        - problems with some combinations of window border and taskbar button
          when reloading settings have been solved


v1.10   - changed border types: none, regular, thin (true and false are still
          accepted, and interpreted as regular and none, respectively)

        - if the window has a border (regular or thin) and mouse_drag is true,
          the window border is also mouse-draggable

        - added inactive_alpha attribute to transparency tag; if it's set to a
          value > 0, it sets the transparency of the window while it's
          inactive

        - you can toggle between configured z-order and "always on top"
          z-order with left double click

        - added "Reload settings" to the menu

        - some performance improvements

        - a new icon (hail and glory to Orcinus :-)

        - a new cursor: pulse rect

        - a new cursor: fading block -> alpha blended fading cursor! :) On
          WinNT it uses fake blend that's really useful only with solid
          backgrounds

        - fixed console cursor

        - added console.inf file that creates "Console Here" entry in the
          Explorer right-click menu

        - added "-c" command line option for sending an initial command to the
          shell

        - entries from the right-click menu are added to Console system menu
          (the one you get when you right-click on Console taskbar button)

        - if no configuration file is given in the cmd-line, Console will try
          to locate configuration file in console.exe directory, *not* in the
          current directory


v1.0    - initial release
