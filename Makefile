# Makefile - JA2.dsw

.PHONY: all
all: \
	Editor \
	Laptop \
	"Standard Gaming Platform" \
	Strategic \
	Tactical \
	TacticalAI \
	TileEngine \
	Utils \
	ja2 

.PHONY: Editor
Editor:
	$(MAKE) -C Editor -f Editor.mak

.PHONY: Laptop
Laptop: 
	$(MAKE) -C Laptop -f Laptop.mak

.PHONY: "Standard Gaming Platform"
"Standard Gaming Platform":

.PHONY: Strategic
Strategic: 
	$(MAKE) -C Strategic -f Strategic.mak

.PHONY: Tactical
Tactical: 
	$(MAKE) -C Tactical -f Tactical.mak

.PHONY: TacticalAI
TacticalAI:
	$(MAKE) -C TacticalAI -f TacticalAI.mak

.PHONY: TileEngine
TileEngine: 
	$(MAKE) -C TileEngine -f TileEngine.mak

.PHONY: Utils
Utils:
	$(MAKE) -C Utils -f Utils.mak

.PHONY: ja2
ja2: 
	$(MAKE) -f JA2.mak

.PHONY: clean
clean:
	$(MAKE) -C Editor -f Editor.mak clean
	$(MAKE) -C Laptop -f Laptop.mak clean

	$(MAKE) -C Strategic -f Strategic.mak clean
	$(MAKE) -C Tactical -f Tactical.mak clean
	$(MAKE) -C TacticalAI -f TacticalAI.mak clean
	$(MAKE) -C TileEngine -f TileEngine.mak clean
	$(MAKE) -C Utils -f Utils.mak clean
	$(MAKE) -f JA2.mak clean

.PHONY: depends
depends:
	$(MAKE) -C Editor -f Editor.mak depends
	$(MAKE) -C Laptop -f Laptop.mak depends

	$(MAKE) -C Strategic -f Strategic.mak depends
	$(MAKE) -C Tactical -f Tactical.mak depends
	$(MAKE) -C TacticalAI -f TacticalAI.mak depends
	$(MAKE) -C TileEngine -f TileEngine.mak depends
	$(MAKE) -C Utils -f Utils.mak depends
	$(MAKE) -f JA2.mak depends
