--[[

    December 27, 2010
    
    TOC
    ===
    
        Preamble
        §0. Constants
        §1. Underground sector list compilation
        §2. Defining enemy garrisons and creature population
        §3. Internal workings
        §3.1. Underground sector names


    Preamble
    ========

    This script sets up and initializes underground sectors. It defines where
    underground sectors are located on the map, what enemy garrisons and
    creature population they feature, and what loading screen to present to
    the player.

    Sector definitions are read whenever a new game starts. They are then
    stored within the savegame meaning various changes to this script don't
    have an effect on a game in progress.

    
    Modders likely want to focus on §1. and §2.


    There is also another script closely related to this one, which defines
    sector names ("undergroundsectornames.lua" and localized versions). See
    §3.1. for more details.
    

    The game will call this script for any of three reasons:
    
      1. When a new game starts it will call 'BuildUndergroundSectorList' in
         order to create and initialize the array of underground sectors.

      2. The game will call 'GetLoadscreen' whenever an underground sector
         needs to be loaded.

      3. When the game requests an underground sector name it will call
         'GetSectorName'.


    Remarks
    
     - Several vanilla JA2 sectors may be unsafe to remove due to hardcoded
       behaviour such as creature spreading, Deidranna escaping or other quest
       related links.
     
     - Contrary to random number generators used in JA2, Lua's math.random
       function includes the range boundaries. Also, indexing in Lua starts at
       1 as opposed to C (starting at 0). For further information about Lua
       consult the Lua documentation at http://www.lua.org/docs.html

]]


-- For debugging purposes you might want to have some basic logging output
-- from the game. Uncommenting the following line will create
-- "initunderground.log" in your profile directory.
--logging = true


-- Initialize the pseudo random number generator
math.randomseed( os.time() ); math.random(); math.random(); math.random()
-- http://lua-users.org/wiki/MathLibraryTutorial



-------------------
-- §0. Constants --
-------------------

Habitat = {             -- creature type distribution in percentages
                        --                young young adult adult
                        -- larvae infants male female male female
    QueenLair      = 0, --   20     40      0     0    30    10    (default)
    Lair           = 1, --   15     35     10     5    25    10
    LairEntrance   = 2, --    0     15     30    10    35    10
    InnerMine      = 3, --    0      0     20    40    10    30
    OuterMine      = 4, --    0      0     10    65     5    20
    FeedingGrounds = 5, --    -      -      -     -     -     -    (unused)
    MineExit       = 6, --    0      0     10    65     5    20
}

CreatureMusic = {
    Compat = 0,     -- default, use creepy music in the presence of creatures
                    -- or blue lights (default, also vanilla default);
    Auto = 1,       -- use creepy music if and only if there any creatures
                    -- present (ignoring lights);
    Never = 2,      -- do not use creepy music, regardless of any creatures;
    Always = 3,     -- do use creepy music, regardless of any creatures
}
    


---------------------------------------------
-- §1. Underground sector list compilation --
---------------------------------------------

--[[

    In this list add underground sectors by specifying their location.
    If no loadscreen attribute is given the script will use "LS_Mine" for
    level 1 sectors and "LS_Cave" for level 2 and level 3 sectors (see §3.).
    
    Also specify any other static attributes in this list (cf. §2.)

]]

local sectorList = {


	{ location = "I13-1", loadscreen = "LS_MINEDAY", },
	{ location = "J13-1", loadscreen = "LS_MINEDAY", },
	{ location = "J14-1", loadscreen = "LS_TUNNELS", },
	{ location = "K14-1", loadscreen = "LS_TUNNELS", },
	{ location = "K15-1", loadscreen = "LS_UPSTAIRS", },
	{ location = "K15-2", loadscreen = "LS_UPSTAIRS", },
	{ location = "L15-2", loadscreen = "LS_COMPLEXBSMNT", },
	{ location = "L15-3", loadscreen = "LS_BASEMENT_GENERIC", },
	
    -- Demoville
    { location = "P1-1", loadscreen = "LS_Basement", },
    {
      location = "P1-2", loadscreen = "LS_Cave",
      creatureHabitat = Habitat.InnerMine, music = CreatureMusic.Compat,
    },
}



