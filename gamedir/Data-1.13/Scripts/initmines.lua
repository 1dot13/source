--[[

*************    
** Remarks **
*************

The purpose of this Lua script is to provide the code and data needed to
initialize the strategic mines. There are two reasons for the game to make
calls to this script:

	1. The strategic layer gets initialized.
	   Right after a new game is started, the program calls InitializeMines to
	   gather the data defining the mines (see §1.). It then uses that data to
	   actually create the mines.
	
	2. The player enters a mine sector.
	   When a mine sector gets loaded (tactical mode), the program calls
	   InitializeHeadMiners to collect information on creation of head miners
	   and how to distribute them among the mines (see §2.).


**********************
** Global variables **
**********************
   
   - newDIFFICULTY_LEVEL (Start new game setting)
        1 = easy
		2 = experienced
		3 = expert
		4 = insane
   
    - iniWHICH_MINE_SHUTS_DOWN (ja2_options.ini setting)
        -1 = Game chooses a mine randomly.
		 0 = No mine will shut down!
		 2 = Drassen
		 3 = Alma
		 4 = Cambria
		 5 = Chitzena
		 6 = Grumm
]]

-- For debugging purposes you might want to have some basic logging output
-- from the game. Uncommenting the following line will create "initmines.log"
-- in your profile directory.
--logging = true



----------------------------------------
-- §0. Some setup code to get started --
----------------------------------------

-- Initialize the pseudo random number generator
math.randomseed( os.time() ); math.random(); math.random(); math.random()
-- http://lua-users.org/wiki/MathLibraryTutorial

MineType = { Silver = 0, Gold = 1 }



-----------------------------
-- §1. Mine initialization --
-----------------------------

--[[
    
    function InitializeMines()
    
    Gets called on initialization of strategic layer.

    This function is supposed to return an array table of initialization data
    for each mine. Every element itself must be a table consisting of the
    following members:
    
    - Location (required)
      String, defining the sector to place the mine, e.g. "A10".
    
    - Type
      Use MineType.Gold or MineType.Silver (cp. §0.).
    
    - MaxRemovalRate
      Amount of income to be gained at each production period ( = 1/4 of
      daily income).
      
    - RemainingOreSupply
      Total amount of money available to be extracted.
    
    - Infectible (0/1)
      Boolean, specifying whether or not a mine can be infected by crepitus.
      This is kind of a hack until creature quest becomes externalized.
      Do not set this to 1 for mines other than Drassen, Alma, Cambria, Grumm.
    
    - AssociatedUnderground
      Array table of strings specifying sector coordinates, e.g. "A10-1".
      This relates underground sectors to the mine. In these sectors the game
      adds miners working underground, and also uses that information to build
      the sector name.
      
]]

