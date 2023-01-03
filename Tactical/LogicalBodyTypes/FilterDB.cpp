#include "FilterDB.h"

namespace LogicalBodyTypes {

	FilterDB::FilterDB(void) : AbstractXMLLoader(StartElementHandle, EndElementHandle, CharacterDataHandle, MakeParseData) {
		RegisterEnumerators();
	}

	FilterDB::~FilterDB(void) {
		for (FilterMap::iterator i = filters.begin(); i != filters.end(); i++) {
			delete i->second;
		}
	}

	bool FilterDB::AddFilter(std::string name, Filter* filter) {
		iterator i = filters.find(name);
		if (i != filters.end()) {
			return false;
		}
		filters[name] = filter;
		return true;
	}

	Filter* FilterDB::FindFilter(std::string name) {
		iterator i = filters.find(name);
		return (i != filters.end()) ? i->second : NULL;
	}

	FilterDB::FilterMap::const_iterator FilterDB::begin() {
		return filters.begin();
	}

	FilterDB::FilterMap::const_iterator FilterDB::end() {
		return filters.end();
	}

	AbstractXMLLoader::ParseData* FilterDB::MakeParseData(XML_Parser* parser) {
		return new ParseData(parser);
	}

	void XMLCALL FilterDB::StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts) {
		ParseData* data = (ParseData*)userData;
		switch (data->state) {
		case E_NONE:
			if (strcmp(name, "Filters") == 0) {
				data->state = E_ELEMENT_FILTERS;
				break;
			}
		case E_ELEMENT_FILTERS:
			if (strcmp(name, "Filter") == 0) {
				data->state = E_ELEMENT_FILTER;
				data->currentFilter = new Filter();
				XML_Char const* attrName = GetAttribute("name", atts);
				if (attrName == NULL) throw XMLParseException("Mandatory attribute missing!", name, data->pParser);
				data->filterName = attrName;
				break;
			}
		case E_ELEMENT_FILTER:
			if (strcmp(name, "AND") == 0) {
				data->state = E_OPERATION;
				data->operationFlags = Filter::_REQ_AND;
				break;
			}
			if (strcmp(name, "OR") == 0) {
				data->state = E_OPERATION;
				data->operationFlags = Filter::_REQ_OR;
				break;
			}
		case E_OPERATION:
		{
			data->state = E_CRITERION;
			data->criterionType = 0;
			int ord;
			data->criterionName = name;
			std::string enumeratorString = "Filter::REQ_";
			enumeratorString += name;
			if (EnumeratorDB::Instance().GetEnumeratorByStr("IntegerFilterCriterionTypes", enumeratorString.c_str(), ord)) {
				data->criterionType = ord;
				data->criterionType |= Filter::_TYPE_INTEGER;
			}
			else if (EnumeratorDB::Instance().GetEnumeratorByStr("StringFilterCriterionTypes", enumeratorString.c_str(), ord)) {
				data->criterionType = ord;
				data->criterionType |= Filter::_TYPE_STRING;
			}
			else if (EnumeratorDB::Instance().GetEnumeratorByStr("EnumFilterCriterionTypes", enumeratorString.c_str(), ord)) {
				data->criterionType = ord;
				data->criterionType |= Filter::_TYPE_ENUM;
			}
			else if (strcmp(name, "FILTER") == 0) {
				data->criterionType |= Filter::_TYPE_FILTER;
			}
			else throw XMLParseException("Unexpected element (unknown criterion)!", name, data->pParser);

			XML_Char const* cmpOp = GetAttribute("op", atts);
			if (cmpOp != NULL) {
				if (strcmp(cmpOp, "gt") == 0) {
					if ((data->criterionType & Filter::_TYPE_FILTER) == Filter::_TYPE_FILTER) {
						throw XMLParseException("Greater than operator not supported for filter criterion types!", name, data->pParser);
					}
					data->criterionType |= Filter::_REQ_GT;
				}
				else if (strcmp(cmpOp, "lt") == 0) {
					if ((data->criterionType & Filter::_TYPE_FILTER) == Filter::_TYPE_FILTER) {
						throw XMLParseException("Lesser than operator not supported for filter criterion types!", name, data->pParser);
					}
					data->criterionType |= Filter::_REQ_LT;
				}
				else if (strcmp(cmpOp, "eq") == 0) data->criterionType |= Filter::_REQ_EQ;
				else if (strcmp(cmpOp, "btwn") == 0) {
					if ((data->criterionType & Filter::_TYPE_INTEGER) != Filter::_TYPE_INTEGER) {
						// TODO: also allow between for enums
						throw XMLParseException("Between operator only handles numerical values!", name, data->pParser);
					}
					data->criterionType |= Filter::_REQ_BTWN | Filter::_TYPE_PAIR;
				}
				else if (strcmp(cmpOp, "in") == 0) {
					if (data->criterionType & (Filter::_TYPE_INTEGER | Filter::_TYPE_STRING)) {
						// TODO: also allow in for enums
						data->criterionType |= Filter::_REQ_IN | Filter::_TYPE_LIST;
					}
					else throw XMLParseException("In operator only handles numerical or string values!", name, data->pParser);
				}
				else throw XMLParseException("Undefined operator!", name, data->pParser);
			}
			else {
				data->criterionType |= Filter::_REQ_EQ;
			}
			XML_Char const* not = GetAttribute("not", atts);
			if (not != NULL) {
				if (strcmp(not, "") != 0) throw XMLParseException("Not attribute must not have a value assigned!", name, data->pParser);
				data->criterionType |= Filter::_REQ_NOT;
			}
			break;
		}
		default:
			throw XMLParseException("Unexpected element!", name, data->pParser);
		}
	}

	void XMLCALL FilterDB::EndElementHandle(void* userData, const XML_Char* name) {
		ParseData* data = (ParseData*)userData;
		switch (data->state) {
		case E_CRITERION:
			if (strcmp(name, data->criterionName.c_str()) == 0) {
				data->state = E_OPERATION;
				break;
			}
		case E_OPERATION:
			if (((data->operationFlags & Filter::_REQ_AND) && strcmp(name, "AND") == 0) || ((data->operationFlags & Filter::_REQ_OR) && strcmp(name, "OR") == 0)) {
				data->state = E_ELEMENT_FILTER;
				break;
			}
		case E_ELEMENT_FILTER:
			if (strcmp(name, "Filter") == 0) {
				data->state = E_ELEMENT_FILTERS;
				if (!Instance().AddFilter(data->filterName, data->currentFilter)) {
					throw XMLParseException("Filter could not be added. Filter already defined!", name, data->pParser);
				}
				data->currentFilter = NULL; // in case an exception is raised between here and the start of the next filter element
				break;
			}
		case E_ELEMENT_FILTERS:
			if (strcmp(name, "Filters") == 0) {
				data->state = E_NONE;
				break;
			}
		default:
			throw XMLParseException("Unexpected element! Closing tag missing?", name, data->pParser);
			return;
		}
	}

	void XMLCALL FilterDB::CharacterDataHandle(void* userData, const XML_Char* str, int len) {
		ParseData* data = (ParseData*)userData;
		switch (data->state) {
		case E_CRITERION:
			// TODO: trim character data!
			char* text = new char[len + 1];
			strncpy(text, str, len);
			text[len] = '\0';
			if (data->criterionType & Filter::_TYPE_STRING) {
				STR16 wStr = NULL;
				int len = MultiByteToWideChar(CP_UTF8, 0, text, -1, wStr, 0);
				wStr = (STR16)MemAlloc(len * sizeof(CHAR16));
				MultiByteToWideChar(CP_UTF8, 0, text, -1, wStr, len);
				if (data->criterionType & Filter::_TYPE_LIST) {
					Filter::StringList list;
					wchar_t* token = wcstok(wStr, L", ");
					while (token != NULL) {
						list.push_back(token);
						token = wcstok(NULL, L", ");
					}
					data->currentFilter->AddCriterion(data->operationFlags | data->criterionType, list);
				}
				else {
					data->currentFilter->AddCriterion(data->operationFlags | data->criterionType, wStr);
				}
				MemFree(wStr);
			}
			else if (data->criterionType & Filter::_TYPE_INTEGER) {
				if ((data->criterionType & Filter::_TYPE_LIST) || (data->criterionType & Filter::_TYPE_PAIR)) {
					Filter::NumberList list;
					char* token = strtok(text, ", ");
					while (token != NULL) {
						INT32 val;
						if (!ConvertStringToINT32(token, &val)) {
							delete text;
							throw XMLParseException("Invalid value!", data->criterionName.c_str(), data->pParser);
						}
						list.push_back(val);
						token = strtok(NULL, ", ");
					}
					if (data->criterionType & Filter::_TYPE_PAIR) {
						if (list.size() != 2) {
							delete text;
							throw XMLParseException("Exactly 2 operands must be specified for 'between' operator!", data->criterionName.c_str(), data->pParser);
						}
						data->currentFilter->AddCriterion(data->operationFlags | data->criterionType, list.front(), list.back());
					}
					else if (data->criterionType & Filter::_TYPE_LIST) {
						data->currentFilter->AddCriterion(data->operationFlags | data->criterionType, list);
					}
				}
				else {
					INT32 val;
					if (!ConvertStringToINT32(text, &val)) {
						delete text;
						throw XMLParseException("Invalid value!", data->criterionName.c_str(), data->pParser);
					}
					data->currentFilter->AddCriterion(data->operationFlags | data->criterionType, val);
				}
			}
			else if (data->criterionType & Filter::_TYPE_ENUM) {
				INT32 ord;
				if (EnumeratorDB::Instance().GetEnumeratorByStr(data->criterionName.c_str(), text, ord)) {
					// For the filter no enum type exists.
					// Since we now know the integer value of the enumerator we treat it like any other integer criterion
					data->criterionType &= ~Filter::_TYPE_ENUM;
					data->criterionType |= Filter::_TYPE_INTEGER;
					data->currentFilter->AddCriterion(data->operationFlags | data->criterionType, ord);
				}
				else {
					delete text;
					throw XMLParseException("Can't find specified enumerator!", data->criterionName.c_str(), data->pParser);
				}
			}
			else if (data->criterionType & Filter::_TYPE_FILTER) {
				Filter* f = Instance().FindFilter(text);
				if (f == NULL) {
					throw XMLParseException("Can't find specified filter. Make sure it is defined before the filter that references it!", data->criterionName.c_str(), data->pParser);
				}
				data->currentFilter->AddCriterion(data->operationFlags | data->criterionType, f);
			}
			delete text;
			break;
		}
	}

	void FilterDB::RegisterEnumerators() {

		/*****************************************
		  Filter string criterion types
		******************************************/
		LOGBT_ENUMDB_ADD("StringFilterCriterionTypes", 3,
			Filter::REQ_NAME,
			Filter::REQ_PROFILENAME,
			Filter::REQ_NICKNAME
		);

		/*****************************************
		  Filter enum criterion types
		******************************************/
		LOGBT_ENUMDB_ADD("IntegerFilterCriterionTypes", 33,
			Filter::REQ_HELMETPOS,
			Filter::REQ_VESTPOS,
			Filter::REQ_LEGPOS,
			Filter::REQ_HEAD1POS,
			Filter::REQ_HEAD2POS,
			Filter::REQ_HANDPOS,
			Filter::REQ_SECONDHANDPOS,
			Filter::REQ_VESTPOCKPOS,
			Filter::REQ_LTHIGHPOCKPOS,
			Filter::REQ_RTHIGHPOCKPOS,
			Filter::REQ_CPACKPOCKPOS,
			Filter::REQ_BPACKPOCKPOS,
			Filter::REQ_GUNSLINGPOCKPOS,
			Filter::REQ_KNIFEPOCKPOS,
			Filter::REQ_TEAM,
			Filter::REQ_CAMO,
			Filter::REQ_URBANCAMO,
			Filter::REQ_DESERTCAMO,
			Filter::REQ_SNOWCAMO,
			Filter::REQ_EXPLEVEL,
			Filter::REQ_STRENGTH,
			Filter::REQ_LEADERSHIP,
			Filter::REQ_WISDOM,
			Filter::REQ_SKILLTRAIT1,
			Filter::REQ_SKILLTRAIT2,
			Filter::REQ_FACEINDEX,
			Filter::REQ_WEAPON_IN_HAND,
			Filter::REQ_CALIBRE,
			Filter::REQ_VEST_AMOR_PROTECTION,
			Filter::REQ_VEST_AMOR_COVERAGE,
			Filter::REQ_HELMET_AMOR_PROTECTION,
			Filter::REQ_HELMET_AMOR_COVERAGE,
			Filter::REQ_WEARING_BACKPACK
		);

		/*****************************************
		  Filter enum criterion types
		******************************************/
		LOGBT_ENUMDB_ADD("EnumFilterCriterionTypes", 8,
			Filter::REQ_SEX,
			Filter::REQ_MERC_TYPE,
			Filter::REQ_SOLDIER_CLASS,
			Filter::REQ_CIVILIANGROUP,
			Filter::REQ_BODYTYPE,
			Filter::REQ_WEAPON_CLASS,
			Filter::REQ_WEAPON_TYPE,
			Filter::REQ_LEFT_WEAPON_TYPE
		);

		/*****************************************
		  MERC_TYPE
		******************************************/
		LOGBT_ENUMDB_ADD("MERC_TYPE", 7,
			MERC_TYPE__PLAYER_CHARACTER,
			MERC_TYPE__AIM_MERC,
			MERC_TYPE__MERC,
			MERC_TYPE__NPC,
			MERC_TYPE__EPC,
			MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT,
			MERC_TYPE__VEHICLE
		);

		/*****************************************
		  SOLDIER_CLASS
		******************************************/
	LOGBT_ENUMDB_ADD("SOLDIER_CLASS", SOLDIER_CLASS_MAX,
			SOLDIER_CLASS_NONE,
			SOLDIER_CLASS_ADMINISTRATOR,
			SOLDIER_CLASS_ELITE,
			SOLDIER_CLASS_ARMY,
			SOLDIER_CLASS_GREEN_MILITIA,
			SOLDIER_CLASS_REG_MILITIA,
			SOLDIER_CLASS_ELITE_MILITIA,
			SOLDIER_CLASS_CREATURE,
			SOLDIER_CLASS_MINER,
			SOLDIER_CLASS_ZOMBIE,
			SOLDIER_CLASS_TANK,
			SOLDIER_CLASS_JEEP,
			SOLDIER_CLASS_BANDIT,
			SOLDIER_CLASS_ROBOT
		);

		/*****************************************
		  Filter enum criterion types
		******************************************/
		LOGBT_ENUMDB_ADD("SEX", 2,
			MALE,
			FEMALE
		);

		/*****************************************
		  SoldierBodyTypes
		******************************************/
		LOGBT_ENUMDB_ADD("BODYTYPE", TOTALBODYTYPES,
			REGMALE,
			BIGMALE,
			STOCKYMALE,
			REGFEMALE,
			ADULTFEMALEMONSTER,
			AM_MONSTER,
			YAF_MONSTER,
			YAM_MONSTER,
			LARVAE_MONSTER,
			INFANT_MONSTER,
			QUEENMONSTER,
			FATCIV,
			MANCIV,
			MINICIV,
			DRESSCIV,
			HATKIDCIV,
			KIDCIV,
			CRIPPLECIV,
			COW,
			CROW,
			BLOODCAT,
			ROBOTNOWEAPON,
			HUMVEE,
			TANK_NW,
			TANK_NE,
			ELDORADO,
			ICECREAMTRUCK,
			JEEP,
			COMBAT_JEEP
		);

		/*****************************************
		  Civilian Groups
		******************************************/
		LOGBT_ENUMDB_ADD("CIVILIANGROUP", NUM_CIV_GROUPS,
			NON_CIV_GROUP,
			REBEL_CIV_GROUP,
			KINGPIN_CIV_GROUP,
			SANMONA_ARMS_GROUP,
			ANGELS_GROUP,
			BEGGARS_CIV_GROUP,
			TOURISTS_CIV_GROUP,
			ALMA_MILITARY_CIV_GROUP,
			DOCTORS_CIV_GROUP,
			COUPLE1_CIV_GROUP,
			HICKS_CIV_GROUP,
			WARDEN_CIV_GROUP,
			JUNKYARD_CIV_GROUP,
			FACTORY_KIDS_GROUP,
			QUEENS_CIV_GROUP,
			UNNAMED_CIV_GROUP_15,
			UNNAMED_CIV_GROUP_16,
			UNNAMED_CIV_GROUP_17,
			UNNAMED_CIV_GROUP_18,
			UNNAMED_CIV_GROUP_19,
			ASSASSIN_CIV_GROUP,
			POW_PRISON_CIV_GROUP,
			UNNAMED_CIV_GROUP_22,
			UNNAMED_CIV_GROUP_23,
			VOLUNTEER_CIV_GROUP,
			BOUNTYHUNTER_CIV_GROUP,
			DOWNEDPILOT_CIV_GROUP,
			SCIENTIST_GROUP,
			RADAR_TECHNICIAN_GROUP,
			AIRPORT_STAFF_GROUP,
			BARRACK_STAFF_GROUP,
			FACTORY_GROUP,
			ADMINISTRATIVE_STAFF_GROUP,
			LOYAL_CIV_GROUP,
			BLACKMARKET_GROUP,
			UNNAMED_CIV_GROUP_35,
			UNNAMED_CIV_GROUP_36,
			UNNAMED_CIV_GROUP_37,
			UNNAMED_CIV_GROUP_38,
			UNNAMED_CIV_GROUP_39,
			UNNAMED_CIV_GROUP_40,
			UNNAMED_CIV_GROUP_41,
			UNNAMED_CIV_GROUP_42,
			UNNAMED_CIV_GROUP_43,
			UNNAMED_CIV_GROUP_44,
			UNNAMED_CIV_GROUP_45,
			UNNAMED_CIV_GROUP_46,
			UNNAMED_CIV_GROUP_47,
			UNNAMED_CIV_GROUP_48,
			UNNAMED_CIV_GROUP_49,
			UNNAMED_CIV_GROUP_50,
			UNNAMED_CIV_GROUP_51,
			UNNAMED_CIV_GROUP_52,
			UNNAMED_CIV_GROUP_53,
			UNNAMED_CIV_GROUP_54,
			UNNAMED_CIV_GROUP_55,
			UNNAMED_CIV_GROUP_56,
			UNNAMED_CIV_GROUP_57,
			UNNAMED_CIV_GROUP_58,
			UNNAMED_CIV_GROUP_59,
			UNNAMED_CIV_GROUP_60,
			UNNAMED_CIV_GROUP_61,
			UNNAMED_CIV_GROUP_62,
			UNNAMED_CIV_GROUP_63,
			UNNAMED_CIV_GROUP_64,
			UNNAMED_CIV_GROUP_65,
			UNNAMED_CIV_GROUP_66,
			UNNAMED_CIV_GROUP_67,
			UNNAMED_CIV_GROUP_68,
			UNNAMED_CIV_GROUP_69,
			UNNAMED_CIV_GROUP_70,
			UNNAMED_CIV_GROUP_71,
			UNNAMED_CIV_GROUP_72,
			UNNAMED_CIV_GROUP_73,
			UNNAMED_CIV_GROUP_74,
			UNNAMED_CIV_GROUP_75,
			UNNAMED_CIV_GROUP_76,
			UNNAMED_CIV_GROUP_77,
			UNNAMED_CIV_GROUP_78,
			UNNAMED_CIV_GROUP_79,
			UNNAMED_CIV_GROUP_80,
			UNNAMED_CIV_GROUP_81,
			UNNAMED_CIV_GROUP_82,
			UNNAMED_CIV_GROUP_83,
			UNNAMED_CIV_GROUP_84,
			UNNAMED_CIV_GROUP_85,
			UNNAMED_CIV_GROUP_86,
			UNNAMED_CIV_GROUP_87,
			UNNAMED_CIV_GROUP_88,
			UNNAMED_CIV_GROUP_89,
			UNNAMED_CIV_GROUP_90,
			UNNAMED_CIV_GROUP_91,
			UNNAMED_CIV_GROUP_92,
			UNNAMED_CIV_GROUP_93,
			UNNAMED_CIV_GROUP_94,
			UNNAMED_CIV_GROUP_95,
			UNNAMED_CIV_GROUP_96,
			UNNAMED_CIV_GROUP_97,
			UNNAMED_CIV_GROUP_98,
			UNNAMED_CIV_GROUP_99,
			UNNAMED_CIV_GROUP_100,
			UNNAMED_CIV_GROUP_101,
			UNNAMED_CIV_GROUP_102,
			UNNAMED_CIV_GROUP_103,
			UNNAMED_CIV_GROUP_104,
			UNNAMED_CIV_GROUP_105,
			UNNAMED_CIV_GROUP_106,
			UNNAMED_CIV_GROUP_107,
			UNNAMED_CIV_GROUP_108,
			UNNAMED_CIV_GROUP_109,
			UNNAMED_CIV_GROUP_110,
			UNNAMED_CIV_GROUP_111,
			UNNAMED_CIV_GROUP_112,
			UNNAMED_CIV_GROUP_113,
			UNNAMED_CIV_GROUP_114,
			UNNAMED_CIV_GROUP_115,
			UNNAMED_CIV_GROUP_116,
			UNNAMED_CIV_GROUP_117,
			UNNAMED_CIV_GROUP_118,
			UNNAMED_CIV_GROUP_119,
			UNNAMED_CIV_GROUP_120,
			UNNAMED_CIV_GROUP_121,
			UNNAMED_CIV_GROUP_122,
			UNNAMED_CIV_GROUP_123,
			UNNAMED_CIV_GROUP_124,
			UNNAMED_CIV_GROUP_125,
			UNNAMED_CIV_GROUP_126,
			UNNAMED_CIV_GROUP_127,
			UNNAMED_CIV_GROUP_128,
			UNNAMED_CIV_GROUP_129,
			UNNAMED_CIV_GROUP_130,
			UNNAMED_CIV_GROUP_131,
			UNNAMED_CIV_GROUP_132,
			UNNAMED_CIV_GROUP_133,
			UNNAMED_CIV_GROUP_134,
			UNNAMED_CIV_GROUP_135,
			UNNAMED_CIV_GROUP_136,
			UNNAMED_CIV_GROUP_137,
			UNNAMED_CIV_GROUP_138,
			UNNAMED_CIV_GROUP_139,
			UNNAMED_CIV_GROUP_140,
			UNNAMED_CIV_GROUP_141,
			UNNAMED_CIV_GROUP_142,
			UNNAMED_CIV_GROUP_143,
			UNNAMED_CIV_GROUP_144,
			UNNAMED_CIV_GROUP_145,
			UNNAMED_CIV_GROUP_146,
			UNNAMED_CIV_GROUP_147,
			UNNAMED_CIV_GROUP_148,
			UNNAMED_CIV_GROUP_149,
			UNNAMED_CIV_GROUP_150,
			UNNAMED_CIV_GROUP_151,
			UNNAMED_CIV_GROUP_152,
			UNNAMED_CIV_GROUP_153,
			UNNAMED_CIV_GROUP_154,
			UNNAMED_CIV_GROUP_155,
			UNNAMED_CIV_GROUP_156,
			UNNAMED_CIV_GROUP_157,
			UNNAMED_CIV_GROUP_158,
			UNNAMED_CIV_GROUP_159,
			UNNAMED_CIV_GROUP_160,
			UNNAMED_CIV_GROUP_161,
			UNNAMED_CIV_GROUP_162,
			UNNAMED_CIV_GROUP_163,
			UNNAMED_CIV_GROUP_164,
			UNNAMED_CIV_GROUP_165,
			UNNAMED_CIV_GROUP_166,
			UNNAMED_CIV_GROUP_167,
			UNNAMED_CIV_GROUP_168,
			UNNAMED_CIV_GROUP_169,
			UNNAMED_CIV_GROUP_170,
			UNNAMED_CIV_GROUP_171,
			UNNAMED_CIV_GROUP_172,
			UNNAMED_CIV_GROUP_173,
			UNNAMED_CIV_GROUP_174,
			UNNAMED_CIV_GROUP_175,
			UNNAMED_CIV_GROUP_176,
			UNNAMED_CIV_GROUP_177,
			UNNAMED_CIV_GROUP_178,
			UNNAMED_CIV_GROUP_179,
			UNNAMED_CIV_GROUP_180,
			UNNAMED_CIV_GROUP_181,
			UNNAMED_CIV_GROUP_182,
			UNNAMED_CIV_GROUP_183,
			UNNAMED_CIV_GROUP_184,
			UNNAMED_CIV_GROUP_185,
			UNNAMED_CIV_GROUP_186,
			UNNAMED_CIV_GROUP_187,
			UNNAMED_CIV_GROUP_188,
			UNNAMED_CIV_GROUP_189,
			UNNAMED_CIV_GROUP_190,
			UNNAMED_CIV_GROUP_191,
			UNNAMED_CIV_GROUP_192,
			UNNAMED_CIV_GROUP_193,
			UNNAMED_CIV_GROUP_194,
			UNNAMED_CIV_GROUP_195,
			UNNAMED_CIV_GROUP_196,
			UNNAMED_CIV_GROUP_197,
			UNNAMED_CIV_GROUP_198,
			UNNAMED_CIV_GROUP_199,
			UNNAMED_CIV_GROUP_200,
			UNNAMED_CIV_GROUP_201,
			UNNAMED_CIV_GROUP_202,
			UNNAMED_CIV_GROUP_203,
			UNNAMED_CIV_GROUP_204,
			UNNAMED_CIV_GROUP_205,
			UNNAMED_CIV_GROUP_206,
			UNNAMED_CIV_GROUP_207,
			UNNAMED_CIV_GROUP_208,
			UNNAMED_CIV_GROUP_209,
			UNNAMED_CIV_GROUP_210,
			UNNAMED_CIV_GROUP_211,
			UNNAMED_CIV_GROUP_212,
			UNNAMED_CIV_GROUP_213,
			UNNAMED_CIV_GROUP_214,
			UNNAMED_CIV_GROUP_215,
			UNNAMED_CIV_GROUP_216,
			UNNAMED_CIV_GROUP_217,
			UNNAMED_CIV_GROUP_218,
			UNNAMED_CIV_GROUP_219,
			UNNAMED_CIV_GROUP_220,
			UNNAMED_CIV_GROUP_221,
			UNNAMED_CIV_GROUP_222,
			UNNAMED_CIV_GROUP_223,
			UNNAMED_CIV_GROUP_224,
			UNNAMED_CIV_GROUP_225,
			UNNAMED_CIV_GROUP_226,
			UNNAMED_CIV_GROUP_227,
			UNNAMED_CIV_GROUP_228,
			UNNAMED_CIV_GROUP_229,
			UNNAMED_CIV_GROUP_230,
			UNNAMED_CIV_GROUP_231,
			UNNAMED_CIV_GROUP_232,
			UNNAMED_CIV_GROUP_233,
			UNNAMED_CIV_GROUP_234,
			UNNAMED_CIV_GROUP_235,
			UNNAMED_CIV_GROUP_236,
			UNNAMED_CIV_GROUP_237,
			UNNAMED_CIV_GROUP_238,
			UNNAMED_CIV_GROUP_239,
			UNNAMED_CIV_GROUP_240,
			UNNAMED_CIV_GROUP_241,
			UNNAMED_CIV_GROUP_242,
			UNNAMED_CIV_GROUP_243,
			UNNAMED_CIV_GROUP_244,
			UNNAMED_CIV_GROUP_245,
			UNNAMED_CIV_GROUP_246,
			UNNAMED_CIV_GROUP_247,
			UNNAMED_CIV_GROUP_248,
			UNNAMED_CIV_GROUP_249,
			UNNAMED_CIV_GROUP_250,
			UNNAMED_CIV_GROUP_251,
			UNNAMED_CIV_GROUP_252,
			UNNAMED_CIV_GROUP_253,
			UNNAMED_CIV_GROUP_254
		);

		/*****************************************
		  Weapon Class
		******************************************/
		LOGBT_ENUMDB_ADD("WEAPON_CLASS", NUM_WEAPON_CLASSES,
			NOGUNCLASS,
			HANDGUNCLASS,
			SMGCLASS,
			RIFLECLASS,
			MGCLASS,
			SHOTGUNCLASS,
			KNIFECLASS,
			MONSTERCLASS
		);

		/*****************************************
		  Weapon Type
		******************************************/
		LOGBT_ENUMDB_ADD("WEAPON_TYPE", GUN_TYPES_MAX,
			NOT_GUN,
			GUN_PISTOL,
			GUN_M_PISTOL,
			GUN_SMG,
			GUN_RIFLE,
			GUN_SN_RIFLE,
			GUN_AS_RIFLE,
			GUN_LMG,
			GUN_SHOTGUN
		);

		LOGBT_ENUMDB_ADD("LEFT_WEAPON_TYPE", GUN_TYPES_MAX,
			NOT_GUN,
			GUN_PISTOL,
			GUN_M_PISTOL,
			GUN_SMG,
			GUN_RIFLE,
			GUN_SN_RIFLE,
			GUN_AS_RIFLE,
			GUN_LMG,
			GUN_SHOTGUN
		);
	};

}
