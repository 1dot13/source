#include "Filter.h"

namespace LogicalBodyTypes {

Filter::Filter(void) {
}

Filter::~Filter(void) {
	for (CriteriaMap::iterator i = criteria.begin(); i != criteria.end(); i++) {
		if (i->first & _TYPE_STRING) {
			if (i->first & _TYPE_LIST) delete i->second.stringList;
			else delete i->second.string;
		}
		else if (i->first & _TYPE_INTEGER) {
			if (i->first & _TYPE_PAIR) delete i->second.numberPair;
			else if (i->first & _TYPE_LIST) delete i->second.numberList;
		}
	}
}

void Filter::AddCriterion(UINT32 type, std::wstring& value) {
	type &= ~_TYPE_INTEGER;
	type |= _TYPE_STRING;
	std::wstring* str = new std::wstring(value);
	criterionVariant val;
	val.string = str;
	criteria.insert(CriteriaMap::value_type(type, val));
}

void Filter::AddCriterion(UINT32 type, STR16 value) {
	type &= ~_TYPE_INTEGER;
	type |= _TYPE_STRING;
	std::wstring* str = new std::wstring(value);
	criterionVariant val;
	val.string = str;
	criteria.insert(CriteriaMap::value_type(type, val));
}

void Filter::AddCriterion(UINT32 type, StringList& list) {
	type &= ~_TYPE_INTEGER;
	type |= _TYPE_STRING;
	criterionVariant val;
	val.stringList = new StringList(list); // I don't trust copy constructors, but oh well...
	criteria.insert(CriteriaMap::value_type(type, val));
}


void Filter::AddCriterion(UINT32 type, INT32 value) {
	type &= ~_TYPE_STRING;
	type |= _TYPE_INTEGER;
	criterionVariant val;
	val.number = value;
	criteria.insert(CriteriaMap::value_type(type, val));
}

void Filter::AddCriterion(UINT32 type, INT32 value1, INT32 value2) {
	type &= ~_TYPE_STRING;
	type |= _TYPE_INTEGER;
	criterionVariant val;
	val.numberPair = new NumberPair(value1, value2);
	criteria.insert(CriteriaMap::value_type(type, val));
}

void Filter::AddCriterion(UINT32 type, NumberList& list) {
	type &= ~_TYPE_STRING;
	type |= _TYPE_INTEGER;
	criterionVariant val;
	val.numberList = new NumberList(list); // I don't trust copy constructors, but oh well...
	criteria.insert(CriteriaMap::value_type(type, val));
}

void Filter::AddCriterion(UINT32 type, Filter* filter) {
	type |= _TYPE_FILTER;
	criterionVariant val;
	val.filter = filter;
	criteria.insert(CriteriaMap::value_type(type, val));
}

void Filter::Dump(std::wstringstream& stream) {
	stream << L"<-------- Filter" << std::endl;
	for (CriteriaMap::iterator ii = criteria.begin(); ii != criteria.end(); ii++) {
		UINT32 flags = ii->first & _UNSET_FLAGS;
		UINT32 cType = ii->first & ~_UNSET_FLAGS;
		std::string op = "";
		if (flags & _REQ_OR) op += "OR";
		if (flags & _REQ_AND) op += "AND";
		std::string cmpop = "";
		if ((flags & _REQ_NOT) && (flags & _REQ_EQ)) cmpop = "!=";
		if (((flags & _REQ_NOT) != _REQ_NOT) && (flags & _REQ_EQ)) cmpop = "==";
		if ((flags & _REQ_NOT) && (flags & _REQ_GT)) cmpop = ">";
		if (((flags & _REQ_NOT) != _REQ_NOT) && (flags & _REQ_GT)) cmpop = "<=";
		if ((flags & _REQ_NOT) && (flags & _REQ_LT)) cmpop = "<";
		if (((flags & _REQ_NOT) != _REQ_NOT) && (flags & _REQ_LT)) cmpop = ">=";
		if (flags & Filter::_TYPE_STRING) {
			std::string enumStr;
			EnumeratorDB::Instance().GetEnumeratorStr("StringFilterCriterionTypes", cType, enumStr);
			stream << L"criterion: " << op << L" str " << enumStr << L" " << cmpop << L" " << *ii->second.string << std::endl;
		}
		if (flags & Filter::_TYPE_INTEGER) {
			std::string enumStr;
			// first try to find the enumerator in the integer criterion type list then in the enum list
			bool foundEnumerator = EnumeratorDB::Instance().GetEnumeratorStr("IntegerFilterCriterionTypes", cType, enumStr);
			if (!foundEnumerator) EnumeratorDB::Instance().GetEnumeratorStr("EnumFilterCriterionTypes", cType, enumStr);
			stream << L"criterion: " << op << L" str " << enumStr << L" " << cmpop << L" " << ii->second.number << std::endl;
		}
		// TODO: drop method or update to match the additional types that have been added.
	}
	stream << L"-------->" << std::endl;
}

// quite an ugly implementation. Should be re-done
bool Filter::Match(SOLDIERTYPE* pSoldier) {
	bool orReq = false;
	bool orRes = false;
	UINT32 operation = _REQ_AND;
	// TODO: make sure container is a BT, since we need no random access anyways!
	for (CriteriaMap::iterator ii = criteria.begin(); ii != criteria.end(); ii++) {
		UINT32 r = ii->first;
		if ((r & _REQ_AND) == _REQ_AND) {
			operation = _REQ_AND;
		}
		else if ((r & _REQ_OR) == _REQ_OR) {
			if (orRes) continue;
			orReq = true;
			operation = _REQ_OR;
		}
		char sgn;
		if (r & _TYPE_INTEGER) {
			INT32 cmp_val;
			UINT32 q = r & ~_UNSET_FLAGS;
			switch (q) {
				case REQ_BODYTYPE:
					cmp_val = pSoldier->ubBodyType;
					break;
				case REQ_TEAM:
					cmp_val = pSoldier->bTeam;
					break;
				case REQ_SOLDIER_CLASS:
					cmp_val = pSoldier->ubSoldierClass;
					break;
				case REQ_CIVILIANGROUP:
					cmp_val = pSoldier->ubCivilianGroup;
					break;
				case REQ_CAMO:
					cmp_val = pSoldier->bCamo;
					break;
				case REQ_URBANCAMO:
					cmp_val = pSoldier->urbanCamo;
					break;
				case REQ_DESERTCAMO:
					cmp_val = pSoldier->desertCamo;
					break;
				case REQ_SNOWCAMO:
					cmp_val = pSoldier->snowCamo;
					break;
				case REQ_EXPLEVEL:
					cmp_val = pSoldier->stats.bExpLevel;
					break;
				case REQ_STRENGTH:
					cmp_val = pSoldier->stats.bStrength;
					break;
				case REQ_LEADERSHIP:
					cmp_val = pSoldier->stats.bLeadership;
					break;
				case REQ_WISDOM:
					cmp_val = pSoldier->stats.bWisdom;
					break;
/*				case REQ_SKILLTRAIT1:
					cmp_val = pSoldier->stats.ubSkillTrait1;
					break;
				case REQ_SKILLTRAIT2:
					cmp_val = pSoldier->stats.ubSkillTrait2;
					break;*/
				case REQ_FACEINDEX:
					if (pSoldier->ubProfile == NO_PROFILE
						|| pSoldier->ubProfile < 1
						|| pSoldier->ubProfile > NUM_PROFILES - 2) return false;
					cmp_val = gMercProfiles[pSoldier->ubProfile].ubFaceIndex;
					break;
				case REQ_WEAPON_IN_HAND:
					cmp_val = WeaponInHand(pSoldier);
					break;
				case REQ_MERC_TYPE:
					cmp_val = pSoldier->ubWhatKindOfMercAmI;
					break;
				case REQ_WEAPON_CLASS:
					cmp_val = Weapon[pSoldier->inv[HANDPOS].usItem].ubWeaponClass;
					break;
				case REQ_WEAPON_TYPE:
					cmp_val = Weapon[pSoldier->inv[HANDPOS].usItem].ubWeaponType;
					break;
				case REQ_LEFT_WEAPON_TYPE:
					cmp_val = Weapon[pSoldier->inv[SECONDHANDPOS].usItem].ubWeaponType;
					break;
				case REQ_CALIBRE:
					cmp_val = Weapon[pSoldier->inv[HANDPOS].usItem].ubCalibre;
					break;
				case REQ_VEST_AMOR_PROTECTION:
					cmp_val = Armour[Item[pSoldier->inv[VESTPOS].usItem].ubClassIndex].ubProtection;
					break;
				case REQ_VEST_AMOR_COVERAGE:
					cmp_val = Armour[Item[pSoldier->inv[VESTPOS].usItem].ubClassIndex].ubCoverage;
					break;
				case REQ_HELMET_AMOR_PROTECTION:
					cmp_val = Armour[Item[pSoldier->inv[HELMETPOS].usItem].ubClassIndex].ubProtection;
					break;
				case REQ_HELMET_AMOR_COVERAGE:
					cmp_val = Armour[Item[pSoldier->inv[HELMETPOS].usItem].ubClassIndex].ubCoverage;
					break;
				case REQ_WEARING_BACKPACK:
					cmp_val = pSoldier->inv[BPACKPOCKPOS].exists();
					break;
				default:
					if (q < NUM_REQTYPESINV) {
						cmp_val = pSoldier->inv[q].usItem;
					} else {
						// not implemented. We should log this somehow(must be cached).
						return false;
					}
			}
			if (r & _TYPE_LIST) {
				NumberList* nl = ii->second.numberList;
				sgn = -1; // -1 or 1 doesn't matter
				for (NumberList::iterator i = nl->begin(); i != nl->end(); i++) {
					if (*i == cmp_val) {
						sgn = 0;
						break;
					}
				}
			} else if (r & _TYPE_PAIR) {
				NumberPair* nl = ii->second.numberPair;
				// TODO: resolve compiler warning about usigned/signed and the comparison ops?!?
				sgn = (nl->second < cmp_val) ? -1 : (nl->first > cmp_val) ? 1 : 0;
			} else {
				sgn = (ii->second.number < cmp_val) ? -1 : (ii->second.number > cmp_val) ? 1 : 0;
			}
		} else if (r & _TYPE_STRING) {
			// string comparison is quite costly at the moment. should be avoided or reimplemented.
			// would probably be much more sensible just to loop and make a wchar for wchar check.
			// I wanted to use the signum compare as with the integer values. But since that 
			// is more like a code gimmick than an actual useful feature, probably better to
			// drop the check for > < for strings alltogether.
			std::wstring cmp_val;
			CHAR16 wStr[31];
			switch (r & ~_UNSET_FLAGS) {
				case REQ_NAME:
					wcscpy(wStr, pSoldier->name);
					cmp_val = wStr;
					break;
				case REQ_PROFILENAME:
					if (pSoldier->ubProfile == NO_PROFILE
						|| pSoldier->ubProfile < 1
						|| pSoldier->ubProfile > NUM_PROFILES - 2) return false;
					wcscpy(wStr, gMercProfiles[pSoldier->ubProfile].zName);
					cmp_val = wStr;
					break;
				case REQ_NICKNAME:
					if (pSoldier->ubProfile == NO_PROFILE
						|| pSoldier->ubProfile < 1
						|| pSoldier->ubProfile > NUM_PROFILES - 2) return false;
					wcscpy(wStr, gMercProfiles[pSoldier->ubProfile].zNickname);
					cmp_val = wStr;
					break;
				default:
					// not implemented. We should log this somehow(must be cached).
					return false;
			}
			if (r & _TYPE_LIST) {
				StringList* sl = ii->second.stringList;
				sgn = -1; // -1 or 1 doesn't matter
				for (StringList::iterator i = sl->begin(); i != sl->end(); i++) {
					if (*i == cmp_val) {
						sgn = 0;
						break;
					}
				}
			}
			else {
				std::wstring* sp = ii->second.string;
				sgn = cmp_val.compare(*sp);
			}
		} else if (r & _TYPE_FILTER) {
			if (ii->second.filter == NULL) {
				// TODO: log or exception
				return false;
			}
			sgn = (ii->second.filter->Match(pSoldier)) ? 0 : -1; // -1 or 1 doesn't matter
		} else {
			// TODO: log or exception
			return false;
		}
		bool res = ((r & _REQ_NOT) == _REQ_NOT) ^ (
				   (((r & _REQ_EQ) == _REQ_EQ) && sgn == 0)
				|| (((r & _REQ_IN) == _REQ_IN) && sgn == 0)
				|| (((r & _REQ_BTWN) == _REQ_BTWN) && sgn == 0)
				|| (((r & _REQ_GT) == _REQ_GT) && sgn == 1)
				|| (((r & _REQ_LT) == _REQ_LT) && sgn == -1)
			);

		if (operation == _REQ_AND) {
			if (!res) return false;
		} else if (operation == _REQ_OR) {
			orRes |= res;
		}
	}
	return !orReq || orRes;
};

};
