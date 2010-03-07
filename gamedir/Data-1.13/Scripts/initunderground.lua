-----------------------
-- Set some stuff up --
-----------------------

-- Initialize the pseudo random number generator
math.randomseed( os.time() ); math.random(); math.random(); math.random()
-- http://lua-users.org/wiki/MathLibraryTutorial



--[[

    REMARKS
    =======
    
     - Several vanilla JA2 sectors may be unsafe to remove due to hardcoded
       behaviour such as creature spreading, Deidranna escaping or other quest
       related links.
     
     - Contrary to random number generators used in JA2, Lua's math.random
       function includes the range boundaries. Also, indexing in Lua starts at
       1 as opposed to C (starting at 0). Find further information about Lua at
       http://www.lua.org/docs.html
    
     - Global variables:
        - difficultyLevel
            1: easy, 2: experienced, 3: expert, 4: insane
        - gameStyle
            0: realistic, 1: scifi
    
     - To add an underground sector to the list, create a table and pass it as
       an argument to addSector function. These tables may consist of the
       following members:
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
          This requires bloodcat placements to be set.
        
        - numCreatures
          integer, specifying number of creatures in total, default: 0
          Distribution of creature types depends on creature habitat (see below)

        - creatureHabitat
          integer, specifying creature distribution type
          Use one of the constants below (also featuring details).

]]

Habitat = {           -- creature type distribution in percentages
                      --                young young adult adult
                      -- larvae infants male female male female
    QueenLair    = 0, --   20     40      0     0    30    10
    Lair         = 1, --   15     35     10     5    25    10
    LairEntrance = 2, --    0     15     30    10    35    10
    InnerMine    = 3, --    0      0     20    40    10    30
    OuterMine    = 4, --    0      0     10    65     5    20
    MineExit     = 6, --    0      0     10    65     5    20
}



-----------------------------
-- INTERESTING STUFF BELOW --
-----------------------------


    -- Miguel's basement
addSector( { location = "A10-1" } )



    -- Chitzena mine
addSector( { location = "B2-1" } )



    -- San Mona mine
addSector( { location = "D4-1" } )
addSector( { location = "D5-1" } )



    -- Tixa
tixa_1 = { }
tixa_1.location = "J9-1"
tixa_1.numTroops = ({ 8, 11, 15, 20 })[difficultyLevel]
addSector( tixa_1 )

    -- feeding zone
tixa_2 = { }
tixa_2.location = "J9-2"
tixa_2.numCreatures = 2 + difficultyLevel*2 + math.random(0, 1)
addSector( tixa_2 )



    -- Orta
orta = { }
orta.location = "K4-1"
orta.numTroops = 6 + difficultyLevel*2 + math.random(0, 2)
orta.numElites = 4 + difficultyLevel + math.random(0, 1)
addSector( orta )



    -- Meduna
o3 = { location = "O3-1" }
o3.numTroops = 6 + difficultyLevel*2 + math.random(0, 2)
o3.numElites = 4 + difficultyLevel + math.random(0, 1)
addSector( o3 )

p3 = { location = "P3-1" }
if difficultyLevel == 1 then
    -- easy
    p3.numElites = 8 + math.random(0, 2)
elseif difficultyLevel == 2 then
    -- medium
    p3.numElites = 10 + math.random(0, 5)
elseif difficultyLevel == 3 then
    -- hard
    p3.numElites = 14 + math.random(0, 6)
elseif difficultyLevel == 4 then
    -- insane
    p3.numElites = 20
end
addSector(p3)



    -- Drassen mine
addSector( { location = "D13-1" } )
addSector( { location = "E13-1" } )
addSector( { location = "E13-2" } )
addSector( { location = "F13-2" } )
addSector( { location = "G13-2" } )
addSector( { location = "G13-3" } )
addSector( { location = "F13-3" } )



    -- Cambria mine
addSector( { location = "H8-1" } )
addSector( { location = "H9-1" } )
addSector( { location = "H9-2" } )
addSector( { location = "H8-2" } )
addSector( { location = "H8-3" } )
addSector( { location = "I8-3" } )
addSector( { location = "J8-3" } )



    -- Alma Mine
addSector( { location = "I14-1" } )
addSector( { location = "J14-1" } )
addSector( { location = "J14-2" } )
addSector( { location = "J13-2" } )
addSector( { location = "J13-3" } )
addSector( { location = "K13-3" } )



    -- Grumm mine
addSector( { location = "H3-1" } )
addSector( { location = "I3-1" } )
addSector( { location = "I3-2" } )
addSector( { location = "H3-2" } )
addSector( { location = "H4-2" } )
addSector( { location = "H4-3" } )
addSector( { location = "G4-3" } )



    -- Demoville
p1_1 = { location = "P1-1" }
p1_1.numTroops = ({ 12, 16, 16,  0 })[difficultyLevel]
p1_1.numElites = ({  0,  0,  4, 24 })[difficultyLevel]
addSector(p1_1)

p1_2 = { location = "P1-2", creatureHabitat = Habitat.InnerMine }
p1_2.numCreatures = ({ 3, 5, 8, 13 })[difficultyLevel]
addSector(p1_2)
