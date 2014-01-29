#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	TAUNT_VALUES	curTaunt;
	TAUNT_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef tauntParseData;

BOOLEAN localizedTextOnly_Taunts;

UINT16 num_found_taunt = 0;	// the correct number is set on reading the xml

static void XMLCALL
tauntStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	tauntParseData * pData = (tauntParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "TAUNTS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			if ( !localizedTextOnly_Taunts )
				memset(pData->curArray,0,sizeof(TAUNT_VALUES)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "TAUNT") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			if ( !localizedTextOnly_Taunts )
				memset(&pData->curTaunt,0,sizeof(TAUNT_VALUES));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szText") == 0 ||
				strcmp(name, "szCensoredText") == 0 ||

				strcmp(name, "aggressive") == 0 ||
				strcmp(name, "defensive") == 0 ||
				strcmp(name, "cunning") == 0 ||
				strcmp(name, "cunning_aid") == 0 ||
				strcmp(name, "cunning_solo") == 0 ||
				strcmp(name, "brave") == 0 ||
				strcmp(name, "brave_aid") == 0 ||
				strcmp(name, "brave_solo") == 0 ||

				strcmp(name, "attack") == 0 ||
				strcmp(name, "fire_gun") == 0 ||
				strcmp(name, "fire_launcher") == 0 ||
				strcmp(name, "attack_blade") == 0 ||
				strcmp(name, "attack_hth") == 0 ||

				strcmp(name, "throw_knife") == 0 ||
				strcmp(name, "throw_grenade") == 0 ||

				strcmp(name, "out_of_ammo") == 0 ||
				strcmp(name, "reload") == 0 ||

				strcmp(name, "steal") == 0 ||

				strcmp(name, "charge_blade") == 0 ||
				strcmp(name, "charge_hth") == 0 ||		
				strcmp(name, "run_away") == 0 ||
				strcmp(name, "seek_noise") == 0 ||
				strcmp(name, "alert") == 0 ||
				strcmp(name, "suspicious") == 0 ||
				strcmp(name, "noticed_unseen") == 0 ||
				strcmp(name, "say_hi") == 0 ||
				strcmp(name, "inform_about") == 0 ||

				strcmp(name, "got_hit") == 0 ||
				strcmp(name, "got_hit_gunfire") == 0 ||
				strcmp(name, "got_hit_blade") == 0 ||
				strcmp(name, "got_hit_hth") == 0 ||
				strcmp(name, "got_hit_fallroof") == 0 ||
				strcmp(name, "got_hit_bloodloss") == 0 ||
				strcmp(name, "got_hit_explosion") == 0 ||
				strcmp(name, "got_hit_gas") == 0 ||
				strcmp(name, "got_hit_tentacle") == 0 ||
				strcmp(name, "got_hit_structure_explosion") == 0 ||
				strcmp(name, "got_hit_object") == 0 ||
				strcmp(name, "got_hit_throwing_knife") == 0 ||

				strcmp(name, "got_deafened") == 0 ||
				strcmp(name, "got_blinded") == 0 ||

				strcmp(name, "got_robbed") == 0 ||

				strcmp(name, "got_missed") == 0 ||
				strcmp(name, "got_missed_gunfire") == 0 ||
				strcmp(name, "got_missed_blade") == 0 ||
				strcmp(name, "got_missed_hth") == 0 ||
				strcmp(name, "got_missed_throwing_knife") == 0 ||

				strcmp(name, "hit") == 0 ||
				strcmp(name, "hit_gunfire") == 0 ||
				strcmp(name, "hit_blade") == 0 ||
				strcmp(name, "hit_hth") == 0 ||
				strcmp(name, "hit_throwing_knife") == 0 ||

				strcmp(name, "kill") == 0 ||
				strcmp(name, "kill_gunfire") == 0 ||
				strcmp(name, "kill_blade") == 0 ||
				strcmp(name, "kill_hth") == 0 ||
				strcmp(name, "kill_throwing_knife") == 0 ||

				strcmp(name, "head_pop") == 0 ||

				strcmp(name, "miss") == 0 ||
				strcmp(name, "miss_gunfire") == 0 ||
				strcmp(name, "miss_blade") == 0 ||
				strcmp(name, "miss_hth") == 0 ||
				strcmp(name, "miss_throwing_knife") == 0 ||

				strcmp(name, "riposte_quote") == 0 ||

				strcmp(name, "enemy") == 0 ||
				strcmp(name, "admin") == 0 ||
				strcmp(name, "army") == 0 ||
				strcmp(name, "elite") == 0 ||
				strcmp(name, "militia") == 0 ||
				strcmp(name, "green") == 0 ||
				strcmp(name, "regular") == 0 ||
				strcmp(name, "veteran") == 0 ||

				strcmp(name, "admin_profile") == 0 ||
				strcmp(name, "army_profile") == 0 ||
				strcmp(name, "elite_profile") == 0 ||
				strcmp(name, "green_profile") == 0 ||
				strcmp(name, "regular_profile") == 0 ||
				strcmp(name, "veteran_profile") == 0 ||

				strcmp(name, "male") == 0 ||
				strcmp(name, "female") == 0 ||

				strcmp(name, "exp_level_gt") == 0 ||
				strcmp(name, "exp_level_lt") == 0 ||
				strcmp(name, "health_gt") == 0 ||
				strcmp(name, "health_lt") == 0 ||
				strcmp(name, "health_max_gt") == 0 ||
				strcmp(name, "health_max_lt") == 0 ||
				strcmp(name, "energy_gt") == 0 ||
				strcmp(name, "energy_lt") == 0 ||
				strcmp(name, "energy_max_gt") == 0 ||
				strcmp(name, "energy_max_lt") == 0 ||
				strcmp(name, "morale_gt") == 0 ||
				strcmp(name, "morale_lt") == 0 ||
				strcmp(name, "progress_gt") == 0 ||
				strcmp(name, "progress_lt") == 0 ||

				strcmp(name, "target_type") == 0 ||
				strcmp(name, "target_male") == 0 ||
				strcmp(name, "target_female") == 0 ||
				strcmp(name, "target_exp_level_gt") == 0 ||
				strcmp(name, "target_exp_level_lt") == 0 ||
				strcmp(name, "target_health_gt") == 0 ||
				strcmp(name, "target_health_lt") == 0 ||
				strcmp(name, "target_health_max_gt") == 0 ||
				strcmp(name, "target_health_max_lt") == 0 ||
				strcmp(name, "target_energy_gt") == 0 ||
				strcmp(name, "target_energy_lt") == 0 ||
				strcmp(name, "target_energy_max_gt") == 0 ||
				strcmp(name, "target_energy_max_lt") == 0 ||
				strcmp(name, "target_morale_gt") == 0 ||
				strcmp(name, "target_morale_lt") == 0 ||
				strcmp(name, "target_appearance") == 0 ||
				strcmp(name, "target_merc_profile") == 0 ) ||
				strcmp(name, "target_zombie") == 0 )
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
tauntCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	tauntParseData * pData = (tauntParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}