--[[

    PopulateSectors

Remarks

    This function is supposed to initialize sector settings that depend on
    what difficulty level and game style the player chooses.

Parameters

    difficultyLevel (integer)
    
        Indicates the level of difficulty.
            1: easy, 2: experienced, 3: expert, 4: insane
    
    gameStyle (integer)
    
        Indicates whether or not the player selected the scifi option.
            0: realistic, 1: scifi
]]

local function PopulateSectors(difficultyLevel, gameStyle)

    local s = sectorList


    ----------------------------------------------------------
    -- §2. Defining enemy garrisons and creature population --
    ----------------------------------------------------------
    
	if difficultyLevel == 1 then
		s["K15-1"].numTroops = 14  + math.random(0, 6)
		s["K15-1"].numElites = 0
	elseif difficultyLevel == 2 then
		s["K15-1"].numTroops = 14  + math.random(0, 4)
		s["K15-1"].numElites = 4  + math.random(0, 3)
	elseif difficultyLevel == 3 then
		s["K15-1"].numTroops = 15  + math.random(0, 4)
		s["K15-1"].numElites = 6  + math.random(0, 4)
	elseif difficultyLevel == 4 then
		s["K15-1"].numTroops = 14 + math.random(0, 6)
		s["K15-1"].numElites = 10 + math.random(0, 2)
	end
	
	if difficultyLevel == 1 then
		s["K15-2"].numTroops = 10 + math.random(0, 3)
		s["K15-2"].numElites = 0
	elseif difficultyLevel == 2 then
		s["K15-2"].numTroops = 15 + math.random(0, 2)
		s["K15-2"].numElites = 5 + math.random(0, 2)
	elseif difficultyLevel == 3 then
		s["K15-2"].numTroops = 12 + math.random(0, 3)
		s["K15-2"].numElites = 10 + math.random(0, 2)
	elseif difficultyLevel == 4 then
		s["K15-2"].numTroops = 14 + math.random(0, 6)
		s["K15-2"].numElites = 10 + math.random(0, 2)
	end
	
	if difficultyLevel == 1 then
		s["L15-2"].numTroops = 12 + math.random(0, 2)
		s["L15-2"].numElites = 1 + math.random(0, 2)
	elseif difficultyLevel == 2 then
		s["L15-2"].numTroops = 17 + math.random(0, 2)
		s["L15-2"].numElites = 5 + math.random(0, 2)
	elseif difficultyLevel == 3 then
		s["L15-2"].numTroops = 10 + math.random(0, 3)
		s["L15-2"].numElites = 14 + math.random(0, 2)
	elseif difficultyLevel == 4 then
		s["L15-2"].numTroops = 14 + math.random(0, 6)
		s["L15-2"].numElites = 10 + math.random(0, 2)
	end
    
	if difficultyLevel == 1 then
		s["L15-3"].numTroops = 26
		s["L15-3"].numElites = 2
	elseif difficultyLevel == 2 then
		s["L15-3"].numTroops = 20
		s["L15-3"].numElites = 8
	elseif difficultyLevel == 3 then
		s["L15-3"].numTroops = 13
		s["L15-3"].numElites = 15
	elseif difficultyLevel == 4 then
		s["L15-3"].numTroops = 10
		s["L15-3"].numElites = 22
	end

end



---------------------------
-- §3. Internal workings --
---------------------------

--[[

    BuildUndergroundSectorList

Remarks
    
    This function gets called by the game when a new game starts. It is
    supposed to return the initialized list of underground sectors.
    
Parameters

    gameSettings
    
        Table holding these keys:
        
        - difficultyLevel (integer)
          1: easy, 2: experienced, 3: expert, 4: insane
          
        - gameStyle (integer)
          0: realistic, 1: scifi
          
Return value

    This function shall return a table of tables that may feature the
    following keys:
    
    - location (required)
      string of the form "[R][C]-[L]", where [R] is a row
      identifier (A-P), [C] is a column identifier (1-16), [L] is a
      sublevel identifier (1-3), e.g. "A9-1"

    - numAdmins
    - numTroops
    - numElites
      integers, specifying numbers of enemy garrisons, default: 0

    - numBloodcats
      integer, specifying quantity of bloodcat population, default: 0
      This requires bloodcat placements to be set in the map itself.
    
    - numCreatures
      integer, specifying number of creatures in total, default: 0
      Distribution of creature types depends on creature habitat (see §0.)

    - creatureHabitat
      integer, specifying creature distribution type
      Use one of the constants from the Habitat enumeration (see §0.)
    
    - music
      integer, specifying under which circumstances to use creepy music
      Use one of the costants from CreatureMusic enumeration (see §0.)
]]

