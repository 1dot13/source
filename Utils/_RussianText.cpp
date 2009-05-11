#pragma setlocale("RUSSIAN")
#ifdef PRECOMPILEDHEADERS
#include "Utils All.h"
#else
#endif
#include "Language Defines.h"
#include "text.h"
#include "Fileman.h"

#ifdef RUSSIAN

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
	L"@T,{;Разработчики JA2 v1.13",
	L"@T,C144,R134,{;Программирование",
	L"@T,C144,R134,{;Графика и звук",
	L"@};(Многое взято из других модов)",
	L"@T,C144,R134,{;Предметы",
	L"@T,C144,R134,{;Также помогали",
	L"@};(И многие другие, предложившие хорошие идеи и высказавшие важные замечания!)",
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
//	L",38 кал",
//	L"9мм",
//	L",45 кал",
//	L",357 кал",
//	L"12 кал",
//	L"ОББ",
//	L"5,45мм",
//	L"5,56мм",
//	L"7,62мм НАТО",
//	L"7,62мм ВД",
//	L"4,7мм",
//	L"5,7мм",
//	L"Монстр",
//	L"Ракета",
//	L"", // дротик
//	L"", // пламя
////	L".50 cal", // barrett
////	L"9mm Hvy", // Val silent
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
//	L",38 кал",
//	L"9мм",
//	L",45 кал",
//	L",357 кал",
//	L"12 кал",
//	L"ОББ",
//	L"5,45мм",
//	L"5,56мм",
//	L"7,62мм Н.",
//	L"7,62мм ВД",
//	L"4,7мм",
//	L"5.7мм",
//	L"Монстр",
//	L"Ракета",
//	L"", // дротик
////	L"", // flamethrower
////	L".50 cal", // barrett
////	L"9mm Hvy", // Val silent
//};


CHAR16 WeaponType[MAXITEMS][30] =
{
	L"",			//Other
	L"Пистолет",		//Pistol
	L"Авт.пистолет",	//MP 'Автоматический пистолет'
	L"ПП",			//SMG 'Пистолет-пулемет'
	L"Винтовка",		//Rifle
	L"Сн.винтовка",		//Sniper rifle 'Снайперская винтовка'
	L"Шт.винтовка",		//Assault rifle 'Штурмовая винтовка'
	L"Ручной пулемет",	//LMG 'Ручной пулемет'
	L"Ружье",		//Shotgun 'Гладкоствольное ружье'
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"Ход Игрока", // player's turn
	L"Ход Противника",
	L"Ход Тварей",
	L"Ход Ополчения",
	L"Ход Гражданских",
	L"Player_Plan",// planning turn
	L"Client №1",//hayden
	L"Client №2",//hayden
	L"Client №3",//hayden
	L"Client №4",//hayden
};

CHAR16 Message[][STRING_LENGTH] =
{
	L"",

	// In the following 8 strings, the %s is the merc's name, and the %d (if any) is a number.

	L"%s получает ранение в голову и теряет в интеллекте!",
	L"%s получает ранение в плечо и теряет в ловкости!",
	L"%s получает ранение в грудь и теряет в силе!",
	L"%s получает ранение в ногу и теряет в проворности!",
	L"%s получает ранение в голову и теряет %d единиц интеллекта!",
	L"%s получает ранение в плечо и теряет %d единиц ловкости!",
	L"%s получает ранение в грудь и теряет %d единиц силы!",
	L"%s получает ранение в ногу и теряет %d единиц проворности!",
	L"Перехват!",

	// The first %s is a merc's name, the second is a string from pNoiseVolStr,
	// the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

	L"", //OBSOLETE
	L"К вам на помощь прибыло подкрепление!",

	// In the following four lines, all %s's are merc names

	L"%s перезаряжает оружие.",
	L"%s недостаточно очков действия!",
	L"%s оказывает первую помощь (любая клавиша - отмена).",
	L"%s и %s оказывают первую помощь (любая клавиша - отмена).",
	// the following 17 strings are used to create lists of gun advantages and disadvantages
	// (separated by commas)
	L"надёжно",
	L"ненадёжно",
	L"простой ремонт",
	L"сложный ремонт",
	L"большой урон",
	L"малый урон",
	L"скорострельное",
	L"нескорострельное",
	L"дальний бой",
	L"ближний бой",
	L"лёгкое",
	L"тяжёлое",
	L"компактное",
	L"очередями",	//fast burst fire
	L"нет отсечки очереди",
	L"бол.магазин",
	L"мал.магазин",

	// In the following two lines, all %s's are merc names

	L"%s: камуфляжная краска стёрлась.",
	L"%s: камуфляжная краска смылась.",

	// The first %s is a merc name and the second %s is an item name

	L"Второе оружие: закончились патроны!",
	L"%s крадёт %s.",

	// The %s is a merc name

	L"%s: оружие не стреляет очередями.",

	L"Уже установлено!",
	L"Объединить?",

	// Both %s's are item names

	L"Нельзя присоединить %s к %s.",

	L"Ничего",
	L"Разрядить", 
	L"Навеска",
	
	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex:  You cannot use sun goggles and you gas mask at the same time.
	L"Нельзя использовать %s и %s одновременно.",

	L"Этот предмет можно присоединить к другим предметам, поместив его в одно из четырех мест для приспособлений.",
	L"Этот предмет можно присоединить к другим предметам, поместив его в одно из четырех мест для приспособлений. (Однако эти предметы несовместимы)",
	L"В секторе еще остались враги!",
	L"%s требует полную оплату, нужно заплатить еще %s", 
	L"%s: попадание в голову!",
	L"Покинуть битву?",
	L"Это несъемное приспособление. Установить его?",
	L"%s чувствует прилив энергии!",
	L"%s поскальзывается на стеклянных шариках!",
	L"%s не удалось отобрать %s у врага!",
	L"%s чинит %s",
	L"Перехватили ход: ",
	L"Сдаться?",
	L"Человек отверг вашу помощь.",
	L"Вам это надо?",
  L"Чтобы воспользоваться вертолетом Небесного Всадника - выберите 'Машина/Вертолет'.",
	L"%s успевает зарядить только одно оружие.", //%s only had enough time to reload ONE gun
	L"Ход Кошек-Убийц", //Bloodcats' turn
	L"автоматический", //full auto
	L"неавтоматический", //no full auto
	L"точный", //accurate
	L"неточный", //inaccurate
	L"нет одиночных", //no semi auto
	L"Враг обобран до нитки!",
	L"У врага в руках ничего нет!",
};


// the names of the towns in the game

CHAR16 pTownNames[MAX_TOWNS][MAX_TOWN_NAME_LENGHT] =
{
	L"",
	L"Омерта",
	L"Драссен",
	L"Альма",
	L"Грам",
	L"Тикса",
	L"Камбрия",
	L"Сан-Мона",
	L"Эстони",
	L"Орта",
	L"Балайм",
	L"Медуна",
	L"Читзена",
};

// the types of time compression. For example: is the timer paused? at normal speed, 5 minutes per second, etc.
// min is an abbreviation for minutes

STR16 sTimeStrings[] =
{
	L"Пауза",
	L"Норма",
	L"5 мин",
	L"30 мин",
	L"60 мин",
	L"6 часов",
};


// Assignment Strings: what assignment does the merc  have right now? For example, are they on a squad, training, 
// administering medical aid (doctor) or training a town. All are abbreviated. 8 letters is the longest it can be.

STR16 pAssignmentStrings[] =
{
	L"Отряд 1",
	L"Отряд 2",
	L"Отряд 3",
	L"Отряд 4",
	L"Отряд 5",
	L"Отряд 6",
	L"Отряд 7",
	L"Отряд 8",
	L"Отряд 9",
	L"Отряд 10",
	L"Отряд 11",
	L"Отряд 12",
	L"Отряд 13",
	L"Отряд 14",
	L"Отряд 15",
	L"Отряд 16",
	L"Отряд 17",
	L"Отряд 18",
	L"Отряд 19",
	L"Отряд 20",
	L"На службе",	// on active duty
	L"Медик",	// administering medical aid
	L"Пациент",	// getting medical aid
	L"Транспорт",	// in a vehicle
	L"В пути",	// in transit - abbreviated form
	L"Ремонт",	// repairing
	L"Практика",	// training themselves  
  L"Ополчение",		// training a town to revolt 
	L"Тренер",	// training a teammate
	L"Ученик",	// being trained by someone else 
	L"Мертв",	// dead
	L"Недеесп.",	// abbreviation for incapacitated
	L"В плену",	// Prisoner of war - captured
	L"Госпиталь",	// patient in a hospital 
	L"Пуст",	// Vehicle is empty
};


STR16 pMilitiaString[] =
{
	L"Ополчение", // the title of the militia box
	L"Запас", //the number of unassigned militia troops
	L"Нельзя перераспределять ополчение, когда враг находится в этом районе!",
};


STR16 pMilitiaButtonString[] =
{
	L"Авто", // auto place the militia troops for the player
	L"Готово", // done placing militia troops
};

STR16 pConditionStrings[] = 
{
	L"Отличное",	//the state of a soldier .. excellent health
	L"Хорошее",	//good health
	L"Сносное",	//fair health
	L"Ранен",	//wounded health
	L"Устал",	//tired
	L"Кровотечение", //bleeding to death
	L"Без сознания", //knocked out 
	L"Умирает",	//near death
	L"Мертв",	//dead
};

STR16 pEpcMenuStrings[] =
{
	L"Сражаться", // set merc on active duty
	L"Пациент", // set as a patient to receive medical aid
	L"Транспорт", // tell merc to enter vehicle
	L"Без эскорта", // let the escorted character go off on their own
	L"Отмена", // close this menu
};


// look at pAssignmentString above for comments

STR16 pPersonnelAssignmentStrings[] =
{
	L"Отряд 1",
	L"Отряд 2",
	L"Отряд 3",
	L"Отряд 4",
	L"Отряд 5",
	L"Отряд 6",
	L"Отряд 7",
	L"Отряд 8",
	L"Отряд 9",
	L"Отряд 10",
	L"Отряд 11",
	L"Отряд 12",
	L"Отряд 13",
	L"Отряд 14",
	L"Отряд 15",
	L"Отряд 16",
	L"Отряд 17",
	L"Отряд 18",
	L"Отряд 19",
	L"Отряд 20",
	L"На службе",
	L"Медик",
	L"Пациент",
	L"Транспорт",
	L"В пути",
	L"Ремонт",
	L"Практика", 
  L"Ополчение",
	L"Тренер",
	L"Ученик", 
	L"Мертв",
	L"Недеесп.",
	L"В плену",
	L"Госпиталь", 
	L"Пуст",	// Vehicle is empty
};


// refer to above for comments

STR16 pLongAssignmentStrings[] =
{
	L"Отряд 1",
	L"Отряд 2",
	L"Отряд 3",
	L"Отряд 4",
	L"Отряд 5",
	L"Отряд 6",
	L"Отряд 7",
	L"Отряд 8",
	L"Отряд 9",
	L"Отряд 10",
	L"Отряд 11",
	L"Отряд 12",
	L"Отряд 13",
	L"Отряд 14",
	L"Отряд 15",
	L"Отряд 16",
	L"Отряд 17",
	L"Отряд 18",
	L"Отряд 19",
	L"Отряд 20",
	L"На службе",
	L"Медик",
	L"Пациент",
	L"Транспорт",
	L"В пути",
	L"Ремонт",
	L"Практика", 
  L"Ополчение",
	L"Тренер",
	L"Ученик", 
	L"Мертв",
	L"Недеесп.",
	L"В плену",
	L"Госпиталь", // patient in a hospital 
	L"Пуст",	// Vehicle is empty
};


// the contract options

STR16 pContractStrings[] =
{
	L"Изменение контракта:", 
	L"", // a blank line, required
	L"Продлить на 1 день", // offer merc a one day contract extension
	L"Продлить на 7 дней", // 1 week
	L"Продлить на 14 дней", // 2 week
	L"Уволить", // end merc's contract
	L"Отмена", // stop showing this menu
};

STR16 pPOWStrings[] =
{
	L"В плену",  //an acronym for Prisoner of War
	L"??",
};

STR16 pLongAttributeStrings[] =
{
    L"СИЛА", 
	L"ЛОВКОСТЬ",  
	L"ПРОВОРНОСТЬ", 
	L"ИНТЕЛЛЕКТ", 
	L"МЕТКОСТЬ",
	L"МЕДИЦИНА",
	L"МЕХАНИКА",
	L"ЛИДЕРСТВО",
	L"ВЗРЫВЧАТКА",
	L"УРОВЕНЬ",
};

STR16 pInvPanelTitleStrings[] =
{
	L"Броня", // the armor rating of the merc
	L"Вес", // the weight the merc is carrying
	L"Камуф.", // the merc's camouflage rating
	L"Камуфляж:",
	L"Броня:",
};

STR16 pShortAttributeStrings[] =
{
	L"Прв", // the abbreviated version of : agility
	L"Лов", // dexterity
	L"Сил", // strength
	L"Лид", // leadership
	L"Инт", // wisdom
	L"Опт", // experience level
	L"Мет", // marksmanship skill
	L"Взр", // explosive skill
	L"Мех", // mechanical skill
	L"Мед", // medical skill
};


STR16 pUpperLeftMapScreenStrings[] =
{
	L"Назначение", // the mercs current assignment 
	L"Контракт", // the contract info about the merc
	L"Здоровье", // the health level of the current merc
	L"Боев.дух", // the morale of the current merc
	L"Сост.",	// the condition of the current vehicle
	L"Бензин",	// the fuel level of the current vehicle
};

STR16 pTrainingStrings[] =
{
	L"Тренинг", // tell merc to train self 
    L"Ополчение", // tell merc to train town 
	L"Тренер", // tell merc to act as trainer
	L"Ученик", // tell merc to be train by other 
};