static void XMLCALL
tauntEndElementHandle(void *userData, const XML_Char *name)
{
	tauntParseData * pData = (tauntParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "TAUNTS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "TAUNT") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			//if(pData->curTaunt.uiIndex < pData->maxArraySize)
			if(num_found_taunt < pData->maxArraySize)
			{
				if ( localizedTextOnly_Taunts )
				{			
					// WANNE: Not working
					/*
					MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curArray[pData->curTaunt.uiIndex].szText, sizeof(pData->curTaunt.szText)/sizeof(pData->curTaunt.szText[0]) );
					pData->curTaunt.szText[sizeof(pData->curTaunt.szText)/sizeof(pData->curTaunt.szText[0]) - 1] = '\0';
					MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curArray[pData->curTaunt.uiIndex].szCensoredText, sizeof(pData->curTaunt.szCensoredText)/sizeof(pData->curTaunt.szCensoredText[0]) );
					pData->curTaunt.szCensoredText[sizeof(pData->curTaunt.szCensoredText)/sizeof(pData->curTaunt.szCensoredText[0]) - 1] = '\0';
					*/

					// WANNE: Working
					wcscpy(zTaunt[pData->curTaunt.uiIndex].szText,pData->curTaunt.szText);
					wcscpy(zTaunt[pData->curTaunt.uiIndex].szCensoredText,pData->curTaunt.szCensoredText);
				}
				else
				{
					// no attitude specified -> set all
					if( !(pData->curTaunt.uiFlags & ( TAUNT_A_AGGRESSIVE | TAUNT_A_DEFENSIVE | TAUNT_A_BRAVE_AID | TAUNT_A_BRAVE_SOLO |
						TAUNT_A_CUNNING_AID | TAUNT_A_CUNNING_SOLO) ) )
					{
						pData->curTaunt.uiFlags |= TAUNT_A_AGGRESSIVE;
						pData->curTaunt.uiFlags |= TAUNT_A_DEFENSIVE;
						pData->curTaunt.uiFlags |= TAUNT_A_BRAVE_AID;
						pData->curTaunt.uiFlags |= TAUNT_A_BRAVE_SOLO;
						pData->curTaunt.uiFlags |= TAUNT_A_CUNNING_AID;
						pData->curTaunt.uiFlags |= TAUNT_A_CUNNING_SOLO;
					}
					// no gender specified -> set all
					if( !(pData->curTaunt.uiFlags2 & ( TAUNT_G_FEMALE | TAUNT_G_MALE ) ) )
					{
						pData->curTaunt.uiFlags2 |= TAUNT_G_FEMALE;
						pData->curTaunt.uiFlags2 |= TAUNT_G_MALE;
					}
					if( !(pData->curTaunt.uiFlags2 & TAUNT_T_FEMALE) &&
						!(pData->curTaunt.uiFlags2 & TAUNT_T_MALE) )
					{
						pData->curTaunt.uiFlags2 |= TAUNT_T_FEMALE;
						pData->curTaunt.uiFlags2 |= TAUNT_T_MALE;
					}

					// no class specified -> set all enemies
					if( !(pData->curTaunt.uiFlags2 & ( TAUNT_C_ADMIN | TAUNT_C_ARMY | TAUNT_C_ELITE | TAUNT_C_GREEN | 
						TAUNT_C_REGULAR | TAUNT_C_VETERAN ) ) )
					{
						pData->curTaunt.uiFlags2 |= TAUNT_C_ADMIN;
						pData->curTaunt.uiFlags2 |= TAUNT_C_ARMY;
						pData->curTaunt.uiFlags2 |= TAUNT_C_ELITE;
						pData->curTaunt.uiFlags2 |= TAUNT_C_GREEN;
						pData->curTaunt.uiFlags2 |= TAUNT_C_REGULAR;
						pData->curTaunt.uiFlags2 |= TAUNT_C_VETERAN;
					}

					pData->curArray[pData->curTaunt.uiIndex] = pData->curTaunt;
					//pData->curArray[num_found_taunt-1] = pData->curTaunt;
				}
			}	
			num_found_taunt++;
			//num_found_taunt = pData->curTaunt.uiIndex;
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiIndex	= (UINT16) atol(pData->szCharData);
			for(UINT16 i = 0; i < TAUNT_MAX; i++)
			{
				pData->curTaunt.value[i] = (-1);
			}	
			pData->curTaunt.uiFlags = 0;
			pData->curTaunt.uiFlags2 = 0;
		}
		else if(strcmp(name, "szText") == 0 )
		{
			pData->curElement = ELEMENT;
			
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curTaunt.szText, sizeof(pData->curTaunt.szText)/sizeof(pData->curTaunt.szText[0]) );
			pData->curTaunt.szText[sizeof(pData->curTaunt.szText)/sizeof(pData->curTaunt.szText[0]) - 1] = '\0';
		}
		else if(strcmp(name, "szCensoredText") == 0 )
		{
			pData->curElement = ELEMENT;
			
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curTaunt.szCensoredText, sizeof(pData->curTaunt.szCensoredText)/sizeof(pData->curTaunt.szCensoredText[0]) );
			pData->curTaunt.szCensoredText[sizeof(pData->curTaunt.szCensoredText)/sizeof(pData->curTaunt.szCensoredText[0]) - 1] = '\0';
		}
		else if(strcmp(name, "aggressive") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_A_AGGRESSIVE : 0;
		}
		else if(strcmp(name, "defensive") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_A_DEFENSIVE : 0;
		}
		else if(strcmp(name, "brave_aid") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_A_BRAVE_AID : 0;
		}
		else if(strcmp(name, "brave_solo") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_A_BRAVE_SOLO : 0;
		}
		else if(strcmp(name, "cunning_aid") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_A_CUNNING_AID : 0;
		}
		else if(strcmp(name, "cunning_solo") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_A_CUNNING_SOLO : 0;
		}
		else if(strcmp(name, "brave") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_A_BRAVE_AID : 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_A_BRAVE_SOLO : 0;
		}
		else if(strcmp(name, "cunning") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_A_CUNNING_AID : 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_A_CUNNING_SOLO : 0;
		}
		else if(strcmp(name, "attack") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_FIRE_GUN : 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_FIRE_LAUNCHER : 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_ATTACK_BLADE : 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_ATTACK_HTH : 0;
		}
		else if(strcmp(name, "fire_gun") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_FIRE_GUN : 0;
		}
		else if(strcmp(name, "fire_launcher") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_FIRE_LAUNCHER : 0;
		}
		else if(strcmp(name, "attack_blade") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_ATTACK_BLADE : 0;
		}
		else if(strcmp(name, "attack_hth") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_ATTACK_HTH : 0;
		}

		else if(strcmp(name, "throw_knife") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_THROW_KNIFE : 0;
		}
		else if(strcmp(name, "throw_grenade") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_THROW_GRENADE: 0;
		}

		else if(strcmp(name, "out_of_ammo") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_OUT_OF_AMMO : 0;
		}
		else if(strcmp(name, "reload") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_RELOAD: 0;
		}

		else if(strcmp(name, "charge_blade") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_CHARGE_BLADE : 0;
		}
		else if(strcmp(name, "charge_hth") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_CHARGE_HTH : 0;
		}
		else if(strcmp(name, "steal") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_STEAL : 0;
		}
		else if(strcmp(name, "run_away") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_RUN_AWAY : 0;
		}
		else if(strcmp(name, "seek_noise") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_SEEK_NOISE : 0;
		}
		else if(strcmp(name, "alert") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_ALERT : 0;
		}
		else if(strcmp(name, "suspicious") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_SUSPICIOUS : 0;
		}
		else if(strcmp(name, "noticed_unseen") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_NOTICED_UNSEEN : 0;
		}
		else if(strcmp(name, "say_hi") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_SAY_HI : 0;
		}
		else if(strcmp(name, "inform_about") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_INFORM_ABOUT : 0;
		}

		else if(strcmp(name, "got_hit") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_GUNFIRE: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_BLADE: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_HTH: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_FALLROOF: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_BLOODLOSS: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_EXPLOSION: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_GAS: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_TENTACLES: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_STRUCTURE_EXPLOSION: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_THROWING_KNIFE: 0;
		}
		else if(strcmp(name, "got_hit_gunfire") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_GUNFIRE: 0;
		}
		else if(strcmp(name, "got_hit_blade") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_BLADE: 0;
		}
		else if(strcmp(name, "got_hit_hth") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_HTH: 0;
		}
		else if(strcmp(name, "got_hit_fallroof") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_FALLROOF: 0;
		}
		else if(strcmp(name, "got_hit_bloodloss") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_BLOODLOSS: 0;
		}
		else if(strcmp(name, "got_hit_explosion") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_EXPLOSION: 0;
		}
		else if(strcmp(name, "got_hit_gas") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_GAS: 0;
		}
		else if(strcmp(name, "got_hit_tentacle") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_TENTACLES: 0;
		}
		else if(strcmp(name, "got_hit_structure_explosion") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_STRUCTURE_EXPLOSION: 0;
		}
		else if(strcmp(name, "got_hit_object") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_OBJECT: 0;
		}
		else if(strcmp(name, "got_hit_throwing_knife") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_HIT_THROWING_KNIFE: 0;
		}

		else if(strcmp(name, "got_deafened") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_DEAFENED: 0;
		}
		else if(strcmp(name, "got_blinded") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_BLINDED: 0;
		}
		else if(strcmp(name, "got_robbed") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_ROBBED: 0;
		}

		else if(strcmp(name, "got_missed") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_MISSED: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_MISSED_GUNFIRE: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_MISSED_BLADE: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_MISSED_HTH: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_MISSED_THROWING_KNIFE: 0;
		}
		else if(strcmp(name, "got_missed_gunfire") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_MISSED_GUNFIRE: 0;
		}
		else if(strcmp(name, "got_missed_blade") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_MISSED_BLADE: 0;
		}
		else if(strcmp(name, "got_missed_hth") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_MISSED_HTH: 0;
		}
		else if(strcmp(name, "got_missed_throwing_knife") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_GOT_MISSED_THROWING_KNIFE: 0;
		}

		else if(strcmp(name, "hit") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_HIT: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_HIT_GUNFIRE: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_HIT_BLADE: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_HIT_HTH: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_HIT_THROWING_KNIFE: 0;
		}
		else if(strcmp(name, "hit_gunfire") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_HIT_GUNFIRE: 0;
		}
		else if(strcmp(name, "hit_blade") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_HIT_BLADE: 0;
		}
		else if(strcmp(name, "hit_hth") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_HIT_HTH: 0;
		}
		else if(strcmp(name, "hit_throwing_knife") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_HIT_THROWING_KNIFE: 0;
		}

		else if(strcmp(name, "kill") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_KILL : 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_KILL_GUNFIRE: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_KILL_BLADE: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_KILL_HTH: 0;
		}
		else if(strcmp(name, "kill_gunfire") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_KILL_GUNFIRE: 0;
		}
		else if(strcmp(name, "kill_blade") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_KILL_BLADE: 0;
		}
		else if(strcmp(name, "kill_hth") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_KILL_HTH: 0;
		}
		else if(strcmp(name, "kill_throwing_knife") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_KILL_THROWING_KNIFE: 0;
		}
		else if(strcmp(name, "head_pop") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_HEAD_POP : 0;
		}

		else if(strcmp(name, "miss") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_MISS: 0;
		}
		else if(strcmp(name, "miss_gunfire") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_MISS_GUNFIRE: 0;
		}
		else if(strcmp(name, "miss_blade") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_MISS_BLADE: 0;
		}
		else if(strcmp(name, "miss_hth") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_MISS_HTH: 0;
		}
		else if(strcmp(name, "miss_throwing_knife") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_S_MISS_THROWING_KNIFE: 0;
		}

		if(strcmp(name, "riposte_quote") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_RIPOSTE_QUOTE] = (INT16) atol(pData->szCharData);
		}

		if(strcmp(name, "enemy") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_ADMIN: 0;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_ARMY: 0;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_ELITE: 0;
		}
		else if(strcmp(name, "admin") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_ADMIN: 0;
		}
		else if(strcmp(name, "army") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_ARMY: 0;
		}
		else if(strcmp(name, "elite") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_ELITE: 0;
		}
		else if(strcmp(name, "militia") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_GREEN: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_REGULAR: 0;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_VETERAN: 0;
		}
		else if(strcmp(name, "green") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_GREEN: 0;
		}
		else if(strcmp(name, "regular") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_REGULAR: 0;
		}
		else if(strcmp(name, "veteran") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_C_VETERAN: 0;
		}
		else if(strcmp(name, "admin_profile") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_PROFILE_ADMIN] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "army_profile") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_PROFILE_ARMY] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "elite_profile") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_PROFILE_ELITE] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "green_profile") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_PROFILE_GREEN] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "regular_profile") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_PROFILE_REGULAR] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "veteran_profile") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_PROFILE_VETERAN] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "male") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_G_MALE: 0;
		}
		else if(strcmp(name, "female") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_G_FEMALE: 0;
		}
		else if(strcmp(name, "exp_level_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_EXP_LEVEL_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "exp_level_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_EXP_LEVEL_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "health_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_HEALTH_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "health_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_HEALTH_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "health_max_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_HEALTH_MAX_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "health_max_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_HEALTH_MAX_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "energy_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_ENERGY_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "energy_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_ENERGY_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "energy_max_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_ENERGY_MAX_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "energy_max_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_ENERGY_MAX_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "morale_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_MORALE_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "morale_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_MORALE_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "progress_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_MORALE_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "progress_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_MORALE_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "fact_true") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_FACT_TRUE] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "fact_false") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_FACT_FALSE] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "veteran_profile") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_PROFILE_VETERAN] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "veteran_profile") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_PROFILE_VETERAN] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_type") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_TYPE] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_male") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_T_MALE: 0;
		}
		else if(strcmp(name, "target_female") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2	|= (UINT16) atol(pData->szCharData) ? TAUNT_T_FEMALE: 0;
		}
		else if(strcmp(name, "target_exp_level_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_EXP_LEVEL_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_exp_level_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_EXP_LEVEL_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_health_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_HEALTH_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_health_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_HEALTH_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_health_max_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_HEALTH_MAX_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_health_max_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_HEALTH_MAX_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_energy_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_ENERGY_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_energy_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_ENERGY_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_energy_max_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_ENERGY_MAX_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_energy_max_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_ENERGY_MAX_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_morale_gt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_MORALE_GT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_morale_lt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_MORALE_LT] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_appearance") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_APPEARANCE] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_merc_profile") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.value[TAUNT_TARGET_MERC_PROFILE] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "target_zombie") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curTaunt.uiFlags2 |= (UINT16) atol(pData->szCharData) ? TAUNT_T_ZOMBIE: 0;
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInTaunts(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	tauntParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EnemyTaunts.xml" );

	localizedTextOnly_Taunts = localizedVersion;
		
	// Open file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( FALSE );

	uiFSize = FileGetSize(hFile);
	lpcBuffer = (CHAR8 *) MemAlloc(uiFSize+1);

	//Read in block
	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) )
	{
		MemFree(lpcBuffer);
		return( FALSE );
	}

	lpcBuffer[uiFSize] = 0; //add a null terminator

	FileClose( hFile );


	XML_SetElementHandler(parser, tauntStartElementHandle, tauntEndElementHandle);
	XML_SetCharacterDataHandler(parser, tauntCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = &(zTaunt[num_found_taunt]);
	pData.maxArraySize = NUM_TAUNT;

	XML_SetUserData(parser, &pData);
	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in EnemyTaunts.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}


BOOLEAN WriteTaunts( STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<TAUNTS>\r\n");
		for(cnt = 0; cnt < NUM_TAUNT; ++cnt)
		{
			FilePrintf(hFile,"\t<TAUNT>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",				cnt);
			
			FilePrintf(hFile,"\t</TAUNT>\r\n");
		}
		FilePrintf(hFile,"</TAUNTS>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
