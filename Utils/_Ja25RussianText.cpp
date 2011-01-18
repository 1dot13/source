//#pragma setlocale("RUSSIAN")
#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
	#include "_Ja25RussianText.h"
#else
	#include "Language Defines.h"
	#ifdef RUSSIAN
		#include "text.h"
		#include "Fileman.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_Ja25RussianText_public_symbol(void){;}

#ifdef RUSSIAN

// VERY TRUNCATED FILE COPIED FROM JA2.5 FOR ITS FEATURES FOR JA2 GOLD

STR16	zNewTacticalMessages[]=
{
	//L"Расстояние до цели: %d ед., Освещенность: %d/%d",
	L"Передатчик подключен к вашему ноутбуку.",
	L"Вы не можете нанять %s",
	L"Предложение действует ограниченное время и покрывает стоимость найма на всю миссию, плюс вы так же получите оборудование, перечисленное ниже.",
	L"Наемник %s - наше невероятное суперпредложение 'одна плата за все'. Вы также бесплатно получите его персональную экипировку.",
	L"Гонорар",
	L"В секторе кто-то есть...",
	//L"Дальнобойность оружия: %d ед., Шанс попасть: %d%%",
	L"Показать укрытия",
	L"Линия прицела",
	L"Новые наемники не могут высадиться здесь.",
	L"Так как ваш ноутбук лишился антенны, то вы не сможете нанять новых наемников. Возможно, сейчас вам стоит загрузить одну из сохраненных игр, или начать игру заново!",
	L"%s слышит металлический хруст под телом Джерри. Кажется, это чмо сломало антенну вашего ноутбука.",  //the %s is the name of a merc.
	L"После прочтения записей, оставленных помощником командира Морриса, %s видит, что не все еще потеряно. В записке содержатся координаты городов Арулько для запуска по ним ракет. Кроме того, там также указаны координаты самой ракетной базы.",
	L"Изучив панель управления, %s понимает, что координаты цели можно изменить, и тогда ракета уничтожит эту базу.  %s не собирается умирать, а значит нужно быстрее отсюда выбираться. Похоже, что самый быстрый способ это лифт...",
	L"В начале игры вы выбрали сохранение лишь в \"мирное время\" и теперь не можете записываться во время боя.",
	L"(Нельзя сохраняться во время боя)",
	L"Текущая кампания длиннее 30 символов.",
	L"Текущая кампания не найдена.",
	L"Кампания: По умолчанию ( %S )",
	L"Кампания: %S",
	L"Вы выбрали кампанию %S. Эта кампания является модификацией оригинальной кампании Unfinished Business. Вы уверены, что хотите играть кампанию %S?",
	L"Чтобы воспользоваться редактором, смените кампанию по умолчанию на другую.",
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - New STOMP laptop strings
//these strings match up with the defines in IMP Skill trait.cpp
STR16 gzIMPSkillTraitsText[]=
{
	// made this more elegant
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
	L"Холодное оружие",
	L"Снайпер",
	L"Камуфляж",
	L"Боевые искусства",

	L"Нет",
	L"I.M.P.: Специализация",
	L"(эксперт)",
};

//added another set of skill texts for new major traits
STR16 gzIMPSkillTraitsTextNewMajor[]=
{
	L"Автоматчик",	//Auto Weapons
	L"Гренадёр",	//Heavy Weapons
	L"Стрелок",	//Marksman
	L"Охотник",	//Hunter
	L"Ковбой",	//Gunslinger
	L"Боксёр",	//Hand to Hand
	L"Старшина",	//Deputy
	L"Механик-электронщик",	//Technician
	L"Санитар",	//Paramedic

	L"Нет",
	L"I.M.P.: Основные навыки",	//I.M.P. Major Traits
	// second names
	L"Пулемётчик",	//Machinegunner
	L"Артиллерист",	//Bombardier
	L"Снайпер",	//Sniper
	L"Лесничий",	//Ranger
	L"Пистолетчик",	//Gunfighter
	L"Боевые искусства",	//Martial Arts
	L"Командир",	//Squadleader
	L"Инженер",	//Engineer
	L"Доктор",	//Doctor
};

//added another set of skill texts for new minor traits
STR16 gzIMPSkillTraitsTextNewMinor[]=
{
	L"Ловкач",	//Ambidextrous
	L"Мастер клинка",	//Melee
	L"Мастер по метанию",	//Throwing
	L"Человек ночи",	//Night Ops
	L"Бесшумный убийца",	//Stealthy
	L"Спортсмен",	//Athletics
	L"Культурист",	//Bodybuilding
	L"Подрывник",	//Demolitions
	L"Инструктор",	//Teaching
	L"Разведчик",	//Scouting

	L"Нет",
	L"I.M.P.: Дополнительные навыки",	//I.M.P. Minor Traits
};

//these texts are for help popup windows, describing trait properties
STR16 gzIMPMajorTraitsHelpTextsAutoWeapons[]=
{
	L"+%d%s к шансу поразить из автомата\n",
	L"+%d%s к шансу поразить из пистолет-пулемёта\n",
	L"+%d%s к шансу поразить из ручного пулемёта\n",
	L"-%d%s ОД на стрельбу из ручного пулемёта в режиме очереди или очереди с отсечкой\n",
	L"-%d%s ОД на вскидку ручного пулемёта\n",
	L"Штраф на шанс попадания в автоматическом режиме огня и в режиме очереди понижен на %d%s\n",
	L"Понижен шанс лишних выстрелов при автоматической стрельбе\n",

};
STR16 gzIMPMajorTraitsHelpTextsHeavyWeapons[]=
{
	L"-%d%s ОД на стрельбу из гранатомёта\n",
	L"-%d%s ОД на стрельбу из реактивного гранатомёта\n",
	L"+%d%s к шансу поразить из гранатомёта\n",
	L"+%d%s к шансу поразить из реактивного гранатомёта\n",
	L"-%d%s ОД на залп из миномёта\n",
	L"Понижен штраф на шанс попадания при стрельбе с миномёта на %d%s\n",
	L"+%d%s к урону танкам от поражения из тяжёлого оружия, гранат и взрывчатки\n",
	L"+%d%s к урону иным целям из тяжёлого оружия\n",

};
STR16 gzIMPMajorTraitsHelpTextsSniper[]=
{
	L"+%d%s к шансу поразить из винтовки\n",
	L"+%d%s к шансу попасть из снайперской винтовки\n",
	L"-%d%s эффективной дальности до цели для всего вида оружия\n",	//-%d%s effective range to target with all weapons
	L"+%d%s к бонусу прицеливания на каджый щелчок мыши (особенно к пистолетам)\n",
	L"+%d%s к повреждению от выстрела",	//+%d%s damage on shot
	L" плюс",
	L" с каждым кликом",
	L" после первого",
	L" после второго",
	L" после третьего",
	L" после четвёртого",
	L" после пятого",
	L" после шестого",
	L" после седьмого",
	L"-%d%s ОД на передёргивание затвора у полуавтоматических винтовок\n",
	L"Плюс 1 клик-прицеливания к оружию типа винтовки\n",
	L"Плюс %d клик-прицеливания к оружию типа винтовки\n",

};
STR16 gzIMPMajorTraitsHelpTextsRanger[]=
{
	L"+%d%s к шансу поразить из винтовки\n",
	L"+%d%s к шансу поразить из ружья\n",
	L"-%d%s ОД на перезарядку ружья\n",
	L"+%d%s к скорости передвижения группы между секторами, если идти пешком\n",
	L"+%d%s к скорости передвижения группы между секторами, \nесли передвигаться на транспорте (в особенности на вертолёте)\n",
	L"-%d%s к затрате энергии при переходе между секторами\n",	//меньше тратит сил
	L"-%d%s на штраф погодных условий\n",
	L"+%d%s к эффективности камуфляжа\n",
	L"-%d%s на скорость ухудшения камуфляжа от воды и времени\n",

};
STR16 gzIMPMajorTraitsHelpTextsGunslinger[]=
{
	L"-%d%s ОД необходимые для выстрела из пистолетов и револьверов\n",
	L"+%d%s к эффективной дальнобойности пистолетов и револьверов\n",
	L"+%d%s к шансу попадания из пистолетов и револьверов\n",
	L"+%d%s к шансу попадания из пистолет-пулемётов",
	L" (лишь для одиночного выстрела)",
	L"+%d%s бонуса на один щелчок мыши при прицеливание на пистолеты, пистолет-пулемёты и револьверы\n",
	L"-%d%s ОД необходимых на вскидку пистолета и револьвера\n",
	L"-%d%s ОД необходимых на перезарядку пистолета, пистолет-пулемёта и револьвера\n",
	L"Даёт %d дополнительный щелчок мыши на прицеливание пистолетам, пистолет-пулемётам и револьверам",
	L"Даёт %d дополнительных щелчков мыши на прицеливание пистолетам, пистолет-пулемётам и револьверам\n",

};
STR16 gzIMPMajorTraitsHelpTextsMartialArts[]=
{
	L"-%d%s AP cost of hand to hand attacks(bare hands or with brass knuckles)\n",
	L"+%d%s chance to hit with hand to hand attacks with bare hands\n",
	L"+%d%s chance to hit with hand to hand attacks with brass knuckles\n",
	L"+%d%s damage of hand to hand attacks(bare hands or with brass knuckles)\n",
	L"+%d%s breath damage of hand to hand attacks(bare hands or with brass knuckles)\n",
	L"Enemy knocked out due to your HtH attacks takes slightly longer to recuperate\n",
	L"Enemy knocked out due to your HtH attacks takes longer to recuperate\n",
	L"Enemy knocked out due to your HtH attacks takes much longer to recuperate\n",
	L"Enemy knocked out due to your HtH attacks takes very long to recuperate\n",
	L"Enemy knocked out due to your HtH attacks takes extremely long to recuperate\n",
	L"Enemy knocked out due to your HtH attacks takes long hours to recuperate\n",
	L"Enemy knocked out due to your HtH attacks probably never stand up\n",
	L"Focused (aimed) punch deals +%d%s more damage\n",
	L"Your special spinning kick deals +%d%s more damage\n",
	L"+%d%s change to dodge hand to hand attacks\n",
	L"+%d%s on top chance to dodge HtH attacks with bare hands",
	L" or brass knuckles",
	L" (+%d%s with brass knuckles)",
	L"+%d%s on top chance to dodge HtH attacks with brass knuckles\n",
	L"+%d%s chance to dodge attacks by any melee weapon\n",
	L"-%d%s APs needed to steal weapon from enemy hands\n",
	L"-%d%s APs needed to change stance (stand, crouch, lie down), turn around, climb on/off roof and jump obstacles\n",
	L"-%d%s APs needed to change stance (stand, crouch, lie down)\n",
	L"-%d%s APs needed to turn around\n",
	L"-%d%s APs needed to climb on/off roof and jump obstacles\n",
	L"+%d%s chance to kick doors in\n",
	L"You gain special animations for hand to hand combat\n",

};
STR16 gzIMPMajorTraitsHelpTextsSquadleader[]=
{
	L"+%d%s APs per round of other mercs in vicinity\n",
	L"+%d effective exp level of other mercs in vicinity, which have lesser level than the %s\n",
	L"+%d effective exp level to count as a standby when counting friends' bonus for suppression\n",
	L"+%d%s total suppression tolerance for other mercs in the vicinity and %s himself\n",
	L"+%d morale gain for other mercs in the vicinity\n",
	L"-%d morale loss for other mercs in the vicinity\n",
	L"The vicinity for bonuses is %d tiles",
	L" (%d tiles with extended ears)",
	L"(Max simultaneous bonuses for one soldier is %d)\n",
	L"+%d%s fear resistence of %s\n",
	L"Drawback: %dx morale loss for %s's death for all other mercs\n",

};
STR16 gzIMPMajorTraitsHelpTextsTechnician[]=
{
	L"+%d%s to repairing speed\n",
	L"+%d%s to lockpicking (normal/electronic locks)\n",
	L"+%d%s to disarming electronic traps\n",
	L"+%d%s to attaching special items and combining things\n",
	L"+%d%s to unjamming a gun in combat\n",
	L"Reduce penalty to repair electronic items by %d%s\n",
	L"Increased chance to detect traps and mines (+%d detect level)\n",
	L"+%d%s CtH of robot controlled by the %s\n",
	L"%s trait grants you the ability to repair the robot\n",
	L"Reduced penalty to repair speed of the robot by %d%s\n",
};
STR16 gzIMPMajorTraitsHelpTextsDoctor[]=
{
	L"Has ability to perform surgical intervention by using medical bag on wounded soldier\n",
	L"Surgery instantly returns %d%s of lost health back.",
	L" (This drains the medical bag a lot.)",
	L"Can heal lost stats (from critical hits) by the",
	L" surgery or",
	L" doctor assignment.\n",
	L"+%d%s effectiveness on doctor-patient assignment\n",
	L"+%d%s bandaging speed\n",
	L"+%d%s natural regeneration speed for all soldiers in the same sector",
	L" (max %d of these bonuses per sector stack)",

};
STR16 gzIMPMajorTraitsHelpTextsNone[]=
{
	L"Нет преимуществ",	//No bonuses
};

STR16 gzIMPMinorTraitsHelpTextsAmbidextrous[]=
{
	L"Понижен штраф на стрельбу с двух рук на %d%s\n",
	L"+%d%s к скорости на перезарядку оружия магазином\n",
	L"+%d%s к скорости на дозарядку магазина оружия\n",
	L"-%d%s ОД чтобы поднять предмет\n",
	L"-%d%s ОД на манипуляции с рюкзаком\n",
	L"-%d%s ОД на действия с дверью\n",
	L"-%d%s ОД, необходимых для установки/обезвреживания бомб и мин\n",
	L"-%d%s ОД, необходимых на присоединение навески\n",
};
STR16 gzIMPMinorTraitsHelpTextsMelee[]=
{
	L"-%d%s APs needed to attack by blades\n",
	L"+%d%s chance to hit with blades\n",
	L"+%d%s chance to hit with blunt melee weapons\n",
	L"+%d%s damage with blades\n",
	L"+%d%s damage with blunt melee weapons\n",
	L"Aimed attack with any melee weapon deals +%d%s damage\n",
	L"+%d%s chance to dodge attack by melee blades\n",
	L"+%d%s on top chance to dodge melee blades if holding a blade\n",
	L"+%d%s chance to dodge attack by blunt melee weapons\n",
	L"+%d%s on top chance to dodge blunt melee weapons if holding a blade\n",

};
STR16 gzIMPMinorTraitsHelpTextsThrowing[]=
{
	L"-%d%s basic APs needed to throw blades\n",
	L"+%d%s max range when throwing blades\n",
	L"+%d%s chance to hit when throwing blades\n",
	L"+%d%s chance to hit when throwing blades per aim click\n",
	L"+%d%s damage with throwing blades\n",
	L"+%d%s damage with throwing blades per aim click\n",
	L"+%d%s chance to inflict critical hit with throwing blade if not seen or heard\n",
	L"+%d critical hit with throwing blade multiplier\n",
	L"Adds %d more aim click for throwing blades\n",
	L"Adds %d more aim clicks for throwing blades\n",

};
STR16 gzIMPMinorTraitsHelpTextsNightOps[]=
{
	L"+%d к зрению в темноте\n",
	L"+%d к дальности слуха\n",
	L"+%d дополнительно к слуху в темноте\n",
	L"+%d к вероятности перехвата хода в ночи\n",
	L"-%d к нужде в сне\n",

};
STR16 gzIMPMinorTraitsHelpTextsStealthy[]=
{
	L"-%d%s к ОД, необходимых для тихого передивжения\n",
	L"+%d%s двигаться тихо\n",
	L"+%d%s к скрытности (быть 'невидимым' если вас не обнаружили)\n",
	L"Уменьшение штрафа на видимость в укрытии на %d%s\n",

};
STR16 gzIMPMinorTraitsHelpTextsAthletics[]=
{
	L"-%d%s ОД на движения (бег, шаг, шаг вприсядку, переползание, плавание и т.д.)\n",
	L"-%d%s на затраты энергии при движении, вскарабкивание на крышу, прыжки через препядствия, плавание и т.д.\n",
};
STR16 gzIMPMinorTraitsHelpTextsBodybuilding[]=
{
	L"Имеет %d%s устойчивости к повреждениям\n",
	L"+%d%s к силе на переноску снаряжения\n",
	L"Уменьшена потеря сил при пропущенных ударах в ближнем бою на %d%s\n",
	L"Повышен урон, необходимый чтобы свалить с ног при ранении в ногу на %d%s\n",

};
STR16 gzIMPMinorTraitsHelpTextsDemolitions[]=
{
	L"-%d%s APs needed to throw grenades\n",
	L"+%d%s max range when throwing grenades\n",
	L"+%d%s chance to hit when throwing grenades\n",
	L"+%d%s damage of set bombs and mines\n",
	L"+%d%s to attaching detonators check\n",
	L"+%d%s to planting/removing bombs check\n",
	L"Decreases chance enemy will detect your bombs and mines (+%d bomb level)\n",
	L"Increased chance shaped charge will open the doors (damage multiplied by %d)\n",

};
STR16 gzIMPMinorTraitsHelpTextsTeaching[]=
{
	L"+%d%s bonus to militia training speed\n",
	L"+%d%s bonus to effective leadership for determining militia training\n",
	L"+%d%s bonus to teaching other mercs\n",
	L"Skill value counts to be +%d higher for being able to teach this skill to other mercs\n",
	L"+%d%s bonus to train stats through self-practising assignment\n",

};
STR16 gzIMPMinorTraitsHelpTextsScouting[]=
{
	L"+%d to effective sight range with scopes on weapons\n",
	L"+%d to effective sight range with binoculars (and scopes separated from weapons)\n",
	L"-%d tunnel vision with binoculars (and scopes separated from weapons)\n",
	L"If in sector, adjacent sectors will show exact number of enemies\n",
	L"If in sector, adjacent sectors will show presence of enemies, if any\n",
	L"Prevents enemy ambushes on your squad\n",
	L"Prevents bloodcat ambushes on your squad\n",

};
STR16 gzIMPMinorTraitsHelpTextsNone[]=
{
	L"Нет преимуществ",	//No bonuses
};

STR16 gzIMPOldSkillTraitsHelpTexts[]=
{
	L"+%d%s bonus to lockpicking\n",
	L"+%d%s hand to hand chance to hit\n",
	L"+%d%s hand to hand damage\n",
	L"+%d%s chance to dodge hand to hand attacks\n",
	L"Eliminates the penalty to repair and handle\nelectronic things (locks, traps, rem. detonators, robot, etc.)\n",
	L"+%d to effective sight range in the dark\n",
	L"+%d to general effective hearing range\n",
	L"+%d extra hearing range in the dark\n",
	L"+%d to interrupts modifier in the dark\n",
	L"-%d need to sleep\n",
	L"+%d%s max range when throwing anything\n",
	L"+%d%s chance to hit when throwing anything\n",
	L"+%d%s chance to instantly kill by throwing knife if not seen or heard\n",
	L"+%d%s bonus to militia training and other mercs instructing speed\n",
	L"+%d%s effective leadership for militia training calculations\n",
	L"+%d%s chance to hit with rocket/greande launchers and mortar\n",
	L"Auto fire/burst chance to hit penalty is divided by %d\n",
	L"Reduced chance for shooting unwanted bullets on autofire\n",
	L"+%d%s chance to move quietly\n",
	L"+%d%s stealth (being 'invisible' if unnoticed)\n",
	L"Eliminates the CtH penalty when firing two weapons at once\n",
	L"+%d%s chance to hit with melee blades\n",
	L"+%d%s chance to dodge attacks by melee blades if having blade in hands\n",
	L"+%d%s chance to dodge attacks by melee blades if having anything else in hands\n",
	L"+%d%s chance to dodge hand to hand attacks if having blade in hands\n",
	L"-%d%s effective range to target with all weapons\n",
	L"+%d%s aiming bonus per aim click\n",
	L"Provides permanent camouflage\n",
	L"+%d%s hand to hand chance to hit\n",
	L"+%d%s hand to hand damage\n",
	L"+%d%s chance to dodge hand to hand attacks if having empty hands\n",
	L"+%d%s chance to dodge hand to hand attacks if not having empty hands\n",
	L"+%d%s chance to dodge attacks by melee blades\n",
	L"Can perform spinning kick attack on weakened enemies to deal double damage\n",
	L"You gain special animations for hand to hand combat\n",
	L"Нет преимуществ",	//No bonuses
};

STR16 gzIMPNewCharacterTraitsHelpTexts[]=
{
//I.M.P. Character Traits help text
//Neutral
	L"плюсы: Нет преимуществ.\n \nминусы: Без изъян.",
//Sociable
	L"плюсы: Лучше работает в команде.\n \nминусы: Боевой дух не растёт, когда наёмник работает один.",
//Loner
	L"плюсы: Лучше работает в одиночестве.\n \nминусы: Боевой дух не растёт в присутствии других бойцов.",
//Optimist
	L"плюсы: Боевой дух растет быстрее, а снижается медленнее обычного.\n \nминусы: Шанс обнаружить мины и ловушки ниже среднего.",
//Assertive
	L"плюсы: Лучше ладит с людьми и тренирует ополчение.\n \nминусы: Действия других бойцов не влияют на его боевой дух.",
//Intellectual
	L"плюсы: Немного быстрее обучается.\n \nминусы: Обладает меньшим сопротивлением страху и подавлению.",
//Primitive
	L"плюсы: Устаёт медленнее других, если не работает как врач, ремонтник, тренер или ученик.\n \nминусы: Его мудрость, лидерство, взрывное дело, механика и медицина растут медленнее обычного.",
//Aggressive
	L"плюсы: Имеет бонус к стрельбе очередями и урону в рукопашной. \nПри убийстве врага боевой дух растёт больше, чем у других.\n \nминусы: Хуже исполняет обязанности, для которых требуется терпение: \nремонт, вскрытие замков, снятие ловушек, лечение, тренировка ополчения.",
//Phlegmatic
	L"плюсы: Лучше исполняет обязанности, требующие терпения: \nремонт, вскрытие замков, снятие ловушек, лечение, тренировка ополчения.\n \nминусы: Имеет меньший шанс перехватить ход врага.",
//Dauntless
	L"плюсы: Имеет повышенное сопротивление подавлению и страху. \nБоевой дух при ранениях и гибели товарищей понижается медленнее, чем у других.\n \nминусы: Может быть с большей вероятностью поражен во время движения.",
//Pacifist
	L"плюсы: Боевой дух повышается при выполнении небоевых заданий (кроме тренировки ополчения).\n \nминусы: Убийство врагов не повышает боевой дух.",
//Malicious
	L"плюсы: Имеет больший шанс нанести болезненные раны и травмы, приводящие к ухудшению параметров.\n \nминусы: Имеет проблемы в общении и быстро теряет боевой дух, если не сражается.",
//Show-off
	L"плюсы: Лучше работает в компании представителей противоположного пола.\n \nминусы: Боевой дух бойцов того же пола в его присутствии растёт медленнее.",
};

STR16 gzIMPDisabilitiesHelpTexts[]=
{
	L"Никакого влияния.",
	L"Уменьшается работоспособность и возникают проблемы с дыханием \nесли находится в пустынной или тропической местности.",
	L"Может впасть в панику если оставить одного в определённых ситуациях.",
	L"Понижается работоспособноть в замкнутых помещениях, подземельях.",
	L"При попытке плыть может с лёгкостью утонуть.",
	L"При виде больших насекомых может впасть в крайности и наворотить дел... \nНахождение в тропических лесах так же понижает его работоспособность.",
	L"Иногда забывает приказы, из-за чего теряет \nнекоторое количество Очков Действия во время боя.",
	L"Иногда бывают приступы помутнения рассудка. \nВ такие моменты он расстреливает весь магазин до последней пули. \nПадает духом, если его оружие этого не позволяет.",
};


STR16 gzIMPProfileCostText[]=
{
	L"Составление вашей харрактеристики стоит %d$. Подтвердить оплату? ",
};

STR16 zGioNewTraitsImpossibleText[]=
{
	L"Нельзя выбрать новые умения IMP персонажа с отключенным PROFEX. Проверьте значение файла настроек JA2_Options.ini, ключ: READ_PROFILE_DATA_FROM_XML.",	//You cannot choose the New Trait System with PROFEX utility deactivated. Check your JA2_Options.ini for entry: READ_PROFILE_DATA_FROM_XML.
};

//@@@:  New string as of March 3, 2000.
STR16	gzIronManModeWarningText[]=
{
	L"Ваш выбор позволит сохраняться лишь в \"мирное время\". Проходить игру станет гораздо сложнее, так как сохраняться вы сможете только между боями. После старта игры изменить эту настройку нельзя. Вы уверены, что готовы расстаться с возможностью сохраняться в бою?",
};

STR16 gzDisplayCoverText[]=
{
	L"Местность: %d/100 %s, Освещённость: %d/100",
	L"Дальнобойность оружия: %d/%d ед., шанс попасть: %d/100",
	L"Отключено выделение видимых зон наёмника и врага",
	L"Видимые зоны наёмнка",
	L"Опасные зоны для наёмника",
	L"Джунгли", //Wood //wanted to use jungle , but wood is shorter in german too (dschungel vs wald)
	L"Город",
	L"Пустыня",
	L"Снег",	//NOT USED!!!
	L"Лес и пустыня",
	L"" // yes empty for now
};


#endif