STR16 pGuardMenuStrings[] =
{
	L"Ведение огня:", // the allowable rate of fire for a merc who is guarding
	L" Агрессивная атака", // the merc can be aggressive in their choice of fire rates
	L" Беречь патроны", // conserve ammo 
	L" Воздержаться от стрельбы", // fire only when the merc needs to 
	L"Другие параметры:", // other options available to merc
	L" Может отступить", // merc can retreat
	L" Может искать укрытие",  // merc is allowed to seek cover
	L" Может помочь команде", // merc can assist teammates
	L"Готово", // done with this menu
	L"Отмена", // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by the player

STR16 pOtherGuardMenuStrings[] =
{
	L"Ведение огня:",
	L" *Агрессивная атака*",
	L" *Беречь патроны*",
	L" *Воздержаться от стрельбы*",
	L"Другие параметры:",
	L" *Может отступить*",
	L" *Может искать укрытие*",
	L" *Может помочь команде*",
	L"Готово",
	L"Отмена",
};

STR16 pAssignMenuStrings[] =
{
	L"На службе", // merc is on active duty
	L"Медик", // the merc is acting as a doctor
	L"Пациент", // the merc is receiving medical attention
	L"Машина", // the merc is in a vehicle
	L"Ремонт", // the merc is repairing items 
	L"Обучение", // the merc is training //обучение
	L"Отмена", // cancel this menu
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"В атаку", // set militia to aggresive
	L"Держать оборону", // set militia to stationary
	L"Отступать", // retreat militia
	L"За мной", // retreat militia
	L"Ложись", // retreat militia	
	L"В укрытие",
	L"Все в атаку", 
	L"Всем держать оборону",
	L"Всем отступать",
	L"Все за мной",
	L"Всем рассеяться",
	L"Всем залечь",
	L"Всем в укрытие",
	//L"Всем искать предметы",
	L"Отмена", // cancel this menu
};

//STR16 pTalkToAllMenuStrings[] =
//{
//	L"В атаку", // set militia to aggresive
//	L"Держать оборону", // set militia to stationary
//	L"Отступать", // retreat militia
//	L"За мной", // retreat militia
//	L"Ложись", // retreat militia		 
//	L"Отмена", // cancel this menu
//};

STR16 pRemoveMercStrings[] =
{
	L"Убрать бойца", // remove dead merc from current team
	L"Отмена",
};

STR16 pAttributeMenuStrings[] =
{
	L"Сила",
	L"Ловкость",
	L"Проворность",
	L"Здоровье",
	L"Меткость",
	L"Медицина",
	L"Механика",
	L"Лидерство",
	L"Взрывчатка",
	L"Отмена",
};

STR16 pTrainingMenuStrings[] =
{
 L"Практика", // train yourself 
 L"Ополчение", // train the town 
 L"Тренер", // train your teammates 
 L"Ученик",  // be trained by an instructor 
 L"Отмена", // cancel this menu
};


STR16 pSquadMenuStrings[] =
{
	L"Отряд  1",
	L"Отряд  2",
	L"Отряд  3",
	L"Отряд  4",
	L"Отряд  5",
	L"Отряд  6",
	L"Отряд  7",
	L"Отряд  8",
	L"Отряд  9",
	L"Отряд 10",
	L"Отряд 11",
	L"Отряд 12",
	L"Отряд 13",
	L"Отряд 14",
	L"Отряд 15",
	L"Отряд 16",
	L"Отряд 17",
	L"Отряд 18",
	L"Отряд 19",
	L"Отряд 20",
	L"Отмена",
};

STR16 pPersonnelTitle[] =
{
	L"Команда", // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] =
{
	L"Здоровье:",		// health of merc
	L"Проворность:", 
	L"Ловкость:",
	L"Сила:",
	L"Лидерство:",
	L"Интеллект:",
	L"Опыт:",		// experience level
	L"Меткость:", 
	L"Механика:",
	L"Взрывчатка:",
	L"Медицина:",
	L"Мед. депозит:",	// amount of medical deposit put down on the merc
	L"До конца контракта:",	// cost of current contract
	L"Убил врагов:",	// number of kills by merc
	L"Помог убить:",	// number of assists on kills by merc
	L"Гонорар за день:",	// daily cost of merc
	L"Общая цена услуг:",	// total cost of merc
	L"Контракт:",		// cost of current contract
	L"У вас на службе:",	// total service rendered by merc
	L"Задолж. жалования:",	// amount left on MERC merc to be paid
	L"Процент попаданий:",	// percentage of shots that hit target
	L"Боев:", 		// number of battles fought
	L"Ранений:",		// number of times merc has been wounded
	L"Навыки:",
	L"Нет навыков",
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = 
{
	L"Нет навыка",
	L"Взлом замков",
	L"Рукопашный бой",
	L"Электроника",
	L"Ночные операции", 
	L"Метание",
	L"Инструктор",
	L"Тяжелое оружие",
	L"Авт. оружие",
	L"Скрытность",
	L"Стрельба с двух рук",
	L"Воровство",
	L"Боевые искусства",
	L"Холодное оружие",
	L"Снайпер", 
	L"Камуфляж",
	L"Камуфляж (Город)",
	L"Камуфляж (Пустыня)",
	L"Камуфляж (Снег)",
	L"(Эксперт)",
};


// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] =
{
	L"Встать/Идти (|S)", 
	L"Присесть/Гусиный шаг (|C)",
	L"Стоять/Бежать (|R)",
	L"Лечь/Ползти (|P)",
	L"Поворот (|L)",
	L"Действие",
	L"Поговорить",
	L"Осмотреть (|C|t|r|l)",

	// Pop up door menu
	L"Открыть",
	L"Искать ловушки", 
	L"Вскрыть отмычками", 
	L"Открыть cилой",
	L"Обезвредить",
	L"Запереть",
	L"Отпереть",
	L"Использовать заряд взрывчатки", 
	L"Взломать ломом",
	L"Отмена (|E|s|c)",
	L"Закрыть",
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.

STR16 pDoorTrapStrings[] =
{
	L"нет ловушки",
	L"бомба-ловушка",
	L"электроловушка",
	L"сирена",
	L"сигнализация",
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.

STR16 pContractExtendStrings[] =
{
	L"1 день",
	L"7 дней",
	L"14 дней",
};

// On the map screen, there are four columns. This text is popup help text that identifies the individual columns.

STR16 pMapScreenMouseRegionHelpText[] =
{
	L"Выбрать наемника",
	L"Отдать приказ",
	L"Проложить путь движения",
	L"Контракт наемника (|C)",
	L"Местонахождение бойца",
	L"Спать", 
};

// volumes of noises 

STR16 pNoiseVolStr[] =
{
	L"ТИХИЙ",
	L"ЧЕТКИЙ",
	L"ГРОМКИЙ",
	L"ОЧЕНЬ ГРОМКИЙ",
};

// types of noises 

STR16 pNoiseTypeStr[] = // OBSOLETE
{
	L"НЕЗНАКОМЫЙ",
	L"ЗВУК ШАГОВ",
	L"СКРИП",
	L"ВСПЛЕСК",
	L"УДАР",
	L"ВЫСТРЕЛ",
	L"ВЗРЫВ",
	L"КРИК",
	L"УДАР",
	L"УДАР",
	L"ЗВОН",
	L"ГРОХОТ",
};

// Directions that are used to report noises

STR16 pDirectionStr[] = 
{
	L"c СЕВЕРО-ВОСТОКА",
	L"c ВОСТОКА",
	L"c ЮГО-ВОСТОКА",
	L"c ЮГА",
	L"c ЮГО-ЗАПАДА",
	L"c ЗАПАДА",
	L"c СЕВЕРО-ЗАПАДА",
	L"c СЕВЕРА",
};

// These are the different terrain types. 

STR16 pLandTypeStrings[] =
{
	L"Город",
	L"Дорога",
	L"Равнина",
	L"Пустыня",
	L"Прерия",
	L"Лес",	 
	L"Болото",
	L"Вода",	
	L"Холмы",
	L"Непроходимо",
	L"Река",	//river from north to south
	L"Река",	//river from east to west
	L"Чужая страна",
	//NONE of the following are used for directional travel, just for the sector description.
	L"Тропики",
	L"Ферма",
	L"Поля, дорога",
	L"Леса, дорога",
	L"Ферма, дорога",
	L"Тропики, дорога",
	L"Леса, дорога",
	L"Побережье",
	L"Горы, дорога",
	L"Берег, дорога",
	L"Пустыня, дорога",
	L"Болота, дорога",
	L"Прерия, ПВО", 
	L"Пустыня, ПВО",
	L"Тропики, ПВО",
	L"Медуна, ПВО",
	
	//These are descriptions for special sectors
	L"Госпиталь Камбрии",
	L"Аэропорт Драссена",
	L"Аэропорт Медуны",
	L"База ПВО",
	L"Убежище повстанцев",	//The rebel base underground in sector A10
	L"Подвалы Тиксы",	//The basement of the Tixa Prison (J9)
	L"Логово тварей",	//Any mine sector with creatures in it
	L"Подвалы Орты",	//The basement of Orta (K4)
	L"Туннель",		//The tunnel access from the maze garden in Meduna 
				//leading to the secret shelter underneath the palace
	L"Убежище",		//The shelter underneath the queen's palace
	L"",			//Unused
};

STR16 gpStrategicString[] =
{
	L"",	//Unused
	L"%s замечен в секторе %c%d, и другой отряд уже на подходе.",	//STR_DETECTED_SINGULAR
	L"%s замечен в секторе %c%d, и остальные отряды уже на подходе.",	//STR_DETECTED_PLURAL
	L"Желаете дождаться прибытия остальных?",													//STR_COORDINATE

	//Dialog strings for enemies.

	L"Враг предлагает вам сдаться.",			//STR_ENEMY_SURRENDER_OFFER
	L"Оставшиеся без сознания бойцы попали в плен.",	//STR_ENEMY_CAPTURED

	//The text that goes on the autoresolve buttons							

	L"Отступить", 		//The retreat button		//STR_AR_RETREAT_BUTTON
	L"OK",		//The done button			//STR_AR_DONE_BUTTON

	//The headers are for the autoresolve type (MUST BE UPPERCASE)

	L"ОБОРОНА",								//STR_AR_DEFEND_HEADER
	L"АТАКА",								//STR_AR_ATTACK_HEADER
	L"ВСТРЕЧА",								//STR_AR_ENCOUNTER_HEADER
	L"Сектор",		//The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

	//The battle ending conditions						

	L"ПОБЕДА!",					//STR_AR_OVER_VICTORY
	L"ПОРАЖЕНИЕ!",					//STR_AR_OVER_DEFEAT
	L"СДАЛСЯ!",					//STR_AR_OVER_SURRENDERED
	L"ПЛЕНЕН!",					//STR_AR_OVER_CAPTURED
	L"ОТСТУПИЛ!",					//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"Ополченец",					//STR_AR_MILITIA_NAME,
	L"Элита",					//STR_AR_ELITE_NAME,
	L"Солдат",					//STR_AR_TROOP_NAME,
	L"Смотритель",					//STR_AR_ADMINISTRATOR_NAME,
	L"Рептион",					//STR_AR_CREATURE_NAME,

	//Label for the length of time the battle took

	L"Прошло времени",				//STR_AR_TIME_ELAPSED,

	//Labels for status of merc if retreating.  (UPPERCASE)			

	L"ОТСТУПИЛ",					//STR_AR_MERC_RETREATED,
	L"ОТСТУПАЕТ",					//STR_AR_MERC_RETREATING,
	L"ОТСТУПИТЬ",					//STR_AR_MERC_RETREAT,

	//PRE BATTLE INTERFACE STRINGS 
	//Goes on the three buttons in the prebattle interface.  The Auto resolve button represents
	//a system that automatically resolves the combat for the player without having to do anything.
	//These strings must be short (two lines -- 6-8 chars per line)

	L"Авто битва",					//STR_PB_AUTORESOLVE_BTN,
	L"Перейти в сектор",				//STR_PB_GOTOSECTOR_BTN,
	L"Уйти из сектора",				//STR_PB_RETREATMERCS_BTN,

	//The different headers(titles) for the prebattle interface.
	L"ВСТРЕЧА С ВРАГОМ",				//STR_PB_ENEMYENCOUNTER_HEADER,
	L"НАСТУПЛЕНИЕ ВРАГА",				//STR_PB_ENEMYINVASION_HEADER, // 30
	L"ВРАЖЕСКАЯ ЗАСАДА",				//STR_PB_ENEMYAMBUSH_HEADER
	L"ВРАЖЕСКИЙ СЕКТОР",				//STR_PB_ENTERINGENEMYSECTOR_HEADER
	L"АТАКА ТВАРЕЙ",				//STR_PB_CREATUREATTACK_HEADER
	L"ЗАСАДА КОШЕК-УБИЙЦ",				//STR_PB_BLOODCATAMBUSH_HEADER
	L"ВХОД В ЛОГОВИЩЕ КОШЕК-УБИЙЦ",			//STR_PB_ENTERINGBLOODCATLAIR_HEADER

	//Various single words for direct translation.  The Civilians represent the civilian
	//militia occupying the sector being attacked.  Limited to 9-10 chars

	L"Локация",
	L"Враг",
	L"Наемники",
	L"Ополчение",
	L"Рептионы",
	L"Кошки-убийцы",
	L"Сектор",
	L"Нет",			//If there are no uninvolved mercs in this fight.
	L"Н/Д",			//Acronym of Not Applicable
	L"д",			//One letter abbreviation of day
	L"ч",			//One letter abbreviation of hour

	//TACTICAL PLACEMENT USER INTERFACE STRINGS
	//The four buttons 

	L"Отмена",
	L"Случайно",
	L"Группой",
	L"B aтaку!",

	//The help text for the four buttons.  Use \n to denote new line (just like enter).

	L"Убирает все позиции бойцов \nи позволяет заново расставить их. (|C)", 
	L"При каждом нажатии распределяет \nбойцов случайным образом. (|S)",
	L"Позволяет выбрать место, \nгде сгруппировать ваших бойцов. (|G)",
	L"Нажмите эту кнопку, когда завершите \nвыбор позиций для бойцов. (|В|в|о|д)",
	L"Вы должны разместить всех своих бойцов \nдо того, как начать бой.",

	//Various strings (translate word for word)

	L"Сектор",
	L"Выбор точек входа",

	//Strings used for various popup message boxes.  Can be as long as desired.

	L"Препятствие. Место недоступно. Попробуйте пройти другим путем.",
	L"Поместите бойцов в незатененную часть карты.",

	//This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"прибыл(а) в сектор", 

	//These entries are for button popup help text for the prebattle interface.  All popup help
	//text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
	L"Автоматически просчитывает бой\nбез загрузки карты. (|A)",
	L"Нельзя включить автобой\nво время нападения.",
	L"Войти в сектор, чтобы атаковать врага. (|E)",
	L"Отступить отрядом в предыдущий сектор. (|R)",				//singular version
	L"Всем отрядам отступить в предыдущий сектор. (|R)", //multiple groups with same previous sector

	//various popup messages for battle conditions.  

	//%c%d is the sector -- ex:  A9
	L"Враги атаковали ваших ополченцев в секторе %c%d.",
	//%c%d сектор -- напр:  A9
	L"Твари атаковали ваших ополченцев в секторе %c%d.",
	//1st %d refers to the number of civilians eaten by monsters,  %c%d is the sector -- ex:  A9
	//Note:  the minimum number of civilians eaten will be two.
	L"Твари убили %d гражданских во время атаки сектора %s.",
	//%s is the sector location -- ex:  A9: Omerta
	L"Враги атаковали ваших наемников в секторе %s. Ни один из ваших бойцов не в состоянии сражаться!",
	//%s is the sector location -- ex:  A9: Omerta
	L"Твари атаковали ваших наемников в секторе %s. Ни один из ваших бойцов не в состоянии сражаться!",

};

STR16 gpGameClockString[] = 
{
	//This is the day represented in the game clock.  Must be very short, 4 characters max.
	L"День",	
};

//When the merc finds a key, they can get a description of it which 
//tells them where and when they found it.
STR16 sKeyDescriptionStrings[2] =
{
	L"Найдено в секторе:",
	L"Найдено за день:",
};

//The headers used to describe various weapon statistics.

CHAR16		gWeaponStatsDesc[][ 16 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"Состояние:",
	L"Вес (%s):", 
	L"Нужно ОД",	
	L"Дист:",		// Range
	L"Урон:",		// Damage
	L"Всего:", 		// Number of bullets left in a magazine
	L"ОД:",			// abbreviation for Action Points
	L"=",
	L"=",
						//Lal: additional strings for tooltips
	L"Точность:",	//9
	L"Дист:",		//10	
	L"Урон:", 	//11
	L"Вес:",		//12
	L"Оглушение:",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	L"AUTOPEN:",	//14
	L"AUTO/5:",		//15
	L"Всего:",		//16

};

// HEADROCK: Several arrays of tooltip text for new Extended Description Box
// Please note, several of these are artificially inflated to 19 entries to help fix a complication with
// changing item type while watching its description box
STR16		gzWeaponStatsFasthelp[ 27 ] =
{
	L"Дальнобойность",						//Range
	L"Урон",							//Damage
	L"Штраф за отдачу при\nстрельбе очередью\n(c отсечкой/без)",	//Burst/Auto Penalty
	L"Выстрелов в автоматическом\nрежиме за 5 ОД",			//Autofire shots per 5 AP
	L"Точность",							//Accuracy
	L"Надёжность",							//Reliability
	L"Простота ремонта",						//Repair Ease
	L"ОД на вскидку",						//APs to ready
	L"ОД на 1 выстрел",						//APs to fire Single
	L"ОД на огонь с отсечкой",					//APs to fire Burst
	L"ОД на огонь очередью",					//APs to fire Auto
	L"ОД на замену магазина",					//APs to Reload
	L"ОД на досылку патрона",					//APs to Reload Manually
	L"Пламегаситель",						//Flash Suppression
	L"Шумность (чем меньше, тем лучше)",				//Loudness (Lower is better)
	L"Модификатор точности",					//To-Hit Modifier
	L"Радиус наилучшей видимости\nточки прицеливания",		//Average Best Laser Range
	L"Модификатор прицельной стрельбы",				//Aiming Modifier
	L"Минимальная эффективная дальность",				//Min. Range for Aiming Bonus
	L"Бонус от сошек\n(при стрельбе лёжа)",				//Bipod Modifier
	L"ОД на бросок",						//APs to Throw
	L"ОД на выстрел",						//APs to Launch
	L"ОД на удар ножом",						//APs to Stab
	L"Не стреляет одиночными!",					//No Single Shot!
	L"Нет отсечки патрона!",					//No Burst Mode!
	L"Нет автоматического режима!",					//No Auto Mode!
	L"ОД на удар",							//APs to Bash
};

STR16		gzWeaponStatsFasthelpTactical[ 27 ] =
{
	L"Дальнобойность",
	L"Урон",
	L"Штраф за отдачу при\nстрельбе очередью\n(c отсечкой/без)",
	L"Выстрелов в автоматическом\nрежиме за 5 ОД",
	L"Точность",
	L"Надёжность",
	L"Простота ремонта",
	L"ОД на вскидку",
	L"ОД на 1 выстрел",
	L"ОД на огонь с отсечкой",
	L"ОД на огонь очередью",
	L"ОД на замену магазина",
	L"ОД на досылку патрона",
	L"Модификатор точности",
	L"Радиус наилучшей видимости\nточки прицеливания",
	L"Модификатор прицельной стрельбы",
	L"Минимальная эффективная дальность",
	L"Пламегаситель",
	L"Шумность\n(чем меньше, тем лучше)",
	L"Бонус от сошек\n(при стрельбе лёжа)",
	L"ОД на бросок",
	L"ОД на выстрел",
	L"ОД на удар ножом",
	L"Не стреляет одиночными!",
	L"Нет отсечки патрона!",
	L"Нет автоматического режима!",
	L"ОД на удар",
};

STR16		gzAmmoStatsFasthelp[ 20 ] =
{
	L"Проникающий эффект\n(чем меньше, тем лучше)",		//Armor Penetration (Lower is better)
	L"Блуждающий эффект\n(чем выше, тем лучше)",		//Bullet Tumble (Higher is better)
	L"Разрывной эффект\n(чем выше, тем лучше)",		//Pre-impact Explosion (Higher is better)
	L"Трассирующий эффект",					//Tracer Effect
	L"Противотанковые",					//Anti-Tank
	L"Разрушают замок",					//Lockbuster
	L"Игнорируется наличие брони",				//Ignores Armor
	L"Кислотный эффект",					//Acidic
	L"Модификатор дальнобойности",				//Range Modifier
	L"Модификатор урона",					//Damage Modifier
	L"Модификатор точности",				//To-Hit Modifier
	L"Модификатор штрафа\nза отдачу при\nстрельбе очередью\n(c отсечкой/без)",	//Burst/Auto Penalty Modifier (Higher is better)",
	L"Модификатор надёжности",				//Reliability Modifier
	L"Модификатор шумности\n(чем меньше, тем лучше)",	//Loudness Modifier (Lower is better)
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
};

STR16		gzArmorStatsFasthelp[ 20 ] =
{
	L"Защита",						//Protection
	L"Покрытие тела\n(чем больше, тем лучше)",		//Coverage (Higher is better)
	L"Изнашиваемость\n(чем меньше, тем лучше)",		//Degrade Rate (Lower is better)
	L"Влияние на подвижность",				//AP Modifier
	L"Модификатор точности",				//To-Hit Modifier
	L"Камуфляж 'Лес'",
	L"Камуфляж 'Город'",
	L"Камуфляж 'Пустыня'",
	L"Камуфляж 'Снег'",
	L"Модификатор скрытности",				//Stealth Modifier
	L"Модификатор диапазона\nвидимости",			//Vision Range Modifier
	L"Модификатор диапазона\nвидимости днём",		//Day Vision Range Modifier
	L"Модификатор диапазона\nвидимости ночью",		//Night Vision Range Modifier
	L"Модификатор диапазона\nвидимости при ярком освещении",//Bright Light Vision Range Modifier
	L"Модификатор диапазона\nвидимости в пещере",		//Cave Vision Range Modifier
	L"Сужение сектора обзора",				//Tunnel Vision Percentage
	L"Модификатор диапазона\nслышимости",			//Hearing Range Modifier
	L"",
	L"",
	L"",
};

STR16		gzExplosiveStatsFasthelp[ 20 ] =
{
	L"Урон",						//Damage
	L"Оглушение", //отнимает энергию			//Stun Damage
	L"Зона слышимости взрыва\n(чем меньше, тем лучше)",	//Blast Loudness (Lower is better)
	L"Взрывоопасность!!!\n(чем меньше, тем лучше)",		//Volatility!!! (Lower is better)
	L"Зона действия взрыва",				//Blast Radius
	L"Начальная зона действия",				//Effect Start Radius
	L"Конечная зона действия",				//Effect Final Radius
	L"Время действия",					//Effect Duration
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
	L"Модификатор размера предмета\n(чем меньше, тем лучше)",		//Item Size Modifier (Lower is better)
	L"Модификатор надёжности",						//Reliability Modifier
	L"Модификатор шумности\n(чем меньше, тем лучше)",			//Loudness Modifier (Lower is better)
	L"Скрывает вспышку",							//Hides Muzzle Flash
	L"Модификатор сошек",							//Bipod Modifier
	L"Модификатор дальнобойности",						//Range Modifier
	L"Модификатор точности",						//To-Hit Modifier
	L"Радиус наилучшей видимости\nточки прицеливания",			//Best Laser Range
	L"Модификатор бонусов оптики",						//Aiming Bonus Modifier
	L"Модификатор очереди с отсечкой",					//Burst Size Modifier
	L"Модификатор штрафа за отдачу\nпри стрельбе c отсечкой\n(чем больше, тем лучше)",	//Burst Penalty Modifier (Higher is better)
	L"Модификатор штрафа за отдачу\nпри стрельбе очередью\n(чем больше, тем лучше)",	//Auto-Fire Penalty Modifier (Higher is better)
	L"Модификатор ОД",							//AP Modifier
	L"Модификатор ОД\nна очередь с отсечкой\n(чем меньше, тем лучше)",	//AP to Burst Modifier (Lower is better)
	L"Модификатор ОД\nна очередь без отсечки\n(чем меньше, тем лучше)",	//AP to Auto-Fire Modifier (Lower is better)
	L"Модификатор ОД на вскидку\n(чем меньше, тем лучше)",			//AP to Ready Modifier (Lower is better)
	L"Модификатор ОД\nна замену магазина\n(чем меньше, тем лучше)",		//AP to Reload Modifier (Lower is better)
	L"Модификатор объёма магазина",						//Magazine Size Modifier
	L"Модификатор ОД на выстрел\n(чем меньше, тем лучше)",			//AP to Attack Modifier (Lower is better)
	L"Модификатор урона",							//Damage Modifier
	L"Модификатор урона\nв ближнем бою", 					//Melee Damage Modifier
	L"Камуфляж 'Лес'",
	L"Камуфляж 'Город'",
	L"Камуфляж 'Пустыня'",
	L"Камуфляж 'Снег'",
	L"Модификатор скрытности", // 25
	L"Модификатор диапазона\nслышимости",
	L"Модификатор диапазона\nвидимости",
	L"Модификатор диапазона\nвидимости днём",
	L"Модификатор диапазона\nвидимости ночью",
	L"Модификатор диапазона\nвидимости при ярком освещении", //30
	L"Модификатор диапазона\nвидимости в пещере",
	L"Сужение сектора обзора\n(чем меньше, тем лучше)",		//Tunnel Vision Percentage (Lower is better)
	L"Минимальная эффективная\nдальность оптики",				//Minimum Range for Aiming Bonus
};

// HEADROCK: End new tooltip text

//The headers used for the merc's money.

CHAR16 gMoneyStatsDesc[][ 13 ] =
{
	L"Кол-во",
	L"Осталось:", //this is the overall balance
	L"Кол-во",
	L"Отделить:", //the amount he wants to separate from the overall balance to get two piles of money

	L"Текущий",
	L"Баланс",
	L"Снимаемая",
	L"Сумма",
};

//The health of various creatures, enemies, characters in the game. The numbers following each are for comment
//only, but represent the precentage of points remaining. 

CHAR16 zHealthStr[][13] =
{
	L"УМИРАЕТ",		//	>= 0
	L"КРИТИЧЕН", 		//	>= 15
	L"ПЛОХ",		//	>= 30
	L"РАНЕН",    	//	>= 45
	L"ЗДОРОВ",    	//	>= 60
	L"СИЛЕН",     	// 	>= 75
  L"ОТЛИЧНО",		// 	>= 90
};

STR16	gzMoneyAmounts[6] = 
{ 
	L"1000$",
	L"100$",
	L"10$",
	L"Снять",
	L"Разделить",
	L"Взять",
};

// short words meaning "Advantages" for "Pros" and "Disadvantages" for "Cons."
CHAR16		gzProsLabel[10] = 
{
	L"+",
};

CHAR16		gzConsLabel[10] = 
{
	L"-",
};

//Conversation options a player has when encountering an NPC
CHAR16 zTalkMenuStrings[6][ SMALL_STRING_LENGTH ] =
{
	L"Повторить", 	//meaning "Repeat yourself"
	L"Дружественно",		//approach in a friendly
	L"Напрямую",		//approach directly - let's get down to business
	L"Угрожать",		//approach threateningly - talk now, or I'll blow your face off
	L"Дать",		
	L"Нанять",
};

//Some NPCs buy, sell or repair items. These different options are available for those NPCs as well.
CHAR16 zDealerStrings[4][ SMALL_STRING_LENGTH ]=
{
	L"Купить/Продать",
	L"Купить",
	L"Продать",
	L"Ремонтировать",
};

CHAR16 zDialogActions[1][ SMALL_STRING_LENGTH ] = 
{
	L"До встречи",
};


//These are vehicles in the game.

STR16 pVehicleStrings[] =
{
 L"Эльдорадо",
 L"Хаммер", // a hummer jeep/truck -- military vehicle
 L"Фургон",
 L"Джип",
 L"Танк",
 L"Вертолет",
};

STR16 pShortVehicleStrings[] =
{
	L"Эльдор",
	L"Хаммер",			// the HMVV
	L"Фургон",
	L"Джип",
	L"Танк",
	L"Верт.", 				// the helicopter
};

STR16	zVehicleName[] =
{
	L"Эльдорадо",
	L"Хаммер",		//a military jeep. This is a brand name.
	L"Фургон",			// Ice cream truck
	L"Джип",
	L"Танк",
	L"Вертолет", 		//an abbreviation for Helicopter
};


//These are messages Used in the Tactical Screen

CHAR16 TacticalStr[][ MED_STRING_LENGTH ] =
{
	L"Воздушный Рейд",
	L"Оказать первую помощь?",
	
	// CAMFIELD NUKE THIS and add quote #66.
	
	L"%s замечает, что некоторые вещи отсутствуют в посылке.",
	
	// The %s is a string from pDoorTrapStrings
	
	L"Замок (%s).",
	L"Тут нет замка.",
	L"Успех!",
	L"Провал.",
	L"Успех!",
	L"Провал",
	L"На замке нет ловушки.",
	L"Успех!",
	// The %s is a merc name
	L"У %s нет подходящего ключа",
	L"Ловушка обезврежена",
	L"На замке не найдено ловушки.",
	L"Заперто",
	L"ДВЕРЬ",
	L"С ЛОВУШКОЙ",
	L"ЗАПЕРТАЯ",
	L"НЕЗАПЕРТАЯ",
	L"СЛОМАНАЯ",
	L"Тут есть кнопка. Нажать?",
	L"Разрядить ловушку?",
	L"Пред...",
	L"След...",
	L"Еще...",

	// In the next 2 strings, %s is an item name
	
	L"%s помещен(а) на землю.",
	L"%s отдан(а) %s.",

	// In the next 2 strings, %s is a name

	L"%s: Оплачено сполна.",
	L"%s: Еще должен %d.",
	L"Установить частоту радиодетонатора:",  	//in this case, frequency refers to a radio signal
	L"Количество ходов до взрыва:",	//how much time, in turns, until the bomb blows
	L"Выберите частоту радиодетонатора на пульте:", 	//in this case, frequency refers to a radio signal
	L"Обезвредить ловушку?",
	L"Убрать синий флаг?",
	L"Поставить здесь синий флаг?",
	L"Завершающий ход",

	// In the next string, %s is a name. Stance refers to way they are standing.

	L"Вы действительно хотите атаковать %s?",
	L"Увы, в машине боец не может изменить положение.",
	L"Робот не может менять положение.",

	// In the next 3 strings, %s is a name

	L"%s не может поменять положение здесь.",
	L"%s не может получить первую помощь.",
	L"%s не нуждается в медицинской помощи.",
	L"Туда идти нельзя.",
	L"У вас уже полная команда, мест нет.",	//there's no room for a recruit on the player's team

	// In the next string, %s is a name

	L"%s нанят(а).",

	// Here %s is a name and %d is a number

	L"%s должен получить $%d.",

	// In the next string, %s is a name

	L"Сопроводить %s?",

	// In the next string, the first %s is a name and the second %s is an amount of money (including $ sign)

	L"Нанять %s за %s в день?",

	// This line is used repeatedly to ask player if they wish to participate in a boxing match. 

	L"Хотите участвовать в поединке?",

	// In the next string, the first %s is an item name and the 
	// second %s is an amount of money (including $ sign)

	L"Купить %s за %s?",

	// In the next string, %s is a name

	L"%s сопровождается отрядом %d.",

	// These messages are displayed during play to alert the player to a particular situation

	L"ОТКАЗ",					//weapon is jammed.
	L"Роботу нужны патроны %s калибра.",		//Robot is out of ammo
	L"Бросить туда не получится.",		//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"Режим скрытности (|Z)",
	L"Карта (|M)",
	L"Завершить ход (|D)",
	L"Говорить",
	L"Молчать",
	L"Подняться (|P|g|U|p)",
	L"Смена уровня (|T|a|b)",
	L"Забраться/Спрыгнуть (|J)",
	L"Присесть/Лечь (|P|g|D|n)",
	L"Осмотреть (|C|t|r|l)",
	L"Предыдущий боец",
	L"Следующий боец (|П|p|o|б|e|л)",
	L"Настройки (|O)",
	L"Режим очереди (|B)",
	L"Смотреть/Повернуться (|L)",
	L"Здоровье: %d/%d\nЭнергия: %d/%d\nБоевой дух: %s",
	L"Ну и?",					//this means "what?"
	L"Продолж.",					// an abbrieviation for "Continued"
	L"%s будет говорить.",
	L"%s будет молчать.",
	L"Состояние: %d/%d\nТопливо: %d/%d",
	L"Выйти из машины",
	L"Сменить отряд (|S|h|i|f|t |П|p|о|б|e|л)",
	L"Ехать",
	L"Н/Д",						//this is an acronym for "Not Applicable."
	L"Рукопашный бой",
	L"Применить оружие",
	L"Воспользоваться ножом",
	L"Использовать взрывчатку",
	L"Воспользоваться аптечкой",
	L"(Ловит)",
	L"(Перезарядка)",
	L"(Дать)",
	L"Сработала %s.",	// The %s here is a string from pDoorTrapStrings  ASSUME all traps are female gender
	L"%s прибыл(а).",
	L"%s: истратил(а) все очки действия.",
	L"%s сейчас не может действовать.",
	L"%s перевязан(а).",
	L"%s: закончились бинты.",
	L"Враг в секторе!",
	L"Врагов в поле зрения нет.",
	L"Недостаточно очков действия.",
	L"Оденьте на голову одного из наемников пульт управления роботом.",
	L"Последняя очередь опустошила магазин!",
	L"СОЛДАТ",
	L"РЕПТИОН",
	L"ОПОЛЧЕНЕЦ",
	L"ЖИТЕЛЬ",
	L"Выход из сектора",
	L"ДА",
	L"ОТМЕНА",
	L"Выбранный боец",
	L"Все бойцы отряда",
	L"Идти в сектор",
	L"Идти на карту",
	L"Этот сектор отсюда покинуть нельзя.",
	L"%s слишком далеко.",
	L"Скрыть кроны деревьев",
	L"Показать кроны деревьев",
	L"ВОРОНА",				//Crow, as in the large black bird
	L"ШЕЯ",
	L"ГОЛОВА",
	L"ТОРС",
	L"НОГИ",
	L"Рассказать Королеве то, что она хочет знать?",
	L"Регистрация отпечатков пальцев пройдена.",
	L"Неопознанные отпечатки пальцев. Оружие заблокировано.",
	L"Цель захвачена",
	L"Путь заблокирован",
	L"Положить/Снять деньги",		// Help text over the $ button on the Single Merc Panel 
	L"Никто не нуждается в медицинской помощи.",
	L"отказ",								// Short form of JAMMED, for small inv slots
	L"Туда вскарабкаться невозможно.",					// used ( now ) for when we click on a cliff
	L"Путь блокирован. Хотите поменяться местами с этим человеком?",
	L"Человек отказывается двигаться.",
	// In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
	L"Вы согласны заплатить %s?",
	L"Принять бесплатное лечение?",
	L"Согласиться выйти замуж за Дэррела?",
	L"Связка ключей",
	L"С эскортируемыми этого сделать нельзя.",
	L"Пощадить сержанта?",
	L"За пределами прицельной дальности.",
	L"Шахтер",
	L"Машина может ездить только между секторами.",
	L"Ни у кого из наемников нет аптечки",
	L"Путь для %s заблокирован",
	L"Ваши бойцы, захваченные армией Дейдраны, томятся здесь в плену!",
	L"Замок поврежден.",
	L"Замок разрушен.",
	L"Кто-то с другой стороны пытается открыть эту дверь.",
	L"Состояние: %d/%d\nТопливо: %d/%d",
	L"%s не видит %s.",  // Cannot see person trying to talk to
	L"Принадлежность отсоединена",
	L"Вы не можете содержать еще одну машину, довольствуйтесь уже имеющимися двумя.",
};

//Varying helptext explains (for the "Go to Sector/Map" checkbox) what will happen given different circumstances in the "exiting sector" interface.
STR16 pExitingSectorHelpText[] =
{
	//Helptext for the "Go to Sector" checkbox button, that explains what will happen when the box is checked.
	L"Если выбрано, то карта соседнего сектора будет сразу же загружена.",
	L"Если выбрано, то вы автоматически попадете на экран карты,\nтак как путешествие займет некоторое время.",

	//If you attempt to leave a sector when you have multiple squads in a hostile sector.
	L"Этот сектор оккупирован врагом, и вы не можете выйти отсюда.\nВы должны разобраться с этим, прежде чем перейти в любой другой сектор.",

	//Because you only have one squad in the sector, and the "move all" option is checked, the "go to sector" option is locked to on. 
	//The helptext explains why it is locked.
	L"Как только оставшиеся наемники покинут этот сектор,\nсразу будет загружен соседний сектор.",
	L"Выведя оставшихся наемников из этого сектора,\nвы автоматически попадете на экран карты,\nтак как на путешествие потребуется некоторое время.",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted.  The "single" button is disabled.
	L"%s нуждается в сопровождении ваших наемников и не может в одиночку покинуть сектор.",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s не может покинуть сектор один, так как он сопровождает %s.", //male singular
	L"%s не может покинуть сектор одна, так как она сопровождает %s.", //female singular
	L"%s не может покинуть сектор один, так как он сопровождает группу из нескольких человек.", //male plural
	L"%s не может покинуть сектор одна, так как она сопровождает группу из нескольких человек.", //female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the  "move all" option is disabled,
	//and this helptext explains why.
	L"Все ваши наемники должны быть в машине,\nчтобы отряд смог отправиться в место назначения.",

	L"", //UNUSED

	//Standard helptext for single movement.  Explains what will happen (splitting the squad)
	L"Если выбрать, то %s отправится в одиночку\nи автоматически будет переведен в отдельный отряд.",

	//Standard helptext for all movement.  Explains what will happen (moving the squad)
	L"Если выбрать, данный отряд отправится\nв место назначения, покинув этот сектор.",

	//This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear.  This is just like the situation where
	//This string is special, as it is not used as helptext.  Do not use the special newline character (\n) for this string.
	L"%s сопровождается вашими наемниками и не может покинуть этот сектор в одиночку. Остальные наемники должны быть рядом, прежде чем вы сможете покинуть сектор.",
};



STR16 pRepairStrings[] = 
{
	L"Предметы", 		// tell merc to repair items in inventory
	L"База ПВО", 		// tell merc to repair SAM site - SAM is an acronym for Surface to Air Missile
	L"Отмена", 		// cancel this menu
	L"Робот", 		// repair the robot
};


// NOTE: combine prestatbuildstring with statgain to get a line like the example below.
// "John has gained 3 points of marksmanship skill."

STR16 sPreStatBuildString[] =
{
	L"теряет", 			// the merc has lost a statistic
	L"получает", 		// the merc has gained a statistic
	L"единицу",	// singular
	L"единиц",	// plural
	L"уровень",	// singular
	L"уровня",	// plural
};

STR16 sStatGainStrings[] =
{
	L"здоровья.",
	L"проворности.",
	L"ловкости.",
	L"интеллекта.",
	L"медицины.",
	L"взрывного дела.",
	L"механики.",
	L"меткости.",
	L"опыта.",
	L"силы.",
	L"лидерства.",
};


STR16 pHelicopterEtaStrings[] =
{
	L"Общая дистанция:", 			// total distance for helicopter to travel
	L"Безопасно:  ", 			// distance to travel to destination
	L"Опасно:", 			// distance to return from destination to airport
	L"Итого:", 		// total cost of trip by helicopter
	L"ОВП:", 			// ETA is an acronym for "estimated time of arrival" 
	L"У вертолета закончилось топливо. Придется совершить посадку на вражеской территории!",	// warning that the sector the helicopter is going to use for refueling is under enemy control -> 
  L"Пассажиры:",
  L"Выбрать вертолет или точку высадки?",
  L"Вертолет",
  L"Высадка",
};

STR16 sMapLevelString[] =
{
	L"Подуровень:", 			// what level below the ground is the player viewing in mapscreen
};

STR16 gsLoyalString[] =
{
	L"Лояльность",	// the loyalty rating of a town ie : Loyal 53%
};


// error message for when player is trying to give a merc a travel order while he's underground.

STR16 gsUndergroundString[] =
{
	L"не может выйти на марш в подземельях.", 
};

STR16 gsTimeStrings[] =
{
	L"ч",				// hours abbreviation
	L"м",				// minutes abbreviation
	L"с",				// seconds abbreviation
	L"д",				// days abbreviation
};

// text for the various facilities in the sector

STR16 sFacilitiesStrings[] =
{
	L"Нет",	//важные объекты сектора
	L"Госпиталь",
	L"Фабрика",
	L"Тюрьма",
	L"Военная база",
	L"Аэропорт",
	L"Стрельбище",		// a field for soldiers to practise their shooting skills
};

// text for inventory pop up button

STR16 pMapPopUpInventoryText[] =
{
	L"Инвентарь",
	L"Выйти",
};

// town strings

STR16 pwTownInfoStrings[] =
{
	L"Размер",					// 0 // size of the town in sectors
	L"", 						// blank line, required
	L"Контроль", 					// how much of town is controlled
	L"Нет", 					// none of this town
	L"Шахта города", 				// mine associated with this town
	L"Лояльность",					// 5 // the loyalty level of this town
	L"Готовы", 					// the forces in the town trained by the player
	L"",
	L"Важные объекты", 				// main facilities in this town
	L"Уровень", 					// the training level of civilians in this town
	L"Тренировка ополчения",				// 10 // state of civilian training in town
	L"Ополчение", 					// the state of the trained civilians in the town
};

// Mine strings

STR16 pwMineStrings[] =
{
	L"Шахта",						// 0
	L"Серебро",
	L"Золото",
	L"Дневная выработка",
	L"Производственные возможности",
	L"Заброшена",				// 5
	L"Закрыта",
	L"Истощается",
	L"Идет добыча",
	L"Статус",
	L"Уровень добычи",
	L"Тип руды",				// 10
	L"Принадлежность",
	L"Лояльность",
//	L"Работ.шахтеры",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"Вражеские силы",
	L"Сектор",
	L"Количество предметов",
	L"Неизвестно",
	L"Под контролем",
	L"Да",
	L"Нет",
};

// error strings for inventory

STR16 pMapInventoryErrorString[] =
{
	L"%s слишком далеко.",	//Merc is in sector with item but not close enough
	L"Нельзя выбрать этого бойца.",  //MARK CARTER
	L"%s вне этого сектора, и не может подобрать предмет.",
	L"Во время боя вам придется подбирать вещи с земли.",
	L"Во время боя вам придется выкладывать вещи на землю на тактической карте.",
	L"%s вне этого сектора, и не может оставить предмет.",
	L"Во время битвы вы не можете заряжать оружие патронами из короба.",
};

STR16 pMapInventoryStrings[] =
{
	L"Локация", 			// sector these items are in
	L"Всего предметов", 		// total number of items in sector
};


// help text for the user

STR16 pMapScreenFastHelpTextList[] =
{
	L"Чтобы перевести наемника в другой отряд, назначить его врачом или отдать приказ ремонтировать вещи, щелкните по колонке 'ЗАНЯТИЕ'.",
	L"Чтобы приказать наемнику перейти в другой сектор, щелкните в колонке 'КУДА'.",
	L"Как только наемник получит приказ на передвижение, включится сжатие времени.",
	L"Нажатием левой кнопки мыши выбирается сектор. Еще одно нажатие нужно, чтобы отдать наемникам приказы на передвижение. Нажатие правой кнопки мыши на секторе откроет экран дополнительной информации.",
	L"Чтобы вызвать экран помощи - в любой момент времени нажмите 'h'.",
	L"Тестовый текст",
	L"Тестовый текст",
	L"Тестовый текст",
	L"Тестовый текст",
	L"Вы практически ничего не сможете сделать на этом экране, пока не прибудете в Арулько. Когда познакомитесь со своей командой, включите сжатие времени (кнопки в правом нижнем углу). Это ускорит течение времени, пока ваша команда не прибудет в Арулько.",
};

// movement menu text

STR16 pMovementMenuStrings[] =
{
	L"Отправить наемников в сектор", 	// title for movement box 
	L"Путь", 		// done with movement menu, start plotting movement
	L"Отмена", 		// cancel this menu
	L"Другое",		// title for group of mercs not on squads nor in vehicles
};


STR16 pUpdateMercStrings[] =
{
	L"Ой!:", 			// an error has occured
	L"Срок контракта истек:", 	// this pop up came up due to a merc contract ending
	L"Задание выполнили:", // this pop up....due to more than one merc finishing assignments
	L"Бойцы вернулись к своим обязанностям:", // this pop up ....due to more than one merc waking up and returing to work
	L"Бойцы ложатся спать:", // this pop up ....due to more than one merc being tired and going to sleep
	L"Скоро закончатся контракты у:", // this pop up came up due to a merc contract ending
};

// map screen map border buttons help text

STR16 pMapScreenBorderButtonHelpText[] =
{
	L"Населенные пункты (|W)",
	L"Шахты (|M)",
	L"Отряды и враги (|T)",
	L"Карта воздушного пространства (|A)",
	L"Вещи (|I)",
	L"Ополчение и враги (|Z)",
};


STR16 pMapScreenBottomFastHelp[] =
{
	L"Лэптоп (|L)",
	L"Тактический экран (|E|s|c)",
	L"Настройки (|O)",
	L"Сжатие времени (|+)", 	// time compress more 
	L"Сжатие времени (|-)", 	// time compress less
	L"Предыдущее сообщение (|С|т|р|е|л|к|а |в|в|е|р|х)\nПредыдущая страница (|P|g|U|p)", 	// previous message in scrollable list
	L"Следующее сообщение (|С|т|р|е|л|к|а |в|н|и|з)\nСледующая страница (|P|g|D|n)", 	// next message in the scrollable list
	L"Включить / выключить\nсжатие времени (|П|р|о|б|е|л)",	// start/stop time compression
};

STR16 pMapScreenBottomText[] =
{
	L"Текущий баланс", 	// current balance in player bank account
};

STR16 pMercDeadString[] =
{
	L"%s мертв(а)",
};


STR16 pDayStrings[] =
{
	L"День",
};

// the list of email sender names

STR16 pSenderNameList[] =
{
	L"Энрико",
	L"Psych Pro Inc.",
	L"Помощь",
	L"Psych Pro Inc.",
	L"Спек",
	L"R.I.S.",		//5
	L"Барри",
	L"Блад",
	L"Рысь",
	L"Гризли",
	L"Вики",			//10
	L"Тревор",
	L"Хряп",
	L"Иван",
	L"Анаболик",
	L"Игорь",			//15
	L"Тень",
	L"Рыжий",
	L"Жнец (Потрошитель)",
	L"Фидель",
	L"Лиска",				//20
	L"Сидней",
	L"Гас",
	L"Сдоба",
	L"Айс",
	L"Паук",		//25
	L"Скала (Клифф)",
	L"Бык",
	L"Стрелок",
	L"Тоска",
	L"Рейдер",		//30
	L"Сова",
	L"Статик",
	L"Лен",
	L"Дэнни",
	L"Маг",
	L"Стефан",
	L"Лысый",
	L"Злобный",
	L"Доктор Кью",
	L"Гвоздь",
	L"Тор",
	L"Стрелка",
	L"Волк",
	L"ЭмДи",
	L"Лава",
	//----------
	L"M.I.S. Страховка",	
	L"Бобби Рэй",
	L"Босс",
	L"Джон Кульба",
	L"А.I.М.",
};


// next/prev strings

STR16 pTraverseStrings[] =
{
  L"<<",
  L">>",
};

// new mail notify string 

STR16 pNewMailStrings[] =
{
 L"Получена новая почта...",
};


// confirm player's intent to delete messages

STR16 pDeleteMailStrings[] =
{
 L"Удалить письмо?",
 L"Удалить, НЕ ПРОЧИТАВ?",
};


// the sort header strings

STR16 pEmailHeaders[] =
{
	L"От:",
	L"Тема:",
	L"День:",
};

// email titlebar text

STR16 pEmailTitleText[] =
{
	L"Почтовый ящик",
};


// the financial screen strings
STR16 pFinanceTitle[] =
{
	L"Финансовый отчет",		//the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] =
{
	L"Доход:", 				// credit (subtract from) to player's account
	L"Расход:", 				// debit (add to) to player's account
	L"Вчерашний чистый доход:",
	L"Вчерашние другие поступления:",
	L"Вчерашний расход:",
	L"Баланс на конец дня:",
	L"Чистый доход сегодня:",
	L"Другие поступления за сегодня:",
	L"Расход за сегодня:",
	L"Текущий баланс:",
	L"Ожидаемый доход:",
	L"Ожидаемый баланс:", 		// projected balance for player for tommorow
};


// headers to each list in financial screen

STR16 pFinanceHeaders[] =
{
  L"День", 					// the day column
	L"Доход", 				// the credits column
	L"Расход",				// the debits column
	L"Операции", 			// transaction type - see TransactionText below
	L"Баланс", 				// balance at this point in time
	L"Стр.", 				// page number
	L"Дней", 				// the day(s) of transactions this page displays 
};


STR16 pTransactionText[] =
{
	L"Начисленный процент",			// interest the player has accumulated so far
	L"Анонимный взнос",
	L"Перевод средств", 
	L"Нанят", 				// Merc was hired
	L"Покупки у Бобби Рэя", 		// Bobby Ray is the name of an arms dealer
	L"Оплата счета M.E.R.C.",
	L"%s: страховка.", 		// medical deposit for merc
	L"I.M.P. анализ профиля", 		// IMP is the acronym for International Mercenary Profiling
	L"%s: куплена страховка.", 
	L"%s: Страховка уменьшена",
	L"%s: Продление страховки", 				// johnny contract extended
	L"для %s: Страховка аннулирована", 
	L"%s: Требуется страховка", 		// insurance claim for merc
	L"1 день", 				// merc's contract extended for a day
	L"7 дней", 				// merc's contract extended for a week
	L"14 дней", 				// ... for 2 weeks
	L"Доход шахты", 
	L"", //String nuked
	L"Куплены цветы",
	L"%s: Возврат мед. депозита",
	L"%s: Остаток мед. депозита",
	L"%s: Мед. депозит удержан",
	L"%s: оплата услуг.",		// %s is the name of the npc being paid
	L"%s берет наличные.", 			// transfer funds to a merc
	L"%s: переводит деньги.", 		// transfer funds from a merc
	L"%s: оружие ополчению.", // initial cost to equip a town's militia
	L"%s продал вам вещи.",	//is used for the Shop keeper interface.  The dealers name will be appended to the end of the string.
	L"%s кладет наличные на счет.",
	L"Снаряжение продано населению",
};

STR16 pTransactionAlternateText[] =
{
	L"Страховка для", 				// insurance for a merc
	L"%s: контракт продлен на 1 день.", 				// entend mercs contract by a day
	L"%s: контракт продлен на 7 дней.",
	L"%s: контракт продлен на 14 дней.",
};

// helicopter pilot payment 

STR16 pSkyriderText[] =
{
	L"Небесному Всаднику заплачено $%d", 			// skyrider was paid an amount of money
	L"Вы все еще должны Небесному Всаднику $%d.", 		// skyrider is still owed an amount of money
	L"Небесный Всадник завершил заправку.",	// skyrider has finished refueling
	L"",//unused
	L"",//unused
	L"Небесный Всадник готов к полету.", // Skyrider was grounded but has been freed
	L"У Небесного Всадника нет пассажиров. Если вы хотите переправить бойцов в этот сектор, посадите их в вертолет (приказ 'Машина/Вертолет')."
};


// strings for different levels of merc morale

STR16 pMoralStrings[] =
{
	L"Отлично", 
	L"Хорошо",
	L"Норма",
	L"Низкая",
	L"Паника",
	L"Ужас",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.

STR16 pLeftEquipmentString[] =
{ 
	L"%s оставляет свою экипировку в Омерте (A9).", 	//%s может взять заказанную экипировку в Омерте (A9).
	L"%s оставляет свою экипировку в Драссене (B13).",	//%s может взять заказанную экипировку в Драссене (B13).
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] =
{
	L"Здоровье",
	L"Энергия",
	L"Боевой дух",
	L"Состояние",	// the condition of the current vehicle (its "health")
	L"Бензин",	// the fuel level of the current vehicle (its "energy")
};


STR16 pMapScreenPrevNextCharButtonHelpText[] =
{
	L"Предыдущий боец\n(|С|т|р|е|л|к|а |В|л|е|в|о)", 			// previous merc in the list
	L"Следующий боец\n(|С|т|р|е|л|к|а |В|п|р|а|в|о)", 				// next merc in the list
};


STR16 pEtaString[] =
{
	L"РВП:", 				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"Вы больше никогда не увидите этот предмет. Уверены?", 	// do you want to continue and lose the item forever
	L"Этот предмет кажется ОЧЕНЬ важным. Вы ДЕЙСТВИТЕЛЬНО хотите выкинуть его?", // does the user REALLY want to trash this item
};


STR16 pMapErrorString[] = 
{
	L"Отряд не может выйти на марш, когда один из наемников спит.",

//1-5
	L"Сначала выведите отряд на поверхность.",
	L"Выйти на марш? Да тут же враги повсюду!",
	L"Чтобы выйти на марш, наемники должны быть назначены в отряд или посажены в машину.",
	L"У вас еще нет ни одного бойца.",	// you have no members, can't do anything 
	L"Наемник не может выполнить.",		// merc can't comply with your order
//6-10
	L"нуждается в сопровождении чтобы идти. Назначьте его с кем-нибудь в отряд.", // merc can't move unescorted .. for a male
	L"нуждается в сопровождении чтобы идти. Назначьте ее с кем-нибудь в отряд.", // for a female
	L"Наемник еще не прибыл в Арулько!",
	L"Кажется, сначала надо уладить проблемы с контрактом.",
	L"Бежать от самолета? Только после вас!",	// Cannot give a movement order. Air raid is going on.
//11-15
	L"Выступить на марш? Да у нас тут бой идет!",
	L"Вы попали в засаду кошек-убийц в секторе %s!",
	L"Вы только что попали в логово кошек-убийц, сектор I16!", 
	L"", 
	L"База ПВО в %s была захвачена.",
//16-20
	L"Шахта в %s была захвачена врагом. Ваш дневной доход сократился до %s в день.",
	L"Враг занял без сопротивления сектор %s.",
	L"Как минимум один из ваших бойцов не может выполнить этот приказ.",
	L"%s не может присоединиться к %s - нет места.",
	L"%s не может присоединиться к %s - слишком большое расстояние.",
//21-25
	L"Шахта в %s была захвачена войсками Дейдраны!",
	L"Войска Дейдраны только что вторглись на базу ПВО в %s.",
	L"Войска Дейдраны только что вторглись в %s.",
	L"Войска Дейдраны были замечены в %s.",
	L"Войска Дейдраны только что захватили %s.",
//26-30
	L"Как минимум один из ваших бойцов не хочет спать.",
	L"Как минимум один из ваших бойцов не может проснуться.",
	L"Ополченцы не появятся, пока не завершат тренировку.",
	L"%s сейчас не в состоянии принять приказ о перемещении.",
	L"Ополченцы вне границ города не могут перейти в другой сектор.",
//31-35
	L"Вы не можете держать ополченцев в %s.",
	L"Пустая машина не может двигаться!",
	L"%s из-за тяжелых ранений не может идти!",
	L"Сначала вам нужно покинуть музей!",
	L"%s мертв(а)!",
//36-40
	L"%s не может переключиться на %s, так как находится в движении.",
	L"%s не может сесть в машину с этой стороны.",
	L"%s не может вступить в %s",
	L"Вы не можете включить сжатие времени, пока не наймете новых бойцов!",
	L"Эта машина может двигаться только по дорогам!",
//41-45
	L"Вы не можете переназначить наемников на марше.",
	L"У машины закончился бензин!",
	L"%s еле волочит ноги и идти не может.",
	L"Ни один из пассажиров не в состоянии вести машину.",
	L"Один или несколько наемников из этого отряда не могут сейчас двигаться.",
//46-50
	L"Один или несколько наемников не могут сейчас двигаться.",
	L"Машина сильно повреждена!",
	L"Внимание! Тренировать ополченцев в одном секторе могут не более двух наемников.",
	L"Роботом обязательно нужно управлять. Назначьте наемника с пультом и робота в один отряд.",
};


// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"Еще раз щелкните по точке назначения, чтобы подтвердить путь или щелкните по другому сектору, чтобы установить больше путевых точек.",
	L"Путь движения подтвержден.",
	L"Точка назначения не изменена.",
	L"Путь движения отменен.",
	L"Путь сокращен.",
};


// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"Выберите сектор, в который прибудут наемники.",
	L"Вновь прибывшие наемники высадятся в %s.",
	L"Высадить здесь наемников нельзя. Воздушное пространство не безопасно!",
	L"Отменено. Сектор прибытия не изменился.",
	L"Небо над %s более не безопасно! Место высадки было перемещено в %s.",
};


// help text for mouse regions

STR16 pMiscMapScreenMouseRegionHelpText[] =
{
	L"Показать снаряжение (|В|в|о|д)",
	L"Выкинуть предмет",
	L"Скрыть снаряжение (|В|в|о|д)",
};



// male version of where equipment is left
STR16 pMercHeLeaveString[] =
{
	L"Должен ли %s оставить свою амуницию здесь (в %s) или позже в Драссене (B13) перед отлетом?",
	L"Должен ли %s оставить свою амуницию здесь (в %s) или позже в Омерте (A9) перед отлетом?",
	L"скоро уходит и оставит свою амуницию в Омерте (А9).",
	L"скоро уходит и оставит свою амуницию в Драссене (B13).",
	L"%s скоро уходит и оставит свою амуницию в %s.",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"Должна ли %s оставить свою амуницию здесь (в %s) или позже в Драссене (B13) перед отлетом?",
	L"Должна ли %s оставить свою амуницию здесь (в %s) или позже в Омерте (A9) перед отлетом?",
	L"скоро уходит и оставит свою амуницию в Омерте (А9).",
	L"скоро уходит и оставит свою амуницию в Драссене (B13).",
	L"%s скоро уходит и оставит свою амуницию в %s.",
};


STR16 pMercContractOverStrings[] =
{
	L"отправляется домой, так как его контракт завершен.", 		// merc's contract is over and has departed
	L"отправляется домой, так как ее контракт завершен.", 		// merc's contract is over and has departed
	L"уходит, так как его контракт был прерван.", 		// merc's contract has been terminated
	L"уходит, так как ее контракт был прерван.",		// merc's contract has been terminated
	L"Вы должны M.E.R.C. слишком много денег, так что %s уходит.", // Your M.E.R.C. account is invalid so merc left
};

// Text used on IMP Web Pages

STR16 pImpPopUpStrings[] =
{
	L"Неверный код доступа.",
	L"Это приведет к потере уже полученных результатов тестирования. Вы уверены?",
	L"Введите правильное имя и укажите пол.",
	L"Предварительный анализ вашего счета показывает, что вы не можете позволить себе пройти тестирование.",
	L"Сейчас вы не можете выбрать этот пункт.",
	L"Чтобы закончить анализ, нужно иметь место еще хотя бы для одного члена команды.",
	L"Профиль уже создан.",
	L"Не могу загрузить I.M.P.-персонаж с диска.",
	L"Вы достигли максимального количества I.M.P.-персонажей.",
	L"У вас в команде уже есть три I.M.P.-персонажа того же пола.",
	L"Вы не можете позволить себе такой I.M.P.-персонаж.",
	L"Новый I.M.P.-персонаж присоединился к команде.",
};


// button labels used on the IMP site

STR16 pImpButtonText[] =
{
	L"Информация о нас", 			// about the IMP site
	L"НАЧАТЬ", 			// begin profiling
	L"Способности", 		// personality section
	L"Характеристики", 		// personal stats/attributes section
	L"Портрет", 			// the personal portrait selection
	L"Голос: %d", 			// the voice selection
	L"Готово", 			// done profiling
	L"Начать сначала", 		// start over profiling
	L"Да, я выбираю отмеченный ответ.", 
	L"Да", 
	L"Нет",
	L"Готово", 			// finished answering questions
	L"Назад", 			// previous question..abbreviated form
	L"Дальше", 			// next question
	L"ДА.", 		// yes, I am certain 
	L"НЕТ, Я ХОЧУ НАЧАТЬ СНОВА.", // no, I want to start over the profiling process
	L"ДА",
	L"НЕТ",
	L"Назад", 			// back one page
	L"Отменить", 			// cancel selection
	L"Да, все верно.",
	L"Нет, еще раз взгляну.",
	L"Регистрация", 			// the IMP site registry..when name and gender is selected
	L"Анализ данных", 			// analyzing your profile results
	L"Готово",
	L"Голос",
};

STR16 pExtraIMPStrings[] =
{
	L"Сейчас необходимо указать ваши способности.",
	L"Способности героя заданы. Чтобы завершить процесс, укажите свои характеристики.",
	L"Чтобы создать профиль, необходимо указать некоторые данные. Начните с выбора портрета.",
	L"Теперь, выберите голос, который вам подходит.",
};

STR16 pFilesTitle[] =
{
  L"Просмотр данных",
};

STR16 pFilesSenderList[] =
{
  L"Отчет разведки", 		// the recon report sent to the player. Recon is an abbreviation for reconissance
	L"В розыске №1", 		// first intercept file .. Intercept is the title of the organization sending the file...similar in function to INTERPOL/CIA/KGB..refer to fist record in files.txt for the translated title
	L"В розыске №2",	   // second intercept file
	L"В розыске №3",			 // third intercept file
	L"В розыске №4", // fourth intercept file
	L"В розыске №5", // fifth intercept file
	L"В розыске №6", // sixth intercept file
};

// Text having to do with the History Log

STR16 pHistoryTitle[] =
{
	L"Журнал событий", 
};

STR16 pHistoryHeaders[] =
{
	L"День", 			// the day the history event occurred
	L"Стр.", 			// the current page in the history report we are in
	L"День", 			// the days the history report occurs over
	L"Локация", 			// location (in sector) the event occurred
	L"Событие", 			// the event label
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
	L"Нанят(а) %s из A.I.M.", 										// merc was hired from the aim site
	L"Нанят(а) %s из M.E.R.C.", 									// merc was hired from the aim site
	L"%s мертв(а).", 															// merc was killed
	L"Оплачены услуги M.E.R.C.",				// paid outstanding bills at MERC
	L"Принято задание от Энрико Чивалдори",	
	//6-10
	L"Воспользовались услугами I.M.P.",
	L"Оформлена страховка для %s.", 				// insurance contract purchased
	L"%s: cтраховой контракт аннулирован.", 				// insurance contract canceled
	L"Выплата страховки %s.", 							// insurance claim payout for merc
	L"%s: контракт продлен на 1 день.", 						// Extented "mercs name"'s for a day
	//11-15
	L"%s: контракт продлен на 7 дней.", 					// Extented "mercs name"'s for a week
	L"%s: контракт продлен на 14 дней.", 					// Extented "mercs name"'s 2 weeks
	L"Вы уволили %s.", 													// "merc's name" was dismissed.
	L"%s уходит от вас.", 																		// "merc's name" quit.
	L"получено задание.", 															// a particular quest started
	//16-20
	L"задание выполнено.",
	L"Поговорили со старшим горняком города %s",									// talked to head miner of town
	L"%s освобожден(а).",
	L"Включен режим чит-кодов",
	L"Провизия будет доставлена в Омерту завтра.",
	//21-25
	L"%s ушла, чтобы выйти замуж за Дерила Хика.",
	L"Истек контракт у %s.",
	L"Нанят(а) %s.",
	L"Энрико сетует на отсутствие успехов в кампании.",
	L"Победа в сражении!",
	//26-30
	L"В шахте %s иссякает запас руды.",
	L"Шахта %s истощилась.",
	L"Шахта %s закрыта.",
	L"Шахта %s снова работает.",
	L"Получена информация о тюрьме Тикса.",
	//31-35
	L"Узнали об Орте - секретном военном заводе.",
	L"Ученый из Орты подарил вам ракетные винтовки.",
	L"Королева Дейдрана нашла применение трупам.",
	L"Фрэнк говорил что-то о боях в Сан-Моне.",
	L"Пациенту кажется, что он что-то видел в шахтах.",
	//36-40
	L"Встретили Дэвина - торговца взрывчаткой.",
	L"Пересеклись с бывшим наемником A.I.M., Майком!", 
	L"Встретили Тони, торговца оружием.",
	L"Получена ракетная винтовка от сержанта Кротта.",
	L"Документы на магазин Энджела переданы Кайлу.",
	//41-45
	L"Шиз предлагает построить робота.",	//может, собрать робота?
	L"Болтун может сделать варево, обманывающее жуков.",
	L"Кит отошел от дел.",
	L"Говард поставлял цианиды Дейдране.",
	L"Встретили торговца Кита из Камбрии.",
	//46-50
	L"Встретили Говарда, фармацевта из Балайма.",
	L"Встретили Перко. Он держит небольшую мастерскую.",
	L"Встретили Сэма из Балайма. Он торгует железками.",
	L"Франц разбирается в электронике и других вещах.",
	L"Арнольд держит мастерскую в Граме.",
	//51-55
	L"Фредо из Грама чинит электронику.",
	L"Один богатей из Балайма дал вам денег.",
	L"Встретили старьевщика Джейка.",
	L"Один бродяга дал нам электронную карточку.",
	L"Вальтер подкуплен, он откроет дверь в подвал.",
	//56-60
	L"Дэйв заправит машину бесплатно, если будет бензин.",
	L"Дали взятку Пабло.",
	L"Босс держит деньги в шахте Сан-Моны.",
	L"%s: победа в бое без правил.",
	L"%s: проигрыш в бое без правил.",
	//61-65
	L"%s: дисквалификация в бое без правил.",	//дисквалификация из боя?
	L"В заброшенной шахте найдена куча денег.",
	L"Встречен убийца, посланный Боссом.",
	L"Потерян контроль над сектором",				//ENEMY_INVASION_CODE
	L"Отбита атака врага",
	//66-70
	L"Сражение проиграно",							//ENEMY_ENCOUNTER_CODE
	L"Смертельная засада",						//ENEMY_AMBUSH_CODE
	L"Вырвались из засады",
	L"Атака провалилась!",			//ENTERING_ENEMY_SECTOR_CODE
	L"Успешная атака!",
	//71-75
	L"Атака тварей",			//CREATURE_ATTACK_CODE
	L"Кошки-убийцы уничтожили ваш отряд.",			//BLOODCAT_AMBUSH_CODE
	L"Все кошки-убийцы убиты",
	L"%s был убит(а).",
	L"Отдали Кармену голову террориста.",
	L"Убийца ушел.",
	L"%s убит(а) вашим отрядом.",
};

STR16 pHistoryLocations[] =
{
	L"Н/Д",						// N/A is an acronym for Not Applicable
};

// icon text strings that appear on the laptop

STR16 pLaptopIcons[] =
{
	L"Почта",
	L"Сайты",
	L"Финансы",
	L"Команда",
	L"Журнал",
	L"Данные",
	L"Выключить",
	L"sir-FER 4.0",			// our play on the company name (Sirtech) and web surFER
};

// bookmarks for different websites
// IMPORTANT make sure you move down the Cancel string as bookmarks are being added

STR16 pBookMarkStrings[] =
{
	L"A.I.M.",
	L"Бобби Рэй",
	L"I.M.P.",
	L"М.Е.R.С.",
	L"Похороны",
	L"Цветы",
	L"Страховка",
	L"Отмена",
};

STR16 pBookmarkTitle[] =
{
	L"Закладки",
	L"Позже это меню можно вызвать правым щелчком мыши.",
};

// When loading or download a web page

STR16 pDownloadString[] =
{
	L"Загрузка",
	L"Обновление",
};

//This is the text used on the bank machines, here called ATMs for Automatic Teller Machine

STR16 gsAtmSideButtonText[] =
{
	L"OK",
	L"Взять", 			// take money from merc
	L"Дать", 			// give money to merc
	L"Отмена", 			// cancel transaction
	L"Очист.", 			// clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] =
{
	L"Перевести $", 		// transfer money to merc -- short form
	L"Параметры", 			// view stats of the merc
	L"Снаряжение", 			// view the inventory of the merc
	L"Контракт",
};

STR16 sATMText[ ]=
{
	L"Перевести средства?", 		// transfer funds to merc?
	L"Уверены?", 			// are we certain?
	L"Ввести сумму", 		// enter the amount you want to transfer to merc
	L"Выбрать тип", 		// select the type of transfer to merc
	L"Не хватает денег", 	// not enough money to transfer to merc
	L"Сумма должна быть кратна $10", // transfer amount must be a multiple of $10
};

// Web error messages. Please use foreign language equivilant for these messages. 
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator

STR16 pErrorStrings[] =
{
	L"Ошибка",
	L"Сервер не имеет записи DNS.",	
	L"Проверьте адрес и попробуйте ещё раз.",
	L"OK", //Превышено время ожидания ответа сервера.
	L"Обрыв соединения с сервером.",
};


STR16 pPersonnelString[] =
{
	L"Бойцов:", 			// mercs we have
};


STR16 pWebTitle[ ]=
{
	L"sir-FER 4.0",		// our name for the version of the browser, play on company name
};


// The titles for the web program title bar, for each page loaded

STR16 pWebPagesTitles[] =
{
	L"А.I.M.",
	L"A.I.M. Состав",
	L"A.I.M. Портреты",		// a mug shot is another name for a portrait
	L"A.I.M. Сортировка",
	L"A.I.M.",
	L"A.I.M. Галерея",
	L"A.I.M. Правила",
	L"A.I.M. История",
	L"A.I.M. Ссылки",
	L"M.E.R.C.",
	L"M.E.R.C. Счета",
	L"M.E.R.C. Регистрация",
	L"M.E.R.C. Оглавление",
	L"Бобби Рэй",
	L"Бобби Рэй - оружие",
	L"Бобби Рэй - боеприпасы",
	L"Бобби Рэй - броня",
	L"Бобби Рэй - разное",							//misc is an abbreviation for miscellaneous				
	L"Бобби Рэй - вещи б/у.",
	L"Бобби Рэй - почтовый заказ",
	L"I.M.P.",
	L"I.M.P.",
	L"\"Цветы по всему миру\"",
	L"\"Цветы по всему миру\" - галерея",
	L"\"Цветы по всему миру\" - бланк заказа",
	L"\"Цветы по всему миру\" - открытки",
	L"Страховые агенты: Малеус, Инкус и Стэйпс",
	L"Информация",
	L"Контракт",
	L"Комментарии",
	L"Похоронная служба Макгилликатти",
	L"",
	L"Адрес не найден.",
	L"Бобби Рэй - последние поступления",//@@@3 Translate new text
	L"",
	L"",
};

STR16 pShowBookmarkString[] =
{
	L"Подсказка",
	L"Щелкните еще раз по кнопке \"Сайты\" для отображения меню сайтов.",
};

STR16 pLaptopTitles[] =
{
	L"Почтовый ящик",
	L"Просмотр данных",
	L"Персонал",
	L"Финансовый отчет",
	L"Журнал",
};

STR16 pPersonnelDepartedStateStrings[] =
{
	//reasons why a merc has left.
	L"Погиб в бою",
	L"Уволен",
	L"Другое",
	L"Замужем",
	L"Контракт истек",
	L"Выход",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] =
{
	L"В команде",
	L"Выбывшие",
	L"Гонорар за день:",
	L"Высший гонорар:",
	L"Низший гонорар:",
	L"Погибло в боях:",
	L"Уволено:",
	L"Другое:",
};


STR16 pPersonnelCurrentTeamStatsStrings[] =
{
	L"Худший",
	L"Среднее",
	L"Лучший",
};


STR16 pPersonnelTeamStatsStrings[] =
{
	L"ЗДР",
	L"ПРВ",
	L"ЛОВ",
	L"СИЛ",
	L"ЛИД",
	L"ИНТ",
	L"ОПТ",
	L"МЕТ",
	L"МЕХ",
	L"ВЗР",
	L"МЕД",
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
	L"Контракт",
};

// text that appears on the update panel buttons

STR16 pUpdatePanelButtons[] =
{
	L"Далее",
	L"Стоп",
};

// Text which appears when everyone on your team is incapacitated and incapable of battle

CHAR16 LargeTacticalStr[][ LARGE_STRING_LENGTH ] =
{
	L"Вы потерпели поражение в этом секторе!",
	L"Рептионы, не испытывая угрызений совести, пожрут всех до единого!",
	L"Ваши бойцы захвачены в плен (некоторые без сознания)!",
	L"Ваши бойцы захвачены в плен.",
};


//Insurance Contract.c
//The text on the buttons at the bottom of the screen.

STR16			InsContractText[] = 
{ 
	L"Назад",
	L"Дальше",
	L"Да",
	L"Очистить",
};



//Insurance Info
// Text on the buttons on the bottom of the screen

STR16		InsInfoText[] = 
{ 
	L"Назад", 
	L"Дальше",
};



//For use at the M.E.R.C. web site. Text relating to the player's account with MERC

STR16			MercAccountText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Внести сумму",
	L"В начало", 
	L"Номер счета:",
	L"Наемник",
	L"Дней",
	L"Ставка",	//5
	L"Стоимость",
	L"Всего:",
	L"Вы подтверждаете платеж в размере %s?",		//the %s is a string that contains the dollar amount ( ex. "$150" )
};

// Merc Account Page buttons
STR16			MercAccountPageText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Назад",
  L"Дальше",
};

//For use at the M.E.R.C. web site. Text relating a MERC mercenary


STR16			MercInfo[] =
{
	L"Здоровье",
	L"Проворность",
	L"Ловкость",
	L"Сила",
	L"Лидерство",
	L"Интеллект",
	L"Уровень опыта",
	L"Меткость",
	L"Механика",
	L"Взрывчатка",
	L"Медицина",

	L"Назад",
	L"Нанять",
	L"Дальше",
	L"Дополнительная информация",
	L"В начало",
	L"Нанят",
	L"Oплaтa",
	L"в день",
	L"Погиб",

	L"Похоже, вы пытаетесь нанять более 18 наемников, а это недопустимо.",
	L"Недоступно",											
};



// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16			MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"Открыть счет",
	L"Отмена",
	L"Вы еще не зарегистрировались. Желаете открыть счет?",
};



// For use at the M.E.R.C. web site. MERC Homepage

STR16			MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Спек Т. Кляйн, основатель и хозяин",
	L"Открыть счет",
	L"Просмотр счета",
	L"Просмотр файлов",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",
};

// For use at MiGillicutty's Web Page.

STR16			sFuneralString[] =
{
	L"Похоронное агентство Макгилликатти: скорбим вместе с семьями усопших с 1983.",
	L"Директор по похоронам и бывший наемник А.I.М. - Мюррэй Макгилликатти \"Папаша\", специалист по части похорон.",
	L"Всю жизнь Папашу сопровождали смерть и утраты, поэтому он, как никто, познал их тяжесть.",
	L"Похоронное агентство Макгилликатти предлагает широкий спектр ритуальных услуг - от жилетки, в которую можно поплакать, до восстановления сильно поврежденных останков.",
	L"Похоронное агентство Макгилликатти поможет вам и вашим родственникам покоиться с миром.",

	// Text for the various links available at the bottom of the page
	L"ДОСТАВКА ЦВЕТОВ",
	L"КОЛЛЕКЦИЯ УРН И ГРОБОВ",
	L"УСЛУГИ ПО КРЕМАЦИИ",
	L"ПОМОЩЬ В ПРОВЕДЕНИИ ПОХОРОН",
	L"ПОХОРОННЫЕ РИТУАЛЫ",

	// The text that comes up when you click on any of the links ( except for send flowers ).
	L"К сожалению, наш сайт не закончен, в связи с утратой в семье. Мы постараемся продолжить работу после прочтения завещания и выплат долгов умершего. Сайт вскоре откроется.",
	L"Мы искренне сочувствуем вам в это трудное время. Заходите еще.",
};

// Text for the florist Home page

STR16			sFloristText[] = 
{
	//Text on the button on the bottom of the page

	L"Галерея",

	//Address of United Florist

	L"\"Мы сбросим ваш букет где угодно!\"",
	L"1-555-SCENT-ME",
	L"333 NoseGay Dr,Seedy City, CA USA 90210",
	L"http://www.scent-me.com",

	// detail of the florist page

	L"Мы работаем быстро и эффективно!",
	L"Гарантируем доставку на следующий день практически в любой уголок мира. Есть некоторые ограничения.",
	L"Гарантируем самые низкие цены в мире!",
	L"Покажите нам рекламу более дешевого сервиса и получите 10 бесплатных роз.",
	L"\"Крылатая Флора\", занимаемся фауной и цветами с 1981 г.",
	L"Наши летчики, бывшие пилоты бомбардировщиков, сбросят ваш букет в радиусе 10 миль от заданного района. Когда угодно и сколько угодно!",
	L"Позвольте нам удовлетворить ваши цветочные фантазии.",
	L"Пусть Брюс, известный во всем мире садовник, сам соберет вам отличный букет в нашем саду.",
	L"И запомните, если у нас нет таких цветов, мы быстро вырастим то, что вам надо!",
};



//Florist OrderForm

STR16			sOrderFormText[] = 
{
	//Text on the buttons

	L"Назад",
	L"Послать",
	L"Отмена",
	L"Галерея",

	L"Название букета:",
	L"Цена:",			//5
	L"Номер заказа:",
	L"Доставить",
	L"Завтра",
	L"Как будете в тех краях",
	L"Место доставки",			//10
	L"Дополнительно",
	L"Сломать цветы ($10)",
	L"Черные розы ($20)",
	L"Увядший букет ($10)",
	L"Фруктовый пирог (если есть) ($10)",		//15
	L"Текст поздравления:",
	L"Ввиду небольшого размера открытки, постарайтесь уложиться в 75 символов.",
	L"...или выберите одну из",

	L"СТАНДАРТНЫХ ОТКРЫТОК",	
	L"Информация о счете",//20

	//The text that goes beside the area where the user can enter their name

	L"Название:",
};




//Florist Gallery.c

STR16		sFloristGalleryText[] =
{
	//text on the buttons

	L"Назад",	//abbreviation for previous
	L"Дальше",	//abbreviation for next

	L"Выберите букет, которые хотите послать.",
	L"Примечание: Если Вам нужно послать увядший или сломанный букет - заплатите еще $10.",

	//text on the button

	L"В начало",
};

//Florist Cards

STR16			sFloristCards[] =
{
	L"Выберите текст, который будет напечатан на открытке.",
	L"Назад",
};



// Text for Bobby Ray's Mail Order Site

STR16			BobbyROrderFormText[] = 
{
	L"Бланк заказа",				//Title of the page
	L"Штк",					// The number of items ordered
	L"Вес (%s)",			// The weight of the item
	L"Название",				// The name of the item
	L"цена 1 вещи",				// the item's weight
	L"Итого",				//5	// The total price of all of items of the same type
	L"Стоимость",				// The sub total of all the item totals added
	L"ДиУ (см. Место Доставки)",		// S&H is an acronym for Shipping and Handling 
	L"Всего",			// The grand total of all item totals + the shipping and handling
	L"Место доставки",				
	L"Скорость доставки",			//10	// See below
	L"Цена (за %s.)",			// The cost to ship the items
	L"Экспресс-доставка",			// Gets deliverd the next day
	L"2 рабочих дня",			// Gets delivered in 2 days
	L"Обычная доставка",			// Gets delivered in 3 days
	L"ОЧИСТИТЬ",//15			// Clears the order page
	L"ЗАКАЗАТЬ",			// Accept the order
	L"Назад",				// text on the button that returns to the previous page
	L"В начало",				// Text on the button that returns to the home page
	L"* - вещи, бывшие в употреблении",		// Disclaimer stating that the item is used
	L"Вы не можете это оплатить.",		//20	// A popup message that to warn of not enough money
	L"<НЕ ВЫБРАНО>",				// Gets displayed when there is no valid city selected
	L"Вы действительно хотите отправить груз в %s?",		// A popup that asks if the city selected is the correct one
	L"Вес груза**",			// Displays the weight of the package
	L"** Мин. вес",				// Disclaimer states that there is a minimum weight for the package
	L"Заказы",	
};


STR16			BobbyRFilter[] =
{
	// Guns
	L"Тяжелое",
	L"Пистолеты",
	L"Авт.пистол.",
	L"ПП",
	L"Винтовки",
	L"Сн.винтовки",
	L"Шт.винтовки",
	L"Пулеметы",
	L"Ружья",

	// Ammo
	L"Пистолеты",
	L"Авт.пистол.",
	L"ПП",
	L"Винтовки",
	L"Сн.винтовки",
	L"Шт.винтовки",
	L"Пулеметы",
	L"Ружья",

	// Used
	L"Оружие",
	L"Броня",
	L"Разгр.с-мы",
	L"Разное",

	// Armour
	L"Каски",
	L"Жилеты",
	L"Брюки",
	L"Пластины",

	// Misc
	L"Режущие",
	L"Метательн.",
	L"Дробящие",
	L"Гранаты",
	L"Бомбы",
	L"Аптечки",
	L"Наборы",
	L"Головные",
	L"Разгр.с-мы",
	L"Разное",
};


// This text is used when on the various Bobby Ray Web site pages that sell items

STR16			BobbyRText[] = 
{
	L"Заказать",				// Title
	// instructions on how to order
	L"Нажмите на товар. Левая кнопка - добавить, правая кнопка - уменьшить. После того как выберете товар, оформите заказ.",			

	//Text on the buttons to go the various links

	L"Назад",		// 
	L"Оружие", 			//3
	L"Патроны",			//4
	L"Броня",			//5
	L"Разное",			//6	//misc is an abbreviation for miscellaneous
	L"Б/У",			//7
	L"Далее",
	L"БЛАНК ЗАКАЗА",
	L"В начало",			//10

	//The following 2 lines are used on the Ammunition page.  
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"У вашей команды есть",//11
	L"оруж., использующее этот тип боеприпасов", //12

	//The following lines provide information on the items

	L"Вес:",		// Weight of all the items of the same type
	L"Кал.:",			// the caliber of the gun
	L"Маг:",			// number of rounds of ammo the Magazine can hold
	L"Дист:",			// The range of the gun
	L"Урон:",			// Damage of the weapon	
	L"Скор:",			// Weapon's Rate Of Fire, acronym ROF
	L"Цена:",			// Cost of the item
	L"Склад:",			// The number of items still in the store's inventory
	L"Штук в заказе:",		// The number of items on order
	L"Урон:",			// If the item is damaged
	L"Вес:",			// the Weight of the item
	L"Итого:",			// The total cost of all items on order
	L"* %% до износа",		// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	
	L"Чёрт! Эта форма поддерживает не более 10 предметов в одном заказе. Если вы хотите заказать больше (а мы надеемся, вы хотите), то заполните еще один заказ и примите наши извинения за неудобства.", 

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"Извините, но данного товара нет на складе. Попробуйте заглянуть позже.",

	//A popup that tells the user that the store is temporarily sold out

	L"Извините, но данного товара пока нет на складе.",

};


// Text for Bobby Ray's Home Page

STR16			BobbyRaysFrontText[] =
{
	//Details on the web site

	L"Здесь вы найдете лучшие и новейшие образцы оружия",
	L"Мы снабдим вас всем, что нужно для победы над противником",
	L"ВЕЩИ Б/У",

	//Text for the various links to the sub pages

	L"РАЗНОЕ",
	L"ОРУЖИЕ",
	L"БОЕПРИПАСЫ",		//5
	L"БРОНЯ",

	//Details on the web site

	L"Если у нас чего-то нет, то этого нет нигде!",
	L"В разработке",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"А.I.M. Состав",				// Title
	// Title for the way to sort
	L"Сортировка:",					

	// sort by...

	L"Цена",											
	L"Опыт",
	L"Меткость",
	L"Медицина",
	L"Взрывчатка",
	L"Механика",

	//Text of the links to other AIM pages

	L"Показать фотографии наемников",
	L"Просмотреть информацию о наемниках",
	L"Просмотреть архивную галерею A.I.M.", 

	// text to display how the entries will be sorted

	L"По возрастанию",
	L"По убыванию",
};


//Aim Policies.c
//The page in which the AIM policies and regulations are displayed

STR16		AimPolicyText[] =
{
	// The text on the buttons at the bottom of the page

	L"Назад",
	L"В начало", 
	L"Оглавление",
	L"Дальше",
	L"Не согласен",
	L"Согласен",
};



//Aim Member.c
//The page in which the players hires AIM mercenaries

// Instructions to the user to either start video conferencing with the merc, or to go the mug shot index

STR16			AimMemberText[] =
{
	L"Левая кнопка мыши",
	L"чтобы связаться с бойцом.",
	L"Правая кнопка мыши",
	L"экран с фотографиями.",
};					

//Aim Member.c
//The page in which the players hires AIM mercenaries

STR16			CharacterInfo[] =
{
	// The various attributes of the merc

	L"Здоровье",										
	L"Проворность",										
	L"Ловкость",									
	L"Сила",
	L"Лидерство",
	L"Интеллект",
	L"Уровень опыта",
	L"Меткость",
	L"Механика",
	L"Взрывчатка",
	L"Медицина",				//10

	// the contract expenses' area

	L"Гонорар",							
	L"Срок",				
	L"1 день",					
	L"7 дней",
	L"14 дней",

	// text for the buttons that either go to the previous merc, 
	// start talking to the merc, or go to the next merc

	L"<<",
	L"Связаться",
	L">>",

	L"Дополнительная информация",				// Title for the additional info for the merc's bio
	L"Действующий состав",		//20		// Title of the page
	L"Снаряжение:",				// Displays the optional gear cost
	L"Стоимость Мед. депозита",			// If the merc required a medical deposit, this is displayed
};


//Aim Member.c
//The page in which the player's hires AIM mercenaries

//The following text is used with the video conference popup

STR16			VideoConfercingText[] =
{
	L"Сумма контракта:",				//Title beside the cost of hiring the merc

	//Text on the buttons to select the length of time the merc can be hired

	L"1 день",										
	L"7 дней",
	L"14 дней",

	//Text on the buttons to determine if you want the merc to come with the equipment 

	L"Без снаряжения",
	L"Со снаряжением",

	// Text on the Buttons

	L"ОПЛАТИТЬ",			// to actually hire the merc
	L"ОТМЕНА",				// go back to the previous menu
	L"НАНЯТЬ",				// go to menu in which you can hire the merc
	L"ОТБОЙ",				// stops talking with the merc
	L"ЗАКРЫТЬ",									
	L"СООБЩЕНИЕ",			// if the merc is not there, you can leave a message 

	//Text on the top of the video conference popup

	L"Видеоконференция с",		
	L"Подключение. . .",

	L"+ страховка"			// Displays if you are hiring the merc with the medical deposit
};



//Aim Member.c
//The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16			AimPopUpText[] =
{
	L"ПРОИЗВЕДЕН ЭЛЕКТРОННЫЙ ПЛАТЕЖ",	// You hired the merc
	L"НЕЛЬЗЯ ПЕРЕВЕСТИ СРЕДСТВА",		// Player doesn't have enough money, message 1
	L"НЕ ХВАТАЕТ СРЕДСТВ",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"На задании",											
	L"Пожалуйста, оставьте сообщение",
	L"Скончался",

	//If you try to hire more mercs than game can support

	L"У вас уже полная команда из наемников.",

	L"Автоответчик",
	L"Сообщение оставлено на автоответчике",
};


//AIM Link.c

STR16			AimLinkText[] =
{
	L"A.I.M. Ссылки",	//The title of the AIM links page
};



//Aim History

// This page displays the history of AIM

STR16			AimHistoryText[] =
{
	L"A.I.M. История",					//Title

	// Text on the buttons at the bottom of the page

	L"Назад",
	L"В начало", 
	L"A.I.M. Галерея",
	L"Дальше",
};


//Aim Mug Shot Index

//The page in which all the AIM members' portraits are displayed in the order selected by the AIM sort page.

STR16			AimFiText[] =
{
	// displays the way in which the mercs were sorted

	L"Цена",
	L"Опыт",
	L"Меткость",
	L"Медицина",
	L"Взрывчатка",
	L"Механика",

	// The title of the page, the above text gets added at the end of this text

	L"Сортировка состава A.I.M. По возрастанию, критерий - %s",
	L"Сортировка состава A.I.M. По убыванию, критерий - %s",

	// Instructions to the players on what to do

	L"Левый щелчок",
	L"Выбрать наемника",			//10
	L"Правый щелчок",
	L"Критерий сортировки",

	// Gets displayed on top of the merc's portrait if they are...

	L"Выбыл",
	L"Скончался",						//14
	L"На задании",
};



//AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are no longer with AIM

STR16			AimAlumniText[] =
{
	// Text of the buttons

	L"СТР. 1",
	L"СТР. 2",
	L"СТР. 3",

	L"A.I.M. Галерея",	// Title of the page

	L"ОК"			// Stops displaying information on selected merc
};






//AIM Home Page

STR16			AimScreenText[] =
{
	// AIM disclaimers
	
	L"A.I.M. и логотип A.I.M. - зарегистрированные во многих странах торговые марки.",
	L"Так что и не думай подражать нам.",
	L"(с) 1998-1999 A.I.M., Ltd. Все права защищены.",

	//Text for an advertisement that gets displayed on the AIM page

	L"\"Цветы по всему миру\"",
	L"\"Мы сбросим ваш букет где угодно!\"",				//10
	L"Сделай как надо",
	L"...в первый раз",
	L"Если у нас нет такого ствола, то он вам и не нужен.",
};


//Aim Home Page

STR16			AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages
	L"В начало",
	L"Наемники",
	L"Архив",
	L"Правила",
	L"Информация",
	L"Ссылки",
};



//ShopKeeper Interface
// The shopkeeper interface is displayed when the merc wants to interact with 
// the various store clerks scattered through out the game.

STR16 SKI_Text[ ] = 
{
	L"ИМЕЮЩИЕСЯ ТОВАРЫ",		//Header for the merchandise available
	L"СТР.",				//The current store inventory page being displayed
	L"ОБЩАЯ ЦЕНА",				//The total cost of the the items in the Dealer inventory area
	L"ОБЩАЯ ЦЕННОСТЬ",			//The total value of items player wishes to sell
	L"ОЦЕНКА",				//Button text for dealer to evaluate items the player wants to sell
	L"ПЕРЕВОД",			//Button text which completes the deal. Makes the transaction.
	L"УЙТИ",				//Text for the button which will leave the shopkeeper interface.
	L"ЦЕНА РЕМОНТА",			//The amount the dealer will charge to repair the merc's goods
	L"1 ЧАС",			// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d ЧАСОВ",		// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"ИСПРАВНО",		// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"Вам уже некуда класть вещи.",	//Message box that tells the user there is no more room to put there stuff
	L"%d МИНУТ",		// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"Выбросить предмет на землю.",
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
	L"OK",						//Transfer the money
	L"Взять",					//Take money from the player
	L"Дать",					//Give money to the player
	L"Отмена",					//Cancel the transfer
	L"Очистить",					//Clear the money display
};


//Shopkeeper Interface
STR16	gzSkiAtmText[] = 
{	

	// Text on the bank machine panel that....
	L"Select Type",				//tells the user to select either to give or take from the merc
	L"Введите сумму",			//Enter the amount to transfer
	L"Перевести деньги бойцу",		//Giving money to the merc
	L"Забрать деньги у бойца",		//Taking money from the merc
	L"Недостаточно средств",		//Not enough money to transfer
	L"Баланс",				//Display the amount of money the player currently has
};


STR16	SkiMessageBoxText[] =
{
	L"Желаете снять со счета %s, чтобы покрыть разницу?",
	L"Недостаточно средств. Не хватает %s",
	L"Желаете снять со счета %s, чтобы оплатить полную стоимость?",
	L"Попросить торговца сделать перевод",
	L"Попросить торговца починить выбранные предметы",
	L"Закончить беседу", //вспл. подсказка по нажатии кнопки кэнсэл при торговле\ремонте
	L"Текущий баланс",
};


//OptionScreen.c

STR16	zOptionsText[] = 
{
	//button Text
	L"Сохранить",
	L"Загрузить",
	L"Выход",
	L">>",
	L"<<",
	L"Готово",

	//Text above the slider bars
	L"Звуки",
	L"Речь",
	L"Музыка",

	//Confirmation pop when the user selects..
	L"Выйти из игры и вернуться в главное меню?",

	L"Необходимо выбрать или \"Речь\", или \"Субтитры\"",
};


//SaveLoadScreen 
STR16			zSaveLoadText[] = 
{
	L"Сохранить",
	L"Загрузить",
	L"Отмена",
	L"Сохранение выбрано",
	L"Загрузка выбрана",

	L"Игра успешно сохранена",
	L"ОШИБКА сохранения игры!",
	L"Игра успешно загружена",
	L"ОШИБКА загрузки игры!",

	L"Это сохранение было сделано иной версией игры. Скорее всего, загрузить его не удастся. Все равно продолжить?",
	L"Возможно файлы сохранений повреждены. Желаете их удалить?",

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Версия сохранений игры была изменена. Просим сообщить если это изменение привело к какой-либо ошибке. Пытаемся загрузить?",
#else
	L"Попытка загрузить сохранение игры устаревшей версии. Автоматически обновить его и загрузить?",
#endif

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Версия игры и файлов сохранений была изменена. Просим сообщить если это изменение привело к какой-либо ошибке. Пытаемся загрузить?",
#else
	L"Попытка загрузить сохранение игры устаревшей версии. Автоматически обновить его и загрузить?",
#endif

	L"Вы решили записать игру на существующее сохранение #%d?",
	L"Хотите загрузить игру из сохранения #",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"У вас заканчивается свободное место на жестком диске. Сейчас свободно %d Мб, а требуется %d Мб свободного места для JA.",	

	L"Сохраняю...",			//When saving a game, a message box with this string appears on the screen

	L"Нормальный",
	L"Огромный",
	L"Реалистичный",
	L"Фантастический",

	L"Стиль игры",
	L"Золотая серия",	//Placeholder English
	L"Ассортимент Бобби Рэя",
	L"Нормальный",
	L"Большой",
	L"Огромный",
	L"Всё и сразу",

	L"Сохраненная игра была начата в режиме 'реалистичного инвентаря', этот режим не работат при разрешении экрана 640х480. Измените разрешение и загрузите игру снова.",
	L"Загрузка игры, начатой в режиме 'реалистичного инвентаря', невозможна. Установите в Ja2.ini игровую папку 'Data-1.13' и повторите попытку.",
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"Уровень карты",
	L"У вас нет ополченцев. Чтобы они появились, вам нужно склонить на свою сторону горожан.",
	L"Доход в сутки",
	L"Наемник застрахован", 
	L"%s не нуждается в отдыхе.", 
	L"%s на марше и не может лечь спать.", 
	L"%s валится с ног от усталости, погоди немного.",
	L"%s ведет машину.",
	L"Отряд не может двигаться, когда один из наемников спит.",

	// stuff for contracts
	L"Хотя у вас и есть деньги на подписание контракта, но их не хватит, чтобы оплатить страховку наемника.",
	L"%s: продление страховки составит %s за %d дополнительных дней. Желаете заплатить?",
	L"Предметы в секторе",
	L"Жизнь наемника застрахована.",
	
	// other items
	L"Медики", // people acting a field medics and bandaging wounded mercs 
	L"Раненые", // people who are being bandaged by a medic 
	L"Готово", // Continue on with the game after autobandage is complete 
	L"Стоп", // Stop autobandaging of patients by medics now 
	L"Извините, этот пункт недоступен в демонстрационной версии.", // informs player this option/button has been disabled in the demo 
	L"%s: нет инструментов.",
	L"%s: нет аптечки.",
	L"Здесь недостаточно добровольцев для тренировки.",
	L"В %s максимальное количество ополченцев.",
	L"У наемника ограниченный контракт.", 
  L"Контракт наемника не застрахован",
	L"Карта Арулько",		// 24
};


STR16 pLandMarkInSectorString[] =
{
	L"Отряд %d заметил кого-то в секторе %s.",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] =
{
	L"Тренировка отряда ополченцев будет стоить $", // telling player how much it will cost
	L"Подтвердить платеж?", // asking player if they wish to pay the amount requested
	L"Вы не можете себе этого позволить.", // telling the player they can't afford to train this town
	L"Продолжить тренировку в %s (%s %d)?", // continue training this town?
	L"Цена $", // the cost in dollars to train militia
	L"( Д/Н )",   // abbreviated yes/no
	L"",	// unused
	L"Тренировка ополчения в секторе %d будет стоить $%d. %s", // cost to train sveral sectors at once
	L"У вас нет $%d, чтобы приступить к тренировке ополчения.",
	L"%s: Требуется не менее %d процентов лояльности, чтобы продолжить тренировку ополчения.",
	L"Больше вы не можете тренировать ополчение в %s.",
};

//Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom of the single merc panel
STR16	gzMoneyWithdrawMessageText[] = 
{
	L"За один раз вы можете снять со счета не более $20 000.",
	L"Вы решили положить %s на свой счет?",
};

STR16	gzCopyrightText[] = 
{
	L"Авторские права (C) 1999 Sir-Tech Canada Ltd. Все права защищены.",
};

//option Text
STR16		zOptionsToggleText[] = 
{
	L"Речь",
	L"Молчаливые герои",
	L"Субтитры",
	L"Пауза в диалогах",
	L"Анимированный дым",
	L"Кровь и жестокость", 
	L"Не трогать мышь!",
	L"Старый метод выбора", 
	L"Показать путь движения",
	L"Показать промахи",
	L"Игра в реальном времени",
	L"Подтверждение сна/подъема",
	L"Метрическая система",
	L"Движущаяся подсветка бойца",
	L"Курсор на бойцов",
	L"Курсор на дверь", 
	L"Мерцание вещей",
	L"Показать кроны деревьев",
	L"Показать каркасы",
	L"Трехмерный курсор",
	L"Показать шанс попадания",
	L"Курсор очереди для гранат",
	L"Враг оставляет всё снаряжене",
	L"Стрельба гранатой навесом",
	L"Классическое прицеливание",
	L"Выбор пробелом след. отряда",
	L"Тени предметов в инвентаре",
	L"Дальность оружия в тайлах",
	L"Одиночный трассер",
	L"Шум дождя",
	L"Вороны",
	L"Случайный I.M.P персонаж",
	L"Автосохранение каждый ход",
	L"Молчаливый пилот вертолёта",
	L"Низкая загрузка процессора",
	L"Подробное описание предметов", //Enhanced Description Box
	L"Только пошаговый режим",			// arynn : add Forced Turn Mode
	// arynn : reset all toggle options, in cases for corrupted JA2.set files (and general debugging of options)
	L"Сброс всех игровых настроек",	//Reset ALL game options
	L"В самом деле хотите этого?",	//Do you really want to reset?
	// arynn : a sample options screen options header, just text, not a real option                                                             
	L"--Настройки отладочной версии--",	//--DEBUG OPTIONS--
	// arynn : allow debug options that were set in debug.exe to continue in a rel.exe that shares same JA2.set file
	L"Retain Debug Options in Rel",
	// arynn : a sample option which affects options screen listing only                                            
	L"DEBUG Render Option group",
	// arynn : a sample DEBUG build option, this will draw slash-rects through current mouse region "hot spots"
	L"Render Mouse Regions",
	// arynn : a sample options screen options divider, just text, not a real option                                                             
	L"-----------------",

	L"Option044",	// arynn : note : everything south of here (should) only ever show in debug.. i doubt translating would prove useful
	L"Option045",
	L"Option046",
	L"Option047",
	L"Option048",
	L"Option049",
	L"Option050",
	L"Option051",
	L"Option052",
	L"Option053",
	L"Option054",
	L"Option055",
	L"Option056",
	L"Option057",
	L"Option058",
	L"Option059",
	L"Option060",
	L"Option061",
	L"Option062",
	L"Option063",
	L"Option064",
	L"Option065",
	L"Option066",
	L"Option067",
	L"Option068",
	L"Option069",
	L"Option070",
	L"Option071",
	L"Option072",
	L"Option073",
	L"Option074",
	L"Option075",
	L"Option076",
	L"Option077",
	L"Option078",
	L"Option079",
	L"Option080",
	L"Option081",
	L"Option082",
	L"Option083",
	L"Option084",
	L"Option085",
	L"Option086",
	L"Option087",
	L"Option088",
	L"Option089",
	L"Option090",
	L"Option091",
	L"Option092",
	L"Option093",
	L"Option094",
	L"Option095",
	L"Option096",
	L"Option097",
	L"Option098",
	L"Option099",
	L"Option100",

	// arynn : this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"Последняя_Строка_Настроек",	//THE_LAST_OPTION
};

//This is the help text associated with the above toggles.
STR16	zOptionsScreenHelpText[] =
{
	//speech
	L"Включить или выключить\nголос во время диалогов.",

	//Mute Confirmation
	L"Включить или выключить речевое\nподтверждение выполнения приказов.",

		//Subtitles
	L"Включить или выключить отображение\nсубтитров во время диалогов.",

	//Key to advance speech
	L"Если субтитры включены, выберите этот пункт,\nчтобы успеть прочитать диалоги персонажей.",

	//Toggle smoke animation
	L"Отключите анимацию дыма,\nесли он замедляет игру.",

	//Blood n Gore
	L"Отключите этот пункт, если боитесь крови.",

	//Never move my mouse
	L"Если выключено, то курсор автоматически перемещается\nна кнопку всплывающего окна диалога.", 

	//Old selection method
	L"Если включено, то будет использоваться старый метод выбора наемников\n(для тех, кто привык к управлению предыдущих частей Jagged Alliance).",

	//Show movement path
	L"Если включено, то в режиме реального времени будет отображаться путь передвижения\n(если выключено,  нажмите |S|H|I|F|T, чтобы увидеть путь).",

	//show misses
	L"Если включено, то камера будет отслеживать\nтраекторию пуль, прошедших мимо цели.",
	
	//Real Time Confirmation
	L"Если включено, то для приказа на передвижение будет требоваться\nдополнительный, подтверждающий щелчок мыши на месте назначения.",

	//Display the enemy indicator
	L"Если включено, то вы получите предупреждение,\nкогда наемники лягут спать или проснутся.",

	//Use the metric system
	L"Если включено, то используется метрическая система мер,\nиначе будет британская.",

	//Merc Lighted movement
	L"При ходьбе карта подсвечивается вокруг бойца (|G).\nОтключите эту настройку для повышения производительности системы.",

	//Smart cursor
	L"Если включено, то перемещение курсора возле наемника\nавтоматически выбирает его.",

	//snap cursor to the door
	L"Если включено, то перемещение курсора возле двери\nавтоматически помещает его на дверь.",

	//glow items 
	L"Если включено, то все предметы подсвечиваются. (|I)",

	//toggle tree tops
	L"Если включено, то отображаются кроны деревьев. (|T)",

	//toggle wireframe
	L"Если включено, то у препятствий\nдополнительно показывается каркас. (|W)",

	L"Если включено, то курсор передвижения\nотображается в 3D. (|Home )",

	// Options for 1.13
	L"Если включено, шанс попадания\nпоказывается над курсором.",
	L"Если включено, очередь из гранатомета\nиспользует курсор стрельбы очередями.",
	L"Если включено, из убитых врагов\nвыпадает все их снаряжение.",
	L"Если включено, гранатометы выстреливают заряд под большим углом к горизонту (|Q).",
	L"Если включено, из винтовок нельзя целится\nдольше 4 очков действия.",
	L"Если включено, |П|р|о|б|е|л выделяет следующий отряд автоматически.",
	L"Если включено, показываются тени предметов в инвентаре.",
	L"Если включено, дальность оружия показывается в тайлах.",
	L"Если включено, трассирующий эффект\nсоздается одиночным выстрелом.",	
	L"Если включено, вы услышите шум дождя во время непогоды.",
	L"Если включено, вороны присутствуют в игре.",
	L"Если включено, персонаж I.M.P будет получать\nслучайную внешность и характеристики.",
	L"Если включено, игра будет автоматически сохраняться\nпосле каждого хода игрока.",
	L"Если включено, Небесный Всадник\nне будет вас раздражать болтливостью.",
	L"Если включено, игра будет использовать\nменьше процессорного времени.",
	L"Если включено, будет задействовано\nподробное описание предметов.", //EDB description
	L"Если включено и в секторе присутствует враг, \nпошаговый режим будет задействован \nдо полной зачистки сектора (|C|T|R|L+|S|H|I|F|T+|A|L|T+|T).", //When ON and enemy present, Turn Base mode persists untill sector is free (|C|T|R|L+|S|H|I|F|T+|A|L|T+|T). // arynn : add forced turn mode
	// arynn : immediate effect option that fixes corrupt ja2.set keeping of toggle options
	L"Если включить, \nповреждённые игровые настройки будут восстановлены.",	//Click me to fix corrupt game settings
	L"Отметьте строку для подтверждения сброса игровых настроек.",	//Click me to fix corrupt game settings
	// arynn : a sample options screen options header, this pop up help text never render (well as of yet)
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER",
	// arynn : allow debug options that were set in debug.exe to continue in a rel.exe that shares same JA2.set file
	L"Allows debug options that were set in debug.exe to continue in a rel.exe that shares same JA2.set file",
	// arynn : a sample option which affects options screen listing only                                            
	L"Toggle to display debugging render options",
	// arynn : a sample DEBUG build option                                                                          
	L"Attempts to display slash-rects around mouse regions",
	// arynn : a sample options screen options divider, this pop up help text never render (well as of yet)
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END",
	// text past this point is for debugging, translating would doubtfully prove usefull
	L"044",
	L"045",
	L"046",
	L"047",
	L"048",
	L"049",
	L"050",
	L"051",
	L"052",
	L"053",
	L"054",
	L"055",
	L"056",
	L"057",
	L"058",
	L"059",
	L"060",
	L"061",
	L"062",
	L"063",
	L"064",
	L"065",
	L"066",
	L"067",
	L"068",
	L"069",
	L"070",
	L"071",
	L"072",
	L"073",
	L"074",
	L"075",
	L"076",
	L"077",
	L"078",
	L"079",
	L"080",
	L"081",
	L"082",
	L"083",
	L"084",
	L"085",
	L"086",
	L"087",
	L"088",
	L"089",
	L"090",
	L"091",
	L"092",
	L"093",
	L"094",
	L"095",
	L"096",
	L"097",
	L"098",
	L"099",
	L"100",

	// arynn : this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"Последняя строка в списке настроек.",	//TOPTION_LAST_OPTION
};


STR16	gzGIOScreenText[] =
{
	L"УСТАНОВКИ НАЧАЛА ИГРЫ",
	L"Стиль игры",
	L"Реалистичный",
	L"Фантастический",
	L"Золотая серия", //Placeholder English
	L"Ассортимент оружия в игре",
	L"Огромный",
	L"Нормальный",
	L"Уровень сложности",
	L"Лёгкий",
	L"Средний",
	L"Трудный",
	L"БЕЗУМИЕ",
	L"Начать игру",
	L"Главное меню",
	L"Дополнительная сложность",
	L"Сохранение в любое время",
	L"СТАЛЬНАЯ ВОЛЯ",
	L"Отключено в демо-версии", 
	L"Ассортимент Бобби Рэя",
	L"Нормальный",
	L"Большой",
	L"Огромный",
	L"Всё и сразу",
	L"Система инвентаря",
	L"Классическая",
	L"Реалистичная",
	L"Загрузить",
	L"УСТАНОВКИ ИГРЫ (будут заменены настройками сервера)",
};

STR16	gzMPJScreenText[] =
{
	L"СЕТЕВАЯ ИГРА",	//MULTIPLAYER
	L"Присоединиться",	//Join
	L"Создать игру",	//Host
	L"Отмена",	//Cancel
	L"Обновить",	//Refresh
	L"Имя игрока",	//Player Name
	L"IP сервера",	//Server IP
	L"Порт",	//Port
	L"Имя сервера",	//Server Name
	L"# Plrs",
	L"Версия",	//Version
	L"Game Type",
	L"Ping",
	L"Впишите имя игрока.",	//You must enter a player name
	L"Впишите корректный IP адрес. \n(пример 84.114.195.239).",	//You must enter a valid server IP address.\n (eg 192.168.0.1)
	L"Впишите корректный порт сервера (используйте диапазон от 1 до 65535).",	//You must enter a valid Server Port between 1 and 65535
};

STR16	gzMPHScreenText[] =
{
	L"СТАРТОВЫЕ УСТАНОВКИ СЕРВЕРА",			//HOST GAME
	L"Начать игру",					//Start
	L"Главное меню",				//Cancel
	L"Имя сервера",					//Server Name
	L"Тип игры",					//Game Type
	L"Deathmatch",
	L"Team Deathmatch",
	L"Co-operative",
	L"Кол-во игроков",				//Max Players
	L"Солдат в отряде",				//Squad Size
	L"Merc Selection",
	L"Случайные бойцы",				//Random Mercs
	L"Нанят игроком",				//Hired by Player
	L"Стартовый баланс",				//Starting Balance
	L"Можно нанимать тех же бойцов",		//Can Hire Same Merc
	L"Сообщения о нанятых бойцах",			//Report Hired Mercs
	L"Бобби Рэй доступен",				//Allow Bobby Rays
	L"Высадка со случайной стороны",		//Randomise Starting Edge
	L"Впишите имя сервера",				//You must enter a server name
	L"Количество игроков должно быть в диапазоне от 2 до 4",		//Max Players must be between 2 and 4
	L"Количество бойцов в отряде должно быть от 1-го до 5-и",		//Squad size must be between 1 and 6
	L"Время высадки",							//Time of Day
	L"Время высадки должно быть в 24-х часовом формате (ЧЧ:ММ).\n\n Помните, что 13:30 = 1.30pm",	//Time of Day must be a 24 hr time (HH:MM)\n\n eg. 13:30 = 1.30pm
	L"Стартовый баланс должен быть целым числом",				//Starting Cash must be a valid dollar amount ( no cents )\n\n eg. 150000
	L"Множитель урона",							//Damage Multiplier
	L"Множитель урона должен находиться в промежутке от 0 до 5.",		//Damage Multiplier must be a number between 0 and 5
	L"Множитель время хода",						//Turn Timer Multiplier
	L"Множитель время хода должен быть числом в промежутке от 1 до 200.",	//Turn Timer multiplier must be a number between 1 and 200
	L"Присутствуют гражданские в CO-OP",						//Enable Civilians in CO-OP
	L"Реалистичная система инвентаря (NIV)",				//Use New Inventory (NIV)
	L"Enforce Maximum AI Enemies",
	L"Sync. MP Clients Directory",
	L"MP Sync. Directory",
	L"You must enter a file transfer directory.",
	L"(Use '/' instead of '\\' for directory delimiters.)",
	L"The specified synchronisation directory does not exist.",
};

STR16 pDeliveryLocationStrings[] =
{
	L"Остин",			//Austin, Texas, USA	
	L"Багдад",			//Baghdad, Iraq (Suddam Hussein's home)
	L"Драссен",			//The main place in JA2 that you can receive items.  The other towns are dummy names...
	L"Гонконг",		//Hong Kong, Hong Kong
	L"Бейрут",			//Beirut, Lebanon	(Middle East)
	L"Лондон",			//London, England
	L"Лос-Анджелес",	//Los Angeles, California, USA (SW corner of USA)
	L"Медуна",			//Meduna -- the other airport in JA2 that you can receive items.
	L"Метавира",		//The island of Metavira was the fictional location used by JA1
	L"Майами",				//Miami, Florida, USA (SE corner of USA)
	L"Москва",			//Moscow, USSR
	L"Нью-Йорк",		//New York, New York, USA
	L"Оттава",			//Ottawa, Ontario, Canada -- where JA2 was made!
	L"Париж",				//Paris, France
	L"Триполи",			//Tripoli, Libya (eastern Mediterranean)
	L"Токио",				//Tokyo, Japan
	L"Ванкувер",		//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation.  It is possible to select 0 ability
	//in a skill meaning you can't use it.  This text is confirmation to the player.
	L"Вы уверены? Значение 0 означает отсутствие этого навыка вообще.",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"( до 8 символов )",
};

STR16 pIMPFinishButtonText[ 1 ]=
{
	L"Анализ",
};

STR16 pIMPFinishStrings[ ]=
{
	L"Спасибо, %s", //%s is the name of the merc
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] =
{
	L"Голос", 
};

STR16 pDepartedMercPortraitStrings[ ]=
{
	L"Погиб в бою",
	L"Уволен",
	L"Другое",
};

// title for program
STR16 pPersTitleText[] =
{
	L"Досье",
};

// paused game strings
STR16 pPausedGameText[] =
{
	L"Пауза в игре",
	L"Продолжить (|P|a|u|s|e)",
	L"Пауза (|P|a|u|s|e)",
};


STR16 pMessageStrings[] =
{
	L"Выйти из игры?",
	L"Да",
	L"ДА",
	L"НЕТ",
	L"ОТМЕНА",
	L"НАНЯТЬ",
	L"СОЛГАТЬ",
	L"Нет описания.", //Save slots that don't have a description.
	L"Игра сохранена.",
	L"Игра сохранена.",
	L"QuickSave", //The name of the quicksave file (filename, text reference)
	L"SaveGame",	//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",				//The 3 character dos extension (represents sav)
	L"..\\SavedGames", //The name of the directory where games are saved.
	L"День",
	L"Наемн",
	L"Свободное место", //An empty save game slot
	L"Демо",				//Demo of JA2
	L"Ловля Багов",				//State of development of a project (JA2) that is a debug build
	L"Релиз",			//Release build for JA2
	L"пвм",				//Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.
	L"мин",					//Abbreviation for minute.
	L"м",					//One character abbreviation for meter (metric distance measurement unit).
	L"пуль",				//Abbreviation for rounds (# of bullets)
	L"кг",					//Abbreviation for kilogram (metric weight measurement unit)
	L"фунт",				//Abbreviation for pounds (Imperial weight measurement unit)
	L"В начало",				//Home as in homepage on the internet.
	L"USD",					//Abbreviation to US dollars
	L"н/д",					//Lowercase acronym for not applicable.
	L"Посмотрим что происходит тем временем в другом месте",		//Meanwhile
	L"%s: прибыл в сектор %s%s",//30	Name/Squad has arrived in sector A9.  Order must not change without notifying
																		//SirTech
	L"Версия",
	L"Пустая ячейка быстрого сохр",
	L"Эта ячейка зарезервирована для Быстрого Сохранения, которое можно провести с тактической карты или с глобальной карты, нажав клавиши ALT+S.",
	L"Открытая",
	L"Закрытая",
	L"У вас заканчивается свободное дисковое пространство. На диске есть всего %sMб свободного места, а для Jagged Alliance 2 требуется %sMб.",
	L"Из A.I.M. нанят боец %s.", 
	L"%s ловит %s.",		//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s принимает препарат.", //'Merc name' has taken the drug
	L"%s: отсутствуют навыки в медицине.",//40	'Merc name' has no medical skill.

	//CDRom errors (such as ejecting CD while attempting to read the CD)
	L"Нарушена целостность программы.",
	L"ОШИБКА: CD-ROM открыт.",

	//When firing heavier weapons in close quarters, you may not have enough room to do so.
	L"Нет места, чтобы вести отсюда огонь.",
	
	//Can't change stance due to objects in the way...
	L"Сейчас нельзя изменить положение тела.",

	//Simple text indications that appear in the game, when the merc can do one of these things.
	L"Выкинуть",
	L"Бросить",
	L"Передать",

	L"%s передан %s.", //"Item" passed to "merc".  Please try to keep the item %s before the merc %s, otherwise,
											 //must notify SirTech.
	L"Не хватает места, чтобы передать %s %s.", //pass "item" to "merc".  Same instructions as above.

	//A list of attachments appear after the items.  Ex:  Kevlar vest ( Ceramic Plate 'Attached )'
	L" присоединён]",			// 50

	//Cheat modes
	L"Ну и зачем тебе это надо?",
	L"Активирован режим кодов.",

	//Toggling various stealth modes
	L"Отряд идет тихим шагом.",
	L"Отряд идет обычным шагом.",
	L"%s идет тихим шагом.",
	L"%s идет обычным шагом.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in 
	//an isometric engine.  You can toggle this mode freely in the game.
	L"Каркас зданий ВКЛ.",
	L"Каркас зданий ВЫКЛ.",

	//These are used in the cheat modes for changing levels in the game.  Going from a basement level to
	//an upper level, etc.  
	L"Нельзя подняться с этого уровня...",
	L"Нет нижних этажей...",
	L"Входим в подвал. Уровень %d...",
	L"Покидаем подвал...",

	L".",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"Режим следования ВЫКЛ.",
	L"Режим следования ВКЛ.",
	L"3D курсор ВЫКЛ.",
	L"3D курсор ВКЛ.",
	L"Выбран %d-й отряд.",
	L"Не хватает денег, чтобы заплатить %s ежедневный гонорар %s",	//first %s is the mercs name, the seconds is a string containing the salary
	L"Нет",
	L"%s не может уйти в одиночку.",
	L"Файл сохранения был записан под названием SaveGame99.sav. Если необходимо, переименуйте его в SaveGame01 - SaveGame10 и тогда, он станет доступен в экране сохранений.", 
	L"%s: выпил(а) немного %s.",
	L"Посылка прибыла в Драссен.",
	L"%s прибудет в точку назначения (сектор %s) в %dй день, примерно в %s.",		//first %s is mercs name, next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival
	L"В журнал добавлена запись!",
	L"Очереди из гранат используют курсор стрельбы очередями (стрельба по площадям возможна)",
	L"Очереди из гранат используют курсор метания (стрельба по площадям не возможна)",
	L"Выпадение всего снаряжения ВКЛ",
	L"Выпадение всего снаряжения ВЫКЛ",
	L"Гранатометы стреляют под обычным углом",
	L"Гранатометы стреляют навесом",
#ifdef JA2BETAVERSION
	L"Игра сохранена в ячейку авто-сохранения.",
#endif
	L"..\\SavedGames\\MP_SavedGames", //The name of the directory where games are saved.
	L"Client",
};


CHAR16 ItemPickupHelpPopup[][40] =
{
	L"Взять",
	L"Вверх",
	L"Выбрать все",
	L"Вниз",
	L"Отмена",
};

STR16 pDoctorWarningString[] =
{
	L"%s слишком далеко, чтобы подлечиться.",
	L"Ваши медики не могут оказать первую помощь всем раненым.",
};

STR16 pMilitiaButtonsHelpText[] =
{
	L"Уменьшить (правя кнопка)\nУвеличить (левая кнопка)\nчисло новобранцев", // button help text informing player they can pick up or drop militia with this button 
	L"Уменьшить (правая кнопка)\nУвеличить (левая кнопка)\nчисло рядовых",
	L"Уменьшить (правая кнопка)\nУвеличить (левая кнопка)\nчисло элитных солдат",
	L"Равномерно распределить\nополченцев по всем секторам.",
};

STR16 pMapScreenJustStartedHelpText[] =
{
	L"Отправляйтесь в A.I.M. и наймите бойцов (*Подсказка* - это в лэптопе).", // to inform the player to hired some mercs to get things going
	L"Когда будете готовы отправиться в Арулько, включите сжатие времени в правом нижнем углу экрана.", // to inform the player to hit time compression to get the game underway
};

STR16 pAntiHackerString[] = 
{
	L"Ошибка. Пропущен или испорчен файл(ы). Игра прекращает работу.",
};


STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"Просмотреть почту",
	L"Посетить Интернет сайты",
	L"Просмотреть полученные данные",
	L"Просмотреть журнал последних событий",
	L"Показать информацию о команде",
	L"Просмотреть финансовые отчеты",
	L"Закрыть лэптоп",

	//Bottom task bar icons (if they exist):
	L"Получена новая почта",
	L"Получены новые данные",

	//Bookmarks:
	L"Международная Ассоциация Наемников A.I.M.",
	L"Бобби Рэй - заказ оружия через Интернет",
	L"Институт Изучения Личности Наемника I.M.P.",
	L"Центр рекрутов M.E.R.C.",
	L"Похоронная служба Макгилликатти",
	L"'Цветы по всему миру'",
	L"Страховые агенты по контрактам A.I.M.",
};


