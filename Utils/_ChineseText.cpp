#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#if defined( CHINESE )
		#include "text.h"
		#include "Fileman.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_ChineseText_public_symbol(void){;}

#if defined( CHINESE )

/*

******************************************************************************************************
**                                  IMPORTANT TRANSLATION NOTES                                     **
******************************************************************************************************

GENERAL INSTRUCTIONS
- Always be aware that foreign strings should be of equal or shorter length than the English equivalent.
	I know that this is difficult to do on many occasions due to the nature of foreign languages when
	compared to English.  By doing so, this will greatly reduce the amount of work on both sides.  In
	most cases (but not all), JA2 interfaces were designed with just enough space to fit the English word.
	The general rule is if the string is very short (less than 10 characters), then it's short because of
	interface limitations.  On the other hand, full sentences commonly have little limitations for length.
	Strings in between are a little dicey.
- Never translate a string to appear on multiple lines.  All strings L"This is a really long string...",
	must fit on a single line no matter how long the string is.  All strings start with L" and end with ",
- Never remove any extra spaces in strings.  In addition, all strings containing multiple sentences only
	have one space after a period, which is different than standard typing convention.  Never modify sections
	of strings contain combinations of % characters.  These are special format characters and are always
	used in conjunction with other characters.  For example, %s means string, and is commonly used for names,
	locations, items, etc.  %d is used for numbers.  %c%d is a character and a number (such as A9).
	%% is how a single % character is built.  There are countless types, but strings containing these
	special characters are usually commented to explain what they mean.  If it isn't commented, then
	if you can't figure out the context, then feel free to ask SirTech.
- Comments are always started with // Anything following these two characters on the same line are
	considered to be comments.  Do not translate comments.  Comments are always applied to the following
	string(s) on the next line(s), unless the comment is on the same line as a string.
- All new comments made by SirTech will use "//@@@ comment" (without the quotes) notation.  By searching
	for @@@ everytime you recieve a new version, it will simplify your task and identify special instructions.
  Commonly, these types of comments will be used to ask you to abbreviate a string.  Please leave the
	comments intact, and SirTech will remove them once the translation for that particular area is resolved.
- If you have a problem or question with translating certain strings, please use "//!!! comment"
	(without the quotes).  The syntax is important, and should be identical to the comments used with @@@
	symbols.  SirTech will search for !!! to look for your problems and questions.  This is a more
	efficient method than detailing questions in email, so try to do this whenever possible.



FAST HELP TEXT -- Explains how the syntax of fast help text works.
**************

1) BOLDED LETTERS
	The popup help text system supports special characters to specify the hot key(s) for a button.
	Anytime you see a '|' symbol within the help text string, that means the following key is assigned
	to activate the action which is usually a button.

	EX:  L"|Map Screen"

	This means the 'M' is the hotkey.  In the game, when somebody hits the 'M' key, it activates that
	button.  When translating the text to another language, it is best to attempt to choose a word that
	uses 'M'.  If you can't always find a match, then the best thing to do is append the 'M' at the end
	of the string in this format:

	EX:  L"Ecran De Carte (|M)"  (this is the French translation)

	Other examples are used multiple times, like the Esc key  or "|E|s|c" or Space -> (|S|p|a|c|e)

2) NEWLINE
  Any place you see a \n within the string, you are looking at another string that is part of the fast help
	text system.  \n notation doesn't need to be precisely placed within that string, but whereever you wish
	to start a new line.

	EX:  L"Clears all the mercs' positions,\nand allows you to re-enter them manually."

	Would appear as:

				Clears all the mercs' positions,
				and allows you to re-enter them manually.

	NOTE:  It is important that you don't pad the characters adjacent to the \n with spaces.  If we did this
	       in the above example, we would see

	WRONG WAY -- spaces before and after the \n
	EX:  L"Clears all the mercs' positions, \n and allows you to re-enter them manually."

	Would appear as: (the second line is moved in a character)

				Clears all the mercs' positions,
 				 and allows you to re-enter them manually.


@@@ NOTATION
************

	Throughout the text files, you'll find an assortment of comments.  Comments are used to describe the
	text to make translation easier, but comments don't need to be translated.  A good thing is to search for
	"@@@" after receiving new version of the text file, and address the special notes in this manner.

!!! NOTATION
************

	As described above, the "!!!" notation should be used by you to ask questions and address problems as
	SirTech uses the "@@@" notation.

*/

STR16 pCreditsJA2113[] =
{
	L"@T,{;JA2 v1.13 Development Team",
	L"@T,C144,R134,{;Coding",
	L"@T,C144,R134,{;Graphics and Sounds",
	L"@};(Various other mods!)",
	L"@T,C144,R134,{;Items",
	L"@T,C144,R134,{;Other Contributors",
	L"@};(All other community members who contributed input and feedback!)",
};

CHAR16 ItemNames[MAXITEMS][80] =
{
	L"",
};


CHAR16 ShortItemNames[MAXITEMS][80] =
{
	L"",
};

// Different weapon calibres
// CAWS is Close Assault Weapon System and should probably be left as it is
// NATO is the North Atlantic Treaty Organization
// WP is Warsaw Pact
// cal is an abbreviation for calibre
CHAR16 AmmoCaliber[MAXITEMS][20];// =
//{
//	L"0",
//	L".38 cal",
//	L"9mm",
//	L".45 cal",
//	L".357 cal",
//	L"12 gauge",
//	L"CAWS",
//	L"5.45mm",
//	L"5.56mm",
//	L"7.62mm NATO",
//	L"7.62mm WP",
//	L"4.7mm",
//	L"5.7mm",
//	L"Monster",
//	L"Rocket",
//	L"", // dart
//	L"", // flame
//	L".50 cal", // barrett
//	L"9mm Hvy", // Val silent
//};

// This BobbyRayAmmoCaliber is virtually the same as AmmoCaliber however the bobby version doesnt have as much room for the words.
//
// Different weapon calibres
// CAWS is Close Assault Weapon System and should probably be left as it is
// NATO is the North Atlantic Treaty Organization
// WP is Warsaw Pact
// cal is an abbreviation for calibre
CHAR16 BobbyRayAmmoCaliber[MAXITEMS][20] ;//=
//{
//	L"0",
//	L".38 cal",
//	L"9mm",
//	L".45 cal",
//	L".357 cal",
//	L"12 gauge",
//	L"CAWS",
//	L"5.45mm",
//	L"5.56mm",
//	L"7.62mm N.",
//	L"7.62mm WP",
//	L"4.7mm",
//	L"5.7mm",
//	L"Monster",
//	L"Rocket",
//	L"dart", // dart
//	L"", // flamethrower
//	L".50 cal", // barrett
//	L"9mm Hvy", // Val silent
//};


CHAR16 WeaponType[MAXITEMS][30] =
{
	L"其他",
	L"手枪",
	L"自动手枪",
	L"冲锋枪",
	L"步枪",
	L"狙击步枪",
	L"突击步枪",
	L"轻机枪",
	L"霰弹枪",
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"玩家的回合",
	L"敌军的回合",
	L"异形的回合",
	L"民兵的回合",
	L"平民的回合",
	L"玩家部署",
	L"#1 客户端",
	L"#2 客户端",
	L"#3 客户端",
	L"#4 客户端",

};

CHAR16 Message[][STRING_LENGTH] =
{
	L"",

	// In the following 8 strings, the %s is the merc's name, and the %d (if any) is a number.

	L"%s被射中了头部，并且失去了1点智慧！",
	L"%s被射中了肩部，并且失去了1点灵巧！",
	L"%s被射中了胸膛，并且失去了1点力量！",
	L"%s被射中了腿部，并且失去了1点敏捷！",
	L"%s被射中了头部，并且失去了%d点智慧！",
	L"%s被射中了肩部，并且失去了%d点灵巧！",
	L"%s被射中了胸膛，并且失去了%d点力量！",
	L"%s被射中了腿部，并且失去了%d点敏捷！",
	L"中断！",

	// The first %s is a merc's name, the second is a string from pNoiseVolStr,
	// the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

	L"", //OBSOLETE
	L"你的援军到达了！",

	// In the following four lines, all %s's are merc names

	L"%s装填弹药。",
	L"%s没有足够的行动点数。",
	L"%s正在进行包扎。(按任意键取消)",
	L"%s和%s正在进行包扎。(按任意键取消)",
	// the following 17 strings are used to create lists of gun advantages and disadvantages
	// (separated by commas)
	L"耐用",
	L"不耐用",
	L"容易修复",
	L"不易修复",
	L"杀伤力高",
	L"杀伤力低",
	L"射击快",
	L"射击慢",
	L"射程远",
	L"射程近",
	L"轻盈",
	L"笨重",
	L"小巧",
	L"射速高",
	L"没有点射限制",
	L"大容弹量",
	L"小容弹量",

	// In the following two lines, all %s's are merc names

	L"%s的伪装失效了。",
	L"%s的伪装被洗掉了。",

	// The first %s is a merc name and the second %s is an item name

	L"副手武器没有弹药了！",//	L"Second weapon is out of ammo!",
	L"%s偷到了%s。",		//	L"%s has stolen the %s.",

	// The %s is a merc name

	L"%s的武器不能扫射。",	//	L"%s's weapon can't burst fire.",

	L"你已经装上了该附件。",//	L"You've already got one of those attached.",
	L"组合物品？",			//	L"Merge items?",

	// Both %s's are item names

	L"你不能把%s和%s组合在一起。",

	L"无",
	L"退出子弹",
	L"附件",

	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex:  You cannot use sun goggles and you gas mask at the same time.
	L"你不能同时使用%s和%s。",

	L"请把光标选中的物品放到另一物品的任意附件格中，这样就可能合成新物品。",
	L"请把光标选中的物品放到另一物品的任意附件格中，这样就可能合成新物品。(但是这一次，该物品不相容。)",
	L"该分区的敌军尚未被肃清",
	L"你还得给%s%s",
	L"%s被射中了头部。",
	L"放弃战斗？",
	L"这个组合是永久性的。你确认要这样做吗?",
	L"%s感觉精力充沛!",
	L"%s踩到了大理石珠子，滑倒了!",
	L"%s没能从敌人手里抢到%s!",
	L"%s修复了%s。",
	L"中断: ",
	L"投降？",
	L"此人拒绝你的包扎。",
	L"我不这么认为。",
  L"要搭乘Skyrider的直升飞机, 你得先把佣兵分配到交通工具/直升飞机。",
	L"%s的时间只够给一支枪装填弹药",
	L"血猫的回合",
	L"全自动",
	L"无全自动",
	L"精确",
	L"不精确",
	L"无半自动",
	L"敌人已经没有装备可偷了！",
	L"敌人手中没有装备！",
};


// the names of the towns in the game

CHAR16 pTownNames[MAX_TOWNS][MAX_TOWN_NAME_LENGHT] =
{
	L"",
	L"Omerta",
	L"Drassen",
	L"Alma",
	L"Grumm",
	L"Tixa",
	L"Cambria",
	L"San Mona",
	L"Estoni",
	L"Orta",
	L"Balime",
	L"Meduna",
	L"Chitzena",
};

// the types of time compression. For example: is the timer paused? at normal speed, 5 minutes per second, etc.
// min is an abbreviation for minutes

STR16 sTimeStrings[] =
{
	L"暂停",
	L"普通",
	L"5分钟",
	L"30分钟",
	L"60分钟",
	L"6小时",
};


// Assignment Strings: what assignment does the merc  have right now? For example, are they on a squad, training,
// administering medical aid (doctor) or training a town. All are abbreviated. 8 letters is the longest it can be.

STR16 pAssignmentStrings[] =
{
	L"第1小队",
	L"第2小队",
	L"第3小队",
	L"第4小队",
	L"第5小队",
	L"第6小队",
	L"第7小队",
	L"第8小队",
	L"第9小队",
	L"第10小队",
	L"第11小队",
	L"第12小队",
	L"第13小队",
	L"第14小队",
	L"第15小队",
	L"第16小队",
	L"第17小队",
	L"第18小队",
	L"第19小队",
	L"第20小队",
	L"编队",// on active duty
	L"医生",// administering medical aid
	L"病人", // getting medical aid
	L"交通工具", // in a vehicle
	L"在途中",// in transit - abbreviated form
	L"修理", // repairing
	L"锻炼", // training themselves
  L"民兵", // training a town to revolt
	L"教练", // training a teammate
	L"学员", // being trained by someone else
	L"死亡", // dead
	L"无力中", // abbreviation for incapacitated
	L"战俘", // Prisoner of war - captured
	L"医院", // patient in a hospital
	L"空",	// Vehicle is empty
};


STR16 pMilitiaString[] =
{
	L"民兵", // the title of the militia box
	L"未分配的民兵", //the number of unassigned militia troops
	L"本地区有敌军存在，你无法重新分配民兵！",
};


STR16 pMilitiaButtonString[] =
{
	L"自动", // auto place the militia troops for the player
	L"完成", // done placing militia troops
};

STR16 pConditionStrings[] =
{
	L"极好", //the state of a soldier .. excellent health
	L"良好", // good health
	L"普通", // fair health
	L"受伤", // wounded health
	L"疲劳", // tired
	L"失血", // bleeding to death
	L"昏迷", // knocked out
	L"垂死", // near death
	L"死亡", // dead
};

STR16 pEpcMenuStrings[] =
{
	L"编队", // set merc on active duty
	L"病人",// set as a patient to receive medical aid
	L"交通工具", // tell merc to enter vehicle
	L"无护送", // let the escorted character go off on their own
	L"取消", // close this menu
};


// look at pAssignmentString above for comments

STR16 pPersonnelAssignmentStrings[] =
{
	L"第1小队",
	L"第2小队",
	L"第3小队",
	L"第4小队",
	L"第5小队",
	L"第6小队",
	L"第7小队",
	L"第8小队",
	L"第9小队",
	L"第10小队",
	L"第11小队",
	L"第12小队",
	L"第13小队",
	L"第14小队",
	L"第15小队",
	L"第16小队",
	L"第17小队",
	L"第18小队",
	L"第19小队",
	L"第20小队",
	L"编队",
	L"医生",
	L"病人",
	L"交通工具",
	L"在途中",
	L"修理",
	L"锻炼",
  L"训练民兵",
	L"教练",
	L"学员",
	L"死亡",
	L"无力中",
	L"战俘",
	L"医院",
	L"空",	// Vehicle is empty
};


// refer to above for comments

STR16 pLongAssignmentStrings[] =
{
	L"第1小队",
	L"第2小队",
	L"第3小队",
	L"第4小队",
	L"第5小队",
	L"第6小队",
	L"第7小队",
	L"第8小队",
	L"第9小队",
	L"第10小队",
	L"第11小队",
	L"第12小队",
	L"第13小队",
	L"第14小队",
	L"第15小队",
	L"第16小队",
	L"第17小队",
	L"第18小队",
	L"第19小队",
	L"第20小队",
	L"编队",
	L"医生",
	L"病人",
	L"交通工具",
	L"在途中",
	L"修理",
	L"练习",
  L"训练民兵",
	L"训练队友",
	L"学员",
	L"死亡",
	L"无力中",
	L"战俘",
	L"医院",// patient in a hospital
	L"空", // Vehicle is empty
};


// the contract options

STR16 pContractStrings[] =
{
	L"合同选项: ",
	L"", // a blank line, required
	L"雇佣一日",// offer merc a one day contract extension
	L"雇佣一周", // 1 week
	L"雇佣两周", // 2 week
	L"解雇",// end merc's contract
	L"取消", // stop showing this menu
};

STR16 pPOWStrings[] =
{
	L"囚禁",  //an acronym for Prisoner of War
	L"??",
};

STR16 pLongAttributeStrings[] =
{
	L"力量",
	L"灵巧",
	L"敏捷",
	L"智慧",
	L"枪法",
	L"医疗",
	L"机械",
	L"领导",
	L"爆破",
	L"级别",
};

STR16 pInvPanelTitleStrings[] =
{
	L"护甲", // the armor rating of the merc
	L"负重", // the weight the merc is carrying
	L"伪装", // the merc's camouflage rating
	L"伪装",
	L"防护",
};

STR16 pShortAttributeStrings[] =
{
	L"敏捷", // the abbreviated version of : agility
	L"灵巧", // dexterity
	L"力量", // strength
	L"领导", // leadership
	L"智慧", // wisdom
	L"级别", // experience level
	L"枪法", // marksmanship skill
	L"爆破", // explosive skill
	L"机械", // mechanical skill
	L"医疗", // medical skill
};


STR16 pUpperLeftMapScreenStrings[] =
{
	L"任务", // the mercs current assignment
	L"合同",// the contract info about the merc
	L"生命", // the health level of the current merc
	L"士气", // the morale of the current merc
	L"状态",	// the condition of the current vehicle
	L"油量",	// the fuel level of the current vehicle
};

STR16 pTrainingStrings[] =
{
	L"锻炼", // tell merc to train self
  L"民兵",// tell merc to train town
	L"教练", // tell merc to act as trainer
	L"学员", // tell merc to be train by other
};