function InitializeMines()

    --------------------------------------
    -- §1.1. Set up initial mine status --
    --------------------------------------

    local mines = 
	{
        -- San Mona
        {
            Location = "D4", Type = MineType.Gold,
            MinimumProduction = 0,
            AssociatedUnderground = { "D4-1", "D5-1" },
        },

        -- Drassen
        {
            Location = "D13", Type = MineType.Silver,
            MinimumProduction = 1000,
            AssociatedUnderground = { "D13-1", "E13-1" },
            Infectible = 1,
        },

        -- Alma
        {
            Location = "I14", Type = MineType.Silver,
            MinimumProduction = 1500,
            AssociatedUnderground = { "I14-1", "J14-1" },
            Infectible = 1,
        },

        -- Cambria
        {
            Location = "H8", Type = MineType.Silver,
            MinimumProduction = 1500,
            AssociatedUnderground = { "H8-1", "H9-1" },
            Infectible = 1,
        },

        -- Chitzena
        {
            Location = "B2", Type = MineType.Silver,
            MinimumProduction = 500,
            AssociatedUnderground = { "B2-1" },
        },

        -- Grumm
        {
            Location = "H3", Type = MineType.Gold,
            MinimumProduction = 2000,
            AssociatedUnderground = { "H3-1", "I3-1", "I3-2", "H3-2", "H4-2" },
            Infectible = 1,
        },
    }


    -----------------------------------
    -- §1.2. Vanilla JA2 logic below --
    -----------------------------------

    -- create variables for easy identification at quest related stuff
    local DrassenMine = mines[2]
    local AlmaMine = mines[3]


    -- more initialization
    for i = 1, #mines do
        local m = mines[i]
        m.MaxRemovalRate = m.MinimumProduction

        if m.MinimumProduction > 0 then
            m.RemainingOreSupply = 999999999
        else
            m.RemainingOreSupply = 0
        end
    end


    -- Randomize the exact size of each mine. The total production is always
    -- the same and depends on the game difficulty, but some mines will
    -- produce more in one game than another, while others produce less.

    local numProductionIncreases = ({ 25, 25, 20, 15 })[newDIFFICULTY_LEVEL]

    while numProductionIncreases > 0 do
        local m

        -- pick a producing mine at random and increase its production
        repeat
            m = mines[math.random(#mines)]
        until m.MinimumProduction > 0

        -- increase mine production by 20% of the base (minimum) rate
        m.MaxRemovalRate = m.MaxRemovalRate + m.MinimumProduction / 5

        numProductionIncreases = numProductionIncreases - 1
    end


    
    -- Incorporate cheat option...
    -- switch on iniWHICH_MINE_SHUTS_DOWN
    -- 0: no mine runs out, skip this altogether
    -- -1: choose mine randomly
    -- >0: ID of the mine that shall run out

    if iniWHICH_MINE_SHUTS_DOWN ~= 0 then

        local m
    
        if iniWHICH_MINE_SHUTS_DOWN == -1 then
            -- Choose which mine will run out of production.
            -- This will never be the Alma mine or an empty mine (San Mona)...

            -- choose randomly
            repeat
                m = mines[math.random(#mines)]
                -- Alma mine can't run out for quest-related reasons
            until m.MinimumProduction > 0 and m ~= AlmaMine
        else
             -- Select mine by given index
             -- No safety checks whatsoever, any error will be pretty much in
             -- your face. If it breaks it's all their fault, I don't care...
             m = mines[iniWHICH_MINE_SHUTS_DOWN]
        end
        
        if m == DrassenMine then
            minDaysBeforeDepletion = 20
        else
            minDaysBeforeDepletion = 10
        end

        m.RemainingOreSupply = minDaysBeforeDepletion * 4 * m.MaxRemovalRate

        -- We don't want the creatures to infect the mine that runs out.
        m.Infectible = 0
    end

    return mines;

end -- function InitializeMines



------------------------------------
-- §2. Head miners initialization --
------------------------------------

--[[

    function InitializeHeadMiners(mines, currentMine)

    This function is called when the player enters a mine sector.
    
    It can return either nil or the head miners initialization and
    distribution. If it returns nil, it will be called again when a mine
    sector gets loaded. If it returns a value other than nil, it will not be
    called again.
    
    Parameters:

    - mines
      An array of tables featuring only the following member:
      - MaxRemovalRate
    
    - currentMine
      This is the index of the mine being entered by the player (cp. §1.1.).
    
    If the function does not return nil but the head miners distribution, it
    must return an array of tables consisting of the following members:
    
    - Profile
      The profile ID of the head miner.
    
    - Quotes
      Array of exactly four integers defining the quote IDs of the head miner
      for the following strategic events:
      1. RUNNING_OUT
      2. CREATURES_ATTACK
      3. CREATURES_GONE
      4. CREATURES_AGAIN
      It is valid to insert -1 for missing quotes.
    
    - MineID
      The index of the mine the head miner is assigned to (cp. §1.1.).

 ]]
 
function InitializeHeadMiners(mines, currentMine)

    local headMinerInfo = {

        -- Fred
        { Profile = 106, Quotes = { 17, 18, 27, 26 }, },

        -- Matt
        { Profile = 148, Quotes = { -1, 18, 32, 31 }, },

        -- Oswald
        { Profile = 156, Quotes = { 14, 15, 24, 23 }, },

        -- Calvin
        { Profile = 157, Quotes = { 14, 15, 24, 23 }, },

        -- Carl
        { Profile = 158, Quotes = { 14, 15, 24, 23 }, },

    }

    local Fred = headMinerInfo[1]
    local Matt = headMinerInfo[2]

    local AlmaID = 3 -- cp. §1.1.


    -- don't place miners until player enters a producing mine
    if mines[currentMine].MaxRemovalRate == 0 then
        return nil
    end


    -- Matt is always head miner in Alma
    Matt.MineID = AlmaID


    -- Fred is the first miner the player encounters unless it's Alma
    if currentMine ~= AlmaID then
        Fred.MineID = currentMine
    end


    -- collect IDs of all producing mines except Alma mine and Fred's
    local MinesLeft = {}
    for i = 1, #mines do
        if i ~= AlmaID and i ~= currentMine then
            if mines[i].MaxRemovalRate > 0 then
                table.insert(MinesLeft, i)
            end
        end
    end

    -- randomly distribute miners
    for i = 1, #headMinerInfo do
        if headMinerInfo[i].MineID == nil then
            local index = math.random(#MinesLeft)
            headMinerInfo[i].MineID = MinesLeft[index]
            table.remove(MinesLeft, index)
        end
    end

    return headMinerInfo

end -- function InitializeHeadMiners
