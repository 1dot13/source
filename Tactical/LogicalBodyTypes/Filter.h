#pragma once
#ifndef _LBT_FILTER__H_
#define _LBT_FILTER__H_

#include <string>
#include <map>
#include "DEBUG.H"
#include "Soldier Control.h"
#include "soldier profile type.h"
#include "Soldier Profile.h"
#include "Weapons.h"
#include "LogicalBodyTypes/EnumeratorDB.h"

namespace LogicalBodyTypes {

class Filter {

public:
	enum ConditionTypes
	{
		// lowest two bytes are reserved for enumerators
		REQ_HELMETPOS,
		REQ_VESTPOS,
		REQ_LEGPOS,
		REQ_HEAD1POS,
		REQ_HEAD2POS,
		REQ_HANDPOS,
		REQ_SECONDHANDPOS,
		REQ_VESTPOCKPOS,
		REQ_LTHIGHPOCKPOS,
		REQ_RTHIGHPOCKPOS,
		REQ_CPACKPOCKPOS,
		REQ_BPACKPOCKPOS,
		REQ_GUNSLINGPOCKPOS,
		REQ_KNIFEPOCKPOS,
		NUM_REQTYPESINV,
		REQ_SEX,
		REQ_MERC_TYPE,
		REQ_NAME,
		REQ_SOLDIER_CLASS,
		REQ_TEAM,
		REQ_CIVILIANGROUP,
		REQ_CAMO,
		REQ_URBANCAMO,
		REQ_DESERTCAMO,
		REQ_SNOWCAMO,
		REQ_BODYTYPE,
		REQ_EXPLEVEL,
		REQ_STRENGTH,
		REQ_LEADERSHIP,
		REQ_WISDOM,
		REQ_SKILLTRAIT1,
		REQ_SKILLTRAIT2,
		REQ_FACEINDEX,
		REQ_PROFILENAME,
		REQ_NICKNAME,
		REQ_WEAPON_IN_HAND,
		REQ_WEAPON_CLASS,
		REQ_LEFT_WEAPON_CLASS,
		REQ_WEAPON_TYPE,
		REQ_LEFT_WEAPON_TYPE,
		REQ_CALIBRE,
		REQ_WEAPON_TWOHANDED,
		REQ_LEFT_WEAPON_TWOHANDED,
		REQ_VEST_AMOR_PROTECTION,
		REQ_VEST_AMOR_COVERAGE,
		REQ_HELMET_AMOR_PROTECTION,
		REQ_HELMET_AMOR_COVERAGE,
		REQ_WEARING_BACKPACK,
		REQ_HELMETPOSATTACHMENT0,
		REQ_HELMETPOSATTACHMENT1,
		REQ_HELMETPOSATTACHMENT2,
		REQ_HELMETPOSATTACHMENT3,
		REQ_LEGPOSATTACHMENT0,
		REQ_LEGPOSATTACHMENT1,
		REQ_LEGPOSATTACHMENT2,
		REQ_LEGPOSATTACHMENT3,
		NUM_REQTYPES,
		// 3rd byte is for operator flags
		_REQ_BTWN = 0x20000,
		_REQ_NOT = 0x40000,
		_REQ_EQ = 0x80000,
		_REQ_LT = 0x100000,
		_REQ_GT = 0x200000,
		_REQ_AND = 0x400000,
		_REQ_OR = 0x800000,
		_REQ_IN = 0x1000000,
		// 4th byte is for type flags
		_TYPE_STRING = 0x2000000,
		_TYPE_INTEGER = 0x4000000,
		_TYPE_ENUM = 0x8000000,
		_TYPE_LIST = 0x10000000,
		_TYPE_PAIR = 0x20000000,
		_TYPE_FILTER = 0x40000000,
		// TODO rename enumerator _ALL_FLAGS or somesuch
		_UNSET_FLAGS = _TYPE_FILTER | _TYPE_PAIR | _TYPE_LIST | _TYPE_ENUM | _TYPE_STRING | _TYPE_INTEGER | _REQ_IN | _REQ_OR | _REQ_AND | _REQ_BTWN | _REQ_NOT | _REQ_EQ | _REQ_LT | _REQ_GT
	};
	typedef std::pair <INT32, INT32> NumberPair;
	typedef std::list <std::wstring> StringList;
	typedef std::vector <INT32> NumberList;
	union criterionVariant {
		INT32 number;
		std::wstring* string;
		NumberPair* numberPair;
		StringList* stringList;
		NumberList* numberList;
		Filter* filter;
	} typedef criterionVariant;
	typedef std::multimap <UINT32, criterionVariant> CriteriaMap;

public:
	Filter(void);
	~Filter(void);
	void AddCriterion(UINT32 type, std::wstring& value);
	void AddCriterion(UINT32 type, STR16 value);
	void AddCriterion(UINT32 type, StringList& list);
	void AddCriterion(UINT32 type, INT32 value);
	void AddCriterion(UINT32 type, INT32 value1, INT32 value2);
	void AddCriterion(UINT32 type, NumberList& list);
	void AddCriterion(UINT32 type, Filter* filter);
	void Dump(std::wstringstream& stream);
	bool Match(SOLDIERTYPE* pSoldier);

private:
	CriteriaMap criteria;

};

}

#endif