STR16	gzHelpScreenText[] =
{
	L"Закрыть экран помощи",	
};

STR16 gzNonPersistantPBIText[] =
{
	L"Идет бой. Вы можете отступить только через тактический экран.",
	L"Войти в сектор, чтобы продолжить бой. (|E)",
	L"Провести бой автоматически (|A).",
	L"Во время атаки врага автоматическую битву включить нельзя.",
	L"После того как вы попали в засаду, автоматическую битву включить нельзя.",
	L"Рядом рептионы - автоматическую битву включить нельзя.",
	L"Рядом враждебные гражданские - автоматическую битву включить нельзя.",
	L"Рядом кошки-убийцы - автоматическую битву включить нельзя.",
	L"ИДЕТ БОЙ",
	L"Сейчас вы не можете отступить.",
};

STR16 gzMiscString[] =
{
	L"Ваши ополченцы продолжают бой без помощи наемников...",
	L"Сейчас машине топливо не требуется.",
	L"Топливный бак полон на %d%%.",
	L"%s полностью под контролем Дейдраны.",
	L"Вы потеряли заправочную станцию.",
};

STR16	gzIntroScreen[] = 
{
	L"Не удается найти вступительный видеоролик",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to 
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH."
STR16 pNewNoiseStr[] =
{
	L"%s слышит %s звук %s.",
	L"%s слышит %s звук движения %s.",
	L"%s слышит %s скрип, идущий %s.",
	L"%s слышит %s звук всплеска %s.",
	L"%s слышит %s звук удара %s.",
	L"%s слышит %s звук взрыва %s.",
	L"%s слышит %s крик %s.",
	L"%s слышит %s звук удара %s.",
	L"%s слышит %s звук удара %s.",
	L"%s слышит %s звон %s.",
	L"%s слышит %s грохот %s.",
};

STR16 wMapScreenSortButtonHelpText[] =
{
	L"Сортировка по имени (|F|1)",
	L"Сортировка по роду деятельности (|F|2)",
	L"Сортировка по состоянию сна (|F|3)",
	L"Сортировка по месту пребывания (|F|4)",
	L"Сортировка по месту назначения (|F|5)",
	L"Сортировка по времени контракта (|F|6)",
};



STR16		BrokenLinkText[] = 
{
	L"Ошибка 404",
	L"Сайт не найден.",
};


STR16 gzBobbyRShipmentText[] = 
{
	L"Последние поступления",
	L"Заказ №",
	L"Количество",
	L"Заказано",
};


STR16	gzCreditNames[]=
{
	L"Chris Camfield",
	L"Shaun Lyng",
	L"Kris Mдrnes",
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
	L"Ведущий программист игры", 				// Chris Camfield
	L"Дизайнер/Сценарист",					// Shaun Lyng
	L"Программист стратегической части и редактора",	//Kris \"The Cow Rape Man\" Marnes
	L"Продюсер/Дизайнер",					// Ian Currie
	L"Дизайнер/Дизайн карт",				// Linda Currie
	L"Художник",						// Eric \"WTF\" Cheng
	L"Тестирование, поддержка",				// Lynn Holowka
	L"Главный художник",					// Norman \"NRG\" Olsen
	L"Мастер по звуку",					// George Brooks
	L"Дизайнер экранов/художник",				// Andrew Stacey
	L"Ведущий художник/аниматор",				// Scot Loving
	L"Ведущий программист",					// Andrew \"Big Cheese Doddle\" Emmons
	L"Программист",						// Dave French
	L"Программист стратегии и баланса игры",		// Alex Meduna
	L"Художник-портретист",					// Joey \"Joeker\" Whelan",
};

STR16	gzCreditNameFunny[]=
{
	L"", 																			// Chris Camfield
	L"(Всё ещё зубрит правила пунктуации)",			// Shaun Lyng
	L"(\"Готово! Осталось только баги исправить.\")",	//Kris \"The Cow Rape Man\" Marnes
	L"(Уже слишком стар для всего этого)",			// Ian Currie
	L"(Также работает над Wizardry 8)",			// Linda Currie
	L"(Заставили тестировать под дулом пистолета)",		// Eric \"WTF\" Cheng
	L"(Ушла от нас в CFSA - скатертью дорожка...)",		// Lynn Holowka
	L"",							// Norman \"NRG\" Olsen
	L"",							// George Brooks
	L"(Поклонник джаза и группы Dead Head)",		// Andrew Stacey
	L"(Его настоящее имя Роберт)",				// Scot Loving
	L"(Единственный ответственный человек)",		// Andrew \"Big Cheese Doddle\" Emmons
	L"(Может опять заняться мотогонками)",			// Dave French
	L"(Украден с работы над Wizardry 8)",			// Alex Meduna
	L"(Делал предметы и загрузочные экраны!)",		// Joey \"Joeker\" Whelan",
};

STR16 sRepairsDoneString[] =
{
	L"%s: завершён ремонт личных вещей.",
	L"%s: завершён ремонт всего оружия и брони.",
	L"%s: завершён ремонт всей экипировки отряда.",
	L"%s: завершён ремонт всех крупных вещей отряда.",	//%s finished repairing everyone's large carried items
	L"%s: завершён ремонт всех малых вещей отряда.",	//%s finished repairing everyone's medium carried items
	L"%s: завершён ремонт всех мелких вещей отряда.",	//%s finished repairing everyone's small carried items
	L"%s: завершён ремонт разгрузочных систем отряда.",	//%s finished repairing everyone's LBE gear
};

STR16 zGioDifConfirmText[]=
{
	L"Вы выбрали ЛЕГКИЙ режим. Этот режим предназначен для тех, кто не знаком с Jagged Alliance и стратегическими играми вообще. Ваш выбор определит ход всей игры, так что будьте осторожны. Вы действительно хотите начать игру в этом режиме?",
	L"Вы выбрали СРЕДНИЙ режим. Этот режим предназначен для тех, кто знаком с Jagged Alliance или другими подобными играми. Ваш выбор определит ход всей игры, так что будьте осторожны. Вы действительно хотите начать игру в этом режиме?",
	L"Вы выбрали ТРУДНЫЙ режим. Предупреждаем: не сетуйте на нас, если вас быстро разгромят. Ваш выбор определит ход всей игры, так что будьте осторожны. Вы действительно хотите начать игру в этом режиме?",
	L"Вы выбрали режим БЕЗУМИЕ. Если игра уже пройдена на предыдущих трех уровнях сложности, то нужно подумать о смысле жизни, о происхождении вселенной и вообще... Но если вам все равно на что тратить время и нервы, то можете играть в этом режиме. Страшно?",
};

STR16 gzLateLocalizedString[] =
{
	L"%S файл для загрузки экрана не найден...",

	//1-5
	L"Робот не сможет покинуть этот сектор, пока кто-нибудь не возьмет пульт управления.",

	//This message comes up if you have pending bombs waiting to explode in tactical.
	L"Сейчас нельзя включить сжатие времени. Дождитесь взрыва!", 

	//'Name' refuses to move.
	L"%s отказывается подвинуться.",

	//%s a merc name
	L"%s: недостаточно очков действия для изменения положения.",

	//A message that pops up when a vehicle runs out of gas.
	L"%s: закончилось топливо. Машина осталась в %c%d.",

	//6-10

	// the following two strings are combined with the pNewNoise[] strings above to report noises
	// heard above or below the merc
	L"сверху",
	L"снизу",

	//The following strings are used in autoresolve for autobandaging related feedback.
	L"Никто из ваших наемников не имеет медицинских навыков.",
	L"Нечем бинтовать. Ни у кого из наемников нет аптечки.",
	L"Чтобы перевязать всех наемников, не хватило бинтов.",
	L"Никто из ваших наемников не нуждается в перевязке.",
	L"Автоматически перевязывать бойцов.",
	L"Все ваши наемники перевязаны.",

	//14
	L"Арулько",

  L"(на крыше)",

	L"Здоровье: %d/%d",

	//In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8"
	//"vs." is the abbreviation of versus.
	L"%d против %d",
	
	L"%s полон!",  //(ex "The ice cream truck is full")

  L"%s нуждается не в первой помощи или перевязке, а в серьезном лечении и/или отдыхе.", 

	//20
	//Happens when you get shot in the legs, and you fall down.
	L"Из-за ранения в ногу %s падает на землю!",
	//Name can't speak right now.
	L"%s сейчас не может говорить",

	//22-24 plural versions @@@2 elite to veteran
	L"%d новобранца из ополчения произведены в элитных солдат.",
	L"%d новобранца из ополчения произведены в рядовые.",
	L"%d рядовых ополченца произведены в элитных солдат.",

	//25
	L"Кнопка",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"У %s приступ безумия!",

	//27-28
	//Messages why a player can't time compress.
	L"Сейчас небезопасно включать сжатие времени - у вас есть наемники в секторе %s.",
	L"Сейчас небезопасно включать сжатие времени - у вас есть наемники в пещерах с жуками.",

	//29-31 singular versions @@@2 elite to veteran
	L"1 новобранец из ополчения стал элитным солдатом.",
	L"1 новобранец из ополчения стал рядовым ополченцем.",
	L"1 рядовой ополченец стал элитным солдатом.",

	//32-34
	L"%s ничего не говорит.",
	L"Выбраться на поверхность?",
	L"(%dй отряд)",

	//35
	//Ex: "Red has repaired Scope's MP5K".  Careful to maintain the proper order (Red before Scope, Scope before MP5K)
	L"%s отремонтировал(а) у %s %s",

	//36
	L"ГЕПАРД",

	//37-38 "Name trips and falls"
	L"%s спотыкается и падает.",
	L"Этот предмет отсюда взять невозможно.",

	//39
	L"Оставшиеся бойцы не могут сражаться. Сражение с тварями продолжит ополчение.",

	//40-43
	//%s is the name of merc.
	L"%s: закончились медикаменты!",
	L"%s: недостаточно навыков для лечения.",
	L"%s: закончился ремонтный набор!",
	L"%s: недостаточно навыков для ремонта.",

	//44-45
	L"Время ремонта",
	L"%s не видит этого человека.",

	//46-48
	L"%s: отвалилась ствольная насадка!",
	L"В одном секторе может быть не более %d тренеров ополчения.",
  L"Вы уверены?",

	//49-50
	L"Сжатие времени.",
	L"Бак машины полон.",

	//51-52 Fast help text in mapscreen.
	L"Возобновить сжатие времени (|П|р|о|б|е|л)",
	L"Прекратить сжатие времени (|E|s|c)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11" 
	L"%s починил(а) %s",
	L"%s починил(а) %s (%s)",

	//55 
	L"Нельзя включить сжатие времени при просмотре предметов в секторе.",

	L"CD Агония Власти не найден. Программа выходит в ОС.",

	L"Предметы успешно совмещены.",
	
	//58
	//Displayed with the version information when cheats are enabled.
	L"Прогресс игры текущий/максимально достигнутый: %d%%/%d%%",

	//59
	L"Сопроводить Джона и Мэри?",
	
	//60
	L"Кнопка нажата.",

	L"%s чувствует что в бронежилете что-то треснуло!",
	L"%s выпустил на %d больше пуль!",
	L"%s выпустил на %d пулю больше!",
};

STR16 gzCWStrings[] = 
{
	L"Нужна ли поддержка ополчения из соседних секторов?",
};

// WANNE: Tooltips
STR16 gzTooltipStrings[] =
{
	// Debug info
	L"%s|Место: %d\n",
	L"%s|Яркость: %d / %d\n",
	L"%s|Дистанция до |Цели: %d\n",
	L"%s|I|D: %d\n",
	L"%s|Приказы: %d\n",
	L"%s|Настрой: %d\n",
	L"%s|Текущие |A|Ps: %d\n",
	L"%s|Текущее |Здоровье: %d\n",
	// Full info
	L"%s|Каска: %s\n",
	L"%s|Жилет: %s\n",
	L"%s|Брюки: %s\n",
	// Limited, Basic
	L"|Броня: ",
	L"Каска ",
	L"Жилет ",
	L"Брюки",
	L"Одет",
	L"нет брони",
	L"%s|П|Н|В: %s\n",
	L"нет ПНВ",
	L"%s|Противогаз: %s\n",
	L"нет противогаза",
	L"%s|Голова,|Слот |1: %s\n",
	L"%s|Голова,|Слот |2: %s\n",
	L"\n(в рюкзаке) ",
	L"%s|Оружие: %s ",
	L"без оружия",
	L"Пистолет",
	L"Пистолет-пулемет",
	L"Винтовка",
	L"Ручной пулемет",
	L"Ружье",
	L"Нож",
	L"Тяжелое оружие",
	L"без каски",
	L"без бронежилета",
	L"без поножей",
	L"|Броня: %s\n", 
};

STR16 New113Message[] =
{
	L"Началась буря.",
	L"Буря закончилась.",
	L"Начался дождь.",
	L"Дождь закончился.",
	L"Опасайтесь снайперов...",
	L"Огонь на подавление!",	//suppression fire!
	L"*",				//BRST - всегда стабильна по количеству выпущеных пуль
	L"***",				//AUTO - регулируемая очередь (три звездочки - это потому что она можеть быть намного длиннее очереди с отсечкой)
	L"ГР",				//гранатомет
	L"ГР *",
	L"ГР ***",
	L"Снайпер!",
	L"Невозможно разделить деньги из-за предмета на курсоре.",
	L"Точка высадки новых наемников перенесена в %s, так как предыдущая точка высадки %s захвачена противником.",
	L"Выброшена вещь.",
	L"Выброшены все вещи выбранной группы.",
	L"Вещь продана голодающему населению Арулько.",
	L"Проданы все вещи выбранной группы.",
	L"Проверь что солдату мешает лучше видеть.",	//You should check your goggles
};

STR16 New113HAMMessage[] = 
{
	L"%s в страхе пытается укрыться!",		//%s cowers in fear!
};

// WANNE: This are the email texts, when one of the 4 new 1.13 MERC mercs have levelled up, that Speck sends
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16	New113MERCMercMailTexts[] =
{
	// Gaston: Text from Line 39 in Email.edt
	L"Пожалуйста, примите к сведению, что с настоящего момента гонорар Гастона увеличивается вследствие повышения его профессионального уровня. ± ± Спек Т. Кляйн ± ",
	// Stogie: Text from Line 43 in Email.edt
	L"Пожалуйста, примите к сведению, что повышение боевых навыков лейтенанта Хорга 'Сигары' влечет за собой повышение его гонорара. ± ± Спек Т. Кляйн ± ", 
	// Tex: Text from Line 45 in Email.edt
	L"Прошу принять к сведению, что заслуги Текса позволяют ему требовать более достойной оплаты. Поэтому его гонорар был увеличен, чтобы соответствовать его умениям. ± ± Спек Т. Кляйн ± ",
	// Biggens: Text from Line 49 in Email.edt
	L"Ставим в известность, что отличная работа полковника Фредерика Биггенса заслуживает поощрения в виде повышения гонорара. Постановление считать действительным с текущего момента. ± ± Спек Т. Кляйн ± ",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper
	L"Снайпер: У вас глаза ястреба. В свободное время вы развлекаетесь отстреливая крылышки у мух с расстояния 100 метров! ± ",
	// Camouflage
	L"Маскировка: На вашем фоне кусты выглядят синтетическими! ± ",	
};

STR16 NewInvMessage[] = 
{
	L"В данный момент поднять рюкзак нельзя.",
	L"Вы не можете одновременно носить 2 рюкзака.",
	L"Вы потеряли свой рюкзак...",
	L"Замок рюкзака работает лишь во время битвы.",
	L"Вы не можете передвигаться с открытым рюкзаком.",
	L"Вы уверены что хотите продать весь хлам этого сектора голодающему населению Арулько?",
	L"Вы уверены что хотите выбросить весь хлам, валяющийся в этом секторе?",
	L"Тяжеловато будет взбираться с полным рюкзаком на крышу. Может снимем?",
};

// WANNE - MP: Multiplayer messages
STR16 MPServerMessage[] =
{
	// 0
	L"Запускается сервер RakNet...",
	L"Сервер запущен, ожидание подключений...",
	L"Теперь вам надо подключиться к серверу, нажав '2'.",
	L"Сервер уже запущен.",
	L"Не удалось запустить сервер. Прекращаю работу.",
	// 5
	L"%d/%d клиентов готовы к режиму реального времени.",
	L"Сервер отключился и прекратил свою работу.",
	L"Сервер не запущен.",
	L"Подождите пожалуйста, игроки все еще загружаются...",
	L"Вы не можете изменять зону высадки после запуска сервера.",
	// 10
	L"Sent file '%S' - 100/100",
	L"Finished sending files to '%S'.",
	L"Started sending files to '%S'.",
};

STR16 MPClientMessage[] =
{
	// 0
	L"Запускается клиент RakNet...",		
	L"Подключение к IP: %S ...",
	L"Получены настройки игры:",
	L"Вы уже подключены.",
	L"Вы уже подключаетесь...",
	// 5
	L"Клиент №%d - '%S' нанял %s.",
	L"Клиент №%d - '%S' нанял еще бойца.",
	L"Вы готовы к бою (всего готово = %d/%d).",
	L"Вы отменили готовность к бою (всего готово = %d/%d).",
	L"Отряды подтягиваются к месту битвы...",	//'Starting battle...'
	// 10
	L"Клиент №%d - '%S' готов к бою (всего готово = %d/%d).",
	L"Клиент №%d - '%S' отменил готовность к бою (всего готово = %d/%d).",
	L"Похоже, вы уже готовы к бою, однако, придется подождать остальных. (Если хотите изменить расположение своих бойцов, нажмите кнопку 'ДА').",
	L"Начнем же битву!",
	L"Для начала игры необходимо запустить клиент.",
	// 15
	L"Игра не может быть начата, вы не наняли ни одного бойца.",
	L"Ждем, когда сервер даст добро на доступ к лэптопу...",
	L"Перехвачен",			//Interrupted
	L"Finish from interrupt",	//Finish from interrupt
	L"Координаты курсора:",		//Mouse Grid Coordinates
	// 20
	L"X: %d, Y: %d",
	L"Номер квадрата: %d",		//Grid Number
	L"Доступно лишь для сервера.",
	L"Выберите какую ступень игры принудительно запустить:  ('1' - открыть лэптоп/найм бойцов)  ('2' - запустить/загрузить уровень)  ('3' - разблокировать пользовательский интерфейс)  ('4' - завершить расстановку)",
	//L"Sector=%s, Max Clients=%d, Max Mercs=%d, Game_Mode=%d, Same Merc=%d, Damage Multiplier=%f, Enemies=%d, Creatures=%d, Militias=%d, Civilians=%d, Timed Turns=%d, Secs/Tic=%d, Starting Cash=$%d, Tons of Guns=%d, Sci-Fi=%d, Difficulty=%d, Iron-Man=%d, BobbyRays Range=%d, Dis BobbyRay=%d, Dis Aim/Merc Equip=%d, Dis Morale=%d, Testing=%d",
	L"Sector=%s, Max Clients=%d, Max Mercs=%d, Game_Mode=%d, Same Merc=%d, Damage Multiplier=%f, Timed Turns=%d, Secs/Tic=%d, Dis BobbyRay=%d, Dis Aim/Merc Equip=%d, Dis Morale=%d, Testing=%d",

	// 25
	L"Тестовая чит-функция '9' включена.",
	L"Новый игрок: клиент №%d - '%S'.",
	L"Команда: %d.",//not used any more
	L"%s (клиент %d - '%S') был убит %s (клиент %d - '%S')",
	L"Клиент №%d - '%S' выкинут из игры.",
	// 30
	L"Принудительно дать ход клиенту: №1 - '%S' | №2 - '%S' | №3 - '%S' | №4 - '%S'",
	L"Начался ход клиента №%d",
	L"Запрос перехода в режим реального время...",
	L"Переход в режим реального времени.",
	L"Ошибка: что-то пошло не так, возвращаю обратно.",
	// 35
	L"Открыть доступ к лэптопу? (Уверены что все игроки подключились?)",
	L"Сервером был открыт доступ к лэптопу. Приступайте к найму бойцов!",
	L"Перехватчик.",
	L"Клиент не может изменять зону высадки, доступно лишь серверу.",
	L"Вы отказались от предложения сдаться, потому что это не актуально в сетевой игре.",
	// 40
	L"Все ваши бойцы были убиты!",
	L"Активизирован режим наблюдения.",
	L"Вы потерпели поражение!",
	L"Извините, залезать на крышу в сетевой игре запрещено.",
	L"Вы наняли %s.",
	// 45
	L"You cant change the map once purchasing has commenced",
	L"Map changed to '%s'",
	L"Client '%s' disconnected, removing from game",
	L"You were disconnected from the game, returning to the Main Menu",
	L"Connection failed, Retrying in 5 seconds, %i retries left...",
	//50
	L"Connection failed, giving up...",
	L"You cannot start the game until another player has connected",
	L"%s : %s",
	L"Send to All",
	L"Allies only",
	// 55
	L"Cannot join game. This game has already started.",
	L"%s (team): %s",
	L"Client #%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"Received all files from server.",
	L"'%S' finished downloading from server.",
	L"'%S' started downloading from server.",
	L"Cannot start the game until all clients have finished receiving files",
	L"This server requires that you download modified files to play, do you wish to continue?",
	// 65
	L"Press 'Ready' to enter tactical screen.",
	L"Cannot connect because your version %S is different from the server version %S.",
};

STR16 MPHelp[] =
{
	// 0
	L"Запущена сетевая игра Jagged Alliance 2",
	L"Нажмите F1 для просмотра помощи.",
	L"Управление сетевой игрой (на стратегическом экране)",
	L"* Сперва установите настройки в файле ja2_mp.ini *",
	L"'1' - Запустить сервер.", 
	// 5
	L"'2' - Подключиться к серверу.",
	L"'3' - Для сервера - открыть лэптоп, для клиента - готов к бою.",
	L"'4' - Отключение сервера или клиента.",
	L"'5' - Показ координат под мышью на тактическом экране.",
	L"'7' - Вызов меню принудительных изменений серверных установок.",
	// 10
	L"'F2' - Показать дополнительную помощь.",
	L"Более подробное описание вы найдете в файле readme_mp.html",
	L"Советы: (считаем, что настройки в ja2_mp.ini верны)",
	L"",	//unuse '* Make sure all clients have unique CLIENT_NUM *'
	L"* В сохраненной игре не записывается заказанное у Бобби Рэя *",
	// 15
	L"* Старайтесь не ставить бойцов ненавидящих друг-друга в прямой видимости *",
	L"'F1' - Показать основную помощь.",
};

STR16 gszMPEdgesText[] =
{
	L"С",	//N
	L"Ю",	//S
	L"В",	//E
	L"З"	//W
};

STR16 gszMPTeamNames[] =
{
	L"Фокстрот",	//Foxtrot
	L"Браво",	//Bravo
	L"Дельта",	//Delta
	L"Чарли"	//Charlie
};

STR16 gszMPMapscreenText[] =
{
	L"Тип игры: ",		//Game Type: 
	L"Игроков: ",		//Players: 
	L"Всего бойцов: ",	//Mercs each: 
	L"Нельзя изменять сторону высадки отряда после открытия лэптопа.",	//You cannot change starting edge once Laptop is unlocked.
	L"Нельзя изменить имя команды после открытия лэптопа.",	//You cannot change teams once the Laptop is unlocked.
	L"Случ. бойцы: ",	//Random Mercs: 
	L"Да",			//Y
	L"Сложность:",		//Difficulty:
	L"Server Version:",
};

STR16 gzMPSScreenText[] =
{
	L"Доска счёта",		//Scoreboard
	L"Продолжить",		//Continue
	L"Отмена",		//Cancel
	L"Игрок",		//Player
	L"Убито",		//Kills
	L"Погибло",		//Deaths
	L"Королевская армия",	//Queen's Army
	L"Выстрелов",		//Hits
	L"Промахи",		//Misses
	L"Меткость",		//Accuracy
	L"Нанесённый урон",	//Damage Dealt
	L"Полученный урон",	//Damage Taken
};

STR16 gzMPCScreenText[] =
{
	L"Отмена",						//Cancel
	L"Подключаюсь к серверу...",				//Connecting to Server
	L"Получаю настройки от сервера...",			//Getting Server Settings
	L"Скачиваю выбранные файлы...",				//Downloading custom files
	L"Нажмите 'ESC' для отмены или 'Y' чтобы войти в чат.",	//Press 'ESC' to cancel or 'Y' to chat
	L"Нажмите 'ESC' для отмены",				//Press 'ESC' to cancel
	L"Выполнено."						//Ready
};

STR16 gzMPChatToggleText[] =
{
	L"Отправть всем",		//Send to All
	L"Отправть только союзникам",	//Send to Allies only
};

STR16 gzMPChatboxText[] =
{
	L"Чат сетевой игры Jagged Alliance 2 v1.13",	//Multiplayer Chat
	L"Заметка: нажмите |В|В|О|Д для отправки сообщения, |К|Л|Ю|Ч для выхода из чата.",	//Chat: press 'ENTER' to send of 'ESC' to cancel
};

#endif //RUSSIAN
