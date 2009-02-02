#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
	#include "_Ja25Englishtext.h"
#else
	#include "Language Defines.h"
	#ifdef CHINESE
		#include "text.h"
		#include "Fileman.h"
	#endif
#endif

#ifdef CHINESE

// VERY TRUNCATED FILE COPIED FROM JA2.5 FOR ITS FEATURES FOR JA2 GOLD

STR16	zNewTacticalMessages[]=
{
	/*
	L"Range to target: %d tiles, Brightness: %d/%d",
	L"Attaching the transmitter to your laptop computer.",
	L"You cannot afford to hire %s",
	L"For a limited time, the above fee covers the cost of the entire mission and includes the equipment listed below.",
	L"Hire %s now and take advantage of our unprecedented 'one fee covers all' pricing.  Also included in this unbelievable offer is the mercenary's personal equipment at no charge.",
	*/
	L"目标的距离: %d格, 亮度: %d/%d",
	L"将发报机装到笔记本电脑上。",
	L"你无法支付雇佣%s的费用",
	L"在限定时间内，以上的费用包括了整个行动和下列装备的花费。",
	L"现在就雇请%s吧。您可以享受我们提供的空前的“一次付费，全部服务”的优惠价格。在这个难以置信的出价里，佣兵的随身装备是免费的哦。",	
	/*
	L"Fee",
	L"There is someone else in the sector...",
	L"Gun Range: %d tiles, Chance to hit: %d percent",
	L"Display Cover",
	L"Line of Sight",
	*/
	L"费用",
	L"在本分区发现有人……",
	L"枪的射程: %d格， 命中率: %d％",
	L"显示覆盖物",
	L"视距",

/*
	L"New Recruits cannot arrive there.",
	L"Since your laptop has no transmitter, you won't be able to hire new team members.  Perhaps this would be a good time to load a saved game or start over!",
	L"%s hears the sound of crumpling metal coming from underneath Jerry's body.  It sounds disturbingly like your laptop antenna being crushed.",  //the %s is the name of a merc.  @@@  Modified
	L"After scanning the note left behind by Deputy Commander Morris, %s senses an oppurtinity.  The note contains the coordinates for launching missiles against different towns in Arulco.  It also gives the coodinates of the origin - the missile facility.",
	L"Noticing the control panel, %s figures the numbers can be reveresed, so that the missile might destroy this very facility.  %s needs to find an escape route.  The elevator appears to offer the fastest solution...",
*/
	L"新雇请的佣兵无法到达那里。",
	L"由于你的笔记本电脑没有发报机，你无法雇请新的队员。也许你得读取存档或者重新开始游戏！",
	L"%s听到了Jerry的身体下面传来金属的破碎的声音。听起来令人不安，似乎你的笔记本电脑的天线被压断了。", 
	L"看完副指挥官Morris留下的备忘录后，%s觉得有机会了。备忘录里有向Arulco各个城镇发射导弹的基地的坐标。它还给出了这个罪恶计划的发源地的坐标 —— 导弹工厂。",
	L"看到了控制面板后， %s发现它正在倒计时，因此导弹会把这个工厂炸毁。%s得找出个脱逃的路线。使用电梯看起来是最快的办法...",
/*
	L"This is an IRON MAN game and you cannot save when enemies are around.",	//	@@@  new text
	L"(Cannot save during combat)", //@@@@ new text
	L"The current campaign name is greater than 30 characters.",							// @@@ new text
	L"The current campaign cannot be found.",																	// @@@ new text
	L"Campaign: Default ( %S )",																							// @@@ new text
*/
	L"现在您在铁人模式进行游戏，周围有敌人的时候不能存档。",	
	L"(不能在战斗时存盘)",	
	L"当前的战役名称超过了30个字符。",						
	L"无法找到当前的战役。",											// @@@ new text
	L"战役: 默认 ( %S )",											// @@@ new text
/*
	L"Campaign: %S",																													// @@@ new text
	L"You have selected the campaign %S. This campaign is a player-modified version of the original Unfinished Business campaign. Are you sure you wish to play the %S campaign?",			// @@@ new text
	L"In order to use the editor, please select a campaign other than the default.",		///@@new
*/
	L"战役: %S",												// @@@ new text
	L"你选择了%S战役。 该战役是原版UB战役的玩家自定义游戏版本。你确认你要在 %S 战役下进行游戏吗?",			
	L"如果你要使用编辑器的话，请选择一个战役，不要用默认战役。",			
};

//these strings match up with the defines in IMP Skill trait.cpp
STR16 gzIMPSkillTraitsText[]=
{
	L"开锁",	//"Lock picking",
	L"徒手格斗", //"Hand to hand combat",
	L"电子",	//"Electronics",
	L"夜战",	//"Night operations",
	L"投掷",	//"Throwing",
	L"教学",	//"Teaching",
	L"重武器",	//"Heavy Weapons",
	L"自动武器",	//"Auto Weapons",
	L"潜行",	//"Stealth",
	L"双手武器",	//"Ambidextrous",
	L"刀技",	//"Knifing",
	L"狙击手",	//"Sniper",
	L"伪装",	//"Camouflage",
	L"武术",	//"Martial Arts",

	L"无",	//"None",
	L"I.M.P 专属技能",	//"I.M.P. Specialties",
};
//@@@:  New string as of March 3, 2000.
STR16	gzIronManModeWarningText[]=
{
//	L"You have chosen IRON MAN mode. This setting makes the game considerably more challenging as you will not be able to save your game when in a sector occupied by enemies. This setting will affect the entire course of the game.  Are you sure want to play in IRON MAN mode?",
	L"你选择了铁人模式。这将会游戏变得相当有挑战性，因为你无法在敌人占据的分区存档。 这个设置会影响游戏的整个进程。你确认你要在铁人模式下进行游戏吗?",
};


#endif