STR16 pGuardMenuStrings[] =
{
	L"射速: ", // the allowable rate of fire for a merc who is guarding
	L" 进攻式射击", // the merc can be aggressive in their choice of fire rates
	L" 节约弹药", // conserve ammo
	L" 有保留的射击", // fire only when the merc needs to
	L"其它选择: ", // other options available to merc
	L" 可以撤退", // merc can retreat
	L" 可以寻找掩体",  // merc is allowed to seek cover
	L" 可以帮助队友", // merc can assist teammates
	L"完成", // done with this menu
	L"取消", // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by the player

STR16 pOtherGuardMenuStrings[] =
{
	L"射速: ",
	L" *进攻式射击*",
	L" *节约弹药o*",
	L" *有保留的射击*",
	L"其它选择: ",
	L" *可以撤退*",
	L" *可以看见覆盖物*",
	L" *可以帮助队友*",
	L"完成",
	L"取消",
};

STR16 pAssignMenuStrings[] =
{
	L"编队",
	L"医生",
	L"病人",
	L"交通工具",
	L"修理",
	L"训练",
	L"取消",
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"进攻", // set militia to aggresive
	L"原地坚守", // set militia to stationary
	L"撤退", // retreat militia
	L"向我靠拢", // retreat militia
	L"卧倒", // retreat militia
	L"隐蔽",
	L"全体: 进攻",
	L"全体: 原地坚守",
	L"全体: 撤退",
	L"全体: 向我靠拢",
	L"全体: 分散",
	L"全体: 卧倒",
	L"全体: 隐蔽",
	//L"All: Find items",
	L"取消", // cancel this menu
};

//STR16 pTalkToAllMenuStrings[] =
//{
//	L"Attack", // set militia to aggresive
//	L"Hold Position", // set militia to stationary
//	L"Retreat", // retreat militia
//	L"Come to me", // retreat militia
//	L"Get down", // retreat militia
//	L"Cancel", // cancel this menu
//};

STR16 pRemoveMercStrings[] =
{
	L"移除佣兵", // remove dead merc from current team
	L"取消"
};

STR16 pAttributeMenuStrings[] =
{
	L"力量",	//"Strength",
	L"灵巧",	//"Dexterity",
	L"敏捷",	//"Agility",
	L"生命",	//"Health",
	L"枪法",	//"Marksmanship",
	L"医疗",	//"Medical",
	L"机械",	//"Mechanical",
	L"领导",	//"Leadership",
	L"爆破",	//"Explosives",
	L"取消",	//"Cancel",
};

STR16 pTrainingMenuStrings[] =
{
	L"锻炼", // train yourself
	L"民兵", // train the town
	L"教练", // train your teammates
	L"学员", // be trained by an instructor
	L"取消", // cancel this menu
};


STR16 pSquadMenuStrings[] =
{
	L"第1小队",
	L"第2小队",
	L"第3小队",
	L"第4小队",
	L"第5小队",
	L"第6小队",
	L"第7小队",
	L"第8小队",
	L"第9小队",
	L"第10小队",
	L"第11小队",
	L"第12小队",
	L"第13小队",
	L"第14小队",
	L"第15小队",
	L"第16小队",
	L"第17小队",
	L"第18小队",
	L"第19小队",
	L"第20小队",
	L"取消",
};

STR16 pPersonnelTitle[] =
{
	L"人事", // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] =
{
	L"生命: ", // health of merc
	L"敏捷: ",
	L"灵巧: ",
	L"力量: ",
	L"领导: ",
	L"智慧: ",
	L"级别: ", // experience level
	L"枪法: ",
	L"机械: ",
	L"爆破: ",
	L"医疗: ",
	L"医疗保证金: ", // amount of medical deposit put down on the merc
	L"合同剩余时间: ", // cost of current contract
	L"击毙数: ", // number of kills by merc
	L"击伤数: ",// number of assists on kills by merc
	L"日薪: ", // daily cost of merc
	L"花费总数: ",// total cost of merc
	L"合同花费: ",
	L"总日数: ",// total service rendered by merc
	L"欠付佣金: ",// amount left on MERC merc to be paid
	L"命中率: ",// percentage of shots that hit target
	L"战斗次数: ", // number of battles fought
	L"受伤次数: ", // number of times merc has been wounded
	L"技能: ",
	L"没有技能",
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] =
{
	L"没有技能",
	L"开锁",
	L"徒手格斗",		//JA25: modified
	L"电子",
	L"夜战",			//JA25: modified
	L"投掷",
	L"教学",
	L"重武器",
	L"自动武器",
	L"潜行",
	L"双手武器",
	L"偷窃",
	L"武术",
	L"刀技",
	L"狙击手",
	L"伪装",					//JA25: modified
	L"伪装（城市）",
	L"伪装（沙漠）",
	L"伪装（雪地）",
	L"专家",
};


// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] =
{
	L"站立/行走 (|S)",
	L"蹲伏/蹲伏前进(|C)",
	L"站立/奔跑 (|R)",
	L"匍匐/匍匐前进(|P)",
	L"查看(|L)",
	L"行动",
	L"交谈",
	L"检查 (|C|t|r|l)",

	// Pop up door menu
	L"用手开门",
	L"检查陷阱",
	L"开锁",
	L"踢门",
	L"解除陷阱",
	L"关门",
	L"开门",
	L"使用破门炸药",
	L"使用撬棍",
	L"取消 (|E|s|c)",
	L"关闭",
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.

STR16 pDoorTrapStrings[] =
{
	L"没有陷阱",
	L"一个爆炸陷阱",
	L"一个带电陷阱",
	L"一个警报陷阱",
	L"一个无声警报陷阱",
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.

STR16 pContractExtendStrings[] =
{
	L"日",
	L"周",
	L"两周",
};

// On the map screen, there are four columns. This text is popup help text that identifies the individual columns.

STR16 pMapScreenMouseRegionHelpText[] =
{
	L"选择人物",
	L"分配任务",
	L"安排行军路线",
	L"签约 (|C)",
	L"移除佣兵",
	L"睡觉",
};

// volumes of noises

STR16 pNoiseVolStr[] =
{
	L"微弱的",
	L"清晰的",
	L"大声的",
	L"非常大声的",
};

// types of noises

STR16 pNoiseTypeStr[] = // OBSOLETE
{
	L"未知",
	L"移动声",
	L"辗扎声",
	L"溅泼声",
	L"撞击声",
	L"枪声",
	L"爆炸声",
	L"尖叫声",
	L"撞击声",
	L"撞击声",
	L"粉碎声",
	L"破碎声",
};

// Directions that are used to report noises

STR16 pDirectionStr[] =
{
	L"东北方",
	L"东方",
	L"东南方",
	L"南方",
	L"西南方",
	L"西方",
	L"西北方",
	L"北方"
};

// These are the different terrain types.

STR16 pLandTypeStrings[] =
{
	L"城市",
	L"公路",
	L"平原",
	L"沙漠",
	L"灌木",
	L"森林",
	L"沼泽",
	L"湖泊",
	L"山地",
	L"不可通行",
	L"河流",	//river from north to south
	L"河流",	//river from east to west
	L"外国",
	//NONE of the following are used for directional travel, just for the sector description.
	L"热带",
	L"农田",
	L"平原，公路",
	L"灌木，公路",
	L"农庄，公路",
	L"热带，公路",
	L"森林，公路",
	L"海滨",
	L"山地，公路",
	L"海滨，公路",
	L"沙漠，公路",
	L"沼泽，公路",
	L"灌木，SAM导弹基地",
	L"沙漠，SAM导弹基地",
	L"热带，SAM导弹基地",
	L"Meduna, SAM导弹基地",

	//These are descriptions for special sectors
	L"Cambria医院",
	L"Drassen机场",
	L"Meduna机场",
	L"SAM导弹基地",
	L"抵抗军隐蔽处",//The rebel base underground in sector A10
	L"Tixa地牢",//The basement of the Tixa Prison (J9)
	L"异形巢穴",//Any mine sector with creatures in it
	L"Orta地下室",	//The basement of Orta (K4)
	L"地道",		//The tunnel access from the maze garden in Meduna
										//leading to the secret shelter underneath the palace
	L"地下掩体",				//The shelter underneath the queen's palace
	L"",							//Unused
};

STR16 gpStrategicString[] =
{
	L"",	//Unused
	L"%s在%c%d分区被发现了，另一小队即将到达。",	//STR_DETECTED_SINGULAR
	L"%s在%c%d分区被发现了，其它几个小队即将到达。",	//STR_DETECTED_PLURAL
	L"你想调整为同时到达吗？",												//STR_COORDINATE

	//Dialog strings for enemies.

	L"敌军给你一个投降的机会。",
	L"敌军俘虏了昏迷中的佣兵。",

	//The text that goes on the autoresolve buttons

	L"撤退",		//The retreat button
	L"完成",		//The done button				//STR_AR_DONE_BUTTON

	//The headers are for the autoresolve type (MUST BE UPPERCASE)

	L"防守",							//STR_AR_DEFEND_HEADER
	L"攻击",						//STR_AR_ATTACK_HEADER
	L"遭遇战",						//STR_AR_ENCOUNTER_HEADER
	L"分区",	//The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

	//The battle ending conditions

	L"胜利！",								//STR_AR_OVER_VICTORY
	L"失败！",								//STR_AR_OVER_DEFEAT
	L"投降！",							//STR_AR_OVER_SURRENDERED
	L"被俘！",								//STR_AR_OVER_CAPTURED
	L"撤退！",								//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"民兵",							//STR_AR_MILITIA_NAME,
	L"精兵",							//STR_AR_ELITE_NAME,
	L"部队",							//STR_AR_TROOP_NAME,
	L"行政人员",								//STR_AR_ADMINISTRATOR_NAME,
	L"异形",								//STR_AR_CREATURE_NAME,

	//Label for the length of time the battle took

	L"战斗用时",						//STR_AR_TIME_ELAPSED,

	//Labels for status of merc if retreating.  (UPPERCASE)

	L"已撤退",						//STR_AR_MERC_RETREATED,
	L"正在撤退",						//STR_AR_MERC_RETREATING,
	L"撤退",						//STR_AR_MERC_RETREAT,

	//PRE BATTLE INTERFACE STRINGS
	//Goes on the three buttons in the prebattle interface.  The Auto resolve button represents
	//a system that automatically resolves the Combat for the player without having to do anything.
	//These strings must be short (two lines -- 6-8 chars per line)

	L"自动战斗",							//STR_PB_AUTORESOLVE_BTN,
	L"进入战区",							//STR_PB_GOTOSECTOR_BTN,
	L"撤退佣兵",							//STR_PB_RETREATMERCS_BTN,

	//The different headers(titles) for the prebattle interface.
	L"遭遇敌军",						//STR_PB_ENEMYENCOUNTER_HEADER,
	L"敌军入侵",						//STR_PB_ENEMYINVASION_HEADER, // 30
	L"敌军伏击",
	L"进入敌占区",			//STR_PB_ENTERINGENEMYSECTOR_HEADER
	L"异形攻击",					//STR_PB_CREATUREATTACK_HEADER
	L"血猫伏击",					//STR_PB_BLOODCATAMBUSH_HEADER
	L"进入血猫巢穴",

	//Various single words for direct translation.  The Civilians represent the civilian
	//militia occupying the sector being attacked.  Limited to 9-10 chars

	L"地区",
	L"敌军",
	L"佣兵",
	L"民兵",
	L"异形",
	L"血猫",
	L"分区",
	L"无人",	//If there are no uninvolved mercs in this fight.
	L"N/A",			//Acronym of Not Applicable
	L"日",			//One letter abbreviation of day
	L"小时",			//One letter abbreviation of hour

	//TACTICAL PLACEMENT USER INTERFACE STRINGS
	//The four buttons

	L"清除",
	L"分散",
	L"集中",
	L"完成",

	//The help text for the four buttons.  Use \n to denote new line (just like enter).

	L"清除所有佣兵的布阵，然后你可以\n重新手动对他们进行安排。(|c)",
	L"每次按本按钮，就会重新 \n随机分散地布阵佣兵。(|s)",
	L"你可以选择你想集中地布阵佣兵的地方。(|g)",
	L"当你完成对佣兵布阵后，\n请按本按钮。 (|E|n|t|e|r)",
	L"开始战斗前，你必须\n对所有佣兵完成布阵。",

	//Various strings (translate word for word)

	L"分区",
	L"选择进入的位置",

	//Strings used for various popup message boxes.  Can be as long as desired.

	L"看起来不太好。无法进入这里。换个不同的位置吧。",
	L"请把佣兵放在地图的高亮分区里。",

	//This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"已到达该地区",

	//These entries are for button popup help text for the prebattle interface.  All popup help
	//text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
	L"自动解决战斗，不需要\n载入该分区地图。",
	L"当玩家在攻击时，无法使用\n自动战斗功能。",
	L"进入该分区和敌军作战(|E)",
	L"将小队撤退到先前的分区。(|R)",				//singular version
	L"将所有小队撤退到先前的分区。(|R)", //multiple groups with same previous sector

	//various popup messages for battle conditions.

	//%c%d is the sector -- ex:  A9
	L"敌军向你在%c%d分区的民兵发起了攻击。",
	//%c%d is the sector -- ex:  A9
	L"异形向你在%c%d分区的民兵发起了攻击。",
	//1st %d refers to the number of civilians eaten by monsters,  %c%d is the sector -- ex:  A9
	//Note:  the minimum number of civilians eaten will be two.
	L"异形攻击了分区%s，吃掉了%d名平民。",
	//%s is the sector location -- ex:  A9: Omerta
	L"敌军向你在分区%s的佣兵发起了攻击。你的佣兵中没人能进行战斗。",
	//%s is the sector location -- ex:  A9: Omerta
	L"异形向你在分区%s的佣兵发起了攻击。你的佣兵中没人能进行战斗。",

};

STR16 gpGameClockString[] =
{
	//This is the day represented in the game clock.  Must be very short, 4 characters max.
	L"日",
};

//When the merc finds a key, they can get a description of it which
//tells them where and when they found it.
STR16 sKeyDescriptionStrings[2] =
{
	L"找到钥匙的分区: ",
	L"找到钥匙的日期: ",
};

//The headers used to describe various weapon statistics.

CHAR16		gWeaponStatsDesc[][ 16 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"状态: ",
	L"重量 (%s): ",
	L"AP Costs",
	L"射程: ",		// Range
	L"杀伤力: ",		// Damage
	L"弹药", 	// Number of bullets left in a magazine
	L"AP: ",			// abbreviation for Action Points
	L"=",
	L"=",
					//Lal: additional strings for tooltips
	L"准确性: ", //9
	L"射程: ", //10
	L"杀伤力: ", //11
	L"重量: ", //12
	L"晕眩杀伤力: ",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	L"连发惩罚: ",	//14
	L"连发/5AP: ",		//15
	L"数量: ",		//16

};

// HEADROCK: Several arrays of tooltip text for new Extended Description Box
// Please note, several of these are artificially inflated to 19 entries to help fix a complication with
// changing item type while watching its description box
STR16		gzWeaponStatsFasthelp[ 27 ] =
{
	L"射程",
	L"杀伤",
	L"点射/自动惩罚",
	L"连发数量/5AP",
	L"精度",
	L"可靠性",
	L"修理难度",
	L"举枪AP",
	L"单发AP",
	L"点射AP",
	L"连发AP",	//10
	L"上弹AP",
	L"手动上弹AP",
	L"消焰",
	L"噪音（越低越好）",
	L"命中修正",
	L"平均最佳激光瞄准距离",
	L"精瞄修正",
	L"精瞄加成所需最小距离",
	L"两角架修正",
	L"投掷AP",	//20
	L"发射AP",
	L"捅人AP",
	L"无法单发！",
	L"无点射模式！",
	L"无连发模式！",
	L"格斗AP",
};

STR16		gzWeaponStatsFasthelpTactical[ 27 ] =
{
	L"射程",
	L"杀伤",
	L"点射/连发惩罚",
	L"连发数量/5AP",
	L"精度",
	L"可靠性",
	L"修理难度",
	L"举枪所需AP",
	L"单发所需AP",
	L"点射所需AP",
	L"连发所需AP",	//10
	L"上弹所需AP",
	L"手动上弹所需AP",
	L"命中修正",
	L"平均最佳激光瞄准距离",
	L"精瞄修正",
	L"精瞄加成所需最小距离",
	L"消焰",
	L"噪音（越低越好）",
	L"脚架修正",
	L"投掷所需AP",	//20
	L"发射所需AP",
	L"捅人所需AP",
	L"无法单发！",
	L"无点射模式！",
	L"无连发模式！",
	L"击打所需AP",
};

STR16		gzAmmoStatsFasthelp[ 20 ] =
{
	L"侵彻力（越低越好）",
	L"子弹翻滚力（越高越好）",
	L"初期爆炸冲击力（越高越好）",
	L"曳光弹效果",
	L"反坦克",
	L"开锁弹",
	L"忽视防御",
	L"酸性腐蚀",
	L"射程修正",
	L"杀伤修正",
	L"命中修正",
	L"点射/连发惩罚修正（越高越好）",
	L"可靠性修正",
	L"噪音修正（越低越好）",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
};

STR16		gzArmorStatsFasthelp[ 20 ] =
{
	L"防御",
	L"覆盖率（越高越好）",
	L"损坏率（越低越好）",
	L"AP修正",
	L"命中修正",
	L"林地伪装",
	L"城市伪装",
	L"沙漠伪装",
	L"雪地伪装",
	L"潜行修正",
	L"视距修正",
	L"白天视距修正",
	L"夜晚视距修正",
	L"亮光下视距修正",
	L"洞穴视距修正",
	L"视野狭窄百分比",
	L"听力范围修正",
	L"",
	L"",
	L"",
};

STR16		gzExplosiveStatsFasthelp[ 20 ] =
{
	L"杀伤",
	L"眩晕力度",
	L"爆炸噪音（越低越好）",
	L"挥发性!!!（越低越好）",
	L"爆炸范围",
	L"初始范围",
	L"最终范围 ",
	L"效果持续时间",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
};

STR16		gzMiscItemStatsFasthelp[ 34 ] =
{
	L"物品大小修正（越低越好）", // 0
	L"可靠性修正",
	L"噪音修正（越低越好）",
	L"枪口消焰",
	L"脚架修正",
	L"射程修正", // 5
	L"命中率修正",
	L"最佳激光瞄准距离",
	L"精瞄加成修正",
	L"点射长度修正",
	L"点射惩罚修正（越高越好）", // 10
	L"连发惩罚修正（越高越好）",
	L"AP修正",
	L"点射AP修正（越低越好）",
	L"连发AP修正（越低越好）",
	L"举枪AP修正（越低越好）", // 15
	L"上弹AP修正（越低越好）",
	L"弹容量修正",
	L"攻击AP修正（越低越好）",
	L"杀伤修正",
	L"近战杀伤修正", // 20
	L"丛林迷彩",
	L"城市迷彩",
	L"沙漠迷彩",
	L"雪地迷彩",
	L"潜行修正", // 25
	L"听觉距离修正",
	L"视距修正",
	L"白天视距修正",
	L"夜晚视距修正",
	L"亮光下视距修正", //30
	L"洞穴视距修正",
	L"隧道视野百分比（越低越好）",
	L"精瞄加成所需最小距离",
};

// HEADROCK: End new tooltip text

//The headers used for the merc's money.

CHAR16 gMoneyStatsDesc[][ 13 ] =
{
	L"剩余",
	L"金额: ",//this is the overall balance
	L"分割",
	L"金额: ", // the amount he wants to separate from the overall balance to get two piles of money

	L"当前",
	L"余额",
	L"提取",
	L"金额",
};

//The health of various creatures, enemies, characters in the game. The numbers following each are for comment
//only, but represent the precentage of points remaining.

CHAR16 zHealthStr[][13] =
{
	L"垂死",	//"DYING",		//	>= 0
	L"濒危",	//"CRITICAL", 		//	>= 15
	L"虚弱",	//"POOR",		//	>= 30
	L"受伤",	//"WOUNDED",    	//	>= 45
	L"健康",	//"HEALTHY",    	//	>= 60
	L"强壮",	//"STRONG",     	// 	>= 75
  L"极好",	//"EXCELLENT",		// 	>= 90
};

STR16	gzMoneyAmounts[6] =
{
	L"$1000",
	L"$100",
	L"$10",
	L"完成",
	L"分割",
	L"提取",
};

// short words meaning "Advantages" for "Pros" and "Disadvantages" for "Cons."
CHAR16		gzProsLabel[10] =
{
	L"优点: ",
};

CHAR16		gzConsLabel[10] =
{
	L"缺点: ",
};

//Conversation options a player has when encountering an NPC
CHAR16 zTalkMenuStrings[6][ SMALL_STRING_LENGTH ] =
{
	L"再说一次？",	//meaning "Repeat yourself"
	L"友好",		//approach in a friendly
	L"直率",		//approach directly - let's get down to business
	L"恐吓",		//approach threateningly - talk now, or I'll blow your face off
	L"给予",
	L"招募",
};

//Some NPCs buy, sell or repair items. These different options are available for those NPCs as well.
CHAR16 zDealerStrings[4][ SMALL_STRING_LENGTH ]=
{
	L"买/卖",
	L"买",
	L"卖",
	L"修理",
};

CHAR16 zDialogActions[1][ SMALL_STRING_LENGTH ] =
{
	L"完成",
};


//These are vehicles in the game.

STR16 pVehicleStrings[] =
{
 L"凯迪拉克",
 L"悍马",	// a hummer jeep/truck -- military vehicle
 L"冰激凌车",
 L"吉普",
 L"坦克",
 L"直升飞机",
};

STR16 pShortVehicleStrings[] =
{
	L"凯迪拉克",
	L"悍马",				// the HMVV
	L"冰激凌车",
	L"吉普",
	L"坦克",
	L"直升飞机", 				// the helicopter
};

STR16	zVehicleName[] =
{
	L"凯迪拉克",
	L"悍马",			//a military jeep. This is a brand name.
	L"冰激凌车",			// Ice cream truck
	L"吉普",
	L"坦克",
	L"直升飞机", 		//an abbreviation for Helicopter
};


//These are messages Used in the Tactical Screen

CHAR16 TacticalStr[][ MED_STRING_LENGTH ] =
{
	L"空袭",
	L"自动包扎?",

	// CAMFIELD NUKE THIS and add quote #66.

	L"%s发现运来的货品短缺了几件。",

	// The %s is a string from pDoorTrapStrings

	L"锁上有%s。",
	L"没有上锁。",
	L"成功！",
	L"失败。",
	L"成功！",
	L"失败",
	L"锁上没有被设置陷阱。",
	L"成功！",
	// The %s is a merc name
	L"%s没有对应的钥匙。",
	L"锁上的陷阱被解除了。",
	L"锁上没有被设置陷阱。",
	L"锁住了。",
	L"门",
	L"有陷阱的",
	L"锁住的",
	L"没锁的",
	L"被打烂的",
	L"这里有一个开关。启动它吗？",
	L"解除陷阱？",
	L"上一个...",
	L"下一个...",
	L"更多的...",

	// In the next 2 strings, %s is an item name

	L"%s被放在地上了。",
	L"%s被交给%s了。",

	// In the next 2 strings, %s is a name

	L"%s已经被完全支付。",
	L"%s还拖欠%d。",
	L"选择引爆的频率", //in this case, frequency refers to a radio signal
	L"设定几个回合后爆炸: ", //how much time, in turns, until the bomb blows
	L"设定遥控雷管的频率: ",//in this case, frequency refers to a radio signal
	L"解除诡雷?",
	L"移掉蓝旗？",
	L"在这里插上蓝旗吗？",
	L"结束回合",

	// In the next string, %s is a name. Stance refers to way they are standing.

	L"你确定要攻击%s吗？",
	L"车辆无法变动姿势。",
	L"机器人无法变动姿势。",

	// In the next 3 strings, %s is a name

	L"%s无法在这里变为该姿势。",
	L"%s无法在这里被包扎。",
	L"%s不需要包扎。",
	L"不能移动到那儿。",
	L"你的队伍已经满员了。没有空位雇佣新队员。",	//there's no room for a recruit on the player's team

	// In the next string, %s is a name

	L"%s已经被招募。",

	// Here %s is a name and %d is a number

	L"尚拖欠%s$%d.",

	// In the next string, %s is a name

	L"护送%s吗?",

	// In the next string, the first %s is a name and the second %s is an amount of money (including $ sign)

	L"要雇佣%s吗(每日得支付%s)?",

	// This line is used repeatedly to ask player if they wish to participate in a boxing match.

	L"你要进行拳击比赛吗?",

	// In the next string, the first %s is an item name and the
	// second %s is an amount of money (including $ sign)

	L"要买%s吗(得支付%s)?",

	// In the next string, %s is a name

	L"%s接受第%d小队的护送。",

	// These messages are displayed during play to alert the player to a particular situation

	L"卡壳",					//weapon is jammed.
	L"机器人需要%s口径的子弹。",	//Robot is out of ammo
	L"扔到那儿？那不可能。",		//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"潜行模式 (|Z)",
	L"地图屏幕 (|M)",
	L"结束回合 (|D)",
	L"谈话",
	L"禁音",
	L"起身 (|P|g|U|p)",
	L"光标层次 (|T|a|b)",
	L"攀爬/跳跃",
	L"伏下 (|P|g|D|n)",
	L"检查",
	L"上一个佣兵",
	L"下一个佣兵 (|S|p|a|c|e)",
	L"选项 (|O)",
	L"扫射模式 (|B)",
	L"查看/转向(|L)",
	L"生命: %d/%d\n精力: %d/%d\n士气: %s",
	L"厄?",		//this means "what?"
	L"继续",		//an abbrieviation for "Continued"
	L"对%s关闭禁音模式。",
	L"对%s打开禁音模式。",
	L"耐久度: %d/%d\n油量: %d/%d",
	L"下车",
	L"切换小队 ( |S|h|i|f|t |S|p|a|c|e )",
	L"驾驶",
	L"N/A",		//this is an acronym for "Not Applicable."
	L"使用 (拳头)",
	L"使用 (武器)",
	L"使用 (刀具)",
	L"使用 (爆炸品)",
	L"使用 (医疗用品)",
	L"(抓住)",
	L"(装填弹药)",
	L"(给予)",
	L"%s被触发了。",
	L"%s已到达。",
	L"%s用完了行动点数(AP)。",
	L"%s无法行动。",
	L"%s包扎好了。",
	L"%s用完了绷带。",
	L"这个分区中有敌军。",
	L"视野中没有敌军。",
	L"没有足够的行动点数(AP)。",
	L"没人使用遥控器。",
	L"扫射光了子弹!",
	L"敌兵",
	L"异形",
	L"民兵",
	L"平民",
	L"离开分区",
	L"确定",
	L"取消",
	L"选择佣兵",
	L"小队的所有佣兵",
	L"前往分区",
	L"前往地图",
	L"你不能从这边离开这个分区。",
	L"%s太远了。",
	L"不显示树冠",
	L"显示树冠",
	L"乌鸦"	,				//Crow, as in the large black bird
	L"颈部",
	L"头部",
	L"躯体",
	L"腿部",
	L"要告诉女王她想知道的情报吗？",
	L"获得指纹ID",
	L"指纹ID无效。无法使用该武器。",
	L"达成目标",
	L"路被堵住了",
	L"存钱/取钱",	//Help text over the $ button on the Single Merc Panel
	L"没人需要包扎。",
	L"卡壳",											// Short form of JAMMED, for small inv slots
	L"无法到达那里。", // used ( now ) for when we click on a cliff
	L"路被堵住了。你要和这个人交换位置吗?",
	L"那人拒绝移动。",
	// In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
	L"你同意支付%s吗？",
	L"你要接受免费治疗吗?",
	L"你同意让佣兵和Daryl结婚吗?",
	L"钥匙面板",
	L"你不能这样用EPC。",
	L"不杀Krott?",
	L"超出武器的有效射程。",
	L"矿工",
	L"车辆只能在分区间旅行",
	L"现在不能自动包扎",
	L"%s被堵住了",
	L"被Deidranna的军队俘虏的佣兵，被关押在这里！",
	L"锁被击中了",
	L"锁被破坏了",
	L"其他人在使用这扇门。",
	L"耐久度: %d/%d\n油量: %d/%d",
	L"%s看不见%s。", // Cannot see person trying to talk to
	L"附件被移除",
	L"你已经有了两辆车，无法拥有更多的车辆。",
};

//Varying helptext explains (for the "Go to Sector/Map" checkbox) what will happen given different circumstances in the "exiting sector" interface.
STR16 pExitingSectorHelpText[] =
{
	//Helptext for the "Go to Sector" checkbox button, that explains what will happen when the box is checked.
	L"如果勾中，将立即进入邻近的分区。",
	L"如果勾中，你将被立即自动放置在地图屏幕，\n因为你的佣兵要花些时间来行军。",

	//If you attempt to leave a sector when you have multiple squads in a hostile sector.
	L"该分区被敌军占据。你不能将佣兵留在这里。\n在进入其它分区前，你必须把这里的问题解决。",

	//Because you only have one squad in the sector, and the "move all" option is checked, the "go to sector" option is locked to on.
	//The helptext explains why it is locked.
	L"让留下的佣兵离开本分区，\n将立即进入邻近的分区。",
	L"让留下的佣兵离开本分区，\n你将被立即自动放置在地图屏幕，\n因为你的佣兵要花些时间来行军。",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted.  The "single" button is disabled.
	L"%s需要被你的佣兵护送，他（她）无法独自离开本分区。",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s无法独自离开本分区，因为他得护送%s。", //male singular
	L"%s无法独自离开本分区，因为她得护送%s。", //female singular
	L"%s无法独自离开本分区，因为他得护送多人。", //male plural
	L"%s无法独自离开本分区，因为她得护送多人。", //female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the  "move all" option is disabled,
	//and this helptext explains why.
	L"如果要让小队在分区间移动的话，\n你的全部队员都必须在附近。",

	L"", //UNUSED

	//Standard helptext for single movement.  Explains what will happen (splitting the squad)
	L"如果勾中， %s将独自行军，\n而且被自动重新分配到一个单独的小队中。",

	//Standard helptext for all movement.  Explains what will happen (moving the squad)
	L"如果勾中，你当前选中的小队\n将会离开本分区，开始行军。",

	//This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear.  This is just like the situation where
	//This string is special, as it is not used as helptext.  Do not use the special newline character (\n) for this string.
	L"%s正在被你的佣兵护送，他（她）无法独自离开本分区。你的佣兵必须在附近以护送他（她）离开。",
};



STR16 pRepairStrings[] =
{
	L"物品",	// tell merc to repair items in inventor
	L"SAM导弹基地",		// tell merc to repair SAM site - SAM is an acronym for Surface to Air Missile
	L"取消",		// cancel this menu
	L"机器人",		// repair the robot
};


// NOTE: combine prestatbuildstring with statgain to get a line like the example below.
// "John has gained 3 points of marksmanship skill."

STR16 sPreStatBuildString[] =
{
	L"丧失",// the merc has lost a statistic
	L"获得",// the merc has gained a statistic
	L"点",// singular
	L"点",// plural
	L"级",//singular
	L"级",//plural
};

STR16 sStatGainStrings[] =
{
	L"生命。",
	L"敏捷。",
	L"灵巧。",
	L"智慧。",
	L"医疗技能。",
	L"爆破技能。",
	L"机械技能。",
	L"枪法技能。",
	L"级别",
	L"力量",
	L"领导",
};


STR16 pHelicopterEtaStrings[] =
{
	L"总距离: ", 			// total distance for helicopter to travel
	L"安全: ", 			// distance to travel to destination
	L"不安全: ",		// distance to return from destination to airport
	L"总价: ", 		// total cost of trip by helicopter
	L"耗时: ", 			// ETA is an acronym for "estimated time of arrival"
	L"直升机油量不够，必须在敌占区着陆。", 	// warning that the sector the helicopter is going to use for refueling is under enemy control ->
  L"乘客: ",
  L"选择Skyrider还是“着陆点”？",
  L"Skyrider",
  L"着陆点",
};

STR16 sMapLevelString[] =
{
	L"地层: ", 			// what level below the ground is the player viewing in mapscreen
};

STR16 gsLoyalString[] =
{
	L"忠诚度: ",	// the loyalty rating of a town ie : Loyal 53%
};


// error message for when player is trying to give a merc a travel order while he's underground.

STR16 gsUndergroundString[] =
{
	L"不能在地底下达行军命令。",
};

STR16 gsTimeStrings[] =
{
	L"小时",				// hours abbreviation
	L"分钟",				// minutes abbreviation
	L"秒",				// seconds abbreviation
	L"日",				// days abbreviation
};

// text for the various facilities in the sector

STR16 sFacilitiesStrings[] =
{
	L"无",
	L"医院",
	L"工厂",
	L"监狱",
	L"军事基地",
	L"机场",
	L"靶场",	// a field for soldiers to practise their shooting skills
};

// text for inventory pop up button

STR16 pMapPopUpInventoryText[] =
{
	L"存货",
	L"离开",
};

// town strings

STR16 pwTownInfoStrings[] =
{
	L"大小",				// 0 // size of the town in sectors
	L"", 					// blank line, required
	L"占领度",				// how much of town is controlled
	L"无",				// none of this town
	L"矿区",			// mine associated with this town
	L"忠诚度",				// 5 // the loyalty level of this town
	L"民兵",				// the forces in the town trained by the player
	L"",
	L"主要设施",			// main facilities in this town
	L"等级",			// the training level of civilians in this town
	L"民兵训练度",		// 10 // state of civilian training in town
	L"民兵",			// the state of the trained civilians in the town
};

// Mine strings

STR16 pwMineStrings[] =
{
	L"矿井",			// 0
	L"银块",
	L"金块",
	L"当前日产量",
	L"最高产量",
	L"废弃",				// 5
	L"关闭",
	L"矿脉耗尽",
	L"生产",
	L"状态",
	L"生产率",
	L"矿石类型",			// 10
	L"占领度",
	L"忠诚度",
//	L"在岗矿工",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"敌军",
	L"分区",
	L"物品数量",
	L"未知",
	L"已占领",
	L"是",
	L"否",
};

// error strings for inventory

STR16 pMapInventoryErrorString[] =
{
	L"%s不够近。",	//Merc is in sector with item but not close enough
	L"无法选择该佣兵。",  //MARK CARTER
	L"%s不在这个分区，不能拿到这个物品。",
	L"在战斗时，你只能动手捡起物品。",
	L"在战斗时，你只能动手放下物品。",
	L"%s不在该分区，不能放下那个物品。",
	L"在战斗时你没有时间开启成箱的弹药。",
};

STR16 pMapInventoryStrings[] =
{
	L"位置",		// sector these items are in
	L"物品总数",	// total number of items in sector
};


// help text for the user

STR16 pMapScreenFastHelpTextList[] =
{
	L"如果要改变佣兵的分配任务，比如分到另一个小队、当医生、进行修理等，请按 '任务' 栏。",
	L"要让佣兵以另一个分区为行军目标，请按'Dest'栏。",
	L"一旦对佣兵下达了行军命令 ，请按时间压缩按钮以让他们开始行进。",
	L"鼠标左击以选择该分区。再次鼠标左击以对佣兵下达行军命令, 或者鼠标右击以获取分区信息小结。",
	L"任何时候在该屏幕下都可以按'h'键，以弹出帮助窗口。",
	L"测试文本",
	L"测试文本",
	L"测试文本",
	L"测试文本",
	L"您尚未开始Arulco之旅，现在在这个屏幕上您无事可做。当您把队员都雇佣好后，请左击右下方的“时间压缩”按钮。这样在您的队伍到达Arulco前，时间就前进了。",
};

// movement menu text

STR16 pMovementMenuStrings[] =
{
	L"调动佣兵", 	// title for movement box
	L"安排行军路线",	// done with movement menu, start plotting movement
	L"取消",		// cancel this menu
	L"其它",		// title for group of mercs not on squads nor in vehicles
};


STR16 pUpdateMercStrings[] =
{
	L"糟了: ", 			// an error has occured
	L"佣兵合同到期了: ", 	// this pop up came up due to a merc contract ending
	L"佣兵完成了分配的任务: ", // this pop up....due to more than one merc finishing assignments
	L"佣兵醒来了，继续干活: ", // this pop up ....due to more than one merc waking up and returing to work
	L"佣兵困倦了: ", // this pop up ....due to more than one merc being tired and going to sleep
	L"合同快到期了: ", 	// this pop up came up due to a merc contract ending
};

// map screen map border buttons help text

STR16 pMapScreenBorderButtonHelpText[] =
{
	L"显示城镇 (|W)",
	L"显示矿井 (|M)",
	L"显示队伍和敌人 (|T)",
	L"显示领空 (|A)",
	L"显示物品 (|I)",
	L"显示民兵和敌人 (|Z)",
};


STR16 pMapScreenBottomFastHelp[] =
{
	L"笔记本电脑 (|L)",
	L"战术屏幕 (|E|s|c)",
	L"选项 (|O)",
	L"时间压缩 (|+)", 	// time compress more
	L"时间压缩 (|-)", 	// time compress less
	L"上一信息 (|U|p)\n上一页 (|P|g|U|p)", // previous message in scrollable list
	L"下一信息 (|D|o|w|n)\n下一页 (|P|g|D|n)", 	// next message in the scrollable list
	L"开始/停止时间压缩 (|S|p|a|c|e)",	//"Start/Stop Time (|S|p|a|c|e)",	// start/stop time compression
};

STR16 pMapScreenBottomText[] =
{
	L"帐户余额", 	// current balance in player bank account
};

STR16 pMercDeadString[] =
{
	L"%s死了。",
};


STR16 pDayStrings[] =
{
	L"日",
};

// the list of email sender names

STR16 pSenderNameList[] =
{
	L"Enrico",
	L"Psych Pro Inc",
	L"桌面帮助",
	L"Psych Pro Inc",
	L"Speck",
	L"R.I.S.",		//5
	L"Barry",
	L"Blood",
	L"Lynx",
	L"Grizzly",
	L"Vicki",			//10
	L"Trevor",
	L"Grunty",
	L"Ivan",
	L"Steroid",
	L"Igor",			//15
	L"Shadow",
	L"Red",
	L"Reaper",
	L"Fidel",
	L"Fox",				//20
	L"Sidney",
	L"Gus",
	L"Buns",
	L"Ice",
	L"Spider",		//25
	L"Cliff",
	L"Bull",
	L"Hitman",
	L"Buzz",
	L"Raider",		//30
	L"Raven",
	L"Static",
	L"Len",
	L"Danny",
	L"Magic",
	L"Stephen",
	L"Scully",
	L"Malice",
	L"Dr.Q",
	L"Nails",
	L"Thor",
	L"Scope",
	L"Wolf",
	L"MD",
	L"Meltdown",
	//----------
	L"M.I.S. 保险公司",
	L"Bobby Rays",
	L"Kingpin",
	L"John Kulba",
	L"A.I.M.",
};


// next/prev strings

STR16 pTraverseStrings[] =
{
  L"上一个",
  L"下一个",
};

// new mail notify string

STR16 pNewMailStrings[] =
{
 L"你有新的邮件...",
};


// confirm player's intent to delete messages

STR16 pDeleteMailStrings[] =
{
  L"删除邮件？",
  L"删除未读的邮件？",
};


// the sort header strings

STR16 pEmailHeaders[] =
{
	L"来自: ",
	L"标题: ",
	L"日期: ",
};

// email titlebar text

STR16 pEmailTitleText[] =
{
	L"邮箱",
};


// the financial screen strings
STR16 pFinanceTitle[] =
{
	L"帐簿",	//the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] =
{
	L"收入: ", 				// credit (subtract from) to player's account
	L"支出: ",				// debit (add to) to player's account
	L"昨日实际收入: ",
	L"昨日其它存款: ",
	L"昨日支出: ",
	L"昨日日终余额: ",
	L"今日实际收入: ",
	L"今日其它存款: ",
	L"今日支出: ",
	L"今日当前余额: ",
	L"预期收入: ",
	L"明日预计余额: ",		// projected balance for player for tommorow
};


// headers to each list in financial screen

STR16 pFinanceHeaders[] =
{
  L"天数",			// the day column
	L"收入", 			// the credits column (to ADD money to your account)
	L"支出",				// the debits column (to SUBTRACT money from your account)
	L"交易记录", // transaction type - see TransactionText below
	L"余额",		// balance at this point in time
	L"页数",				// page number
	L"日", 		// the day(s) of transactions this page displays
};


STR16 pTransactionText[] =
{
	L"自然增值利息",		// interest the player has accumulated so far
	L"匿名存款",
	L"交易费用",
	L"已雇佣",			// Merc was hired
	L"在Bobby Ray购买货品", // Bobby Ray is the name of an arms dealer
	L"在M.E.R.C开户。",
	L"%s的医疗保证金",  	// medical deposit for merc
	L"IMP心理剖析分析", 		// IMP is the acronym for International Mercenary Profiling
	L"为%s购买保险",
	L"缩短%s的保险期限",
	L"延长%s的保险期限",			// johnny contract extended
	L"取消%s的保险",
	L"%s的保险索赔",	// insurance claim for merc
	L"1日", 				// merc's contract extended for a day
	L"1周",				// merc's contract extended for a week
	L"2周",				// ... for 2 weeks
	L"采矿收入",
	L"", //String nuked
	L"买花",
	L"%s的医疗保证金的全额退款",
	L"%s的医疗保证金的部分退款",
	L"%s的医疗保证金没有退款",
	L"付给%s金钱",// %s is the name of the npc being paid
	L"支付给%s的佣金", 	// transfer funds to a merc
	L"%s退回的佣金",	 // transfer funds from a merc
	L"在%s训练民兵",	 // initial cost to equip a town's militia
	L"向%s购买了物品。", //is used for the Shop keeper interface.  The dealers name will be appended to the end of the string.
	L"%s存款",
	L"将装备卖给了当地人",
};

STR16 pTransactionAlternateText[] =
{
	L"的保险",  		// insurance for a merc
	L"延长%s的合同一日。",	// entend mercs contract by a day
	L"延长%s的合同一周。",
	L"延长%s的合同两周。",
};

// helicopter pilot payment

STR16 pSkyriderText[] =
{
	L"付给 Skyrider $%d",	// skyrider was paid an amount of money
	L"还欠 Skyrider $%d",	// skyrider is still owed an amount of money
	L"Skyrider 完成补给汽油",// skyrider has finished refueling
	L"",//unused
	L"",//unused
	L"Skyrider 已作好再次飞行的准备。", // Skyrider was grounded but has been freed
	L"Skyrider 没有乘客。如果你试图运送这个分区的佣兵，首先要分配他们进入“交通工具”－>“直升飞机”。",
};


// strings for different levels of merc morale

STR16 pMoralStrings[] =
{
	L"高涨",
	L"良好",
	L"稳定",
	L"低下",
	L"恐慌",
	L"糟糕",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.

STR16 pLeftEquipmentString[] =
{
	L"%s的装备现在可以在Omerta (A9)获得。",
	L"%s的装备现在可以在Drassen (B13)获得。",
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] =
{
	L"生命",
	L"精力",
	L"士气",
	L"状态",	// the condition of the current vehicle (its "health")
	L"油量",	// the fuel level of the current vehicle (its "energy")
};


STR16 pMapScreenPrevNextCharButtonHelpText[] =
{
	L"上一佣兵 (|L|e|f|t)",	//"Previous Merc (|L|e|f|t)", 			// previous merc in the list
	L"下一佣兵 (|R|i|g|h|t)",	//"Next Merc (|R|i|g|h|t)", 				// next merc in the list
};


STR16 pEtaString[] =
{
	L"耗时: ",				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"你将不会再见到它了。你确定吗？",	 	// do you want to continue and lose the item forever
	L"这个物品看起来非常非常重要。你真的要扔掉它吗？",	// does the user REALLY want to trash this item
};


STR16 pMapErrorString[] =
{
	L"小队不能行军，因为有人在睡觉。",

//1-5
	L"首先要回到地面，才能移动小队。",
	L"行军？那里是敌占区!",
	L"必须给佣兵分配小队或者交通工具才能开始行军。",
	L"你现在没有任何队员。", 	// you have no members, can't do anything
	L"佣兵无法遵从命令。",		// merc can't comply with your order
//6-10
	L"需要有人护送才能行军。请把他分进一个小队里。", // merc can't move unescorted .. for a male
	L"需要有人护送才能行军。请把她分进一个小队里。", // for a female
	L"佣兵尚未到达Arulco!",
	L"看来得先谈妥合同。",
	L"无法发出行军命令。目前有空袭。",
//11-15
	L"行军? 这里正在战斗中!",
	L"你在分区%s被血猫伏击了!",
	L"你刚刚进入了I16分区，这里是血猫的巢穴!",
	L"",
	L"在%s的SAM导弹基地被敌军占领了。",
//16-20
	L"在%s的矿井被敌军占领了。你的日收入下降为每日%s。",
	L"敌军未遭到抵抗，就占领了%s",
	L"至少有一名你的佣兵不能被分配这个任务。",
	L"%s无法加入%s，因为它已经满员了",
	L"%s无法加入%s，因为它太远了。",
//21-25
	L"在%s的矿井被敌军占领了!",
	L"敌军入侵了%s处的SAM导弹基地",
	L"敌军入侵了%s",
	L"敌军在%s出没。",
	L"敌军占领了%s.",
//26-30
	L"你的佣兵中至少有一人不能睡眠。",
	L"你的佣兵中至少有一人不能醒来。",
	L"训练完毕，才会出现民兵。",
	L"现在无法对%s下达行军命令。",
	L"不在城镇边界的民兵无法行军到另一个分区。",
//31-35
	L"你不能在%s拥有民兵。",
	L"车是空的，无法移动。!",
	L"%s受伤太严重了，无法行军!",
	L"你必须首先离开博物馆！",
	L"%s死了！",
//36-40
	L"%s无法转到%s因为它在移动中",
	L"%s无法那样进入交通工具",
	L"%s无法加入%s",
	L"在你雇佣新的佣兵前，你不能压缩时间。",
	L"车辆只能在公路上开！",
//41-45
	L"在佣兵移动时，你不能重新分配任务",
	L"车辆没油了！",
	L"%s太累了，以致不能行军。",
	L"车上没有人能够驾驶。",
	L"这个小队的佣兵现在不能移动。",
//46-50
	L"其他佣兵现在不能移动。",
	L"车辆被损坏得太严重了！",
	L"每个分区只能由两名佣兵来训练民兵。",
	L"没有遥控员，机器人无法移动。请把他们分配在同一个小队。",
};


// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"再点击一下目的地以确认你的最后路线，或者点击下一个分区以设置更多的路点。",
	L"行军路线已确认。",
	L"目的地未改变。",
	L"行军路线已取消。",
	L"行军路线已缩短。",
};


// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"点击你想让佣兵着陆的分区。",
	L"好的。佣兵将在%s着陆",
	L"佣兵不能飞往那里，领空不安全!",
	L"取消。着陆分区未改变",
	L"%s上的领空现在不安全了!着陆分区被改为%s。",
};


// help text for mouse regions

STR16 pMiscMapScreenMouseRegionHelpText[] =
{
	L"进入存货屏幕 (|E|n|t|e|r)",
	L"扔掉物品",	//"Throw Item Away",
	L"离开存货屏幕 (|E|n|t|e|r)",
};



// male version of where equipment is left
STR16 pMercHeLeaveString[] =
{
	L"让%s把装备留在他现在所在的地方(%s)，或者在Drassen (B13)登机飞离Arulco，把装备留在那里?",
	L"让%s把装备留在他现在所在的地方(%s)，或者在Omerta (A9)登机飞离Arulco，把装备留在那里?",
	L"要离开了，他的装备将被留在Omerta (A9)。",
	L"要离开了，他的装备将被留在Drassen (B13)。",
	L"%s要离开了，他的装备将被留在%s。",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"让%s把装备留在她现在所在的地方(%s)，或者在Drassen (B13)登机飞离Arulco，把装备留在那里?",
	L"让%s把装备留在她现在所在的地方(%s)，或者在Omerta (A9)登机飞离Arulco，把装备留在那里?",
	L"要离开了，她的装备将被留在Omerta (A9)。",
	L"要离开了，她的装备将被留在Drassen (B13)。",
	L"%s要离开了，她的装备将被留在%s。",
};


STR16 pMercContractOverStrings[] =
{
	L"的合同到期了，所以他回家了。",
	L"的合同到期了，所以她回家了。",
	L"的合同中止了，所以他离开了。",
	L"的合同中止了，所以她离开了。",
	L"你欠了M.E.R.C.太多钱，所以%s离开了。",
};

// Text used on IMP Web Pages

// WDS: Allow flexible numbers of IMPs of each sex
//    note: I only updated the English text to remove "three" below
STR16 pImpPopUpStrings[] =
{
	L"无效的授权号",
	L"你试图重新开始整个测试。你确定吗？",
	L"请输入正确的全名和性别。",
	L"对你的财政状况的预先分析显示了你无法负担心理剖析的费用。",
	L"现在不是个有效的选择。",
	L"要进行心理剖析，你的队伍中必须至少留一个空位。",
	L"测试完毕。",
	L"无法从磁盘上读入I.M.P.人物数据。",
	L"你已经达到I.M.P.人物上限。",
	L"你已经达到I.M.P.该性别人物上限。",
	L"你无法支付此I.M.P.人物的费用。",
	L"新的I.M.P.人物加入了你的队伍。",
};


// button labels used on the IMP site

STR16 pImpButtonText[] =
{
	L"关于我们",			// about the IMP site
	L"开始",		// begin profiling
	L"性格",		// personality section
	L"属性", 		// personal stats/attributes section
	L"肖像",		// the personal portrait selection
	L"嗓音%d",		// the voice selection
	L"完成",		// done profiling
	L"重新开始",	// start over profiling
	L"是的，我选择了高亮突出的回答。",
	L"是",
	L"否",
	L"结束",			// finished answering questions
	L"上一个",			// previous question..abbreviated form
	L"下一个", 			// next question
	L"是的，我确定。",	// yes, I am certain
	L"不，我想重新开始。",
	L"是",
	L"否",
	L"后退",		// back one page
	L"取消",		// cancel selection
	L"是的，我确定。",
	L"不，让我再看看。",
	L"注册",			// the IMP site registry..when name and gender is selected
	L"分析...", 		// analyzing your profile results
	L"完成",
	L"嗓音",
};

STR16 pExtraIMPStrings[] =
{
	L"现在嗓音选好了，接着选择您所掌握的技能。",
	L"最后选择你的属性完成注册。",
	L"开始实际分析，选择您的肖像",
	L"接着选择与您最相似的嗓音。",
};

STR16 pFilesTitle[] =
{
  L"文件查看器",
};

STR16 pFilesSenderList[] =
{
  L"侦察报告",
	L"1号通缉令",
	L"2号通缉令",
	L"3号通缉令",
	L"4号通缉令",
	L"5号通缉令",
	L"6号通缉令",
};

// Text having to do with the History Log

STR16 pHistoryTitle[] =
{
	L"日志",
};

STR16 pHistoryHeaders[] =
{
	L"日",		// the day the history event occurred
	L"页数",	// the current page in the history report we are in
	L"日数",	// the days the history report occurs over
	L"位置",	// location (in sector) the event occurred
	L"事件",	// the event label
};

// various history events
// THESE STRINGS ARE "HISTORY LOG" STRINGS AND THEIR LENGTH IS VERY LIMITED.
// PLEASE BE MINDFUL OF THE LENGTH OF THESE STRINGS. ONE WAY TO "TEST" THIS
// IS TO TURN "CHEAT MODE" ON AND USE CONTROL-R IN THE TACTICAL SCREEN, THEN
// GO INTO THE LAPTOP/HISTORY LOG AND CHECK OUT THE STRINGS. CONTROL-R INSERTS
// MANY (NOT ALL) OF THE STRINGS IN THE FOLLOWING LIST INTO THE GAME.
STR16 pHistoryStrings[] =
{
	L"",																						// leave this line blank
	//1-5
	L"从A.I.M雇佣了%s。",
	L"从M.E.R.C雇佣了%s。",
	L"%s死了。",	//"%s died.", 															// merc was killed
	L"在M.E.R.C开户。",
	L"接受Enrico Chivaldori的委托",	//"Accepted Assignment From Enrico Chivaldori",
	//6-10
	L"IMP已生成",
	L"为%s购买保险。",
	L"取消%s的保险合同。",
	L"收到%s的保险索赔。",
	L"延长一日%s的合同。",
	//11-15
	L"延长一周%s的合同。",
	L"延长两周%s的合同。",
	L"%s被解雇了。",
	L"%s退出了。",
	L"开始任务。",
	//16-20
	L"完成任务。",
	L"和%s的矿主交谈",
	L"解放了%s",
	L"启用作弊",
	L"食物会在明天送达Omerta",
	//21-25
	L"%s离队并成为了Daryl Hick的妻子",
	L"%s的合同到期了。",
	L"招募了%s。",
	L"Enrico抱怨进展缓慢",
	L"战斗胜利",
	//26-30
	L"%s的矿井开始缺乏矿石",
	L"%s的矿井采完了矿石",
	L"%s的矿井关闭了",	//"%s mine was shut down",
	L"%s的矿井复工了",
	L"发现一个叫Tixa的监狱。",
	//31-35
	L"打听到一个叫Orta的秘密武器工厂。",
	L"在Orta的科学家捐赠了大量的火箭枪。",
	L"Deidranna女王在利用死尸做某些事情。",
	L"Frank谈到了在San Mona的拳击比赛。",
	L"一个病人说他在矿井里看到了一些东西。",
	//36-40
	L"遇到一个叫Devin的人，他出售爆炸物。",
	L"偶遇Mike，前AIM名人！",
	L"遇到Tony，他做武器买卖。",
	L"从Krott中士那里得到一把火箭枪。",
	L"把Angel皮衣店的契约给了Kyle。",
	//41-45
	L"Madlab提议做一个机器人。",
	L"Gabby能制作对付虫子的隐形药。",
	L"Keith歇业了。",
	L"Howard给Deidranna女王提供氰化物。",
	L"遇到Keith -Cambria的杂货商。",
	//46-50
	L"遇到Howrd，一个在Balime的医药商。",
	L"遇到Perko，他开了一家小修理档口。。",
	L"遇到在Balime的Sam，他有一家五金店。",
	L"Franz做电子产品和其他货物的生意。",
	L"Arnold在Grumm开了一家修理店。",
	//51-55
	L"Fredo在Grumm修理电子产品。",
	L"收到在Balime的有钱人的捐款。",
	L"遇到一个叫Jake的废品商人。",
	L"一个流浪者给了我们一张电子钥匙卡。",
	L"贿赂了Walter，让他打开地下室的门。",
	//56-60
	L"如果Dave有汽油，他会免费进行加油。",
	L"贿赂Pablo。",
	L"Kingping拿回了San Mona矿井中的钱。",
	L"%s赢了拳击赛",
	L"%s输了拳击赛",
	//61-65
	L"%s丧失了拳击赛的参赛资格",
	L"在废弃的矿井里找到一大笔钱。",
	L"遭遇Kingpin派出的杀手。",
	L"该分区失守",	//ENEMY_INVASION_CODE
	L"成功防御该分区",
	//66-70
	L"作战失败",			//ENEMY_ENCOUNTER_CODE
	L"致命的伏击",		//ENEMY_AMBUSH_CODE
	L"杀光了敌军的伏兵",
	L"攻击失败",		//ENTERING_ENEMY_SECTOR_CODE
	L"攻击成功！",
	//71-75
	L"异形攻击",		//CREATURE_ATTACK_CODE
	L"被血猫吃掉了",	//BLOODCAT_AMBUSH_CODE
	L"宰掉了血猫",
	L"%s被干掉了",
	L"把一个恐怖分子的头颅给了Carmen。",
	L"Slay走了",
	L"干掉了%s",
};

STR16 pHistoryLocations[] =
{
	L"N/A",						// N/A is an acronym for Not Applicable
};

// icon text strings that appear on the laptop

STR16 pLaptopIcons[] =
{
	L"邮箱",
	L"网页",
	L"财务",
	L"人事",
	L"日志",
	L"文件",
	L"关闭",
	L"sir-FER 4.0 简体中文版",		// our play on the company name (Sirtech) and web surFER
};

// bookmarks for different websites
// IMPORTANT make sure you move down the Cancel string as bookmarks are being added

STR16 pBookMarkStrings[] =
{
	L"A.I.M.",
	L"Bobby Ray's",
	L"I.M.P",
	L"M.E.R.C.",
	L"公墓",
	L"花店",
	L"MIS保险公司",
	L"取消",
};

STR16 pBookmarkTitle[] =
{
	L"收藏夹",
	L"右击以放进收藏夹，便于以后访问本页面。",
};

// When loading or download a web page

STR16 pDownloadString[] =
{
	L"下载...",
	L"重载...",
};

//This is the text used on the bank machines, here called ATMs for Automatic Teller Machine

STR16 gsAtmSideButtonText[] =
{
	L"好的",
	L"拿取",		// take money from merc
	L"给予",		//give money to merc
	L"取消",		// cancel transaction
	L"清除",		//clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] =
{
	L"转帐 $",  	// transfer money to merc -- short form
	L"状态",			// view stats of the merc
	L"物品清单", 			// view the inventory of the merc
	L"雇佣合同",
};

STR16 sATMText[ ]=
{
	L"转帐？",		// transfer funds to merc?
	L"确定？",		// are we certain?
	L"输入金额",		// enter the amount you want to transfer to merc
	L"选择类型",	// select the type of transfer to merc
	L"资金不足",  //not enough money to transfer to merc
	L"必须是$10的倍数", // transfer amount must be a multiple of $10
};

// Web error messages. Please use foreign language equivilant for these messages.
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator

STR16 pErrorStrings[] =
{
	L"错误",
	L"服务器没有DNS入口。",
	L"请检查URL地址，再次尝试连接。",
	L"好的",
	L"主机连接时断时续。预计需要较长的传输时间。",
};


STR16 pPersonnelString[] =
{
	L"佣兵: ", 			// mercs we have
};


STR16 pWebTitle[ ]=
{
	L"sir-FER 4.0 简体中文版",		// our name for the version of the browser, play on company name
};


// The titles for the web program title bar, for each page loaded

STR16 pWebPagesTitles[] =
{
	L"A.I.M.",
	L"A.I.M. 成员",
	L"A.I.M. 肖像",		// a mug shot is another name for a portrait
	L"A.I.M. 排序",
	L"A.I.M.",
	L"A.I.M. 前成员",
	L"A.I.M. 规则",
	L"A.I.M. 历史",
	L"A.I.M. 链接",
	L"M.E.R.C.",
	L"M.E.R.C. 账号",
	L"M.E.R.C. 注册",
	L"M.E.R.C. 索引",
	L"Bobby Ray's",
	L"Bobby Ray's - 枪械",
	L"Bobby Ray's - 弹药",
	L"Bobby Ray's - 护甲",
	L"Bobby Ray's - 杂货",	//"Bobby Ray's - Misc",							//misc is an abbreviation for miscellaneous
	L"Bobby Ray's - 二手货",
	L"Bobby Ray's - 邮购",
	L"I.M.P.",
	L"I.M.P.",
	L"联合花卉服务公司",
	L"联合花卉服务公司 - 花卉",
	L"联合花卉服务公司 - 订单",
	L"联合花卉服务公司 - 贺卡",
	L"Malleus, Incus & Stapes保险公司",
	L"信息",
	L"合同",
	L"评论",
	L"McGillicutty's 公墓",
	L"",
	L"无法找到URL",
	L"Bobby Ray's - 最近的运货",
	L"",
	L"",
};

STR16 pShowBookmarkString[] =
{
	L"Sir-Help",
	L"再次点击页面以放进收藏夹。",
};

STR16 pLaptopTitles[] =
{
	L"邮箱",
	L"文件查看器",
	L"人事",
	L"帐簿",
	L"日志",
};

STR16 pPersonnelDepartedStateStrings[] =
{
	//reasons why a merc has left.
	L"阵亡",
	L"解雇",
	L"其它: ",
	L"结婚",
	L"合同到期",
	L"退出",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] =
{
	L"当前成员: ",
	L"离队成员: ",
	L"每日花费: ",
	L"最高日薪: ",
	L"最低日薪: ",
	L"行动中牺牲: ",
	L"解雇: ",
	L"其它: ",
};


STR16 pPersonnelCurrentTeamStatsStrings[] =
{
	L"最低",
	L"平均",
	L"最高",
};


STR16 pPersonnelTeamStatsStrings[] =
{
	L"生命",
	L"敏捷",
	L"灵巧",
	L"力量",
	L"领导",
	L"智慧",
	L"级别",
	L"枪法",
	L"机械",
	L"爆破",
	L"医疗",
};


// horizontal and vertical indices on the map screen

STR16 pMapVertIndex[] =
{
	L"X",
	L"A",
	L"B",
	L"C",
	L"D",
	L"E",
	L"F",
	L"G",
	L"H",
	L"I",
	L"J",
	L"K",
	L"L",
	L"M",
	L"N",
	L"O",
	L"P",
};

STR16 pMapHortIndex[] =
{
	L"X",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L"10",
	L"11",
	L"12",
	L"13",
	L"14",
	L"15",
	L"16",
};

STR16 pMapDepthIndex[] =
{
	L"",
	L"-1",
	L"-2",
	L"-3",
};

// text that appears on the contract button

STR16 pContractButtonString[] =
{
	L"合同",
};

// text that appears on the update panel buttons

STR16 pUpdatePanelButtons[] =
{
	L"继续",
	L"停止",
};

// Text which appears when everyone on your team is incapacitated and incapable of battle

CHAR16 LargeTacticalStr[][ LARGE_STRING_LENGTH ] =
{
	L"你在这个地区战败了！",
	L"敌人冷酷无情地处死了你的队员！",
	L"你的昏迷的队员被俘虏了！",
	L"你的队员成了敌军的俘虏。",
};


//Insurance Contract.c
//The text on the buttons at the bottom of the screen.

STR16			InsContractText[] =
{
	L"上一页",
	L"下一页",
	L"接受",
	L"清除",
};



//Insurance Info
// Text on the buttons on the bottom of the screen

STR16		InsInfoText[] =
{
	L"上一页",
	L"下一页",
};



//For use at the M.E.R.C. web site. Text relating to the player's account with MERC

STR16			MercAccountText[] =
{
	// Text on the buttons on the bottom of the screen
	L"支付",
	L"主页",
	L"账号: ",
	L"佣兵",
	L"日数",
	L"日薪",	//5
	L"索价",
	L"合计: ",
	L"你确定要支付%s吗？",
};

// Merc Account Page buttons
STR16			MercAccountPageText[] =
{
	// Text on the buttons on the bottom of the screen
	L"上一页",
  L"下一页",
};


//For use at the M.E.R.C. web site. Text relating a MERC mercenary


STR16			MercInfo[] =
{
	L"生命",
	L"敏捷",
	L"灵巧",
	L"力量",
	L"领导",
	L"智慧",
	L"级别",
	L"枪法",
	L"机械",
	L"爆破",
	L"医疗",

	L"上一位",	//"Previous",
	L"雇佣",	//"Hire",
	L"下一位",	//"Next",
	L"附加信息",	//"Additional Info",
	L"主页",	//"Home",
	L"已雇佣",	//"Hired",
	L"日薪: ",	//"Salary:",
	L"每日",		//"Per Day",
	L"阵亡",	//"Deceased",

	L"看起来你雇佣了太多的佣兵。最多只能雇18人。",	//"Looks like you're trying to hire too many mercs. Your limit is 18.",
	L"不可雇佣",	//"Unavailable",
};



// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16			MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"开户",	//"Open Account",
	L"取消",	//"Cancel",
	L"你没有帐户。你希望开一个吗？",	//"You have no account.  Would you like to open one?",
};



// For use at the M.E.R.C. web site. MERC Homepage

STR16			MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Speck T. Kline, 创办者和所有权拥有者",
	L"开户点击这里",	//"To open an account press here",
	L"查看帐户点击这里",	//"To view account press here",
	L"查看文件点击这里",	//"To view files press here",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",	//"Speck Com v3.2",
};

// For use at MiGillicutty's Web Page.

STR16			sFuneralString[] =
{
	L"McGillicutty公墓: 1983开业，办理家庭悼念业务。",
	L"葬礼部经理兼A.I.M.前佣兵 Murray \"Pops\" McGillicutty是一名经验丰富、业务熟练的殡仪业者。",
	L"Pops跟死亡和葬礼打了一辈子交道，他非常熟悉该业务。",
	L"McGillicutty公墓提供各种各样的悼念服务: 从可以依靠着哭泣的肩膀到对严重变形的遗体做美容美体服务。",
	L"McGillicutty公墓是你所爱的人的安息地。",

	// Text for the various links available at the bottom of the page
	L"献花",
	L"骨灰盒",
	L"火葬服务",
	L"安排葬礼",
	L"葬礼规则",

	// The text that comes up when you click on any of the links ( except for send flowers ).
	L"很抱歉，由于家里有人去世，本网站的剩余部分尚未完成。一旦解决了宣读遗嘱和财产分配问题，本网站会尽快建设好。",
	L"很抱歉，但是，现在还是测试期间，请以后再来访问。",
};

// Text for the florist Home page

STR16			sFloristText[] =
{
	//Text on the button on the bottom of the page

	L"花卉",

	//Address of United Florist

	L"\"We air-drop anywhere\"",
	L"1-555-SCENT-ME",
	L"333 NoseGay Dr, Seedy City, CA USA 90210",
	L"http://www.scent-me.com",

	// detail of the florist page

	L"我们快速高效!",
	L"保证把鲜花在第二天送到世界上大部分地区，但有少量限制。",
	L"保证价格是世界上最低廉的!",
	L"向我们反应比我们价格更低的送花服务广告，我们会送你一打绝对免费的玫瑰。",
	L"自从1981年来，我们送植物、送动物、送鲜花。",
	L"我们雇请了被颁发过勋章的前轰炸机飞行员，他们能把你的鲜花空投在指定位置的十英里半径内。总是这样 - 每次这样!",
	L"让我们满足你对鲜花的品位。",
	L"让Bruce，我们的世界闻名的花卉设计师，从我们的花房里为你亲手摘取最新鲜、最优质的花束。",
	L"还有请记住，如果我们没有你要的花，我们能种出来 - 很快!",
};



//Florist OrderForm

STR16			sOrderFormText[] =
{
	//Text on the buttons

	L"后退",	//"Back",
	L"发送",	//"Send",
	L"清除",	//"Clear",
	L"花卉",	//"Gallery",

	L"花卉名称: ",			//"Name of Bouquet:",
	L"价格: ",			//"Price:",//5
	L"订单号: ",			//"Order Number:",
	L"送货日期",			//"Delivery Date",
	L"第二天",			//"next day",
	L"慢慢送去",			//"gets there when it gets there",
	L"送货目的地",			//"Delivery Location",//10
	L"额外服务",			//"Additional Services",
	L"变形的花卉($10)",		//"Crushed Bouquet($10)",
	L"黑玫瑰($20)",			//"Black Roses($20)",
	L"枯萎的花卉($10)",		//"Wilted Bouquet($10)",
	L"水果蛋糕(如果有的话)($10)",	//"Fruit Cake (if available)($10)",	//15
	L"私人密语: ",			//"Personal Sentiments:",
	L"你写的话不能多于75字。",
	L"...或者选择我们提供的",	//L"...or select from one of our",

	L"标准贺卡",			//"STANDARDIZED CARDS",
	L"传单信息",			//"Billing Information",	//20

	//The text that goes beside the area where the user can enter their name

	L"姓名: ",				//"Name:",
};




//Florist Gallery.c

STR16		sFloristGalleryText[] =
{
	//text on the buttons

	L"上一页",	//abbreviation for previous
	L"下一页",	//abbreviation for next

	L"点击你想要订购的花卉。",
	L"请注意: 为了防止运输中的枯萎和变形，每束花另收$10包装费。",

	//text on the button

	L"主页",	//"Home",
};

//Florist Cards

STR16			sFloristCards[] =
{
	L"请点击你想要订购的贺卡",	//"Click on your selection",
	L"后退",			//"Back",
};



// Text for Bobby Ray's Mail Order Site

STR16			BobbyROrderFormText[] =
{
	L"订单",		//"Order Form",		//Title of the page
	L"数量",			//"Qty",			// The number of items ordered
	L"重量 （%s）",		//"Weight (%s)",	// The weight of the item
	L"物品名称",		//"Item Name",		// The name of the item
	L"物品单价",		//"Unit Price",			// the item's weight
	L"总价",		//"Total",		//5	// The total price of all of items of the same type
	L"全部物品总价",		//"Sub-Total",		// The sub total of all the item totals added
	L"运费 (视目的地而定)",	//	"S&H (See Delivery Loc.)",	// S&H is an acronym for Shipping and Handling
	L"全部费用",		//"Grand Total",		// The grand total of all item totals + the shipping and handling
	L"送货目的地",	//"Delivery Location",
	L"运输速度",	//"Shipping Speed",		//10	// See below
	L"运费(每%s)",	//"Cost (per %s.)",		// The cost to ship the items
	L"连夜速递",	//"Overnight Express",		// Gets deliverd the next day
	L"2工作日",	//"2 Business Days",		// Gets delivered in 2 days
	L"标准服务",	//"Standard Service",		// Gets delivered in 3 days
	L"清除订单",		//"Clear Order",	//15// Clears the order page
	L"确认订单",	//"Accept Order",		// Accept the order
	L"后退",		//"Back",	// text on the button that returns to the previous page
	L"主页",		//"Home",	// Text on the button that returns to the home page
	L"*代表二手货",	//"* Denotes Used Items",	// Disclaimer stating that the item is used
	L"你无法支付所需费用。",	//"You can't afford to pay for this.",		//20	// A popup message that to warn of not enough money
	L"<无>",		//"<NONE>",		// Gets displayed when there is no valid city selected
	L"你确定要把该订单里订购的物品送往%s吗?",	//"Are you sure you want to send this order to %s?",	// A popup that asks if the city selected is the correct one
	L"包裹重量**",	//"Package Weight**",		// Displays the weight of the package
	L"** 最小重量: ",		//"** Min. Wt.",			// Disclaimer states that there is a minimum weight for the package
	L"运货",		//"Shipments",
};

STR16			BobbyRFilter[] =
{
	// Guns
	L"重武器", //"Heavy W.",
	L"手枪", //"Pistol",
	L"自动手枪", //"M. Pistol",
	L"冲锋枪", //"SMG",
	L"歩枪", //"Rifle",
	L"狙击歩枪", //"SN Rifle",
	L"突击歩枪", //"AS Rifle",
	L"机枪", //"MG",
	L"霰弹枪", //"Shotgun",

	// Ammo
	L"手枪", //"Pistol",
	L"自动手枪", //"M. Pistol",
	L"冲锋枪", //"SMG",
	L"歩枪", //"Rifle",
	L"狙击歩枪", //"SN Rifle",
	L"突击歩枪", //"AS Rifle",
	L"机枪", //"MG",
	L"霰弹枪", //"Shotgun",

	// Used
	L"枪械", //"Guns",
	L"护甲", //"Armor",
	L"携行具", //"LBE Gear",
	L"其他", //"Misc",

	// Armour
	L"头盔", //"Helmets",
	L"防弹衣", //"Vests",
	L"作战裤", //"Leggings",
	L"防弹板", //"Plates",

	// Misc
	L"刀具", //"Blades",
	L"飞刀", //"Th. Knives",
	L"格斗武器", //"Punch. W.",
	L"手雷/榴弹", //"Grenades",
	L"炸药", //"Bombs",
	L"医疗用品", //"Med. Kits",
	L"工具套装", //"Kits",
	L"通讯/夜视", //"Face Items",
	L"携行具", //"LBE Gear",
	L"附件/瞄准具", //"Misc.",
};


// This text is used when on the various Bobby Ray Web site pages that sell items

STR16			BobbyRText[] =
{
	L"订购",				//"To Order",	// Title
	// instructions on how to order
	L"请点击该物品。如果要订购多件，请连续点击。右击以减少要订购的数量。一旦选好了你要订购的，请前往订单页面。",

	//Text on the buttons to go the various links

	L"上一页",	//"Previous Items",		//
	L"枪械",	//"Guns", 			//3
	L"弹药",	//"Ammo",			//4
	L"护甲",	//"Armor",			//5
	L"其他",	//"Misc.",			//6	//misc is an abbreviation for miscellaneous
	L"二手货",	//"Used",			//7
	L"下一页",	//"More Items",
	L"订货单",	//"ORDER FORM",
	L"主页",	//"Home",			//10

	//The following 2 lines are used on the Ammunition page.
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"你的队伍有",		//"Your team has",//11
	L"件可使用该类型弹药的武器", 	//"weapon(s) that use this type of ammo", //12

	//The following lines provide information on the items

	L"重量: ",	//"Weight:",		// Weight of all the items of the same type
	L"口径: ",	//"Cal:",			// the caliber of the gun
	L"载弹量: ",	//"Mag:",			// number of rounds of ammo the Magazine can hold
	L"射程: ",	//"Rng:",			// The range of the gun
	L"杀伤力: ",	//"Dam:",			// Damage of the weapon
	L"射速: ",	//"ROF:",			// Weapon's Rate Of Fire, acronym ROF
	L"单价: ",	//"Cost:",			// Cost of the item
	L"库存: ",	//"In stock:",			// The number of items still in the store's inventory
	L"购买量: ",	//"Qty on Order:",		// The number of items on order
	L"已损坏",	//"Damaged",			// If the item is damaged
	L"重量: ",	//"Weight:",			// the Weight of the item
	L"小计: ",	//"SubTotal:",			// The total cost of all items on order
	L"* %％ 可用",	//"* %% Functional",		// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time

	L"靠!  我们这里的在线订单一次只接受10件物品的订购。如果你想要订购更多东西（我们希望如此），请接受我们的歉意，再开一份订单。",

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"抱歉，这种商品我们现在正在进货。请稍后再来订购。",

	//A popup that tells the user that the store is temporarily sold out

	L"抱歉，这种商品我们现在缺货。",

};


// Text for Bobby Ray's Home Page

STR16			BobbyRaysFrontText[] =
{
	//Details on the web site

	L"这里有最新最火爆的军火供应",	//"This is the place to be for the newest and hottest in weaponry and military supplies",
	L"我们提供硬件满足您所有破坏欲望！",	//"We can find the perfect solution for all your explosives needs",
	L"二手货",	//"Used and refitted items",

	//Text for the various links to the sub pages

	L"杂货",	//"Miscellaneous",
	L"枪械",	//"GUNS",
	L"弹药",	//"AMMUNITION",		//5
	L"护甲",	//"ARMOR",

	//Details on the web site

	L"独此一家，别无分店。",	//"If we don't sell it, you can't get it!",
	L"网站建设中",	//"Under Construction",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"A.I.M成员",	//"A.I.M. Members",				// Title
	// Title for the way to sort
	L"排序: ",	//"Sort By:",

	// sort by...

	L"薪金",	//"Price",
	L"级别",	//"Experience",
	L"枪法",	//"Marksmanship",
	L"医疗",	//"Medical",
	L"爆破",	//"Explosives",
	L"机械",	//"Mechanical",

	//Text of the links to other AIM pages

	L"查看佣兵的肖像索引",	//"View the mercenary mug shot index",
	L"查看单独的佣兵档案",	//"Review the individual mercenary's file",
	L"浏览A.I.M前成员",	//"Browse the A.I.M. Alumni Gallery",

	// text to display how the entries will be sorted

	L"升序",	//"Ascending",
	L"降序",	//"Descending",
};


//Aim Policies.c
//The page in which the AIM policies and regulations are displayed

STR16		AimPolicyText[] =
{
	// The text on the buttons at the bottom of the page

	L"上一页",	//"Previous Page",
	L"AIM主页",	//"AIM HomePage",
	L"规则索引",	//"Policy Index",
	L"下一页",	//"Next Page",
	L"不同意",	//Disagree",
	L"同意",	//"Agree",
};



//Aim Member.c
//The page in which the players hires AIM mercenaries

// Instructions to the user to either start video conferencing with the merc, or to go the mug shot index

STR16			AimMemberText[] =
{
	L"鼠标左击",	//"Left Click",
	L"联系佣兵。",	//"to Contact Merc.",
	L"鼠标右击",	//"Right Click",
	L"回到肖像索引。",	//"for Mug Shot Index.",
};

//Aim Member.c
//The page in which the players hires AIM mercenaries

STR16			CharacterInfo[] =
{
	// The various attributes of the merc

	L"生命",
	L"敏捷",
	L"灵巧",
	L"力量",
	L"领导",
	L"智慧",
	L"级别",
	L"枪法",
	L"机械",
	L"爆破",
	L"医疗",

	// the contract expenses' area

	L"费用",	//"Fee",
	L"合同",	//"Contract",
	L"一日",	//"one day",
	L"一周",	//"one week",
	L"两周",	//"two weeks",

	// text for the buttons that either go to the previous merc,
	// start talking to the merc, or go to the next merc

	L"上一位",	//"Previous",
	L"联系",	//"Contact",
	L"下一位",	//"Next",

	L"附加信息",	//"Additional Info",				// Title for the additional info for the merc's bio
	L"现役成员",	//"Active Members",		//20		// Title of the page
	L"可选装备: ",	//"Optional Gear:",				// Displays the optional gear cost
	L"所需医疗保证金",	//"MEDICAL deposit required",			// If the merc required a medical deposit, this is displayed
};


//Aim Member.c
//The page in which the player's hires AIM mercenaries

//The following text is used with the video conference popup

STR16			VideoConfercingText[] =
{
	L"合同总价: ",	//"Contract Charge:",				//Title beside the cost of hiring the merc

	//Text on the buttons to select the length of time the merc can be hired

	L"一日",	//"One Day",
	L"一周",	//"One Week",
	L"两周",	//"Two Weeks",

	//Text on the buttons to determine if you want the merc to come with the equipment

	L"不买装备",	//"No Equipment",
	L"购买装备",	//"Buy Equipment",

	// Text on the Buttons

	L"转帐",	//"TRANSFER FUNDS",			// to actually hire the merc
	L"取消",	//"CANCEL",				// go back to the previous menu
	L"雇佣",	//"HIRE",				// go to menu in which you can hire the merc
	L"挂断",	//"HANG UP",				// stops talking with the merc
	L"完成",	//"OK",
	L"留言",	//"LEAVE MESSAGE",			// if the merc is not there, you can leave a message

	//Text on the top of the video conference popup

	L"视频通讯: ",	//"Video Conferencing with",
	L"建立连接……",	//"Connecting. . .",

	L"包括医保",	//"with medical"			// Displays if you are hiring the merc with the medical deposit
};



//Aim Member.c
//The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16			AimPopUpText[] =
{
	L"电子转帐成功",	//"ELECTRONIC FUNDS TRANSFER SUCCESSFUL",	// You hired the merc
	L"无法处理转帐",	//"UNABLE TO PROCESS TRANSFER",		// Player doesn't have enough money, message 1
	L"资金不足",	//"INSUFFICIENT FUNDS",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"执行任务中",	//"On Assignment"
	L"请留言",	//"Please Leave Message",
	L"阵亡",	//"Deceased",

	//If you try to hire more mercs than game can support

	L"你的队伍现有18人，已经满员了。",	//"You have a full team of 18 mercs already.",

	L"预录消息",	//"Pre-recorded message",
	L"留言已记录",	//"Message recorded",
};


//AIM Link.c

STR16			AimLinkText[] =
{
	L"A.I.M 链接",//	L"A.I.M. Links",	//The title of the AIM links page
};



//Aim History

// This page displays the history of AIM

STR16			AimHistoryText[] =
{
	L"A.I.M 历史",	//"A.I.M. History",					//Title

	// Text on the buttons at the bottom of the page

	L"上一页",	//"Previous Page",
	L"主页",	//"Home",
	L"A.I.M 前成员",	//"A.I.M. Alumni",
	L"下一页",	//"Next Page",
};


//Aim Mug Shot Index

//The page in which all the AIM members' portraits are displayed in the order selected by the AIM sort page.

STR16			AimFiText[] =
{
	// displays the way in which the mercs were sorted

	L"薪金",	//"Price",
	L"级别",	//"Experience",
	L"枪法",	//"Marksmanship",
	L"医疗",	//"Medical",
	L"爆破",	//"Explosives",
	L"机械",	//"Mechanical",

	// The title of the page, the above text gets added at the end of this text

	L"根据%s升序排列的A.I.M成员",	//"A.I.M. Members Sorted Ascending By %s",
	L"根据%s降序排列的A.I.M成员",	//"A.I.M. Members Sorted Descending By %s",

	// Instructions to the players on what to do

	L"鼠标左击",	//"Left Click",
	L"选择佣兵。",	//"To Select Merc",			//10
	L"鼠标右击",	//"Right Click",
	L"回到排序选项。",	//"For Sorting Options",

	// Gets displayed on top of the merc's portrait if they are...

	L"离开",	//"Away",
	L"阵亡",	//"Deceased",						//14
	L"任务中",	//"On Assign",
};



//AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are no longer with AIM

STR16			AimAlumniText[] =
{
	// Text of the buttons

	L"第一页",
	L"第二页",
	L"第三页",

	L"A.I.M 前成员",

	L"完成",
};






//AIM Home Page

STR16			AimScreenText[] =
{
	// AIM disclaimers

	L"A.I.M. 和 A.I.M. 图标在世界大多数国家已经注册。",
	L"版权所有，仿冒必究。",
	L"Copyright 1998-1999 A.I.M., Ltd.  All rights reserved.",

	//Text for an advertisement that gets displayed on the AIM page

	L"联合花卉服务公司",
	L"\"我们将花空运到任何地方\"",				//10
	L"把活干好",
	L"... 第一次",
	L"枪械和杂货，只此一家，别无分店。",
};


//Aim Home Page

STR16			AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages
	L"主页",	//"Home",
	L"成员",	//"Members",
	L"前成员",	//"Alumni",
	L"规则",	//"Policies",
	L"历史",	//"History",
	L"链接",	//"Links",
};



//ShopKeeper Interface
// The shopkeeper interface is displayed when the merc wants to interact with
// the various store clerks scattered through out the game.

STR16 SKI_Text[ ] =
{
	L"库存商品",	//"MERCHANDISE IN STOCK",	//Header for the merchandise available
	L"页面",			//"PAGE",	//The current store inventory page being displayed
	L"总价格",			//"TOTAL COST",		//The total cost of the the items in the Dealer inventory area
	L"总价值",			//"TOTAL VALUE",	//The total value of items player wishes to sell
	L"估价",			//"EVALUATE",		//Button text for dealer to evaluate items the player wants to sell
	L"确认交易",			//"TRANSACTION",	//Button text which completes the deal. Makes the transaction.
	L"完成",			//"DONE",	//Text for the button which will leave the shopkeeper interface.
	L"修理费",			//"REPAIR COST",	//The amount the dealer will charge to repair the merc's goods
	L"1小时",			//"1 HOUR",// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d小时",		//"%d HOURS",// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"已经修好",		//"REPAIRED",// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"你没有空余的位置来放东西了。",	//"There is not enough room in your offer area.",//Message box that tells the user there is no more room to put there stuff
	L"%d分钟",		//"%d MINUTES",	// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"把物品放在地上。",	//"Drop Item To Ground.",
};

//ShopKeeper Interface
//for the bank machine panels. Referenced here is the acronym ATM, which means Automatic Teller Machine

STR16	SkiAtmText[] =
{
	//Text on buttons on the banking machine, displayed at the bottom of the page
	L"0",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L"确认",	//"OK",				// Transfer the money
	L"拿",		//"Take",				// Take money from the player
	L"给",		//"Give",				// Give money to the player
	L"取消",	//"Cancel",				// Cancel the transfer
	L"清除",	//"Clear",				// Clear the money display
};


//Shopkeeper Interface
STR16	gzSkiAtmText[] =
{

	// Text on the bank machine panel that....
	L"选择类型",			//"Select Type",	// tells the user to select either to give or take from the merc
	L"输入数额",		//"Enter Amount",	// Enter the amount to transfer
	L"把钱给佣兵",	//"Transfer Funds To Merc",// Giving money to the merc
	L"从佣兵那拿钱",	//"Transfer Funds From Merc",	// Taking money from the merc
	L"资金不足",		//"Insufficient Funds",	// Not enough money to transfer
	L"余额",			//"Balance",	// Display the amount of money the player currently has
};


STR16	SkiMessageBoxText[] =
{
	L"你要从主帐户中提取%s来支付吗?",
	L"资金不足。你缺少%s。",
	L"你要从主帐户中提取%s来支付吗?",
	L"请求商人开始交易",
	L"请求商人修理选定物品",
	L"结束对话",
	L"当前余额",
};


//OptionScreen.c

STR16	zOptionsText[] =
{
	//button Text
	L"保存游戏",	//"Save Game",
	L"载入游戏",	//"Load Game",
	L"退出",	//"Quit",
	L"下一页",	//L"Next",
	L"上一页",	//L"Prev",
	L"完成",	//"Done",

	//Text above the slider bars
	L"效果音",	//"Effects",
	L"语音",	//"Speech",
	L"音乐",	//"Music",

	//Confirmation pop when the user selects..
	L"退出并回到游戏主菜单？",

	L"你必须选择“语音”和“对话显示”中的至少一项。",
};


//SaveLoadScreen
STR16			zSaveLoadText[] =
{
	L"保存游戏",	//"Save Game",
	L"载入游戏",	//"Load Game",
	L"取消",	//"Cancel",
	L"选择要存档的位置",	//"Save Selected",
	L"选择要读档的位置",	//"Load Selected",

	L"保存游戏成功",	//"Saved the game successfully",
	L"保存游戏错误！",	//"ERROR saving the game!",
	L"载入游戏成功",	//"Loaded the game successfully",
	L"载入游戏错误！",	//"ERROR loading the game!",

	L"存档的游戏版本不同于当前的游戏版本。读取它的话很可能游戏可以正常进行。要读取该存档吗？",
	L"存档可能已经无效。你要删除它们吗？",

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"存档版本已改变。如果出现问题请报告。继续？",
#else
	L"试图载入老版本的存档。自动修正并载入存档？",
#endif

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"存档版本和游戏版本已改变。如果出现问题请报告。继续？",
#else
	L"试图载入老版本的存档。你要自动更新并载入存档吗？",
#endif

	L"你确认你要将#%d位置的存档覆盖吗?",
	L"你要从#号位置载入存档吗",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"你的硬盘空间不够。你现在只有 %dM 可用空间，JA2需要至少 %dM 可用空间。",

	L"保存...",	//"Saving...",			//When saving a game, a message box with this string appears on the screen

	L"普通武器",		//"Normal Guns",
	L"包括前华约武器",	//"Tons of Guns",
	L"现实风格",		//"Realistic style",
	L"科幻风格",		//"Sci Fi style",

	L"难度",		//"Difficulty",
	L"Platinum Mode", //Placeholder English

	L"Bobby Ray's库存",
	L"普通",
	L"较多",
	L"很多",
	L"囧...多",

	L"新携行系统不兼容640x480的屏幕分辨率，请重新设置分辨率。",
	L"新携行系统无法使用默认的 Data 文件夹，请仔细读说明。",
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"地图层次",	//"Map Level",
	L"你没有民兵。要拥有民兵，你得先训练镇民。",	//"You have no militia.  You need to train town residents in order to have a town militia.",
	L"每日收入",	//"Daily Income",
	L"佣兵有人寿保险",	//"Merc has life insurance",
	L"%s不疲劳。",	//"%s isn't tired.",
	L"%s行军中，不能睡觉",	//"%s is on the move and can't sleep",
	L"%s太累了，等会儿再试。",	//"%s is too tired, try a little later.",
	L"%s正在开车。",	//"%s is driving.",
	L"有人在睡觉时，整个队伍不能行动。",	//"Squad can't move with a sleeping merc on it.",

	// stuff for contracts
	L"你能支付合同所需费用，但是你的钱不够给该佣兵购买人寿保险。",
	L"要给%s花费保险金%s 以延长保险合同%d天。你要付费吗？",
	L"分区存货",	//"Sector Inventory",
	L"佣兵有医疗保证金。",	//"Merc has a medical deposit.",

	// other items
	L"医生", 	//"Medics", // people acting a field medics and bandaging wounded mercs
	L"病人", 	//"Patients", // people who are being bandaged by a medic
	L"完成", 	//"Done", // Continue on with the game after autobandage is complete
	L"停止", 	//"Stop", // Stop autobandaging of patients by medics now
	L"抱歉。游戏取消了该选项的功能。",
	L"%s没有工具箱。",	//"%s doesn't have a repair kit.",
	L"%s没有医药箱。",	//"%s doesn't have a medical kit.",
	L"现在没有足够的人愿意加入民兵。",
	L"%s的民兵已经训练满了。",	//"%s is full of militia.",
	L"佣兵有一份限时的合同。",	//"Merc has a finite contract.",
 L"佣兵的合同没投保", //"Merc's contract is not insured",
	L"地图概况",//"Map Overview",		// 24
};


STR16 pLandMarkInSectorString[] =
{
	L"第%d小队在%s地区发现有人",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] =
{
	L"训练一队民兵要花费$",
	L"同意支付吗？",
	L"你无法支付。",
	L"继续在%s (%s %d)训练民兵吗？",
	L"花费$",
	L"( Y/N )",   // abbreviated yes/no
	L"",	// unused
	L"在%d地区训练民兵将花费$%d。%s",
	L"你无法支付$%d以供在这里训练民兵。",
	L"%s的忠诚度必须达到%d以上方可训练民兵。",
	L"你不能在%s训练民兵了。",
};

//Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom of the single merc panel
STR16	gzMoneyWithdrawMessageText[] =
{
	L"你每次最多能提取$20,000。",
	L"你确认要把%s存入你的帐户吗？",
};

STR16	gzCopyrightText[] =
{
	L"Copyright (C) 1999 Sir-tech Canada Ltd.  All rights reserved.",
};

//option Text
STR16		zOptionsToggleText[] =
{
	L"语音",		//"Speech",
	L"确认静默",		//"Mute Confirmations",
	L"显示对话文字",	//"SubTitles",
	L"显示对话文字时暂停",		//"Pause Text Dialogue",
	L"烟火效果",		//"Animate Smoke",
	L"血腥效果",		//"Blood n Gore",
	L"不移动鼠标",		//"Never Move My Mouse!",
	L"旧的选择方式",	//"Old Selection Method",
	L"显示移动路径",	//"Show Movement Path",
	L"显示未击中",		//"Show Misses",
	L"实时确认",		//"Real Time Confirmation",
	L"显示睡觉/醒来时的提示",	//"Display sleep/wake notifications",
	L"使用公制系统",		//"Use Metric System",
	L"佣兵移动时高亮显示",	//"Merc Lights during Movement",
	L"锁定佣兵",	//"Snap Cursor to Mercs",
	L"锁定门",		//"Snap Cursor to Doors",
	L"物品闪亮",	//"Make Items Glow",
	L"显示树冠",	//"Show Tree Tops",
	L"显示轮廓",		//"Show Wireframes",
	L"显示3D光标",	//"Show 3D Cursor",
	L"显示命中机率",	//"Show Chance to Hit on cursor",
	L"榴弹发射器连发时使用枪击光标",	//"GL Burst uses Burst cursor",
	L"敌人掉落所有物品",	//"Enemies Drop all Items",
	L"允许高仰角榴弹发射",	//"High angle Grenade launching",
	L"限制额外的瞄准",	//"Restrict extra Aim Levels",
	L"按空格键选择下一支小队",	//"Space selects next Squad",
	L"显示物品阴影",	//"Show Item Shadow",
	L"用格数显示武器射程",	//"Show Weapon Ranges in Tiles",
	L"单发曳光弹显示曳光",	//"Tracer effect for single shot",
	L"雨声",	//"Rain noises",
	L"允许乌鸦",	//"Allow crows",
	L"随机产生I.M.P人物性格",	//"Random I.M.P personality",
	L"自动存盘",	//"Auto save",
	L"沉默的Skyrider",	//"Silent Skyrider",
	L"降低CPU的使用率",	//"Low CPU usage",
	L"Enhanced Description Box",
	L"强制回合制模式",						// add forced turn mode
	L"--作弊模式选项--",				// TOPTION_CHEAT_MODE_OPTIONS_HEADER,
	L"强制 Bobby Ray 送货",			// force all pending Bobby Ray shipments
	L"-----------------",					// TOPTION_CHEAT_MODE_OPTIONS_END
	L"--DEBUG 选项--",					// an example options screen options header (pure text)
	L"重置所有选项",							// failsafe show/hide option to reset all options
	L"确定要重置？",							// a do once and reset self option (button like effect)
	L"Debug Options in other builds",		// allow debugging in release or mapeditor
	L"DEBUG Render Option group",			// an example option that will show/hide other options
	L"Render Mouse Regions",				// an example of a DEBUG build option
	L"-----------------",					// an example options screen options divider (pure text)

	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"THE_LAST_OPTION",
};

//This is the help text associated with the above toggles.
STR16	zOptionsScreenHelpText[] =
{
	//speech
	L"如果你想听到人物对话，打开这个选项。",

	//Mute Confirmation
	L"打开或关闭人物的口头确认。",

		//Subtitles
	L"是否显示对话的文字。",

	//Key to advance speech
	L"如果“显示对话文字”已打开，这个选项会让你有足够的时间来阅读NPC的对话。",

	//Toggle smoke animation
	L"如果烟火效果使得游戏变慢，关闭这个选项。",

	//Blood n Gore
	L"如果鲜血使你觉得恶心，关闭这个选项。",

	//Never move my mouse
	L"关闭这个选项会使你的光标自动移到弹出的确认对话框上。",

	//Old selection method
	L"打开这个选项，使用铁血联盟1代的佣兵选择方式。",

	//Show movement path
	L"打开这个选项，会实时显示移动路径(关闭此选项。如果你想要显示路径的话，使用SHIFT键)。",

	//show misses
	L"打开这个选项时，会显示未击中目标的子弹落点。",

	//Real Time Confirmation
	L"当打开时，进入实时模式会有一个确认对话框。",

	//Sleep/Wake notification
	L"当打开时，被分配任务的佣兵睡觉和醒来时会提示你。",

	//Use the metric system
	L"当打开时，使用公制系统，否则使用英制系统。",

	//Merc Lighted movement
	L"当打开时，佣兵移动时会照亮地表。关闭这个选项会使游戏的显示速度变快。",

	//Smart cursor
	L"当打开时，光标移动到佣兵身上时会高亮显示佣兵。",

	//snap cursor to the door
	L"当打开时，光标靠近门时会自动定位到门上。",

	//glow items
	L"当打开时，物品会不断的闪烁。(|I)",

	//toggle tree tops
	L"当打开时，显示树冠。(|T)",

	//toggle wireframe
	L"显示未探明的墙的轮廓。(|W)",

	L"打开时，移动时的光标为3D式样。(|H)",

	// Options for 1.13
	L"当打开时，在光标上显示命中机率。",
	L"开启该选项时，榴弹发射器点射使用点射的准星",
	L"当打开时，死亡的敌人掉落所有的物品。",
	L"当打开时，榴弹发射器允许采用较高仰角发射榴弹。(|Q)",
	L"当打开时，狙击步枪以外的枪支最多使用4AP去瞄准。",
	L"当打开时，按空格键自动切换到下一小队。(|S|p|a|c|e)",
	L"开启该选项时，会显示物品阴影",
	L"当打开时，用格数显示武器射程。",
	L"当打开时，单发曳光弹也显示曳光。",
	L"当打开时，下雨时能听到雨水音效。",	//"When ON, you will hear rain noises when it is raining.",
	L"当打开时，允许乌鸦出现。",
	L"当打开时, I.M.P人物的性格和属性为随机生成。",	//"When ON, I.M.P characters will get random personality and attitude.",
	L"当打开时，游戏将在玩家回合后自动存盘",
	L"当打开时，Skyrider将保持沉默。",
	L"当打开时，游戏将使用更少的CPU资源。",
	L"当打开时，将出现物品及武器的“增强描述框”（EDB）。",
	L"当打开时，且在战术画面内存在敌军时，将一直处于回合制模式直至该地区所有敌军被消灭（可以通过快捷键|C|T|R|L+|S|H|I|F|T+|A|L|T+|T来控制打开或关闭强制回合制模式）",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_HEADER",
	L"Force all pending Bobby Ray shipments",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_END",
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER",			// an example options screen options header (pure text)
	L"Click me to fix corrupt game settings",							// failsafe show/hide option to reset all options
	L"Click me to fix corrupt game settings",							// a do once and reset self option (button like effect)
	L"Allows debug options in release or mapeditor builds",				// allow debugging in release or mapeditor
	L"Toggle to display debugging render options",						// an example option that will show/hide other options
	L"Attempts to display slash-rects around mouse regions",			// an example of a DEBUG build option
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END",				// an example options screen options divider (pure text)


	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"TOPTION_LAST_OPTION",
};


STR16	gzGIOScreenText[] =
{
	L"游戏初始设置",
	L"游戏风格",
	L"现实",
	L"科幻",
	L"金版",
	L"物品",
	L"大量武器",
	L"少量武器",
	L"难度",
	L"新手",
	L"老手",
	L"专家",
	L"疯子",
	L"确定",
	L"取消",
	L"额外难度",
	L"可随时存盘",
	L"铁人模式",
	L"在Demo中禁用",
	L"Bobby Ray's 库存",
	L"普通",
	L"较多",
	L"很多",
	L"囧……多",
	L"携行系统",
	L"旧",
	L"新",
	L"读取联机游戏",
	L"游戏初始设置（仅在服务器设置时有效）",
};

STR16	gzMPJScreenText[] =
{
	L"多人游戏",//L"MULTIPLAYER",
	L"加入",//L"Join",
	L"主机",//L"Host",
	L"取消",//L"Cancel",
	L"刷新",//L"Refresh",
	L"玩家名称",//L"Player Name",
	L"服务器 IP",//L"Server IP",
	L"端口",//L"Port",
	L"服务器名",//L"Server Name",
	L"# Plrs",
	L"版本",//L"Version",
	L"游戏类型",//L"Game Type",
	L"Ping",
	L"你必须输入你的玩家名称。",//L"You must enter a player name.",
	L"你必须输入有效的服务器IP地址。\n (例如 84.114.195.239).",//L"You must enter a valid server IP address.\n (eg 84.114.195.239).",
	L"您必须输入正确的服务器端口，范围1~65535。",//L"You must enter a valid Server Port between 1 and 65535.",
};

STR16	gzMPHScreenText[] =
{
	L"建立主机",//L"HOST GAME",
	L"开始",//L"Start",
	L"取消",//L"Cancel",
	L"服务器名",//L"Server Name",
	L"游戏类型",//L"Game Type",
	L"死亡模式",//L"Deathmatch",
	L"团队死亡模式",//L"Team Deathmatch",
	L"合作模式",//L"Co-operative",
	L"最大玩家数",//L"Max Players",
	L"小队规模",//L"Squad Size",
	L"选择佣兵",//L"Merc Selection",
	L"随机佣兵",//L"Random Mercs",
	L"已被雇佣",//L"Hired by Player",
	L"起始平衡",//L"Starting Balance",
	L"可以雇佣相同佣兵",//L"Can Hire Same Merc",
	L"Report Hired Mercs",
	L"开启Bobby Rays网上商店",//L"Allow Bobby Rays",
	L"随机选择起始位置",//L"Randomise Starting Edge",
	L"必须输入服务器名",//L"You must enter a server name",
	L"最大玩家数必须在2-4之间",//L"Max Players must be between 2 and 4",
	L"队伍大小必须在1-5之间",//L"Squad size must be between 1 and 5",
	L"当日时间",//L"Time of Day",
	L"当日时间必须是24小时制 (HH:MM)\n\n 例如 13:30 = 1.30pm",//L"Time of Day must be a 24 hr time (HH:MM)\n\n eg. 13:30 = 1.30pm",
	L"开始资金必须是整数（最小单位为1美元）\n\n 例如 150000" ,//L"Starting Cash must be a valid dollar amount ( no cents )\n\n eg. 150000" ,
	L"伤害系数",//L"Damage Multiplier",
	L"伤害系数必须于0-5之间",//L"Damage Multiplier must be a number between 0 and 5",
	L"回合计时器",//L"Turn Timer Multiplier",
	L"计时器必须设定在1-200之间",//L"Turn Timer multiplier must be a number between 1 and 200",
	L"合作模式中允许平民",//L"Enable Civilians in CO-OP",
	L"使用新携行模式(NIV)",//L"Use New Inventory (NIV)",
	L"强制执行最大AI敌人数量",//L"Enforce Maximum AI Enemies",
	L"同步多人模式终端目录",//L"Sync. MP Clients Directory",
	L"同步多人模式目录",//L"MP Sync. Directory",
	L"你必须进入一个文件传输目录",//L"You must enter a file transfer directory.",
	L"（使用 '/' 代替 '\\' 作为目录分隔符）",//L"(Use '/' instead of '\\' for directory delimiters.)",
	L"指定的同步目录不存在。",//L"The specified synchronisation directory does not exist.",
};

STR16 pDeliveryLocationStrings[] =
{
	L"奥斯汀",	//"Austin",			//Austin, Texas, USA
	L"巴格达",	//"Baghdad",			//Baghdad, Iraq (Suddam Hussein's home)
	L"Drassen",			//The main place in JA2 that you can receive items.  The other towns are dummy names...
	L"香港",	//"Hong Kong",		//Hong Kong, Hong Kong
	L"贝鲁特",	//"Beirut",			//Beirut, Lebanon	(Middle East)
	L"伦敦",	//"London",			//London, England
	L"洛杉矶",	//"Los Angeles",	//Los Angeles, California, USA (SW corner of USA)
	L"Meduna",			//Meduna -- the other airport in JA2 that you can receive items.
	L"Metavira",		//The island of Metavira was the fictional location used by JA1
	L"迈阿密",	//"Miami",				//Miami, Florida, USA (SE corner of USA)
	L"莫斯科",	//"Moscow",			//Moscow, USSR
	L"纽约",	//"New York",		//New York, New York, USA
	L"渥太华",	//"Ottawa",			//Ottawa, Ontario, Canada -- where JA2 was made!
	L"巴黎",	//"Paris",				//Paris, France
	L"的黎波里",	//"Tripoli",			//Tripoli, Libya (eastern Mediterranean)
	L"东京",	//"Tokyo",				//Tokyo, Japan
	L"温哥华",	//"Vancouver",		//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation.  It is possible to select 0 ability
	//in a skill meaning you can't use it.  This text is confirmation to the player.
	L"你确定吗？零意味着你不能拥有这项技能。",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"(最多8个字符)",	//"( 8 Characters Max )",
};

STR16 pIMPFinishButtonText[ 1 ]=
{
	L"分析...",
};

STR16 pIMPFinishStrings[ ]=
{
	L"谢谢你，%s",
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] =
{
	L"嗓音",
};

STR16 pDepartedMercPortraitStrings[ ]=
{
	L"阵亡",	//"Killed in Action",
	L"解雇",		//"Dismissed",
	L"其它",		//"Other",
};

// title for program
STR16 pPersTitleText[] =
{
	L"人事管理",
};

// paused game strings
STR16 pPausedGameText[] =
{
	L"游戏暂停",	//"Game Paused",
	L"继续游戏 (|P|a|u|s|e)",	//"Resume Game (|P|a|u|s|e)",
	L"暂停游戏 (|P|a|u|s|e)",	//"Pause Game (|P|a|u|s|e)",
};


STR16 pMessageStrings[] =
{
	L"退出游戏",	//"Exit Game?",
	L"确定",	//"OK",
	L"是",	//"YES",
	L"否",	//"NO",
	L"取消",	//"CANCEL",
	L"再次雇佣",	//"REHIRE",
	L"撒谎",	//"LIE",		//
	L"没有描述",	//"No description", //Save slots that don't have a description.
	L"游戏已保存。",	//"Game Saved.",
	L"游戏已保存。",	//"Game Saved.",
	L"QuickSave", 	//"QuickSave", //The name of the quicksave file (filename, text reference)
	L"SaveGame",	//"SaveGame",//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",				//The 3 character dos extension (represents sav)
	L"..\\SavedGames", //The name of the directory where games are saved.
	L"日",	//"Day",
	L"个佣兵",	//"Mercs",
	L"空",	//"Empty Slot", //An empty save game slot
	L"Demo",				//Demo of JA2
	L"Debug",				//State of development of a project (JA2) that is a debug build
	L"Release",			//Release build for JA2
	L"rpm",					//Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.
	L"分钟",	//"min",					//Abbreviation for minute.
	L"米",	//"m",						//One character abbreviation for meter (metric distance measurement unit).
	L"发",  //L"rnds",				//Abbreviation for rounds (# of bullets)
	L"公斤",	//"kg",					//Abbreviation for kilogram (metric weight measurement unit)
	L"磅",	//"lb",					//Abbreviation for pounds (Imperial weight measurement unit)
	L"主页",	//"Home",				//Home as in homepage on the internet.
	L"USD", //L"USD",					//Abbreviation to US dollars
	L"n/a",					//Lowercase acronym for not applicable.
	L"与此同时",	//"Meanwhile",	//Meanwhile
	L"%s已到达%s%s分区",	//"%s has arrived in sector %s%s", //Name/Squad has arrived in sector A9.  Order must not change without notifying																		//SirTech

	L"版本", //L"Version",
	L"没有快速存档",	//"Empty Quick Save Slot",
	L"该位置用来放Quick Save（快速存档）。请在战术屏幕或者地图屏幕按ALT+S进行快速存档。",
	L"打开的",	//"Opened",
	L"关闭的",	//"Closed",
	L"磁盘空间不足。只有%sMB可用空间，《铁血联盟2》需要%sMB。",
	L"从AIM雇佣了%s", 		//"Hired %s from AIM",
	L"%s抓住了%s.",	//"%s has caught %s.",	//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s打了针剂。", 	//"%s has taken the drug.", //'Merc name' has taken the drug
	L"%s没有医疗技能",	//"%s has no medical skill",//'Merc name' has no medical skill.

	//CDRom errors (such as ejecting CD while attempting to read the CD)
	L"The integrity of the game has been compromised.",
	L"ERROR: Ejected CD-ROM",

	//When firing heavier weapons in close quarters, you may not have enough room to do so.
	L"没有空间施展你的武器。",	//"There is no room to fire from here.",

	//Can't change stance due to objects in the way...
	L"现在无法改变姿势。",	//"Cannot change stance at this time.",

	//Simple text indications that appear in the game, when the merc can do one of these things.
	L"放下",	//"Drop",
	L"投掷",	//"Throw",
	L"交给",	//"Pass",

	L"把%s交给了%s。", //"%s passed to %s.", //"Item" passed to "merc".  Please try to keep the item %s before the merc %s, otherwise,
											 //must notify SirTech.
	L"没有足够空位以把%s交给%s",	//"No room to pass %s to %s.", //pass "item" to "merc".  Same instructions as above.

	//A list of attachments appear after the items.  Ex:  Kevlar vest ( Ceramic Plate 'Attached )'
	L"附加 )",	//" Attached )",

	//Cheat modes
	L"开启作弊等级一",	//"Cheat level ONE reached",
	L"开启作弊等级二",	//"Cheat level TWO reached",

	//Toggling various stealth modes
	L"小队进入潜行模式。",	//"Squad on stealth mode.",
	L"小队退出潜行模式。",	//"Squad off stealth mode.",
	L"%s进入潜行模式。",	//"%s on stealth mode.",
	L"%s退出潜行模式。",	//"%s off stealth mode.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in
	//an isometric engine.  You can toggle this mode freely in the game.
	L"打开显示轮廓",	//"Extra Wireframes On",
	L"关闭显示轮廓",	//"Extra Wireframes Off",

	//These are used in the cheat modes for changing levels in the game.  Going from a basement level to
	//an upper level, etc.
	L"无法从这层上去...",	//"Can't go up from this level...",
	L"没有更低的层了...",	//"There are no lower levels...",
	L"进入地下室%d层...",	//"Entering basement level %d...",
	L"离开地下室...",			//"Leaving basement...",

	L"的",	//"'s",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"关闭跟随模式。",	//"Follow mode OFF.",
	L"打开跟随模式。",	//"Follow mode ON.",
	L"不显示3D光标。",	//"3D Cursor OFF.",
	L"显示3D光标。",	//"3D Cursor ON.",
	L"第%d小队激活。",	//"Squad %d active.",
	L"你无法支付%s的%s日薪",	//"You cannot afford to pay for %s's daily salary of %s",	//first %s is the mercs name, the seconds is a string containing the salary
	L"跳过",			//"Skip",
	L"%s不能独自离开",	//"%s cannot leave alone.",
	L"一个文件名为SaveGame99.sav的存档被创建了。如果需要的话，将其更名为SaveGame01 - SaveGame10，然后你就能载入这个存档了。",	//"A save has been created called, SaveGame99.sav.  If needed, rename it to SaveGame01 - SaveGame10 and then you will have access to it in the Load screen.",
	L"%s喝了点%s",	//"%s drank some %s",
	L"Drassen收到了包裹。",	//"A package has arrived in Drassen.",
 	L"%s将到达指定的着陆点(分区%s)， 于%d日%s。",	//"%s should arrive at the designated drop-off point (sector %s) on day %d, at approximately %s.",		//first %s is mercs name, next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival
	L"日志已经更新。",	//"History log updated.",
	L"榴弹发射器点射时使用准星光标（可以扫射）",
	L"榴弹发射器连发时使用弹道光标(不可以扫射",	//"Grenade Bursts use Trajectory Cursor (Spread fire disabled)",
	L"启用物品全掉",	//"Drop All Enabled",
	L"禁用物品全掉",	//"Drop All Disabled",
	L"榴弹发射器以正常仰角发射榴弹",	//"Grenade Launchers fire at standard angles",
	L"榴弹发射器以较高仰角发射榴弹",	//L"Grenade Launchers fire at higher angles",
	// forced turn mode strings
    L"强制回合制模式",
	L"正常回合制模式",
	L"离开战斗",
	L"强制回合制模式启动，进入战斗",
#ifdef JA2BETAVERSION
	L"自动储存成功。",
#endif
	L"..\\SavedGames\\MP_SavedGames", //The name of the directory where games are saved.//84
	L"客户端", //"Client",
};


CHAR16 ItemPickupHelpPopup[][40] =
{
	L"确认",		//"OK",
	L"向上滚动",		//"Scroll Up",
	L"选择全部",		//"Select All",
	L"向下滚动",		//"Scroll Down",
	L"取消",		//"Cancel",
};

STR16 pDoctorWarningString[] =
{
	L"%s不够近，不能被治疗。",
	L"你的医生不能包扎完每个人。",
};

STR16 pMilitiaButtonsHelpText[] =
{
	L"调离(右击)/安置(左击)新兵",
	L"调离(右击)/安置(左击)老兵",
	L"调离(右击)/安置(左击)精兵",
	L"在各个分区平均分布民兵",
};

STR16 pMapScreenJustStartedHelpText[] =
{
	L"去AIM雇几个佣兵( *提示* 在笔记本电脑里)",
	L"当你准备出发前往Arulco，点击屏幕右下方的时间压缩按钮。",
};

STR16 pAntiHackerString[] =
{
	L"错误。丢失或损坏文件。游戏将退出。",
};


STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"查看邮件",
	L"浏览网页",
	L"查看文件和邮件的附件",
	L"阅读事件日志",
	L"查看队伍信息",
	L"查看财务简报和记录",
	L"关闭笔记本电脑",

	//Bottom task bar icons (if they exist):
	L"你有新的邮件",
	L"你有新的文件",

	//Bookmarks:
	L"国际佣兵联盟",
	L"Bobby Ray网上武器店",
	L"佣兵心理剖析研究所",
	L"廉价新兵中心",
	L"McGillicutty公墓",
	L"联合花卉服务公司",
	L"A.I.M指定保险代理人",
};


STR16	gzHelpScreenText[] =
{
	L"退出帮助屏幕",
};

STR16 gzNonPersistantPBIText[] =
{
	L"战斗正在进行中，你只能在战术屏幕进行撤退。",
	L"进入该分区，继续战斗。(|E)",
	L"自动解决这次战斗。(|A)",
	L"当你进攻时，不能自动解决战斗。",
	L"当你遭遇伏兵时，不能自动解决战斗。",
	L"当在矿井里和异形作战时，不能自动解决战斗。",
	L"还有敌对的平民时，不能自动解决战斗。",
	L"有血猫时，不能自动解决战斗。",
	L"战斗进行中",
	L"你不能在这时撤退。",
};

STR16 gzMiscString[] =
{
	L"在没有你的佣兵的支援下，民兵继续战斗...",
	L"现在车辆不需要加油。",	//"The vehicle does not need anymore fuel right now.",
	L"油箱装了%d％的油。",	//"The fuel tank is %d%% full.",
	L"Deidranna女王的军队重新完全占领了%s。",
	L"你丢失了加油点。",	//"You have lost a refueling site.",
};

STR16	gzIntroScreen[] =
{
	L"找不到视频文件",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH."
STR16 pNewNoiseStr[] =
{
	L"%s听到%s声音来自%s。",
	L"%s听到%s移动声来自%s。",
	L"%s听到%s吱吱声来自%s。",
	L"%s听到%s溅水声来自%s。",
	L"%s听到%s撞击声来自%s。",
	L"%s听到%s爆炸声发向%s。",
	L"%s听到%s尖叫声发向%s。",
	L"%s听到%s撞击声发向%s。",
	L"%s听到%s撞击声发向%s。",
	L"%s听到%s粉碎声来自%s。",
	L"%s听到%s破碎声来自%s。",
};

STR16 wMapScreenSortButtonHelpText[] =
{
	L"按姓名排序 (|F|1)",
	L"按任务排序 (|F|2)",
	L"按睡眠状态排序 (|F|3)",
	L"按地点排序 (|F|4)",
	L"按目的地排序 (|F|5)",
	L"按预计离队时间排序 (|F|6)",
};



STR16		BrokenLinkText[] =
{
	L"错误404",	//"Error 404",
	L"网站未找到",	//"Site not found.",
};


STR16 gzBobbyRShipmentText[] =
{
	L"近期运货",	//"Recent Shipments",
	L"订单 #",		//"Order #",
	L"物品数量",	//"Number Of Items",
	L"订购日期",		//"Ordered On",
};


STR16	gzCreditNames[]=
{
	L"Chris Camfield",
	L"Shaun Lyng",
	L"Kris M鋜nes",
	L"Ian Currie",
	L"Linda Currie",
	L"Eric \"WTF\" Cheng",
	L"Lynn Holowka",
	L"Norman \"NRG\" Olsen",
	L"George Brooks",
	L"Andrew Stacey",
	L"Scot Loving",
	L"Andrew \"Big Cheese\" Emmons",
	L"Dave \"The Feral\" French",
	L"Alex Meduna",
	L"Joey \"Joeker\" Whelan",
};


STR16	gzCreditNameTitle[]=
{
	L"Game Internals Programmer", 			// Chris Camfield
	L"Co-designer/Writer",							// Shaun Lyng
	L"Strategic Systems & Editor Programmer",					//Kris \"The Cow Rape Man\" Marnes
	L"Producer/Co-designer",						// Ian Currie
	L"Co-designer/Map Designer",				// Linda Currie
	L"Artist",													// Eric \"WTF\" Cheng
	L"Beta Coordinator, Support",				// Lynn Holowka
	L"Artist Extraordinaire",						// Norman \"NRG\" Olsen
	L"Sound Guru",											// George Brooks
	L"Screen Designer/Artist",					// Andrew Stacey
	L"Lead Artist/Animator",						// Scot Loving
	L"Lead Programmer",									// Andrew \"Big Cheese Doddle\" Emmons
	L"Programmer",											// Dave French
	L"Strategic Systems & Game Balance Programmer",					// Alex Meduna
	L"Portraits Artist",								// Joey \"Joeker\" Whelan",
};

STR16	gzCreditNameFunny[]=
{
	L"", 																			// Chris Camfield
	L"(still learning punctuation)",					// Shaun Lyng
	L"(\"It's done. I'm just fixing it\")",	//Kris \"The Cow Rape Man\" Marnes
	L"(getting much too old for this)",				// Ian Currie
	L"(and working on Wizardry 8)",						// Linda Currie
	L"(forced at gunpoint to also do QA)",			// Eric \"WTF\" Cheng
	L"(Left us for the CFSA - go figure...)",	// Lynn Holowka
	L"",																			// Norman \"NRG\" Olsen
	L"",																			// George Brooks
	L"(Dead Head and jazz lover)",						// Andrew Stacey
	L"(his real name is Robert)",							// Scot Loving
	L"(the only responsible person)",					// Andrew \"Big Cheese Doddle\" Emmons
	L"(can now get back to motocrossing)",	// Dave French
	L"(stolen from Wizardry 8)",							// Alex Meduna
	L"(did items and loading screens too!)",	// Joey \"Joeker\" Whelan",
};

// HEADROCK: Adjusted strings for better feedback, and added new string for LBE repair.
STR16 sRepairsDoneString[] =
{
	L"%s 修复了自己的物品。",
	L"%s 修复了所有人的枪和护甲。",
	L"%s 修复了所有人的装备。",
	L"%s 修复所有人携带的大型物品",//L"%s finished repairing everyone's large carried items",
	L"%s 修复所有人携带的中型物品",//L"%s finished repairing everyone's medium carried items",
	L"%s 修复所有人携带的小型物品",//L"%s finished repairing everyone's small carried items",
	L"%s 修复所有人的携行具",//L"%s finished repairing everyone's LBE gear",
};

STR16 zGioDifConfirmText[]=
{
	L"你选择了“新手”模式。这个设置是为那些刚玩铁血联盟的玩家准备的，他们刚接触策略游戏，或者他们希望快点结束战斗。你的选择会在整个游戏中生效，所以请作出明智的选择。你真的要玩“新手”模式吗？",
	L"你选择了“老手”模式。这个设置是为那些已经熟悉铁血联盟或类似游戏的玩家准备的。你的选择会在整个游戏中生效，所以请作出明智的选择。你真的要玩“老手”模式吗？",
	L"你选择了“专家”模式。我们警告你，如果你被装在尸袋里运回来，不要来向我们抱怨。你的选择会在整个游戏中生效，所以请作出明智的选择。你真的要玩“专家”模式吗？",
	L"你选择了“疯狂”模式。警告: 如果你被装在塑料袋里一块块运回来，不要来向我们抱怨。女王会狠狠地凌虐你。你的选择会在整个游戏中生效，所以请作出明智的选择。你真的要玩“疯狂”模式吗？",
};

STR16 gzLateLocalizedString[] =
{
	L"没有找到loadscreen数据文件%S...",	//"%S loadscreen data file not found...",

	//1-5
	L"由于没有人在用遥控器，机器人无法离开本分区。",

	//This message comes up if you have pending bombs waiting to explode in tactical.
	L"你现在无法压缩时间。请等待炸弹爆炸！",

	//'Name' refuses to move.
	L"%s拒绝移动。",

	//%s a merc name
	L"%s精力不足，无法改变姿势。",	//"%s does not have enough energy to change stance.",

	//A message that pops up when a vehicle runs out of gas.
	L"%s汽油耗尽，现在在%c%d抛锚了。",	//"The %s has run out of gas and is now stranded in %c%d.",

	//6-10

	// the following two strings are combined with the pNewNoise[] strings above to report noises
	// heard above or below the merc
	L"上方",
	L"下方",

	//The following strings are used in autoresolve for autobandaging related feedback.
	L"佣兵中没人有医疗技能。",
	L"没有足够的医疗物品进行包扎。",
	L"没有足够的医疗物品给所有人进行包扎。",
	L"佣兵中没人需要包扎。",	//"None of your mercs need bandaging.",
	L"自动包扎佣兵。",	//"Bandages mercs automatically.",
	L"全部佣兵已被包扎完毕。",	//"All your mercs are bandaged.",

	//14
	L"Arulco",

	L"屋顶",

	L"生命: %d/%d",

	//In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8"
	//"vs." is the abbreviation of versus.
	L"%d vs. %d",

	L"%s满了。",

  L"%s现在不用包扎，他（她）需要认真的治疗和休息。",

	//20
	//Happens when you get shot in the legs, and you fall down.
	L"%s被击中腿部，并且倒下了！",
	//Name can't speak right now.
	L"%s现在不能说话。",

	//22-24 plural versions
	L"%d个新兵被提升为精兵。",
	L"%d个新兵被提升为老兵。",
	L"%d个老兵被提升为精兵。",

	//25
	L"开关",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"%疯狂了！",

	//27-28
	//Messages why a player can't time compress.
	L"现在压缩时间不安全，因为你有佣兵在分区%s。",
	L"现在压缩时间不安全，因为你有佣兵在被异形感染的矿井。",

	//29-31 singular versions
	L"1个新兵被晋升为精兵。",
	L"1个新兵被晋升为老兵。",
	L"1个老兵被晋升为精兵。",

	//32-34
	L"%s无语。",	//"%s doesn't say anything.",
	L"回到地面？",
	L"(第%d小队)",	//"(Squad %d)",

	//35
	//Ex: "Red has repaired Scope's MP5K".  Careful to maintain the proper order (Red before Scope, Scope before MP5K)
	L"%s修复了%s的%s。",

	//36
	L"血猫",

	//37-38 "Name trips and falls"
	L"%s踩到陷阱，跌倒了。",
	L"这个物品不能从这里捡起。",

	//39
	L"你现有的佣兵中没人能进行战斗。民兵们将独自和异形作战。",

	//40-43
	//%s is the name of merc.
	L"%s用完了医药箱里的药品！",	//"%s ran out of medical kits!",
	L"%s没有所需技能来医疗他人！",	//"%s lacks the necessary skill to doctor anyone!",
	L"%s用完工具箱里的工具！",	//"%s ran out of tool kits!",
	L"%s没有所需技能来修理物品！",	//"%s lacks the necessary skill to repair anything!",

	//44-45
	L"Repair Time",
	L"%s看不到这个人。",

	//46-48
	L"%s的增程枪管掉下来了！",	//"%s's gun barrel extender falls off!",
	L"在每个分区，只允许有不多于%d名佣兵训练民兵。",	//"No more than %d militia trainers are permitted per sector.",
  	L"你确定吗？",			//"Are you sure?",

	//49-50
	L"时间压缩",
	L"车辆的油箱已经加满油了。",

	//51-52 Fast help text in mapscreen.
	L"继续时间压缩 (|S|p|a|c|e)",
	L"停止时间压缩 (|E|s|c)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11"
	L"%s修理好了卡壳的%s",	//L"%s has unjammed the %s",
	L"%s修理好了卡壳的%s的%s",	//L"%s has unjammed %s's %s",

	//55
	L"查看分区存货时候无法压缩时间",	//L"Can't compress time while viewing sector inventory.",

	L"The Jagged Alliance 2 v1.13 PLAY DISK was not found. Program will now exit.",

	L"物品组合成功。",

	//58
	//Displayed with the version information when cheats are enabled.
	L"当前/最大进展:  %d%％/%d%％",		//"Current/Max Progress: %d%%/%d%%",//zww

	L"护送John和Mary？",

	// 60
	L"开关被激活",		//"Switch Activated.",

	L"%s的陶瓷片已经粉碎了!",	//"%s's ceramic plates have been smashed!",
	L"%s多打了%d发子弹！",	//"%s fires %d more rounds than intended!",
	L"%s多打了%d发子弹！",	//"%s fires %d more round than intended!",
};

STR16 gzCWStrings[] =
{
	L"是否呼叫邻近区域的援兵？",
};

// WANNE: Tooltips
STR16 gzTooltipStrings[] =
{
	// Debug info
	L"%s|位|置: %d\n",
	L"%s|亮|度: %d / %d\n",
	L"%s|目|标|距|离: %d\n",
	L"%s|I|D: %d\n",
	L"%s|订|单: %d\n",
	L"%s|属|性: %d\n",
	L"%s|当|前 |A|Ps: %d\n",
	L"%s|当|前 |生|命: %d\n",
	// Full info
	L"%s|头|盔: %s\n",
	L"%s|防|弹|衣: %s\n",
	L"%s|作|战|裤: %s\n",
	// Limited, Basic
	L"|护|甲: ",
	L"头盔 ",
	L"防弹衣 ",
	L"作战裤",
	L"装备了",
	L"无护甲",
	L"%s|夜|视|仪: %s\n",
	L"无夜视仪",
	L"%s|防|毒|面|具: %s\n",
	L"无防毒面具",
	L"%s|头|部|1: %s\n",
	L"%s|头|部|2: %s\n",
	L"\n(背包内) ",
	L"%s|武|器: %s ",
	L"空手",
	L"手枪",
	L"冲锋枪",
	L"步枪",
	L"机枪",
	L"霰弹枪",
	L"刀",
	L"重武器",
	L"无头盔",
	L"无防弹衣",
	L"无作战裤",
	L"|护|甲: %s\n",
};

STR16 New113Message[] =
{
	L"风暴开始了。",
	L"风暴结束了。",
	L"下雨了。",
	L"雨停了。",
	L"小心狙击手……",
	L"火力压制！",
	L"点射",
	L"自动",
	L"榴弹",
	L"榴弹点射",
	L"榴弹自动",
	L"狙击手！",
	L"已经点选物品，此时无法分钱。",
	L"新兵的会合地被挪至 %s, 因降落地点 %s 目前由敌人占据。",
	L"物品销毁",
	L"此类物品全部销毁",
	L"物品卖出",
	L"此类物品全部卖出",
	L"你得检查一下你的眼部装备",
};

STR16 New113HAMMessage[] =
{
	L"%s 害怕到抖缩。",//	L"%s cowers in fear!",
};

// WANNE: This are the email texts, when one of the 4 new 1.13 MERC mercs have levelled up, that Speck sends
// INFO: Do not replace the ?characters. They indicate the <B2> (-> Newline) from the edt files
STR16	New113MERCMercMailTexts[] =
{
	// Gaston: Text from Line 39 in Email.edt
	L"鉴于Gaston最近发挥异常突出，他的服务费也跟着上涨。以我个人的观点，这一点也不让我惊讶。??Speck T. Kline ?",
	// Stogie: Text from Line 43 in Email.edt
	L"请注意，Stogie近期能力有所提升，他的价格也要随之增长。 ??Speck T. Kline ?",
	// Tex: Text from Line 45 in Email.edt
	L"请注意，因为Tex新获得的经验， 更高的身价才匹配他的能力。 ??Speck T. Kline ?",
	// Biggens: Text from Line 49 in Email.edt
	L"鉴于Biggins发挥有所提高， 他的价格也同时上涨。 ??Speck T. Kline ?",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ?characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper
	L"狙击手: 拥有鹰的眼睛和百步穿杨的枪法！?",
	// Camouflage
	L"伪装: 跟你的伪装迷彩比起来，树丛看起来倒象是假的！?",
};

STR16 NewInvMessage[] =
{
    L"此时无法拾起背包",
	L"背包中无处可放",
	L"没发现背包",
	L"拉锁只在战斗中有效",
	L"背包拉锁打开时无法移动",
	L"你确定要变卖该地区所有物品吗？",
	L"你确定要销毁该地区所有物品吗？",
	L"装备大背包后无法爬上房顶",
};

// WANNE - MP: Multiplayer messages
STR16 MPServerMessage[] =
{
	// 0
	L"启动RakNet服务器...",
	L"服务器启动,等待连接...",
	L"你现在必须按'2'来连接你的客户端和服务器。",
	L"服务器已经在运行。",
	L"服务器启动失败。终止中。",
	// 5
	L"%d/%d 客户端已经准备好进入即时模式。",
	L"服务器断开并关闭。",
	L"服务器没有运行。",
	L"客户端仍在载入, 请稍等...",
	L"服务器启动之后你无法更改着陆点。",
	// 10
	L"已发送文件 '%S' - 100/100",//L"Sent file '%S' - 100/100",
	L"文件已发送到 '%S' ",//	L"Finished sending files to '%S'.",
	L"开始发送文件到 '%S'.",//	L"Started sending files to '%S'.",
};

STR16 MPClientMessage[] =
{
	// 0
	L"启动RakNet客户端...",
	L"正在连接IP: %S ...",
	L"接受游戏设置: ",
	L"你已经连接上了。",
	L"你已经开始连接...",
	// 5
	L"客户端 #%d - '%S' 已被雇佣 '%s'。",
	L"客户端 #%d - '%S' 已雇佣另一名佣兵。",
	L"你已准备 - 准备就绪 = %d/%d.",
	L"你不再准备 - 准备就绪 = %d/%d.",
	L"开始战斗...",
	// 10
	L"客户端 #%d - '%S' 已准备 - 准备就绪 = %d/%d.",
	L"客户端 #%d - '%S' 不再准备 - 准备就绪 = %d/%d",
	L"你已准备。 等候其它客户端... 按'OK'如果你已准备就绪。",
	L"让我们开始战斗！",
	L"要开始游戏必须运行一台客户端。",
	// 15
	L"游戏无法开始。没有佣兵被雇佣...",
	L"等待服务器解锁便携电脑出现'OK'...",
	L"中断",
	L"中断结束",
	L"鼠标方格坐标: ",
	// 20
	L"X: %d, Y: %d",
	L"坐标值: %d",
	L"服务器独占模式",
	L"手动选择服务器优先级:  ('1' - 授权 便携电脑/雇佣)  ('2' - 启动/载入 级别)  ('3' - 解锁 UI)  ('4' - 完成设置)",
	//L"Sector=%s, Max Clients=%d, Max Mercs=%d, Game_Mode=%d, Same Merc=%d, Damage Multiplier=%f, Enemies=%d, Creatures=%d, Militias=%d, Civilians=%d, Timed Turns=%d, Secs/Tic=%d, Starting Cash=$%d, Tons of Guns=%d, Sci-Fi=%d, Difficulty=%d, Iron-Man=%d, BobbyRays Range=%d, Dis BobbyRay=%d, Dis Aim/Merc Equip=%d, Dis Morale=%d, Testing=%d",
	L"分区=%s, 最大客户端数=%d, 最大佣兵数=%d, 游戏模式=%d, 同一佣兵=%d, 伤害倍数=%f, 时间前进=%d, 秒/Tic=%d, 取消 BobbyRay=%d, 取消 Aim/Merc 装备=%d, 取消士气=%d, 测试=%d",

	// 25
	L"测试与欺骗功能键'9'已被激活。",
	L"新建连接: 客户端 #%d - '%S'.",
	L"队: %d.",//not used any more
	L"'%s' (客户端 %d - '%S') 已被 '%s' (客户端 %d - '%S' 杀死)",
	L"踢出客户端 #%d - '%S'",
	// 30
	L"开始排序客户端号: #1 - '%S' | #2 - '%S' | #3 - '%S' | #4 - '%S'",
	L"开始客户端 #%d",
	L"请求即时模式...",
	L"转回即时模式。",
	L"错误: 转回过程中出现错误。",
	// 35
	L"要解锁笔记本电脑开始雇佣吗? (连接所有客户端?)",
	L"服务器已经解锁笔记本电脑。开始雇佣！",
	L"中断。",
	L"你无法更改着陆点，如果你只是客户端而不是服务器。",
	L"你拒绝投降, 因为你在一个多人游戏中。",
	// 40
	L"你所有的佣兵全部死亡！",
	L"激活观看模式。",
	L"你已被击败！",
	L"对不起, 在多人游戏中无法攀登。",
	L"你雇佣了 '%s'",
	// 45
	L"当开始购买后你不能更换地图",//	L"You cant change the map once purchasing has commenced",
	L"地图改为 '%s'",//	L"Map changed to '%s'",
	L"玩家 '%s' 断开连接, 踢出游戏",//	L"Client '%s' disconnected, removing from game",
	L"您已经断开连接，返回主菜单",//	L"You were disconnected from the game, returning to the Main Menu",
	L"连接失败, 5秒内重试, 还有 %i 次重试机会……",//	L"Connection failed, Retrying in 5 seconds, %i retries left...",
	//50
	L"连接失败，放弃……",//	L"Connection failed, giving up...",
	L"在另一个玩家进入前您无法开始游戏",//	L"You cannot start the game until another player has connected",
	L"%s : %s",
	L"发送给所有人",//	L"Send to All",
	L"发送给盟友",//	L"Allies only",
	// 55
	L"此游戏已经开始，无法加入。",//	L"Cannot join game. This game has already started.",
	L"%s (team): %s",
	L"玩家 #%i - '%s'",//	L"Client #%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"已收到服务器的所有文件。",//	L"Received all files from server.",
	L"'%S' 完成下载。",//	L"'%S' finished downloading from server.",
	L"'%S' 开始从服务器下载。",//	L"'%S' started downloading from server".,
	L"在全部客户端的文件未接收完以前不能开始游戏",//	L"Cannot start the game until all clients have finished receiving files",
	L"你需要下载修改后的文件才能继续游戏, 你想继续吗？",//	L"This server requires that you download modified files to play, do you wish to continue?",
	// 65
	L"点击 '准备' 进入战术画面。",//	L"Press 'Ready' to enter tactical screen.",
	L"Cannot connect because your version %S is different from the server version %S.",
};

STR16 MPHelp[] =
{
	// 0
	L"欢迎进入铁血联盟2 v1.13 多人游戏。",
	L"帮助按 'F1' ",
	L"多人控制（在战略画面）",
	L"* 第一次建立 ja2_mp.ini *",
	L"'1' - 启动服务器",
	// 5
	L"'2' - 连接服务器",
	L"'3' - 如果服务器解锁笔记本电脑, 设置好客户端准备战斗",
	L"'4' - 退出服务器和客户端",
	L"'5' - 显示鼠标坐标（在战术画面）",
	L"'7' - 对服务器优先级面板弹出对话框",
	// 10
	L"'F2' - 显示二级帮助",
	L"了解详情请看自述文件",
	L"技巧: (如果 ja2_mp.ini 已建立)",
	L"* 确认所以的客户端都有独立的客户端号码 *",
	L"* 游戏存储不记录bobby rays订单 *",
	// 15
	L"* 避免将对立的佣兵放在一起 *",
	L"'F1' - 显示基本帮助",
};

STR16 gszMPEdgesText[] =
{
	L"N",
	L"S",
	L"E",
	L"W"
};

STR16 gszMPTeamNames[] =
{
	L"Foxtrot",
	L"Bravo",
	L"Delta",
	L"Charlie"
};

STR16 gszMPMapscreenText[] =
{
	L"游戏类型: ",//	L"Game Type: ",
	L"玩家: ",//	L"Players: ",
	L"所拥有的佣兵: ",//	L"Mercs each: ",
	L"手提电脑开启时你无法开始移动。",//	L"You cannot change starting edge once Laptop is unlocked.",
	L"手提电脑开启后你无法更换队伍。",//	L"You cannot change teams once the Laptop is unlocked.",
	L"随机佣兵: ",//	L"Random Mercs: ",
	L"Y",
	L"难度:",//	L"Difficulty:",
	L"Server Version:",
};

STR16 gzMPSScreenText[] =
{
	L"记分板",//	L"Scoreboard",
	L"继续",//	L"Continue",
	L"取消",//	L"Cancel",
	L"玩家",//	L"Player",
	L"杀人数",//	L"Kills",
	L"死亡数",//	L"Deaths",
	L"女王的部队",//	L"Queen's Army",
	L"命中数",//	L"Hits",
	L"脱靶数",//	L"Misses",
	L"准确度",//	L"Accuracy",
	L"损害量",//	L"Damage Dealt",
	L"受损量",//	L"Damage Taken",
	L"请等待服务器指令按‘继续’。"	//L"Please wait for the server to press 'Continue'."
};

STR16 gzMPCScreenText[] =
{
	L"取消",//	L"Cancel",
	L"连接到服务器",//	L"Connecting to Server",
	L"获得服务器设置",//	L"Getting Server Settings",
	L"下载定制文件",//	L"Downloading custom files",
	L"按 'ESC' 取消， 'Y' 开始聊天",//	L"Press 'ESC' to cancel or 'Y' to chat",
	L"按 'ESC' 取消",//	L"Press 'ESC' to cancel",
	L"准备"//	L"Ready"
};

STR16 gzMPChatToggleText[] =
{
	L"发送给所有人",//	L"Send to All",
	L"发送给盟友",//	L"Send to Allies only",
};

STR16 gzMPChatboxText[] =
{
	L"多人聊天",//	L"Multiplayer Chat",
	L"聊天: 按 'Enter' 发送， 'Esc' 取消。",//	L"Chat: Press 'ENTER' to send of 'ESC' to cancel.",
};

// WANNE: Some Chinese specific strings that needs to be in unicode!
STR16 ChineseSpecString1 = L"%％";			//defined in _ChineseText.cpp as this file is already unicode
STR16 ChineseSpecString2 = L"*%3d%％%%";	//defined in _ChineseText.cpp as this file is already unicode
STR16 ChineseSpecString3 = L"%d%％";		//defined in _ChineseText.cpp as this file is already unicode
STR16 ChineseSpecString4 = L"%s (%s) [%d%％]\n%s %d\n%s %d\n%s %d (%d)\n%s (%d) %s\n%s %1.1f %s";
STR16 ChineseSpecString5 = L"%s [%d%％]\n%s %d\n%s %d\n%s %1.1f %s";
STR16 ChineseSpecString6 = L"%s [%d%％]\n%s %d%％ (%d/%d)\n%s %d%％\n%s %1.1f %s";
STR16 ChineseSpecString7 = L"%s [%d%％]\n%s %1.1f %s";

#endif //CHINESE