function BuildUndergroundSectorList(gameSettings)

    local difficultyLevel = gameSettings["difficultyLevel"]
    local gameStyle = gameSettings["gameStyle"]

    PopulateSectors(difficultyLevel, gameStyle)

    return sectorList
    
end


--[[

    GetLoadscreen
    
Remarks

    Provides the game with the name and file format of the loadscreen for a
    given sector. The function forwards the loadscreen attribute as declared
    in the sectorList (cf. §1.) or uses default screens.
    The game will use the name and format as well as screen resolution to
    build the complete file name itself.
    

Parameters

    location (string)
    
        The usual sector location string of the sector the loadscreen is
        requested for, e.g. "A10-1".
    
    timeOfDay (integer)
    
        The time in minutes that has passed since midnight.

Return values

    This function shall return a 2-tuple of strings, of which the first shall
    be the loadscreen name and the second the file extension.

]]

function GetLoadscreen(location, timeOfDay)

    local function Return(name)
        return "Loadscreens\\" .. name, "sti"
    end

    local s = sectorList[location]

    if s ~= nil then

        -- multiple screens to choose from?
        if s.loadscreens ~= nil and #s.loadscreens > 0 then
            local screen = s.loadscreens[math.random(#s.loadscreens)]
            return Return(screen)
        end

        -- single screen?
        if s.loadscreen ~= nil then
            return Return(s.loadscreen)
        end
    end

    -- resorting to defaults
    do
        -- get last character, aka z level
        local level = location:sub(#location, #location)
        if level == "1" then
            return Return("LS_Mine")
        else
            return Return("LS_Cave")
        end
    end
end



------------------------------------
-- §3.1. Underground sector names --
------------------------------------

--[[
    
    Before the game loads this very script it processes another one,
    'undergroundsectornames.lua', specifically. For this purpose, it basically
    concatenates the two scripts.
    
    'undergroundsectornames.lua' shall define a table of strings, which will
    then be referenced in this script to provide the game with sector names.
    That specific table shall be called 'sectornames', so make sure to not
    overwrite that variable by accident.

 ]]
  --[[

    GetSectorName
    
Remarks

    Provides the game with underground sector names.
    This function can be called often (i.e. every frame) since currently the
    result will not be cached.

Parameters:

    sectorCoods
    
        usual coordiantes string, e.g. "A10-1"
        
    sectorDetails
    
        table featuring the following items
        
        - visited (boolean)
          Indicates if the player has entered the sector at least once.
          
        - creaturesPresent (boolean)
          Indicates whether or not there are any monsters alive.
        
        - detailed (boolean)
          Indicates whether the game requests a detailed name.

Return value

    This function shall return the requested sector name as a UTF-8 encoded
    string.

]]

function GetSectorName(sectorCoords, sectorDetails)

    if sectorDetails.visited then
        local dash = sectorCoords:find("-")
        local coords = sectorCoords:sub(1, dash-1)

        if sectornames[sectorCoords] ~= nil then
            return coords .. ": " .. sectornames[sectorCoords]
        elseif sectornames[sectorCoords:lower()] ~= nil then
            -- try lowercase version
            return coords .. ": " .. sectornames[sectorCoords:lower()]
        else
            if sectorDetails.creaturesPresent then
                return coords .. ": " .. sectornames.creatureLair
            else
                return sectorCoords
            end
        end
    else
        -- not visited
        return ""
    end
    
end



-- For convenient access allow indexing the sectorList by location string.
mt = {}
setmetatable(sectorList, mt)
mt.__index = function(table, key)
    for i = 1, #table do
        local t = rawget(table, i)
        if (t.location == key) then
            return t
        end
    end
    return nil
end
