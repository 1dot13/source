// WANNE: Yes, this should be disabled, otherwise we get weird behavior when running the game with a VS 2005 build!
//#pragma setlocale("RUSSIAN")

#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#if defined( RUSSIAN )
		#include "text.h"
		#include "Fileman.h"
		#include "Scheduling.h"
		#include "EditorMercs.h"
		#include "Item Statistics.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_RussianText_public_symbol(void){;}

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

CHAR16	XMLTacticalMessages[1000][MAX_MESSAGE_NAMES_CHARS] = 
{
	L"",
};

//Encyclopedia

STR16 pMenuStrings[] =
{
	//Encyclopedia
	L"Места", // 0
	L"Команда",
	L"Предметы",
	L"Задания",
	L"Меню 5",
	L"Меню 6", //5
	L"Меню 7", 
	L"Меню 8",
	L"Меню 9",
	L"Меню 10",
	L"Меню 11", //10
	L"Меню 12",
	L"Меню 13",
	L"Меню 14",
	L"Меню 15",
	L"Меню 15", // 15
	
	//Briefing Room
	L"Войти",
};

STR16	pOtherButtonsText[] =
{
	L"Инструктаж", 
	L"Принять",
};

STR16	pOtherButtonsHelpText[] =
{
	L"Инструктаж",
	L"Принять задания",
};


STR16	pLocationPageText[] =
{
	L"Пред.",
	L"Фото", 
	L"След.",
};

STR16	pSectorPageText[] =
{
	L"<<",
	L"В начало",
	L">>",
	L"Тип: ",
	L"Нет данных",
	L"Нет поставленных заданий. Добавьте задания в файл TableData\\BriefingRoom\\BriefingRoom.xml. Первое задание должно быть видимым. Чтобы скрыть задание, установите значение = 0.",
	L"Брифинг-зал. Пожалуйста, нажмите кнопку 'Войти'.",
};

STR16	pEncyclopediaTypeText[] = 
{
	L"Неизвестно",// 0 - unknown
	L"Город", //1 - city
	L"База ПВО", //2 - SAM Site
	L"Другое", //3 - other location
	L"Шахты", //4 - mines 
	L"Военный комплекс", //5 - military complex 
	L"Лаборатория",  //6 - laboratory complex 
	L"Фабрика", //7 - factory complex 
	L"Госпиталь", //8 - hospital 
	L"Тюрьма", //9 - prison
	L"Аэропорт", //10 - air port 
};

STR16	pEncyclopediaHelpCharacterText[] = 
{
	L"Показ. всех",
	L"Показ. AIM",
	L"Показ. MERC",
	L"Показ. RPC",
	L"Показ. NPC",
	L"Показ. транс.",
	L"Показ. IMP",
	L"Показ. EPC",
	L"Фильтр",
};

STR16	pEncyclopediaShortCharacterText[] = 
{
	L"Все",
	L"AIM",
	L"MERC",
	L"RPC",
	L"NPC",
	L"Транс.",
	L"IMP",
	L"EPC",
	L"Фильтр",
};

STR16	pEncyclopediaHelpText[] = 
{
	L"Показать всё",
	L"Показать города",
	L"Показать базы ПВО",
	L"Показать другие места",
	L"Показать шахты",
	L"Показать военный комплекс",
	L"Показать лабораторию",
	L"Показать фабрику",
	L"Показать госпиталь",
	L"Показать тюрьму",
	L"Показать аэропорт",
};

STR16	pEncyclopediaSkrotyText[] = 
{
	L"Всё",
	L"Город",
	L"ПВО",
	L"Друг.",
	L"Шахт.",
	L"Воен.",
	L"Лаб.",
	L"Фабр.",
	L"Госп.",
	L"Тюрьм.",
	L"Аэроп.",
};

STR16	pEncyclopediaFilterLocationText[] =
{//major location filter button text max 7 chars
//..L"------v"
	L"Всё",//0
	L"Города",
	L"ПВО",
	L"Шахты",
	L"Аэроп.",
	L"Дик.мес",
	L"Подзем.",
	L"Завед.",
	L"Другое",
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Показать всё",//facility index + 1
	L"Показать города",
	L"Показать базы ПВО",
	L"Показать шахты",
	L"Показать аэропорты",
	L"Показать дикую местность",
	L"Показать подземные секторы",
	L"Показать секторы с заведениями\n[|Л|К|М] переключить фильтр\n[|П|К|М] сбросить фильтр",
	L"Показать другие секторы",
};

STR16 pEncyclopediaSubFilterLocationText[] =
{//item subfilter button text max 7 chars
//..L"------v"
	L"",//reserved. Insert new city filters above!
	L"",//reserved. Insert new SAM filters above!
	L"",//reserved. Insert new mine filters above!
	L"",//reserved. Insert new airport filters above!
	L"",//reserved. Insert new wilderness filters above!
	L"",//reserved. Insert new underground sector filters above!
	L"",//reserved. facility filter texts are dynamicly loaded, leave this marker empty!
	L"",//reserved. Insert new other filters above!
};

STR16	pEncyclopediaFilterCharText[] =
{//major char filter button text
//..L"------v"
	L"Все",//0
	L"A.I.M.",
	L"MERC",
	L"RPC",
	L"NPC",
	L"IMP",
	L"Другое",//add new filter buttons before other
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Показать всех",//Other index + 1
	L"Показать наёмников из A.I.M.",
	L"Показать наёмников из M.E.R.C",
	L"Показать повстанцев",
	L"Показать неигровых персонажей",
	L"Показать персонажей игрока",
	L"Показать другое\n[|Л|К|М] переключить фильтр\n[|П|К|М] сбросить фильтр",
};

STR16	pEncyclopediaSubFilterCharText[] =
{//item subfilter button text
//..L"------v"
	L"",//reserved. Insert new AIM filters above!
	L"",//reserved. Insert new MERC filters above!
	L"",//reserved. Insert new RPC filters above!
	L"",//reserved. Insert new NPC filters above!
	L"",//reserved. Insert new IMP filters above!
//Other-----v"
	L"Транс.",
	L"EPC",
	L"",//reserved. Insert new Other filters above!
};

STR16	pEncyclopediaFilterItemText[] = 
{//major item filter button text max 7 chars
//..L"------v"
	L"Всё",//0
	L"Оружие",
	L"Патроны",
	L"Броня",
	L"Разгр.",
	L"Навеска",
	L"Разное",//add new filter buttons before misc
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Показать всё",//misc index + 1
	L"Показать оружие\n[|Л|К|М] переключить фильтр\n[|П|К|М] сбросить фильтр",
	L"Показать боеприпасы\n[|Л|К|М] переключить фильтр\n[|П|К|М] сбросить фильтр",
	L"Показать броню\n[|Л|К|М] переключить фильтр\n[|П|К|М] сбросить фильтр",
	L"Показать разрузочные комплекты\n[|Л|К|М] переключить фильтр\n[|П|К|М] сбросить фильтр",
	L"Показать навеску\n[|Л|К|М] переключить фильтр\n[|П|К|М] сбросить фильтр",
	L"Показать разное\n[|Л|К|М] переключить фильтр\n[|П|К|М] сбросить фильтр",
};

STR16 pEncyclopediaSubFilterItemText[] =
{//item subfilter button text max 7 chars
//..L"------v"
//Guns......v"
	L"Пистол.",
	L"А.пист.",
	L"ПП",
	L"Винтов.",
	L"Сн.винт",
	L"Автомат",
	L"Пулем.",
	L"Дробов.",
	L"Тяжел.",
	L"",//reserved. insert new gun filters above!
//Amunition.v"
	L"Пистол.",
	L"А.пист.",
	L"ПП",
	L"Винтов.",
	L"Сн.винт",
	L"Автомат",
	L"Пулем.",
	L"Дробов.",
	L"Тяжел.",
	L"",//reserved. insert new ammo filters above!
//Armor.....v"
	L"Шлем",
	L"Жилет",
	L"Штаны",
	L"Пласт.",
	L"",//reserved. insert new armor filters above!
//LBE.......v"
	L"Кобуры",
	L"Жилеты",
	L"Ранцы",
	L"Рюкзаки",
	L"Карманы",
	L"Другое",
	L"",//reserved. insert new LBE filters above!
//Attachments"
	L"Оптика",
	L"Доп.",
	L"Дуло",
	L"Съёмн.",
	L"Встроен.",
	L"Другое",
	L"",//reserved. insert new attachment filters above!
//Misc......v"
	L"Ножи",
	L"Мет.нож",
	L"Удар.",
	L"Гранаты",
	L"Бомбы",
	L"Мед.наб",
	L"Наборы",
	L"Лицо",
	L"Другое",
	L"",//reserved. insert new misc filters above!
//add filters for a new button here
};

STR16 pEncyclopediaFilterQuestText[] =
{//major quest filter button text max 7 chars
//..L"------v"
	L"Все",
	L"Активн.",
	L"Заверш.",
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Показать все",//misc index + 1
	L"Показать активные квесты",
	L"Показать завершённые квесты",
};

STR16 pEncyclopediaSubFilterQuestText[] =
{//Quest subfilter button text max 7 chars, not used, but needed if any subfilters are added
//..L"------v"
	L"",//reserved. insert new active quest subfilters above!
	L"",//reserved. insert new completed quest subfilters above!
};

STR16	pEncyclopediaShortInventoryText[] = 
{
	L"Всё", //0
	L"Оружие",
	L"Патроны",
	L"Разгр.",
	L"Другое",
	
	L"Все", //5
	L"Оружие",
	L"Патроны",
	L"Разгр.",
	L"Другое",
};

STR16			BoxFilter[] =
{
	// Guns
	L"Тяжел.",
	L"Пистол.",
	L"А.пист.",
	L"ПП",
	L"Винтов.",
	L"С.винт.",
	L"Ш.винт.",
	L"Пулем.",
	L"Дробов.",

	// Ammo
	L"Пистол.",
	L"А.пист.", //10
	L"ПП",
	L"Винтов.",
	L"С.винт.",
	L"Ш.винт.",
	L"Пулем.",
	L"Дробов.",

	// Used
	L"Оружие",
	L"Броня",
	L"Разгр.",
	L"Другое", //20

	// Armour
	L"Шлемы",
	L"Жилеты",
	L"Штаны",
	L"Пласт.",

	// Misc
	L"Ножи",
	L"Мет.нож",
	L"Бл.бой",
	L"Гранаты",
	L"Бомбы",
	L"Мед.", //30
	L"Наборы",
	L"Лицо",
	L"Разгр.",
	L"Разное", //34
};

STR16		QuestDescText[] =
{
	L"Доставка письма",
	L"Поставка еды",
	L"Террористы",
	L"Кубок Босса",
	L"Деньги Босса",
	L"Беглец Джои",
	L"Спасение Марии",
	L"Кубок Читзены",
	L"Пленники в Альме",
	L"Допрос",

	L"Проблемные фермеры",								//10
	L"Найти учёного",
	L"Принести видеокамеру",
	L"Кошки-убийцы",
	L"Найти отшельника",
	L"Твари в шахте",
	L"Найти пилота вертолёта",
	L"Сопроводить Всадника",
	L"Освободить Динамо",
	L"Сопроводить туристов",


	L"Проверить Дорин",									//20
	L"Мечта о магазине",
	L"Сопроводить Шенка",
	L"No 23 Yet",
	L"No 24 Yet",
	L"Убить Дейдрану",
	L"No 26 Yet",
	L"No 27 Yet",
	L"No 28 Yet",
	L"No 29 Yet",
};

STR16		FactDescText[] =
{
	L"Омерта освобождена",
	L"Драссен освобождён",
	L"Сан-Мона освобождена",
	L"Камбрия освобождена",
	L"Альма освобождена",
	L"Грам освобождён",
	L"Тикса освобождена",
	L"Читзена освобождена",
	L"Эстони освобождён",
	L"Балайм освобождён",

	L"Орта освобождена",					//10
	L"Медуна освобождена",
	L"Поговорили с Пакосом",
	L"Фатима прочла письмо",
	L"Фатима сбежала от игрока",
	L"Димитрий (#60) мёртв",
	L"Фатима ответила на удивление Димитрия",
	L"Карлос крикнул 'никому не двигаться'",
	L"Фатима рассказала о письме",
	L"Фатима пришла в место назначения",

	L"Димитрий сказал, что у Фатимы есть доказательства",		//20
	L"Мигель выслушал доводы",
	L"Мигель попросил письмо",
	L"Мигель прочёл письмо",
	L"Айра прокоментировала письмо Мигелю",
	L"Повстанцы наши враги",
	L"Разговор Фатимы до передачи письма",
	L"Получено задание с Драссеном",
	L"Мигель предложил Айру",
	L"Пакос ранен или убит",

	L"Пакос в A10",		//30
	L"В секотре безопасно",
	L"Посылка от БР в пути",
	L"Посылка от БР в Драссене",
	L"33 - ВЕРНО и прибыло в течение 2 часов",
	L"33 - ВЕРНО 34 - ЛОЖЬ, более чем 2 часа",
	L"Игрок заметил что часть груза пропала",
	L"36 - ВЕРНО и Пабло был избит игроком",
	L"Пабло проворовался",
	L"Пабло вернул украденное, set 37 false",

	L"Мигель присоединится к команде",		//40
	L"Дали Пабло немного денег",
	L"Небесного всадника сопровождают в город",
	L"Небесный Всадник уже близок к своему вертолёту в Драссене",
	L"Небесный всадник оговорил условия контракта",
	L"Игрок выбрал вертолёт на стратегическом экране как минимум один раз",
	L"Неигровому персонажу должны денег",
	L"Неигровой персонаж ранен",
	L"Неигровой персонаж ранен игроком",
	L"Отцу Джону Уолкеру сказали о нехватке продовольствия",

	L"Айра не в секторе",		//50
	L"Айра ведёт беседу",
	L"Задание с продовольствием выполнено",
	L"Пабло что-то украл с последней посылки",
	L"Последнее отправление повреждено",
	L"Последнее отправление отправлено не туда",
	L"Прошло 24 часа с момента сообщения, что отправление отправлено не в тот аэропорт",
	L"Потерянная посылка пришла с повреждённым грузом. 56 to False",
	L"Потерянная посылка пропала бесследно. Turn 56 False",
	L"Следующее отправление может быть (random) потеряно",

	L"Следующее отправление может быть (random) задержано",		//60
	L"Отправление среднего размера",
	L"Отправление большого размера",
	L"У Дорин проснулась совесть",
	L"Игрок поговорил с Гордоном",
	L"Айра до сих пор неигровой персонаж и находится в A10-2(не присоединилась)",
	L"Динамо просит оказать ему первую помощь",
	L"Динамо можно нанять",
	L"Неигровой персонаж истекает кровью",
	L"Шенк хочет присоединиться",

	L"Неигровой персонаж истекает кровью",		//70
	L"У игрока есть голова и Кармен в Сан-Мона",
	L"У игрока есть голова и Кармен в Камбрии",
	L"У игрока есть голова и Кармен в Драссене",
	L"Отец пьян",
	L"Раненые бойцы игрока находятся ближе 8 тайлов от неигрового персонажа",
	L"1 и только 1 раненый боец ближе 8 тайлов от неигрового персонажа",
	L"Больше одного раненного бойца ближе 8 тайлов от неигрового персонажа",
	L"Бренда в магазине ",
	L"Бренда мертва",

	L"Бренда у себя дома",		//80
	L"Неигровой персонаж - враг",
	L"Уровень разговора >= 84 и < 3 мужчин присутствует",
	L"Уровень разговора >= 84 и хотя бы 3 мужчины присутствует",
	L"Ганс позволил нам встретиться с Тони",
	L"Ганс стоит на позиции 13523",
	L"Сегодня Тони нет",
	L"Женщина разговаривает с неигровым персонажем",
	L"Игрок развлекался в борделе",
	L"Карла доступна",

	L"Синди доступна",		//90
	L"Бэмби доступна",
	L"Свободных девочек нет",
	L"Игрок ожидал девочек",
	L"Игрок заплатил правильную сумму денег",
	L"Mercs walked by goon",
	L"More thean 1 merc present within 3 tiles of NPC",
	L"At least 1 merc present withing 3 tiles of NPC",
	L"Босс ожидает визита игрока",
	L"Дэррен ожидает денег от игрока",

	L"Player within 5 tiles and NPC is visible",			//	100
	L"Carmen is in San Mona",
	L"Player Spoke to Carmen",
	L"KingPin knows about stolen money",
	L"Player gave money back to KingPin",
	L"Frank was given the money ( not to buy booze )",
	L"Player was told about KingPin watching fights",
	L"Past club closing time and Darren warned Player. (reset every day)",
	L"Joey is EPC",
	L"Joey is in C5",
	
	L"Joey is within 5 tiles of Martha(109) in sector G8",		//110
	L"Joey is Dead!",
	L"At least one player merc within 5 tiles of Martha",
	L"Spike is occuping tile 9817",
	L"Angel offered vest",
	L"Angel sold vest",
	L"Maria is EPC",
	L"Maria is EPC and inside leather Shop",
	L"Player wants to buy vest",
	L"Maria rescue was noticed by KingPin goons and Kingpin now enemy",
	
	L"Angel left deed on counter",		//120
	L"Maria quest over",
	L"Player bandaged NPC today",
	L"Doreen revealed allegiance to Queen",
	L"Pablo should not steal from player",
	L"Player shipment arrived but loyalty to low, so it left",
	L"Helicopter is in working condition",
	L"Player is giving amount of money >= $1000",
	L"Player is giving amount less than $1000",
	L"Waldo agreed to fix helicopter( heli is damaged )",
	
	L"Helicopter was destroyed",		//130
	L"Waldo told us about heli pilot",
	L"Father told us about Deidranna killing sick people",
	L"Father told us about Chivaldori family",
	L"Father told us about creatures",
	L"Loyalty is OK",
	L"Loyalty is Low",
	L"Loyalty is High",
	L"Player doing poorly",
	L"Player gave valid head to Carmen",
	
	L"Current sector is G9(Cambria)",		//140
	L"Current sector is C5(SanMona)",
	L"Current sector is C13(Drassen",
	L"Carmen has at least $10,000 on him",
	L"Player has Slay on team for over 48 hours",
	L"Carmen is suspicous about slay",
	L"Slay is in current sector",
	L"Carmen gave us final warning",
	L"Vince has explained that he has to charge",
	L"Vince is expecting cash (reset everyday)",
	
	L"Player stole some medical supplies once",		//150
	L"Player stole some medical supplies again",
	L"Vince can be recruited",
	L"Vince is currently doctoring",
	L"Vince was recruited",
	L"Slay offered deal",
	L"All terrorists killed",
	L"",
	L"Maria left in wrong sector",
	L"Skyrider left in wrong sector",
	
	L"Joey left in wrong sector",		//160
	L"John left in wrong sector",
	L"Mary left in wrong sector",
	L"Walter was bribed",
	L"Shank(67) is part of squad but not speaker",
	L"Maddog spoken to",
	L"Jake told us about shank",
	L"Shank(67) is not in secotr",
	L"Bloodcat quest on for more than 2 days",
	L"Effective threat made to Armand",
	
	L"Queen is DEAD!",												//170
	L"Speaker is with Aim or Aim person on squad within 10 tiles",
	L"Current mine is empty",
	L"Current mine is running out",
	L"Loyalty low in affiliated town (low mine production)",
	L"Creatures invaded current mine",
	L"Player LOST current mine",
	L"Current mine is at FULL production",
	L"Dynamo(66) is Speaker or within 10 tiles of speaker",		
	L"Fred told us about creatures",

	L"Matt told us about creatures",				//180
	L"Oswald told us about creatures",
	L"Calvin told us about creatures",
	L"Carl told us about creatures",
	L"Chalice stolen from museam",
	L"John(118) is EPC",
	L"Mary(119) and John (118) are EPC's",	
	L"Mary(119) is alive",
	L"Mary(119)is EPC",					
	L"Mary(119) is bleeding",		

	L"John(118) is alive",							//190
	L"John(118) is bleeding",					
	L"John or Mary close to airport in Drassen(B13)",
	L"Mary is Dead",
	L"Miners placed",
	L"Krott planning to shoot player",
	L"Madlab explained his situation",
	L"Madlab expecting a firearm",
	L"Madlab expecting a video camera.",			
	L"Item condition is < 70 ",										

	L"Madlab complained about bad firearm.",					//200
	L"Madlab complained about bad video camera.",
	L"Robot is ready to go!",
	L"First robot destroyed.",
	L"Madlab given a good camera.",
	L"Robot is ready to go a second time!",
	L"Second robot destroyed.",
	L"Mines explained to player.",
	L"Dynamo (#66) is in sector J9.",			
	L"Dynamo (#66) is alive.",

	L"One PC hasn't fought, but is able, and less than 3 fights have occured",							//210
	L"Player receiving mine income from Drassen, Cambria, Alma & Chitzena",
	L"Player has been to K4_b1",
	L"Brewster got to talk while Warden was alive",
	L"Warden (#103) is dead.",
	L"Ernest gave us the guns",
	L"This is the first bartender",
	L"This is the second bartender",
	L"This is the third bartender",
	L"This is the fourth bartender",


	L"Manny is a bartender.",									//220
	L"Nothing is repaired yet (some stuff being worked on, nothing to give player right now)",
	L"Player made purchase from Howard (#125)",
	L"Dave sold vehicle",
	L"Dave's vehicle ready",
	L"Dave expecting cash for car",
	L"Dave has gas. (randomized daily)",
	L"Vehicle is present",
	L"First battle won by player",
	L"Robot recruited and moved",			

	L"No club fighting allowed",							//230
	L"Player already fought 3 fights today",
	L"Hans mentioned Joey",
	L"Player is doing better than 50% (Alex's function)",
	L"Player is doing very well (better than 80%)",
	L"Father is drunk and sci-fi option is on",
	L"Micky (#96) is drunk",
	L"Player has attempted to force their way into brothel",
	L"Rat effectively threatened 3 times",
	L"Player paid for two people to enter brothel",		

	L"",						//240
	L"",
	L"Player owns 2 towns including omerta",
	L"Player owns 3 towns including omerta",//							243
	L"Player owns 4 towns including omerta",//							244
	L"",
	L"",
	L"",
	L"Fact male speaking female present",
	L"Fact hicks married player merc",//													249

	L"Fact museum open",//																				250
	L"Fact brothel open",//																			251
	L"Fact club open",//																					252
	L"Fact first battle fought",//																253
	L"Fact first battle being fought",//													254
	L"Fact kingpin introduced self",//														255
	L"Fact kingpin not in office",//															256
	L"Fact dont owe kingpin money",//														257
	L"Fact pc marrying daryl is flo",//													258
	L"",

	L"",					//260
	L"Fact npc cowering",																			//	261,
	L"",
	L"",
	L"Fact top and bottom levels cleared",
	L"Fact top level cleared",//																	265
	L"Fact bottom level cleared",//															266
	L"Fact need to speak nicely",//															267
	L"Fact attached item before",//															268
	L"Fact skyrider ever escorted",//														269

	L"Fact npc not under fire",//																270
	L"Fact willis heard about joey rescue",//										271
	L"Fact willis gives discount",//															272
	L"Fact hillbillies killed",//																273
	L"Fact keith out of business",	//														274												
	L"Fact mike available to army",//														275
	L"Fact kingpin can send assassins",//												276
	L"Fact estoni refuelling possible",//						277
	L"Fact museum alarm went off",//															278
	L"",

	L"Fact maddog is speaker", 																	//280,
	L"",
	L"Fact angel mentioned deed", 															// 282,
	L"Fact iggy available to army",//														283
	L"Fact pc has conrads recruit opinion",//										284
	L"",
	L"",
	L"",
	L"",
	L"Fact npc hostile or pissed off",													//289,

	L"",					//290
	L"Fact tony in building",																		//291,
	L"Fact shank speaking",	//																		292,
	L"Fact doreen alive",//																			293
	L"Fact waldo alive",//																				294
	L"Fact perko alive",//																				295
	L"Fact tony alive",//																				296
	L"",
	L"Fact vince alive",// 																				298,
	L"Fact jenny alive",//																				299

	L"",					//300
	L"",
	L"Fact arnold alive",//																				302,
	L"",
	L"Fact rocket rifle exists",// 																304,
	L"",
	L"",
	L"",
	L"",
	L"",

	L"",					//310
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//320

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//330

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//340

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//350

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//360

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//370

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//380

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//390

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//400

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//410

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//420

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//430

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//440

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//450

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//460

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//470

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//480

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//490

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//500
};

//-----------

// Editor
//Editor Taskbar Creation.cpp
STR16 iEditorItemStatsButtonsText[] =
{
	L"Delete",
	L"Delete item (|D|e|l)",
};

STR16 FaceDirs[8] = 
{ 
	L"north",
	L"northeast",
	L"east",
	L"southeast",
	L"south",
	L"southwest",
	L"west",
	L"northwest"
};

STR16 iEditorMercsToolbarText[] = 
{
 L"Toggle viewing of players", //0
 L"Toggle viewing of enemies",
 L"Toggle viewing of creatures",
 L"Toggle viewing of rebels",
 L"Toggle viewing of civilians",
 
 L"Player",
 L"Enemy",
 L"Creature",
 L"Rebels",
 L"Civilian",
 
 L"DETAILED PLACEMENT", //10
 L"General information mode",
 L"Physical appearance mode",
 L"Attributes mode",
 L"Inventory mode",
 L"Profile ID mode",
 L"Schedule mode",
 L"Schedule mode",
 L"DELETE",
 L"Delete currently selected merc (|D|e|l)",
 L"NEXT", //20
 L"Find next merc (|S|p|a|c|e)\nFind previous merc (|C|t|r|l+|S|p|a|c|e)",
 L"Toggle priority existance",
 L"Toggle whether or not placement\nhas access to all doors",
 
 //Orders
 L"STATIONARY",
 L"ON GUARD",
 L"ON CALL",
 L"SEEK ENEMY",
 L"CLOSE PATROL",
 L"FAR PATROL",
 L"POINT PATROL", //30
 L"RND PT PATROL",
 
 //Attitudes
 L"DEFENSIVE",
 L"BRAVE SOLO",
 L"BRAVE AID",
 L"AGGRESSIVE",
 L"CUNNING SOLO",
 L"CUNNING AID",
 
 L"Set merc to face %s",
 
 L"Find",
 L"BAD", //40
 L"POOR",
 L"AVERAGE",
 L"GOOD",
 L"GREAT",
 
 L"BAD",
 L"POOR",
 L"AVERAGE",
 L"GOOD",
 L"GREAT",
 
 L"Previous color set", //50
 L"Next color set",
 
 L"Previous body type",
 L"Next body type",
 
 L"Toggle time variance (+ or - 15 minutes)",
 L"Toggle time variance (+ or - 15 minutes)",
 L"Toggle time variance (+ or - 15 minutes)",
 L"Toggle time variance (+ or - 15 minutes)",
 
 L"No action",
 L"No action",
 L"No action", //60
 L"No action",
 
 L"Clear Schedule",
 
 L"Find selected merc",
};

STR16 iEditorBuildingsToolbarText[] =
{
	L"ROOFS",  //0
	L"WALLS",
	L"ROOM INFO",

	L"Place walls using selection method",
	L"Place doors using selection method",
	L"Place roofs using selection method",
	L"Place windows using selection method",
	L"Place damaged walls using selection method.",
	L"Place furniture using selection method",
	L"Place wall decals using selection method",
	L"Place floors using selection method", //10
	L"Place generic furniture using selection method",
	L"Place walls using smart method",
	L"Place doors using smart method",
	L"Place windows using smart method",
	L"Place damaged walls using smart method",
	L"Lock or trap existing doors",

	L"Add a new room",
	L"Edit cave walls.",
	L"Remove an area from existing building.",
	L"Remove a building", //20
	L"Add/replace building's roof with new flat roof.",
	L"Copy a building",
	L"Move a building",
	L"Draw room number\n(Hold |S|h|i|f|t to reuse room number)",
	L"Erase room numbers",

	L"Toggle |Erase mode",
	L"Undo last change (|B|a|c|k|s|p|a|c|e)",
	L"Cycle brush size (|A/|Z)",
	L"Roofs (|H)",
	L"|Walls", //30
	L"Room Info (|N)",
};

STR16 iEditorItemsToolbarText[] =
{
	L"Wpns", //0
	L"Ammo",
	L"Armour",
	L"Разгр.",
	L"Exp",
	L"E1",
	L"E2",
	L"E3",
	L"Triggers",
	L"Keys",
	L"Rnd", //10
	L"Previous (|,)",		// previous page
	L"Next (|.)",		// next page
};

STR16 iEditorMapInfoToolbarText[] =
{
	L"Add ambient light source", //0
	L"Toggle fake ambient lights.",
	L"Add exit grids (r-clk to query existing).",
	L"Cycle brush size (|A/|Z)",
	L"Undo last change (|B|a|c|k|s|p|a|c|e)",
	L"Toggle |Erase mode",
	L"Specify north point for validation purposes.",
	L"Specify west point for validation purposes.",
	L"Specify east point for validation purposes.",
	L"Specify south point for validation purposes.",
	L"Specify center point for validation purposes.", //10
	L"Specify isolated point for validation purposes.",
};

STR16 iEditorOptionsToolbarText[]=
{
	L"New outdoor level",  //0
	L"New basement",
	L"New cave level",
	L"Save map (|C|t|r|l+|S)",
	L"Load map (|C|t|r|l+|L)",
	L"Select tileset",
	L"Leave Editor mode",
	L"Exit game (|A|l|t+|X)",
	L"Create radar map",
	L"When checked, the map will be saved in original JA2 map format. Items with ID > 350 will be lost.\nThis option is only valid on 'normal' size maps that do not reference grid numbers (e.g: exit grids) > 25600.",
	L"When checked and you load a map, the map will be enlarged automatically depending on the selected Rows and Cols.",
};

STR16 iEditorTerrainToolbarText[] =
{
	L"Draw |Ground textures", //0
	L"Set map ground textures",
	L"Place banks and |Cliffs",
	L"Draw roads (|P)",
	L"Draw |Debris",
	L"Place |Trees & bushes",
	L"Place |Rocks",
	L"Place barrels & |Other junk",
	L"Fill area",
	L"Undo last change (|B|a|c|k|s|p|a|c|e)",
	L"Toggle |Erase mode", //10
	L"Cycle brush size (|A/|Z)",
	L"Raise brush density (|])",
	L"Lower brush density (|[)",
};

STR16 iEditorTaskbarInternalText[]=
{
	L"Terrain", //0
	L"Buildings",
	L"Items",
	L"Mercs",
	L"Map Info",
	L"Options",
	L"|./|,: Cycle 'width: xx' dimensions\n|P|g |U|p/|P|g |D|n: Previous/Next tile for selected object(s)/in smart method", //Terrain fasthelp text
	L"|./|,: Cycle 'width: xx' dimensions\n|P|g |U|p/|P|g |D|n: Previous/Next tile for selected object(s)/in smart method", //Buildings fasthelp text
	L"|S|p|a|c|e: Select next item\n|C|t|r|l+|S|p|a|c|e: Select previous item\n \n|/: Place same item under mouse cursor\n|C|t|r|l+|/: Place new item under mouse cursor", //Items fasthelp text
	L"|1-|9: Set waypoints\n|C|t|r|l+|C/|C|t|r|l+|V: Copy/Paste merc\n|P|g |U|p/|P|g |D|n: Toggle merc placement level", //Mercs fasthelp text
	L"|C|t|r|l+|G: Go to grid no\n|S|h|i|f|t: Scroll beyond map boundary\n \n(|t|i|l|d|e): Toggle cursor level\n|I: Toggle overhead map\n|J: Toggle draw high ground\n|K: Toggle high ground markers\n|S|h|i|f|t+|L: Toggle map edge points\n|S|h|i|f|t+|T: Toggle treetops\n|U: Toggle world raise\n \n|./|,: Cycle 'width: xx' dimensions", //Map Info fasthelp text
	L"|C|t|r|l+|N: Create new map\n \n|F|5: Show Summary Info/Country Map\n|F|1|0: Remove all lights\n|F|1|1: Reverse schedules\n|F|1|2: Clear schedules\n \n|S|h|i|f|t+|R: Toggle random placement based on quantity of selected object(s)\n \nCommand Line options\n|-|D|O|M|A|P|S: Batch radarmap generation\n|-|D|O|M|A|P|S|C|N|V: Batch radarmap generation and covert maps to latest version", //Options fasthelp text
};

//Editor Taskbar Utils.cpp

STR16 iRenderMapEntryPointsAndLightsText[] =
{
	L"North Entry Point", //0
	L"West Entry Point",
	L"East Entry Point",
	L"South Entry Point",
	L"Center Entry Point",
	L"Isolated Entry Point",
	
	L"Prime",
	L"Night",
	L"24Hour",
};

STR16 iBuildTriggerNameText[] =
{
	L"Panic Trigger1", //0
	L"Panic Trigger2",
	L"Panic Trigger3",
	L"Trigger%d",
	 
	L"Pressure Action",
	L"Panic Action1",
	L"Panic Action2",
	L"Panic Action3",
	L"Action%d",
};

STR16 iRenderDoorLockInfoText[]=
{
	L"No Lock ID", //0
	L"Explosion Trap",
	L"Electric Trap",
	L"Siren Trap",
	L"Silent Alarm",
	L"Super Electric Trap", //5
	L"Brothel Siren Trap",
	L"Trap Level %d",
};

STR16 iRenderEditorInfoText[]=
{
	L"Save map in vanilla JA2 (v1.12) map format (Version: 5.00 / 25)", //0
	L"No map currently loaded.",
	L"File:  %S, Current Tileset:  %s",
	L"Enlarge map on loading",
};
//EditorBuildings.cpp
STR16 iUpdateBuildingsInfoText[] =
{
	L"TOGGLE", //0
	L"VIEWS",
	L"SELECTION METHOD",
	L"SMART METHOD",
	L"BUILDING METHOD",
	L"Room#", //5
};

STR16 iRenderDoorEditingWindowText[] =
{
	L"Editing lock attributes at map index %d.",
	L"Lock ID",
	L"Trap Type",
	L"Trap Level",
	L"Locked",
};

//EditorItems.cpp

STR16 pInitEditorItemsInfoText[] = 
{
	L"Pressure Action", //0
	L"Panic Action1",
	L"Panic Action2",
	L"Panic Action3",
	L"Action%d",
	
	L"Panic Trigger1", //5
	L"Panic Trigger2",
	L"Panic Trigger3",
	L"Trigger%d",
};

STR16 pDisplayItemStatisticsTex[] =
{
	L"Status Info Line 1",
	L"Status Info Line 2",
	L"Status Info Line 3",
	L"Status Info Line 4",
	L"Status Info Line 5",
};

//EditorMapInfo.cpp
STR16 pUpdateMapInfoText[] =
{
	L"R", //0
	L"G",
	L"B",
	
	L"Prime",
	L"Night",
	L"24Hrs", //5

	L"Radius",

	L"Underground",
	L"Light Level",

	L"Outdoors",
	L"Basement", //10
	L"Caves",

	L"Restricted",
	L"Scroll ID",

	L"Destination",
	L"Sector", //15
	L"Destination",
	L"Bsmt. Level",
	L"Dest.",
	L"GridNo",
};
//EditorMercs.cpp
CHAR16 gszScheduleActions[ 11 ][20] =
{
	L"No action",
	L"Lock door",
	L"Unlock door",
	L"Open door",
	L"Close door",
	L"Move to gridno",
	L"Leave sector",
	L"Enter sector",
	L"Stay in sector",
	L"Sleep",
	L"Ignore this!"
};

STR16 zDiffNames[5] =	// NUM_DIFF_LVLS = 5
{ 
	L"Wimp", 
	L"Easy", 
	L"Average",
	L"Tough", 
	L"Steroid Users Only" 
};

STR16 EditMercStat[12] = 
{ 
	L"Max Health",
	L"Cur Health",
	L"Strength",
	L"Agility",
	L"Dexterity",
	L"Charisma",
	L"Wisdom",
	L"Marksmanship",
	L"Explosives",
	L"Medical",
	L"Scientific",
	L"Exp Level", 
};


STR16 EditMercOrders[8] = 
{ 
	L"Stationary",
	L"On Guard",
	L"Close Patrol",
	L"Far Patrol",
	L"Point Patrol",
	L"On Call",
	L"Seek Enemy", 
	L"Random Point Patrol",
};

STR16 EditMercAttitudes[6] = 
{ 
	L"Defensive",
	L"Brave Loner",
	L"Brave Buddy",
	L"Cunning Loner",
	L"Cunning Buddy",
	L"Aggressive", 
};

STR16 pDisplayEditMercWindowText[] =
{
	L"Merc Name:", //0
	L"Orders:",
	L"Combat Attitude:",
};

STR16 pCreateEditMercWindowText[] = 
{
	L"Merc Colors", //0
	L"Done",
	
	L"Previous merc standing orders",
	L"Next merc standing orders", 
	
	L"Previous merc combat attitude",
	L"Next merc combat attitude",	//5
	
	L"Decrease merc stat",
	L"Increase merc stat",
};

STR16 pDisplayBodyTypeInfoText[] =
{
	L"Random", //0
	L"Reg Male",
	L"Big Male",
	L"Stocky Male",
	L"Reg Female",
	L"NE Tank", //5
	L"NW Tank",
	L"Fat Civilian",
	L"M Civilian",
	L"Miniskirt",
	L"F Civilian", //10
	L"Kid w/ Hat",
	L"Humvee",
	L"Eldorado",
	L"Icecream Truck",
	L"Jeep", //15
	L"Kid Civilian",
	L"Domestic Cow",
	L"Cripple",
	L"Unarmed Robot",
	L"Larvae", //20
	L"Infant",
	L"Yng F Monster",
	L"Yng M Monster",
	L"Adt F Monster",
	L"Adt M Monster", //25
	L"Queen Monster",
	L"Bloodcat",
	L"Humvee",		// TODO.Translate
};

STR16 pUpdateMercsInfoText[] = 
{
	L" --=ORDERS=-- ", //0
	L"--=ATTITUDE=--",
	
	L"RELATIVE",
	L"ATTRIBUTES",
	
	L"RELATIVE",
	L"EQUIPMENT",
			
	L"RELATIVE",
	L"ATTRIBUTES",
	
	L"Army",
	L"Admin",
	L"Elite", //10
	
	L"Exp. Level",
	L"Life",
	L"LifeMax",
	L"Marksmanship",
	L"Strength",
	L"Agility",
	L"Dexterity",
	L"Wisdom",
	L"Leadership",
	L"Explosives", //20
	L"Medical",
	L"Mechanical",
	L"Morale",
	
	L"Hair color:",
	L"Skin color:",
	L"Vest color:",
	L"Pant color:",
	
	L"RANDOM",
	L"RANDOM",
	L"RANDOM", //30
	L"RANDOM",
	
	L"By specifying a profile index, all of the information will be extracted from the profile ",
	L"and override any values that you have edited.  It will also disable the editing features ",
	L"though, you will still be able to view stats, etc.  Pressing ENTER will automatically ",
	L"extract the number you have typed.  A blank field will clear the profile.  The current ",
	L"number of profiles range from 0 to ",
	
	L"Current Profile:  n/a              ",
	L"Current Profile: %s",
	
	L"STATIONARY",
	L"ON CALL", //40
	L"ON GUARD",
	L"SEEK ENEMY",
	L"CLOSE PATROL",
	L"FAR PATROL",
	L"POINT PATROL",
	L"RND PT PATROL",

	L"Action",
	L"Time",
	L"V",
	L"GridNo 1", //50
	L"GridNo 2",
	L"1)",
	L"2)",
	L"3)",
	L"4)",
	
	L"lock",
	L"unlock",
	L"open",
	L"close",
	
	L"Click on the gridno adjacent to the door that you wish to %s.", //60
	L"Click on the gridno where you wish to move after you %s the door.",
	L"Click on the gridno where you wish to move to.",
	L"Click on the gridno where you wish to sleep at.	Person will automatically return to original position after waking up.",
	L" Hit ESC to abort entering this line in the schedule.",
};

CHAR16 pRenderMercStringsText[][100] =
{
	L"Slot #%d",
	L"Patrol orders with no waypoints",
	L"Waypoints with no patrol orders",
};

STR16 pClearCurrentScheduleText[] =
{
	L"No action",
};

STR16 pCopyMercPlacementText[] =
{
	L"Placement not copied because no placement selected.",
	L"Placement copied.",
};

STR16 pPasteMercPlacementText[] = 
{
	L"Placement not pasted as no placement is saved in buffer.",
	L"Placement pasted.",
	L"Placement not pasted as the maximum number of placements for this team has been reached.",
};

//editscreen.cpp
STR16 pEditModeShutdownText[] = 
{
	L"Exit editor?",
};

STR16 pHandleKeyboardShortcutsText[] = 
{
	L"Are you sure you wish to remove all lights?", //0
	L"Are you sure you wish to reverse the schedules?",
	L"Are you sure you wish to clear all of the schedules?",
	
	L"Clicked Placement Enabled",
	L"Clicked Placement Disabled",
	
	L"Draw High Ground Enabled", //5
	L"Draw High Ground Disabled",
	
	L"Number of edge points: N=%d E=%d S=%d W=%d",
	
	L"Random Placement Enabled",
	L"Random Placement Disabled",
	
	L"Removing Treetops", //10
	L"Showing Treetops",
	
	L"World Raise Reset",
	
	L"World Raise Set Old",
	L"World Raise Set",
};

STR16 pPerformSelectedActionText[] = 
{
	L"Creating radar map for %S", //0
	
	L"Delete current map and start a new basement level?",
	L"Delete current map and start a new cave level?",
	L"Delete current map and start a new outdoor level?",
	
	L" Wipe out ground textures? ",
};

STR16 pWaitForHelpScreenResponseText[] = 
{
	L"HOME", //0
	L"Toggle fake editor lighting ON/OFF",

	L"INSERT",
	L"Toggle fill mode ON/OFF",

	L"BKSPC",
	L"Undo last change",

	L"DEL",
	L"Quick erase object under mouse cursor",

	L"ESC",
	L"Exit editor",

	L"PGUP/PGDN", //10
	L"Change object to be pasted",

	L"F1",
	L"This help screen",

	L"F10",
	L"Save current map",

	L"F11",
	L"Load map as current",

	L"+/-",
	L"Change shadow darkness by .01",

	L"SHFT +/-",  //20
	L"Change shadow darkness by .05",

	L"0 - 9",
	L"Change map/tileset filename",

	L"b",
	L"Change brush size",

	L"d",
	L"Draw debris",

	L"o",
	L"Draw obstacle",

	L"r", //30
	L"Draw rocks",

	L"t",
	L"Toggle trees display ON/OFF",

	L"g",
	L"Draw ground textures",

	L"w",
	L"Draw building walls",

	L"e",
	L"Toggle erase mode ON/OFF",

	L"h",  //40
	L"Toggle roofs ON/OFF",
};

STR16 pAutoLoadMapText[] =
{
	L"Map data has just been corrupted.  Don't save, don't quit, get Kris!  If he's not here, save the map using a temp filename and document everything you just did, especially your last action!",
	L"Schedule data has just been corrupted.  Don't save, don't quit, get Kris!  If he's not here, save the map using a temp filename and document everything you just did, especially your last action!",
};

STR16 pShowHighGroundText[] =
{
	L"Showing High Ground Markers",
	L"Hiding High Ground Markers",
};

//Item Statistics.cpp
/*CHAR16 gszActionItemDesc[ 34 ][ 30 ] =
{
	L"Klaxon Mine",
	L"Flare Mine",
	L"Teargas Explosion",
	L"Stun Explosion",
	L"Smoke Explosion",
	L"Mustard Gas",
	L"Land Mine",
	L"Open Door",
	L"Close Door",
	L"3x3 Hidden Pit",
	L"5x5 Hidden Pit",
	L"Small Explosion",
	L"Medium Explosion",
	L"Large Explosion",
	L"Toggle Door",
	L"Toggle Action1s",
	L"Toggle Action2s",
	L"Toggle Action3s",
	L"Toggle Action4s",
	L"Enter Brothel",
	L"Exit Brothel",
	L"Kingpin Alarm",
	L"Sex with Prostitute",
	L"Reveal Room",
	L"Local Alarm",
	L"Global Alarm",
	L"Klaxon Sound",
	L"Unlock door",
	L"Toggle lock",
	L"Untrap door",
	L"Tog pressure items",
	L"Museum alarm",
	L"Bloodcat alarm",
	L"Big teargas",
};
*/
STR16 pUpdateItemStatsPanelText[] =
{
	L"Toggle hide flag", //0
	L"No item selected.",
	L"Slot available for",
	L"Random generation.",
	L"Keys not editable.",
	L"ProfileID of owner",
	L"Item class not implemented.",
	L"Slot locked as empty.",
	L"Status",
	L"Rounds",
	L"Trap Level", //10
	L"Quantity",
	L"Trap Level",
	L"Status",
	L"Trap Level",
	L"Status",
	L"Quantity",
	L"Trap Level",
	L"Dollars",
	L"Status",
	L"Trap Level", //20
	L"Trap Level",
	L"Tolerance",
	L"Alarm Trigger",
	L"Exist Chance",
	L"B",
	L"R",
	L"S",
};

STR16 pSetupGameTypeFlagsText[] =
{
	L"Item appears in both Sci-Fi and Realistic modes", //0
	L"Item appears in Realistic mode only",
	L"Item appears in Sci-Fi mode only",
};

STR16 pSetupGunGUIText[] =
{
	L"SILENCER", //0
	L"SNIPERSCOPE",
	L"LASERSCOPE",
	L"BIPOD",
	L"DUCKBILL",
	L"G-LAUNCHER", //5
};

STR16 pSetupArmourGUIText[] =
{
	L"CERAMIC PLATES", //0
};

STR16 pSetupExplosivesGUIText[] =
{
	L"DETONATOR",
};

STR16 pSetupTriggersGUIText[] =
{
	L"If the panic trigger is an alarm trigger,\nenemies won't attempt to use it if they\nare already aware of your presence.",
};

//Sector Summary.cpp

STR16 pCreateSummaryWindowText[]=
{
	L"Okay", //0
	L"A",
	L"G",
	L"B1",
	L"B2",
	L"B3", //5
	L"LOAD",
	L"SAVE",
	L"Update",
};

STR16 pRenderSectorInformationText[] =
{
	L"Tileset:  %s", //0
	L"Version Info:  Summary:  1.%02d,  Map:  %1.2f / %02d",
	L"Number of items:  %d", 
	L"Number of lights:  %d",
	L"Number of entry points:  %d",
	
	L"N",
	L"E",
	L"S",
	L"W",
	L"C",
	L"I", //10
	
	L"Number of rooms:  %d",
	L"Total map population:  %d",
	L"Enemies:  %d",
	L"Admins:  %d",
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	L"Troops:  %d",
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	L"Elites:  %d",
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	L"Civilians:  %d",  //20
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	
	L"Humans:  %d",
	L"Cows:  %d",
	L"Bloodcats:  %d",
	
	L"Creatures:  %d",
	
	L"Monsters:  %d",
	L"Bloodcats:  %d",
	
	L"Number of locked and/or trapped doors:  %d",
	L"Locked:  %d",
	L"Trapped:  %d", //30
	L"Locked & Trapped:  %d",
	
	L"Civilians with schedules:  %d",
	
	L"Too many exit grid destinations (more than 4)...",
	L"ExitGrids:  %d (%d with a long distance destination)",
	L"ExitGrids:  none",
	L"ExitGrids:  1 destination using %d exitgrids",
	L"ExitGrids:  2 -- 1) Qty: %d, 2) Qty: %d",
	L"ExitGrids:  3 -- 1) Qty: %d, 2) Qty: %d, 3) Qty: %d",
	L"ExitGrids:  3 -- 1) Qty: %d, 2) Qty: %d, 3) Qty: %d, 4) Qty: %d",
	L"Enemy Relative Attributes:  %d bad, %d poor, %d norm, %d good, %d great (%+d Overall)", //40
	L"Enemy Relative Equipment:  %d bad, %d poor, %d norm, %d good, %d great (%+d Overall)",
	L"%d placements have patrol orders without any waypoints defined.",
	L"%d placements have waypoints, but without any patrol orders.",
	L"%d gridnos have questionable room numbers.  Please validate.",
	
};

STR16 pRenderItemDetailsText[] =
{
	L"R",  //0
	L"S",
	L"Enemy",
	
	L"TOO MANY ITEMS TO DISPLAY!",
	
	L"Panic1",
	L"Panic2",
	L"Panic3",
	L"Norm1",
	L"Norm2",
	L"Norm3",
	L"Norm4", //10
	L"Pressure Actions",
	
	L"TOO MANY ITEMS TO DISPLAY!",
	
	L"PRIORITY ENEMY DROPPED ITEMS",
	L"None",
	
	L"TOO MANY ITEMS TO DISPLAY!",
	L"NORMAL ENEMY DROPPED ITEMS",
	L"TOO MANY ITEMS TO DISPLAY!",
	L"None",
	L"TOO MANY ITEMS TO DISPLAY!",
	L"ERROR:  Can't load the items for this map.  Reason unknown.", //20	
};

STR16 pRenderSummaryWindowText[] =
{
	L"CAMPAIGN EDITOR -- %s Version 1.%02d", //0
	L"(NO MAP LOADED).",
	L"You currently have %d outdated maps.",
	L"The more maps that need to be updated, the longer it takes.  It'll take ",
	L"approximately 4 minutes on a P200MMX to analyse 100 maps, so",
	L"depending on your computer, it may vary.",
	L"Do you wish to regenerate info for ALL these maps at this time (y/n)?",
	
	L"There is no sector currently selected.",
	
	L"Entering a temp file name that doesn't follow campaign editor conventions...",
	
	L"You need to either load an existing map or create a new map before being",
	L"able to enter the editor, or you can quit (ESC or Alt+x).", //10

	L", ground level",					
	L", underground level 1",	
	L", underground level 2",	
	L", underground level 3",	
	L", alternate G level",					
	L", alternate B1 level",	
	L", alternate B2 level",	
	L", alternate B3 level",
	
	L"ITEM DETAILS -- sector %s",
	L"Summary Information for sector %s:",//20
	
	L"Summary Information for sector %s",
	L"does not exist.",
	
	L"Summary Information for sector %s",
	L"does not exist.",
	
	L"No information exists for sector %s.",
	
	L"No information exists for sector %s.",
	
	L"FILE:  %s",
	
	L"FILE:  %s",
	
	L"Override READONLY",
	L"Overwrite File", //30
	
	L"You currently have no summary data.  By creating one, you will be able to keep track",
	L"of information pertaining to all of the sectors you edit and save.  The creation process",
	L"will analyse all maps in your \\MAPS directory, and generate a new one.  This could",
	L"take a few minutes depending on how many valid maps you have.  Valid maps are",
	L"maps following the proper naming convention from a1.dat - p16.dat.  Underground maps", 
	L"are signified by appending _b1 to _b3 before the .dat (ex:  a9_b1.dat). ",
	
	L"Do you wish to do this now (y/n)?",
	
	L"No summary info.  Creation denied.",
	
	L"Grid",
	L"Progress", //40
	L"Use Alternate Maps",
	
	L"Summary",
	L"Items",
};

STR16 pUpdateSectorSummaryText[] =
{
	L"Analyzing map:  %s...",
};

STR16 pSummaryLoadMapCallbackText[] =
{
	L"Loading map:  %s",
};

STR16 pReportErrorText[] =
{
	L"Skipping update for %s.  Probably due to tileset conflicts...",
};

STR16 pRegenerateSummaryInfoForAllOutdatedMapsText[] =
{
	L"Generating map information",
};

STR16 pSummaryUpdateCallbackText[] =
{
	L"Generating map summary",
};

STR16 pApologizeOverrideAndForceUpdateEverythingText[] =
{
	L"MAJOR VERSION UPDATE",
	L"There are %d maps requiring a major version update.",
	L"Updating all outdated maps",
};

//selectwin.cpp
STR16 pDisplaySelectionWindowGraphicalInformationText[] =
{
	L"%S[%d] from default tileset %s (%d, %S)",
	L"File: %S, subindex: %d (%d, %S)",
	L"Tileset:  %s",
};

STR16 pDisplaySelectionWindowButtonText[] =
{
	L"Accept selections (|E|n|t|e|r)",
	L"Cancel selections (|E|s|c)\nClear selections (|S|p|a|c|e)",
	L"Scroll window up (|U|p)",
	L"Scroll window down (|D|o|w|n)",
};

//Cursor Modes.cpp
STR16 wszSelType[6] = {
 L"Small", 
 L"Medium", 
 L"Large", 
 L"XLarge", 
 L"Width: xx", 
 L"Area" 
 };

//---

CHAR16  gszAimPages[ 6 ][ 20 ] =
{
	L"Cтp. 1/2", //0
	L"Cтp. 2/2",
	
	L"Cтp. 1/3",
	L"Cтp. 2/3",
	L"Cтp. 3/3",
	
	L"Cтp. 1/1", //5
};

// by Jazz
CHAR16 zGrod[][500] =
{
	L"Робот", //0    // Robot
};

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
//	L".38 кал",
//	L"9 мм",
//	L".45 кал",
//	L".357 кал",
//	L"12 кал",
//	L"ОББ",
//	L"5,45 мм",
//	L"5,56 мм",
//	L"7,62 мм НАТО",
//	L"7,62 мм ВД",
//	L"4,7 мм",
//	L"5,7 мм",
//	L"Монстр",
//	L"Ракета",
//	L"", // дротик
//	L"", // пламя
////	L".50 кал", // barrett
////	L"9 мм тяж", // Val silent
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
//	L".38 кал",
//	L"9 мм",
//	L".45 кал",
//	L".357 кал",
//	L"12 кал",
//	L"ОББ",
//	L"5,45 мм",
//	L"5,56 мм",
//	L"7,62 мм Н.",
//	L"7,62 мм ВД",
//	L"4,7 мм",
//	L"5,7 мм",
//	L"Монстр",
//	L"Ракета",
//	L"", // дротик
////	L"", // flamethrower
////	L".50 кал", // barrett
////	L"9 мм тяж", // Val silent
//};


CHAR16 WeaponType[MAXITEMS][30] =
{
	L"Прочие",
	L"Пистолет",
	L"Авт.пистолет",
	L"ПП",
	L"Винтовка",
	L"Сн.винтовка",
	L"Автомат",
	L"Ручной пулемёт",
	L"Дробовик",
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"Ход Игрока", // player's turn
	L"Ход Противника",
	L"Ход Тварей",
	L"Ход Ополчения",
	L"Ход Гражданских",
	L"Планирование",// planning turn
	L"Клиент №1",//hayden
	L"Клиент №2",//hayden
	L"Клиент №3",//hayden
	L"Клиент №4",//hayden

};

CHAR16 Message[][STRING_LENGTH] =
{
	L"",

	// In the following 8 strings, the %s is the merc's name, and the %d (if any) is a number.

	L"%s получает ранение в голову и теряет в интеллекте!",
	L"%s получает ранение в плечо и теряет в ловкости!",
	L"%s получает ранение в грудь и теряет в силе!",
	L"%s получает ранение в ногу и теряет в проворности!",
	L"%s получает ранение в голову и теряет %d ед. интеллекта!",
	L"%s получает ранение в плечо и теряет %d ед. ловкости!",
	L"%s получает ранение в грудь и теряет %d ед. силы!",
	L"%s получает ранение в ногу и теряет %d ед. проворности!",
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
	L"очередями",
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

	L"Этот предмет можно присоединить к другим, поместив его в одно из четырех мест для навески.",
	L"Этот предмет можно присоединить к другим, поместив его в одно из четырех мест для навески. (Однако эти предметы несовместимы)",
	L"В секторе еще остались враги!",
	L"%s требует полную оплату, нужно заплатить ещё %s", 
	L"%s: попадание в голову!",
	L"Покинуть битву?",
	L"Это несъёмное приспособление. Установить его?",
	L"%s чувствует прилив энергии!",
	L"%s поскальзывается на стеклянных шариках!",
	L"%s не удалось отобрать %s у врага!",
	L"%s чинит %s",
	L"Перехватили ход: ",
	L"Сдаться?",
	L"Человек отверг вашу помощь.",
	L"Вам это надо?",
	L"Чтобы воспользоваться вертолётом Небесного Всадника - выберите \"Транспорт/Вертолёт\".",
	L"%s успевает зарядить только одно оружие.",
	L"Ход кошек-убийц",
	L"автоматический",
	L"неавтоматический",
	L"точный",
	L"неточный",
	L"нет одиночных",
	L"Враг обобран до нитки!",
	L"У врага в руках ничего нет!",

	L"%s: песчаный камуфляж стёрся.",
	L"%s: песчаный камуфляж смылся.",
	
	L"%s: растительный камуфляж стёрся.",
	L"%s: растительный камуфляж смылся.",
	
	L"%s: городской камуфляж стёрся.",
	L"%s: городской камуфляж смылся.",
	
	L"%s: зимний камуфляж стёрся.",
	L"%s: зимний камуфляж смылся.",

	L"Нельзя установить навеску %s на это место.",
	L"%s не поместится ни в один слот.",
	L"Недостаточно места для этого кармана.",

	L"%s отремонтировал(а) %s, насколько это было возможно.",
	L"%s отремонтировал(а) у наёмника %s %s, насколько это было возможно.",

	L"%s почистил(а) %s.",	// TODO.Translate
	L"%s почистил(а) у %s %s.",

	L"Assignment not possible at the moment",	// TODO.Translate
	L"No militia that can be drilled present.",

	L"%s has fully explored %s.",	// TODO.Translate
};

// the country and its noun in the game
CHAR16 pCountryNames[][MAX_TOWN_NAME_LENGHT] =
{
#ifdef JA2UB
	L"Тракону",
	L"Траконец",
#else
	L"Арулько",
	L"Арулькиец",
#endif
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
	L"Сканировать частоты",	// scanning for nearby patrols
	L"Практика",	// training themselves  
	L"Ополчение",		// training a town to revolt 
	L"Патруль", //training moving militia units		//M.Militia
	L"Тренер",	// training a teammate
	L"Ученик",	// being trained by someone else
	L"Носильщик",	// move items 
	L"Штат", // operating a strategic facility			//Staff
	L"Питаться",		// eating at a facility (cantina etc.) 
	L"Отдых", // Resting at a facility					//Rest
	L"Допрос",		// Flugente: interrogate prisoners
	L"Мёртв",	// dead
	L"Недеесп.",	// abbreviation for incapacitated
	L"В плену",	// Prisoner of war - captured
	L"Госпиталь",	// patient in a hospital 
	L"Пуст",	// Vehicle is empty
	L"Осведомитель",// facility: undercover prisoner (snitch)
	L"Пропаганда",	// facility: spread propaganda
	L"Пропаганда",	// facility: spread propaganda (globally)
	L"Слухи",	// facility: gather information
	L"Пропаганда",	// spread propaganda
	L"Слухи",	// gather information
	L"Командует",	// militia movement orders
	L"Осмотр", // disease diagnosis
	L"Леч.насел.", // treat disease among the population
	L"Медик",	// administering medical aid
	L"Пациент",	// getting medical aid
	L"Ремонт",	// repairing
	L"Укрепляет",		// build structures according to external layout
	L"Учит рабочих",
	L"Hide",	// TODO.Translate
	L"GetIntel",
	L"DoctorM.",
	L"DMilitia",
	L"Burial",
	L"Admin", // TODO.Translate
	L"Explore",	// TODO.Translate
	L"Event",// rftr: merc is on a mini event // TODO: translate
	L"Mission", // rftr: rebel command
};


STR16 pMilitiaString[] =
{
	L"Ополчение", // the title of the militia box
	L"Резерв", //the number of unassigned militia troops
	L"Нельзя перераспределять ополчение во время боя!",
	L"Некоторые ополченцы не были определены по секторам. Желаете их распустить (уволить)?",
};


STR16 pMilitiaButtonString[] =
{
	L"Авто", // auto place the militia troops for the player
	L"Готово", // done placing militia troops
	L"Распустить", // HEADROCK HAM 3.6: Disband militia
	L"В резерв", // move all milita troops to unassigned pool
};

STR16 pConditionStrings[] = 
{
	L"Отличное",	//the state of a soldier .. excellent health
	L"Хорошее",	//good health
	L"Сносное",	//fair health
	L"Ранен",	//wounded health
	L"Устал",	//tired
	L"Кровоточит", //bleeding to death
	L"Без сознания", //knocked out 
	L"Умирает",	//near death
	L"Мёртв",	//dead
};

STR16 pEpcMenuStrings[] =
{
	L"На службе", // set merc on active duty
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
	L"Сканирует радиочастоты",	// radio scan
	L"Практика", 
	L"Ополчение",
	L"Тренирует патруль",	//Training Mobile Militia
	L"Тренер",
	L"Ученик",
	L"Носильщик",	// move items
	L"Работает с населением",			//Facility Staff
	L"Питается",		// eating at a facility (cantina etc.) 
	L"Отдыхает",		//Resting at Facility
	L"Допрашивает пленных",		// Flugente: interrogate prisoners
	L"Мёртв",
	L"Недеесп.",
	L"В плену",
	L"Госпиталь", 
	L"Пуст",	// Vehicle is empty
	L"Осведомитель",	 // facility: undercover prisoner (snitch)
	L"Ведет пропаганду", // facility: spread propaganda
	L"Ведет пропаганду", // facility: spread propaganda (globally)
	L"Собирает слухи", 	 // facility: gather rumours	
	L"Ведет пропаганду", // spread propaganda
	L"Собирает слухи",   // gather information
	L"Руководит ополчением",	// militia movement orders
	L"Обследование", // disease diagnosis
	L"Лечит население", // treat disease among the population
	L"Медик",
	L"Пациент",
	L"Ремонт",
	L"Укрепляет сектор",		// build structures according to external layout
	L"Учит рабочих",
	L"Hide while disguised",	// TODO.Translate
	L"Get intel while disguised",
	L"Doctor wounded militia",
	L"Drill existing militia",
	L"Bury corpses",
	L"Administration",	// TODO.Translate
	L"Exploration",	// TODO.Translate
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
	L"В транспорте",
	L"В пути",
	L"Ремонтирует",
	L"Сканирует радиочастоты",	// radio scan
	L"Практикуется", 
	L"Тренирует ополчение",
	L"Тренирует патруль",	//Train Mobiles
	L"Тренирует",
	L"Обучается",
	L"Носильщик",	// move items
	L"Работает с населением",		//Staff Facility
	L"Отдыхает в заведении",		//Resting at Facility
	L"Допрашивает пленных",		// Flugente: interrogate prisoners
	L"Мёртв",
	L"Недееспособен",
	L"В плену",
	L"В госпитале", // patient in a hospital 
	L"Без пассажиров",	// Vehicle is empty
	L"Осведомитель",			// facility: undercover prisoner (snitch)
	L"Ведет пропаганду",		// facility: spread propaganda
	L"Ведет пропаганду",		// facility: spread propaganda (globally)
	L"Собирает слухи",			// facility: gather rumours	
	L"Ведет пропаганду",		// spread propaganda
	L"Собирает слухи",			// gather information
	L"Руководит ополчением",	// militia movement orders
	L"Обследование", // disease diagnosis
	L"Лечит население", // treat disease among the population
	L"Медик",
	L"Пациент",
	L"Ремонтирует",
	L"Укрепляет сектор",		// build structures according to external layout
	L"Учит рабочих",
	L"Hide while disguised",	// TODO.Translate
	L"Get intel while disguised",
	L"Doctor wounded militia",
	L"Drill existing militia",
	L"Bury corpses",
	L"Administration",	// TODO.Translate
	L"Exploration",	// TODO.Translate
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
	L"Мех", // mechanical skill
	L"Взр", // explosive skill
	L"Мед", // medical skill
};


STR16 pUpperLeftMapScreenStrings[] =
{
	L"Занятие", // the mercs current assignment 
	L"Контракт", // the contract info about the merc
	L"Здоровье", // the health level of the current merc
	L"Боев.дух", // the morale of the current merc
	L"Сост.",	// the condition of the current vehicle
	L"Топливо",	// the fuel level of the current vehicle
};

STR16 pTrainingStrings[] =
{
	L"Практика", // tell merc to train self 
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
	L"Заболевания", // merc is a doctor doing diagnosis
	L"Пациент", // the merc is receiving medical attention
	L"Транспорт", // the merc is in a vehicle
	L"Ремонт", // the merc is repairing items 
	L"Радио", // Flugente: the merc is scanning for patrols in neighbouring sectors
	L"Осведомитель", // anv: snitch actions
	L"Обучение", // the merc is training
	L"Militia",		// all things militia
	L"Носильщик",	// move items
	L"Укреплять",		// fortify sector
	L"Intel", // covert assignments	// TODO.Translate
	L"Administer",	// TODO.Translate
	L"Explore",	// TODO.Translate
	L"Занятия", // the merc is using/staffing a facility
	L"Отмена", // cancel this menu
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"В атаку", // set militia to aggresive
	L"Держать оборону", // set militia to stationary
	L"Отступать", // retreat militia
	L"За мной",
	L"Ложись",
	L"Присесть",
	L"В укрытие",
	L"Двигаться в точку",
	L"Все в атаку", 
	L"Всем держать оборону",
	L"Всем отступать",
	L"Все за мной",
	L"Всем рассеяться",
	L"Всем залечь",
	L"Всем присесть",
	L"Всем в укрытие",
	//L"Всем искать предметы",
	L"Отмена", // cancel this menu
};

//Flugente
STR16 pTraitSkillsMenuStrings[] =
{
	// radio operator
	L"Артналет",
	L"Постановка помех",
	L"Сканировать радиочастоты",
	L"Прослушивать",
	L"Вызвать подкрепления",
	L"Выключить радиостанцию",
	L"Radio: Activate all turncoats",

	// spy
	L"Hide assignment",
	L"Get Intel assignment",
	L"Recruit turncoat",
	L"Activate turncoat",
	L"Activate all turncoats",

	// disguise
	L"Disguise",
	L"Remove disguise",
	L"Test disguise",
	L"Remove clothes",

	// various
	L"Наблюдатель",
	L"Focus",	// TODO.Translate
	L"Drag",
	L"Fill canteens",
};

//Flugente: short description of the above skills for the skill selection menu
STR16 pTraitSkillsMenuDescStrings[] =
{
	// radio operator
	L"Вызвать артиллерийский удар из сектора...",
	L"Заполнить эфир помехами, чтобы сделать невозможным использование средств связи.",
	L"Искать источник помех.",
	L"Использовать радиопрослушку для обнаружения противника.",
	L"Вызвать подкрепления из соседних секторов.",
	L"Turn off radio set.",	// TODO.Translate
	L"Order all previously turned soldiers in the sector to betray their comrades and join you.",

	// spy
	L"Assignment: hide among the population.",		// TODO.Translate
	L"Assignment: hide among the population and gather intel.",
	L"Try to turn an enemy into a turncoat.",
	L"Order previously turned soldier to betray their comrades and join you.",
	L"Order all previously turned soldiers in the sector to betray their comrades and join you.",

	// disguise
	L"Try to disguise with the merc's current clothes.",
	L"Remove the disguise, but clothes remain worn.",
	L"Test the viability of the disguise.",
	L"Remove any extra clothes.",

	// various
	L"Наблюдать за местностью, чтобы обеспечить более меткую стрельбу своим снайперам.",
	L"Increase interrupt modifier (penalty outside of area).",	// TODO.Translate
	L"Drag a person, corpse or structure while you move.",
	L"Refill your squad's canteens with water from this sector.",
};

STR16 pTraitSkillsDenialStrings[] =
{
	L"Требуется:\n",
	L" - %d ОД\n",
	L" - %s\n",
	L" - %s или выше\n",
	L" - %s или выше или\n",
	L" - %d минут на подготовку\n",
	L" - позиции миномётов в соседних секторах\n",
	L" - %s |и|л|и %s |и %s или %s или выше\n",
	L" - одержим бесами",
	L" - a gun-related trait\n",	// TODO.Translate
	L" - aimed gun\n",
	L" - prone person, corpse or structure next to merc\n",
	L" - crouched position\n",
	L" - free main hand\n",
	L" - covert trait\n",
	L" - enemy occupied sector\n",
	L" - single merc\n",
	L" - no alarm raised\n",
	L" - civilian or soldier disguise\n",
	L" - being our turn\n",
	L" - turned enemy soldier\n",
	L" - enemy soldier\n",
	L" - surface sector\n",
	L" - not being under suspicion\n",
	L" - not disguised\n",
	L" - not in combat\n",
	L" - friendly controlled sector\n",
};

STR16 pSkillMenuStrings[] =
{
	L"Ополчение",
	L"Другие отряды",
	L"Отмена",
	L"%d ополченцев",
	L"Все ополченцы",

	L"More",	// TODO.Translate
	L"Corpse: %s",	// TODO.Translate
};

STR16 pSnitchMenuStrings[] =
{
	// snitch
	L"Осведомитель в отряде",
	L"Городское назначение",	
	L"Отмена",
};

STR16 pSnitchMenuDescStrings[] =
{
	// snitch
	L"Обсудить поведение осведомителя по отношению к его команде.",
	L"Взять задание в этом секторе.",
	L"Отмена",
};

STR16 pSnitchToggleMenuStrings[] =
{
	// toggle snitching
	L"Сообщать о недовольстве",
	L"Не сообщать",
	L"Предотвращать нарушения",
	L"Игнорировать нарушения",
	L"Отмена",
};

STR16 pSnitchToggleMenuDescStrings[] =
{
	L"Сообщать командиру обо всех недовольных.",
	L"Ничего не сообщать.",
	L"Предотвращать попытки воровства и наркомании.",
	L"Не обращать внимания на нарушения.",
	L"Отмена",
};

STR16 pSnitchSectorMenuStrings[] =
{
	// sector assignments
	L"Вести пропаганду",
	L"Собирать слухи",
	L"Отмена",
};

STR16 pSnitchSectorMenuDescStrings[] =
{
	L"Прославлять действия наёмников для повышения лояльности. ",
	L"Собирать слухи о действиях противника.",
	L"",
};

STR16 pPrisonerMenuStrings[] =
{
	L"Допросить полицию",
	L"Допросить солдат",
	L"Допросить спецназ",
	L"Допросить офицеров",
	L"Допросить генералов",
	L"Допросить граждан",
	L"Отмена",
};

STR16 pPrisonerMenuDescStrings[] =	// TODO.Translate
{
	L"Administrators are easy to process, but give only poor results",
	L"Regular troops are common and don't give you high rewards.",
	L"If elite troops defect to you, they can become veteran militia.",
	L"Interrogating enemy officers can lead you to find enemy generals.",
	L"Generals cannot join your militia, but lead to high ransoms.",
	L"Civilians don't offer much resistance, but are second-rate troops at best.",
	L"Отмена",
};

STR16 pSnitchPrisonExposedStrings[] =
{
	L"%s был уличен как осведомитель, но вовремя сумел заметить это и выбраться живым.",
	L"%s был уличен как осведомитель, но сумел разрядить обстановку и выбраться живым.",
	L"%s был уличен как осведомитель, но сумел избежать покушения.",
	L"%s был уличен как осведомитель, но охрана сумела предотвратить насилие.",

	L"%s был уличен как осведомитель и почти утоплен другими заключенными, но охрана успела вмешаться.",
	L"%s был уличен как осведомитель и чуть не забит до смерти, прежде чем охрана вмешалась.",
	L"%s был уличен как осведомитель и едва не заколот, но охрана успела вмешаться.",
	L"%s был уличен как осведомитель и чуть не задушен, но охрана успела вмешаться.",

	L"%s был уличен как осведомитель и утоплен в сортире другими заключенными.",
	L"%s был уличен как осведомитель и забит до смерти другими заключенными.",
	L"%s был уличен как осведомитель и заколот насмерть другими заключенными.",
	L"%s был уличен как осведомитель и задушен другими заключенными.",
};

STR16 pSnitchGatheringRumoursResultStrings[] =
{
	L"%s собрал(а) слухи об активности противника в %d секторах.",

};

STR16 pRemoveMercStrings[] =
{
	L"Убрать бойца", // remove dead merc from current team
	L"Отмена",
};

STR16 pAttributeMenuStrings[] =
{
	L"Здоровье",
	L"Проворность",
	L"Ловкость",
	L"Сила",
	L"Лидерство",
	L"Меткость",
	L"Механика",
	L"Взрывчатка",
	L"Медицина",
	L"Отмена",
};

STR16 pTrainingMenuStrings[] =
{
	L"Практика", // train yourself 
	L"Рабочие",	// Train workers
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
	L"Боёв:", 		// number of battles fought
	L"Ранений:",		// number of times merc has been wounded
	L"Навыки:",
	L"Нет навыков",
	L"Достижения:", //Achievements
};

// SANDRO - helptexts for merc records
STR16 pPersonnelRecordsHelpTexts[] =
{
	L"Спецназа: %d\n",
	L"Солдат: %d\n",
	L"Полиции: %d\n",
	L"Враждебных граждан: %d\n",
	L"Животных: %d\n",
	L"Танков: %d\n", 
	L"Других объектов: %d\n",

	L"Своим: %d\n",
	L"Ополчению: %d\n",
	L"Другим: %d\n",

	L"Выпущено пуль: %d\n",
	L"Выпущено ракет: %d\n",
	L"Брошено гранат: %d\n",
	L"Брошено ножей: %d\n",
	L"Ударов ножом: %d\n",
	L"Ударов кулаками: %d\n",
	L"Удачных попаданий: %d\n",

	L"Замков взломано: %d\n",
	L"Замков сорвано: %d\n",
	L"Ловушек обезврежено: %d\n",
	L"Взрывчатки взорвано: %d\n",
	L"Предметов отремонтированно: %d\n",
	L"Предметов собрано: %d\n",
	L"Вещей украдено: %d\n",
	L"Ополченцев натренировано: %d\n",
	L"Бойцов перевязано: %d\n",
	L"Заданий: %d\n",
	L"Встречено информаторов: %d\n",
	L"Секторов разведано: %d\n",
	L"Предотвращено засад: %d\n",
	L"Заданий жителей выполнено: %d\n",

	L"Тактических сражений: %d\n",
	L"Автобитв: %d\n",
	L"Количество отступлений: %d\n",
	L"Попаданий в засады: %d\n",
	L"Крупнейшая битва: %d врагов\n",

	L"Огнестрельных ран: %d\n",
	L"Ножевых ран: %d\n",
	L"Пропущенных ударов: %d\n",
	L"Подорвался: %d\n",
	L"Ухудшений параметров: %d\n",
	L"Перенёс хир. операций: %d\n",
	L"Травм на производстве: %d\n",

	L"Характер:",
	L"Недостаток:",

	L"По жизни:",	//Attitudes // WANNE: For old traits display instead of "Character:"!

	L"Зомби: %d\n",

	L"Биография:",
	L"Характер:", 

	L"Допросил(а): %d\n",
	L"Заболел(а): %d\n",
	L"Получено урона: %d\n",
	L"Нанесено урона: %d\n",
	L"Вылечено: %d\n",
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = 
{
	// SANDRO - tweaked this
	L"Нет навыка",
	L"Взлом замков",
	L"Рукопашный бой",
	L"Электроника",
	L"Ночные операции", 
	L"Метание",
	L"Инструктор",
	L"Тяжелое оружие",
	L"Автоматическое оружие",
	L"Скрытность",
	L"Ловкач",
	L"Воровство",
	L"Боевые искусства",
	L"Холодное оружие",
	L"Снайпер", 
	L"Камуфляж",
	L"(Эксперт)",
};
//////////////////////////////////////////////////////////
// SANDRO - added this
STR16 gzMercSkillTextNew[] = 
{
	// Major traits
	L"Нет навыка",	// 0
	L"Автоматчик",	// 1
	L"Гренадёр",
	L"Стрелок",
	L"Охотник",
	L"Ковбой",	// 5
	L"Боксёр",
	L"Старшина",
	L"Техник",
	L"Санитар",
	// Minor traits
	L"Ловкач",	// 10
	L"Мастер клинка",
	L"Мастер по метанию",
	L"Ночник",
	L"Бесшумный убийца",
	L"Спортсмен",
	L"Культурист",
	L"Подрывник",
	L"Инструктор",
	L"Разведчик",
	// covert ops is a major trait that was added later
	L"Диверсант",		// 20

	// new minor traits
	L"Радист",	// 21
	L"Осведомитель",	// 22
	L"Спец по выживанию",

	// second names for major skills
	L"Пулемётчик",
	L"Артиллерист",
	L"Снайпер",
	L"Рейнджер",
	L"Пистолетчик",
	L"Боевые искусства",
	L"Командир",
	L"Инженер",
	L"Доктор",
	// placeholders for minor traits
	L"Placeholder",		// 33
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",		// 42
	L"Шпион",			// 43
	L"Placeholder",		// for radio operator (minor trait)
	L"Placeholder",		// for snitch (minor trait)
	L"Placeholder",		// for survival (minor trait)
	L"Ещё...",			// 47
	L"Intel",			// for INTEL	// TODO.Translate
	L"Disguise",		// for DISGUISE
	L"Различные",		// for VARIOUSSKILLS	
	L"Автоперевязка",	// for AUTOBANDAGESKILLS
};
//////////////////////////////////////////////////////////

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
	L"Выбрать наёмника",
	L"Отдать приказ",
	L"Проложить путь движения",
	L"Контракт наёмника (|C)",
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
	L"НЕПОНЯТНЫЙ",
	L"ШАГИ",
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
	L"АЗС  ",
	L"Убежище повстанцев",	//The rebel base underground in sector A10
	L"Подвалы Тиксы",	//The basement of the Tixa Prison (J9)
	L"Логово тварей",	//Any mine sector with creatures in it
	L"Подвалы Орты",	//The basement of Orta (K4)
	L"Туннель",		//The tunnel access from the maze garden in Meduna 
										//leading to the secret shelter underneath the palace
	L"Убежище",		//The shelter underneath the queen's palace
	L"",							//Unused
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
	L"ПЛЕНЁН!",					//STR_AR_OVER_CAPTURED
	L"ОТСТУПИЛ!",					//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"Ополченец",					//STR_AR_MILITIA_NAME,
	L"Спецназ",					//STR_AR_ELITE_NAME,
	L"Солдат",					//STR_AR_TROOP_NAME,
	L"Полиция",					//STR_AR_ADMINISTRATOR_NAME,
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
	L"ENEMY AIRDROP",					//STR_PB_ENEMYINVASION_AIRDROP_HEADER	// TODO.Translate

	//Various single words for direct translation.  The Civilians represent the civilian
	//militia occupying the sector being attacked.  Limited to 9-10 chars

	L"Сектор",
	L"Враг",
	L"Наёмники",
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
	L"Вы должны разместить всех своих бойцов \nперед тем, как начать бой.",

	//Various strings (translate word for word)

	L"Сектор",
	L"Выбор точек входа (используйте стрелки для скроллинга карты)",

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
	L"Враги атаковали ваших наёмников в секторе %s. Ни один из ваших бойцов не в состоянии сражаться!",
	//%s is the sector location -- ex:  A9: Omerta
	L"Твари атаковали ваших наёмников в секторе %s. Ни один из ваших бойцов не в состоянии сражаться!",

	// Flugente: militia movement forbidden due to limited roaming
	L"Ополчение не может быть сюда перемещено (RESTRICT_ROAMING = TRUE).",
	L"War room isn't staffed - militia move aborted!", 	// TODO.Translate

	L"Робот",								//STR_AR_ROBOT_NAME, TODO: translate
	L"Танк",								//STR_AR_TANK_NAME,
	L"Джип",								//STR_AR_JEEP_NAME

	L"\nВосстановление дыхания в час: %d",	// STR_BREATH_REGEN_SLEEP

	L"Zombies",		// TODO.Translate
	L"Bandits",
	L"BLOODCAT ATTACK",
	L"ZOMBIE ATTACK",
	L"BANDIT ATTACK",
	L"Zombie",
	L"Bandit",
	L"Bandits attack and kill %d civilians in sector %s.",
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
	L"День находки:",
};

//The headers used to describe various weapon statistics.

CHAR16		gWeaponStatsDesc[][ 20 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"Состояние:",
	L"Вес:", 
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
	// HEADROCK HAM 3: Replaced #14 with string for Possible Attachments (BR's Tooltips Only)
	// Obviously, THIS SHOULD BE DONE IN ALL LANGUAGES...
	L"Навеска:",	//14	//Attachments
	L"AUTO/5:",		//15
	L"Осталось патрон:",		//16	//Remaining ammo
	L"Предустановка:",	//17 //WarmSteel - So we can also display default attachments
	L"Нагар:",	// 18
	L"Место:", // 19 //space left on Molle items
	L"Разброс:",	// 20

};

// HEADROCK: Several arrays of tooltip text for new Extended Description Box
STR16		gzWeaponStatsFasthelpTactical[ 33 ] =
{
	L"|Д|а|л|ь|н|о|с|т|ь\n \nФактическая дальность стрельбы из этого оружия.\nСтрельба с расстояний, превышающих данный показатель,\nбудет осуществляться со значительными штрафами\nна точность.\n \nБольше - лучше.",
	L"|У|р|о|н\n \nПоказатель потенциального урона оружия.\nПопадание в незащищенную цель нанесет ей\nпримерно столько единиц урона.\n \nБольше - лучше.",
	L"|Т|о|ч|н|о|с|т|ь\n \nПоказатель исходного бонуса (или штрафа!)\nк точности, присущего этому оружию благодаря\nего удачному (или неудачному) дизайну.\n \nБольше - лучше.",
	L"|У|р|о|в|н|и |п|р|и|ц|е|л|и|в|а|н|и|я\n \nМаксимальное число кликов прицеливания,\nвозможных при использовании этого оружия.\n \nС каждым кликом прицеливания атака\nстановится более точной.\n \nБольше - лучше.",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|р|и|ц|е|л|и|в|а|н|и|я\n \nФиксированный модификатор, меняющий\nэффективность каждого клика прицеливания\nпри использовании этого оружия.\n \nБольше - лучше.",
	L"|М|и|н|. |р|а|с|с|т|о|я|н|и|е |д|л|я |б|о|н|у|с|а |п|р|и |п|р|и|ц|е|л|и|в|а|н|и|и\n \nМинимальное расстояние до цели, при котором\nк этому оружию может применяться\nмодификатор прицеливания.\n \nЕсли цель находится ближе, чем\nэтот показатель, то эффективность каждого\nклика прицеливания будет неизменной.\n \nМеньше - лучше.",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|о|п|а|д|а|н|и|я\n \nФиксированный модификатор шанса попадания\nпри любой атаке из этого оружия.\n \nБольше - лучше.",
	L"|О|п|т|и|м|а|л|ь|н|а|я |д|а|л|ь|н|о|с|т|ь |л|а|з|е|р|а\n \nДальность (в тайлах), в пределах которой\nлазерный целеуказатель, установленный на оружии,\nбудет работать с максимальной эффективностью.\n \nПри стрельбе с расстояний, превышающих данный показатель,\nцелеуказатель будет давать меньший бонус или\nне будет давать его вовсе.\n \nБольше - лучше.",
	L"|С|к|р|ы|т|а|я |в|с|п|ы|ш|к|а |в|ы|с|т|р|е|л|а\n \nПоявление этой иконки означает, что\nоружие не производит вспышки при выстреле.\nЭто позволяет стрелку оставаться незамеченным.",
	L"|Г|р|о|м|к|о|с|т|ь\n \nЭто расстояние в тайлах, на которое распространяется\nзвук стрельбы. В пределах этого расстояния\nвраги смогут услышать звук вашего выстрела.\n \nМеньше - лучше.",
	L"|Н|а|д|ё|ж|н|о|с|т|ь\n \nОпределяет, как быстро состояние этого\nоружия ухудшается при использовании.\n \nБольше - лучше.",
	L"|Л|ё|г|к|о|с|т|ь |п|о|ч|и|н|к|и\n \nОпределяет сложность починки этого оружия,\nа также то, кто сможет полностью починить его.\nЗеленый - может починить кто угодно.\n \nЖелтый - только техники и некоторые NPC\nмогут починить его свыше порога ремонта.\n \nКрасный - эту вещь починить невозможно.\n \nБольше - лучше.",
	L"",	//12
	L"ОД на вскидывание",
	L"ОД на 1 выстрел",
	L"ОД на огонь с отсечкой",
	L"ОД на огонь очередью",
	L"ОД на замену магазина",
	L"ОД на досылку патрона",
	L"Штраф за отдачу при\nстрельбе очередью c отсечкой\n(меньше - лучше)",	//19
	L"Бонус от сошек\n(при стрельбе лёжа)",
	L"Выстрелов в автоматическом\nрежиме за 5 ОД",
	L"Штраф за отдачу при \nстрельбе очередью \n(меньше - лучше)",
	L"Штраф за отдачу при\nстрельбе очередью\n(c отсечкой/без) (меньше - лучше)",	//23
	L"ОД на бросок",
	L"ОД на выстрел",
	L"ОД на удар ножом",
	L"Не стреляет одиночными!",
	L"Нет отсечки патрона!",
	L"Нет автоматического режима!",
	L"ОД на удар",
    L"",
	L"|Л|ё|г|к|о|с|т|ь |п|о|ч|и|н|к|и\n \nОпределяет сложность починки этого оружия,\nа также то, кто сможет полностью починить его.\nЗеленый - может починить кто угодно.\n \nЖелтый - только некоторые NPC\nмогут починить его свыше порога ремонта.\n \nКрасный - эту вещь починить невозможно.\n \nБольше - лучше.",
};

STR16		gzMiscItemStatsFasthelp[] =
{
	L"Модификатор размера предмета\n(меньше - лучше)", //0
	L"Модификатор надёжности",
	L"Модификатор шумности\n(меньше - лучше)",
	L"Скрывает вспышку",
	L"Модификатор сошек",
	L"Модификатор дальности", //5
	L"Модификатор точности",
	L"Оптимальная дальность лазера",
	L"Модификатор бонусов оптики",
	L"Модификатор очереди с отсечкой",
	L"Модификатор штрафа за отдачу\nпри стрельбе c отсечкой\n(больше - лучше)",	//10
	L"Модификатор штрафа за отдачу\nпри стрельбе очередью\n(больше - лучше)",
	L"Модификатор ОД",
	L"Модификатор ОД\nна очередь с отсечкой\n(меньше - лучше)",
	L"Модификатор ОД\nна очередь без отсечки\n(меньше - лучше)",
	L"Модификатор ОД на вскидывание\n(меньше - лучше)", //15
	L"Модификатор ОД\nна замену магазина\n(меньше - лучше)",
	L"Модификатор размера магазина",
	L"Модификатор ОД на выстрел\n(меньше - лучше)",
	L"Модификатор урона",
	L"Модификатор урона\nв ближнем бою", //20
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
	L"Туннельное зрение\n(меньше - лучше)",
	L"Минимальное расстояние\nдля бонуса при прицеливании",
	L"Зажмите |C|t|r|l для сравнения предметов",									// item compare help text 
	L"Equipment weight: %4.1f kg",	// 35	// TODO.Translate
};

// HEADROCK: End new tooltip text

// HEADROCK HAM 4: New condition-based text similar to JA1.
STR16 gConditionDesc[] =
{
	L"В ",	//In 
	L"ИДЕАЛЬНОМ",
	L"ОТЛИЧНОМ",
	L"ХОРОШЕМ",	//GOOD
	L"НОРМАЛЬНОМ",	//FAIR
	L"ПЛОХОМ",	//POOR
	L"УЖАСНОМ",	//BAD
	L"НЕРАБОЧЕМ",
	L" состоянии."
};

//The headers used for the merc's money.

CHAR16 gMoneyStatsDesc[][ 14 ] =
{
	L"Кол-во",
	L"Осталось:", //this is the overall balance
	L"Кол-во",
	L"Отделить:", //the amount he wants to separate from the overall balance to get two piles of money

	L"Текущий",
	L"Баланс:",
	L"Снимаемая",
	L"Сумма:",
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
	L"ЗАХВАЧЕН",
};

STR16 gzHiddenHitCountStr[1] =
{
	L"?",	
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
 L"Вертолёт",
};

STR16 pShortVehicleStrings[] =
{
	L"Эльдорадо",
	L"Хаммер",			// the HMVV
	L"Фургон",
	L"Джип",
	L"Танк",
	L"Вертолёт", 				// the helicopter
};

STR16	zVehicleName[] =
{
	L"Эльдорадо",
	L"Хаммер",		//a military jeep. This is a brand name.
	L"Фургон",			// Ice cream truck
	L"Джип",
	L"Танк",
	L"Вертолёт", 		//an abbreviation for Helicopter
};

STR16 pVehicleSeatsStrings[] =
{
	 L"Вы не можете стрелять с этого места.",
	 L"Вы не можете поменять их местами во время боя, не выходя из автомобиля.",
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
	L"Наденьте на голову одного из наёмников пульт управления роботом.",
	L"Последняя очередь опустошила магазин!",
	L"СОЛДАТ",
	L"РЕПТИОН",
	L"ОПОЛЧЕНЕЦ",
	L"ЖИТЕЛЬ",
	L"ЗОМБИ",
	L"Пленный",
	L"Выход из сектора",
	L"ДА",
	L"ОТМЕНА",
	L"Выбранный боец",
	L"Все бойцы отряда",
	L"Идти в сектор",
	L"Идти на карту",
	L"Этот сектор отсюда покинуть нельзя.",
	L"Вы не можете покинуть сектор в походовом режиме.",
	L"%s слишком далеко.",
	L"Скрыть кроны деревьев",
	L"Показать кроны деревьев",
	L"ВОРОНА",				//Crow, as in the large black bird
	L"ШЕЯ",
	L"ГОЛОВА",
	L"ТОРС",
	L"НОГИ",
	L"Рассказать королеве то, что она хочет знать?",
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
	L"Согласиться выйти замуж за %s?",	//Daryl
	L"Связка ключей",
	L"С эскортируемыми этого сделать нельзя.",
	L"Пощадить %s?",	//Krott
	L"За пределами прицельной дальности.",
	L"Шахтер",
	L"Машина может ездить только между секторами.",
	L"Ни у кого из наёмников нет аптечки",
	L"Путь для %s заблокирован",
	L"Ваши бойцы, захваченные армией %s, томятся здесь в плену!", //Deidranna
	L"Замок поврежден.",
	L"Замок разрушен.",
	L"Кто-то с другой стороны пытается открыть эту дверь.",
	L"Состояние: %d/%d\nТопливо: %d/%d",
	L"%s не видит %s.",  // Cannot see person trying to talk to
	L"Навеска снята",
	L"Вы не можете содержать еще одну машину, довольствуйтесь уже имеющимися двумя.",

	// added by Flugente for defusing/setting up trap networks
	L"Выберите частоту активации (1 - 4) или частоту деактивации (A - D):",
	L"Установите частоту деактивации:",
	L"Установите частоту активации (1 - 4) и частоту деактивации (A - D):",
	L"Установите время до активации в ходах (1 - 4) и частоту деактивации (A - D):",
	L"Выберите уровень (1 - 4) и сеть (A - D):",
	
	// added by Flugente to display food status
	L"Здоровье: %d/%d\nЭнергия: %d/%d\nБоевой дух: %s\nВода: %d%s\nПища: %d%s",

	// added by Flugente: selection of a function to call in tactical
	L"Что будем делать?",
	L"Наполнить фляги",
	L"Чистить (один)",
	L"Чистить (все)",
	L"Убрать одежду",
	L"Убрать маскировку",
	L"Ополчение: бросить оружие",
	L"Ополчение: взять оружие",
	L"Проверить маскировку",
	L"",

	// added by Flugente: decide what to do with the corpses
	L"Что будем делать с телом?",
	L"Отсечь голову",
	L"Потрошить",
	L"Забрать одежду",
	L"Забрать тело",

	// Flugente: weapon cleaning
	L"%s чистит %s",

	// added by Flugente: decide what to do with prisoners
	L"As we have no prison, a field interrogation is performed.",		// TODO.Translate
	L"Field interrogation",
	L"Куда отправить %d пленного(-иков)?",
	L"Отпустить",
	L"Что вы хотите сделать?",
	L"Требовать сдаться",
	L"Предложить сдаться",
	L"Distract",	// TODO.Translate
	L"Переговоры",
	L"Recruit Turncoat", // TODO: translate

    // added by sevenfm: disarm messagebox options, messages when arming wrong bomb
    L"Обезвредить",
    L"Исследовать",
    L"Убрать флаг",
    L"Взорвать!",
    L"Активировать сеть",
    L"Деактивировать сеть",
    L"Показать сеть",
    L"Нет детонатора!",
    L"Эта бомба уже взведена!",
    L"Безопасно",
    L"Относительно безопасно",
    L"Рискованно",
    L"Опасно",
    L"Очень опасно!",

    L"Маска",
    L"ПНВ",
    L"Предмет",
        
    L"Эта функция работает только с новой системой инвентаря (NIV)",
    L"Нет предметов в главной руке",
    L"Некуда поместить предмет",
    L"Для этого быстрого слота не определены предметы",
    L"Нет свободной руки для предмета",
    L"Предмет не обнаружен",
    L"Невозможно взять предмет в руку",

	L"Перевязка идуших бойцов...",

	L"Комплектовать",
	L"%s заменил %s на лучшую версию",
	L"%s поднял %s",

	L"%s has stopped chatting with %s", // TODO.Translate
	L"Attempt to turn",
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
	L"Как только оставшиеся наёмники покинут этот сектор,\nсразу будет загружен соседний сектор.",
	L"Выведя оставшихся наёмников из этого сектора,\nвы автоматически попадете на экран карты,\nтак как на путешествие потребуется некоторое время.",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted.  The "single" button is disabled.
	L"%s нуждается в сопровождении ваших наёмников и не может самостоятельно покинуть сектор.",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s не может покинуть сектор один, так как он сопровождает %s.", //male singular
	L"%s не может покинуть сектор одна, так как она сопровождает %s.", //female singular
	L"%s не может покинуть сектор один, так как он сопровождает группу из нескольких человек.", //male plural
	L"%s не может покинуть сектор одна, так как она сопровождает группу из нескольких человек.", //female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the  "move all" option is disabled,
	//and this helptext explains why.
	L"Все ваши наёмники должны быть в машине,\nчтобы отряд смог отправиться в место назначения.",

	L"", //UNUSED

	//Standard helptext for single movement.  Explains what will happen (splitting the squad)
	L"Если выбрать, то %s отправится в одиночку\nи автоматически будет переведен в отдельный отряд.",

	//Standard helptext for all movement.  Explains what will happen (moving the squad)
	L"Если выбрать, данный отряд отправится\nв место назначения, покинув этот сектор.",

	//This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear.  This is just like the situation where
	//This string is special, as it is not used as helptext.  Do not use the special newline character (\n) for this string.
	L"%s сопровождается вашими наёмниками и не может покинуть этот сектор в одиночку. Остальные наёмники должны быть рядом, прежде чем вы сможете покинуть сектор.",
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
	L"ед.",	// singular
	L"ед.",	// plural
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
	L"РВП:", 			// ETA is an acronym for "estimated time of arrival" 
	L"У вертолета закончилось топливо. Придется совершить посадку на вражеской территории!",	// warning that the sector the helicopter is going to use for refueling is under enemy control -> 
	L"Пассажиры:",
	L"Выбрать вертолет или точку высадки?",
	L"Вертолёт",
	L"Высадка",
	L"Вертолет серьезно поврежден и идет на вынужденную посадку во вражеской территории!",	// warning that the sector the helicopter is going to use for refueling is under enemy control ->
	L"Вертолет возвращается на базу, высадить сначала пассажиров?",
	L"Остаток топлива:",
	L"Расст. до заправки:",
};

STR16 pHelicopterRepairRefuelStrings[]=
{
	// anv: Waldo The Mechanic - prompt and notifications
	L"Хотите, чтобы %s начал ремонт? Это будет стоить $%d, вертолет не будет доступен в течение %d часов.",
	L"Вертолет разобран. Подождите, пока не закончится его ремонт.",
	L"Ремонт закончен. Вертолет снова доступен.",
	L"Вертолет полностью заправлен.",

	L"Helicopter has exceeded maximum range!",	// TODO.Translate
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
	L"Завод",	//Factory
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
	L"Ремонт",
	L"Factories",	// TODO.Translate
};

// town strings

STR16 pwTownInfoStrings[] =
{
	L"Размер",						// 0 // size of the town in sectors
	L"", 						// blank line, required
	L"Контроль", 					// how much of town is controlled
	L"Нет", 						// none of this town
	L"Шахта города", 				// mine associated with this town
	L"Лояльность",					// 5 // the loyalty level of this town
	L"Готовы", 						// the forces in the town trained by the player
	L"",
	L"Важные объекты", 				// main facilities in this town
	L"Уровень", 					// the training level of civilians in this town
	L"Тренировка ополчения",		// 10 // state of civilian training in town
	L"Ополчение", 					// the state of the trained civilians in the town

	// Flugente: prisoner texts
	L"Заключенные",
	L"%d (вместимость %d)",
	L"%d Полиция",
	L"%d Солдаты",
	L"%d Спецназ",
	L"%d Офицеры",
	L"%d Генералы",
	L"%d Гражданские",
	L"%d Special1",
	L"%d Special2",
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
	L"Ресурс",					// 10
	L"Принадлежность",
	L"Лояльность",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"Силы врага",
	L"Сектор",
	L"Количество предметов",
	L"Неизвестно",

	L"Под контролем",
	L"Да",
	L"Нет",
	L"Status/Software status:",	// TODO.Translate

	L"Additional Intel", // TODO:Translate
};

// error strings for inventory

STR16 pMapInventoryErrorString[] =
{
	L"%s слишком далеко.",	//Merc is in sector with item but not close enough
	L"Нельзя выбрать этого бойца.",  //MARK CARTER
	L"%s вне этого сектора, и не может подобрать предмет.",
	L"Во время боя вам придется подбирать вещи с земли.",
	L"Во время боя вам придется выкладывать вещи на землю на тактической карте.",
	L"%s вне этого сектора и не может оставить предмет.",
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
	L"Чтобы перевести наёмника в другой отряд, назначить его врачом или отдать приказ ремонтировать вещи, щелкните по колонке 'ЗАНЯТИЕ'.",
	L"Чтобы приказать наёмнику перейти в другой сектор, щелкните в колонке 'КУДА'.",
	L"Как только наёмник получит приказ на передвижение, включится сжатие времени.",
	L"Нажатием левой кнопки мыши выбирается сектор. Еще одно нажатие нужно, чтобы отдать наёмникам приказы на передвижение. Нажатие правой кнопки мыши на секторе откроет экран дополнительной информации.",
	L"Чтобы вызвать экран помощи, в любой момент времени нажмите 'h'.",
	L"Тестовый текст",
	L"Тестовый текст",
	L"Тестовый текст",
	L"Тестовый текст",
	L"Вы практически ничего не сможете сделать на этом экране, пока не прибудете в Арулько. Когда познакомитесь со своей командой, включите сжатие времени (кнопки в правом нижнем углу). Это ускорит течение времени, пока ваша команда не прибудет в Арулько.",
};

// movement menu text

STR16 pMovementMenuStrings[] =
{
	L"Отправить наёмников в сектор", 	// title for movement box 
	L"Путь", 		// done with movement menu, start plotting movement
	L"Отмена", 		// cancel this menu
	L"Другое",		// title for group of mercs not on squads nor in vehicles
	L"Выбрать все",	// Select all squads
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
	L"Show |Disease Data",	// TODO.Translate
	L"Show Weathe|r",
	L"Show |Quests & Intel",
};

STR16 pMapScreenInvenButtonHelpText[] =
{
	L"Следующая \nстраница (|.)",		// next page
	L"Предыдущая \nстраница (|,)",		// previous page
	L"Закрыть инвентарь сектора (|E|s|c)",	// exit sector inventory
	L"Увеличить предметы", // HEAROCK HAM 5: Inventory Zoom Button
	L"Сложить и объединить предметы", // HEADROCK HAM 5: Stack and Merge
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Собрать патроны в ящики\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Собрать патроны в коробки", // HEADROCK HAM 5: Sort ammo
	// 6 - 10
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Снять всю навеску \nс предметов\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: empty LBE in sector", // HEADROCK HAM 5: Separate Attachments
	L"Разрядить всё оружие", //HEADROCK HAM 5: Eject Ammo
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Показать все предметы\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Скрыть все предметы", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть оружие\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать оружие", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть аммуницию\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать аммуницию", // HEADROCK HAM 5: Filter Button
	// 11 - 15
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть взрывчатку\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать взрывчатку", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть холодное оружие\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать холодное оружие", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть броню\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать броню", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть разгрузочные системы\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать разгрузочные системы", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть наборы\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать наборы", // HEADROCK HAM 5: Filter Button
	// 16 - 20
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть прочие предметы\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать прочие предметы",	// HEADROCK HAM 5: Filter Button
	L"Переключить отображение перемещения предметов",											// Flugente: move item display
	L"Save Gear Template",	// TODO.Translate
	L"Load Gear Template...",
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

CHAR16 pSenderNameList[500][128] =
{
	L"",
};
/*
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
	L"Грунти (Хряп)",
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
	L"A.I.M.",
};
*/

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
	L"День", 			// the day column
	L"Доход", 			// the credits column
	L"Расход",			// the debits column
	L"Операции", 		// transaction type - see TransactionText below
	L"Баланс", 			// balance at this point in time
	L"Стр.", 			// page number
	L"Дней", 			// the day(s) of transactions this page displays 
};


STR16 pTransactionText[] =
{
	L"Начисленный процент",			// interest the player has accumulated so far
	L"Анонимный взнос",
	L"Перевод средств", 
	L"Нанят", 				// Merc was hired
	L"Покупки у Бобби Рэя", 		// Bobby Ray is the name of an arms dealer
	L"Оплата счёта M.E.R.C.",
	L"%s: страховка.", 		// medical deposit for merc
	L"I.M.P.: Анализ профиля", 		// IMP is the acronym for International Mercenary Profiling
	L"%s: куплена страховка", 
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
	L"%s берёт наличные.", 			// transfer funds to a merc
	L"%s: переводит деньги.", 		// transfer funds from a merc
	L"%s: найм ополчения.", // initial cost to equip a town's militia
	L"%s продал вам вещи.",	//is used for the Shop keeper interface.  The dealers name will be appended to the end of the string.
	L"%s кладёт наличные на счет.",
	L"Снаряжение продано населению",
	L"Оснащение персонала", // HEADROCK HAM 3.6		//Facility Use
	L"Содержание ополчения", // HEADROCK HAM 3.6		//Militia upkeep
	L"Выкуп за освобожденных заключенных",	// Flugente: prisoner system
	L"ВОЗ, подписка",	// Flugente: disease
	L"Оплата услуг Цербер",	// Flugente: PMC
	L"Стоимость ремонта базы ПВО", // Flugente: SAM repair
	L"Trained workers", // Flugente: train workers	// TODO.Translate
	L"Drill militia in %s",	// Flugente: drill militia	// TODO.Translate
	L"Mini event", // rftr: mini events // TODO: translate
	L"Funds transferred from rebel command", // rftr: rebel command
	L"Funds transferred to rebel command", // rftr: rebel command
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
	L"У Небесного Всадника нет пассажиров. Если вы хотите переправить бойцов в этот сектор, посадите их в вертолёт (приказ \"Транспорт/Вертолёт\")."
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
	L"%s оставляет свою экипировку в Омерте (A9).",
	L"%s оставляет свою экипировку в Драссене (B13).",
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] =
{
	L"Здоровье",
	L"Энергия",
	L"Боевой дух",
	L"Состояние",	// the condition of the current vehicle (its "health")
	L"Бензин",	// the fuel level of the current vehicle (its "energy")
	L"Яд",		// for display of poisoning
	L"Вода",		// drink level
	L"Пища",		// food level
};


STR16 pMapScreenPrevNextCharButtonHelpText[] =
{
	L"Предыдущий боец\n(|С|т|р|е|л|к|а |В|л|е|в|о)", 			// previous merc in the list
	L"Следующий боец\n(|С|т|р|е|л|к|а |В|п|р|а|в|о)", 			// next merc in the list
};


STR16 pEtaString[] =
{
	L"РВП:", 				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"Вы больше никогда не увидите этот предмет. Уверены?", 	// do you want to continue and lose the item forever
	L"Этот предмет кажется ОЧЕНЬ важным. Вы ДЕЙСТВИТЕЛЬНО хотите выбросить его?", // does the user REALLY want to trash this item
};


STR16 pMapErrorString[] = 
{
	L"Отряд не может выйти на марш, когда один из наёмников спит.",

//1-5
	L"Сначала выведите отряд на поверхность.",
	L"Выйти на марш? Да тут же враги повсюду!",
	L"Чтобы выйти на марш, наёмники должны быть назначены в отряд или посажены в машину.",
	L"У вас еще нет ни одного бойца.",	// you have no members, can't do anything 
	L"наёмник не может выполнить.",		// merc can't comply with your order
//6-10
	L"нуждается в сопровождении, чтобы идти. Назначьте его с кем-нибудь в отряд.", // merc can't move unescorted .. for a male
	L"нуждается в сопровождении, чтобы идти. Назначьте ее с кем-нибудь в отряд.", // for a female
	L"Наёмник ещё не прибыл в %s!",
	L"Кажется, сначала надо уладить проблемы с контрактом.",
	L"Бежать от самолета? Только после вас!",
//11-15
	L"Выступить на марш? Да у нас тут бой идет!",
	L"Вы попали в засаду кошек-убийц в секторе %s!",
	L"Вы только что вошли в сектор %s... И наткнулись на логово кошек-убийц!", 
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
	L"Вы не можете переназначить наёмников на марше.",
	L"У машины закончился бензин!",
	L"%s еле волочит ноги и идти не может.",
	L"Ни один из пассажиров не в состоянии вести машину.",
	L"Один или несколько наёмников из этого отряда не могут сейчас двигаться.",
//46-50
	L"Один или несколько наёмников не могут сейчас двигаться.",
	L"Машина сильно повреждена!",
	L"Внимание! Тренировать ополченцев в одном секторе могут не более двух наёмников.",
	L"Роботом обязательно нужно управлять. Назначьте наёмника с пультом и робота в один отряд.",
	L"Невозможно переместить вещи в %s, непонятно куда складывать. Зайдите в сектор, чтобы исправить.",
// 51-55
	L"%d items moved from %s to %s",
};


// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"Ещё раз щелкните по точке назначения, чтобы подтвердить путь, или щелкните по другому сектору, чтобы установить больше путевых точек.",
	L"Путь движения подтверждён.",
	L"Точка назначения не изменена.",
	L"Путь движения отменён.",
	L"Путь сокращен.",
};


// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"Выберите сектор, в который прибудут наёмники.",
	L"Вновь прибывшие наёмники высадятся в %s.",
	L"Высадить здесь наёмников нельзя. Воздушное пространство небезопасно!",
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
	L"Должен ли %s оставить свою амуницию здесь (в %s) или позже (в %s) перед отлетом?",
	L"%s скоро уходит и оставит свою амуницию в %s.",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"Должна ли %s оставить свою амуницию здесь (в %s) или позже (в %s) перед отлетом?",
	L"%s скоро уходит и оставит свою амуницию в %s.",
};


STR16 pMercContractOverStrings[] =
{
	L"отправляется домой, так как время его контракта истекло.", 		// merc's contract is over and has departed
	L"отправляется домой, так как время её контракта истекло.", 		// merc's contract is over and has departed
	L"уходит, так как его контракт был прерван.", 		// merc's contract has been terminated
	L"уходит, так как ее контракт был прерван.",		// merc's contract has been terminated
	L"Вы должны M.E.R.C. слишком много денег, так что %s уходит.", // Your M.E.R.C. account is invalid so merc left
};

// Text used on IMP Web Pages

// WDS: Allow flexible numbers of IMPs of each sex
//    note: I only updated the English text to remove "three" below
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
	L"Вы не можете позволить себе такой I.M.P.-персонаж.",	// 10
	L"Новый I.M.P.-персонаж присоединился к команде.",
	L"Вы уже выбрали максимальное количество навыков.",
	L"Голоса не найдены.",
};


// button labels used on the IMP site

STR16 pImpButtonText[] =
{
	L"Информация о нас", 		// about the IMP site
	L"НАЧАТЬ", 			// begin profiling
	L"Способности", 		//Skills
	L"Характеристики", 		// personal stats/attributes section
	L"Внешность", 			// Appearance
	L"Голос: %d", 			// the voice selection
	L"Готово", 			// done profiling
	L"Начать сначала", 		// start over profiling
	L"Да, я выбираю отмеченный ответ.",
	L"Да", 
	L"Нет",
	L"Готово", 			// finished answering questions
	L"Назад", 			// previous question..abbreviated form
	L"Дальше", 			// next question
	L"ДА.", 			// yes, I am certain 
	L"НЕТ, Я ХОЧУ НАЧАТЬ СНОВА.",	// no, I want to start over the profiling process
	L"ДА",
	L"НЕТ",
	L"Назад", 			// back one page
	L"Отменить", 			// cancel selection
	L"Да, всё верно.",
	L"Нет, ещё раз взгляну.",
	L"Регистрация", 		// the IMP site registry..when name and gender is selected
	L"Анализ данных", 		// analyzing your profile results
	L"Готово",
	L"Личные качества", 			// Character
	L"Нет",
};

STR16 pExtraIMPStrings[] =
{
	// These texts have been also slightly changed
	L"Теперь, когда формирование внешности и личных качеств завершено, укажите ваши способности.",	//With your character traits chosen, it is time to select your skills.
	L"Для завершения выберите свои характеристики.",	//To complete the process, select your attributes.
	L"Для начала подберите наиболее подходящее вам лицо, голос, телосложение и соответствующую расцветку.",	//To commence actual profiling, select portrait, voice and colors.
	L"Теперь, когда вы завершили формирование своей внешности, перейдём к анализу ваших личных качеств.",	//Now that you have completed your appearence choice, proceed to character analysis.
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

// Externalized to "TableData\History.xml"
/*
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
	L"Встретили Девина - торговца взрывчаткой.",
	L"Пересеклись с бывшим наёмником A.I.M., Майком!", 
	L"Встретили Тони, торговца оружием.",
	L"Получена ракетная винтовка от сержанта Кротта.",
	L"Документы на магазин Энжела переданы Кайлу.",
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
	L"%s: дисквалификация в бое без правил.",
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
	//76-80
	L"Убийца ушёл.",
	L"%s убит(а) вашим отрядом.",
	L"Встретили Вальдо, авиатехника.",
	L"Начат ремонт вертолета. Будет закончен через %d часов.",
};
*/

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
	L"M.E.R.C.",
	L"Похороны",
	L"Цветы",
	L"Страховка",
	L"Отмена",
	L"Энциклопедия",
	L"Брифинг-зал",
	L"Пресс-служба",
	L"БоЛТиК",
	L"ВОЗ",
	L"Цербер",
	L"Ополчение",
	L"R.I.S.",
	L"Factories",	// TODO.Translate
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
	L"Параметры", 			// view stats of the merc
	L"Отношения",
	L"Контракт",
	L"Снаряжение", 			// view the inventory of the merc
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
	L"A.I.M.",
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
	L"Бобби Рэй - разное",					//misc is an abbreviation for miscellaneous
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
	L"%s Пресс-служба - Сводка по конфликту",
	L"%s Пресс-служба - Боевые отчеты",
	L"%s Пресс-служба - Последние новости",
	L"%s Пресс-служба - О нас",
	L"\"Бойцы Любят Тебя или Клянут\" О нас",
	L"\"Бойцы Любят Тебя или Клянут\" Анализ команды",
	L"\"Бойцы Любят Тебя или Клянут\" Парные сравнения",
	L"\"Бойцы Любят Тебя или Клянут\" Комментарии",
	L"ВОЗ",
	L"ВОЗ - Заболевания в Арулько",
	L"ВОЗ - Полезно знать",
	L"Цербер",
	L"Цербер - Найм команды",
	L"Цербер - Индивидуальные контракты",
	L"Militia Overview",			// TODO.Translate
	L"Recon Intelligence Services - Information Requests",		// TODO.Translate
	L"Recon Intelligence Services - Information Verification",
	L"Recon Intelligence Services - About us",
	L"Factory Overview",	// TODO.Translate
	L"Бобби Рэй - последние поступления",
	L"Энциклопедия",
	L"Энциклопедия - данные",
	L"Брифинг-зал",
	L"Брифинг-зал - данные",
};

STR16 pShowBookmarkString[] =
{
	L"Подсказка",
	L"Щёлкните ещё раз по кнопке \"Сайты\" для открытия списка.",
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
	L"Вышла замуж",
	L"Контракт истёк",
	L"Выход",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] =
{
	L"В команде",
	L"Выбывшие",
	L"Гонорар за день:",
	L"Наибольший гонорар:",
	L"Наименьший гонорар:",
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
	L"Наёмник",
	L"Дней",
	L"Ставка",
	L"Стоимость",
	L"Всего:",
	L"Вы подтверждаете платеж в размере %s?",		//the %s is a string that contains the dollar amount ( ex. "$150" )
	L"%s (+снаряжение)",
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
	L"Оплата",
	L"в день",
	L"Снаряж.:",
	L"Всего:",
	L"Погиб",

	L"У вас уже полная команда из наёмников.",
	L"Со снаряжением?",
	L"Недоступно",
	L"Неоплаченные счета",
	L"Информация",
	L"Снаряжение",
};



// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16			MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"Открыть счет",
	L"Отмена",
	L"Вы ещё не зарегистрировались. Желаете открыть счёт?",
};



// For use at the M.E.R.C. web site. MERC Homepage

STR16			MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Спек Т. Кляйн, основатель и хозяин",
	L"Открыть счёт",
	L"Просмотр счёта",
	L"Просмотр файлов",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",
	L"Денежный перевод не состоялся. Недостаточно средств на счету.",
};

// For use at MiGillicutty's Web Page.

STR16			sFuneralString[] =
{
	L"Похоронное агентство Макгилликатти: скорбим вместе с семьями усопших с 1983 г.",
	L"Директор по похоронам и бывший наёмник A.I.M. - Мюррей Макгилликатти \"Папаша\", специалист по части похорон.",
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
	L"Мы искренне сочувствуем вам в это трудное время. Заходите ещё.",
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
	L"Цена:",
	L"Номер заказа:",
	L"Доставить",
	L"Завтра",
	L"Как будете в тех краях",
	L"Место доставки",
	L"Дополнительно",
	L"Сломать цветы ($10)",
	L"Черные розы ($20)",
	L"Увядший букет ($10)",
	L"Фруктовый пирог (если есть) ($10)",
	L"Текст поздравления:",
	L"Ввиду небольшого размера открытки, постарайтесь уложиться в 75 символов.",
	L"...или выберите одну из",

	L"СТАНДАРТНЫХ ОТКРЫТОК",
	L"Информация о счете",

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
	L"Бланк заказа",		//Title of the page
	L"Штк",					// The number of items ordered
	L"Вес (%s)",			// The weight of the item
	L"Название",			// The name of the item
	L"цена 1 вещи",			// the item's weight
	L"Итого",				// The total price of all of items of the same type
	L"Стоимость",			// The sub total of all the item totals added
	L"ДиУ (см. Место Доставки)",		// S&H is an acronym for Shipping and Handling 
	L"Всего",				// The grand total of all item totals + the shipping and handling
	L"Место доставки",
	L"Скорость доставки",	// See below
	L"Цена (за %s.)",		// The cost to ship the items
	L"Экспресс-доставка",	// Gets deliverd the next day
	L"2 рабочих дня",		// Gets delivered in 2 days
	L"Обычная доставка",	// Gets delivered in 3 days
	L"ОЧИСТИТЬ",//15			// Clears the order page
	L"ЗАКАЗАТЬ",			// Accept the order
	L"Назад",				// text on the button that returns to the previous page
	L"В начало",			// Text on the button that returns to the home page
	L"* - вещи, бывшие в употреблении",		// Disclaimer stating that the item is used
	L"Вы не можете это оплатить.",		//20	// A popup message that to warn of not enough money
	L"<НЕ ВЫБРАНО>",					// Gets displayed when there is no valid city selected
	L"Вы действительно хотите отправить груз в %s?",		// A popup that asks if the city selected is the correct one
	L"Вес груза**",			// Displays the weight of the package
	L"** Мин. вес",			// Disclaimer states that there is a minimum weight for the package
	L"Заказы",
};

STR16			BobbyRFilter[] =
{
	// Guns
	L"Пистолеты",
	L"Авт.пистол.",
	L"ПП",
	L"Винтовки",
	L"Сн.винтовки",
	L"Автоматы",
	L"Пулемёты",
	L"Дробовики",
	L"Тяжелое",

	// Ammo
	L"Пистолеты",
	L"Авт.пистол.",
	L"ПП",
	L"Винтовки",
	L"Сн.винтовки",
	L"Автоматы",
	L"Пулемёты",
	L"Дробовики",

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
	L"Прицелы", // Madd: new BR filters
	L"Рукоят/ЛЦУ",
	L"Дул.насад.",
	L"Приклады",
	L"Маг./спуск.",
	L"Др. навеска",
	L"Разное",
};


// This text is used when on the various Bobby Ray Web site pages that sell items

STR16			BobbyRText[] = 
{
	L"Заказать",		// Title
	// instructions on how to order
	L"Нажмите на товар. Левая кнопка - добавить, правая кнопка - уменьшить. После того как выберете товар, оформите заказ.",

	//Text on the buttons to go the various links

	L"Назад",
	L"Оружие",
	L"Патроны",
	L"Броня",
	L"Разное",		//misc is an abbreviation for miscellaneous
	L"Б/У",
	L"Далее",
	L"БЛАНК ЗАКАЗА",
	L"В начало",

	//The following 2 lines are used on the Ammunition page.  
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"У вашей команды есть",
	L"оружее, использующее этот тип боеприпасов",

	//The following lines provide information on the items

	L"Вес:",			// Weight of all the items of the same type
	L"Кал.:",			// the caliber of the gun
	L"Маг:",			// number of rounds of ammo the Magazine can hold
	L"Дист:",			// The range of the gun
	L"Урон:",			// Damage of the weapon
	L"Скор:",			// Weapon's Rate Of Fire, acronym ROF
	L"ОД:",				// Weapon's Action Points, acronym AP
	L"Оглушение:",		// Weapon's Stun Damage
	L"Броня:",			// Armour's Protection
	L"Камуф.:",			// Armour's Camouflage
	L"Armor Pen:",		// Ammo's Armour Piercing modifier (see AmmoTypes.xml - armourImpactReduction)
	L"Dmg Mod:",		// Ammo's Bullet Tumble modifier (see AmmoTypes.xml - afterArmourDamage)
	L"Projectiles:",	// Ammo's bullet count (for buckshot) (see AmmoTypes.xml - numberOfBullets)
	L"Цена:",			// Cost of the item
	L"Склад:",			// The number of items still in the store's inventory
	L"Штук в заказе:",	// The number of items on order
	L"Урон:",			// If the item is damaged
	L"Вес:",			// the Weight of the item
	L"Итого:",			// The total cost of all items on order
	L"* %% до износа",	// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	
	L"Чёрт! В эту форму можно внести не более " ,//First part
	L" позиций для одного заказа. Если хотите заказать больше (а мы надеемся, вы хотите), то заполните еще один заказ и примите наши извинения за неудобства.", 

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
	L"БОЕПРИПАСЫ",
	L"БРОНЯ",

	//Details on the web site

	L"Если у нас чего-то нет, то этого нет нигде!",
	L"В разработке",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"A.I.M. Состав",		// Title
	// Title for the way to sort
	L"Сортировка:",

	// sort by...

	L"Цена",
	L"Опыт",
	L"Меткость",
	L"Механика",
	L"Взрывчатка",
	L"Медицина",
	L"Здоровье",
	L"Проворность",
	L"Ловкость",
	L"Сила",
	L"Лидерство",
	L"Интеллект",
	L"Имя",

	//Text of the links to other AIM pages

	L"Фотографии наёмников",
	L"Информация о наёмниках",
	L"Архив A.I.M.", 

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
	L"Левый щелчок",
	L"связаться с бойцом.",
	L"Правый щелчок - ",
	L"фотографии бойцов.",
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
	L"Медицина",

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

	L"Дополнительная информация",		// Title for the additional info for the merc's bio
	L"Действующий состав",				// Title of the page
	L"Снаряжение:",						// Displays the optional gear cost
	L"Снаряж.",				//"gear",	//tais: Displays the optional gear cost in nsgi, this moved and can have only a small room, so just make it "gear" without extra's
	L"Стоимость мед. депозита",			// If the merc required a medical deposit, this is displayed
	L"Набор 1",				// Text on Starting Gear Selection Button 1
	L"Набор 2",				// Text on Starting Gear Selection Button 2
	L"Набор 3",				// Text on Starting Gear Selection Button 3
	L"Набор 4",				// Text on Starting Gear Selection Button 4
	L"Набор 5",				// Text on Starting Gear Selection Button 5
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

	L"У вас уже полная команда из наёмников.",

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
	L"A.I.M. История",			//Title

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
	L"Механика",
	L"Взрывчатка",
	L"Медицина",
	L"Здоровье",
	L"Проворность",
	L"Ловкость",
	L"Сила",
	L"Лидерство",
	L"Интеллект",
	L"Имя",

	// The title of the page, the above text gets added at the end of this text

	L"Состав A.I.M. По возрастанию, критерий - %s",
	L"Состав A.I.M. По убыванию, критерий - %s",

	// Instructions to the players on what to do

	L"Левый щелчок",
	L"Выбрать наёмника",
	L"Правый щелчок",
	L"Критерий сортировки",

	// Gets displayed on top of the merc's portrait if they are...

	L"Выбыл",
	L"Скончался",
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

	L"ОК",			// Stops displaying information on selected merc
	L"След. стр.",
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
	L"\"Мы сбросим ваш букет где угодно!\"",
	L"Сделай как надо",
	L"...в первый раз",
	L"Если у нас нет такого ствола, то он вам и не нужен.",
};


//Aim Home Page

STR16			AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages
	L"В начало",
	L"Наёмники",
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
	L"ИМЕЮЩИЕСЯ ТОВАРЫ",	//Header for the merchandise available
	L"СТР.",				//The current store inventory page being displayed
	L"ОБЩАЯ ЦЕНА",			//The total cost of the the items in the Dealer inventory area
	L"ОБЩАЯ ЦЕННОСТЬ",		//The total value of items player wishes to sell
	L"ОЦЕНКА",				//Button text for dealer to evaluate items the player wants to sell
	L"ПЕРЕВОД",				//Button text which completes the deal. Makes the transaction.
	L"УЙТИ",				//Text for the button which will leave the shopkeeper interface.
	L"ЦЕНА РЕМОНТА",		//The amount the dealer will charge to repair the merc's goods
	L"1 ЧАС",				// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d ЧАСОВ",			// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"ИСПРАВНО",			// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"Вам уже некуда класть вещи.",	//Message box that tells the user there is no more room to put there stuff
	L"%d МИНУТ",			// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"Выбросить предмет на землю.",
	L"БЮДЖЕТ",
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
	L"OK",						// Transfer the money
	L"Взять",				//Take money from the player
	L"Дать",				//Give money to the player
	L"Отмена",				//Cancel the transfer
	L"Очистить",			//Clear the money display
};


//Shopkeeper Interface
STR16	gzSkiAtmText[] = 
{	

	// Text on the bank machine panel that....
	L"Выберите тип",			//tells the user to select either to give or take from the merc
	L"Введите сумму",			//Enter the amount to transfer
	L"Перевести деньги бойцу",	//Giving money to the merc
	L"Забрать деньги у бойца",	//Taking money from the merc
	L"Недостаточно средств",	//Not enough money to transfer
	L"Баланс",					//Display the amount of money the player currently has
};


STR16	SkiMessageBoxText[] =
{
	L"Желаете снять со счета %s, чтобы покрыть разницу?",
	L"Недостаточно средств. Не хватает %s",
	L"Желаете снять со счета %s, чтобы оплатить полную стоимость?",
	L"Попросить торговца сделать перевод",
	L"Попросить торговца починить выбранные предметы",
	L"Закончить беседу",
	L"Текущий баланс",

	L"Do you want to transfer %s Intel to cover the difference?",	// TODO.Translate
	L"Do you want to transfer %s Intel to cover the cost?",
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
	L"1.13 Features",
	L"New in 1.13",
	L"Options",

	//Text above the slider bars
	L"Звуки",
	L"Речь",
	L"Музыка",

	//Confirmation pop when the user selects..
	L"Выйти из игры и вернуться в главное меню?",

	L"Необходимо выбрать или \"Речь\", или \"Субтитры\"",
};

STR16	z113FeaturesScreenText[] =
{
	L"1.13 FEATURE TOGGLES",
	L"Changing these settings during a campaign will affect your experience.",
	L"Hover over a feature to display more information. Some features may be configurable in JA2_Options.ini (or other specified file).",
};

STR16	z113FeaturesToggleText[] =
{
	L"Use These Overrides",
	L"New Chance to Hit",
	L"Enemies Drop All",
	L"Enemies Drop All (Damaged)",
	L"Suppression Fire",
	L"Drassen Counterattack",
	L"City Counterattacks",
	L"Multiple Counterattacks",
	L"Intel",
	L"Prisoners",
	L"Mines Require Workers",
	L"Enemy Ambushes",
	L"Enemy Assassins",
	L"Enemy Roles",
	L"Enemy Role: Medic",
	L"Enemy Role: Officer",
	L"Enemy Role: General",
	L"Kerberus",
	L"Mercs Need Food",
	L"Disease",
	L"Dynamic Opinions",
	L"Dynamic Dialogue",
	L"Arulco Strategic Division",
	L"ASD: Helicopters",
	L"Enemy Vehicles Can Move",
	L"Zombies",
	L"Bloodcat Raids",
	L"Bandit Raids",
	L"Zombie Raids",
	L"Militia Volunteer Pool",
	L"Tactical Militia Command",
	L"Strategic Militia Command",
	L"Militia Uses Sector Equipment",
	L"Militia Requires Resources",
	L"Enhanced Close Combat",
	L"Improved Interrupt System",
	L"Weapon Overheating",
	L"Weather: Rain",
	L"Weather: Lightning",
	L"Weather: Sandstorms",
	L"Weather: Snow",
	L"Mini Events",
	L"Arulco Rebel Command",
};

STR16	z113FeaturesHelpText[] =
{
	L"|U|s|e |T|h|e|s|e |O|v|e|r|r|i|d|e|s\n \nAllow this screen to override some feature toggles present in JA2_Options.ini.\nHover over a feature to see which flag is overridden.\nThese toggles have no effect if this option is disabled.",
	L"|N|C|T|H\nOverrides [Tactical Gameplay Settings] NCTH\n \nUse the new chance to hit system.\n \nFor tweakable values, see CTHConstants.ini.",
	L"|E|n|e|m|i|e|s |D|r|o|p |E|v|e|r|y|t|h|i|n|g\nOverrides [Tactical Difficulty Settings] DROP_ALL\n \nEnemies drop all of their gear when killed.\n \nNot compatible with Enemies Drop All (Damaged).",
	L"|E|n|e|m|i|e|s |D|r|o|p |E|v|e|r|y|t|h|i|n|g |(|D|a|m|a|g|e|d|)\nOverrides [Tactical Difficulty Settings] DROP_ALL\n \nEnemies drop all of their gear when killed, but things that would normally not be dropped are severely damaged.\n \nNot compatible with Enemies Drop All.",
	L"|S|u|p|p|r|e|s|s|i|o|n |F|i|r|e\nOverrides [Tactical Suppression Fire Settings] SUPPRESSION_EFFECTIVENESS\n \nCombatants can be affected by suppression and shock.\n \nFor configurable options, see [Tactical Suppression Fire Settings].",
	L"|D|r|a|s|s|e|n |C|o|u|n|t|e|r|a|t|t|a|c|k\nOverrides [Strategic Event Settings] TRIGGER_MASSIVE_ENEMY_COUNTERATTACK_AT_DRASSEN\n \nThe Queen sends a massive counterattack to Drassen.",
	L"|C|i|t|y |C|o|u|n|t|e|r|a|t|t|a|c|k|s\nOverrides [Strategic Event Settings] AGGRESSIVE_STRATEGIC_AI\n \nThe Queen can send a massive counterattack to cities other than Drassen.\n \nNot compatible with Multiple Counterattacks.",
	L"|M|u|l|t|i|p|l|e |C|o|u|n|t|e|r|a|t|t|a|c|k|s\nOverrides [Strategic Event Settings] AGGRESSIVE_STRATEGIC_AI\n \nThe Queen can send massive counterattacks to cities other than Drassen.\nThis can occur multiple times.\n \nThis is a harder variant of City Counterattacks.",
	L"|I|n|t|e|l\nOverrides [Intel Settings] RESOURCE_INTEL\n \nA new resource gained through covert means.",
	L"|P|r|i|s|o|n|e|r|s\nOverrides [Strategic Gameplay Settings] ALLOW_TAKE_PRISONERS\n \nCapture enemy soldiers and interrogate them.\n \nConfigurable Options:\nENEMY_CAN_SURRENDER\nDISPLAY_SURRENDER_VALUES\nSURRENDER_MULTIPLIER\nPRISONER_RETURN_TO_ARMY_CHANCE\nPRISONER_DEFECT_CHANCE\nPRISONER_INTEL_CHANCE\nPRISONER_RANSOM_CHANCE\nPRISONER_INTERROGATION_POINTS_ADMIN\nPRISONER_INTERROGATION_POINTS_REGULAR\nPRISONER_INTERROGATION_POINTS_ELITE\nPRISONER_INTERROGATION_POINTS_OFFICER\nPRISONER_INTERROGATION_POINTS_GENERAL\nPRISONER_INTERROGATION_POINTS_CIVILIAN",
	L"|M|i|n|e|s |R|e|q|u|i|r|e |W|o|r|k|e|r|s\nOverrides [Financial Settings] MINE_REQUIRES_WORKERS\n \nMines require workers to operate.\n \nConfigurable Options:\nWORKERRATE_PRESENT_INITIALLY\nWORKER_TRAINING_COST\nWORKER_TRAINING_POINTS",
	L"|E|n|e|m|y |A|m|b|u|s|h|e|s\nOverrides [Tactical Difficulty Settings] ENABLE_CHANCE_OF_ENEMY_AMBUSHES\n \nEnemy forces can ambush your mercs as they move in the strategic view.\n \nConfigurable Options:\nENEMY_AMBUSHES_CHANCE_MODIFIER\nAMBUSH_MERCS_SPREAD\nAMBUSH_MERCS_SPREAD_RADIUS\nAMBUSH_ENEMY_ENCIRCLEMENT\nAMBUSH_ENEMY_ENCIRCLEMENT_RADIUS1\nAMBUSH_ENEMY_ENCIRCLEMENT_RADIUS2",
	L"|E|n|e|m|y |A|s|s|a|s|s|i|n|s\nOverrides [Tactical Difficulty Settings] ENEMY_ASSASSINS\n \nAssassins can infiltrate your militia.\nRequires the new trait system.\n \nConfigurable Options:\nASSASSIN_MINIMUM_PROGRESS\nASSASSIN_MINIMUM_MILITIA\nASSASSIN_PROPABILITY_MODIFIER",
	L"|E|n|e|m|y |R|o|l|e|s\nOverrides [Tactical Enemy Role Settings] ENEMYROLES\n \nSpecialists can appear in enemy groups.\n \nConfigurable Options:\nENEMYROLES_TURNSTOUNCOVER",
	L"|E|n|e|m|y |R|o|l|e|: |M|e|d|i|c\nOverrides [Tactical Enemy Role Settings] ENEMY_MEDICS\n \nMedics can appear in enemy groups.\nRequires the Enemy Roles feature to be enabled.\n \nConfigurable Options:\nENEMY_MEDICS_MEDKITDRAINFACTOR\nENEMY_MEDICS_SEARCHRADIUS\nENEMY_MEDICS_WOUND_MINAMOUNT\nENEMY_MEDICS_HEAL_SELF",
	L"|E|n|e|m|y |R|o|l|e|: |O|f|f|i|c|e|r\nOverrides [Tactical Enemy Role Settings] ENEMY_OFFICERS\n \nOfficers can appear in enemy groups.\nRequires the Enemy Roles feature to be enabled.\n \nConfigurable Options:\nENEMY_OFFICERS_REQUIREDTEAMSIZE\nENEMY_OFFICERS_MAX\nENEMY_OFFICERS_SUPPRESSION_RESISTANCE_BONUS\nENEMY_OFFICERS_MORALE_MODIFIER\nENEMY_OFFICERS_SURRENDERSTRENGTHBONUS",
	L"|E|n|e|m|y |R|o|l|e|: |G|e|n|e|r|a|l\nOverrides [Tactical Enemy Role Settings] ENEMY_GENERALS\n \nGenerals increase enemy strategic movement and decision speeds.\nRequires the Enemy Roles feature to be enabled.\n \nConfigurable Options:\nENEMY_GENERALS_NUMBER\nENEMY_GENERALS_BODYGUARDS_NUMBER\nENEMY_GENERALS_STRATEGIC_DECISION_SPEEDBONUS\nENEMY_GENERALS_STRATEGIC_MOVEMENT_SPEEDBONUS",
	L"|K|e|r|b|e|r|u|s\nOverrides [PMC Settings] PMC\n \nHire militia from a private military company.\n \nConfigurable Options:\nPMC_MAX_REGULARS\nPMC_MAX_VETERANS",
	L"|F|o|o|d\nOverrides [Tactical Food Settings] FOOD\n \nYour mercs require food and water to survive.\n \nConfigurable Options:\nFOOD_DIGESTION_HOURLY_BASE_FOOD\nFOOD_DIGESTION_HOURLY_BASE_DRINK\nFOOD_DIGESTION_SLEEP\nFOOD_DIGESTION_TRAVEL_VEHICLE\nFOOD_DIGESTION_TRAVEL\nFOOD_DIGESTION_ASSIGNMENT\nFOOD_DIGESTION_ONDUTY\nFOOD_DIGESTION_COMBAT\nFOOD_DECAY_IN_SECTORS\nFOOD_DECAY_MODIFICATOR\nFOOD_EATING_SOUNDS",
	L"|D|i|s|e|a|s|e\nOverrides [Disease Settings] DISEASE\n \nYour mercs can catch diseases.\n \nConfigurable Options:\nDISEASE_STRATEGIC\nDISEASE_WHO_SUBSCRIPTIONCOST\nDISEASE_CONTAMINATES_ITEMS\nDISEASE_SEVERE_LIMITATIONS",
	L"|D|y|n|a|m|i|c |O|p|i|n|i|o|n|s\nOverrides [Dynamic Opinion Settings] DYNAMIC_OPINIONS\n \nMercs can form dynamic opinions of each other, affecting morale.\n \nConfigurable Options:\nDYNAMIC_OPINIONS_SHOWCHANGE\nWAGE_ACCEPTANCE_FACTOR\nSee [Dynamic Opinion Modifiers Settings] in Morale_Settings.ini.",
	L"|D|y|n|a|m|i|c |D|i|a|l|o|g|u|e\nOverrides [Dynamic Dialogue Settings] DYNAMIC_DIALOGUE\n \nMercs can make brief comments, changing their relationship to each other.\nRequires the Dynamic Opinions feature to be active.\n \nConfigurable Options:\nDYNAMIC_DIALOGUE_TIME_OFFSET",
	L"|A|S|D\nOverrides [Strategic Additional Enemy AI Settings] ASD_ACTIVE\n \nThe Arulcan army gains mechanised forces.\n \nConfigurable Options:\nASD_COST_FUEL/JEEP/TANK/ROBOT\nASD_TIME_FUEL/JEEP/TANK/ROBOT\nASD_ASSIGNS_JEEPS/TANKS/ROBOTS\nASD_FUEL_REQUIRED_JEEP/TANK/ROBOT\nJEEP_MINIMUM_PROGRESS\nTANK_MINIMUM_PROGRESS\nROBOT_MINIMUM_PROGRESS",
	L"|A|S|D |H|e|l|i|c|o|p|t|e|r|s\nOverrides [Enemy Helicopter Settings] ENEMYHELI_ACTIVE\n \nThe AI can use helicopters to deploy troops.\nRequires the Arulco Special Division feature to be enabled.\n \nConfigurable Options:\nASD_COST_HELI\nASD_TIME_HELI\nENEMYHELI_DEFINITE_UNLOCK_AT_PROGRESS\nENEMYHELI_HP\nENEMYHELI_HP_REPAIRTIME\nENEMYHELI_HP_COST\nENEMYHELI_FUEL\nENEMYHELI_FUEL_REFUELTIME",
	L"|E|n|e|m|y |V|e|h|i|c|l|e|s |C|a|n |M|o|v|e\nOverrides [Tactical Gameplay Settings] ENEMY_TANKS_CAN_MOVE_IN_TACTICAL\n \nHostile jeeps and tanks can move in combat.\n \nConfigurable Options:\nALLOW_TANKS_DRIVING_OVER_PEOPLE\nTANKS_RAMMING_MAX_STRUCTURE_ARMOUR\nENEMY_JEEP_RAMMING_MAX_STRUCTURE_ARMOUR",
	L"|Z|o|m|b|i|e|s\nOverrides the \"Allow Zombies\" toggle in the options menu.\n \nThe dead walk!\n \nConfigurable Options:\nZOMBIE_RISE_BEHAVIOUR\nZOMBIE_SPAWN_WAVES\nZOMBIE_RISE_WAVE_FREQUENCY\nZOMBIE_CAN_CLIMB\nZOMBIE_CAN_JUMP_WINDOWS\nZOMBIE_EXPLODING_CIVS\nZOMBIE_DAMAGE_RESISTANCE\nZOMBIE_BREATH_DAMAGE_RESISTANCE\nZOMBIE_ONLY_HEADSHOTS_WORK\nZOMBIE_DIFFICULTY_LEVEL\nZOMBIE_RISE_WITH_ARMOUR\nZOMBIE_ONLY_HEADSHOTS_PERMANENTLY_KILL",
	L"|B|l|o|o|d|c|a|t |R|a|i|d|s\nOverrides [Raid Settings] RAID_BLOODCATS\n \nHungry predators stalk the night.\n \nConfigurable Options:\nRAID_MAXSIZE_BLOODCATS\nRAID_MAXATTACKSPERNIGHT_BLOODCATS",
	L"|B|a|n|d|i|t |R|a|i|d|s\nOverrides [Raid Settings] RAID_BANDITS\n \nOpportunistic bandits may attack your towns.\n \nConfigurable Options:\nRAID_MAXSIZE_BANDITS\nRAID_MAXATTACKSPERNIGHT_BANDITS",
	L"|Z|o|m|b|i|e |R|a|i|d|s\nOverrides [Raid Settings] RAID_ZOMBIES\n \nThe dead raid!\nRequires the Zombies feature to be enabled.\n \nConfigurable Options:\nRAID_MAXSIZE_ZOMBIES\nRAID_MAXATTACKSPERNIGHT_ZOMBIES",
	L"|M|i|l|i|t|i|a |V|o|l|u|n|t|e|e|r |P|o|o|l\nOverrides [Militia Volunteer Pool Settings] MILITIA_VOLUNTEER_POOL\n \nVolunteers are required to train militia.\n \nConfigurable Options:\nMILITIA_VOLUNTEER_POOL_GAINFACTOR_LIBERATION\nMILITIA_VOLUNTEER_POOL_MULTIPLIER_FARM\nMILITIA_VOLUNTEER_POOL_GAINFACTOR_HOURLY",
	L"|T|a|c|t|i|c|a|l |M|i|l|i|t|i|a |C|o|m|m|a|n|d\nOverrides [Tactical Interface Settings] ALLOW_TACTICAL_MILITIA_COMMAND\n \nIssue commands to militia in tactical view.",
	L"|S|t|r|a|t|e|g|i|c |M|i|l|i|t|i|a |C|o|m|m|a|n|d\nOverrides [Militia Strategic Movement Settings] ALLOW_MILITIA_STRATEGIC_COMMAND\n \nIssue commands to militia in the strategic map.\n \nConfigurable Options:\nMILITIA_STRATEGIC_COMMAND_REQUIRES_MERC",
	L"|M|i|l|i|t|i|a |U|s|e|s |S|e|c|t|o|r |E|q|u|i|p|m|e|n|t\nOverrides [Militia Equipment Settings] MILITIA_USE_SECTOR_EQUIPMENT\n \nMilitia uses gear from their current sector.\nNot compatible with the Militia Requires Resources feature.\n \nConfigurable Options:\nMILITIA_USE_SECTOR_EQUIPMENT_ARMOUR\nMILITIA_USE_SECTOR_EQUIPMENT_FACE\nMILITIA_USE_SECTOR_EQUIPMENT_MELEE\nMILITIA_USE_SECTOR_EQUIPMENT_GUN\nMILITIA_USE_SECTOR_EQUIPMENT_AMMO\nMILITIA_USE_SECTOR_EQUIPMENT_GUN_ATTACHMENTS\nMILITIA_USE_SECTOR_EQUIPMENT_GRENADE\nMILITIA_USE_SECTOR_EQUIPMENT_LAUNCHER\nMILITIA_USE_SECTOR_EQUIPMENT_AMMO_MIN\nMILITIA_USE_SECTOR_EQUIPMENT_AMMO_MAX\nMILITIA_USE_SECTOR_EQUIPMENT_AMMO_OPTIMAL_MAG_COUNT\nMILITIA_USE_SECTOR_EQUIPMENT_CLASS_SPECIFIC_TABOOS",
	L"|M|i|l|i|t|i|a |R|e|q|u|i|r|e|s |R|e|s|o|u|r|c|e|s\nOverrides [Militia Resource Settings] MILITIA_REQUIRE_RESOURCES\n \nMilitia require resources to be trained.\nNot compatible with the Militia Uses Sector Equipment feature.\n \nConfigurable Options:\nMILITIA_RESOURCES_PROGRESSFACTOR\nMILITIA_RESOURCES_ITEMCLASSMOD_AMMO_BULLET\nMILITIA_RESOURCES_ITEMCLASSMOD_GUN\nMILITIA_RESOURCES_ITEMCLASSMOD_ARMOUR\nMILITIA_RESOURCES_ITEMCLASSMOD_MELEE\nMILITIA_RESOURCES_ITEMCLASSMOD_BOMB\nMILITIA_RESOURCES_ITEMCLASSMOD_GRENADE\nMILITIA_RESOURCES_ITEMCLASSMOD_FACE\nMILITIA_RESOURCES_ITEMCLASSMOD_LBE\nMILITIA_RESOURCES_ITEMCLASSMOD_ATTACHMENT_LOW\nMILITIA_RESOURCES_ITEMCLASSMOD_ATTACHMENT_MEDIUM\nMILITIA_RESOURCES_ITEMCLASSMOD_ATTACHMENT_HIGH\nMILITIA_RESOURCES_WEAPONMOD_PISTOL\nMILITIA_RESOURCES_WEAPONMOD_M_PISTOL\nMILITIA_RESOURCES_WEAPONMOD_SMG\nMILITIA_RESOURCES_WEAPONMOD_RIFLE\nMILITIA_RESOURCES_WEAPONMOD_SN_RIFLE\nMILITIA_RESOURCES_WEAPONMOD_AS_RIFLE\nMILITIA_RESOURCES_WEAPONMOD_LMG\nMILITIA_RESOURCES_WEAPONMOD_SHOTGUN",
	L"|E|n|h|a|n|c|e|d |C|l|o|s|e |C|o|m|b|a|t\nOverrides [Tactical Gameplay Settings] ENHANCED_CLOSE_COMBAT_SYSTEM\n \nA general improvement to the close combat system.",
	L"|I|m|p|r|o|v|e|d |I|n|t|e|r|r|u|p|t |S|y|s|t|e|m\nOverrides [Tactical Gameplay Settings] IMPROVED_INTERRUPT_SYSTEM\n \nAn overhaul of the interrupt mechanic.\n \nConfigurable Options:\nBASIC_PERCENTAGE_APS_REGISTERED\nPERCENTAGE_APS_REGISTERED_PER_EXP_LEVEL\nBASIC_REACTION_TIME_LENGTH\nALLOW_COLLECTIVE_INTERRUPTS\nALLOW_INSTANT_INTERRUPTS_ON_SPOTTING",
	L"|O|v|e|r|h|e|a|t|i|n|g\nOverrides [Tactical Weapon Overheating Settings] OVERHEATING\n \nWeapons can overheat.\n \nConfigurable Options:\nOVERHEATING_DISPLAY_JAMPERCENTAGE\nOVERHEATING_DISPLAY_THERMOMETER_RED_OFFSET\nOVERHEATING_COOLDOWN_MODIFICATOR_LONELYBARREL",
	L"|W|e|a|t|h|e|r|: |R|a|i|n\nOverrides [Tactical Weather Settings] ALLOW_RAIN\n \nRain can reduce visibility.\n \nConfigurable Options:\nRAIN_EVENTS_PER_DAY\nRAIN_CHANCE_PER_DAY\nRAIN_MIN_LENGTH_IN_MINUTES\nRAIN_MAX_LENGTH_IN_MINUTES\nMAX_RAIN_DROPS\nWEAPON_RELIABILITY_REDUCTION_RAIN\nBREATH_GAIN_REDUCTION_RAIN\nVISUAL_DISTANCE_DECREASE_RAIN\nHEARING_REDUCTION_RAIN",
	L"|W|e|a|t|h|e|r|: |L|i|g|h|t|n|i|n|g\nOverrides [Tactical Weather Settings] ALLOW_LIGHTNING\n \nLightning may occur during rainstorms.\nRequires Weather: Rain\n \nConfigurable Options:\nMIN_INTERVAL_BETWEEN_LIGHTNINGS_IN_REAL_TIME_SECONDS\nMAX_INTERVAL_BETWEEN_LIGHTNINGS_IN_REAL_TIME_SECONDS\nMIN_INTERVAL_BETWEEN_LIGHTNING_AND_THUNDERCLAPS_IN_SECONDS\nMAX_INTERVAL_BETWEEN_LIGHTNING_AND_THUNDERCLAPS_IN_SECONDS\nDELAY_IN_SECONDS_IF_SEEN_SOMEONE_DURING_LIGHTNING_IN_TURNBASED\nCHANCE_TO_DO_LIGHTNING_BETWEEN_TURNS\nWEAPON_RELIABILITY_REDUCTION_THUNDERSTORM\nBREATH_GAIN_REDUCTION_THUNDERSTORM\nVISUAL_DISTANCE_DECREASE_THUNDERSTORM\nHEARING_REDUCTION_THUNDERSTORM",
	L"|W|e|a|t|h|e|r|: |S|a|n|d|s|t|o|r|m|s\nOverrides [Tactical Weather Settings] ALLOW_SANDSTORMS\n \nSevere sandstorms make the battlefield more painful for everyone.\n \nConfigurable Options:\nSANDSTORM_EVENTS_PER_DAY\nSANDSTORM_CHANCE_PER_DAY\nSANDSTORM_MIN_LENGTH_IN_MINUTES\nSANDSTORM_MAX_LENGTH_IN_MINUTES\nWEAPON_RELIABILITY_REDUCTION_SANDSTORM\nBREATH_GAIN_REDUCTION_SANDSTORM\nVISUAL_DISTANCE_DECREASE_SANDSTORM\nHEARING_REDUCTION_SANDSTORM",
	L"|W|e|a|t|h|e|r|: |S|n|o|w\nOverrides [Tactical Weather Settings] ALLOW_SNOW\n \nSnowstorms decrease visibility.\n \nConfigurable Options:\nSNOW_EVENTS_PER_DAY\nSNOW_CHANCE_PER_DAY\nSNOW_MIN_LENGTH_IN_MINUTES\nSNOW_MAX_LENGTH_IN_MINUTES\nWEAPON_RELIABILITY_REDUCTION_SNOW\nBREATH_GAIN_REDUCTION_SNOW\nVISUAL_DISTANCE_DECREASE_SNOW\nHEARING_REDUCTION_SNOW",
	L"|M|i|n|i |E|v|e|n|t|s\nOverrides [Mini Events Settings] MINI_EVENTS_ENABLED\n \nRandom events can occur.\n \nConfigurable Options:\nMINI_EVENTS_MIN_HOURS_BETWEEN_EVENTS\nMINI_EVENTS_MAX_HOURS_BETWEEN_EVENTS\n \nSee MiniEvents.lua for more details.",
	L"|A|R|C\nOverrides [Rebel Command Settings] REBEL_COMMAND_ENABLED\n \nCommand the rebel movement at the strategic level, and upgrade captured towns.\n \nFor tweakable values, see RebelCommand_Settings.ini.",
};

STR16	z113FeaturesPanelText[] =
{
	L"Use the options here to enable some of 1.13's many features. If enabled, the toggle boxes here will take precedence over some booleans in JA2_Options.ini. If disabled, these boxes will have no effect.",
	L"New Chance to Hit (NCTH) is a complete overhaul of the vanilla chance-to-hit system. Compared to the vanilla system, it takes into account more variables when determining the trajectory of a fired bullet.",
	L"This controls whether enemies drop all of their items when they are killed. If enabled, everything is dropped. Otherwise, regular drop chances are used.",
	L"This controls whether enemies drop all of their items when they are killed. If enabled, everything is dropped, and the drop chances are used to determine whether an item is severely damaged.",
	L"Suppression Fire is a way of controlling a battlefield. When under heavy fire, a character accumulates suppression, causing AP loss. In addition to AP loss, Suppression SHOCK can also be accumulated, which makes the character less useful - both Chance-to-Hit and chance to be hit are reduced. Characters can go into negative APs when under suppression fire, losing APs off their NEXT turn. Use suppression fire to prevent enemies from approaching, pin them down, and then advance and kill them while they are hiding. Note that they will try to do the same thing to you!",
	L"When you first capture Drassen, the Queen actually follows through on her command to retake Drassen. This will make holding Drassen VERY difficult in the early game and enabling this option is not recommended for new players!",
	L"Similar to the Drassen Counterattack, other cities can be subjected to massive counterattacks by the Queen after you capture them.",
	L"If City Counterattacks still aren't enough for you, try enabling this! Not only will the Queen send counterattacks, but she may also try to retake multiple cities at the same time.",
	L"You can acquire and spend Intel, a resource closely tied to espionage and information brokering. Intel can be gained by spies, interrogating prisoners, and other ways. It can then be spent at a black market for rare weapons, or at the Recon Intelligence Services website for enemy info.",
	L"Allows you to take prisoners, which can be done by demanding their surrender in combat, or by handcuffing incapacitated soldiers. Once captured, they can be sent to a prison you own and interrogated, which can result in money, intel, or their defection to your militia.",
	L"You will need to invest a little bit in a mine before you can reap its full benefits. Workers are trained like militia, costing time and money. Note that losing control of a sector may cause worker loss!",
	L"Enemy groups have a chance to ambush your squad. If your squad is ambushed, they will be placed in the centre of the map with the enemy group encircling them.",
	L"The queen can now send out assassins. These are elite soldiers that are covert ops experts. They will disguise as members of your militia, and, when the time is right, they will suddenly attack your mercs. New trait system required and new inventory system recommended.",
	L"A variety of enemy types can appear in enemy groups, increasing their overall effectiveness. A small icon will appear next to an enemy who has been observed to have a role. Roles include weapon and equipment specialists, and soldiers carrying keys or keycards.",
	L"Requires the Enemy Roles feature to be enabled. Enemy medics can bandage their wounded comrades and perform field surgery on them.",
	L"Requires the Enemy Roles feature to be enabled. Enemy lieutenants and captains provide sector-wide bonuses for their squad.",
	L"Requires the Enemy Roles feature to be enabled. Generals provide strategic bonuses to the enemy army and may be present in enemy-controlled towns. As high-value targets, they are protected by a small retinue of bodyguards and may flee when they sense danger.",
	L"Some time after you have trained militia, Kerberus will offer its services to you. On their website, you can order security personnel from them, which will act as militia. They require a high down payment but require no training time.",
	L"Your team needs food and water to survive. Without them, they will starve and suffer severe penalties. Keep an eye on the quality of food you're consuming!",
	L"It is possible for your team to catch illnesses from a variety of sources: open wounds, corpses, swamp insects, etc. A disease will give certain mali, and in extreme cases, cause death. Most diseases can be treated by doctors or medicines, and there are items that can provide protection.",
	L"Mercs can form dynamic opinions of each other from a variety of events. These opinion events directly influence a merc's morale and can be both positive and negative. While an opinion event can occur once per day, the impact of one of these events can last for a few days, so it's possible for events to compound over this time period.",
	L"This is an add-on to the Dynamic Opinions feature. This feature allows mercs to talk to each other whenever an opinion event occurs. These dialogue choices may have further impacts on how mercs see each other. If an IMP merc takes part in this, you have a brief window to choose how that character responds.",
	L"The Arulco Special Division is responsible for ordering and deploying mechanised units to the Arulcan army. It uses income gained from mines to purchase vehicles for use against you.",
	L"Requires the Arulco Strategic Division feature to be enabled. At a certain point in your campaign, the ASD can start using helicopters to deploy small squads of elite soldiers to harass you.",
	L"Enemy jeeps and tanks can move around during combat, and may even try to run over your mercs. They will also destroy some obstacles by ramming them.",
	L"Zombies rise from corpses!",
	L"Arulco's deadly bloodcats can attack vulnerable town sectors at night. Civilian deaths will cause loyalty losses.",
	L"Taking advantage of open warfare, bandits can attack vulnerable town sectors. Civilian deaths will cause loyalty losses.",
	L"Requires the Zombies feature to be enabled. Zombies can swarm town sectors. Civilian deaths will cause loyalty losses.",
	L"Before you can train militia, you need willing volunteers. You will need to control both town sectors and the surrounding farmland to bolster your recruit pool.",
	L"Allows you to give commands to militia while in the tactical view. To do this, speak with any militia and a command menu will appear. Mercs wearing Extended Ears or Radio Sets can issue commands to militia that are out of line of sight.",
	L"Allows you to give sector movement commands to militia while in the strategic map. You may need to be in the same sector (unless you have a Radio Operator or someone staffing an HQ) to issue strategic movement orders.",
	L"Militia gear will not be randomly generated, but taken from the sector the militia is currently stationed in. Militia will return their gear to their sector when a new sector is loaded. Gear can be manually dropped in tactical via the 'Ctrl' + '.' menu, under 'militia inspection'. Gear can be prohibited to the militia by hovering over it in the strategic inventory and pressing 'TAB' + 'LMB'. It is up to you to sufficiently distribute gear to your militia.",
	L"In order to be trained, militia require 3 resources: Guns, Armour, and Misc. These can be obtained by converting dropped items in the strategic item view, and ALT + Right clicking on an item. Green Militia just require a Gun, Veteran Militia require a Gun + Armour, and Elite Militia require Gun + Armour + Misc.",
	L"General improvements to the close combat system. Head strikes deal more damage, but are harder to hit. Leg strikes are easier to land, but do less damage. Damage increased to surprised and prone targets. Stealing all items is possible on a knocked-down victim. Several other minor tweaks.",
	L"The Improved Interrupt System (IIS) completely changes how the game determines whether an interrupt occurs. Instead of interrupting as soon as a hostile enters line of sight, the game tracks several variables to simulate a soldier's ability to react and gain an interrupt.",
	L"Weapon barrels heat up as you fire them, potentially opening the door for jams, misfires, and faster weapon degradation. Weapons with replaceable barrels will probably be very useful to keep things cool.",
	L"Toggle rain in tactical. Rain will slightly decrease overall visibility and make it harder to hear things.",
	L"Toggle lightning and thunderstorms during rain. Lightning can reveal soldier positions for both you and the enemy. Thunder makes it significantly harder to hear, and overall breath regeneration is somewhat lowered.",
	L"Toggle sandstorms. Fighting in a sandstorm applies noticable maluses to all combatants - vision and hearing ranges are reduced, weapon degradation is significantly increased, and it is much harder to regain breath.",
	L"Toggle snow. In a snowstorm, it is harder to see, weapons degrade faster, and it is a little harder to regain breath.",
	L"During the course of a campaign, brief events can pop up. You can select one of two responses, which may have positive and/or negative effects. Events can affect a wide variety of things, but mostly your mercs.",
	L"After completing the food delivery quest for the rebels, they will grant you access to their command website (A.R.C.). You can set the rebels' country-wide directive there, and capturing towns allows you to enact policies in that region that provide powerful bonuses. This comes at a price - town loyalty will rise slower, so you will need to work harder to have the locals trust you.",
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

	L"Возможно, файлы сохранений повреждены. Желаете их удалить?",

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Версия сохранений игры была изменена. Просим сообщить, если это изменение привело к какой-либо ошибке. Пытаемся загрузить?",
#else
	L"Попытка загрузить сохранение игры устаревшей версии. Автоматически обновить его и загрузить?",
#endif

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Версия игры и файлов сохранений была изменена. Просим сообщить, если это изменение привело к какой-либо ошибке. Пытаемся загрузить?",
#else
	L"Попытка загрузить сохранение игры устаревшей версии. Автоматически обновить его и загрузить?",
#endif

	L"Вы решили записать игру на существующее сохранение #%d?",
	L"Хотите загрузить игру из сохранения #",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"У вас заканчивается свободное место на жестком диске. Сейчас свободно %d Мб, а требуется %d Мб свободного места для JA.",

	L"Сохранение",		//When saving a game, a message box with this string appears on the screen

	L"Нормальный",
	L"Огромный",
	L"нет",
	L"да",

	L"Элементы фантастики",
	L"Платиновая серия",

	L"Ассортимент Бобби Рэя",
	L"Нормальный",
	L"Большой",
	L"Огромный",
	L"Всё и сразу",

	L"Сохраненная игра была начата в режиме \"Нового Инвентаря\", этот режим не работает при разрешении экрана 640х480. Измените разрешение и загрузите игру снова.",
	L"Загрузка игры, начатой в режиме \"Нового Инвентаря\", невозможна. Установите в Ja2.ini игровую папку 'Data-1.13' и повторите попытку.",

	L"Размер отряда, заданный в сохраненной игре, не поддерживается текущим разрешением. Увеличьте разрешение экрана и попробуйте снова.", 
	L"Количество товара у Бобби Рэя",
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"Уровень карты",
	L"У вас нет ополченцев. Чтобы они появились, вам нужно склонить на свою сторону горожан.",
	L"Доход в сутки",
	L"Наёмник застрахован", 
	L"%s не нуждается в отдыхе.", 
	L"%s на марше и не может лечь спать.", 
	L"%s валится с ног от усталости, погоди немного.",
	L"%s ведет машину.",
	L"Отряд не может двигаться, когда один из наёмников спит.",

	// stuff for contracts
	L"Хотя у вас и есть деньги на подписание контракта, но их не хватит, чтобы оплатить страховку наёмника.",
	L"%s: продление страховки составит %s за %d дополнительных дней. Желаете заплатить?",
	L"Предметы в секторе",
	L"Жизнь наёмника застрахована.",
	
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
	L"У наёмника ограниченный контракт.", 
	L"Контракт наёмника не застрахован",
	L"Стратегическая карта",
	
	// Flugente: disease texts describing what a map view does
	L"This view shows how many rotting corpses are in a sector. The white number are corpses, the green number is accumulated disease, the sector colour indicates how widespread it is. GREY= No disease known of. GREEN to RED = escalating levels of disease.",	// TODO.Translate

	// Flugente: weather texts describing what a map view does
	L"Здесь паказана актуальная погода. Без цвета=солнечно, зеленый=дождь, Синий=Гроза, Оранжевый=песчаная буря, Белый=снег.",

	// Flugente: describe what intel map view does
	L"This view shows which sectors relevant what ongoing quests. Some data bought with intel is also shown here.", // TODO.Translate
};


STR16 pLandMarkInSectorString[] =
{
	L"Отряд %d заметил кого-то в секторе %s.",
	L"Отряд %s заметил кого-то в секторе %s.",
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
	L"освободить больше городских секторов", 

	L"освободить новые городские сектора", 
	L"освободить больше городов", 
	L"восстановить потерянные прогресс",
	L"продвинуться дальше", 

	L"нанять больше повстанцев",
};

//Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom of the single merc panel
STR16	gzMoneyWithdrawMessageText[] = 
{
	L"За один раз вы можете снять со счета не более $20 000.",
	L"Вы решили положить %s на свой счет?",
};

STR16	gzCopyrightText[] = 
{
	L"Авторские права (C) 1999 Sir-tech Canada Ltd. Все права защищены.",
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
	L"Скрывать мешающие кроны",
	L"Показать каркасы",
	L"Трехмерный курсор",
	L"Показать шанс попадания",
	L"Курсор очереди для гранат",
	L"Злорадные враги",	//Allow Enemy Taunts
	L"Стрельба гранатой навесом",
	L"Красться в реальном времени",
	L"Выбор пробелом след. отряда",
	L"Тени предметов в инвентаре",
	L"Дальность оружия в тайлах",
	L"Одиночный трассер",
	L"Шум дождя",
	L"Вороны",
	L"Подсказки над солдатами",	//Show Soldier Tooltips
	L"Автосохранение каждый ход",
	L"Молчаливый пилот вертолёта",
	L"Подробное описание предметов",		//Enhanced Description Box
	L"Только пошаговый режим",				// add forced turn mode
	L"Новая расцветка стратег. карты",	//Alternate Strategy-Map Colors //Change color scheme of Strategic Map
	L"Заметная летящая пуля",				// Show alternate bullet graphics (tracers)
	L"Новая анимация экипировки",
	L"Показать ранг бойца",						// shows mercs ranks
	L"Показать снаряжение на голове",				//Show Face gear graphics
	L"Показать иконки снаряжения",
	L"Отключить меняющийся курсор",		            // Disable Cursor Swap
	L"Тихая тренировка",						// Madd: mercs don't say quotes while training
	L"Тихий ремонт",						// Madd: mercs don't say quotes while repairing
	L"Тихое лечение",						// Madd: mercs don't say quotes while doctoring
	L"Быстрый ход компьютера",			// Automatic fast forward through AI turns
	L"Зомби в игре!",						// Flugente Zombies
	L"Меню в инвентаре бойца",				// the_bob : enable popups for picking items from sector inv
	L"Отметить оставшихся врагов",
	L"Показать содержимое разгрузки",
	L"Инвертировать колесо мыши",
	L"Боевой порядок",						// when multiple mercs are selected, they will try to keep their relative distances
	L"Показывать расположение",				// show locator on last known enemy location
	L"--Читерские настройки--",				// TOPTION_CHEAT_MODE_OPTIONS_HEADER,
	L"Ускорить доставку Бобби Рэя",			// force all pending Bobby Ray shipments
	L"-----------------",					// TOPTION_CHEAT_MODE_OPTIONS_END
	L"--Настройки отладочной версии--",		// an example options screen options header (pure text)
	L"Сообщать координаты промахов",		//Report Miss Offsets			// Screen messages showing amount and direction of shot deviation.
	L"Сброс всех игровых настроек",			// failsafe show/hide option to reset all options
	L"В самом деле хотите этого?",			// a do once and reset self option (button like effect)
	L"Отладочные настройки везде",			//Debug Options in other builds		// allow debugging in release or mapeditor
	L"Показать Отладочные настройки",		//DEBUG Render Option group	// an example option that will show/hide other options
	L"Отображать Mouse Regions",		//Render Mouse Regions	// an example of a DEBUG build option
	L"-----------------",					// an example options screen options divider (pure text)	

	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"THE_LAST_OPTION",
};

//This is the help text associated with the above toggles.
STR16	zOptionsScreenHelpText[] =
{
	// HEADROCK HAM 4: Added more tooltip text to some toggles, in order to explain them better.

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
	L"Отключите этот пункт,\nесли боитесь крови.",

	//Never move my mouse
	L"Если выключено, то курсор\nавтоматически перемещается на кнопку\nвсплывающего окна диалога.", 

	//Old selection method
	L"Если включено, то будет использоваться\nстарый метод выбора бойцов\n(для тех, кто привык к управлению предыдущих частей Jagged Alliance).",

	//Show movement path
	L"Если включено, то в режиме реального времени\nбудет отображаться путь передвижения\n(если выключено, нажмите |S|h|i|f|t, чтобы увидеть путь).",

	//show misses
	L"Если включено, то камера будет отслеживать\nтраекторию пуль, прошедших мимо цели.",
	
	//Real Time Confirmation
	L"Если включено, то для приказа на передвижение\nбудет требоваться дополнительный\nподтверждающий щелчок мыши на месте назначения.",

	//Sleep/Wake notification
	L"Если включено, то вы получите предупреждение,\nкогда наёмники лягут спать или проснутся.",

	//Use the metric system
	L"Если включено, то используется метрическая система мер,\nиначе будет британская.",

	//Merc Lighted movement
	L"При ходьбе карта подсвечивается вокруг бойца.\nОтключите эту настройку для повышения\nпроизводительности системы (|G).",

	//Smart cursor
	L"Если включено, то перемещение курсора возле наёмника\nавтоматически выбирает его.",

	//snap cursor to the door
	L"Если включено, то перемещение курсора возле двери\nавтоматически помещает его на дверь.",

	//glow items 
	L"Если включено, то все предметы подсвечиваются (|C|t|r|l+|A|l|t+|I).",

	//toggle tree tops
	L"Если включено, то отображаются кроны деревьев (|T).",

	//smart tree tops
	L"Если включено, кроны деревьев не будут отображаться около видимых солдат и около курсора.",

	//toggle wireframe
	L"Если включено, то у препятствий\nдополнительно показывается каркас (|C|t|r|l+|A|l|t+|W).",

	L"Если включено, то курсор передвижения\nотображается в 3D (|H|o|m|e).",

	// Options for 1.13
	L"Если включено, шанс попадания\nпоказывается над курсором.",
	L"Если включено, очередь из гранатомёта\nиспользует курсор стрельбы очередями.",
	L"Если включено, враг иногда будет\nкомментировать свои действия.",
	L"Если включено, гранатомёты выстреливают\nзаряд под большим углом к горизонту (|A|l|t+|Q).",
	L"Если включено, игра не переходит\nв пошаговый режим при обнаружении\nпротивника (если враг вас не видит). \nРучной вход в пошаговый режим - |C|t|r|l+|X. (|C|t|r|l+|S|h|i|f|t+|X)",
	L"Если включено, |П|р|о|б|е|л выделяет следующий отряд.",
	L"Если включено, показываются\nтени предметов в инвентаре.",
	L"Если включено, дальность оружия\nуказывается в игровых секторах.",
	L"Если включено, трассирующий эффект\nсоздаётся и одиночным выстрелом.",	
	L"Если включено, дождь будет\nсопровождаться звуком.",
	L"Если включено, вороны присутствуют в игре.",
	L"Если включено, при нажатии кнопки |A|l|t\nи наведении курсора мыши на вражеского солдата\nбудет показана дополнительная информация.",
	L"Если включено, игра будет автоматически\nсохраняться после каждого хода игрока.",
	L"Если включено, Небесный Всадник\nне будет вас раздражать болтливостью.",
	L"Если включено, будет задействовано\nподробное описание предметов.",
	L"Если включено и в секторе присутствует враг,\nпошаговый режим будет задействован\nдо полной зачистки сектора (|C|t|r|l+|T).",
	L"Если включено, необследованные сектора\nна стратегической карте будут чёрно-белыми.",
	L"Если включено, летящая пуля\nбудет более заметной.",
	L"Если включено, анимация наемника будет отображать используемое оружие и экипировку.",
	L"Если включено, на стратегическом экране\nбудет подписан ранг бойца перед его именем.",
	L"Если включено, на портрете наёмника\nбудет отображено надетое головное снаряжение.",
	L"Если включено, в правом нижнем углу\nна портрете наёмника будут отображены иконки\nнадетого головного снаряжения.",
	L"Если включено, курсор не будет меняться,\nпоказывая все возможные действия.\nЧтобы поменяться местами с человеком рядом, нажмите |X.",
	L"Если включено, бойцы не будут\nсообщать о повышении своих умений.",
	L"Если включено, бойцы не будут\nсообщать о статусе ремонта.",
	L"Если включено, бойцы не будут\nсообщать о статусе лечения.",
	L"Если включено, ожидание,\nпока сделают ход противник,\nгражданские и другие существа,\nбудет значительно меньше.",

	L"Если включено, убитые враги\nбудут превращаться в зомби. Веселитесь!",
	L"Если включено, при просмотре\nпредметов сектора в инвентаре бойца\nбудет доступно меню по нажатии\nлевой кнопки на пустой карман.",
	L"Если включено, указывается примерное\nположение последних врагов в секторе.",
	L"Если включено, показывает содержимое разгрузки,\nиначе - обычный интерфейс новой системы навески.",
	L"Если включено, инвертирует направление\nпрокрутки колеса мыши.",
	L"Если выбрано несколько наёмников,\nони будут пытаться сохранять взаимное расположение\nи дистанцию при движении\n(нажмите |C|t|r|l+|A|l|t+|G для переключения или |S|h|i|f|t + клик для движения).",
	L"Если включено, показывает известное расположение противника\n(нажмите |S|h|i|f|t, чтобы показать источник шума).",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_HEADER",
	L"Выберите этот пункт,\nчтобы груз Бобби Рэя прибыл немедленно.",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_END",
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER",	// an example options screen options header (pure text)
	L"|H|A|M |4 |D|e|b|u|g: When ON, will report the distance each bullet deviates from the\ncenter of the target, taking all NCTH factors into account.",
	L"Если включить, повреждённые игровые настройки\nбудут восстановлены.",	// failsafe show/hide option to reset all options
	L"Отметьте строку для подтверждения сброса игровых настроек.",	// a do once and reset self option (button like effect)
	L"Если включено, отладочные настройки\nбудут доступны как в игре,\nтак и в редакторе карт.",	// Allows debug options in release or mapeditor builds
	L"Если включено, отладочные настройки \nбудут показаны в общем списке.",	//Toggle to display debugging render options
	L"Attempts to display slash-rects around mouse regions",	// an example of a DEBUG build option
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END",	// an example options screen options divider (pure text)

	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"Последняя строка в списке настроек.",
};


STR16	gzGIOScreenText[] =
{
	L"УСТАНОВКИ НАЧАЛА ИГРЫ",
#ifdef JA2UB
	L"Случайная история Мануэля",
	L"да",
	L"нет",
#else
	L"Элементы фантастики",
	L"нет",
	L"есть",
#endif	
	L"Платиновая серия",
	L"Ассортимент оружия в игре",
	L"всё доступное",
	L"чуть поменьше",
	L"Уровень сложности",
	L"лёгкий",	//новичок
	L"средний",	//опытный
	L"трудный",	//эксперт
	L"БЕЗУМНЫЙ",	//помешанный
	L"Начать игру",
	L"Главное меню",
	L"Возможность сохранения",
	L"в любое время",
	L"между боями",		//Iron Man
	L"Отключено в демо-версии", 
	L"Ассортимент Бобби Рэя",
	L"хороший",
	L"большой",
	L"огромный",
	L"всё и сразу",
	L"Инвентарь / Навеска",
	L"NOT USED",
	L"NOT USED",
	L"Загрузить",
	L"УСТАНОВКИ ИГРЫ (актуальны только настройки игры сервера)",
	// Added by SANDRO
	L"Умения персонажа",
	L"старые",
	L"новые",
	L"Создаваемых персонажей",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"Враг оставляет всё снаряжение",
	L"нет",
	L"да",
#ifdef JA2UB
	L"Текс и Джон",
	L"Случайно",
	L"Оба сразу",
#else
	L"Число террористов",
	L"случайное",
	L"все сразу",
#endif
	L"Спрятанное оружие секторов",	//Secret Weapon Caches
	L"выборочно",
	L"всё возможное",
	L"Скорость обновления вооружения",	//Progress Speed of Item Choices
	L"очень медленно",
	L"медленно",
	L"умеренно",
	L"быстро",
	L"очень быстро",

	L"старый / старая",
	L"новый / старая",
	L"новый / новая",

	// Squad Size
	L"Бойцов в отряде",
	L"6",
	L"8",
	L"10",
	//L"Faster Bobby Ray Shipments",
	L"Манипуляции с инвентарём за ОД",

	L"Новая система прицеливания",	//New Chance to Hit System
	L"Новая система перехвата хода",
	L"Биография наёмников",
	L"Игра с едой",
	L"Количество товара у Бобби Рэя",

	// anv: extra iron man modes
	L"между перестрелкой",		//Soft Iron Man
	L"один раз в день",		//Extreme Iron Man
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
	L"Тип игры",	//Game Type
	L"Пинг",	//Ping
	L"Впишите имя игрока.",
	L"Впишите корректный IP адрес. (пример 84.114.195.239).",
	L"Впишите корректный порт сервера (используйте диапазон от 1 до 65535).",
};

STR16 gzMPJHelpText[] =
{
	L"Новых игроков можно найти здесь: http://webchat.quakenet.org/?channels=ja2-multiplayer",
	L"Можно нажать 'у', чтобы открыть окно чата внутриигровой, после того как вы были подключены к серверу.",	// TODO.Translate

	L"СОЗДАТЬ ИГРУ",
	L"Введите '127.0.0.1' в поле IP и выберите номер порта начиная с 60000.",	//Enter '127.0.0.1' for the IP and the Port number should be greater than 60000.
	L"Убедитесь что выбранный порт (UDP, TCP) не блокируется роутером. Подробнее читайте здесь: http://portforward.com",
	L"Так же сообщите по IRC или ICQ другим игрокам ваш внешний IP адрес и порт (http://www.whatismyip.com).",
	L"Жмите на кнопку 'Создать игру' для запуска сервера сетевой игры.",
	
	L"ПРИСОЕДИНИТЬСЯ К ИГРЕ",
	L"Создавший игру должен был вам сообщить (по IRC, ICQ и т.д.) свой внешний IP адрес и порт.",
	L"Впишите эти данные в поле IP адреса и номер порта.",
	L"Жмите 'Присоединиться' чтобы подключиться к уже созданной сетевой игре.",
};

STR16	gzMPHScreenText[] =
{
	L"СТАРТОВЫЕ УСТАНОВКИ СЕРВЕРА",			//HOST GAME
	L"Начать игру",					//Start
	L"Главное меню",				//Cancel
	L"Имя сервера",					//Server Name
	L"Тип игры",					//Game Type
	L"Deathmatch",
	L"Team-Deathmatch",
	L"Co-Operative",
	L"Количество игроков",				//Max Players
	L"Солдат в отряде",				//Maximum Mercs
	L"Выбор бойцов",
	L"Найм бойцов",
	L"Нанят игроком",				//Hired by Player
	L"Деньги при старте",			//Starting Cash
	L"Можно нанимать тех же бойцов",	//Allow Hiring Same Merc
	L"Сообщения о нанятых бойцах",			//Report Hired Mercs
	L"Бобби Рэй",				//Bobby Rays
	L"Место высадки",	//Sector Starting Edge
	L"Впишите имя сервера",				//You must enter a server name
	L"",
	L"",
	L"Время суток",	//Starting Time
	L"",
	L"",
	L"Убойность оружия",	//Weapon Damage
	L"",
	L"Время хода",	//Timed Turns
	L"",
	L"Гражданские в CO-OP",		//Enable Civilians in CO-OP
	L"",
	L"Максимум врагов в CO-OP",	//Maximum Enemies in CO-OP
	L"Синхронизация игровых файлов",	//Synchronize Game Directory
	L"MP Sync. Directory",
	L"Укажите директорию для синхронизации передаваемых файлов.",
	L"(Для разделения директорий используйте '/' вместо '\\'.)",
	L"Указанная директория для синхронизации не существует.",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	// Max. Enemies / Report Hired Merc / Enable Civs in CO-OP
	L"да",
	L"нет",
	// Starting Time
	L"утро",
	L"день",
	L"ночь",
	// Starting Cash
	L"мало",
	L"средне",
	L"много",
	L"неограничено",
	// Time Turns
	L"не ограничено",	//Never
	L"медленно",	//Slow
	L"умеренно",	//Medium
	L"быстро",	//Fast
	// Weapon Damage
	L"очень малая",	//Very low
	L"небольшая",	//Low
	L"хорошая",	//Normal
	// Merc Hire
	L"случайно",
	L"самостоятельно",	//Normal
	// Sector Edge
	L"случайно",
	L"выборочно",
	// Bobby Ray / Hire same merc
	L"нет",
	L"есть",
};

STR16 pDeliveryLocationStrings[] =
{
	L"Остин",		//Austin, Texas, USA
	L"Багдад",		//Baghdad, Iraq (Suddam Hussein's home)
	L"Драссен",		//The main place in JA2 that you can receive items.  The other towns are dummy names...
	L"Гонконг",		//Hong Kong, Hong Kong
	L"Бейрут",		//Beirut, Lebanon	(Middle East)
	L"Лондон",		//London, England
	L"Лос-Анджелес",//Los Angeles, California, USA (SW corner of USA)
	L"Медуна",		//Meduna -- the other airport in JA2 that you can receive items.
	L"Метавира",	//The island of Metavira was the fictional location used by JA1
	L"Майами",		//Miami, Florida, USA (SE corner of USA)
	L"Москва",		//Moscow, USSR
	L"Нью-Йорк",	//New York, New York, USA
	L"Оттава",		//Ottawa, Ontario, Canada -- where JA2 was made!
	L"Париж",		//Paris, France
	L"Триполи",		//Tripoli, Libya (eastern Mediterranean)
	L"Токио",		//Tokyo, Japan
	L"Ванкувер",	//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation.  It is possible to select 0 ability
	//in a skill meaning you can't use it.  This text is confirmation to the player.
	L"Вы уверены? Значение ноль означает отсутствие этого навыка вообще.",
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
	L"QuickSave", //10	The name of the quicksave file (filename, text reference)
	L"SaveGame",	//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",				//The 3 character dos extension (represents sav)
	L"..\\SavedGames", //The name of the directory where games are saved.
	L"День",
	L"Наёмн",
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
	L"Посмотрим, что происходит тем временем в другом месте",		//Meanwhile
	L"%s: прибыл в сектор %s%s",//30	Name/Squad has arrived in sector A9.  Order must not change without notifying
//SirTech
	L"Версия",
	L"Пустая ячейка быстрого сохр.",
	L"Эта ячейка зарезервирована для Быстрого Сохранения, которое можно провести с тактической карты или с глобальной карты, нажав клавиши ALT+S.",
	L"Открытая",
	L"Закрытая",
	L"У вас заканчивается свободное дисковое пространство. На диске есть всего %sMб свободного места, а для Jagged Alliance 2 требуется %s Mб.",
	L"Из A.I.M. нанят боец %s.", 
	L"%s ловит %s.",		//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s has taken %s.",	// TODO.Translate
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
	L"Отряд идёт тихим шагом.",
	L"Отряд идёт обычным шагом.",
	L"%s идёт тихим шагом.",
	L"%s идёт обычным шагом.",

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
	L"Нет",	//Skip
	L"%s не может уйти в одиночку.",
	L"Файл сохранения был записан под названием SaveGame249.sav. Если необходимо, переименуйте его в SaveGame01 - SaveGame10 и тогда он станет доступен в списке сохранений.", 
	L"%s: выпил(а) немного %s.",
	L"Посылка прибыла в Драссен.",
	L"%s прибудет в точку назначения (сектор %s) в %dй день, примерно в %s.",		//first %s is mercs name, next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival
	L"В журнал добавлена запись!",
	L"Очереди из гранат используют курсор стрельбы очередями (стрельба по площадям возможна)",
	L"Очереди из гранат используют курсор метания (стрельба по площадям невозможна)",
	L"Включены подписи к солдатам", // Changed from Drop All On (Enabled Soldier Tooltips)
	L"Отключены подписи к солдатам", 	// 80	// Changed from Drop All Off (Disabled Soldier Tooltips)
	L"Гранатометы стреляют под обычным углом",
	L"Гранатометы стреляют навесом",
	// forced turn mode strings
    L"Только пошаговый режим",
	L"Режим реального времени",	//Normal turn mode
	L"Выход из пошагового режима",	//Exit combat mode
	L"Включен только пошаговый режим. Вступаем в бой!",	//Forced Turn Mode Active, Entering Combat
	L"Игра сохранена в поле авто-сохранения.",
	L"..\\SavedGames\\MP_SavedGames", //The name of the directory where games are saved
	L"Клиент",	//Client
	L"Нельзя одновременно установить \"Старый\" инвентарь и \"Новую Систему Навески\".",	//You cannot use the Old Inventory and the New Attachment System at the same time.
	
	L"Auto Save #", //91		// Text des Auto Saves im Load Screen mit ID
	L"Этот слот зарезервирован для автоматической записи, которую можно включить/отключить (AUTO_SAVE_EVERY_N_HOURS) в файле ja2_options.ini.", //92	// The text, when the user clicks on the save screen on an auto save
	L"Пустой слот авто записи #", //93	// The text, when the auto save slot (1 - 5) is empty (not saved yet)
	L"AutoSaveGame",		// 94	// The filename of the auto save, such as AutoSaveGame01 - AutoSaveGame05
	L"End-Turn Save #",	// 95	// The text for the tactical end turn auto save
	L"Saving Auto Save #",	// 96	// The message box, when doing auto save
	L"Saving",	// 97	// The message box, when doing end turn auto save
	L"Empty End-Turn Save Slot #",	// 98	// The message box, when doing auto save
	L"Этот слот зарезервирвоан для автоматической записи в конце каждого хода, которую можно включить/отключить на экране опций.", //99	// The text, when the user clicks on the save screen on an auto save
	// Mouse tooltips
	L"QuickSave.sav",	// 100
	L"AutoSaveGame%02d.sav",	// 101
	L"Auto%02d.sav",	// 102
	L"SaveGame%02d.sav", //103
	// Lock / release mouse in windowed mode (window boundary)
	L"Захватить курсор мыши.",			// 104
	L"Освободить курсор мыши.",			// 105
	L"Движение в боевом порядке",
	L"Движение в обычном порядке",
	L"Подсветка наёмника включена",
	L"Подсветка наёмника отключена",
	L"Выбран отряд %s.",
	L"%s курит %s.",
	L"Активировать чит-коды?",
	L"Деактивировать чит-коды?",
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
	L"Убрать (|П|р|а|в|а|я |к|н|о|п|к|а |м|ы|ш|и)\nДобавить (|Л|е|в|а|я |к|н|о|п|к|а |м|ы|ш|и)\nНовобранцы", // button help text informing player they can pick up or drop militia with this button
	L"Убрать (|П|р|а|в|а|я |к|н|о|п|к|а |м|ы|ш|и)\nДобавить (|Л|е|в|а|я |к|н|о|п|к|а |м|ы|ш|и)\nРядовые",
	L"Убрать (|П|р|а|в|а|я |к|н|о|п|к|а |м|ы|ш|и)\nДобавить (|Л|е|в|а|я |к|н|о|п|к|а |м|ы|ш|и)\nВетераны",
	L"Равномерно распределить ополчение\nпо доступным секторам",
};

STR16 pMapScreenJustStartedHelpText[] =
{
	L"Отправляйтесь в A.I.M. и наймите бойцов (*Подсказка* - это в лэптопе).", // to inform the player to hired some mercs to get things going
#ifdef JA2UB
	L"Когда будете готовы отправиться в Тракону, включите сжатие времени в правом нижнем углу экрана.", // to inform the player to hit time compression to get the game underway
#else
	L"Когда будете готовы отправиться в Арулько, включите сжатие времени в правом нижнем углу экрана.", // to inform the player to hit time compression to get the game underway
#endif
};

STR16 pAntiHackerString[] = 
{
	L"Ошибка. Пропущен или испорчен файл(ы). Игра прекращает работу.",
};


STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"Просмотреть почту",
	L"Посетить интернет сайты",
	L"Просмотреть полученные данные",
	L"Просмотреть журнал последних событий",
	L"Показать информацию о команде",
	L"Просмотреть финансовые отчеты",
	L"Закрыть лэптоп",

	//Bottom task bar icons (if they exist):
	L"Получена новая почта",
	L"Получены новые данные",

	//Bookmarks:
	L"Международная Ассоциация Наёмников A.I.M.",
	L"Бобби Рэй - заказ оружия через Интернет",
	L"Институт Изучения Личности Наёмника I.M.P.",
	L"Центр рекрутов M.E.R.C.",
	L"Похоронная служба Макгилликатти",
	L"'Цветы по всему миру'",
	L"Страховые агенты по контрактам A.I.M.",
	//New Bookmarks
	L"",
	L"Энциклопедия",
	L"Брифинг-зал",
	L"События и факты. Арулькийские новости",
	L"Бойцы Любят Тебя или Клянут",
	L"Всемирная организация здравоохранения",
	L"Цербер - Опыт в безопасности",
	L"Ополчение",
	L"Recon Intelligence Services",	// TODO.Translate
	L"Controlled factories",	// TODO.Translate
};


STR16 gzHelpScreenText[] =
{
	L"Закрыть окно помощи",	
};

STR16 gzNonPersistantPBIText[] =
{
	L"Идет бой. Вы можете отступить только через тактический экран.",
	L"Войти в сектор, чтобы продолжить бой. (|E)",
	L"Провести бой автоматически (|A).",
	L"Во время атаки врага автоматическую битву включить нельзя.",
	L"После того, как вы попали в засаду, автоматическую битву включить нельзя.",
	L"Рядом рептионы - автоматическую битву включить нельзя.",
	L"Рядом враждебные гражданские - автоматическую битву включить нельзя.",
	L"Рядом кошки-убийцы - автоматическую битву включить нельзя.",
	L"ИДЕТ БОЙ",
	L"Сейчас вы не можете отступить.",
};

STR16 gzMiscString[] =
{
	L"Ваши ополченцы продолжают бой без помощи наёмников...",
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
	L"%s слышит %s звук стрельбы %s.", // anv: without this, all further noise notifications were off by 1!
	L"%s слышит %s звук взрыва %s.",
	L"%s слышит %s крик %s.",
	L"%s слышит %s звук удара %s.",
	L"%s слышит %s звук удара %s.",
	L"%s слышит %s звон %s.",
	L"%s слышит %s грохот %s.",
	L"",											 // anv: placeholder for silent alarm
	L"%s слышит чей-то %s голос %s.", // anv: report enemy taunt to player
};

STR16 pTauntUnknownVoice[] =
{
	L"Чей-то голос",
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
	L"Kris Märnes",
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
	L"", 												// Chris Camfield
	L"(Всё ещё зубрит правила пунктуации)",				// Shaun Lyng
	L"(\"Готово! Осталось только баги исправить.\")",	//Kris \"The Cow Rape Man\" Marnes
	L"(Уже слишком стар для всего этого)",				// Ian Currie
	L"(Также работает над Wizardry 8)",					// Linda Currie
	L"(Заставили тестировать под дулом пистолета)",		// Eric \"WTF\" Cheng
	L"(Ушла от нас в CFSA - скатертью дорожка...)",		// Lynn Holowka
	L"",							// Norman \"NRG\" Olsen
	L"",							// George Brooks
	L"(Поклонник джаза и группы Dead Head)",			// Andrew Stacey
	L"(Его настоящее имя Роберт)",						// Scot Loving
	L"(Единственный ответственный человек)",			// Andrew \"Big Cheese Doddle\" Emmons
	L"(Может опять заняться мотогонками)",				// Dave French
	L"(Украден с работы над Wizardry 8)",				// Alex Meduna
	L"(Делал предметы и загрузочные экраны!)",			// Joey \"Joeker\" Whelan",
};

// HEADROCK: Adjusted strings for better feedback, and added new string for LBE repair.
STR16 sRepairsDoneString[] =
{
	L"%s: завершён ремонт личных вещей.",
	L"%s: завершён ремонт всего оружия и брони.",
	L"%s: завершён ремонт всей экипировки отряда.",
	L"%s: завершён ремонт всех крупных вещей отряда.",
	L"%s: завершён ремонт всех малых вещей отряда.",
	L"%s: завершён ремонт всех мелких вещей отряда.",
	L"%s: завершён ремонт разгрузочных систем отряда.",
	L"%s: завершена чистка всего оружия отряда.",
};

STR16 zGioDifConfirmText[]=
{
	L"Вы выбрали ЛЁГКИЙ уровень сложности. Этот режим предназначен для первичного ознакомления с Jagged Alliance. Ваш выбор определит ход всей игры, так что будьте осторожны. Вы действительно хотите начать игру в этом режиме?", 
	L"Вы выбрали СРЕДНИЙ уровень сложности. Этот режим предназначен для тех, кто знаком с Jagged Alliance и подобными играми. Ваш выбор определит ход всей игры, так что будьте осторожны. Вы действительно хотите начать игру в этом режиме?", 
	L"Вы выбрали ТЯЖЁЛЫЙ уровень сложности. В этом режиме вам потребуется немалый опыт игры в Jagged Alliance. Ваш выбор определит ход всей игры, так что будьте осторожны. Вы действительно хотите начать игру в этом режиме?", 
	L"Вы выбрали БЕЗУМНЫЙ уровень сложности. Имейте в виду - в этом режиме возможности Дейдраны воистину за пределами разумного! Но если с головой вы не в ладах, то вам даже понравится. Рискнете?",
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
	L"Никто из ваших наёмников не имеет медицинских навыков.",
	L"Нечем бинтовать. Ни у кого из наёмников нет аптечки.",
	L"Чтобы перевязать всех наёмников, не хватило бинтов.",
	L"Никто из ваших наёмников не нуждается в перевязке.",
	L"Автоматически перевязывать бойцов.",
	L"Все ваши наёмники перевязаны.",

	//14
#ifdef JA2UB
	L"Тракона",
#else
	L"Арулько",
#endif

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

	//22-24 plural versions 
	L"%d новобранца из ополчения произведены в ветераны.",
	L"%d новобранца из ополчения произведены в рядовые.",
	L"%d рядовых ополченца произведены в ветераны.",

	//25
	L"Кнопка",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"У %s приступ безумия!",

	//27-28
	//Messages why a player can't time compress.
	L"Сейчас небезопасно включать сжатие времени - у вас есть наёмники в секторе %s.",
	L"Сейчас небезопасно включать сжатие времени - у вас есть наёмники в пещерах с тварями.",

	//29-31 singular versions 
	L"1 новобранец из ополчения стал ветераном ополченцем.",
	L"1 новобранец из ополчения стал рядовым ополченцем.",
	L"1 рядовой ополченец стал ветераном ополченцем.",

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
	// HEADROCK HAM 3.5: Changed to reflect facility effect.
	L"В этом секторе ополченцев могут тренировать не более %d человек.",	//No more than %d militia trainers are permitted in this sector.
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

	L"Сопроводить Джона и Мэри?",
	
	// 60
	L"Кнопка нажата.",

	L"%s чувствует, что в бронежилете что-то треснуло!",
	L"%s выпустил на %d больше пуль!",
	L"%s выпустил на одну пулю больше!",

	L"Сперва закрой описание предмета!",
		
	L"Невозможно ускорить время - враждебные гражданские или кошки-убийцы в секторе.", // 65
};

// HEADROCK HAM 3.5: Added sector name
STR16 gzCWStrings[] = 
{
	L"Вызвать подкрепление из соседних секторов для %s?",
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
	L"%s|Текущие |О|Д: %d\n",
	L"%s|Текущее |Здоровье: %d\n",
	L"%s|Текущая |Энергия: %d\n",
	L"%s|Текущая |Мораль: %d\n",
	L"%s|Текущий |Шок: %d\n",
	L"%s|Текущий |Уровень |Подавления: %d\n",
	// Full info
	L"%s|Каска: %s\n",
	L"%s|Жилет: %s\n",
	L"%s|Брюки: %s\n",
	// Limited, Basic
	L"|Броня: ",
	L"Каска",
	L"Жилет",
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
	L"Пистолет-пулемёт",
	L"Винтовка",
	L"Ручной пулемёт",
	L"Дробовик",
	L"Нож",
	L"Тяжелое оружие",
	L"без каски",
	L"без бронежилета",
	L"без поножей",
	L"|Броня: %s\n", 
	 // Added - SANDRO
	L"%s|Навык 1: %s\n",	//%s|Skill 1: %s\n
	L"%s|Навык 2: %s\n", 
	L"%s|Навык 3: %s\n",
    // Additional suppression effects - sevenfm
    L"%s|О|Д потеряно от |Подавления: %d\n",
    L"%s|Сопротивление |Подавлению: %d\n",
    L"%s|Эффективный |Уровень |Шока: %d\n",
    L"%s|A|I |Мораль: %d\n",
};

STR16 New113Message[] =
{
	L"Началась буря.",
	L"Буря закончилась.",
	L"Начался дождь.",
	L"Дождь закончился.",
	L"Опасайтесь снайперов...",
	L"Огонь на подавление!",	//suppression fire!
	L"*",			//BRST - стабильна по количеству выпущенных пуль
	L"***",			//AUTO - регулируемая очередь
	L"ГР",
	L"ГР *",
	L"ГР ***",
	L"ПС",
	L"ПС *",
	L"ПС ***",
	L"Штык",
	L"Снайпер!",
	L"Невозможно разделить деньги из-за предмета на курсоре.",
	L"Точка высадки прибывающих наёмников перенесена в %s, так как запланированное место высадки %s захвачено противником.",
	L"Выброшена вещь.",
	L"Выброшены все вещи выбранной группы.",
	L"Вещь продана голодающему населению Арулько.",
	L"Проданы все вещи выбранной группы.",
	L"Проверьте, что вашим бойцам мешает лучше видеть.",	//You should check your goggles
	// Real Time Mode messages
	L"Уже в бою.",	//In combat already
	L"В пределах видимости нет врагов.",	//No enemies in sight
	L"Красться в режиме реального времени ОТКЛ.",	//Real-time sneaking OFF
	L"Красться в режиме реального времени ВКЛ.",	//Real-time sneaking ON
	//L"Enemy spotted! (Ctrl + x to enter turn based)",
	L"Обнаружен враг!", // this should be enough - SANDRO
	//////////////////////////////////////////////////////////////////////////////////////
	// These added by SANDRO
	L"%s отлично справился с кражей!",	//%s was successful at stealing!
	L"%s: недостаточно очков действия, чтобы украсть все выбранные вещи.",	//%s did not have enough action points to steal all selected items.
	L"Хотите провести хирургическую операцию %s перед перевязкой? (Вы сможете восстановить около %i здоровья).",	//Do you want to perform surgery on %s before bandaging? (You can heal about %i Health.)
	L"Хотите провести хирургическую операцию %s? (Вы сможете восстановить около %i здоровья).",	//Do you want to perform surgery on %s? (You can heal about %i Health.)
	L"Do you wish to perform surgeries first? (%i patient(s))",	// TODO.Translate
	L"Хотите провести операцию сначала этому пациенту?",	//Do you wish to perform the surgery on this patient first?
	L"Apply first aid automatically with surgeries or without them?",
	L"Do you want to perform surgery on %s before bandaging? (You can heal about %i health, *: %i by blood bag use.)",	// TODO.Translate
	L"Do you want to perform surgery on %s? (You can heal about %i Health, *: %i by blood bag use.)",
	L"Do you wish to perform surgery on %s? (You can heal about %i Health, *: %i by blood bag use.)",
	L"%s успешно прооперирован(а).",	//Surgery on %s finished.
	L"%s пропустил(а) удар в грудную клетку и теряет единицу максимального значения здоровья!",	//%s is hit in the chest and loses a point of maximum health!
	L"%s пропустил(а) удар в грудную клетку и теряет %d максимального значения здоровья!",	//%s is hit in the chest and loses %d points of maximum health!
	L"%s ослеплен взрывом!",
	L"%s восстановил(а) одну ед. потерянного %s.",	//%s has regained one point of lost %s
	L"%s восстановил(а) %d ед. потерянного %s.",	//%s has regained %d points of lost %s
	L"Ваши навыки разведчика сорвали засаду противника.",
	L"Благодаря вашим навыкам разведчика вы успешно избежали встречи с кошками-убицами!",	
	L"%s получает удар в пах и падает на землю от адской боли!",
	//////////////////////////////////////////////////////////////////////////////////////
	L"Внимание: враг обнаружил труп!!!",
	L"%s [%d патр.]\n%s %1.1f %s",
	L"Недостаточно ОД! Нужно %d, у вас есть %d.",
	L"Совет: %s",
	L"Сила игрока: %d - Сила противника: %6.0f",

	L"Нельзя использовать навык!",
	L"Нельзя строить, пока противник в секторе!",
	L"Невозможно наблюдать за этим местом!",
	L"Некорректные координаты для стрельбы артиллерии!",
	L"Помехи на радиочастотах. Связь невозможна!",
	L"Не удалось использовать радиостанцию!",
	L"Недостаточно миномётных снарядов в секторе для постановки огня!",
	L"Не обнаружены сигнальные мины в Items.xml!",
	L"Нет миномётов, невозможно организовать артналет!",
	L"Режим радиопомех уже включен, нет необходимости делать это снова!",
	L"Режим прослушивания звуков уже включен, нет необходимости делать это снова!",
	L"Режим обнаружения уже включен, нет необходимости делать это снова!",
	L"Режим обнаружения источника помех уже включен, нет необходимости делать это снова!",
	L"%s не может применить %s к %s.",
	L"%s вызывает подкрепления из %s.",
	L"%s радиостанция обесточен.",
	L"работающая радиостанция",
	L"бинокль",
	L"терпение",
	L"%s's shield has been destroyed!",	// TODO.Translate
	L" DELAY",	// TODO.Translate
	L"Yes*",
	L"Yes",
	L"No",
	L"%s applied %s to %s.", // TODO.Translate
};

STR16 New113HAMMessage[] = 
{
	// 0 - 5
	L"%s в страхе пытается укрыться!",
	L"%s прижат(а) к земле вражеским огнём!",
	L"%s дал более длинную очередь!",
	L"Вы не можете тренировать ополчение в этом секторе.",
	L"Ополченец подобрал %s.",
	L"Невозможно тренировать ополчение, пока в секторе враги!",
	// 6 - 10
	L"%s имеет низкий навык Лидерства, чтобы тренировать ополченцев.",
	L"В этом секторе может быть не больше %d тренеров патрульных групп.",
	L"Нет свободных мест в %s или вокруг него для новой патрульной группы!",
	L"Нужно иметь по %d ополченцев в каждом освобождённом секторе города %s, прежде чем можно будет тренировать патруль.",
	L"Невозможно назначить занятие, пока враг в секторе!",
	// 11 - 15
	L"%s имеет недостаточно интеллекта для этого занятия.",
	L"Учереждение %s полностью укомплектованно персоналом.",
	L"Один час этого назначения обойдётся вам в $%d. Согласны оплачивать?",
	L"У вас недостаточно денег, чтобы оплатить за сегодня. $%d выплачено, ещё нужно $%d. Местным это не понравилось.",
	L"У вас недостаточно денег, чтобы выплатить заработную плату всем рабочим. Теперь долг составил $%d. Местным это не понравилось.",
	// 16 - 20
	L"Непогашенный долг составляет $%d, и нет денег, чтобы его погасить!",
	L"Непогашенный долг составляет $%d. Вы не можете выбрать это назначение, пока не погасите задолженность.",
	L"Непогашенный долг составляет $%d. Выплатить деньги по задолженности?",
	L"Н/Д в этом секторе",
	L"Дневной расход",
	// 21 - 25
	L"Недостаточно денег для выплат нанятому ополчению. %d ополченцев было распущено и отправлено домой.",
	L"Для того, чтобы изучить характеристики предмета во время боя, вам нужно сначала взять его.",
	L"Для того, чтобы во время боя присоединить один предмет к другому, вам нужно сначала взять их.",
	L"Для объединения предметов во время боя вам нужно сначала взять их.",
};

// HEADROCK HAM 5: Text dealing exclusively with Item Transformations.
STR16 gzTransformationMessage[] = 
{
	L"Нет доступных преобразований",	
	L"%s был разделен на несколько частей.",
	L"%s был разделен на несколько частей. Предметы находятся в инвентаре %s.",
	L"Из-за нехватки места в инвентаре %s после преобразования некоторые предметы были брошены на землю.",
	L"%s был разделен на несколько частей. Из-за нехватки места в инвентаре %s пришлось бросить некоторые предметы на землю.",
	L"Преобразовать все %d предметов вместе? (Для того, чтобы преобразовать только один предмет, предварительно отделите его)",
	// 6 - 10
	L"Разделить ящик и поместить в инвентарь",
	L"Разделить на магазины емкостью %d",
	L"%s был разделен на %d магазинов по %d патронов в каждом.",
	L"%s был разделен и помещен в инвентарь %s.",
	L"Недостаточно места в инвентаре %s для магазинов данного калибра!",
	L"Instant mode",	// TODO.Translate
	L"Delayed mode",
	L"Instant mode (%d AP)",
	L"Delayed mode (%d AP)",
};

// WANNE: This hardcoded text should not be used anymore in the game, because we now have those texts externalized in the "TableData\Email\EmailMercLevelUp.xml" file!
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

// WANNE: This hardcoded text should not be used anymore in the game, because we now have those texts externalized in the "TableData\Email\EmailMercAvailable.xml" file!
// WANNE: This is email text (each 2 line), when we left a message on AIM and now the merc is back
STR16	New113AIMMercMailTexts[] =
{
	// Monk
	L"FW с сервера A.I.M.: Письмо от Виктора Колесникова",
	L"Привет. Это Монк. Получил твое сообщение. Я вернулся, так что можешь со мной связаться. ± ± Жду звонка. ±",

	// Brain
	L"FW с сервера A.I.M.: Письмо от Янно Аллика",
	L"Я готов обсудить задания. Для всего есть свое время и место. ± ± Янно Аллик ±",

	// Scream
	L"FW с сервера A.I.M.: Письмо от Леннарта Вильде",
	L"Леннарт Вильде вернулся!",

	// Henning
	L"FW с сервера A.I.M.: Письмо от Хеннинга фон Браница",
	L"Получил твое сообщение, спасибо. Если хочешь обсудить работу, свяжись со мной на сайте A.I.M. До встречи! ± ± Хеннинг фон Браниц ±",

	// Luc
	L"FW с сервера A.I.M.: Письмо от Люка Фабра",
	L"Послание получил, мерси! С удовольствием рассмотрю ваши предложения. Вы знаете, где меня найти. ± ± Жду с нетерпением ±",

	// Laura
	L"FW с сервера A.I.M.: Письмо от Лоры Колин",
	L"Привет! Спасибо, что оставили сообщение. Звучит интересно. ± ± Зайдите снова в A.I.M. Хотелось бы услышать больше. ± ± С уважением! ± ± Др. Лора Колин ± ± P.S. Надеюсь, Monk уже в вашей команде? ±",

	// Grace
	L"FW с сервера A.I.M.: Письмо от Грациеллы Джирелли",
	L"Вы хотели связаться со мной, но неудачно.± ± Семейное собрание. Думаю, вы понимаете. Я уже устала от семьи и буду рада. Если вы снова свяжетесь со мной через сайт A.I.M. ± ± Чао! ±",

	// Rudolf
	L"FW с сервера A.I.M.: Письмо от Рудольфа Штайгера",
	L"Ты знаешь, сколько звонков я получаю каждый день? Любой придурок считает, что может позвонить мне. ± ± Но я вернулся, если тебе есть чем меня заинтересовать. ±",

	// WANNE: Generic mail, for additional merc made by modders, index >= 178
	L"FW с сервера A.I.M.: Наёмник доступен",
	L"Я на месте. Жду звонка чтобы обсудить условия контракта. ±",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper
	L"Снайпер: У вас глаза ястреба. В свободное время вы развлекаетесь, отстреливая крылышки у мух с расстояния 100 метров! ± ",	//Sniper: Eyes of a hawk, you can shoot the wings from a fly at a hundred yards!
	// Camouflage
	L"Маскировка: На вашем фоне даже кусты выглядят синтетическими! ± ",
	// SANDRO - new strings for new traits added
	// MINTY - Altered the texts for more natural English, and added a little flavour too
	// Ranger
	L"Рейнджер: Эти любители из Техаса вам и в подметки не годятся! ± ",
	// Gunslinger
	L"Ковбой: С одним револьвером или с двумя - вы так же опасны, как Билли Кид! ± ",
	// Squadleader
	L"Командир: Вы прирождённый лидер, солдаты просто боготворят вас! ± ",
	// Technician
	L"Механик: Ангус МакГайвер по сравнению с вами просто никто! Механика, электроника или взрывчатка - вы отремонтируете что угодно! ± ",
	// Doctor
	L"Доктор: Будь то царапины или вскрытое брюхо, требуется ампутация или же наоборот, пришить что-нибудь - вы с лёгкостью справитесь с любым недугом! ± ",
	// Athletics
	L"Спортсмен: Ваша скорость и выносливость достойны олимпийца! ± ",
	// Bodybuilding
	L"Культурист: Шварц? Да он слабак! Вы с лёгкостью завалите его одной левой! ± ",
	// Demolitions
	L"Подрывник: Сеять гранаты, как семена по полю; минировать поле, как картошку садить - густо и минимум 20 соток; а после созерцать полет конечностей... Вот то, ради чего вы живёте! ± ",
	// Scouting
	L"Разведчик: Ничто не скроется от вашего зоркого взгляда! ± ",
	// Covert ops
	L"Шпион: Агент 007 по сравнению с вами - дилетант! ± ",
	// Radio Operator
	L"Радист: Использование вами средств связи позволяет расширить тактические и стратегические возможности команды. ± ",
	// Survival
	L"Выживание: В условиях дикой природы вы чувствуете себя как дома. ± ",
};

STR16 NewInvMessage[] = 
{
	L"В данный момент поднять рюкзак нельзя.",
	L"Вы не можете одновременно носить 2 рюкзака.",
	L"Вы потеряли свой рюкзак...",
	L"Замок рюкзака работает лишь во время битвы.",
	L"Вы не можете передвигаться с открытым рюкзаком.",
	L"Вы уверены, что хотите продать весь хлам этого сектора голодающему населению Арулько?",
	L"Вы уверены, что хотите выбросить весь хлам, валяющийся в этом секторе?",
	L"Тяжеловато будет взбираться с полным рюкзаком на крышу. Может, снимем?",
	L"Все рюкзаки сняты",
	L"Все рюкзаки надеты",
	L"%s снял(а) рюкзак",
	L"%s надел(а) рюкзак",
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
	L"Отправка файла '%S' - 100/100",	//Sent file '%S' - 100/100
	L"Завершена отправка файлов для '%S'.",	//Finished sending files to '%S'.
	L"Начата отправка файлов для '%S'.",	//Started sending files to '%S'.
	L"Используйте обзор воздушного пространства, чтобы выбрать карту для игры. Если вы ходите сменить карту, это нужно сделать до того, как вы нажмете кнопку 'Начать игру'.",
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
	L"Продолжение после перехвата",	//Finish from interrupt
	L"Координаты курсора:",		//Mouse Grid Coordinates
	// 20
	L"X: %d, Y: %d",
	L"Номер сектора: %d",		//Grid Number
	L"Доступно лишь для сервера.",
	L"Выберите, какую ступень игры принудительно запустить:  ('1' - открыть лэптоп/найм бойцов)  ('2' - запустить/загрузить уровень)  ('3' - разблокировать пользовательский интерфейс)  ('4' - завершить расстановку)",
	L"Sector=%s, Max Clients=%d, Max Mercs=%d, Game_Mode=%d, Same Merc=%d, Damage Multiplier=%f, Timed Turns=%d, Secs/Tic=%d, Dis BobbyRay=%d, Dis Aim/Merc Equip=%d, Dis Morale=%d, Testing=%d",
	// 25
	L"",	//not used any more
	L"Новый игрок: клиент №%d - '%S'.",
	L"Команда: %d.",//not used any more
	L"%s (клиент %d - '%S') был убит %s (клиент %d - '%S')",
	L"Клиент №%d - '%S' выкинут из игры.",
	// 30
	L"Принудительно дать ход клиенту. №1: <Отменить>, №2: %S, №3: %S, №4: %S",
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
	L"Вы не можете изменить карту после начала закупки.",
	L"Карта изменена на '%s'",
	L"Клиент '%s' отключился, убираем его из игры.",
	L"Вы были отключены от игры, возвращаемся в главное меню.",
	L"Подключиться не удалось. Повторная попытка через 5 секунд (осталось %i попыток)",
	//50
	L"Подключиться не удалось, сдаюсь...",
	L"Вы не можете начать игру во время подключения других игроков.",
	L"%s : %s",
	L"Отправить всем",
	L"Только союзникам",
	// 55
	L"Не могу присоединиться к игре. Игра уже началась.",
	L"%s (команда): %s",
	L"#%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"От сервера получены все необходимые файлы.",
	L"'%S' закачка с сервера завершена.",
	L"'%S' начата закачка с сервера.",
	L"Нельзя начать игру пока все игроки не завершать приём файлов от сервера.",
	L"Для игры на этом сервере необходимо скачать некоторые изменённые файлы, желаете продолжить?",
	// 65
	L"Нажмите 'Готов' для входа на тактическую карту.",
	L"Не удаётся подключиться. Версия вашего клиента (%S) отличается от версии сервера (%S).",
	L"Вы убили вражеского солдата.",
	L"Нельзя запустить игру потому что все команды одинаковые.",
	L"Игра на сервере создана с Новым Инвентарём (NIV), а выбранное вами разрешение экрана не поддерживается NIV.",
	// 70
	L"Невозможно сохранить принятый файл '%S'",
	L"%s's бомба была разряжена %s",
	L"Вы проиграли. Стыд и срам!",	// All over red rover
	L"Режим наблюдателя выключен",
	L"Укажите номер клиента, который нужно кикнуть. №1: <Отменить>, №2: %S, №3: %S, №4: %S",
	// 75
	L"Команда %s уничтожена.",
	L"Ошибка при запуске клиента. Завершение операции.",
	L"Клиент отсоединился и закрыт.",
	L"Клиент не запущен.",
	L"ИНФОРМАЦИЯ: Если игра зависла (полоса прогресса противника не двигается), сообщите серверу, чтобы нажал ALT + E для передачи хода обратно вам!",
	// 80
	L"ход AI - осталось %d",
};

STR16 gszMPEdgesText[] =
{
	L"С",	//N
	L"В",	//E
	L"Ю",	//S
	L"З",	//W
	L"Ц",	// "C"enter
};

STR16 gszMPTeamNames[] =
{
	L"Фокстрот",	//Foxtrot
	L"Браво",	//Bravo
	L"Дельта",	//Delta
	L"Чарли",	//Charlie
	L"Н/Д",		// Acronym of Not Applicable
};

STR16 gszMPMapscreenText[] =
{
	L"Тип игры: ",		//Game Type: 
	L"Игроков: ",		//Players: 
	L"Всего бойцов: ",	//Mercs each: 
	L"Нельзя изменять сторону высадки отряда после открытия лэптопа.",
	L"Нельзя изменить имя команды после открытия лэптопа.",
	L"Случ. бойцы: ",	//Random Mercs: 
	L"Да",			//Y
	L"Сложность:",		//Difficulty:
	L"Версия сервера:",	//Server Version:
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
	L"Дождитесь, пожалуйста, пока сервер нажмёт кнопку 'Продолжить'."
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
	L"Отправть всем",
	L"Отправть только союзникам",
};

STR16 gzMPChatboxText[] =
{
	L"Чат сетевой игры Jagged Alliance 2 v1.13",
	L"Заметка: нажмите |В|В|О|Д для отправки сообщения, |К|Л|Ю|Ч для выхода из чата.",
};

// Following strings added - SANDRO
STR16 pSkillTraitBeginIMPStrings[] =
{
	// For old traits
	L"На следующей странице вам нужно выбрать профессиональные навыки в соответствии со специализацией вашего наёмника. Вы можете выбрать не более двух разных навыков, либо один и владеть им в совершенстве.",
	L"Можно выбрать всего один навык или вообще остаться без него. Тогда вам будут даны дополнительные баллы для улучшения некоторых параметров. Внимание: навыки электроники, стрельбы с двух рук и маскировки не могут быть экспертными.",
	// For new major/minor traits
	L"Следующий этап - выбор навыков, которые определят специализацию вашего наёмника. На первой странице можно выбрать до %d основных навыков, которые определят роль бойца в отряде. На второй - дополнительные навыки, подчеркивающие личные качества бойца.", 
	L"Всего можно взять не более %d навыков. Так, если вы не выбрали основной навык, то можно взять %d дополнительных. Если же вы выбрали оба основных навыка (или один улучшенный), то будет доступен лишь %d дополнительный...",  
};

STR16 sgAttributeSelectionText[] = 
{
	L"Откорректируйте свои физические параметры согласно вашим истинным способностям. И не стоит их завышать.",
	L"I.M.P.: Параметры и умения.",	//I.M.P. Attributes and skills review.
	L"Бонус:",	//Bonus Pts.
	L"Ваш уровень",	//Starting Level
	// New strings for new traits
	L"На следующей странице укажите свои физические параметры и умения. \"Физические параметры\" - это здоровье, ловкость, проворность, сила и интеллект. Они не могут быть ниже %d.", 
	L"Оставшиеся \"умения\", в отличие от физических параметров, могут быть установлены в ноль, что означает абсолютную некомпетентность в данных областях.", 
	L"Изначально все параметры установлены на минимум. Заметьте, что минимум для некоторых параметров определяется выбранными навыками, и вы не можете понизить их значение.",
};

STR16 pCharacterTraitBeginIMPStrings[] =
{
	L"I.M.P.: Анализ личных качеств",	//I.M.P. Character Analysis
	L"Следующий шаг - анализ ваших личных качеств. На первой странице вам на выбор будет предложен список черт характера. Уверены, что вам могут быть свойственны несколько из указанных черт, но выбрать нужно лишь одну. Выберите лишь самую ярко выраженную вашу черту характера.",
	L"На второй странице вам будет предложен список проблем, которые, возможно, есть у вас. Если найдёте свою проблемы в списке, отметьте её. Будьте предельно честны при ответах, очень важно предоставить вашим потенциальным работодателям достоверную информацию о вас.",
};

STR16 gzIMPAttitudesText[]=
{
	L"Адекватный",	//Normal
	L"Общительный",	//Friendly
	L"Одиночка",	//Loner
	L"Оптимист",	//Optimist
	L"Пессимист",	//Pessimist
	L"Агрессивный",	//Aggressive
	L"Высокомерный",	//Arrogant
	L"Крутой",	//Big Shot
	L"Мудак",	//Asshole
	L"Трус",	//Coward
	L"I.M.P.: Жизненная позиция",	//I.M.P. Attitudes
};

STR16 gzIMPCharacterTraitText[]=
{
	L"Обычный",	//Neutral
	L"Общительный",	//Sociable
	L"Одиночка",	//Loner
	L"Оптимист",	//Optimist
	L"Самоуверенный",	//Assertive
	L"Мозговитый",	//Intellectual
	L"Простофиля",	//Primitive
	L"Агрессивный",	//Aggressive
	L"Флегматик",	//Phlegmatic
	L"Бесстрашный",	//Dauntless
	L"Миролюбивый",	//Pacifist
	L"Злобный",	//Malicious
	L"Хвастун",	//Show-off
	L"Трус",
	L"I.M.P.: Личностные качества",	//I.M.P. Character Traits
};

STR16 gzIMPColorChoosingText[] = 
{
	L"I.M.P.: Расцветка и телосложение",
	L"I.M.P.: Расцветка",
	L"Выберите соответствующие цвета вашей кожи, волос и одежды, а также укажите ваше телосложение.",
	L"Выберите соответствующие цвета вашей кожи, волос и одежды.",
	L"Отметьте здесь, чтобы ваш персонаж \nдержал автомат одной рукой.",
	L"\n(Важно: вам понадобится прилично сил для этого.)",
};

STR16 sColorChoiceExplanationTexts[]=
{
	L"Цвет волос",	//Hair Color
	L"Цвет кожи",	//Skin Color
	L"Цвет майки",	//Shirt Color
	L"Цвет штанов",	//Pants Color
	L"Нормальное телосложение",	//Normal Body
	L"Мускулистое телосложение",	//Big Body
};

STR16 gzIMPDisabilityTraitText[]=
{
	L"Нет проблем",		//No Disability
	L"Непереносимость жары",	//Heat Intolerant
	L"Нервный",		//Nervous
	L"Клаустрафоб",	//Claustrophobic
	L"Не умеющий плавать",	//Nonswimmer
	L"Боязнь насекомых",	//Fear of Insects
	L"Забывчивый",		//Forgetful
	L"Психопат",		//Psychotic
	L"Глухой",		//Deaf
	L"Близорукий",		//Shortsighted
	L"Гемофилик",		// Hemophiliac
	L"Боязнь высоты",		// Fear of Heights
	L"Самоистезание",		// Self-Harming
	L"I.M.P.: Проблемы",	//I.M.P. Disabilities
};

STR16 gzIMPDisabilityTraitEmailTextDeaf[] =
{
	L"Можем поспорить - вы рады, что это не голосовая почта.",
	L"Вы слишком часто слушали громкую музыку на дискотеке или слишком близко слушали взрывы снарядов. Или это просто возраст. В любом случае, вашей команде стоит изучить язык жестов.",
};

STR16 gzIMPDisabilityTraitEmailTextShortSighted[] =
{
	L"Вы будете бесполезны, если потеряете свои очки.",
	L"Это случается, если проводить много времени перед светящимися прямоугольниками. Нужно было есть больше морковок. Вы когда-нибудь видели кролика в очках?",
};

STR16 gzIMPDisabilityTraitEmailTextHemophiliac[] =
{
	L"Вы УВЕРЕНЫ, что это подходящая работа для Вас?",
	L"Разве что Вы так круты, что никогда не получаете ран или сражаетесь только в хорошо оснащенном госпитале.",
};

STR16 gzIMPDisabilityTraitEmailTextAfraidOfHeights[]=
{
	L"Скажем просто, вы любите быть только на твердой земле.",
	L"Вам больше нравятся миссии, в которых не надо покорять горы и лезть на высокие здания. Рекомендуем завоевать Голландию.",
};

STR16 gzIMPDisabilityTraitEmailTextSelfHarm[] =
{
	L"Возможно вы хотите убедиться в чистоте ваших ножей.",
	L"У вас определенные проблемы с ножами. Т.е. вы не избегаете из, а даже наоборот, серьезно!",
};

// HEADROCK HAM 3.6: Error strings for assigning a merc to a facility
STR16 gzFacilityErrorMessage[]=
{
	L"%s не хватает Силы, чтобы выполнить это действие.",
	L"%s не хватает Ловкости, чтобы выполнить это действие.",
	L"%s не хватает Проворности, чтобы выполнить это действие.",
	L"%s не хватает Здоровья, чтобы выполнить это действие..",
	L"%s не хватает Интеллекта, чтобы выполнить это действие.",
	L"%s не хватает Меткости, чтобы выполнить это действие.",
	// 6 - 10
	L"%s недостаточно развит Медицинский навык, чтобы выполнить это действие.",
	L"%s недостаточно развит навык Механики, чтобы выполнить это действие.",
	L"%s недостаточно развито Лидерство, чтобы выполнить это действие.",
	L"%s недостаточно развит навык Взрывчатки, чтобы выполнить это действие.",
	L"%s недостаточно Опыта, чтобы выполнить это действие.",
	// 11 - 15
	L"У %s слишком плохой боевой дух, чтобы выполнить это действие",
	L"%s слишком устал(а), чтобы выполнить это действие.",
	L"В городе %s вам пока не доверяют. Местные отказываются выполнить этот приказ.",
	L"Слишком много людей уже работают в %s.",
	L"Слишком много людей уже выполняют эту задачу в %s.",
	// 16 - 20
	L"%s не может найти вещи, которые нуждаются в ремонте.",
	L"%s потерял(а) часть %s, пока работал в секторе %s!",
	L"%s потерял(а) часть %s, пока работал над %s в %s!",
	L"%s получил(а) травму, пока работал(а) в секторе %s, и требует незамедлительной медицинской помощи!",
	L"%s получил(а) травму, пока работал(а) над %s в %s, и требует незамедлительной медицинской помощи!",
	// 21 - 25
	L"%s получил(а) травму, пока работал(а) в секторе %s. Травма незначительная.",
	L"%s получил(а) травму, пока работал(а) над %s в %s. Травма незначительная.",
	L"Жители города %s расстроены тем, что %s пребывает в их городе.",
	L"Жители города %s расстроены работой %s в %s.",
	L"%s в секторе %s своими действиями понизил репутацию во всём регионе!",
	// 26 - 30
	L"%s, работая над %s в %s, привёл(а) к понижению репутации во всём регионе!",
	L"%s пьян(а).",
	L"%s заболел(а) в секторе %s и вынужден(а) отложить текущую задачу.",
	L"%s заболел(а) и не может продолжить работу над %s в %s.",
	L"%s получил(а) травму в секторе %s.",
	// 31 - 35
	L"%s получил(а) серьёзную травму в секторе %s.",
	L"Здесь есть пленные, которые осведомлены о личности %s.",
	L"%s хорошо известен как доносчик. Подождите еще хотя бы %d часов.",


};

STR16 gzFacilityRiskResultStrings[]=
{
	L"Сила",	//Strength
	L"Проворность",	//Agility
	L"Ловкость",	//Dexterity
	L"Интеллект",	//Wisdom
	L"Здоровье",	//Health
	L"Меткость",	//Marksmanship
	// 5-10
	L"Лидерство",	//Leadership
	L"Механика",	//Mechanical skill
	L"Медицина",	//Medical skill
	L"Взрывчатка",	//Explosives skill
};

STR16 gzFacilityAssignmentStrings[]=
{
	L"Окружающая среда",	//AMBIENT
	L"Штат",	//Staff
	L"Питание",
	L"Отдых",
	L"Ремонт вещей",
	L"Ремонт %s", // Vehicle name inserted here
	L"Ремонт робота",
	// 6-10
	L"Доктор",
	L"Пациент",
	L"Тренировка Силы",
	L"Тренировка Ловкости",
	L"Тренировка Проворности",
	L"Тренировка Здоровья",
	// 11-15
	L"Тренировка Меткости",
	L"Тренировка Медицины",
	L"Тренировка Механики",
	L"Тренировка Лидерства",
	L"Тренировка Взрывчатки",
	// 16-20
	L"Ученик на Силу",
	L"Ученик на Ловкость",
	L"Ученик на Проворность",
	L"Ученик на Здоровье",
	L"Ученик на Меткость",
	// 21-25
	L"Ученик на Медицину",
	L"Ученик на Механику",
	L"Ученик на Лидерство",
	L"Ученик на Взрывчатку",
	L"Тренер на Силу",
	// 26-30
	L"Тренер на Ловкость",
	L"Тренер на Проворность",
	L"Тренер на Здоровье",
	L"Тренер на Меткость",
	L"Тренер на Медицину",
	// 30-35
	L"Тренер на Механику",
	L"Тренер на Лидерство",
	L"Тренер на Взрывчатку",
	L"Допрашивать пленных",
	L"Осведомитель",
	// 36-40
	L"Вести пропаганду",
	L"Вести пропаганду",	// spread propaganda (globally)
	L"Собирать слухи",
	L"Командовать ополчением",	// militia movement orders
};
STR16 Additional113Text[]=
{
	L"Для запуска Jagged Alliance 2 v1.13 в оконном режиме требуется установить 16-битное качество цветопередачи экрана",
	L"Jagged Alliance 2 v1.13 в полноэкранном режиме с разрешением %d x %d не поддерживается установками вашего экрана.\nИзмените разрешение игры или используйте 16-битный оконный режим.",
	L"Внутренняя ошибка при чтении места %s сохранённой игры: Число мест в сохранённой игре (%d) отличается от определенных параметрами настроек (%d) в ja2_options.ini",
	// WANNE: Savegame slots validation against INI file 
	L"Наёмники (MAX_NUMBER_PLAYER_MERCS) / Машины (MAX_NUMBER_PLAYER_VEHICLES)", 
	L"Противник (MAX_NUMBER_ENEMIES_IN_TACTICAL)", 
	L"Твари (MAX_NUMBER_CREATURES_IN_TACTICAL)", 
	L"Ополчение (MAX_NUMBER_MILITIA_IN_TACTICAL)", 
	L"Гражданские (MAX_NUMBER_CIVS_IN_TACTICAL)",

};

// SANDRO - Taunts (here for now, xml for future, I hope)
// MINTY - Changed some of the following taunts to sound more natural
STR16 sEnemyTauntsFireGun[]=
{
	L"Отведай-ка гостинца!",
	L"Поздоровайся с моим дружком!",
	L"Иди и получи!",
	L"Ты мой!",
	L"Сдохни!",
	L"Обосрался, говнюк?",
	L"Будет больно!",
	L"Давай, ублюдок!",
	L"Давай! Не весь же день тягаться!",
	L"Иди к папочке!",
	L"Закопаю моментом!",
	L"Домой поедешь в деревянном костюме, неудачник!",
	L"Эй, сыграем?",
	L"Сидел бы дома, мудила!",
	L"С-сука!",
};

STR16 sEnemyTauntsFireLauncher[]=
{

	L"Будет, будет... Шашлык из тебя будет!",
	L"Держи подарочек!",
	L"Бах!",
	L"Улыбочку!",
};

STR16 sEnemyTauntsThrow[]=
{
	L"Лови!",
	L"Держи!",
	L"Бум-бах, ой-ой-ой! Умирает зайчик мой!",
	L"Это тебе.",
	L"Муа-ха-ха!",
	L"Лови, свинтус!",
	L"Обожаю этот момент.",
};

STR16 sEnemyTauntsChargeKnife[]=
{
	L"Твой скальп мой, лошара!",
	L"Иди к папочке.",
	L"Сейчас посмотрим на твои кишочки!",
	L"Порву, как Тузик грелку!",
	L"Мясо!",
};

STR16 sEnemyTauntsRunAway[]=
{
	L"Кажется, мы в дерьме...",
	L"Мне говорили вступать в армию, а не в это дерьмо!",
	L"С меня хватит!",
	L"О мой Бог!",
	L"Нам не доплачивают за это дерьмо, валим отсюда...",
	L"Мамочка!",
	L"Я вернусь! И нас будут тысячи!",

};

STR16 sEnemyTauntsSeekNoise[]=
{
	L"Я тебя слышу!",
	L"Кто здесь?",
	L"Что это было?",
	L"Эй! Какого...",

};

STR16 sEnemyTauntsAlert[]=
{
	L"Они здесь!",
	L"Сейчас начнётся веселье!",
	L"Я надеялся, что этого никогда не случится...",

};

STR16 sEnemyTauntsGotHit[]=
{
	L"А-а-г-р-р!",
	L"А-а-а!",
	L"Как же... больно!",
	L"Твою мать!",
	L"Ты пожалеешь... у-м-хх... об этом.",
	L"Что за!..",
	L"Теперь ты меня... разозлил.",

};

STR16 sEnemyTauntsNoticedMerc[]=
{
	L"Что за...!",	
	L"О боже!",
	L"О черт!",
	L"Противник!!!",
	L"Тревога! Тревога!",
	L"Вот он!",
	L"Атаковать!",

};

//////////////////////////////////////////////////////
// HEADROCK HAM 4: Begin new UDB texts and tooltips
//////////////////////////////////////////////////////
STR16 gzItemDescTabButtonText[] =
{
	L"Информация",
	L"Параметры",
	L"Доп. инфо",
};

STR16 gzItemDescTabButtonShortText[] =
{
	L"Инфо.",
	L"Пар.",
	L"Доп.",
};

STR16 gzItemDescGenHeaders[] =
{
	L"Основное",
	L"Дополнительное",
	L"Затраты ОД",
	L"Стрельба очередью",
};

STR16 gzItemDescGenIndexes[] =
{
	L"Парам.",
	L"0",
	L"+/-",
	L"=",
};

STR16 gzUDBButtonTooltipText[]=
{
	L"|И|н|ф|о|р|м|а|ц|и|о|н|н|а|я |ч|а|с|т|ь:\n \nЗдесь вы сможете ознакомиться\nс общим описанием предмета.",
	L"|П|а|р|а|м|е|т|р|ы:\n \nЗдесь вы сможете ознакомиться\nс индивидуальными свойствами\nи параметрами предмета.\n \nДля оружия: нажмите еще раз,\nчтобы открыть вторую страницу.",
	L"|Д|о|п|о|л|н|и|т|е|л|ь|н|а|я| |и|н|ф|о|р|м|а|ц|и|я:\n \nЗдесь вы сможете ознакомиться\nс бонусами, дающимися данным предметом.",
};

STR16 gzUDBHeaderTooltipText[]=
{
	L"|О|с|н|о|в|н|ы|е |п|а|р|а|м|е|т|р|ы:\n \nСвойства и данные этого предмета\n(оружие, броня и т.д.).",
	L"|Д|о|п|о|л|н|и|т|е|л|ь|н|ы|е| |п|а|р|а|м|е|т|р|ы:\n \nДополнительные свойства и/или\nвозможные вторичные характеристики.",
	L"|З|а|т|р|а|т|ы| |О|Д:\n \nКоличество очков действия, необходимых\nна стрельбу и другие действия с оружием.",
	L"|С|т|р|е|л|ь|б|а| |о|ч|е|р|е|д|ь|ю| |- |п|а|р|а|м|е|т|р|ы|:\n \nПараметры данного оружия,\nкасающиеся стрельбы очередью.",
};

STR16 gzUDBGenIndexTooltipText[]=
{
	L"|С|и|м|в|о|л|ь|н|о|е| |о|б|о|з|н|а|ч|е|н|и|е| |п|а|р|а|м|е|т|р|о|в\n \nНаведите курсор на символ,\nчтобы увидеть, что он значит.",
	L"|С|т|а|н|д|а|р|т|н|о|е |з|н|а|ч|е|н|и|е\n \nСтандартное значение праметров предмета\n(без штрафов и бонусов навески и боеприпасов).",
	L"|Б|о|н|у|с|ы| |н|а|в|е|с|к|и\n \nБонусы или штрафы, обусловленные\nнавеской, боеприпасами или повреждениями вещи.",
	L"|С|у|м|м|а|р|н|о|е| |з|н|а|ч|е|н|и|е\n \nСуммарное значение параметров предмета\nс учетом всех бонусов/штрафов навески и боеприпасов",
};

STR16 gzUDBAdvIndexTooltipText[]=
{
	L"Символьное обозначение параметров\n(наведите курсор на символ,\nчтобы увидеть что он значит).",
	L"Бонус/штраф в положении |с|т|о|я.",
	L"Бонус/штраф в положении |с|и|д|я.",
	L"Бонус/штраф в положении |л|ё|ж|а.",
	L"Бонус/штраф",
};

STR16 szUDBGenWeaponsStatsTooltipText[]=
{
	L"|Т|о|ч|н|о|с|т|ь",
	L"|У|р|о|н", 
	L"|Д|а|л|ь|н|о|с|т|ь",
	L"|С|л|о|ж|н|о|с|т|ь |о|б|р|а|щ|е|н|и|я |с |о|р|у|ж|и|е|м",
	L"|Д|о|с|т|у|п|н|ы|е |у|р|о|в|н|и |п|р|и|ц|е|л|и|в|а|н|и|я", 
	L"|К|р|а|т|н|о|с|т|ь |у|в|е|л|и|ч|е|н|и|я |п|р|и|ц|е|л|а",   
	L"|Д|а|л|ь|н|о|с|т|ь |п|р|о|е|ц|и|р|о|в|а|н|и|я", 
	L"|С|к|р|ы|т|а|я |в|с|п|ы|ш|к|а |в|ы|с|т|р|е|л|а", 
	L"|Г|р|о|м|к|о|с|т|ь",
	L"|Н|а|д|ё|ж|н|о|с|т|ь",
	L"|Л|ё|г|к|о|с|т|ь |п|о|ч|и|н|к|и",
	L"|М|и|н|. |р|а|с|с|т|о|я|н|и|е |д|л|я |б|о|н|у|с|а |п|р|и |п|р|и|ц|е|л|и|в|а|н|и|и", 
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|о|п|а|д|а|н|и|я", 	
	L"|О|Д |н|а |в|с|к|и|д|ы|в|а|н|и|е", 
	L"|О|Д |н|а |в|ы|с|т|р|е|л",
	L"|О|Д |н|а |с|т|р|е|л|ь|б|у |о|ч|е|р|е|д|ь|ю", 
	L"|О|Д |н|а |а|в|т|о|м|а|т|и|ч|е|с|к|у|ю |с|т|р|е|л|ь|б|у",
	L"|О|Д |н|а |п|е|р|е|з|а|р|я|д|к|у", 
	L"|О|Д |н|а |д|о|с|ы|л|а|н|и|е |п|а|т|р|о|н|а",
	L"|L|a|t|e|r|a|l |R|e|c|o|i|l",	// No longer used
	L"|П|о|л|н|а|я |о|т|д|а|ч|а",
	L"|К|о|л|-|в|о |п|а|т|р|о|н|о|в |н|а |к|а|ж|д|ы|е |5 |О|Д",
};

STR16 szUDBGenWeaponsStatsExplanationsTooltipText[]=
{
	L"\n \nОпределяет, насколько пули, выпущенные\nиз этого оружия, будут отклоняться от\nточки прицеливания.\n \nДиапазон: 0-100.\nБольше - лучше.",
	L"\n \nОпределяет средний урон от\nпуль, выпущенных из этого оружия,\nне учитывающий броню или ее пробитие.\n \nБольше - лучше.",
	L"\n \nМаксимальное расстояние (в тайлах),\nна которое пролетит пуля из этого оружия,\nпрежде чем начнет падать на землю.\n \nБольше - лучше.",
	L"\n \nОпределяет сложность обращения\nс оружием и стрельбы из него.\n \nБольшая сложность приводит к меньшему\nшансу попасть в цель при прицельной\nи особенно при неприцельной стрельбе.\n \nМеньше - лучше.",
	L"\n \nЭто число доступных уровней прицеливания.\n \nЧем МЕНЬШЕ это число, тем БОЛЬШИЙ бонус\nдается за каждый уровень. То есть\nМЕНЬШЕЕ число уровней позволяет быстрее\nприцеливаться без потери точности.\n \nМеньше - лучше.",
	L"\n \nПри значении больше 1.0 будет уменьшать кол-во\nошибок прицеливания на расстоянии.\n \nПомните, что сильное увеличение прицела снижает\nточность, если цель находится слишком близко.\n \nЗначение 1.0 означает, что оптический прицел не установлен.",
	L"\n \nУменьшает кол-во ошибок прицеливания на расстоянии.\n \nЭтот эффект работает до данного расстояния,\nзатем начинает уменьшаться и, в конце концов,\nпропадает на значительном расстоянии.\n \nБольше - лучше.",
	L"\n \nЕсли это свойство активно, то оружие\nне производит вспышку при выстреле.\n \nВраги не смогут обнаружить вас\nтолько по вспышке выстрела, но по-прежнему будут\nслышать вас.",
	L"\n \nЭто расстояние в тайлах, на которое распространяется\nзвук стрельбы. В пределах этого расстояния\nвраги смогут услышать звук вашего выстрела.\n \nМеньше - лучше.",
	L"\n \nОпределяет, как быстро состояние этого\nоружия ухудшается при использовании.\n \nБольше - лучше.",
	L"\n \nОпределяет сложность починки этого оружия,\nа также то, кто сможет полностью починить его.\nЗеленый - может починить кто угодно.\n \nЖелтый - только техники и некоторые NPC\nмогут починить его свыше порога ремонта.\n \nКрасный - эту вещь починить невозможно.\n \nБольше - лучше.",
	L"\n \nМинимальное расстояние, на котором прицел\nдает бонус к точности прицеливания.",
	L"\n \nМодификатор попадания, дающийся лазерным целеуказателем.",	
	L"\n \nЧисло ОД, необходимое для взятия этого\nоружия на изготовку. После взятия на\nизготовку более не требуется тратить\nэти ОД для всех последующих\nвыстрелов. Оружие автоматически снимается\nс изготовки, если его владелец выполняет\nлюбое другое действие, отличное от стрельбы\nили поворота.\n \nМеньше - лучше.",
	L"\n \nЧисло ОД, необходимое для осуществления\nодиночной атаки этим оружием. Для\nстрелкового оружия - стоимость одиночного\nвыстрела без дополнительного\nприцеливания.\n \nЕсли эта иконка серого цвета, то одиночные\nатаки недоступны для этого оружия.\n \nМеньше - лучше.",
	L"\n \nЧисло ОД, необходимых для стрельбы\nочередью.\n \nЧисло пуль в каждой очереди определяется\nсамим оружием и указано на этой иконке.\n \nЕсли эта иконка серого цвета, то стрельба\nочередью недоступна для этого оружия.\n \nМеньше - лучше.",
	L"\n \nЧисло ОД, необходимых для автоматической\nстрельбы тремя пулями.\n \nЕсли вы хотите выстрелить большим\nчислом пуль, то вам необходимо затратить\nбольшее число ОД.\n \nЕсли эта иконка серого цвета, то автоматическая\nстрельба недоступна для этого оружия.\n \nМеньше - лучше.",
	L"\n \nЧисло ОД, необходимых для перезарядки\nэтого оружия.\n \nМеньше - лучше.",
	L"\n \nЧисло ОД, необходимых для досылания\nпатрона в патронник между выстрелами.\n \nМеньше - лучше.",
	L"\n \nДистанция на которую может сместиться дуло\nв горизонтале между каждой пулей\n в очереди.\n \nПоложительное число указывает на смещение вправо.\nОтрицательное число указывает на смещение влево.\n \nЧем ближе к нулю, тем лучше.", // No longer used
	L"\n \nРасстояние, на которое сдвинется ствол\nпри каждом выстреле в режиме очереди\nили автоматической стрельбы, если не задействуется\nсистема противодействия.\n \nМеньше - лучше.",
	L"\n \nУказывает, какое количество пуль будет\nдобавлено к очереди или залпу при автоматической\nстрельбе за каждые 5 ОД.\n \nБольше - лучше.",
	L"\n \nОпределяет сложность починки этого оружия,\nа также то, кто сможет полностью починить ее.\nЗеленый - может починить кто угодно.\n \nЖелтый - только некоторые NPC\nмогут починить ее свыше порога ремонта.\n \nКрасный - эту вещь починить невозможно.\n \nБольше - лучше.",
};

STR16 szUDBGenArmorStatsTooltipText[]=
{
	L"|С|т|е|п|е|н|ь |з|а|щ|и|т|ы",
	L"|П|о|к|р|ы|т|и|е",
	L"|С|к|о|р|о|с|т|ь |у|х|у|д|ш|е|н|и|я",
	L"|Л|ё|г|к|о|с|т|ь |п|о|ч|и|н|к|и",
};

STR16 szUDBGenArmorStatsExplanationsTooltipText[]=
{
	L"\n \nЭто основное качество брони, оно определяет\nкакой урон будет заблокирован защитой.\n \nПомните, что бронебойные атаки и различные\nслучайные факторы могут повлиять на\nокончательное снижение урона.\n \nБольше - лучше.",
	L"\n \nОпределяет, какая часть защищаемой\nчасти тела покрыта броней. Если покрытие\nменьше 100%, то у любой атаки есть определенный\nшанс на попадание в незащищенную часть тела\nи нанесение ей максимального урона.\n \nБольше - лучше.",
	L"\n \nОпределяет, как быстро состояние этой\nброни ухудшается при попадании в\nнее в зависимости от урона от атаки.\n \nМеньше - лучше.",
	L"\n \nОпределяет сложность починки этой брони,\nа также то, кто сможет полностью починить ее.\nЗеленый - может починить кто угодно.\n \nЖелтый - только техники и некоторые NPC\nмогут починить ее свыше порога ремонта.\n \nКрасный - эту вещь починить невозможно.\n \nБольше - лучше.",
	L"\n \nОпределяет сложность починки этой брони,\nа также то, кто сможет полностью починить ее.\nЗеленый - может починить кто угодно.\n \nЖелтый - только некоторые NPC\nмогут починить ее свыше порога ремонта.\n \nКрасный - эту вещь починить невозможно.\n \nБольше - лучше.",
};

STR16 szUDBGenAmmoStatsTooltipText[]=
{
	L"|П|р|о|б|и|т|и|е |б|р|о|н|и",
	L"|У|р|о|н |п|о|с|л|е |п|р|о|б|и|т|и|я",
	L"|У|р|о|н |п|е|р|е|д |п|о|п|а|д|а|н|и|е|м",
	L"|В|л|и|я|н|и|е |н|а |т|е|м|п|е|р|а|т|у|р|у",
	L"|У|р|о|н |о|т |я|д|а",
	L"|В|л|и|я|н|и|е |н|а |з|а|г|р|я|з|н|е|н|и|е",
};

STR16 szUDBGenAmmoStatsExplanationsTooltipText[]=
{
	L"\n \nСпособность пули пробить броню\nцели. При значении меньше 1.0 пуля снижает\nстепень защиты брони, в которую она\nпопадет. При значении больше 1.0 пуля\nувеличивает степень защиты брони.\n \nМеньше - лучше.",
	L"\n \nОпределяет, как будет изменяться урон\nот пули, пробившей броню.\n \nПри значении больше 1.0 урон увеличивается.\n \nПри значении меньше 1.0 урон уменьшается.\n \nБольше - лучше.",
	L"\n \nМножитель, применяющийся к показателю\nурона непосредственно перед попаданием\nв цель.\n \nПри значении больше 1.0 урон увеличивается.\n \nПри значении меньше 1.0 урон уменьшается.\n \nБольше - лучше.",
	L"\n \nДополнительное тепло, вырабатываемое этими\nбоеприпасами.\n \nМеньше - лучше.",
	L"\n \nОпределяет, какой процент урона пули\nбудет ядовитым.",
	L"\n \nДополнительное загрязнение, вырабатываемое\nэтими боеприпасами.\n \nМеньше - лучше.",
};

STR16 szUDBGenExplosiveStatsTooltipText[]=
{
	L"|У|р|о|н",
	L"|У|р|о|н |о|г|л|у|ш|е|н|и|я",
	L"|В|з|в|р|ы|в |п|р|и |к|о|н|т|а|к|т|е",
	L"|Р|а|д|и|у|с |в|з|р|ы|в|а",
	L"|Р|а|д|и|у|с |о|г|л|у|ш|е|н|и|я",
	L"|Р|а|д|и|у|с |ш|у|м|а", 
	L"|Н|а|ч|а|л|ь|н|ы|й |р|а|д|и|у|с |с|л|е|з|о|т|о|ч|и|в|о|г|о |г|а|з|а",
	L"|Н|а|ч|а|л|ь|н|ы|й |р|а|д|и|у|с |г|о|р|ч|и|ч|н|о|г|о |г|а|з|а",
	L"|Н|а|ч|а|л|ь|н|ы|й |р|а|д|и|у|с |с|в|е|т|а",
	L"|Н|а|ч|а|л|ь|н|ы|й |р|а|д|и|у|с |д|ы|м|а",
	L"|н|а|ч|а|л|ь|н|ы|й |р|а|д|и|у|с |о|г|н|я",
	L"|К|о|н|е|ч|н|ы|й |р|а|д|и|у|с |с|л|е|з|о|т|о|ч|и|в|о|г|о |г|а|з|а",
	L"|К|о|н|е|ч|н|ы|й |р|а|д|и|у|с |г|о|р|ч|и|ч|н|о|г|о |г|а|з|а",
	L"|К|о|н|е|ч|н|ы|й |р|а|д|и|у|с |с|в|е|т|а",
	L"|К|о|н|е|ч|н|ы|й |р|а|д|и|у|с |д|ы|м|а",
	L"|К|о|н|е|ч|н|ы|й |р|а|д|и|у|с |о|г|н|я",
	L"|Д|л|и|т|е|л|ь|н|о|с|т|ь |э|ф|ф|е|к|т|а",
	// HEADROCK HAM 5: Fragmentation
	L"|Ч|и|с|л|о |о|с|к|о|л|к|о|в",
	L"|У|р|о|н |о|т |о|с|к|о|л|к|о|в",
	L"|Д|а|л|ь|н|о|с|т|ь |р|а|з|л|ё|т|а |о|с|к|о|л|к|о|в",
	// HEADROCK HAM 5: End Fragmentations
	L"|Г|р|о|м|к|о|с|т|ь",
	L"|Н|е|с|т|а|б|и|л|ь|н|о|с|т|ь",
	L"|Л|ё|г|к|о|с|т|ь |п|о|ч|и|н|к|и",
};

STR16 szUDBGenExplosiveStatsExplanationsTooltipText[]=
{
	L"\n \nУрон, наносимый взрывом.\n \nОбратите внимание, что бризантная взрывчатка\nнаносит этот урон только один раз (при взрыве),\nа взрывчатка с длительным эффектом наносит\nурон каждый ход, до тех пор, пока эффект\nне закончится.\n \nБольше - лучше.",
	L"\n \nОглушающий урон, наносимый взрывом.\n \nОбратите внимание, что бризантная взрывчатка\nнаносит этот урон только один раз (при взрыве),\nа взрывчатка с длительным эффектом наносит\nурон каждый ход, до тех пор, пока эффект\nне закончится.\n \nБольше - лучше.",
	L"\n \nЭта взрывчатка не будет отскакивать от\nпрепятствий, а взорвется при контакте с ними.",
	L"\n \nРадиус взрывной волны.\n \nЦели будут получать тем меньший урон,\nчем дальше они от эпицентра взрыва.\n \nБольше - лучше.",
	L"\n \nРадиус оглушающей волны.\n \nЦели будут получать тем меньший урон,\nчем дальше они от эпицентра взрыва.\n \nБольше - лучше.",
	L"\n \nРасстояние, которое преодолеет шум от\nэтой ловушки. Солдаты в пределах этого расстояния\nсмогут услышать шум и поднять тревогу.\n \nБольше - лучше.",
	L"\n \nНачальный радиус действия слезоточивого газа.\n \nВраги, попавшие в этот радиус, будут получать\nурон и оглушающий урон каждый ход,\nесли только на них не будет противогаза.\n \nБольше - лучше.",
	L"\n \nНачальный радиус действия горчичного газа.\n \nВраги, попавшие в этот радиус, будут получать\nурон и оглушающий урон каждый ход,\nесли только на них не будет противогаза.\n \nБольше - лучше.",
	L"\n \nНачальный радиус области света.\n \nТайлы ближе к центру будут очень\nяркими, а тайлы ближе к краю - лишь слегка\nярче обычного.\n \nЭффект со временем тускнеет.\n \nБольше - лучше.",
	L"\n \nНачальный радиус облака дыма.\n \nВраги, попавшие в этот радиус, будут получать\nурон и оглушающий урон каждый ход (при наличии),\nесли только на них не будет противогаза.\n Любого, кто окажется в этом облаке, будет\nчрезвычайно трудно заметить, и сам он\nтакже ограничит свое поле зрения.\n \nБольше - лучше.",
	L"\n \nНачальный радиус пожара.\n \nВраги, попавшие в этот радиус, будут получать\nурон и оглушающий урон каждый ход,\nесли только на них не будет противогаза.\n \nБольше - лучше.",
	L"\n \nКонечный радиус действия слезоточивого газа.\n \nВраги, попавшие в этот радиус, будут получать\nурон и оглушающий урон каждый ход,\nесли только на них не будет противогаза.\n \nБольше - лучше.",
	L"\n \nКонечный радиус действия горчичного газа.\n \nВраги, попавшие в этот радиус, будут получать\nурон и оглушающий урон каждый ход,\nесли только на них не будет противогаза.\n \nБольше - лучше.",
	L"\n \nКонечный радиус области света.\n \nТайлы ближе к центру будут очень\nяркими, а тайлы ближе к краю - лишь слегка\nярче обычного.\n \nЭффект со временем тускнеет.\n \nБольше - лучше.",
	L"\n \nКонечный радиус облака дыма.\n \nВраги, попавшие в этот радиус, будут получать\nурон и оглушающий урон каждый ход (при наличии),\nесли только на них не будет противогаза.\n Любого, кто окажется в этом облаке, будет\nчрезвычайно трудно заметить, и сам он\nтакже ограничит свое поле зрения.\n \nБольше - лучше.",
	L"\n \nКонечный радиус пожара.\n \nВраги, попавшие в этот радиус, будут получать\nурон и оглушающий урон каждый ход,\nеслитолько на них не будет противогаза.\n \nБольше - лучше.",
	L"\n \nДлительность эффекта.\n \nКаждый ход радиус эффекта увеличивается\nна один тайл в каждом направлении, пока не\nдостигнет конечного радиуса. По окончании\nвремени действия эффект полностью\nисчезает.\n \nБольше - лучше.",
	// HEADROCK HAM 5: Fragmentation
	L"\n \nЧисло осколков при взрыве.\n \nОсколки действуют по принципу пуль, и они могут\nпопасть в любого, кто стоит достаточно близко\nк взрыву.\n \nБольше - лучше.",
	L"\n \nПотенциальный урон от каждого осколка,\nобразовавшегося при взрыве.\n \nБольше - лучше.",
	L"\n \nСреднее расстояние, на которое полетят осколки\nот взрыва. Некоторые могут пролететь\nгораздо дальше, а некоторые - ближе среднего.\n \nБольше - лучше.",
	// HEADROCK HAM 5: End Fragmentations
	L"\n \nРасстояние в тайлах, в пределах которого\nсолдаты и наёмники услышат звук взрыва.\n \nВраги, услышавшие его, поймут, что вы в\nсекторе.\n \nМеньше - лучше.",
	L"\n \nЭто значение определяет вероятность в\nпроцентах, что эта взрывчатка спонтанно взорвется\nпри ее повреждении (например, при близком взрыве).\n \nНошение с собой в бою нестабильной\nвзрывчатки крайне опасно и не\nрекомендуется.\n \nДиапазон: 0-100.\nМеньше - лучше. ",
	L"\n \nОпределяет сложность починки взрывчатки,\nа также то, кто сможет полностью починить ее.\nЗеленый - может починить кто угодно.\n \nКрасный - эту вещь починить невозможно.\n \nБольше - лучше.",
};
	
STR16 szUDBGenCommonStatsTooltipText[]=
{
	L"|Л|ё|г|к|о|с|т|ь |п|о|ч|и|н|к|и",
	L"|Д|о|с|т|у|п|н|ы|й |о|б|ъ|ё|м",
	L"|О|б|ъ|ё|м",
};

STR16 szUDBGenCommonStatsExplanationsTooltipText[]=
{
	L"\n \nОпределяет сложность починки этого предмета,\nа также то, кто сможет полностью починить его.\nЗеленый - может починить кто угодно.\n \nКрасный - эту вещь починить невозможно.\n \nБольше - лучше.",
	L"\n \nОпределяет сколько доступно места в этом держателе типа MOLLE.\n \nБольше - лучше.",
	L"\n \nОпределяет как много места займёт этот MOLLE карман.\n \nМеньше - лучше.",
};

STR16 szUDBGenSecondaryStatsTooltipText[]=
{
	L"|Т|р|а|с|с|и|р|у|ю|щ|и|е |п|а|т|р|о|н|ы",
	L"|П|р|о|т|и|в|о|т|а|н|к|о|в|ы|е |п|а|т|р|о|н|ы",
	L"|И|г|н|о|р|и|р|у|е|т |б|р|о|н|ю",
	L"|К|и|с|л|о|т|н|ы|е |п|а|т|р|о|н|ы",
	L"|Р|а|з|р|у|ш|а|ю|щ|и|е |з|а|м|к|и |п|а|т|р|о|н|ы",
	L"|У|с|т|о|й|ч|и|в|ы|й |к|о |в|з|р|ы|в|а|м",
	L"|В|о|д|о|н|е|п|р|о|н|и|ц|а|е|м|ы|й",
	L"|Э|л|е|к|т|р|о|н|и|к|а",
	L"|П|р|о|т|и|в|о|г|а|з",
	L"|Н|у|ж|д|а|е|т|с|я |в |б|а|т|а|р|е|й|к|а|х",
	L"|М|о|ж|е|т |в|з|л|а|м|ы|в|а|т|ь |з|а|м|к|и",
	L"|М|о|ж|е|т |р|е|з|а|т|ь |п|р|о|в|о|л|о|к|у",
	L"|М|о|ж|е|т |р|а|з|р|у|ш|а|т|ь |з|а|м|к|и",
	L"|М|е|т|а|л|л|о|и|с|к|а|т|е|л|ь",
	L"|П|у|л|ь|т |д|и|с|т|а|н|ц|и|о|н|н|о|г|о |у|п|р|а|в|л|е|н|и|я",
	L"|Д|и|с|т|а|н|ц|и|о|н|н|ы|й |д|е|т|о|н|а|т|о|р",
	L"|Д|е|т|о|н|а|т|о|р |с |т|а|й|м|е|р|о|м",
	L"|С|о|д|е|р|ж|и|т |т|о|п|л|и|в|о",
	L"|Н|а|б|о|р |и|н|с|т|р|у|м|е|н|т|о|в",
	L"|Т|е|п|л|о|в|а|я |о|п|т|и|к|а",
	L"|Р|е|н|т|г|е|н|-|п|р|и|б|о|р",
	L"|С|о|д|е|р|ж|и|т |п|и|т|ь|е|в|у|ю |в|о|д|у",
	L"|С|о|д|е|р|ж|и|т |а|л|к|о|г|о|л|ь",
	L"|А|п|т|е|ч|к|а |п|е|р|в|о|й |п|о|м|о|щ|и",
	L"|М|е|д|и|ц|и|н|с|к|и|й |н|а|б|о|р",
	L"|Б|о|м|б|а |д|л|я |з|а|м|к|о|в",
	L"|Н|а|п|и|т|о|к",
	L"|П|и|щ|а",
	L"|П|а|т|р|о|н|н|а|я |л|е|н|т|а",
	L"|Ж|и|л|е|т |д|л|я |п|а|т|р|о|н|о|в",
	L"|Н|а|б|о|р |д|л|я |р|а|з|м|и|н|и|р|о|в|а|н|и|я",
	L"|С|к|р|ы|т|ы|й |п|р|е|д|м|е|т",
	L"|Н|е|в|о|з|м|о|ж|н|о |п|о|в|р|е|д|и|т|ь",
	L"|С|д|е|л|а|н|о |и|з |м|е|т|а|л|л|а",
	L"|Т|о|н|е|т",
	L"|Д|в|у|р|у|ч|н|о|е",
	L"|Б|л|о|к|и|р|у|е|т |о|т|к|р|ы|т|ы|й |п|р|и|ц|е|л",
	L"|С|н|а|р|я|д |п|р|о|т|и|в |б|р|о|н|и",
	L"|З|а|щ|и|т|а |д|л|я |л|и|ц|а",
	L"|И|н|ф|е|к|ц|и|о|н|н|а|я |з|а|щ|и|т|а",	// 39
	L"|S|h|i|e|l|d",	// TODO.Translate
	L"|C|a|m|e|r|a",		// TODO.Translate
	L"|B|u|r|i|a|l |M|o|d|i|f|i|e|r",
	L"|E|m|p|t|y |B|l|o|o|d |B|a|g",	// TODO.Translate
	L"|B|l|o|o|d |B|a|g",						// 44
	L"|R|e|s|i|s|t|a|n|t |t|o |F|i|r|e",
	L"|A|d|m|i|n|i|s|t|r|a|t|i|o|n |M|o|d|i|f|i|e|r",
	L"|H|a|c|k|i|n|g |M|o|d|i|f|i|e|r",
	L"|M|e|d|i|c|a|l |S|p|l|i|n|t", // TODO.Translate
	L"|F|i|r|e |R|e|t|a|r|d|a|n|t |A|m|m|o",				// 49	TODO.Translate
	L"|I|n|c|e|n|d|i|a|r|y |A|m|m|o",
};

STR16 szUDBGenSecondaryStatsExplanationsTooltipText[]=
{
	L"\n \nЭти боеприпасы создают трассирующий\nэффект при стрельбе очередью или в режиме\nавтоматической стрельбы.\n \nТрассеры помогают точнее стрелять.\n \nТакже трассеры создают области света,\nкоторые освещают цель в темноте. Но они\nтакже выдают врагу положение стрелка.\n \nТрассирующие патроны автоматически отменяют\nдействие любых навесок по гашению вспышки\nвыстрела, установленных на том же оружии.",
	L"\n \nЭти боеприпасы могут повредить танковую\nброню. Патроны БЕЗ этого свойства не\nнанесут никакого урона никакому танку.\n \nДаже с этим свойством большинство видов\nоружия не нанесут большого урона, так что\nне ожидайте чего-то особенного.",
	L"\n \nЭти боеприпасы полностью игнорируют\nброню.\n \nПри стрельбе по бронированной цели патроны\nбудут вести себя так, как будто цель\nабсолютно не защищена, и нанесут максимально большой\nурон цели.",
	L"\n \nПри попадании этих боеприпасов в броню\nпоследняя очень быстро будет разрушаться.\n \nПотенциально с их помощью можно полностью\nлишить цель брони.",
	L"\n \nЭти боеприпасы отлично работают\nпри разрушении замков.\n \nСтрельба ими по запертым дверям или контейнерам\nнанесет огромный урон замку.",
	L"\n \nЭта броня в три раза более устойчива\nко взрывам.\n \nКогда взрыв попадет в броню, степень защиты\nпоследней будет считаться утроенной\nпо сравнению со стандартным значением.",
	L"\n \nЭта вещь водонепронимаема. Она не\nполучает повреждений при погружении под воду.\n \nСостояние вещей БЕЗ этого свойства постепенно\nухудшается, если их владелец плывет.",
	L"\n \nСложнотехническая вещь с электроникой.\n \nТакие вещи сложнее чинить, по крайней\nмере без навыков электронщика.",
	L"\n \nКогда эта вещь находится на лице\nперсонажа, она защищает его ото всех\nвидов ядовитых газов.\n \nУчтите, что некоторые газы имеют коррозийный\nэффект, так что будут разъедать и маску.",
	L"\n \nЭтому предмету требуются батарейки.\nБез них вы не сможете включить данный предмет.\n \nВставляются батарейки так же, как прицел\nприкрепляется к оружию.",
	L"\n \nЭтот предмет можно использовать\nдля вскрытия запертых дверей и контейнеров.\n \nВзлом не производит шума, но требует\nзначительных навыков механика, без которых\nвозможно вскрыть лишь самые простые\nзамки. Эта вещь улучшает шанс\nвзлома на ", //JMich_SkillsModifiers: needs to be followed by a number
	L"\n \nПри помощи этой вещи можно делать\nпроходы в проволочных заграждениях.\n \nЭто может дать быстрый доступ к\nфлангам или тылу врага.",
	L"\n \nЭтой вещью можно разбивать запертые\nдвери или контейнеры. Разбивание требует\nнедюжинной силы, производит много шума и быстро\nтратит энергию персонажа. Однако это отличный\nспособ вскрыть замок без подходящих\nнавыков или отмычек. Эта вещь улучшает\nшанс взлома на ", //JMich_SkillsModifiers: needs to be followed by a number
	L"\n \nЭтот предмет используется для нахождения\nобъектов под землей.\n \nОсновное назначение - поиск установленных мин.",
	L"\n \nС помощью этого предмета можно\nвзрывать бомбы, на которых установлен\nдистанционный детонатор.\n \nСначала установите бомбу, затем\nвоспользуйтесь дистанционным детонатором для\nвзрыва в нужное вам время.",
	L"\n \nЕсли этот детонатор присоединить\nк взрывчатке, то его можно будет взорвать\nпри помощи пульта дистанционного\nуправления.\n \nВзрывчатка с таким детонатором полезна в качестве\nловушки, потому как взрывается только по\nвашему приказу.",
	L"\n \nПосле присоединения этого детонатора к\nвзрывчатке и взвода его начинется обратный\nотсчет. По достижении нуля взрывчатка\nвзрывается.",
	L"\n \nЭтот предмет содержит в себе\nбензин.\n \nОн может пригодиться, если вам необходимо\nбудет заправить машину.",
	L"\n \nЭтот предмет содержит в себе\nразличные инструменты, при помощи которых\nможно чинить другие вещи.\n \nТакой набор необходим, если вы\nхотите дать задание наёмникам чинить\nвещи. Эта вещь изменяет эффективность\nремонта на ", //JMich_SkillsModifiers: need to be followed by a number
	L"\n \nЕсли надеть этот предмет, то с его\nпомощью вы сможете увидеть врагов\nчерез стены благодаря выделяемому\nими теплу.",
	L"\n \nЭто устройство можно использовать\nдля нахождения врагов при помощи\nрентгеновских лучей.\n \nОно покажет местонахождение всех\nврагов в определенном радиусе на короткое\nвремя.\n \nДержите подальше от репродуктивных органов!",
	L"\n \nВ этом предмете находится свежая\nпитьевая вода.\n \nИспользуйте, когда появится жажда.",
	L"\n \nВ этом предмете находится огненная вода,\nалкоголь, бухло - называйте как хотите.\n \nПрименять с осторожностью. Не пейте за рулем.\n \nМожет вызвать цирроз печени.",
	L"\n \nЭто базовый набор для оказания\nпервой медицинской помощи.\n \nИспользуйте, чтобы перевязать ваших бойцов\nи не дать им истечь кровью.\n \nДля настоящего лечения используйте\nмедицинский набор и/или продолжительный\nотдых.",
	L"\n \nЭто настоящий медицинский набор,\nкоторый можно использовать для проведения\nхирургических операций.\n \nТакой набор необходим, если вы\nхотите дать задание наёмникам заниматься\nлечением.",
	L"\n \nС помощью этой вещи можно\nвзрывать запертые двери и контейнеры.\n \nДля правильного использования требуется\nнавык обращения со взрывчаткой.\n \nВзрыв очень громкий, и он опасен\nдля большинства персонажей.",
	L"\n \nЭтот предмет утолит вашу жажду,\nесли вы выпьете его.",
	L"\n \nЭтот предмет утолит ваш голод\n,если вы съедите его.",
	L"\n \nВы сможете питать патронами\nчей-нибудь пулемёт, если будете держать\nэту ленту в руках.",
	L"\n \nВ этом жилете можно хранить\nпатронные ленты для боепитания вашего\nпулемёта.",
	L"\n \nЭтот предмет улучшает шанс\nобезвреживания ловушки на ",
	L"\n \nЭтот предмет и все, что к нему\nприсоединено или находится внутри\nнего, скрыто от посторонних глаз.",
	L"\n \nЭтот предмет нельзя повредить.",
	L"\n \nЭтот предмет сделан из металла.\nОн получает меньше урона, чем\nдругие вещи.",
	L"\n \nЭтот предмет тонет в воде.",
	L"\n \nДля использования этого предмета\nтребуются обе руки.",
	L"\n \nЭтот предмет блокирует открытые\nприцельные приспособления оружия, так что вы не\nсможете воспользоваться ими.",
	L"\n \nЭтот боеприпас может уничтожать тонкие стены\nи некоторые другие объекты.",
	L"\n \nЕсли одето на лицо, понижает шанс\nзаражения от других людей.",
	L"\n \nЕсли хранить в своём кармане,\nпонижается шанс заражения\n от других людей.",
	L"\n \nIf equipped in a hand, this will block incoming damage.",	// TODO.Translate
	L"\n \nYou can take photos with this.",		// TODO.Translate
	L"\n \nThis item makes you more effective at burying corpses.",
	L"\n \nA paramedic can extract blood\nfrom a donor with this.",	// TODO.Translate
	L"\n \nA paramedic can use up this item to increase\nthe amount of health regenerated by surgery.",	// 44
	L"\n \nThis armor lowers fire damage by %i%%.",
	L"\n \nThis item makes you more effective at\nadministrative work by %i%%.",
	L"\n \nThis item improves your hacking skills by %i%%.",
	L"\n \nOnce applied, this item increases the healing\nspeed of severe wounds to either your arms or legs.", // TODO.Translate
	L"\n \nThis ammo can extinguish fire.",	// 49	 TODO.Translate
	L"\n \nThis ammo can cause fire.",
};

STR16 szUDBAdvStatsTooltipText[]=
{
	L"|М|о|д|и|ф|и|к|а|т|о|р |т|о|ч|н|о|с|т|и",
	L"|Ф|и|к|с|и|р|о|в|а|н|н|ы|й |м|о|д|и|ф|и|к|а|т|о|р |с|т|р|е|л|ь|б|ы",
	L"|П|р|о|ц|е|н|т|н|ы|й |м|о|д|и|ф|и|к|а|т|о|р |с|т|р|е|л|ь|б|ы",
	L"|Ф|и|к|с|и|р|о|в|а|н|н|ы|й |м|о|д|и|ф|и|к|а|т|о|р |п|р|и|ц|е|л|и|в|а|н|и|я",
	L"|П|р|о|ц|е|н|т|н|ы|й |м|о|д|и|ф|и|к|а|т|о|р |п|р|и|ц|е|л|и|в|а|н|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р |д|о|с|т|у|п|н|ы|х |у|р|о|в|н|е|й |п|р|и|ц|е|л|и|в|а|н|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|о|р|о|г|а |п|р|и|ц|е|л|и|в|а|н|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р |о|б|р|а|щ|е|н|и|я |с |о|р|у|ж|и|е|м ",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|а|д|е|н|и|я |п|у|л|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |о|т|с|л|е|ж|и|в|а|н|и|я |ц|е|л|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |у|р|о|н|а",	
	L"|М|о|д|и|ф|и|к|а|т|о|р |р|у|к|о|п|а|ш|н|о|г|о |у|р|о|н|а",
	L"|М|о|д|и|ф|и|к|а|т|о|р |д|а|л|ь|н|о|с|т|и",
	L"|К|р|а|т|н|о|с|т|ь |у|в|е|л|и|ч|е|н|и|я |п|р|и|ц|е|л|а",
	L"|Д|а|л|ь|н|о|с|т|ь |п|р|о|е|ц|и|р|о|в|а|н|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р |б|о|к|о|в|о|й |о|т|д|а|ч|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |в|е|р|т|и|к|а|л|ь|н|о|й |о|т|д|а|ч|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |м|а|к|с|и|м|а|л|ь|н|о|г|о |п|р|о|т|и|в|о|д|е|й|с|т|в|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р т|о|ч|н|о|с|т|и |п|р|и |п|р|о|т|и|в|о|д|е|й|с|т|в|и|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |ч|а|с|т|о|т|ы |п|р|о|т|и|в|о|д|е|й|с|т|в|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р |в|с|е|х |О|Д",
	L"|М|о|д|и|ф|и|к|а|т|о|р |О|Д |н|а |в|с|к|и|д|ы|в|а|н|и|е",
	L"|М|о|д|и|ф|и|к|а|т|о|р |О|Д |о|д|н|о|й |а|т|а|к|и", 
	L"|М|о|д|и|ф|и|к|а|т|о|р |О|Д |о|ч|е|р|е|д|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |О|Д |а|в|т|о|м|а|т|и|ч|е|с|к|о|й |с|т|р|е|л|ь|б|ы",
	L"|М|о|д|и|ф|и|к|а|т|о|р |О|Д |п|е|р|е|з|а|р|я|д|к|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |р|а|з|м|е|р|а |м|а|г|а|з|и|н|а",
	L"|М|о|д|и|ф|и|к|а|т|о|р |р|а|з|м|е|р|а |о|ч|е|р|е|д|и",
	L"|С|к|р|ы|т|а|я |в|с|п|ы|ш|к|а |в|ы|с|т|р|е|л|а",
	L"|М|о|д|и|ф|и|к|а|т|о|р |г|р|о|м|к|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |р|а|з|м|е|р|а |п|р|е|д|м|е|т|а",
	L"|М|о|д|и|ф|и|к|а|т|о|р |н|а|д|ё|ж|н|о|с|т|и",
	L"|Л|е|с|н|о|й |к|а|м|у|ф|л|я|ж",
	L"|Г|о|р|о|д|с|к|о|й |к|а|м|у|ф|л|я|ж",
	L"|П|у|с|т|ы|н|н|ы|й |к|а|м|у|ф|л|я|ж",
	L"|З|и|м|н|и|й |к|а|м|у|ф|л|я|ж",
	L"|М|о|д|и|ф|и|к|а|т|о|р |с|к|р|ы|т|н|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |с|л|ы|ш|и|м|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |о|б|ы|ч|н|о|й |в|и|д|и|м|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |н|о|ч|н|о|й |в|и|д|и|м|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |д|н|е|в|н|о|й |в|и|д|и|м|o|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |в|и|д|и|м|о|с|т|и |п|р|и |я|р|к|о|м |с|в|е|т|е",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|е|щ|е|р|н|о|й |в|и|д|и|м|о|с|т|и",
	L"|Т|у|н|н|е|л|ь|н|о|е |з|р|е|н|и|е",
	L"|М|а|к|с|и|м|а|л|ь|н|о|е |п|р|о|т|и|в|о|д|е|й|с|т|в|и|е",
	L"|Ч|а|с|т|о|т|а |п|р|о|т|и|в|о|д|е|й|с|т|в|и|я",
	L"|Б|о|н|у|с| |п|о|п|а|д|а|н|и|я",
	L"|Б|о|ну|с |п|р|и|ц|е|л|и|в|а|н|и|я",
	L"|Т|е|м|п|е|р|а|т|у|р|а |о|д|н|о|г|о |в|ы|с|т|р|е|л|а",
	L"|С|к|о|р|о|с|т|ь |о|с|т|ы|в|а|н|и|я",
	L"|П|о|р|о|г |о|с|е|ч|к|и",
	L"|П|о|р|о|г |п|о|в|р|е|ж|д|е|н|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р |т|е|м|п|е|р|а|т|у|р|ы",
	L"|М|о|д|и|ф|и|к|а|т|о|р |о|с|т|ы|в|а|н|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|о|р|о|г|а |о|с|е|ч|к|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|о|р|о|г|а |п|о|в|р|е|ж|д|е|н|и|я",
	L"|У|р|о|в|е|н|ь |я|д|а",
	L"|М|о|д|и|ф|и|к|а|т|о|р |з|а|г|р|я|з|н|е|н|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р |я|д|а",
	L"|У|т|о|л|е|н|и|е |г|о|л|о|д|а",
	L"|У|т|о|л|е|н|и|е |ж|а|ж|д|ы",
	L"|Р|а|з|м|е|р |п|о|р|ц|и|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |м|о|р|а|л|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|о|р|ч|и",
	L"|О|п|т|и|м|а|л|ь|н|а|я |д|а|л|ь|н|о|с|т|ь |л|а|з|е|р|а",
	L"|М|о|д|и|ф|и|к|а|т|о|р |о|т|д|а|ч|и |в |%",
	L"|F|a|n |t|h|e |H|a|m|m|e|r",	// TODO.Translate
	L"|B|a|r|r|e|l |C|o|n|f|i|g|u|r|a|t|i|o|n|s",
};

// Alternate tooltip text for weapon Advanced Stats. Just different wording, nothing spectacular.
STR16 szUDBAdvStatsExplanationsTooltipText[]=
{
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет изменяет значение его точности.\n \nПовышенная точность оружия позволяет\nчаще попадать из него по более удаленным\nцелям.\n \nДиапазон: -100..+100.\nБольше - лучше.",
	L"\n \nЭтот предмет изменяет точность,\nс которой стрелок совершает ЛЮБОЙ\nвыстрел, на указанное значение.\n \nДиапазон: -100..+100.\nБольше - лучше.",
	L"\n \nЭтот предмет изменяет точность,\nс которой стрелок совершает ЛЮБОЙ\nвыстрел, на указанный процент.\n \nБольше - лучше.",
	L"\n \nЭтот предмет изменяет точность,\nдающуюся за каждый дополнительный\nуровень прицеливания, на указанное значение.\n \nДиапазон: -100..+100.\nБольше - лучше.",
	L"\n \nЭтот предмет изменяет точность,\nдающуюся за каждый дополнительный\nуровень прицеливания, на указанный процент.\n \nДиапазон: -100..+100.\nБольше - лучше.",
	L"\n \nЭтот предмет изменяет число\nдоступных уровней прицеливания.\n \nЧем МЕНЬШЕ это число, тем БОЛЬШИЙ бонус\nдается за каждый уровень. То есть\nМЕНЬШЕЕ число уровней позволяет быстрее\nприцеливаться без потери точности.\n \nМеньше - лучше.",
	L"\n \nЭтот предмет изменяет максимальную\nточность стрелка при использовании огнестрельного\nоружия на указанный процент.\n \nБольше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет изменяет сложность обращения\nс ним.\n \nНизкая сложность означает, что с оружием\nлегче управляться независимо от уровня\nприцеливания.\n \nЭтот модификатор основан на изначальной\nсложности обращения с оружием, высокой у винтовок\nи тяжелого вооружения и низкой у пистолетов и ПП.\n \nМеньше - лучше.",
	L"\n \nЭтот предмет изменяет вероятность\nпули пролететь дальше максимальной\nдальности выстрела для данного оружия.\n \nВысокий бонус может заставить пулю\nпролететь дальше максимальной дальности\nкак минимум на пару тайлов.\n \nБольше - лучше.",
	L"\n \nЭтот предмет изменяет вероятность\nпопадания по бегущей цели.\n \nБольше - лучше.",
	L"\n \nЭтот предмет изменяет урон от данного\nоружия на указанное значение.\n \nБольше - лучше.",
	L"\n \nЭтот предмет изменяет урон от данного\nоружия ближнего боя на указанное значение.\n \nБольше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет изменяет максимальную дальность\nвыстрела.\n \nМаксимальная дальность - это расстояние, пролетев которое,\nпуля начнет падать на землю.\n \nБольше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет дает возможность оптического\nприближения удаленных целей.\n \nПомните, что сильное увеличение прицела снижает\nточность, если цель находится слишком близко.\n \nБольше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет проецирует точку на цели,\nчто облегчает прицеливание.\n \nЭтот эффект работает до данного расстояния,\nзатем начинает уменьшаться и, в конце концов,\nпропадает на значительном расстоянии.\n \nБольше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nспособному вести огонь очередями, этот\nпредмет изменяет горизонтальную отдачу\nпри такой стрельбе на указанный процент.\n \nСнижение отдачи позволяет дольше\nудерживать ствол оружия в направлении цели\nво время залпа.\n \nМеньше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nспособному вести огонь очередями, этот\nпредмет изменяет вертикальную отдачу\nпри такой стрельбе на указанный процент.\n \nСнижение отдачи позволяет дольше\nудерживать ствол оружия в направлении цели\nво время залпа.\n \nМеньше - лучше.",
	L"\n \nЭтот предмет изменяет способность\nстрелка справляться с отдачей оружия\nпри стрельбе очередями.\n \nВысокое значение позволяет уверенно\nконтролировать оружие с сильной отдачей\nдаже персонажам с малым значением силы.\n \nБольше - лучше.",
	L"\n \nЭтот предмет изменяет способность\nстрелка при стрельбе очередями\nудерживать ствол оружия направленным\nна цель.\n \nВысокое значение позволяет попадать\nв цель при стрельбе очередями\nдаже на дальних дистанциях.\n \nБольше - лучше.",
	L"\n \nЭтот предмет изменяет частоту,\nс которой стрелок при стрельбе очередями\nоценивает, сколько ему нужно приложить сил\nдля противодействия отдаче.\n \nБольшая частота означает, что при правильном \nприложении силы очередь в целом будет точнее.\n \nБольше - лучше.",
	L"\n \nЭтот предмет напрямую изменяет\nколичество ОД, доступных персонажу\nв начале каждого хода.\n \nБольше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет изменяет количество ОД,\nтребуемых для взятия оружия на изготовку.\n \nМеньше - лучше.",
	L"\n \nПрисоединенный к любому оружию,\nэтот предмет изменяет количество ОД,\nтребуемых для совершения одной атаки.\n \nОбратите внимание, что для оружия,\nспособного вести огонь очередями,\nэтот модификатор влияет и на этот режим\nстрельбы тоже.\n \nМеньше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nспособному вести огонь очередями, этот\nпредмет изменяет количество ОД,\nтребуемых для такой стрельбы.\n \nМеньше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nспособному вести автоматический огонь,\nэтот предмет изменяет количество ОД,\nтребуемых для такой стрельбы.\n \nОбратите внимание, что этот модификатор\nне влияет на стоимость добавления одной\nпули к очереди, а только на начальную стоимость\nочереди.\n \nМеньше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет изменяет количество ОД,\nтребуемых для перезарядки этого оружия.\n \nМеньше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет изменяет количество пуль в\nмагазине, который можно зарядить в это оружие.\n \nБольше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет изменяет количество пуль,\nвыпускаемых очередью.\n \nЕсли изначально у оружия не было режима\nстрельбы очередью, то после присоединения\nэтого предмета указанный режим появится.\n \nИ наоборот, большое отрицательное значение\nданного параметра может полностью\nвыключить этот режим.\n \nБольше ОБЫЧНО лучше, но одной из\nзадач стрельбы очередью является\nэкономия патронов...",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет будет скрывать вспышку\nвыстрела.\n \nБлагодаря этому враги не смогут обнаружить\nстрелка по вспышке, что особенно важно\nночью.",
	L"\n \nПрисоединенный к оружию, этот предмет\nизменяет расстояние, с которого звук выстрела\nэтого оружия слышен врагами и наёмниками.\n \nЕсли этот модификатор опускает\nзначение громкости оружия до 0,\nто оружие считается совершенно бесшумным.\n \nМеньше - лучше.",
	L"\n \nЭтот предмет изменяет размер\nлюбой другой вещи, к которой\nон присоединен.\n \nРазмер вещей важен в новой\nсистеме инвентаря, в которой в слоты\nможно поместить вещи определенных\nразмеров и форм.\n \nУвеличив размер вещи, вы уже не сможете\nпоместить ее в слот, в который\nона раньше спокойно вмещалась.\n \nУменьшив размер вещи, вы сможете\nпоместить ее в слот меньшего размера\nили же поместить больше таких вещей\nв тот же слот.\n \nВ целом, меньше - лучше.",
	L"\n \nПрисоединенный к любому оружию,\nэтот предмет изменяет надежность этого\nоружия.\n \nПри положительном значении состояние оружия\nбудет ухудшаться медленнее, и наоборот.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет значение\nлесного камуфляжа владельца.\n \nЧтобы этот камуфляж сработал,\nвладельцу необходимо находиться возле\nдеревьев или в высокой траве.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет значение\nгородского камуфляжа владельца.\n \nЧтобы этот камуфляж сработал,\nвладельцу необходимо находиться на\nасфальте или бетоне.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет значение\nпустынного камуфляжа владельца.\n \nЧтобы этот камуфляж сработал,\nвладельцу необходимо находиться на\nпеске, гравии или среди пустынной растительности.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет значение\nзимнего камуфляжа владельца.\n \nЧтобы этот камуфляж сработал,\nвладельцу необходимо находиться на\nснежной поверхности.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет показатель\nскрытности владельца, что влияет на возможность\nврагов УСЛЫШАТЬ его передвижение в режиме\nскрытности.\n \nЭтот модификатор меняет НЕ видимость\nвладельца, а лишь производимый им шум.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет уровень слуха\nвладельца на указанный процент.\n \nПри положительном значении владелец\nсможет услышать шум с более дальних\nрасстояний, и наоборот.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет уровень зрения\nвладельца на указанный процент.\n \nЭтот модификатор работает для всех\nусловий.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет уровень зрения\nвладельца на указанный процент.\n \nЭтот модификатор работает для ночных\nусловий.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет уровень зрения\nвладельца на указанный процент.\n \nЭтот модификатор работает для условий,\nкогда освещенность средняя или выше.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет уровень зрения\nвладельца на указанный процент.\n \nЭтот модификатор работает для условий,\nкогда освещенность очень высока, например\nпри использовании осветителя или в полдень.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет уровень зрения\nвладельца на указанный процент.\n \nЭтот модификатор работает только в темноте\nи только под землей.\n \nБольше - лучше.",
	L"\n \nНадевание этой вещи или прикрепление\nее к носимой вещи изменяет поле зрения\nвладельца.\n \nБольшее значение означает более выраженный\nэффект туннельного зрения.\n \nМеньше - лучше.",
	L"\n \nСпособность стрелка справляться с\nотдачей при стрельбе очередями или автоматической\nстрельбе.\n \nБольше - лучше.",
	L"\n \nСпособность стрелка чаще оценивать,\nсколько ему нужно приложить сил\nдля противодействия отдаче.\n \nБольшая частота означает, что при правильном\nприложении силы очередь в целом будет точнее.\n \nБольше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет изменяет шанс попадания из\nэтого оружия.\n \nПри условии точного прицеливания\nповышенный шанс позволяет чаще попадать\nиз оружия.\n \nБольше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет изменяет бонус прицеливания для\nэтого оружия.\n \nПри условии точного прицеливания\nэтот бонус позволяет чаще попадать из\nоружия на дальних дистанциях.\n \nБольше - лучше.",
	L"\n \nОдиночный выстрел повышает температуру\nна это значение.\n \nРазличные типы боеприпасов и навесок\nвлияют на это значение.\n \nМеньше - лучше.",
	L"\n \nКаждый ход температура снижается\nна это значение.\n \nБольше - лучше.",
	L"\n \nЕсли температура выше этого значения,\nто оружие будет давать осечки чаще.\n \nБольше - лучше.",
	L"\n \nЕсли температура предмета выше этого\nзначения, то его состояние будет ухудшаться\nбыстрее.\n \nБольше - лучше.",
	L"\n \nОдиночный выстрел из оружия будет\nповышать температуру на этот процент.\n \nМеньше - лучше. ",
	L"\n \nСкорость остывания оружия изменяется\nна этот процент.\n \nБольше - лучше.",
	L"\n \nПорог осечки для оружия изменяется\nна этот процент.\n \nБольше - лучше.",
	L"\n \nПорог повреждения оружия изменяется\nна этот процент.\n \nБольше - лучше.",
	L"\n \nОпределяет, какой процент от нанесенного\nурона будет ядовитым.\n \nЭффективность определяется степенью защиты\nили поглощения яда у врага.",
	L"\n \nОдиночный выстрел загрязняет оружие\nна это значение.\n \nРазличные типы боеприпасов и навесок\nвлияют на это значение.\n \nМеньше - лучше.",
	L"\n \nКоличество энергии в килокалориях.\n \nБольше - лучше.",
	L"\n \nОбъем воды в литрах.\n \nБольше - лучше.",
	L"\n \nОпределяет, какой процент\nбудет съеден за раз.\n \nМеньше - лучше.",
	L"\n \nМораль изменяется на это значение.\n \nБольше - лучше.",
	L"\n \nПища портится со временем.\nЕсли она на 50% покрыта плесенью,\nто она становится ядовитой.\nЭтот модификатор определяет, с какой\nскоростью будет портиться еда.\n \nМеньше - лучше.",
	L"\n \nПрисоединенный к огнестрельному оружию,\nэтот предмет проецирует точку на цели,\nчто облегчает прицеливание.\n \nЭтот эффект работает до данного расстояния,\nзатем начинает уменьшаться и, в конце концов,\nпропадает на значительном расстоянии.\n \nБольше - лучше.",
	L"",
	L"\n \nПрисоединенный к огнестрельному оружию,\nспособному вести огонь очередями, этот\nпредмет изменяет отдачу оружия на\nуказанный процент.\n \nСнижение отдачи позволяет дольше\nудерживать ствол оружия в направлении цели\nво время залпа.\n \nМеньше - лучше.", // 65
	L"\n \nIf a gunslinger wields this gun two-handed,\nthey can burst in hipfire.",	// TODO.Translate
	L"\n \nToggling firemodes also toggles how many\nbarrels you can fire at the same time.",
};

STR16 szUDBAdvStatsExplanationsTooltipTextForWeapons[]=
{
	L"\n \nТочность этого оружия изменена\nбоеприпасами, навеской или внутренними\nсвойствами.\n \nПовышенная точность оружия позволяет\nчаще попадать из него по более удаленным\nцелям.\n \nДиапазон: -100..+100.\nБольше - лучше.",
	L"\n \nЭто оружие изменяет точность,\nс которой стрелок совершает ЛЮБОЙ\nвыстрел, на указанное значение.\n \nДиапазон: -100..+100.\nБольше - лучше.",
	L"\n \nЭто оружие изменяет точность,\nс которой стрелок совершает ЛЮБОЙ\nвыстрел, на указанный процент.\n \nБольше - лучше.",
	L"\n \nЭто оружие изменяет точность,\nдающуюся за каждый дополнительный\nуровень прицеливания, на указанное значение.\n \nДиапазон: -100..+100.\nБольше - лучше.",
	L"\n \nЭто оружие изменяет точность,\nдающуюся за каждый дополнительный\nуровень прицеливания, на указанный процент.\n \nДиапазон: -100..+100.\nБольше - лучше.",
	L"\n \nЧисло доступных уровней прицеливания\n для этого оружия изменено боеприпасами,\nнавеской или внутренними свойствами.\n \nЧем МЕНЬШЕ это число, тем БОЛЬШИЙ бонус\nдается за каждый уровень. То есть\nМЕНЬШЕЕ число уровней позволяет быстрее\nприцеливаться без потери точности.\n \nМеньше - лучше.",
	L"\n \nЭто оружие изменяет максимальную\nточность стрелка на указанный процент.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия или его\nвнутренние свойства изменяют сложность\nобращения с ним.\n \nНизкая сложность означает, что с оружием\nлегче управляться независимо от уровня\nприцеливания.\n \nЭтот модификатор основан на изначальной\nсложности обращения с оружием, высокой у винтовок\nи тяжелого вооружения и низкой у пистолетов и ПП.\n \nМеньше - лучше.",
	L"\n \nНавеска этого оружия или его\nвнутренние свойства изменили вероятность\nпули пролететь дальше максимальной\nдальности выстрела для данного оружия.\n \nВысокий бонус может увеличить максимальную\nдальность как минимум на пару тайлов.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия или его\nвнутренние свойства изменили вероятность\nпопадания по бегущей цели.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили урон от данного\nоружия на указанное значение.\n \nБольше - лучше.",
	L"\n \nНавеска или внутренние свойства данного\nоружия ближнего боя изменили урон\nна указанное значение.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили максимальную дальность выстрела.\n \nМаксимальная дальность - это расстояние, пролетев которое,\n пуля начнет падать на землю.\n \nБольше - лучше.",
	L"\n \nЭто оружие оборудовано оптическим\nприцелом, что позволяет уверенно поражать\nудаленные цели.\n \nПомните, что сильное увеличение прицела снижает\nточность, если цель находится слишком близко.\n \nБольше - лучше.",
	L"\n \nЭто оружие оборудовано устройством,\nкоторое проецирует точку на цели,\nчто облегчает прицеливание.\n \nЭтот эффект работает до данного расстояния,\nзатем начинает уменьшаться и, в конце концов,\nпропадает на значительном расстоянии.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили силу горизонтальной отдачи.\n \nСнижение отдачи позволяет дольше\nудерживать ствол оружия в направлении цели\nво время залпа.\n \nМеньше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили силу вертикальной отдачи.\n \nСнижение отдачи позволяет дольше\nудерживать ствол оружия в направлении цели\nво время залпа.\n \nМеньше - лучше.",
	L"\n \nНавеска этого оружия или его\nвнутренние свойства изменяют способность\nстрелка справляться с отдачей\nпри стрельбе очередями.\n \nВысокое значение позволяет уверенно\nконтролировать оружие с сильной отдачей\nдаже персонажам с малым значением силы.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия или его\nвнутренние свойства изменяют способность\nстрелка при стрельбе очередями\nудерживать ствол оружия направленным\nна цель.\n \nВысокое значение позволяет попадать\nв цель при стрельбе очередями\nдаже на дальних дистанциях.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия или его\nвнутренние свойства изменяют частоту,\nс которой стрелок при стрельбе очередями\nоценивает, сколько ему нужно приложить сил\nдля противодействия отдаче.\n \nБольшая частота означает, что при правильном \nприложении силы очередь в целом будет точнее.\n \nБольше - лучше.",
	L"\n \nЕсли это оружие держать в руках,\nто оно изменит количество ОД,\nдоступных его владельцу на начало\nкаждого хода.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили стоимость ОД, требуемых для\nвзятия оружия на изготовку.\n \nМеньше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили стоимость ОД, требуемых для\nсовершения одной атаки.\n \nОбратите внимание, что для оружия,\nспособного вести огонь очередями,\nэтот модификатор влияет и на эти режимы\nстрельбы тоже.\n \nМеньше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили стоимость ОД, требуемых для\nстрельбы очередями.\n \nМеньше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили стоимость ОД, требуемых для\nавтоматической стрельбы.\n \nОбратите внимание, что этот модификатор\nне влияет на стоимость добавления одной\nпули к очереди, а только на начальную стоимость\nочереди.\n \nМеньше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили стоимость ОД, требуемых для\nперезарядки оружия.\n \nМеньше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили количество пуль в\nмагазине, который можно зарядить в это оружие.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили количество пуль,\nвыпускаемых очередью.\n \nЕсли изначально у оружия не было режима\nстрельбы очередью, то после присоединения\nэтого предмета указанный режим появится.\n \nИ наоборот, большое отрицательное значение\nданного параметра может полностью\nвыключить этот режим.\n \nБольше ОБЫЧНО лучше, но одной из\nзадач стрельбы очередью является\nэкономия патронов...",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nскрывают вспышку\nвыстрела.\n \nБлагодаря этому враги не смогут обнаружить\nстрелка по вспышке, что особенно важно\nночью.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили расстояние, с которого звук выстрела\nэтого оружия слышен врагами и наёмниками.\n \nЕсли этот модификатор опускает\nзначение громкости оружия до 0,\nто оружие считается совершенно бесшумным.\n \nМеньше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили размер\nэтого оружия.\n \nРазмер вещей важен в новой\nсистеме инвентаря, в которой в слоты\nможно поместить вещи определенных\nразмеров и форм.\n \nУвеличив размер вещи, вы уже не сможете\nпоместить ее в слот, в который\nона раньше помещалась.\n \nУменьшив размер вещи, вы сможете\nпоместить ее в слот меньшего размера\nили же поместить больше таких вещей\nв тот же слот.\n \nВ целом, меньше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили надежность этого оружия.\n \nПри положительном значении состояние оружия\nбудет ухудшаться медленнее, и наоборот.\n \nБольше - лучше.",
	L"\n \nЕсли это оружие держать в руках,\nто оно изменит значение\nлесного камуфляжа владельца.\n \nЧтобы этот камуфляж сработал,\nвладельцу необходимо находиться возле\nдеревьев или в высокой траве.\n \nБольше - лучше.",
	L"\n \nЕсли это оружие держать в руках,\nто оно изменит значение\nгородского камуфляжа владельца.\n \nЧтобы этот камуфляж сработал,\nвладельцу необходимо находиться на\nасфальте или бетоне.\n \nБольше - лучше.",
	L"\n \nЕсли это оружие держать в руках,\nто оно изменит значение\nпустынного камуфляжа владельца.\n \nЧтобы этот камуфляж сработал,\nвладельцу необходимо находиться на\nпеске, гравии или среди пустынной растительности.\n \nБольше - лучше.",
	L"\n \nЕсли это оружие держать в руках,\nто оно изменит значение\nзимнего камуфляжа владельца.\n \nЧтобы этот камуфляж сработал,\nвладельцу необходимо находиться на\nснежной поверхности.\n \nБольше - лучше.",
	L"\n \nЕсли это оружие держать в руках,\nто оно изменит показатель\nскрытности владельца, что влияет на возможность\nврагов УСЛЫШАТЬ его передвижение в режиме\nскрытности.\n \nЭтот модификатор меняет НЕ видимость\nвладельца, а лишь производимый им шум.\n \nБольше - лучше.",
	L"\n \nЕсли это оружие держать в руках,\nто оно изменит уровень слуха\nвладельца на указанный процент.\n \nПри положительном значении владелец\nсможет услышать шум с более дальних\nрасстояний, и наоборот.\n \nБольше - лучше.",
	L"\n \nЕсли взять это оружие на изготовку, оно изменит\nуровень зрения его владельца на\nуказанный процент благодаря навеске\nили внутренним свойствам оружия.\n \nЭтот модификатор работает для всех\nусловий.\n \nБольше - лучше.",
	L"\n \nЕсли взять это оружие на изготовку, оно изменит\nуровень зрения его владельца на\nуказанный процент благодаря навеске\nили внутренним свойствам оружия.\n \nЭтот модификатор работает для ночных\nусловий.\n \nБольше - лучше.",
	L"\n \nЕсли взять это оружие на изготовку, оно изменит\nуровень зрения его владельца на\nуказанный процент благодаря навеске\nили внутренним свойствам оружия.\n \nЭтот модификатор работает для условий,\nкогда освещенность средняя или выше.\n \nБольше - лучше.",
	L"\n \nЕсли взять это оружие на изготовку, оно изменит\nуровень зрения его владельца на\nуказанный процент благодаря навеске\nили внутренним свойствам оружия.\n \nЭтот модификатор работает для условий,\nкогда освещенность очень высока, например\nпри использовании осветителя или в полдень.\n \nБольше - лучше.",
	L"\n \nЕсли взять это оружие на изготовку, оно изменит\nуровень зрения его владельца на\nуказанный процент благодаря навеске\nили внутренним свойствам оружия.\n \nЭтот модификатор работает только в темноте\nи только под землей.\n \nБольше - лучше.",
	L"\n \nЕсли взять это оружие на изготовку,\nоно изменит поле зрения владельца.\n \nБольшее значение означает более выраженный\nэффект туннельного зрения.\n \nМеньше - лучше.",
	L"\n \nСпособность стрелка справляться с\nотдачей при стрельбе очередями или автоматической\nстрельбе.\n \nБольше - лучше.",
	L"\n \nСпособность стрелка чаще оценивать\nсколько ему нужно приложить сил\nдля противодействия отдаче.\n \nМеньшая частота означает, что при правильном\nприложении силы очередь в целом будет точнее.\n \nМеньше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили шанс попадания из\nэтого оружия.\n \nПри условии точного прицеливания\nповышенный шанс позволяет чаще попадать\nиз оружия.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили бонус прицеливания для\nэтого оружия.\n \nПри условии точного прицеливания\nэтот бонус позволяет чаще попадать из\nоружия на дальних дистанциях.\n \nБольше - лучше.",
	L"\n \nОдиночный выстрел повышает температуру\nна это значение.\n \nРазличные типы боеприпасов\nвлияют на это значение.\n \nМеньше - лучше.",
	L"\n \nКаждый ход температура снижается\nна это значение.\n \nБольше - лучше.",
	L"\n \nЕсли температура выше этого значения,\nто оружие будет давать осечки чаще.\n \nБольше - лучше.",
	L"\n \nЕсли температура предмета выше этого\nзначения, то его состояние будет ухудшаться\nбыстрее.\n \nБольше - лучше.",
	L"\n \nНавеска этого оружия, боеприпасы\nк нему или его внутренние свойства\nизменили отдачу оружия на\nуказанный процент.\n \nСнижение отдачи позволяет дольше\nудерживать ствол оружия в направлении цели\nво время залпа.\n \nМеньше - лучше.",
};

// HEADROCK HAM 4: Text for the new CTH indicator.
STR16 gzNCTHlabels[]=
{
	L"ОДИНОЧНЫЙ",	//SINGLE
	L"ОД",
};
//////////////////////////////////////////////////////
// HEADROCK HAM 4: End new UDB texts and tooltips
//////////////////////////////////////////////////////

// HEADROCK HAM 5: Screen messages for sector inventory sorting reports.
STR16 gzMapInventorySortingMessage[] =
{
	L"В секторе  %c%d завершена сборка аммуниции в ящики.",
	L"С предметов в секторе %c%d снята вся навеска.",
	L"В секторе %c%d всё оружие разряжено.",
	L"В секторе %c%d все вещи сгруппированы и объединены.",
	// Bob: new strings for emptying LBE items
	L"Finished emptying LBE items in sector %c%d.",
	L"Dropped %i item(s) from %s",	// Bunch of stuff removed from LBE item %s
	L"No droppable items in %s!",	// LBE item %s had a LBENode assigned but it contained no items (error!)
	L"%s is now empty.",			// LBE item %s contained stuff and was emptied
	L"Cannot empty %s!",			// Removed everything we could from LBE item %s but it's still not marked as empty (error!)
	L"%s contents lost!",			// LBE item %s not marked as empty but LBENode not found (error!!!)
};

STR16 gzMapInventoryFilterOptions[] =
{
	L"Показать всё",
	L"Оружие",
	L"Патроны",
	L"Взрывчатка",
	L"Холодное оружие",
	L"Броня",
	L"Разгруз. системы",
	L"Наборы",
	L"Прочие предметы",
	L"Скрыть всё",
};

// MercCompare (MeLoDy)
// TODO.Translate
STR16 gzMercCompare[] =
{
	L"???",
	L"Первое мнение:",

	L"Не нравятся %s %s",
	L"Симпатизирует %s %s",

	L"Люто ненавидит %s",
	L"Ненавидит %s",			// 5

	L"Лютый расист в отношении к %s",
	L"Расист в отношении к %s",

	L"Излишне заботится о внешности",
	L"Заботится о внешности",

	L"Лютый сексист",			// 10
	L"Сексист",

	L"Не нравится другое прошлое",
	L"Не нравится другое прошлое",

	L"Затаил обиду",	//Past grievances
	L"____",				// 15
	L"/",
	L"* Мнение всегда между [%d; %d]",
};

// Flugente: Temperature-based text similar to HAM 4's condition-based text.
STR16 gTemperatureDesc[] =
{
	L"Температура ",
	L"очень низкая",
	L"низкая",
	L"умеренная",
	L"высокая",
	L"очень высокая",
	L"опасная",
	L"КРИТИЧЕСКАЯ",
	L"ФАТАЛЬНАЯ",
	L"неизвестна",
	L"."
};

// Flugente: food condition texts
STR16 gFoodDesc[] =
{
	L"Пища ",
	L"свежая",
	L"хорошая",
	L"нормальная",
	L"несвежая",
	L"порченная",
	L"подгнившая",
	L"."
};

CHAR16* ranks[] = 
{	L"",			//ExpLevel 0
	L"Рядовой ",		//ExpLevel 1
	L"Ефрейтор ",		//ExpLevel 2
	L"Мл. сержант ",	//ExpLevel 3
	L"Сержант ",		//ExpLevel 4
	L"Лейтенант ",		//ExpLevel 5
	L"Капитан ",		//ExpLevel 6
	L"Майор ",		//ExpLevel 7
	L"Подполк. ",		//ExpLevel 8
	L"Полковник ",		//ExpLevel 9
	L"Генерал "		//ExpLevel 10
};


STR16	gzNewLaptopMessages[]=
{
	L"Спрашивайте о нашем специальном предложении!",
	L"Временно недоступно",
	L"Ознакомительная игра 'Jagged Alliance 2: Цена Свободы' содержит только первые 6 карт секторов. Финальная версия игры будет включать в себя гораздо больше возможностей (полная информация содержится в приложенном файле Readme.txt).",
};

STR16	zNewTacticalMessages[]=
{
	//L"Расстояние до цели: %d ед., Освещенность: %d/%d",
	L"Передатчик подключен к вашему ноутбуку.",
	L"Вы не можете нанять %s",
	L"Предложение действует ограниченное время и покрывает стоимость найма на всю миссию, плюс вы так же получите оборудование, перечисленное ниже.",
	L"Наёмник %s - наше невероятное суперпредложение 'одна плата за все'. Вы также бесплатно получите его персональную экипировку.",
	L"Гонорар",
	L"В секторе кто-то есть...",
	//L"Дальнобойность оружия: %d ед., Шанс попасть: %d%%",
	L"Показать укрытия",
	L"Линия прицела",
	L"Новые наёмники не могут высадиться здесь.",
	L"Так как ваш ноутбук лишился антенны, то вы не сможете нанять новых наёмников. Возможно, сейчас вам стоит загрузить одну из сохраненных игр, или начать игру заново!",
	L"%s слышит металлический хруст под телом Джерри. Кажется, это чмо сломало антенну вашего ноутбука.",  //the %s is the name of a merc.
	L"После прочтения записей, оставленных помощником командира Морриса, %s видит, что не все еще потеряно. В записке содержатся координаты городов Арулько для запуска по ним ракет. Кроме того, там также указаны координаты самой ракетной базы.",
	L"Изучив панель управления, %s понимает, что координаты цели можно изменить, и тогда ракета уничтожит эту базу.  %s не собирается умирать, а значит нужно быстрее отсюда выбираться. Похоже, что самый быстрый способ это лифт...",
	L"В начале игры вы выбрали сохранение \"между боями\", поэтому нельзя сохранить игру во время боя.",
	L"(Нельзя сохраняться во время боя)",
	L"Название кампании длиннее 30 символов.",
	L"Текущая кампания не найдена.",
	L"Кампания: По умолчанию ( %S )",
	L"Кампания: %S",
	L"Вы выбрали кампанию %S. Она является модификацией оригинальной игры Цена Свободы. Вы уверены, что хотите играть кампанию %S?",
	L"Чтобы воспользоваться редактором, смените кампанию по умолчанию на другую.",
	// anv: extra iron man modes
	L"В начале игры вы выбрали сохранение \"между перестрелками\", поэтому нельзя сохранить игру в пошаговом режиме.",
	L"В начале игры вы выбрали сохранение \"один раз в день\", поэтому сохранить игру можно лишь раз в сутки, в %02d:00.",
};

// The_bob : pocket popup text defs
STR16	gszPocketPopupText[]=
{
	L"Гранатомёты",	// POCKET_POPUP_GRENADE_LAUNCHERS,
	L"Ракетницы",	// POCKET_POPUP_ROCKET_LAUNCHERS
	L"Холодное и метательное оружие",	// POCKET_POPUP_MEELE_AND_THROWN
	L"- нет подходящих патронов -",	//POCKET_POPUP_NO_AMMO
	L"- нет оружия в инвентаре -",	//POCKET_POPUP_NO_GUNS
	L"ещё...",		//POCKET_POPUP_MOAR
};

// Flugente: externalised texts for some features
STR16	szCovertTextStr[]=
{
	L"%s одет в камуфляж!",
	L"%s несет рюкзак!",
	L"%s несет труп!",
	L"%s: %s выглядит подозрительно!",
	L"%s: %s - это военное оборудование!",
	L"%s несет слишком много оружия!",
	L"%s: %s слишком круто для солдата %s!",
	L"%s: Слишком много навески на %s!",
	L"%s занимается подозрительной деятельностью!",
	L"%s не выглядит как гражданский!",
	L"%s имеет кровотечение!",
	L"%s пьян и ведет себя не как солдат!",
	L"При ближайшем рассмотрении, маскировка %s не выдерживает проверки!",
	L"%s не должен находиться здесь!",
	L"%s не должен находиться здесь в данное время!",
	L"%s был замечен около свежего трупа!",
	L"%s имеет нестандартную экипировку!",
	L"%s целится в %s!",
	L"%s сумел разоблачить маскировку %s!",
	L"В Items.xml нет предметов одежды!",
	L"Не работает со старой системой (OAS)!",
	L"Недостаточно ОД!",
	L"Обнаружена неверная палитра!",
	L"Вам необходим навык шпиона для этого!",
	L"Не найдена униформа!",
	L"%s переоделся в гражданского.",
	L"%s переоделся в солдата.",
	L"%s носит униформу неправильного образца!",
	L"Требовать сдачи в плен, будучи переодетым, было не очень осмотрительно...",
	L"%s был разоблачен!",
	L"%s: маскировка выглядит нормально...",
	L"%s: маскировка не выдержит проверки.",
	L"%s был пойман на краже!",
	L"%s пытался залезть в инвентарь %s.",
	L"Элитный солдат не распознал %s!",	// TODO.Translate
	L"Офицер не знаком с %s!",
};

STR16	szCorpseTextStr[]=
{
	L"Предмета 'голова' не найдено в Items.xml!",
	L"Тело невозможно обезглавить!",
	L"Мясных изделий не найдено в Items.xml!",
	L"Ты больной! Тебе лечиться надо! Действие невозможно.",
	L"Снимать с тела нечего!",
	L"%s не может снять одежду с этого трупа!",
	L"Это тело невозможно забрать с собой!", //This corpse cannot be taken!
	L"Освободите руки, чтобы тянуть тело!", //No free hand to carry corpse
	L"Предметов тела не найдено в Items.xml!", //No corpse item found in Items.xml
	L"Неверный ID тела!", //Invalid corpse ID
};

STR16	szFoodTextStr[]=
{
	L"%s не хочет есть %s",
	L"%s не хочет пить %s",
	L"%s ест %s",
	L"%s пьёт %s",
	L"%s ослабел от переедания!",
	L"%s ослабел от голода!",
	L"%s потерял здоровье от переедания!",
	L"%s потерял здоровье от голода!",
	L"%s ослабел от того, что выпил слишком много воды!",
	L"%s ослабел от жажды!",
	L"%s потерял здоровье от того, что выпил слишком много воды!",
	L"%s потерял здоровье от жажды!",
	L"Наполнение фляжек в секторе невозможно, Система Еды отключена!"
};

STR16	szPrisonerTextStr[]=
{
	L"%d офицеров, %d спецназа, %d солдат, %d полиции, %d генералов и %d гражданских было допрошено",
	L"Gained $%d as ransom money.",		// TODO.Translate
	L"%d пленных выдали расположение отрядов армии.",
	L"%d офицеров, %d спецназ, %d рядовых и %d полицейских решили присоединиться к нам.",
	L"Пленные устроили бунт в %s!",
	L"%d пленные были отправлены в %s!",
	L"Пленные были отпущены!",
	L"Армия контролирует тюрьму %s, все пленные были освобождены!",
	L"Противник отказывается сдаться!",
	L"Противник отказывается взять вас в плен - они предпочли бы видеть вас мертвыми!",
	L"Этот режим отключен в настройках.",
	L"%s освободил %s!",
	L"В %s был раскрыт высокопоставленный офицер!",
	L"Вражеский командир отказывается даже подумать о сдаче!",
	L"%d заключенных добровольно присоединились к нашим силам.",
};

STR16	szMTATextStr[]=
{
	L"ничего",
	L"укрепление строится",
	L"укрепление убирается",
	L"взлом",
	L"%s был вынужден прекратить %s.",
	L"Выбранное укрепление не может быть построено в этом секторе",
};

STR16	szInventoryArmTextStr[]=
{
	L"Взорвать (%d ОД)",
	L"Взорвать",
	L"Активировать (%d ОД)",
	L"Активировать",
	L"Обезвредить (%d ОД)",
	L"Обезвредить",
};

STR16		szBackgroundText_Flags[]=
{
	L" может употреблять наркотики из инвентаря\n",
	L" вне зависимости от других особенностей биографии\n",
	L" +1 уровень в подземных секторах\n",
	L" steals money from the locals sometimes\n", // TODO.Translate

	L" +1 уровень ловушек при установке мин\n",
	L" разлагает других наёмников\n",
	L" только женский",							// won't show up, text exists for compatibility reasons
	L" только мужской",							// won't show up, text exists for compatibility reasons

	L" значительный штраф к лояльности во всех городах, если умирает\n",

	L" отказывается атаковать животных\n",
	L" отказывается атаковать тех, кто состоит в той же группе\n",
};

STR16	szBackgroundText_Value[]=
{
	L" %s%d%% ОД в холодных секторах\n",
	L" %s%d%% ОД в пустынных секторах\n",	
	L" %s%d%% ОД в болотных секторах\n",
	L" %s%d%% ОД в городских секторах\n",
	L" %s%d%% ОД в лесных секторах\n",
	L" %s%d%% ОД на равнинных секторах\n",
	L" %s%d%% ОД в речных секторах\n",
	L" %s%d%% ОД в тропических секторах\n",
	L" %s%d%% ОД в секторах на побережье\n",
	L" %s%d%% ОД в горных секторах\n",

	L" %s%d%% подвижность\n",
	L" %s%d%% ловкость\n",
	L" %s%d%% сила\n",
	L" %s%d%% лидерство\n",
	L" %s%d%% меткость\n",
	L" %s%d%% механика\n",	
	L" %s%d%% взрывчатка\n",
	L" %s%d%% медицина\n",
	L" %s%d%% интеллект\n",

	L" %s%d%% ОД на крышах\n",
	L" %s%d%% ОД для плавания\n",
	L" %s%d%% ОД для строительства укреплений\n",
	L" %s%d%% ОД для стрельбы из миномётов\n",
	L" %s%d%% ОД для доступа к инвентарю\n",
	L" смотрит в разных направлениях при высадке\n %s%d%% ОД после высадки\n",
	L" %s%d%% ОД на первом ходу при штурме сектора\n",
		
	L" %s%d%% скорость движения пешком\n",
	L" %s%d%% скорость движения на машине\n",
	L" %s%d%% скорость движения на воздушном транспорте\n",
	L" %s%d%% скорость движения на водном транспорте\n",

	L" %s%d%% сопротивление страху\n",
	L" %s%d%% сопротивление подавлению\n",
	L" %s%d%% физическое сопротивление\n",
	L" %s%d%% сопротивление алкогол.\n",
	L" %s%d%% сопротивление заболеванию\n",

	L" %s%d%% эффективность ведения допроса\n",
	L" %s%d%% эффективность тюремной охраны\n",
	L" %s%d%% лучше цены при торговле оружием и патронами\n",
	L" %s%d%% лучшие цены при торговле броней, разгрузками, ножами, наборам и др.\n",
	L" %s%d%% к силе капитуляции при ведении переговоров\n",
	L" %s%d%% быстрее бег\n",
	L" %s%d%% скорость перевязки\n",
	L" %s%d%% востановление дыхания\n",
	L" %s%d%% сила для переноски вещей\n",
	L" %s%d%% потребление еды\n",
	L" %s%d%% потребление воды\n",
	L" %s%d потребность в сне\n",
	L" %s%d%% урон от ножей\n",	
	L" %s%d%% точность атаки ножами\n",
	L" %s%d%% эффективность камуфляжа\n",
	L" %s%d%% скрытность\n",
	L" %s%d%% макс. шанс попадания\n",
	L" %s%d дальность слуха ночью\n",
	L" %s%d дальность слуха днем\n",
	L" %s%d эффективность при разминировании ловушек\n",
	L" %s%d%% шанс попадания ПВО\n",

	L" %s%d%% эффективность дружеского обращения\n",
	L" %s%d%% эффективность прямого обращении\n",
	L" %s%d%% эффективность угроз\n",
	L" %s%d%% эффективность найма\n",

	L" %s%d%% шанс попадания с вышибными зарядами\n",
	L" %s%d%% шанс попадания с оружием против животных\n",
	L" %s%d%% стоимость страховки\n",
	L" %s%d%% эффективность наблюдателя в снайперской паре\n",
	L" %s%d%% эффективность в диагностике заболевания\n",
	L" %s%d%% эффективность в лечении населения от болезней\n",
	L"Замечает следы на расстоянии до %d метров\n",
	L" %s%d%% начальное расстояние до противника в засаде\n",
	L" %s%d%% шанс избежать укуса змей\n",

	L" не нравиться другое прошлое\n",
	L"Курит",
	L"Не курит",
	L" %s%d%% enemy CTH if crouched against thick cover in their direction\n",
	L" %s%d%% кровотечение\n",
	L" Взлом: %s%d ",
	L" %s%d%% burial speed\n",	// TODO.Translate
	L" %s%d%% administration effectiveness\n",	// TODO.Translate
	L" %s%d%% exploration effectiveness\n",	// TODO.Translate
};

STR16		szBackgroundTitleText[] = 
{
	L"I.M.P. Биография",
};

// Flugente: personality
STR16		szPersonalityTitleText[] = 
{
	L"I.M.P. Предубеждения",
};

STR16		szPersonalityDisplayText[]=
{
	L"Вы выглядите",
	L"и ваша внешность",
	L"важна для вас.",
	L"У вас",
	L" и вы",
	L"переживаете за это.",
	L"Вы",
	L"и ненавидите всех",
	L".",
	L"расист по отношению к не ",
	L".",
};

// texts showing up when hovering over the box, used to explain what a selection does. Do not use more than 200 characters!
STR16		szPersonalityHelpText[]=
{
	L"Как вы выглядите?",
	L"Насколько для вас важен внешний вид других?",
	L"Ваша манера поведения?",
	L"Насколько для вас важна манера поведения других?",
	L"Ваша национальность?",
	L"Какая национальность вам неприятна?",
	L"Насколько вам неприятна эта национальность?",
	L"насколько вы расист?",
	L"Ваша раса? Вы будете\nотноситься плохо к другим расам.",
	L"Насколько вы сексист по отношению к другим полам?",
};


STR16		szRaceText[]=
{
	L"белым",
	L"черным",
	L"азиатам",
	L"эскимосам",
	L"испанцам",
};

STR16		szAppearanceText[]=
{
	L"средне",
	L"уродливо",
	L"невзрачно",
	L"привлекательно",
	L"как ребенок",
};

STR16		szRefinementText[]=
{
	L"обычное поведение",
	L"хамское поведение",
	L"снобское поведение",
};

STR16		szRefinementTextTypes[] =
{
	L"нормальные люди",
	L"жлобы",
	L"снобы",
};

STR16		szNationalityText[]=
{
	L"Американец",		// 0
	L"Араб",
	L"Австралиец",
	L"Британец",
	L"Канадец",
	L"Кубинец",			// 5
	L"Датчанин",
	L"Француз",
	L"Русский",
	L"Нигериец",
	L"Швейцарец",			// 10
	L"Ямаец",
	L"Поляк",
	L"Китаец",
	L"Ирладец",
	L"Южноафриканец",	// 15
	L"Венгр",
	L"Шотландец",
	L"Арулькиец",
	L"Немец",
	L"Африканец",			// 20
	L"Итальянец",
	L"Голландец",
	L"Румын",
	L"Метавирец",

	// newly added from here on
	L"Грек",			// 25
	L"Эстонец",
	L"Венесуэлец",
	L"Японец",
	L"Турок",
	L"Индиец",			// 30
	L"Мексиканец",
	L"Норвежец",
	L"Испанец",
	L"Бразилец",
	L"Финн",			// 35
	L"Иранец",
	L"Израильтянин",
	L"Болгарин",
	L"Швед",
	L"Иракец",			// 40
	L"Сириец",
	L"Бельгиец",
	L"Португалец",
	L"Белорус",
	L"Серб",			// 45
	L"Пакистанец",
	L"Албанец",
	L"Аргентинец",
	L"Армянин",
	L"Азербайджанец", // 50
	L"Боливиец",
	L"Чилиец",
	L"Черкес",
	L"Колумбиец",
	L"Египтянин", // 55
	L"Эфиоп",
	L"Грузин",
	L"Иорданец",
	L"Казахстанец",
	L"Кениец", // 60
	L"Кореец",
	L"Кыргызстанец",
	L"Монгол",
	L"Палестинец",
	L"Панамец", // 65
	L"Родезиец",
	L"Сальвадорец",
	L"Саудовец",
	L"Сомалиец",
	L"Таец", // 70
	L"Украинец",
	L"Узбекистанец",
	L"Валлиец",
	L"Езид",
	L"Зимбабвиец", // 75
};

STR16		szNationalityTextAdjective[] =
{
	L"американцев",		// 0
	L"арабов",
	L"австралийцев",
	L"британцев",
	L"канадцев",
	L"кубинцев",			// 5
	L"датчан",
	L"французов",
	L"русских",
	L"нигерийцев",
	L"швейцарцев",			// 10
	L"ямайцев",
	L"поляков",
	L"китайцев",
	L"ирладцев",
	L"южноафриканцев",	// 15
	L"венгров",
	L"шотландецев",
	L"арулькийцев",
	L"немцев",
	L"африканцев",			// 20
	L"итальянцев",
	L"голландцев",
	L"румынов",
	L"метавирцев",

	// newly added from here on
	L"греков",			// 25
	L"эстонцев",
	L"венесуэльцев",
	L"японцев",
	L"турков",
	L"индийцев",			// 30
	L"мексиканцев",
	L"норвежцев",
	L"испанцев",
	L"бразильцев",
	L"финнов",			// 35
	L"иранцев",
	L"израильтянинов",
	L"болгаров",
	L"шведов",
	L"иракцев",			// 40
	L"сирийцев",
	L"бельгийцев",
	L"португальцев",
	L"белорусов",
	L"сербов",			// 45
	L"пакистанцев",
	L"албанцев",
	L"аргентинцев",
	L"армян",
	L"азербайджанцев", // 50
	L"боливийцев",
	L"чилийцев",
	L"черкесов",
	L"колумбийцев",
	L"египтян", // 55
	L"эфиопов",
	L"грузин",
	L"иорданцев",
	L"казахстанцев",
	L"кенийцев", // 60
	L"корейцев",
	L"кыргызстанцев",
	L"монголов",
	L"палестинцев",
	L"панамцев", // 65
	L"родезийцев",
	L"сальвадорцев",
	L"саудовцев",
	L"сомалийцев",
	L"тайцев", // 70
	L"украинцев",
	L"узбекистанцев",
	L"валлийцев",
	L"езидов",
	L"зимбабвийцев", // 75
};

// special text used if we do not hate any nation (value of -1)
STR16		szNationalityText_Special[]=
{
	L"нормально относится к другим национальностям.",		// used in personnel.cpp
	L"неизвестно",		// used in IMP generation
};

STR16		szCareLevelText[]=
{
	L"не",
	L"немного",
	L"сильно",
};

STR16		szRacistText[]=
{
	L"не",
	L"немного",
	L"сильно",
};

STR16		szSexistText[]=
{
	L"не сексист",
	L"сексист",
	L"убежденный сексист",
	L"джентльмен",
};

// Flugente: power pack texts
STR16 gPowerPackDesc[] =
{
	L"Батарейки: ",
	L"заряжены",
	L"хороший заряд",
	L"половина заряда",
	L"низкий заряд",
	L"разряжены",
	L"."
};

// WANNE: Special characters like % or someting else should go here
// We can't put them directly in the CPP code files, because they need special encoding (UTF8) for some languages (e.g: Chinese)
STR16 sSpecialCharacters[] =
{
	L"%",		// Percentage character
};

STR16	szSoldierClassName[]=
{
	L"Наёмник",
	L"Новобранец",
	L"Рядовой",
	L"Ветеран",

	L"Гражданский",

	L"Полицейский",
	L"Солдат",
	L"Спецназ",
	L"Танк",

	L"Тварь",
	L"Зомби",
};

STR16	szCampaignHistoryWebSite[]=
{
	L"Пресс-служба %s",
	L"Министерство распространения информации %s",
	L"Революционное движение %s",
	L"The Times International",
	L"International Times",
	L"R.I.S. (Служба информационной разведки)",

	L"Информация от %s",
	L"Мы - нейтральный источник информации. Мы собираем различные новостные статьи от %s. Мы не оцениваем эти источники - мы просто публикуем их, так что вы сами можете составить свое мнение. Мы публикуем статьи из различных источников, среди них ",
	
	L" Общая сводка",
	L" Боевые отчёты",
	L" Новости",
	L" О нас",
};

STR16	szCampaignHistoryDetail[]=
{
	L"%s, %s %s %s в %s.",

	L"ополченцы",
	L"армия",

	L"атаковали",
	L"попал в засаду",
	L"высадился",
		
	L"Атака осуществлена с %s.",
	L"Подкрепление для %s пришли с %s.",
	L"Атакованы с %s, %s получили подкрепления с %s.",
	L"севера",
	L"востока",
	L"юга",
	L"запада",
	L"и",
	L"неизвестно",  //an unknown location

	L"Здания в секторе получили повреждения.",
	L"Во время боя здания в секторе получили повреждения, %d гражданских были убиты и %d ранены.",
	L"Во время атаки %s и %s вызвали подкрепления.",
	L"Во время атаки %s вызвали подкрепления.",
	L"Очевидцы отмечают использование химического оружия обеими сторонами.",
	L"Химическое оружие было использовано %s.",
	L"В результате серьезной эскалации конфликта отмечается использование танков обеими сторонами.",
	L"%d танков было использовано %s, %d было уничтожено в ожесточенном бою.",
	L"Обе стороны утверждают, что использовали снайперов.",
	L"По непроверенным данным, у %s в столкновении участвовали снайперы.",
	L"Этот сектор имеет большое стратегическое значение, поскольку в нем расположена одна из батарей ПВО, которыми располагает армия %s. Аэрофотосьемка показывает, что центр управления получил серьезные повреждения. В результате воздушное пространство над %s некоторое время будет незащищенным.",
	L"Ситуация становится все более запутанной, так как, судя по всему, разногласия между повстанцами достигли серьезного уровня. У нас есть подтверждение, что между повстанцами и иностранными наёмниками произошел бой.",
	L"Положение правительственных войск оказалось более шатким, чем представлялось ранее. Есть свидетельства раскола и стрельбы между армейскими подразделениями.",
};

STR16	szCampaignHistoryTimeString[]=
{
	L"Глубокой ночью",		// 23 - 3
	L"На рассвете",					// 3 - 6
	L"Рано утром",	// 6 - 8
	L"Утром",	// 8 - 11
	L"В полдень",					// 11 - 14
	L"После полудня",		// 14 - 18
	L"Вечером",			// 18 - 21
	L"Ночью",		// 21 - 23
};

STR16	szCampaignHistoryMoneyTypeString[]=
{
	L"Начальные финансы",
	L"Доход от шахт",
	L"Торговля",
	L"Другие источники",
};

STR16	szCampaignHistoryConsumptionTypeString[]=
{
	L"Патроны",
	L"Взрывчатка",
	L"Еда",
	L"Медикаменты",
	L"Обслуживание",
};

STR16	szCampaignHistoryResultString[]=
{
	L"Армейские силы были уничтожены без особого сопротивления.",

	L"Повстанцы легко сокрушили противника, нанеся тяжелый урон.",	
	L"Повстанцы без особых усилий нанесли противнику тяжелый урон и захватили несколько пленных.",

	L"Повстанцы сумели победить противника в результате тяжелого боя. Армия понесла потери.",
	L"Повстанцы понесли потери, но смогли нанести поражение правительственным войскам. По непроверенным данным, несколько солдат могли попасть в плен.",

	L"Повстанцы сумели нанести поражение правительственным войскам, но из-за высоких потерь эта победа может оказаться пирровой. Неизвестно, сумеют ли они удержать завоеванные позиции под непрерывными атаками противника.",
	
	L"Численное превосходство армии принесло свои плоды. Повстанцы не имели ни единого шанса и вынуждены были отступить, чтобы избежать гибели или плена.",	
	L"Несмотря на численное превосходство повстанцев в этом секторе, армия легко разделалась с ними.",
	
	L"Повстанцы не были готовы противостоять правительственным войскам, которые превосходили их в численности и вооружении, и были разбиты армией с легкостью.",	
	L"Несмотря на то, что на стороне повстанцев было численное преимущество, армия оказалась лучше вооружена. Повстанцы потерпели поражение",

	L"В результате ожесточенного боя обе стороны понесли существенные потери, но в итоге сказалось численное преимущество армии. Повстанцы были разгромлены. Возможно, часть из них сумела выжить, но в настоящий момент мы не можем проверить эту информацию.",	
	L"В произошедшей интенсивной перестрелке сказалась лучшая подготовка армии, и повстанцы были вынуждены отступить.",

	L"Ни одна из сторон не собиралась уступать. Несмотря на то, что армия устранила угрозу восстания в районе, понесенные потери означают, что армейское подразделение существует только формально. Однако, если армия может позволить себе воевать с таким уровнем потерь, повстанцы очень скоро физически закончатся.",
};

STR16	szCampaignHistoryImportanceString[]=
{
	L"Неприменимый",
	L"Неважный",
	L"Заметный",
	L"Примечательный",
	L"Существенный",
	L"Интересный",
	L"Важный",
	L"Очень важный",
	L"Серьезный",
	L"Значительный",
	L"Важнейший",
};

STR16	szCampaignHistoryWebpageString[]=
{
	L"Убито",
	L"Ранено",
	L"Пленных",
	L"Выстрелов",

	L"Денег",
	L"Издержки",
	L"Потери",
	L"Участники",

	L"Повышеный",
	L"Общий",
	L"Полный",
	L"Назад",

	L"Далее",
	L"Эпизод",
	L"День",
};

STR16	szCampaignStatsOperationPrefix[] =	// TODO.Translate
{
	L"Славный %s",
	L"Мощный %s",
	L"Грозный %s",
	L"Пугающий %s",

	L"Могучий %s",
	L"Поразительный %s",
	L"Вероломный %s",
	L"Скорый %s",

	L"Неистовый %s",
	L"Жестокий %s",
	L"Безжалостный %s",
	L"Беспощадный %s",

	L"Каннибальский %s",
	L"Великолепный %s",
	L"Плутливый %s",
	L"Неясный %s",

	L"Неопределенный %s",
	L"Сжигающий %s",
	L"Разгневанный %s",
	L"Воображаемый %s",

	// 20
	L"Ужастный %s",
	L"Плевать-на-право %s",
	L"Спровоцированный %s",
	L"Непрестанный %s",

	L"Жесткий %s",
	L"Упорный %s",
	L"Безрассудный %s",
	L"Невозможный %s",

	L"Страшнейший %s",
	L"Внезапный %s",
	L"Демократический %s",
	L"Разрывной %s",

	L"Двухпартийный %s",
	L"Кровавый %s",
	L"Румяный %s",
	L"Невинный %s",

	L"Злобный %s",
	L"Дразнящий %s",
	L"Уничтожающий %s",
	L"Стойкий %s",

	// 40
	L"Немилосердный %s",
	L"Сумасшедший %s",
	L"Окончательный %s",
	L"Яростный %s",

	L"Лучше избежать нашего %s",
	L"Страшный %s",
	L"Благодарный %s!",
	L"Защитить %s",

	L"Внимательный %s",
	L"Крушаший %s",
	L"Колющий %s",
	L"Побеждающий %s",

	L"Садистский %s",
	L"Горящий %s",
	L"Гневный %s",
	L"Невидимый %s",

	L"Виновный %s",
	L"Гниющий %s",
	L"Очищающий %s",
	L"Беспокойный %s",

	// 60
	L"Страый %s",
	L"Голодный %s",
	L"Спящий %s",
	L"Угрюмый %s",

	L"Гибкий %s",
	L"Назойливый %s",
	L"Обиженный %s",
	L"Привлекательный %s",

	L"Кричащий %s",
	L"Прячущийся %s",
	L"Молящийся %s",
	L"Бродящий %s",

	L"Хладнокровный %s",
	L"Бесстрашный %s",
	L"Быстроногий %s",
	L"Праклятый %s",

	L"Вегетарианский %s",
	L"Нелепый %s",
	L"Отсталый %s",
	L"Превосходный %s",

	// 80
	L"Героический %s",
	L"Подходящий %s",
	L"Смотрящий %s",
	L"Отравленный %s",

	L"Неожиданный %s",
	L"Продолжительный %s",
	L"Веселящий %s",
	L"%s - обмен!",

	L"%s на стероидах",
	L"%s против Прищельцев",
	L"%s с твистом",
	L"Удовлетворяющий %s",

	L"Гибрид Человек-%s",
	L"Пустынный %s",
	L"Дорогой %s",
	L"Полуночный %s",

	L"Капиталистический %s",
	L"Коммунистический %s",
	L"Значительный %s",
	L"Устойчивый %s",

	// 100
	L"Ннарколептический %s",
	L"Отбеливающий %s",
	L"Дробящий %s",
	L"Бьющий %s",

	L"Кровожадный %s",
	L"Тучный %s",
	L"Лукавый %s",
	L"Древовидный %s",

	L"Дешевый %s",
	L"Удовлетворенный %s",
	L"Ложный %s",
	L"%s в помощь",

	L"Крабы против %s",
	L"%s в космосе!!!",
	L"%s против Годзиллы",
	L"Ннеукрощённый %s",

	L"Надёжный %s",
	L"Медный %s",
	L"Алчный %s",
	L"Ночной %s",

	// 120
	L"Мешающий %s",
	L"Сердитый %s",
	L"Противный %s",
	L"Мманиакальный %s",

	L"Древний %s",
	L"Крадущийся %s",
	L"%s Рока",
	L"Месть %s",

	L"%s в бегах",
	L"%s вне времени",
	L"Один с %s",
	L"%s из Ада",

	L"Супер-%s",
	L"Ультра-%s",
	L"Мега-%s",
	L"Гига-%s",

	L"Частичный %s",
	L"Отличный %s",
	L"Дрожащий %s",
	L"Ободряющий %s",

	// 140
};

STR16	szCampaignStatsOperationSuffix[] =
{
	L"Дракон",
	L"Горный Лев",
	L"Красноголовый Змей",
	L"Терьерь Рассела",

	L"Грод Немезида",
	L"Василиск",
	L"Клинок",
	L"Щит",

	L"Молот",
	L"Призрак",
	L"Конкресс",
	L"Нефтяной Бур",

	L"Друг",
	L"Товарищ",
	L"Муж",
	L"Тесть",

	L"Дюнный Змей",
	L"Банкир",
	L"Удав",
	L"Кот",

	// 20
	L"Съезд",
	L"Сенат",
	L"Церковник",
	L"Задира",

	L"Штык",
	L"Волк",
	L"Солдат",
	L"Древестник",

	L"Писец",
	L"Куст",
	L"Асфальт",
	L"Закат",

	L"Ураган",
	L"Оцелот",
	L"Тигр",
	L"Завод",

	L"Леопард",
	L"Демон",
	L"Овод",
	L"Ротвейлер",

	// 40
	L"Кузен",
	L"Дед",
	L"Новорожденный",
	L"Сектант",

	L"Антибиотик",
	L"Демократ",
	L"Полкоодец",
	L"Молот Судного Дня",

	L"Министр",
	L"Бобр",
	L"Террорист",
	L"Дождь Смерти",

	L"Пророк",
	L"Торговец",
	L"Крестоносец",
	L"Управленец",

	L"Пульсар",
	L"Отпуск",
	L"Взрыв",
	L"Хищник",

	// 60
	L"Мантикор",
	L"Ледяной Гигант",
	L"Антракт",
	L"Средний Класс",

	L"Крикун",
	L"Козел",
	L"Пес",
	L"Ответ",

	L"Бункер",
	L"Мим",
	L"Проводник",
	L"Работодатель",

	L"Француз",
	L"Момент",
	L"Тритон",
	L"Дурак",

	L"Степной Волк",
	L"Железный Молот",
	L"Лорд",
	L"Правитель",

	// 80
	L"Диктатор",
	L"Старик",
	L"Измельчитель",
	L"пылесос",

	L"Хомяк",
	L"Гипноз",
	L"Диджей",
	L"Uробовщик",

	L"Гном",
	L"Ребенок",
	L"Гангстер",
	L"Мясоед",

	L"Бог",
	L"Гендер",
	L"Моль",
	L"Малыш",

	L"Самолет",
	L"Гражданин",
	L"Любовни",
	L"Фонд",

	// 100
	L"Формат",
	L"Меч",
	L"Барс",
	L"Ирбис",

	L"Кентавр",
	L"Скорпион",
	L"Серпент",
	L"Черный Паук",

	L"Тарантул",
	L"Гриф",
	L"Еретик",
	L"Кретин",

	L"Образец",
	L"Цербер",
	L"Мангуст",
	L"Ухажер",

	L"Монах",
	L"Скиталец",
	L"Гад",
	L"Сом",

	// 120
	L"Греховник",
	L"Праведник",
	L"Астероид",
	L"Метеор",

	L"Клубок Проблем",
	L"Рыбий Жир",
	L"Молочник",
	L"Ус",

	L"Псих",
	L"Безумец",
	L"рефлекс",
	L"Знак",

	L"Король",
	L"Принц",
	L"Епископ",
	L"Раб",

	L"Оплот",
	L"Дворец",
	L"Конь",
	L"Суд",

	// 140
};

STR16	szMercCompareWebSite[] =
{
	// main page
	L"Бойцы Любят Тебя или Клянут",
	L"Ваш эксперт №1 по сплочению комманды",

	L"Кто мы?",
	L"Анализ команды",
	L"Парные сравнения",
	L"Персоналии",
	L"Отзывы клиентов",

	L"Если ваш бизнес предполагает инновационные решения для критических приложений, работающих в режиме реального времени, возможно, некоторые из этих наблюдений будут вам знакомы:",
	L"Ваша команда борется сама с собой.",
	L"Ваши сотрудники тратят время мешая друг другу.",
	L"В вашем коллективе большая текучесть кадров.",
	L"Вы постоянно получаете низкие оценки по удовлетворенности персонала работой.",	
	L"Если вы столкнулись хотя бы с одной из этих ситуаций, то, возможно, в вашем деле есть проблемы. И скорее всего, ваши сотрудники не станут работать в полную силу. Но благодаря нашей запатентованной и простой для понимания системе \"БоЛТиК\", вы сможете  вернуть производительность и счастливые улыбки на лицах всех ваших сотрудников в мгновение ока!",

	// customer quotes
	L"Несколько цитат наших благодарных клиентов:",
	L"Мои прошлые отношения были просто ужасны. Я винила во всём себя... но теперь то я знаю. Все мужчины заслуживают смерти! Спасибо тебе, \"БоЛТиК\", за моё прозрение!",
	L"-Луиза Г., романист-",
	L"Я никогда не ладил со своими братьями, а в последнее время отношения стали совсем из рук вон плохими. Вы показали мне, что всему виной было постыдное недоверие к отцу. Спасибо вам за это! Я должен открыто всё сказать отцу.",
	L"-Конрад C., Исправительное учреждение-",
	L"Я одиночка по жизни, и работа в команде была просто пыткой. Но методика \"БоЛТиК\" показала мне как стать частью команды. Этот вклад просто неоценим!",
	L"-Грант В., Заклинатель змей-",
	L"В моей работе необходимо доверять каждому члену команды на все сто процентов. Вот почему мы обратились к экспертам - к компании \"БоЛТиК\".",
	L"-Халлэ Л., СПК-",
	L"Прежде всего, хочу признать, что наш коллектив был весьма разношерстным, из-за чего часто случались конфликты. Но со временем мы достигли взаимоуважения, и теперь дополняем друг друга.",
	L"-Майкл C., НАСА-",
	L"Рекоммендую отдать предпочтение этому сайту!",
	L"-Каспар Х., Логистическая компания H&C-",
	L"Наш процесс тренировок должен быть максимально быстрым, поэтому мы должны знать, с кем имеем дело. Компания \"БоЛТиК\" стала очевидным выбором.",
	L"-Стэнли Дюк, Корпорация Цербер-",

	// analyze
	L"Выберите наёмника",
	L"Выберите отряд",

	// error messages
	L"В настоящее время у вас нет наёмников на службе. Боевой дух бойцов ниже нормы - основная причина частых увольнений среди персонала.",
};

STR16	szMercCompareEventText[]=
{
	L"%s подстрелил(а) меня!",
	L"%s плетёт интриги за моей спиной",
	L"%s вмешивается в мои дела",
	L"%s дружит с моим врагом",

	L"%s получил(а) контракт до меня",
	L"%s приказал(а) позорно отступить",
	L"%s убивает невинных",
	L"%s тормозит нас",

	L"%s не делится едой",
	L"%s ставит под угрозу миссию",
	L"%s наркоман",
	L"%s воровская гадина",

	L"%s никакущий командир",
	L"%s переплачивают",
	L"%s получает всё самое лучшее",
	L"%s держит меня на мушке",

	L"%s обрабатывал(а) мои раны",
	L"Хорошо посидели за бутылочкой с %s",
	L"C %s классно напиваться",
	L"%s доставучий(-ая) когда пьян(а)",

	L"%s идиот когда пьян",
	L"%s не поддерживает нашу точку зрения",
	L"%s поддерживает нашу позицию",
	L"%s соглашается с нашими доводами",

	L"Убеждения %s противоречат нашим",
	L"%s знает, как успокоить людей",
	L"%s бесчувственный(-ая)",
	L"%s ставит людей на своё место",

	L"%s очень импульсивен(а)",
	L"%s - разносчик заразы",
	L"%s лечит мои заболевания",
	L"%s не сдерживается в бою",

	L"%s наслаждается битвой сверх меры",
	L"%s хороший преподаватель",
	L"%s привёл(а) нас к победе",
	L"%s спас(ла) мою жизнь",

	L"%s украл(а) моё убийство",
	L"%s и я отлично работаем вместе",
	L"%s заставил(а) противника сдаться",
	L"%s ранил(а) гражданских",
};

STR16	szWHOWebSite[] =
{
	// main page
	L"Всемирная организация здравоохранения",
	L"Возвращаем здоровье в жизнь",

	// links to other pages
	L"О ВОЗ",
	L"Болезни Арулько",
	L"О заболеваниях",

	// text on the main page
	L"ВОЗ является органом, направляющим и координирующим международную работу в области здравоохранения в рамках системы ООН.",
	L"ВОЗ выполняет следующие функции:                                                     -обеспечение глобального лидерства в области общественного здравоохранения;   -составление повестки дня исследований в области здравоохранения;                        -установление норм и стандартов;                                                                -формулирование этических и основанных на фактических данных вариантов политики;   -оказание технической поддержки странам;                                                   -мониторинг ситуации и оценка тенденций в области здравоохранения.",
	L"Здравоохранение в 21-м веке является общей ответственностью, включая равноправный доступ к основной помощи и коллективной защите от международных угроз.",

	// contract page
	L"Маленькую страну Арулько в настоящее время потрясают хаотичные вспышки смертельного вируса арулькийской чумы.",
	L"Из-за катастрофического состояния системы здравоохранения страны только армейский лечебный корпус способен бороться со смертельным заболеванием.",
	L"Страна Арулько закрыта для партнёров ООН, поэтому, всё что мы можем предоставить - это подробные карты о текущем состоянии инфекционных заболеваний. Из-за трудностей в работе с Арулько мы вынуждены брать ежедневную плату со всех желающих получать актуальную информацию по заболеваемости в размере %d$.",
	L"Желаете приобрести подробные данные о текущем состоянии заболеваемости в Арулько? Доступ к этим данным вы можете получить на стратегической карте, оплатив стоимость нашей услуги.",
	L"В настоящее время вы не имеете доступа к данным ВОЗ по арулькийской чуме.",
	L"Вы приобрели подробные карты о состоянии заболеваемости.",
	L"Заказать обновление карт",
	L"Отказаться от карт",

	// helpful tips page
	L"Арулькийская чума - это смертельный штамм чумы, который распространён лишь на небольшой части территории Арулько. Первые жертвы штамма были заражены через укусы комаров в болотистой либо тропической местности и непреднамеренно стали причиной заражения населения близлежащих городов.",
	L"Симптомы болезни проявляются не сразу, и до первых проявлений заражения может пройти несколько дней.",
	L"Чтобы посмотреть от каких известных болезней страдают ваши наёмники, надо подвести курсор к портрету бойца на стратегической карте.",
 	L"Большинство заболеваний прогрессирует с течением времени, поэтому следует начинать лечение как можно скорее.",
	L"Определённые болезни можно вылечить с помощью медикаментов. Некоторые из них вы можете найти в специализированных аптеках.",
	L"Врачам можно дать указание обследовать на предмет заболеваний всех товарищей по команде, находящихся в секторе. Это позволит узнать о болезни, прежде, чем произойдёт массовое заражение!",
	L"Врачи находятся в большей степени риска заразиться при лечении инфицированных пациентов. Средства индивидуальной защиты снизят риск заражения.",
	L"Если клинковое оружие ранит инфицированного человека, лезвие оружия становится заражённым и может стать причиной распространения инфекции.",
};

STR16	szPMCWebSite[] =
{
	// main page
	L"Цербер",
	L"Ваше превосходство в безопасности",

	// links to other pages
	L" Что есть Цербер",
	L" Нанять команду",
	L" Нанять солдата",

	// text on the main page
	L"Корпорация Цербер - хорошо известный международный частный военный подрядчик. Начиная с 1983 года, мы предоставляем свои услуги охраны и обучения подразделений боевого назначения по всему миру.",
	L"Наш профессионально обученный персонал обеспечивает безопасность более 30 правительств по всему миру. В том числе в нескольких горячих точках.",
	L"Наши тренировочные центры есть почти в каждой точке мира, включая Индонезию, Колумбию, Катар, Южную Африку и Румынию. Благодаря этому мы можем, как правило, выполнить свои обязательства по контракту в течение 24 часов.",
	L"В разделе \"Нанять солдата\" мы предлагаем индивидуальные договоры с опытными ветеранами в сфере безопасности.",
	L"Конечно же, вы можете нанять сразу целую роту солдат. В разделе \"Нанять команду\", вы можете указать количество солдат к найму, а так же, выбрать место, где необходимы их услуги. В связи с трагическим инцидентом в прошлом, мы доставляем своих контрактников лишь в зоны, которые находятся под вашим контролем.",
	L"Своих сотрудников мы можем доставить воздухом, в этом случае, в месте прибытия, конечно, должен быть аэропорт. В зависимости от региона прибытия также возможны внедрения/проникновения через порты или пограничные посты.",
	L"Работаем только по предоплате. Далее ежедневная оплата услуг наших сотрудников будет списываться с вашего счёта.",
	
	// militia contract page
	L"Выберите уровень и количество ополченцев:",
	L"Новобранец",
	L"Солдат рядовой",
	L"Солдат ветеран",

	L"%d чел., %d$ каждый",
	L"К найму: %d",
	L"Стоимость: %d$",

	L"Выберите место высадки солдат:",
	L"Общая стоимость: %d$",
	L"РВП: %02d:%02d",
	L"Оплатить",

	L"Благодарим за сотрудничество! Наши солдаты прибудут в место назначения завтра в %02d:%02d.",
	L"Подкрепление из Цербер прибыло в %s.",
	L" Информация: рядовых - %d чел., ветеранов - %d чел. прибудут в %s,  время прибытия %02d:%02d, день %d.",
	L"Под Вашим контролем нет мест, куда бы мы могли прислать своих солдат!",

	// individual contract page
};

STR16   szTacticalInventoryDialogString[]=
{
        L"Команды инвентаря",

        L"ПНВ",
        L"Перезар.",
        L"Собрать",
        L"",

        L"Сортиров",
        L"Объед.",
        L"Отдел.",
        L"Организ.",

        L"Ящики",
        L"Коробки",
        L"Сбросить",
        L"Поднять",

        L"",
        L"",
        L"",
        L"",
};

STR16   szTacticalCoverDialogString[]=
{
        L"Режим отображения укрытий",

        L"Выкл",
        L"Враги",
        L"Боец",
        L"",

        L"Роли",
	L"Укреп.",
        L"Следы",
	L"Шанс",

        L"Ловушки",
        L"Сеть",
        L"Детект.",
        L"",

        L"Сеть A",
        L"Сеть B",
        L"Сеть C",
        L"Сеть D",
};

STR16   szTacticalCoverDialogPrintString[]=
{
        
        L"Выключение отображения укрытий или ловушек",
        L"Показывает опасные зоны",
        L"Показывает что видит боец",
        L"",

		L"Показать символ роли противников",
		L"Показать планируемые укрепления",
		L"Показать следы противника",
        L"",

        L"Показать сеть ловушек",
        L"Показать цветную сеть ловушек",
        L"Показать ближайшие ловушки",
        L"",

        L"Показать сеть ловушек A",
        L"Показать сеть ловушек B",
        L"Показать сеть ловушек C",
        L"Показать сеть ловушек D",
};


STR16	szDynamicDialogueText[40][17] =	// TODO.Translate
{
	// OPINIONEVENT_FRIENDLYFIRE
	L"Какого черта! $CAUSE$ атаковал(а) меня!",
	L"",
	L"",
	L"Что? Я? Никогда! Я целился(-ась) во врага!",
	L"Ой.",
	L"",
	L"",
	L"$CAUSE$ атаковал(а) $VICTIM$. Что ты делаешь?",
	L"Хмм, это должно быть дружественный огонь!",
	L"Да, Я тоже это видел(а)!",
	L"Не увиливай, $CAUSE$. Ты всё четко видел(а)! На чьей ты стороне?",
	L"Я все видел(а), это точно был дружественный огонь!",
	L"Такое может случиться в пылу битвы. $CAUSE$, будь повнимательнее в следующий раз.",
	L"Мы на войне! Люди постоянно ловят пули!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SNITCHSOLDMEOUT
	L"Эй! Забали варежку, $CAUSE$! Стукач!",
	L"",
	L"",
	L"Ага, как же! А ты слабачек",
	L"Ты слышишь это? Чёрт.",
	L"",
	L"",
	L"$VICTIM$ зол с $CAUSE$ потому что $CAUSE_GENDER$ говорил с тобой. Что ты делаешь?",
	L"Nono, $CAUSE$, speak up... What did $VICTIM$ do?",
	L"Yeah, mind your own business, $CAUSE$!",
	L"This isn't girls college, keep your snickering to yourself, $CAUSE$.",
	L"Yeah. Man up!",
	L"I'm not sure whether it was the correct way, but $CAUSE_GENDER$ does have a point...",
	L"This again? Keep your bickering to yourself, we have no time for this!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SNITCHINTERFERENCE
	L"$CAUSE$ is bullying me again!",
	L"",
	L"",
	L"You're jeopardising the entire mission, and I won't let that stand any longer!",
	L"Hey, it's for your own good. You'll thank me later.",
	L"",
	L"",
	L"$CAUSE$ has stopped $VICTIM$ from misbehaving, and $VICTIM_GENDER$ is out for revenge. What do you do?",
	L"Then stop giving reasons for that!",
	L"Drop it, $CAUSE$, who are you to tell us what to do?!",
	L"You won't let that stand? Cute. Who ever made you the boss?",
	L"Agreed. We can't let our guard down for a single moment!",
	L"Can't you two sort this out?",
	L"Meh meh meh. Have any of you losers lost their bib? You're pathetic. ",
	L"",
	L"",
	L"",

	// OPINIONEVENT_FRIENDSWITHHATED
	L"Hmpf. Typical $CAUSE$, figured $CAUSE_GENDER$ would hang out with the wrong crowd!",
	L"",
	L"",
	L"My friends are none of your business!",
	L"Can't you two all get along, $VICTIM$?",
	L"",
	L"",
	L"$VICTIM$ doesn't like $CAUSE$'s friend. What do you do?",
	L"Everybody can hang out with whom $CAUSE_GENDER$ wants!",
	L"Yeah, you guys are ugly!",
	L"Then you should change your business. 'Cause its bad.",
	L"What's that to you, $VICTIM$?",
	L"Yeah yeah, terrible business, that. Are you sure that is the MOST PRESSING issue right now?",
	L"Nobody wants to hear all this crap...",
	L"",
	L"",
	L"",

	// OPINIONEVENT_CONTRACTEXTENSION
	L"Yeah, sure. My contract's about to end, but no, gotta take care of $CAUSE$ first.",
	L"",
	L"",
	L"Oh yeah? I'm actually useful. Perhaps that's why you didn't get an extension.",
	L"I'm sure you'll get your extension soon. You know how odd online banking can be.",
	L"",
	L"",
	L"$VICTIM$ is jealous as we extended $CAUSE$'s contract early. What do you do?",
	L"You are still getting paid, no? What does it matter as long as you get the money?",
	L"And your contract ends so soon, $CAUSE$... I hope you get an extension.",
	L"Yeah. All that worth hasn't shown yet though.",
	L"Aww, that burns, doesn't it, $VICTIM$?",
	L"We have limited funds. Someone needs to get paid first, right?",
	L"You'll all get paid in time. Unless you continue like this. I'm sure there are less annoying mercs out there.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_ORDEREDRETREAT
	L"Nice command, $CAUSE$! Why would you order retreat?",
	L"",
	L"",
	L"I just got us out of that hellhole, you should thank me for saving your life.",
	L"They were flanking us, there was no other way!",
	L"",
	L"",
	L"$VICTIM$ dislikes the retreat command $CAUSE$ gave. What do you do?",
	L"You know you can go back if you want... nobody's stopping you.",
	L"We were rockin' it until that point.",
	L"Oh, now YOU got us out? By being the first to leave? How noble of you.",
	L"I sure did, $CAUSE$. Man, I don't want to go back THERE again.",
	L"We're still alive, this is what counts.",
	L"The more pressing issue is when will we go back, and finish the job?",
	L"",
	L"",
	L"",

	// OPINIONEVENT_CIVKILLER
	L"What the hell is wrong with you, $CAUSE$? You just killed an innocent!",
	L"",
	L"",
	L"He had a gun, I saw it!",
	L"Oh god. No! What have I done?",
	L"",
	L"",
	L"$VICTIM$ is furious: $CAUSE$ killed a civilian. What do you do?",
	L"Better safe than sorry I say...",
	L"Yeah. The poor sod never had a chance. Damn.",
	L"Don't talk crap. That was an unarmed civilian. We are here to protect these people!",
	L"And you took him down nice and clean, good job!",
	L"This is war. People die all the time... though I'd prefer it if it were less obvious that we are so, ehm, proactive.",
	L"Who cares? Can't make a cake without breaking a few eggs.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SLOWSUSDOWN
	L"Can we get rid of $CAUSE$, please? That slowpoke is holding us back.",
	L"",
	L"",
	L"I wouldn't if you would carry your fair share of the gear, $VICTIM$!",
	L"It's all this stuff, it's so heavy!",
	L"",
	L"",
	L"$VICTIM$ feels $CAUSE$ slows down the team. What do you do?",
	L"We leave nobody behind, not even $CAUSE$!",
	L"We have to move fast, the enemy isn't far behind!",
	L"Everybody carries his gear. How do you expect to fight if you can't even carry your gun?",
	L"Aye, stop complaining. We go through this together or we don't do it at all.",
	L"If you are so annoyed that $CAUSE$ is slow, $VICTIM$, maybe you could lend a hand.",
	L"If you still have enough breath left to complain, $VICTIM$, you should lend $CAUSE_GENDER$ a hand.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_NOSHARINGFOOD
	L"Damn $CAUSE$, $CAUSE_GENDER$ is keeping all that tasty food to $CAUSE_PRONOUN$self...",
	L"",
	L"",
	L"Not my problem if you already ate all your rations.",
	L"Oh $VICTIM$, why didn't you say something then?",
	L"",
	L"",
	L"$VICTIM$ wants $CAUSE$'s food. What do you do?",
	L"We all get the same. If you used up yours already that's your problem.",
	L"If $CAUSE$ shares, I want some too!",
	L"Why do you have so much food anyway? Do I smell extra rations there?.",
	L"Right, everyone got enough back at the base...",
	L"There's enough food left at the base, so no need to fight, ok?",
	L"I wouldn't call that stuff 'tasty', but if you ladies wanna fight about it, fine by me.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_ANNOYINGDISABILITY
	L"Oh, come on, $CAUSE$. It's not a good moment!",
	L"",
	L"",
	L"Just what I need. Good advice. Thanks, $VICTIM$, you are a big help.",
	L"It's my only weakness, I can't help it!",
	L"",
	L"",
	L"$CAUSE$' tick is getting on $VICTIM$'s nerves. What do you do?",
	L"What does it even matter to you? Don't you have something to do?",
	L"Really $CAUSE$. This is a military organization and not a ward.",
	L"Well, we are pros, an you're not, $CAUSE$.",
	L"Don't be such a snob, $VICTIM$.",
	L"Uhm. Is $CAUSE_GENDER$ okay?",
	L"Bla bla blah. Another notable moment of the crazies squad.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_ADDICT
	L"$CAUSE$ is high like a zeppelin! How am I supposed to work with that junkie?",
	L"",
	L"",
	L"Taking the stick out of your butt would be a starter, jeez...",
	L"I've seen things man. And some... stuff!",
	L"",
	L"",
	L"$CAUSE$ took drugs and $VICTIM$ saw it. What do you do?",
	L"Hey, $CAUSE$ needs to ease the stress, ok?",
	L"How unprofessional.",
	L"This is war and not a stoner party. Cut it out, $CAUSE$!",
	L"Hehe. So true.",
	L"I am sure there is a good explanation for this. Am I right, $CAUSE$?",
	L"You can inject yourself with whatever you like, but only after the battle is over.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_THIEF
	L"What the... Hey! $CAUSE$! Give it back, you damn thief!",
	L"",
	L"",
	L"Have you been drinking? What the hell is your problem?",
	L"You noticed? Dammit... 50/50?",
	L"",
	L"",
	L"$VICTIM$ saw $CAUSE$ steal an item. What do you do?",
	L"If you don't have any proof, don't throw around accusations, $VICTIM$.",
	L"So that's the kind of people were stuck with, $VICIM$? I better watch my wallet then.",
	L"HEY! You put that back right now!",
	L"No idea. All of a sudden $VICTIM$ is all drama.",
	L"Perhaps we could get a raise to resolve this... issue?",
	L"Shut up! There is more than enough loot for all of us!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_WORSTCOMMANDEREVER
	L"What a disaster. That was worst command ever, $CAUSE$!",
	L"",
	L"",
	L"I don't have to take that from a grunt like you!",
	L"Dream on, $VICTIM$. I'm worth every penny, and you know it!",
	L"",
	L"",
	L"$CAUSE$ does not trust $VICTIM$'s orders. What do you do?",
	L"Someone had take the lead and $CAUSE$ did it. Did you have a better plan?",
	L"Well, we sure aren't going to win the war at this rate...",
	L"Then take it from me... that wasn't 'commanding' as much as 'handwaving'.",
	L"We have a clear chain of command, and you sure as hell aren't on top, $VICTIM$!",
	L"We will not forget their sacrifice. They died so we could fight on.",
	L"What? This is the business. Screw up and you're dead. They knew the risks. Move on.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_RICHGUY
	L"How can $CAUSE$ earn this much? It's not fair!",
	L"",
	L"",
	L"Dream on, $VICTIM$. I'm worth every penny, and you know it!",
	L"You don't expect me to complain, do you?",
	L"",
	L"",
	L"$CAUSE$ is jealous of $VICTIM$'s paycheck. What do you do?",
	L"Quit whining about the money, you get more than enough yourself!",
	L"In all honesty, $VICTIM$, I think you should adjust your rate!",
	L"Worth it? All you do is pose!",
	L"Relax. At least some of us appreciate your service, $CAUSE$.",
	L"Perhaps $CAUSE_GENDER$ is just good at salary negotiations?",
	L"Everybody gets what the deserve. If you complain, you deserve less.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BETTERGEAR
	L"$CAUSE$ is stocking the good gear for $CAUSE_PRONOUN$self. Not fair!",
	L"",
	L"",
	L"Contrary to you, I actually know how to use them.",
	L"Well, someone has to use it, right? Better luck next time!",
	L"",
	L"",
	L"$CAUSE$ is jealous of $VICTIM$'s equipment. What do you do?",
	L"You're just making up an excuse for your poor marksmanship.",
	L"Yeah, this smells of cronyism to me.",
	L"If by 'use' you mean 'waste a lot bullets', then yeah, you are a pro.",
	L"I'll second that!",
	L"Is that so? Well, I expect superior marksmanship from $CAUSE_GENDER$ from now on.",
	L"Did it ever occur to you that our supplies are limited? We can't ALL get the one good gun.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_YOUMOUNTEDAGUNONMYBREASTS
	L"Do I look like a bipod? Get that thing off me, $CAUSE$!",
	L"",
	L"",
	L"Don't be such a wuss. This is war!",
	L"Oh. Didn't see you for a minute there.",
	L"",
	L"",
	L"$CAUSE$ used $VICTIM$'s chest as a gun rack. How odd. What do you do?",
	L"Don't be such a wuss. This is war!",
	L"Wow. Are you trying to be a jerk, $CAUSE$, or is this normal for you?",
	L"You are and always will be an insensitive jerk, $CAUSE$.",
	L"Sometimes you just have to use your surroundings!",
	L"Ehm... what are you two DOING?",
	L"This is hardly the time to fondle each other, dammit.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BANDAGED
	L"Спасибо, $CAUSE$. Я уж думал я истеку кровью до смерти.",
	L"",
	L"",
	L"Я делаю свою работу, ты можешь вернуться к своей!",
	L"Эй, мы должны присматривать друг за другом, ты же ведь сделашь также в следующий раз, $VICTIM$.",
	L"",
	L"",
	L"$CAUSE$ перевязал $VICTIM$. Что ты делаешь?",
	L"Patched together again? Good, now move!",
	L"Всегда пожалуйста.",
	L"Jeez. Woken up on the wrong foot today?",
	L"Talk about a no-nonsense approach...",
	L"How did you even get wounded? Where did the attack come from?",
	L"You need to be more careful. Now, where did the attack come from?",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DRINKBUDDIES_GOOD
	L"Yeah, $CAUSE$! You get it! How 'bout next round?",
	L"",
	L"",
	L"Pah. I think you've had enough.",
	L"Sure. Bring it on!",
	L"",
	L"",
	L"Drunk $VICTIM$ likes $CAUSE$. What do you do?",
	L"Yeah, enough booze for you today.",
	L"Hoho, party!",
	L"Party pooper!",
	L"You sure like to keep a cool head, $CAUSE$.",
	L"Alright, ladies, party's over, move on!",
	L"Who told you to slack off? You have a job to do!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DRINKBUDDIES_SUPER
	L"$CAUSE$... you're... you are... hic... you're the best!",
	L"",
	L"",
	L"Tehehe. $VICTIM$, you are soooo wasted. You.. hic. You need to restrain yourself. Discipline, you know? Like me!",
	L"Hic... yeah. You too, $VICTIM$. You. hic.. too!",
	L"",
	L"",
	L"Drunk $VICTIM$ dislikes $CAUSE$. What do you do?",
	L"Whatever. We still have a job to do, so this party is over for you, $VICTIM$.",
	L"Heeeey... this is actually kinda nice for a change.",
	L"'I know discipline', said the clueless drunk...",
	L"Yeargh. Dis... gulp! Disciplined like... ehm... us!",
	L"Drink one for me too! But not now, because war.",
	L"You celebrate already ? This in't over yet. Not by a longshot!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DRINKBUDDIES_BAD
	L"Damn, $CAUSE$! You... you got enough... Learn to keep your liquor...",
	L"",
	L"",
	L"Cut it out, $VICTIM$! You clearly don't know when to stop!",
	L"Tehehe. You are RIGHT. Hehe. Always right. Hic!",
	L"",
	L"",
	L"$VICTIM$ is wasted and wants to be $VICTIM$'s buddy. What do you do?",
	L"Relax, it's just a bit of booze.",
	L"Hey keep it down over there, okay?",
	L"You're just as drunk. Beat it, $CAUSE$!",
	L"Leave alcohol to the big ones, okay?",
	L"Later, ok?",
	L"We might've won this battle, but not this godamn war. So move it, soldier!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DRINKBUDDIES_WORSE
	L"What the hell, $CAUSE$! Ugh... I'm never drinking with you again, you sicko.",
	L"",
	L"",
	L"Oh SHUT UP $VICTIM$! You don't know how to beh.. beha... beha? Ehm... You make no sense. At all. Yeah. You're no fun.",
	L"Well you're no fu... fu... fu? Fun! You are not. Hic!. No.",
	L"",
	L"",
	L"$VICTIM$ is wasted and wants to tear $VICTIM$ a new one. What do you do?",
	L"Jeez $VICTIM$, why so uptight all of a sudden? Anything happened?",
	L"This party was cool until $CAUSE$ ruined it!",
	L"$CAUSE$! Shut it! You are a disgrace for this unit. Get out and sober up!",
	L"Word!",
	L"Why don't you two sober up? You're pretty wasted...",
	L"Both of you, shut up! You are a disgrace to this unit!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_AGAINST_US
	L"I knew I couldn't depend on you, $CAUSE$!",
	L"",
	L"",
	L"Depend? It was all your fault!",
	L"Touched a nerve there, didn't I?",
	L"",
	L"",
	L"$VICTIM$ does not like was $CAUSE$ has to say. What do you do?",
	L"So you depend on others to do your job? Then what good are you?!",
	L"Indeed. Way to let $VICTIM$ hang!",
	L"This isn't some schoolyard sympathy crap. It WAS your fault!",
	L"Yeah, what's with the attitude?",
	L"Zip it, both of you!",
	L"Silence, now!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_FOR_US
	L"Ха! Видите? Даже $CAUSE$ поддерживает меня.",
	L"",
	L"",
	L"'Даже'? Что это значит?",
	L"Да. Я польностью с $VICTIM$ здесь.",
	L"",
	L"",
	L"$VICTIM$ likes what $CAUSE$ has to say about $VICTIM_GENDER$. What do you do?",
	L"Don't let it go to your head.",
	L"Hey, don't forget about me!",
	L"Apparently, sometimes even $CAUSE$ is deemed important...",
	L"Do I smell trouble here??",
	L"This is pointless! Discuss that in private, but not now.",
	L"$VICTIM$! $CAUSE$! Less talk, more action, please!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_AGAINST_ENEMY
	L"Yeah, $CAUSE$ saw you do it!",
	L"",
	L"",
	L"No I did not!",
	L"And we won't be quiet about it, no sir!",
	L"",
	L"",
	L"$VICTIM$ likes what $CAUSE$ said about the others. What do you do?",
	L"I don't think so...",
	L"Yeah, totally!",
	L"Hu? You said you did.",
	L"Yeah, don't twist what happened!",
	L"Who cares? We have a job to do.",
	L"If you ladies keep this on, we're going to have a real problem soon.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_FOR_ENEMY
	L"I can't believe you wouldn't agree with me on this, $CAUSE$.",
	L"",
	L"",
	L"It's because you're wrong, that's why.",
	L"I'm surprised too, but that's how it is.",
	L"",
	L"",
	L"$VICTIM$ does not like $CAUSE$ siding with the others. What do you do?",
	L"Ugh. What now...",
	L"Hum. Never saw that coming.",
	L"Oh come down from your high horse, $CAUSE$.",
	L"Yeah, you are wrong, $VICTIM$!",
	L"Can I shut down squad radio, so I don't have to listen to you?",
	L"Yes, very melodramatic. How is any of this relevant?",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SOLVECONFLICT_REASON_GOOD
	L"Yeah... you're right, $CAUSE$. Peace?",
	L"",
	L"",
	L"No. I won't let this go.",
	L"Hmm... ok!",
	L"",
	L"",
	L"$VICTIM$ appreciates $CAUSE$'s conflict resolution. What do you do?",
	L"You're not getting away this easy.",
	L"Glad you guys are not angry anymore.",
	L"Oh come on. $VICTIM$ is letting it go, what's your issue now?",
	L"You tell 'em, $CAUSE$!",
	L"*Sigh* Shake hands or whatever you people do, and then back to business.",
	L"Yes, please. Discuss your petty grievings in full detail, we are all DYING to hear it.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SOLVECONFLICT_REASON_BAD
	L"No. This is a matter of principle.",
	L"",
	L"",
	L"As if you had any to start with.",
	L"Suit yourself then..",
	L"",
	L"",
	L"$VICTIM$ does not like $CAUSE$'s appeal. What do you do?",
	L"You're just asking for trouble, right?",
	L"Guess you're not getting away that easy, $CAUSE$.",
	L"Don't be so flippant.",
	L"Who needs principles anyway?",
	L"Now that this is out of the way, perhaps we could continue?",
	L"Shut up, both of you!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_GOOD
	L"Alright alright. Jeez. I'm over it, okay?",
	L"",
	L"",
	L"Cut it, drama queen.",
	L"As long as it does not happen again.",
	L"",
	L"",
	L"$VICTIM$ was reined in by $CAUSE$. What do you do?",
	L"No reason to be so stiff about it.",
	L"Yeah, keep it down, will ya?",
	L"Pah. You're the one making all the fuss about it...",
	L"Yeah, drop that attitude, $VICTIM$.",
	L"*Sigh* More of this?",
	L"Why am I even listening to you people...",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_BAD
	L"Who do you think you are, $CAUSE$? No, I won't be quiet about this!",
	L"",
	L"",
	L"I'm the one who tells you to shut up! I'm your superior, $VICTIM$!",
	L"The two of us are going to have real problem soon, $VICTIM$.",
	L"",
	L"",
	L"$VICTIM$ did not take $CAUSE$'s words of action well. What do you do?",
	L"Pfft. Don't make a fuss out of it.",
	L"Yeah, you won't boss us around anymore!",
	L"You are certainly nobodies superior!",
	L"Not sure about that, but yep!",
	L"Hey. Hey! Both of you, cut it out! What are you doing?",
	L"If anybody is superior here, then that's me... and I'm ordering you to stand down!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DISEASE_DISGUSTING
	L"Ewww! $CAUSE$ is sick! Get away from me, that looks disgusting!",
	L"",
	L"",
	L"Oh yeah? Back off, before I cough on you!",
	L"It really is. I... *cough* don't feel so well...",
	L"",
	L"",
	L"$VICTIM$ is offended by $CAUSE$ diseases. What do you do?",
	L"Stop behaving like a first grader. We need to get $CAUSE$ to a doctor!",
	L"This does look unhealthy. That better not be contagious!",
	L"Stop it!  We don't need more of whatever it is you have!",
	L"Yeah, there's nothing you can do against this stuff, right?",
	L"The important thing is to get $CAUSE$ to a doctor, and to make sure $CAUSE_GENDER$ doesn't infect anybody else.",
	L"Stop whining! $CAUSE$, get that treated, and the rest of you, back to business!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DISEASE_TREATMENT
	L"Thanks, $CAUSE$. I'm already feeling better.",
	L"",
	L"",
	L"How did you get that in the first place? Did you forget to wash your hands again?",
	L"No problem, we can't have you running around coughing blood, right? Riiight?",
	L"",
	L"",
	L"$VICTIM$ has treated $CAUSE$'s diseases. What do you do?",
	L"Where did you get that stuff from in the first place?",
	L"Great. Are you sure it's fully treated now?",
	L"The important thing is that it's gone now... It is, right?",
	L"I told you people before... this country a dirty place, so beware of what you touch.",
	L"We have to be careful. The army isn't the only thing that wants us dead.",
	L"Great. Everything done? Then let's get back to shooting stuff!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BRUTAL_GOOD
	L"Отлично, $CAUSE$!",
	L"",
	L"",
	L"Whoa. Are you really getting off on that?",
	L"Now THIS is action!",
	L"",
	L"",
	L"$VICTIM$ applauds $CAUSE$'s excessive violence. Does that seem good to you?",
	L"This isn't a video game, kid...",
	L"That was so wicked!",
	L"What are you apologizing for? That was awesome!",
	L"You are sick, $VICTIM$.",
	L"Killing them is enough. No need to make a show out of it.",
	L"Cross us, and this is what you get. Waste the rest of these guys.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BRUTAL_BAD
	L"Черт, $CAUSE$, тебе надо было просто убить их, а не испарить!",
	L"",
	L"",
	L"А чё, если разница?",
	L"Ого. Мощная штучка!",
	L"",
	L"",
	L"$VICTIM$ is appalled by $CAUSE$'s excessive violence. What do you think?",
	L"Don't tell me you've never seen blood before...",
	L"That was a bit excessive...",
	L"Does that mean you aren't even remotely familiar with your gun?",
	L"On the plus side, I doubt this guy is going to complain.",
	L"Don't waste ammunition, $CAUSE$.",
	L"They put up a fight, we put them in a bag. End of story.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_TEACHER
	L"Thanks for giving me a few pointers, $CAUSE$. I've learned a lot from you.",
	L"",
	L"",
	L"About that... you still have a lot to learn, $VICTIM$.",
	L"You're welcome!",
	L"",
	L"",
	L"$CAUSE$ is unimpressed by $VICTIM$'s progress. What do you think?",
	L"At some point you'll have to do on your own though.",
	L"Yeah, you better stick to $CAUSE$.",
	L"Nah, $VICTIM_GENDER$ is doing fine.",
	L"Yes, $VICTIM_GENDER$ still needs training.",
	L"This training is a good preparation, keep up the good work.",
	L"We need everybody at full capacity if we're going to win this campaign, so keep it up.",
	L"",
	L"",
	L"",
	
	// OPINIONEVENT_BESTCOMMANDEREVER
	L"Yeah! Take that, losers! That was a mighty fine strategy, $CAUSE$!",
	L"",
	L"",
	L"I couldn't have done it without you people.",
	L"Well, I do have my moments.",
	L"",
	L"",
	L"$CAUSE$ praises $VICTIM$'s leadership. What's your opinion?",
	L"Well... it's not like $CAUSE_GENDER$ pulled that all by $CAUSE_PRONOUN$self...",
	L"Agreed. $CAUSE$ is a fine squadleader.",
	L"It's alright. You deserve this.",
	L"You did everything right, $CAUSE$. Good work!",
	L"Yeah. We're turning into quite the outfit, aren't we?",
	L"We might have won this battle, but the war is far from over. We'll have to repeat victories like this.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BATTLE_SAVIOUR
	L"Wow, that was close. Thanks, $CAUSE$, I owe you!",
	L"",
	L"",
	L"Don't mention it.",
	L"You'd do the same for me.",
	L"",
	L"",
	L"$CAUSE$ saved $VICTIM$'s life. What's your opinion?",
	L"Pff, that guy was dead anyway.",
	L"How bad is it, $VICTIM$? Can you still fight?",
	L"Then I will. Good job!",
	L"Yeah, I was worried there for a moment.",
	L"Good job, but let's focus on ending this first, okay?",
	L"When you're done hugging, could we go back to the issue at hand? You know, the guys shooting at us?",
	L"",
	L"",
	L"",

	// OPINIONEVENT_FRAGTHIEF
	L"Hey, I had him in my sights! That guy was MINE!",
	L"",
	L"",
	L"What? Are you nuts? We're in the middle of a firefight, and you set up a killcount?",
	L"What. Then where's my target?",
	L"",
	L"",
	L"$VICTIM$ is angry with $CAUSE$ for stealing their kill. What's your take on this?",
	L"This isn't some videogame, you moron. Nobody gives a shit about your godamn killcount.",
	L"Yeah, I hate it when people don't stick to their firing lane.",
	L"Stick to shooting whom you're supposed to, $CAUSE$.",
	L"Jeez. This feels like nineties videogaming. What's next, $VICTIM_GENDER$'ll accuse the enemy of camping?",
	L"You can sort this out later, but right now, everybody make sure we're not missing any angle.",
	L"Just stick to your firing sector, kill 'em all, and there'll be plenty of kills for everybody.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BATTLE_ASSIST
	L"Boom! We sure took care of that guy.",
	L"",
	L"",
	L"Well, it was mostly me, but you did help too.",
	L"Yup. Ready for the next one.",
	L"",
	L"",
	L"$CAUSE$ and $VICTIM$ brought down an enemy. Any comment?",
	L"If you weren't such a bad shot, $CAUSE$ wouldn't have to finish your job.",
	L"It takes several people to take them down? Jeez, what kind of body armour do they have?",
	L"Blah blah, everybody look at me. $VICTIM$ is such a showoff.",
	L"Hehe, they've got no chance.",
	L"Hmm. We might need more firepower if it takes several of us to take those guys down, but good work anyway.",
	L"I guess you get to share what he had. $CAUSE$, you may draw first.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BATTLE_TOOK_PRISONER
	L"Good thinking. We've already won, no need for more senseless slaughter.",
	L"",
	L"",
	L"We'll see about that... I won't hesitate to use force against them if they resist.",
	L"Yeah. Perhaps these guys have some intel we can use.",
	L"",
	L"",
	L"The rest of the enemy team surrendered to $CAUSE$. Now what?",
	L"No offense, but if you cannot handle death, $CAUSE$, perhaps this might not be the best job for you?",
	L"Good job, $CAUSE$. Makes our job hell of a lot easier.",
	L"Hey! Cut the crap. They already surrendered.",
	L"Yeah, you can't trust these guys, they're totally reckless.",
	L"We should move these guys to a prison ASAP. I'm sure they know what the army is up to.",
	L"Pah. I'd have preferred from wasting these losers. They'll just slow us down.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_CIV_ATTACKER
	L"Watch it, $CAUSE$! They are on our side!",
	L"",
	L"",
	L"That's just a scratch, they'll live.",
	L"Oops.",
	L"",
	L"",
	L"$VICTIM$ is angry: $CAUSE$ injured a civilian. What do you do?",
	L"Well, this can happen. As long as they live it's okay.",
	L"This won't look good in the after-action report.",
	L"What are you doing? Watch it, $CAUSE$!",
	L"Don't worry. Happens to me too all the time.",
	L"Maintain fire discipline! After this is over, $VICTIM$ and $CAUSE$ will take care of the wounded.",
	L"If $CAUSE$ had intended it, they would be dead, so no worries here.",
	L"",
	L"",
	L"",
};


STR16	szDynamicDialogueText_DOST_VICTIM_TO_INTERJECTOR_DENY[] =
{
	L"Что?!",
	L"Нет!",
	L"Это ложь!",
	L"Это не правда!",

	L"Ложь, ложь, ложь. Ничего, кроме лжи!",
	L"Обманщик!",
	L"Предатель!",
	L"Следи за языком!",

	L"Не твоего ума дело.",
	L"Кто вообще тебя позвал?",
	L"Никто и не спрашивал мнения тебя, $INTERJECTOR$.",
	L"Держись от меня подальше.",

	L"Почему вы все против меня?",
	L"Почему ты против меня, $INTERJECTOR$?",
	L"Я знал(а) это! $VICTIM$ и $INTERJECTOR$ - соучастники!",
	L"Не слушаю..!",

	L"Ненавижу этот дурдом.",
	L"Ненавижу этот бредлам.",
	L"Отвали!",
	L"Вранье, вранье, вранье...",

	L"Никогда!",
	L"Не правда.",
	L"Это такие враки.",
	L"Я знаю, то что у меня перед глазами.",

	L"Я бес понятия о чем $INTERJECTOR_GENDER$ ведет речь.",
	L"Да не слушайте, $INTERJECTOR_PRONOUN$!",
	L"Неа.",
	L"Ошибаешься.",
};

STR16	szDynamicDialogueText_DOST_VICTIM_TO_INTERJECTOR_AGREE[] =
{
	L"I knew you'd back me, $INTERJECTOR$",
	L"I knew $INTERJECTOR$ would back me.",
	L"What $INTERJECTOR$ said.",
	L"Что $INTERJECTOR_GENDER$ сказал.",

	L"Спасибо, $INTERJECTOR$!",
	L"Еще раз, я прав(а)!",
	L"Видишь, $CAUSE$? Правда на моей стороне!",
	L"Еще раз - $SPEAKER$ говорит дело!",

	L"Ну да.",
	L"Угу.",
	L"Ага",
	L"Да.",

	L"Конечно.",	
	L"Правда.",
	L"Ха!",
	L"Видишь?",

	L"Именно!",
};

STR16	szDynamicDialogueText_DOST_SIDEWITH_VICTIM[] =
{
	L"Так!",
	L"Действительно!",
	L"Именно",
	L"$CAUSE$ всегда это делает.",

	L"$VICTIM$ прав(а)!",
	L"Я тоже хотел(а) это сказать!",
	L"Что сказал(а) $VICTIM$.",
	L"Это наша $CAUSE$!",

	L"Дааа!",
	L"Становится интересно...",
	L"Скажи им, $VICTIM$!",
	L"Соглашаюсь с $VICTIM$...",

	L"Классический $CAUSE$.",
	L"Невозможно сказать лучше.",
	L"Именно это и случилось.",
	L"Согласен!",

	L"Ага.",
	L"Верно.",
	L"В точку.",
};

STR16	szDynamicDialogueText_DOST_SIDEWITH_CAUSE[] =
{
	L"А теперь подождите...",
	L"Скундочку, это не верно...",
	L"Что? Нет.",
	L"Это не то, что произошло.",

	L"Хватит трепаться о $CAUSE$!",
	L"Ой, закнись, $VICTIM$, а!",
	L"Не-не-не, было не так.",
	L"Вау. Почему такая резкая неожиданность, $VICTIM$?",

	L"И я предполагаю такого не было, $VICTIM$?",
	L"Хммммм... нет.",
	L"Отлично, давайте услышим доводы. Позоже у нас нет других вариантов...",
	L"Ты ошибаешься!",

	L"Ты ошибаешься!",
	L"Я и $CAUSE$ никогад не сделали бы такого.",
	L"Хах, не может быть.",
	L"Я так не думаю.",

	L"Зачем про это вспоминать сейчас?",
	L"Серьезно, $VICTIM$? Это необходимо?",
};

STR16	szDynamicDialogueText_DOST_INTERJECTOR_DIALOGUESELECTION_SHORTTEXT[] =
{
	L"Тихо",
	L"Поддерживаю $VICTIM$",
	L"Поддерживаю $CAUSE$",
	L"подумайте",
	L"Заткнитесь",
};

STR16	szDynamicDialogueText_GenderText[] = 
{
	L"он",
	L"она",
	L"его",
	L"её",
};

STR16	szDiseaseText[] =
{
	L" %s%d%% к проворности\n",
	L" %s%d%% к ловкости\n",
	L" %s%d%% к силе\n",
	L" %s%d%% к интеллекту\n",
	L" %s%d%% к уровню\n",

	L" %s%d%% к очкам действия\n",
	L" %s%d к уровню энергии\n",
	L" %s%d%% к силе для переноски вещей\n",
	L" %s%2.2f восстановление здоровья в час\n",
	L" %s%d к нужде во сне\n",
	L" %s%d%% потребление воды\n",
	L" %s%d%% потребление еды\n",

	L"%s: поставлен диагноз - %s!",
	L"%s излечен(а) от %s!",

	L"Обследование",
	L"Лечение населения",
	L"Burial",	// TODO.Translate
	L"Отмена",

	L"\n\n%s (недиагностирована) - %d / %d\n",

	L"High amount of distress can cause a personality split\n",	// TODO.Translate
	L"Contaminated items found in %s' inventory.\n",
	L"Whenever we get this, a new disability is added.\n",	// TODO.Translate

	L"Only one hand can be used.\n",
	L"Only one hand can be used.\nA medical splint was applied to speed up the healing process.\n",
	L"Leg functionality severely limited.\n",
	L"Leg functionality severely limited.\nA medical splint was applied to speed up the healing process.\n",
};

STR16	szSpyText[] =
{
	L"Hide",			// TODO.Translate
	L"Get Intel",
};

STR16	szFoodText[] =
{
	L"\n\n|В|о|д|а: %d%%\n",
	L"\n\n|П|и|щ|а: %d%%\n",

	L"изменение к макс. боевому духу %s%d\n",
	L" %s%d к нужде во сне\n",
	L" %s%d%% к восстановлению дыхания\n",
	L" %s%d%% к эффективности в задании\n",
	L" %s%d%% шанс потерять в параметрах\n",
};

STR16 szIMPGearWebSiteText[] =
{
	// IMP Gear Entrance
	L"Как будет роздано снаряжение?",
	L"Автоматически - случайный выбор, согласно ваших ответов.",
	L"Самостоятельно - вы сами снарядите свой персонаж.",
	L"Автоматически",
	L"Самостоятельно",

	// IMP Gear Entrance
	L"I.M.P. Экипировка",
	L"Additional Cost: %d$ (%d$ prepaid)",	// TODO.Translate
};

STR16 szIMPGearPocketText[] =
{
	L"Выбрать шлем", //L"Select helmet",
	L"Выбрать жилет", //L"Select vest",
	L"Выбрать поножи", //L"Select pants",
	L"Выбрать для лица", //L"Select face gear",
	L"Выбрать для лица", //L"Select face gear",

	L"Выбрать осн. оружие", //L"Select main gun",
	L"Выбрать доп. оружие", //L"Select sidearm",

	L"Выбрать разгр. жилета", //L"Select LBE vest",  
	L"Выбрать разгр. кобуру", //L"Select left LBE holster",
	L"Выбрать разгр. кобуру", //L"Select right LBE holster",
	L"Выбрать разгр. ранец", //L"Select LBE combat pack",
	L"Выбрать разгр. рюкзак", //L"Select LBE backpack",

	L"Select launcher / rifle",
	L"Выбрать оружие ближ. боя", //L"Select melee weapon",

	L"Выбрать доп. предметы", //L"Select additional items", //BIGPOCK1POS
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Select medkit", //MEDPOCK1POS
	L"Select medkit",
	L"Select medkit",
	L"Select medkit",
	L"Select main gun ammo", //SMALLPOCK1POS
	L"Select main gun ammo",
	L"Select main gun ammo",
	L"Select main gun ammo",
	L"Select main gun ammo",
	L"Select launcher / rifle ammo", //SMALLPOCK6POS
	L"Select launcher / rifle ammo",
	L"Select launcher / rifle ammo",
	L"Select launcher / rifle ammo",
	L"Select launcher / rifle ammo",
	L"Select sidearm ammo", //SMALLPOCK11POS
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Выбрать доп. предметы", //L"Select additional items", //SMALLPOCK19POS
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items",
	L"Выбрать доп. предметы", //L"Select additional items", //SMALLPOCK30POS
	L"Left click to select item / Right click to close window",
};

STR16 szMilitiaStrategicMovementText[] =
{
	L"Нельзя отдавать приказы в этом секторе, команды ополчению невозможны.",
	L"Незадействованы",
	L"Группа №",
	L"Далее",

	L"РВП",
	L"Группа %d (новая)",
	L"Группа %d",
	L"Final",

	L"Желающих: %d (+%5.3f)",
};

STR16 szEnemyHeliText[] =
{
	L"Вражеский вертолёт сбит в %s!",
	L"Мы... эм... в настоящее время не контролируем это ПВО, командир...",
	L"ПВО не нуждается сейчас в обслуживании.",
	L"Мы уже отдали приказ заняться ремонтом, исполнение займёт время.",

	L"Нам нехватает ресурсов, чтобы выполнить приказ.",
	L"Отремонтировать оборудование ПВО? Это обойдётся в %d$ и займёт %d часов.",
	L"Вражеский вертолёт подбит в %s.",
	L"%s стреляет %s по вражескому вертолюту в %s.",

	L"База ПВО в %s обстреляла вражеский вертолёт в %s.",
};

STR16 szFortificationText[] =	// TODO.Translate
{
	L"No valid structure selected, nothing added to build plan.",
	L"No gridno found to create items in %s - created items are lost.",
	L"Structures could not be built in %s - people are in the way.",
	L"Structures could not be built in %s - the following items are required:",

	L"No fitting fortifications found for tileset %d: %s",
	L"Tileset %d: %s",
};

STR16	szMilitiaWebSite[] =
{
	// main page
	L"Ополчение",
	L"Силы ополчения",
};

STR16 szIndividualMilitiaBattleReportText[] =
{
	L"Участи в Операции %s",
	L"Пришел в день %d, %d:%02d in %s",
	L"Повышен в день %d, %d:%02d",
	L"KIA, Operation %s",

	L"Мелкое ранение в операции %s",
	L"Сильное ранение в операции %s",
	L"Критическое ранение в операции %s",
	L"Проявил(а) доблесть в операции %s",

	L"Нанят из Цербера в день %d, %d:%02d in %s",
	L"Defected to us on  Day %d, %d:%02d in %s",
	L"Contract terminated on Day %d, %d:%02d",
	L"Defected to us on Day %d, %d:%02d in %s",
};

STR16 szIndividualMilitiaTraitRequirements[] =
{
	L"HP",
	L"AGI",
	L"DEX",
	L"STR",

	L"LDR",
	L"MRK",
	L"MEC",
	L"EXP",

	L"MED",
	L"WIS",
	L"\nMust have regular or elite rank",
	L"\nMust have elite rank",

	L"\n\n|F|u|l|f|i|l|l|e|d |R|e|q|u|i|r|e|m|e|n|t|s",
	L"\n\n|F|a|i|l|e|d |R|e|q|u|i|r|e|m|e|n|t|s",
	L"\n%d %s",
	L"\nBasic version of trait",

	L" (Expert)"
};

STR16	szIdividualMilitiaWebsiteText[] =
{
	L"Operations",
	L"Are you sure you want to release %s from your service?",
	L"HP ratio: %3.0f %% Daily Wage: %3d$ Age: %d years",
	L"Daily Wage: %3d$ Age: %d years",

	L"Kills: %d Assists: %d",
	L"Status: Deceased",
	L"Status: Fired",
	L"Status: Active",

	L"Terminate Contract",
	L"Personal Data",
	L"Service Record",
	L"Inventory",

	L"Militia name",
	L"Sector name",
	L"Weapon",
	L"HP ratio: %3.1f%%%%%%%",

	L"%s is not active in the currently loaded sector.",
	L"%s has been promoted to regular militia",
	L"%s has been promoted to elite militia",
	L"Status: Deserted",	// TODO:Translate
};

STR16	szIdividualMilitiaWebsiteFilterText_Dead[] =
{
	L"All statuses",
	L"Deceased",
	L"Active",
	L"Fired",
};

STR16	szIdividualMilitiaWebsiteFilterText_Rank[] =
{
	L"All ranks",
	L"Green",
	L"Regular",
	L"Elite",
};

STR16	szIdividualMilitiaWebsiteFilterText_Origin[] =
{
	L"All origins",
	L"Rebel",
	L"PMC",
	L"Defector",
};

STR16	szIdividualMilitiaWebsiteFilterText_Sector[] =
{
	L"Все сектора",
};

STR16	szNonProfileMerchantText[] =
{
	L"Торговец настроен враждебно и не будет торговать.",
	L"Торговец не в состоянии вести дела",
	L"Торговец отказывается работать во время боя.",
	L"Торговец отказывается разговаривать.",
};

STR16	szWeatherTypeText[] =
{
	L"обычно",
	L"дождь",
	L"гроза",
	L"песч. буря",

	L"снег",
};

STR16	szSnakeText[] =
{
	L"%s избежал(а) от укуса змеи!",
	L"%s был(а) укушена змеёй!",
};

STR16	szSMilitiaResourceText[] =
{
	L"Превратил %s в ресурсы",
	L"Оружие: ",
	L"Броня: ",
	L"Прочее: ",

	L"Не осталось кандидатов в ополчение!",
	L"Не хватает ресурсов для обучения ополчения!",
};

STR16	szInteractiveActionText[] =
{
	L"%s начал взлом.",
	L"%s получил доступ, но не узнал ничего интересного.",
	L"%s не достаточно навыка для взлома.",
	L"%s прочел документ, но не узнал ничего интересного.",

	L"%s can't make sense out of this.",
	L"%s не может использовать источник воды.",
	L"%s купил %s.",
	L"%s не может купить - нет достаточно денег. Даже неловко как-то.",

	L"%s выпил из источника воды",
	L"Эта штука похоже не работает.",
};

STR16	szLaptopStatText[] =	// TODO.Translate
{
	L"эффективность угроз %d\n",
	L"лидерство %d\n",
	L"модификатор подхода %.2f\n",
	L"модификатор прошлого %.2f\n",

	L"+50 (other) for assertive\n",
	L"-50 (other) for malicious\n",
	L"Хороший человек",
	L"%s сторонится лишнего насилия и будет акатовать только явных врагов.",

	L"Дружески",
	L"Прямо",
	L"Threaten approach",
	L"Recruit approach",

	L"%s learns with normal speed.",	// TODO.Translate
	L"%s does not learn at all.",
	L"%s unlearns his skills.",
	L"%s learns with 3/4 speed.",

	L"%s learns with 1/2 speed.",
	L"%s learns with 1/4 speed.",
};

STR16	szGearTemplateText[] =	// TODO.Translate
{
	L"Enter Template Name",
	L"Not possible during combat.",
	L"Selected mercenary is not in this sector.",
	L"%s is not in that sector.",
	L"%s could not equip %s.",
	L"We cannot attach %s (item %d) as that might damage items.",	// TODO.Translate
};

STR16	szIntelWebsiteText[] =
{
	L"Recon Intelligence Services",
	L"Your need to know base",
	L"Information Requests",
	L"Information Verification",

	L"About us",
	L"You have %d Intel.",
	L"We currently have information on the following items, available in exchange for intel as usual:",
	L"There is currently no other information available.",

	L"%d Intel - %s",
	L"We can provide aerial reconnaissance of a map region. This will last until  %02d:00.",
	L"Buy data - 50 intel",
	L"Buy detailed data - 70 Intel",

	L"Select map region on which you want info on:",

	L"North-west",
	L"North-north-west",
	L"North-north-east",
	L"North-east",

	L"West-north-west",
	L"Center-north-west",
	L"Center-north-east",
	L"East-north-east",

	L"West-south-west",
	L"Center-south-west",
	L"Center-south-east",
	L"East-south-east",

	L"South-west",
	L"South-south-west",
	L"South-south-east",
	L"South-east",

	// about us
	L"On the 'Information Requests' page, you can buy information on various enemy targets for intel.",
	L"This includes information on enemy patrols & garrisons, noteworthy persons of interests, enemy aircraft etc..",
	L"Some of that information may be of temporary nature.",
	L"On the 'Information Verification' page, you can upload data you took of significant intelligence.",

	L"We will verify the data (this process usually takes several hours) and compensate you accordingly.",
	L"Note that you will reveive less intel if outside conditions have rendered the information less useful (e.g. the person in question having died since the data was acquired).",

	// sell info
	L"You can upload the following facts:",
	L"Previous",
	L"Next",
	L"Upload",

	L"You have already received compensation for the following:",
	L"You have nothing to upload.",
};

STR16	szIntelText[] =
{
	L"No more enemies present, %s is no longer in hiding!",
	L"%s has been discovered and goes into hiding for %d hours.",
	L"%s has been discovered, going to sector!",
	L"Enemy general present\n",

	L"Terrorist present\n",
	L"%s on %02d:%02d\n",
	L"No data found",
	L"Data no longer eligible.",

	L"Whereabouts of a high-ranking officer of the royal army.",
	L"Flight plans of an airforce helicopter.",
	L"Coordinates of a recently imprisoned member of your force.",
	L"Location of a high-value fugitive.",

	L"Information on possible bloodcat attacks against settlements.",
	L"Time and place of possible zombie attacks against settlements.",
	L"Information on planned bandit raids.",
};

STR16	szChatTextSpy[] =
{
	L"... so imagine my surprise when suddenly...",
	L"... and did I ever tell you the story of that one time...",
	L"... so, in conclusion, the colonel decided...",
	L"... tell you, they did not see that coming...",

	L"... so, without further consideration...",
	L"... but then SHE said...",
	L"... and, speaking of llamas...",
	L"... there I was, in the middle of the dustbowl, when...",

	L"... and let me tell, those things chafe...",
	L"... you should have seen his face...",
	L"... which wasn't the last of what we saw of them...",
	L"... which reminds me, my grandmother used to say...",

	L"... who, by the way, is a total berk...",
	L"... also, the roots were off by a margin...",
	L"... and I was like, 'Back off, heathen!'...",
	L"... at that point the vicars were in oben rebellion...",

	L"... not that I would've minded, you know, but...",
	L"... if not for that ridiculous hat...",
	L"... besides, it wasn't his favourite leg anyway...",
	L"... even though the ships were still watertight...",

	L"... aside from the fact that giraffes can't do that...",
	L"... totally wasted that fork, mind you...",
	L"... and no bakery in sight. After that...",
	L"... even though regulations are clear in that regard...",
};

STR16	szChatTextEnemy[] =
{
	L"Whoa. I had no idea!",
	L"Really?",
	L"Uhhhh....",
	L"Well... you see, uhh...",

	L"I am not entirely sure that...",
	L"I... well...",
	L"If I could just...",
	L"But...",

	L"I don't mean to intrude, but...",
	L"Really? I had no idea!",
	L"What? All of it?",
	L"No way!",

	L"Haha!",
	L"Whoa, the guys are not going to believe me!",
	L"... yeah, just...",
	L"That's just like the gypsy woman said!",

	L"... yeah, is that why...",
	L"... hehe, talk about refurbishing...",
	L"... yeah, I guess...",
	L"Wait. What?",

	L"... wouldn't have minded seeing that...",
	L"... now that you mention it...",
	L"... but where did all the chalk go...",
	L"... had never even considered that...",
};

STR16	szMilitiaText[] =
{
	L"Train new militia",
	L"Drill militia",
	L"Doctor militia",
	L"Cancel",
};

STR16	szFactoryText[] =	// TODO.Translate
{
	L"%s: Production of %s switched off as loyalty is too low.",
	L"%s: Production of %s switched off due to insufficient funds.",
	L"%s: Production of %s switched off as it requires a merc to staff the facility.",
	L"%s: Production of %s switched off due to required items missing.",
	L"Item to build",

	L"Preproducts",	// 5
	L"h/item",
};

STR16	szTurncoatText[] =
{
	L"%s now secretly works for us!",
	L"%s is not swayed by our offer. Suspicion against us rises...",
	L"Suspicion against us is high. We should stop trying to turn more soldiers to our side and lay low for a while.",
	L"Recruit approach (%d)",
	L"Use seduction (%d)",

	L"Bribe ($%d) (%d)",	// 5
	L"Offer %d intel (%d)",
	L"How to convince the soldier to join your forces?",
	L"Do it",
	L"%d turncoats present",
};

// rftr: better lbe tooltips
STR16 gLbeStatsDesc[14] =
{
	L"MOLLE Доступный объем:",
	L"MOLLE Требуемый объем:",
	L"MOLLE Маленьких слотов:",
	L"MOLLE Средних слотов:",
	L"MOLLE Маленький",
	L"MOLLE Средний",
	L"MOLLE Средний (питьевая система)",
	L"Набедренная платформа",
	L"Жилет",
	L"Ранец",
	L"Рюкзак", // 10
	L"MOLLE Карман",
	L"Совместимые рюкзаки:",
	L"Совместимые ранцы:",
};

STR16 szRebelCommandText[] = // TODO.Translate
{
	L"National Overview",
	L"Regional Overview",
	L"Mission Overview",
	L"Select View:",
	L"Regional (2)",
	L"National (1)",
	L"Mission (3)",
	L"Supplies:",
	L"Incoming Supplies",
	L"Intel:",
	L"/day",
	L"Current Directive",
	L"Improve Directive ($%d)",
	L"Improving the selected directive will cost $%d. Confirm expenditure?",
	L"Insufficient funds.",
	L"New directive will take effect at 00:00.",
	L"Militia Overview",
	L"Green:",
	L"Regular:",
	L"Elite:",
	L"Projected Daily Total:",
	L"Volunteer Pool:",
	L"Resources Available:",
	L"Guns:",
	L"Armour:",
	L"Misc:",
	L"Training Cost:",
	L"Upkeep Cost Per Soldier Per Day:",
	L"Training Speed Bonus:",
	L"Combat Bonuses:",
	L"Physical Stats Bonus:",
	L"Marksmanship Bonus:",
	L"Upgrade Stats ($%d)",
	L"Upgrading militia stats will cost $%d. Confirm expenditure?",
	L"Region:",
	L"Next",
	L"Previous",
	L"Administration Team:",
	L"None",
	L"Active",
	L"Inactive",
	L"Loyalty:",
	L"Maximum Loyalty:",
	L"Deploy Administration Team (%d supplies)",
	L"Reactivate Administration Team (%d supplies)",
	L"It is currently not safe to deploy an administration team to this region. You must establish a foothold by controlling at least one town sector first.",
	L"No regional actions available in Omerta.",
	L"Administration teams can be deployed to other regions once you capture at least one town sector. Once active, they will be able to expand your influence and power in the region. However, they will need supplies to operate and enact policies.",
	L"Be mindful of where you choose to direct supplies. Enacting regional policies will increase supply costs for other policies in the same region and nationally (to a lesser extent).",
	L"Administrative Actions:",
	L"Establish %s",
	L"Improve %s",
	L"Current Tier: %d",
	L"Taking any administrative action in this region will cost %d supplies.",
	L"Dead drop intel gain: %d",
	L"Smuggler supply gain: %d",
	L"A small group of militia from a nearby safehouse have joined the battle!",
	L"[A.R.C. WEBSITE AVAILABLE] With the delivery of food and basic supplies to Omerta, you have convinced the rebels that you're here to make an impact. You have been granted access to the command system they've been working on, which is now available through your laptop.",
	L"It is currently not safe to reactivate the administration team here. Recapture a town sector first.",
	L"Mine raid successful. Stole $%d.",
	L"Insufficient Intel to create turncoats!",
	L"Change Admin Action",
	L"Cancel",
	L"Confirm",
	L"<",
	L">",
	L"Changing this Admin Action will cost $%d and reset its tier. Confirm expenditure?",
	L"Insufficient supplies! Admin Actions have been DISABLED.",
	L"New missions will be available every %d hours.",
	L"Mission preparations in progress.",
	L"Mission duration: %d days",
	L"Chance of success: %d%s",
	L"[REDACTED]",
	L"Name: %s",
	L"Location: %s",
	L"Assignment: %s",
	L"Contract: %d days",
	L"Contract: %d hours",
	L"Contract: ---",
	L"Agent bonus:",
	L"Chance of success +%d%s (%s)",
	L"Deployment range +%d (%s)",
	L"Time +%2.0f%s (%s)",
	L"Vision -%2.0f%s (%s)",
	L"Gear quality -%d (%s)",
	L"Overall stats -%d (%s)",
	L"Max trainers: %d (%s)",
	L"Duration +%d hours (%s)",
	L"Agent not in loyal town",
	L"Agent unavailable",
	L"Agent contract expiring",
	L"Battle in progress",
	L"Start Mission (%d supplies)",
	L"View active mission effects",
	L"View available mission list",
	L"New missions will be available on Day %d at 00:00.",
	L"Active missions:",
	L"%s - Preparing - Ready on Day %d, %02d:%02d",
	L"%s - Active - Expires on Day %d, %02d:%02d",
	L"[%s (%d supplies)]",
	L"%s Send a rebel agent to prepare this mission?",
	L"%s Send %s to prepare this mission? He will return in 24 hours.",
	L"%s Send %s to prepare this mission? She will return in 24 hours.",
	L"Mission \"%s\" is now in effect.",
	L"Preparations for mission \"%s\" failed.",
	L"Mission \"%s\" has expired and is no longer in effect.",
};

STR16 szRebelCommandHelpText[] = // TODO.Translate
{
	L"|S|u|p|p|l|i|e|s\n \nFood, water, medical supplies, weapons, and anything else that\nthe rebels might find useful. Supplies are obtained automatically\nby the rebels.",
	L"|I|n|c|o|m|i|n|g |S|u|p|p|l|i|e|s\n \nEach day, the rebels will gather supplies on their own. As you\ntake over more towns, the amount of supplies they will be\nable to find per day will increase.\n \n+%d (Base income)",
	L"|C|u|r|r|e|n|t |D|i|r|e|c|t|i|v|e\n \nYou can choose how the rebels will prioritise their strategic\nobjectives. New directives will become available as you make\nprogress.",
	L"|A|d|m|i|n|i|s|t|r|a|t|i|o|n |T|e|a|m\n \nOnce deployed, an admin team is responsible for handling the\nday-to-day affairs of the region. This includes supporting\nlocals, creating rebel propaganda, establishing regional\npolicies, and more.",
	L"|L|o|y|a|l|t|y\n \nThe effectiveness of many Administrative Actions depends on\nthe region's loyalty to your cause. It is in your best interest\nto raise loyalty as high as possible.",
	L"|M|a|x|i|m|u|m |L|o|y|a|l|t|y\n \nYou will need to convince the locals to fully trust you. This\ncan be done by creating a supply line to them, showing that\nyou intend to improve their quality of life.",
	L"This Admin Action applies its bonus to town sectors only.", //TODO.Translate
	L"This Admin Action applies its bonus to town sectors, and\nsectors immediately adjacent to them.",
	L"This Admin Action applies its bonus to town sectors, one\nsector away at Tier 1, and up to two sectors away at Tier 2.",
	L"This Admin Action applies its bonus to town sectors, up to\ntwo sectors away at Tier 1, and up to three sectors away at Tier 2.",
};

// follows a specific format:
// x:	"Admin Action Button Text",
// x+1:	"Admin action description text",
STR16 szRebelCommandAdminActionsText[] = // TODO.Translate
{
	L"Supply Line",
	L"Distribute much-needed supplies amongst the local population. Increases maximum regional loyalty.",
	L"Rebel Radio",
	L"Begin broadcasting rebel public radio in the region. The town gains loyalty daily.",
	L"Safehouses",
	L"Construct rebel safehouses in the countryside, far from prying eyes. Bonus militia may join you in combat when operating in this region.",
	L"Supply Disruption",
	L"The rebels will try to disrupt enemy movements in the area by targetting their supplies. Enemies fighting in this region are weaker.",
	L"Scout Patrols",
	L"Start regular scout patrols to monitor hostile activity in the area. Enemy groups will be spotted further from town.",
	L"Dead Drops",
	L"Set up dead drops for rebel scouts and infiltrators to deliver their intel. Provides daily intel.",
	L"Smugglers",
	L"Enlist the aid of smugglers to bring in supplies for the rebels. Provides an unreliable daily supply boost.",
	L"Militia Warehouses",
	L"Construct warehouses in remote areas, allowing the rebels to stockpile weapons for the militia. Provides daily militia resources.",
	L"Regional Taxes",
	L"Collect money from the locals to assist your efforts. Increases daily income, but regional loyalty falls daily.",
	L"Civilian Aid",
	L"Assign some rebels to directly assist and support civilians in the area. Increases daily volunteer pool growth.",
	L"Merc Support",
	L"Set up facilities to directly support your mercs assigned in the town. Increases the effectiveness of merc assignments (doctoring, repairing, militia training, etc).",
	L"Mining Policy",
	L"Import better equipment and work with the town's miners to create more balanced and efficient shift schedules. Increases the town's mine income.",
	L"Pathfinders",
	L"The locals guide your teams through shortcuts in the area. Reduces on-foot travel time in the region.",
	L"Harriers",
	L"The rebels harass nearby enemy groups, significantly increasing their travel time in the region.",
	L"Fortifications",
	L"Set up killzones and defensive positions. Friendly forces are more effective when fighting in this town. Autoresolve only.",
};

// follows a specific format:
// x:	"Directive Name",
// x+1:	"Directive Bonus Description",
// x+2:	"Directive Help Text",
// x+3:	"Directive Improvement Button Description",
STR16 szRebelCommandDirectivesText[] = // TODO.Translate
{
	L"Gather Supplies",
	L"Gain an additional %.0f supplies per day.",
	L"Increase daily supply income by amassing supplies from\nsympathetic locals, and seeking aid from international aid groups.",
	L"Improving this directive will increase the amount of supplies that the rebels can gather daily.",
	L"Support Militia",
	L"Reduce militia daily upkeep costs. Militia daily upkeep cost modifier: %.2f.",
	L"The rebels will help out with logistics about the militia\nyou've trained, reducing the strain on your wallet.",
	L"Improving this directive will reduce the daily upkeep costs of your militia.",
	L"Train Militia",
	L"Reduce militia training costs and increase militia training speed. Militia training cost modifier: %.2f. Militia training speed modifier: %.2f.",
	L"The rebels will assist when you are training militia,\nincreasing the efficiency at which you can train them.",
	L"Improving this directive will further reduce training cost and increase training speed.",
	L"Propaganda Campaign",
	L"Town loyalty rises faster. Loyalty gain modifier: %.2f.",
	L"Your victories and feats will be embellished as news reaches\nthe locals.",
	L"Improving this directive will increase how quickly town loyalty rises.",
	L"Deploy Elites",
	L"%.0f elite militia appear in Omerta each day.",
	L"The rebels release a small number of their highly-trained forces to your command.",
	L"Improving this directive will increase the number of militia\nthat appear each day.",
	L"High Value Target Strikes",
	L"Enemy groups are less likely to have specialised soldiers.",
	L"Surgical strikes will be conducted against enemy groups. Officers,\nmedics, radio operators, and other specialists are targetted.",
	L"Improving this directive will make strikes more successful and effective.",
	L"Spotter Teams",
	L"When in combat, approximate enemy locations are revealed in the overhead map (press INSERT button in tactical).",
	L"A small team will be attached to each of your squads, providing\napproximate locations of enemies when in combat.",
	L"Improving this directive will make the locations of unspotted enemies more precise.",
	L"Raid Mines",
	L"Steal some income from mines not under your control. This directive becomes less useful as you claim mines.",
	L"Conduct smash-and-grab raids on hostile mines. While not always\nsuccessful, the raids that do succeed should provide a\nsmall income bump.",
	L"Improving this directive will increase the maximum value of stolen income.",
	L"Create Turncoats",
	L"Create %.0f turncoats in a random enemy group per day. Consumes %.1f Intel per day.",
	L"Convince enemy soldiers to betray their army and work for you\nthrough a combination of bribery, threats, and blackmail.",
	L"Improving this directive will increase the number of soldiers turned daily.",
	L"Draft Civilians",
	L"Gain %.0f volunteers each day. All towns lose some loyalty each day.",
	L"Draft civilians as recruits for militia. The general population\nprobably won't be too happy about it, though. Effectiveness\nincreases as you capture more towns.",
	L"Improving this directive will increase the number of volunteers gained per day.",
};

STR16 szRebelCommandAgentMissionsText[] =
{
	L"Deep Deployment",
	L"Coordinate efforts to find ways to sneak up on the enemy, but be careful: it's equally possible to put yourself in a disadvantaged deployment area. When attacking enemy forces, the deployment area is much larger.",
	L"Strategic Intel",
	L"Intercept plans and discover where enemies intend to strike. When viewing teams on the strategic map, sectors prioritised by the enemy will be marked in red.",
	L"Improve Local Shops",
	L"Set up ways for merchants across the country to acquire better goods more easily. Shopkeepers will have better than usual inventories.",
	L"Slow Strategic Decisions",
	L"Sow confusion and misdirection at the highest levels of enemy command. The enemy takes longer to make decisions at a strategic level.",
	L"Lower Readiness",
	L"Trick enemy soldiers into letting their guard down. Enemy soldiers have reduced vision range until they are alerted to your mercs' presence.",
	L"Sabotage Equipment",
	L"Disrupt enemy supply chains and prevent the enemy from maintaining their gear properly. Enemy soldiers use equipment that is worse than normal.",
	L"Sabotage Vehicles",
	L"Sabotage vehicle maintenance hubs to reduce their combat effectiveness and readiness. Enemy vehicles encountered have reduced stats.",
	L"Train Militia Anywhere",
	L"Create training areas in the wilderness that can be quickly set up and torn down. Militia can be trained in uncontested sectors outside of town.",
};

STR16 szRobotText[] = // TODO: Translate
{
	L"The robot's installed weapon cannot be changed.",
	L"It is not possible to add attachments to the robot's weapon.",
	L"Installed Weapon",
	L"Reserve Ammo",
	L"Targeting Upgrade",
	L"Chassis Upgrade",
	L"Utility Upgrade",
	L"Storage",
	L"No Bonus",
	L"The laser bonuses of this item are applied to the robot.",
	L"The night- and cave-vision bonuses of this item are applied to the robot.",
	L"This kit degrades instead of the robot's weapon.",
	L"The robot's cleaning kit was depleted!",
	L"Mines adjacent to the robot are automatically flagged.",
	L"Periodic X-Ray scans during combat. No batteries required.",
	L"The robot has activated an x-ray scan!",
	L"The robot can use the radio set.",
	L"The robot's chassis is strengthened, giving it better combat performance.",
	L"The camouflage bonuses of this item are applied to the robot.",
	L"The robot is tougher and takes less damage.",
	L"The robot's extra armour plating was destroyed!",
	L"The robot gains the benefit of the %s skill trait.",
};

#endif //RUSSIAN
