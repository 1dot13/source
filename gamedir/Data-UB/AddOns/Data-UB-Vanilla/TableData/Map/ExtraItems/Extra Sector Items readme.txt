This is an old 1.13 Feature which seems to be hidden for Modders long time.
This feature has been reworked by Headrock
(edit: and since it got lost again, now revived)

-------------------------
XML filename format:
-------------------------

\tabledata\Map\ExtraItems\[SectorGrid]_[ZLevel]_ExtraItems_[DifficultyName].xml

Examples:
\TableData\Map\ExtraItems\A9_0_ExtraItems_Novice.xml	-> for entering the A9 surface sector (Omerta) when playing novice difficulty.

(edit: -> underground-levels see [NOTE] below)


The following "DifficultyNames" are valid:

- Novice
- Experienced
- Expert
- Insane


-------------------------
XML content format:
-------------------------

<ExtraItems>
   <Item>
      <uiIndex>#</uiIndex>
      <quantity>#</quantity>
      <condition>#</condition>
      <gridno>#</gridno>
      <visible>#</visible>
   </Item>
</ExtraItems>


-------------------------
XML notes:
-------------------------

- For clarity, uiIndex should always come first. However, I've set it so that the position of uiIndex
  in the list is not relevant (so long as it exists). Most XMLs work that way, and I think that's fine. 

- All other tags can come in any order. 

- Quantity can be set to any number, but will only create an item (or more) if above 0. 
  Quantity of 0 or less will not crash the game, it simply won't create items. 
  If this tag is completely omitted, the game will create one item by default.

- Condition can be set to 1-100. If it is set outside these values, the item will not be created 
  (the game won't crash). If this tag is completely omitted, the item will have 100 condition. 

- Gridno can be set to any positive number, including 0. However, if it is out of bounds, 
  the game will generate a screen message that it could not place the item, but will not crash the game. 
  Obviously, there is no rational default for this tag, so it must always be included. 

- Visible can be set to 0 or 1. When 1, will create an item that is visible from the moment you enter the map. 
  Otherwise, a merc must spot the item by simple sight, like all other items on the map. 
  If this tag is omitted, the item is created with 0 visibility by default (must be spotted). 

- if GridNo is at closed container (like a crate, furniture, etc.), visible should be set to 1 (hidden)

- random-items can also be used

- As I understand, that's happening in realtime. So it's probably advised to not overdo it

  It's meant to add a few goodies (differing on which difficulty level we're at), not to replace the item-placement via mapeditor

- added a few example files to portrait the idea

[NOTE: It's a bummer, but it doesn't seem to work with underground-levels. That might be an old issue, at least all examples I found, even going back to v4870, also only had ground-level in use as well. So, for the time being, it only works on ground-level (using only 0 for level, like in "A9_0_ExtraItems_Novice.xml")]	

(edited 22.08.2023)