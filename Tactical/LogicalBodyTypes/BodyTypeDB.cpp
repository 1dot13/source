#include "BodyTypeDB.h"

namespace LogicalBodyTypes {

	BodyTypeDB::BodyTypeDB(void) : AbstractXMLLoader(StartElementHandle, EndElementHandle, CharacterDataHandle, MakeParseData) {
		RegisterEnumerators();
	}

	BodyTypeDB::~BodyTypeDB(void) {
		for (BodyTypeList::iterator i = bodyTypes.begin(); i != bodyTypes.end(); i++) {
			delete *i;
		}
	}

	void BodyTypeDB::Add(BodyType* bodyType) {
		bodyTypes.push_back(bodyType);
	}

	AbstractXMLLoader::ParseData* BodyTypeDB::MakeParseData(XML_Parser* parser) {
		return new ParseData(parser);
	}

	void BodyTypeDB::StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts) {
		ParseData* data = (ParseData*)userData;
		switch (data->state) {
		case E_NONE:
			if (strcmp(name, "LogicalBodyTypes") == 0) {
				data->state = E_ELEMENT_BODYTYPES;
				break;
			}
		case E_ELEMENT_BODYTYPES:
			if (strcmp(name, "LogicalBodyType") == 0) {
				data->state = E_ELEMENT_BODYTYPE;
				XML_Char const* filterIdent = GetAttribute("filter", atts);
				XML_Char const* cacheSize = GetAttribute("cachesize", atts);
				Filter* f = NULL;
				if (filterIdent != NULL && strcmp(filterIdent, "") != 0) {
					f = FilterDB::Instance().FindFilter(filterIdent);
					if (f == NULL) throw XMLParseException("Unknown filter specified!", name, data->pParser);
				}
				UINT32 size = 16;
				if (cacheSize != NULL) {
					if (!ConvertStringToUINT32(cacheSize, &size)) throw XMLParseException("Attribute 'value' doesn't have a valid value!", name, data->pParser);
					if (size < 2) throw XMLParseException("Minimum cache size is 2!", name, data->pParser);
				}
				data->currentBodyType = new BodyType(size, f);
				data->layerPropertiesConfig.clear();
				break;
			}
		case E_ELEMENT_BODYTYPE:
			if (strcmp(name, "LayerConfigurations") == 0) {
				data->state = E_ELEMENT_LAYER_CONFIGS;
				break;
			}
			if (strcmp(name, "LogicalAnimationSurfaces") == 0) {
				data->state = E_ELEMENT_SURFACES;
				break;
			}
		case E_ELEMENT_LAYER_CONFIGS:
			if (strcmp(name, "LayerConfiguration") == 0) {
				data->state = E_ELEMENT_LAYER_CONFIG;
				XML_Char const* aName = GetAttribute("name", atts);
				XML_Char const* aRender = GetAttribute("render", atts);
				XML_Char const* aShadow = GetAttribute("shadow", atts);
				if (aName == NULL) {
					throw XMLParseException("Mandatory attribute missing!", name, data->pParser);
				}
				std::string strLayer = aName;
				Layers::LayerPropertiesVector::size_type layer = Layers::Instance().GetIndex(strLayer);
				if (layer == -1) throw XMLParseException("Unknown layer specified!", name, data->pParser);
				Layers::LayerProperties temp = Layers::Instance().GetLayerProperties(layer); // copy default configuration for layer
				if (aRender != NULL) {
					BOOLEAN fRender;
					if (!ConvertStringToBOOLEAN(aRender, &fRender)) throw XMLParseException("Attribute 'value' doesn't have a valid value!", name, data->pParser);
					temp.render = fRender == TRUE;

				}
				if (aShadow != NULL) {
					BOOLEAN fShadow;
					if (!ConvertStringToBOOLEAN(aShadow, &fShadow)) throw XMLParseException("Attribute 'value' doesn't have a valid value!", name, data->pParser);
					temp.renderShadows = fShadow == TRUE;
				}
				data->layerPropertiesConfig[layer] = temp;
				break;
			}
		case E_ELEMENT_SURFACES:
			if (strcmp(name, "Layer") == 0) {
				data->state = E_ELEMENT_LAYER;
				XML_Char const* aLayer = GetAttribute("name", atts);
				if (aLayer == NULL) {
					throw XMLParseException("Mandatory attribute missing!", name, data->pParser);
				}
				std::string strLayer = aLayer;
				data->layer = Layers::Instance().GetIndex(strLayer);
				if (data->layer == -1) throw XMLParseException("Unknown layer specified!", name, data->pParser);
				break;
			}
		case E_ELEMENT_LAYER:
			if (strcmp(name, "LayerProp") == 0) {
				data->state = E_ELEMENT_PROP;
				XML_Char const* aFilter = GetAttribute("filter", atts);
				XML_Char const* aPalette = GetAttribute("palette", atts);
				data->filter = NULL;
				data->paletteTable = NULL;
				if (aFilter != NULL && strcmp(aFilter, "") != 0) {
					data->filter = FilterDB::Instance().FindFilter(aFilter);
					if (data->filter == NULL) throw XMLParseException("Unknown filter specified!", name, data->pParser);
				}
				if (aPalette != NULL && strcmp(aFilter, "") != 0 && strcmp(aFilter, "default") != 0) {
					data->paletteTable = PaletteDB::Instance().FindPaletteTable(aPalette);
					if (data->paletteTable == NULL) throw XMLParseException("Unknown palette specified!", name, data->pParser);
				}
				break;
			}
		case E_ELEMENT_PROP:
			if (strcmp(name, "Surface") == 0) {
				data->state = E_ELEMENT_SURFACE;
				XML_Char const* aSurface = GetAttribute("name", atts);
				XML_Char const* aAnimState = GetAttribute("animstate", atts);
				XML_Char const* aPhysSurface = GetAttribute("animsurface", atts);
				if (aSurface == NULL || (aAnimState == NULL && aPhysSurface == NULL)) {
					throw XMLParseException("Mandatory attribute missing!", name, data->pParser);
				}
				if (aAnimState == NULL && aPhysSurface == NULL) {
					// we could allow this
					// but we should than make 2 seperate animation surface type instances (that are equivalent) which is not nice
					// we could use the same instance, but it has to bee ascertained that the cache can handle it first!
					throw XMLParseException("Only one attribute of animstate and animsurface may be specified!!", name, data->pParser);
				}
				AnimationSurfaceType* surface = SurfaceDB::Instance().GetAnimSurfaceType(aSurface);
				std::string nameAlpha = aSurface;
				nameAlpha += "###ALPHA";
				AnimationSurfaceType* surfaceAlpha = SurfaceDB::Instance().GetAnimSurfaceType(nameAlpha.c_str());
				if (surface == NULL) throw XMLParseException("Unknown animation surface specified!", name, data->pParser);
				if (aAnimState != NULL) {
					int iAnimState;
					if (!EnumeratorDB::Instance().GetEnumeratorByStr("AnimationStates", aAnimState, iAnimState)) {
						throw XMLParseException("Unknown animation state specified!", name, data->pParser);
					}
					data->currentBodyType->AddSurfaceForAnimState(iAnimState, data->layer, surface, surfaceAlpha, data->filter, data->paletteTable);
				}
				if (aPhysSurface != NULL) {
					int iAnimSurface;
					if (!EnumeratorDB::Instance().GetEnumeratorByStr("AnimationSurfaces", aPhysSurface, iAnimSurface)) {
						throw XMLParseException("Unknown animation surface specified!", name, data->pParser);
					}
					data->currentBodyType->AddSurfaceForPhysSurface(iAnimSurface, data->layer, surface, surfaceAlpha, data->filter, data->paletteTable);
				}
				break;
			}
		default:
			throw XMLParseException("Unexpected element!", name, data->pParser);
		}
	}

	void BodyTypeDB::EndElementHandle(void* userData, const XML_Char* name) {
		ParseData* data = (ParseData*)userData;
		switch (data->state) {
		case E_ELEMENT_SURFACE:
			if (strcmp(name, "Surface") == 0) {
				data->state = E_ELEMENT_PROP;
				break;
			}
		case E_ELEMENT_PROP:
			if (strcmp(name, "LayerProp") == 0) {
				data->state = E_ELEMENT_LAYER;
				break;
			}
		case E_ELEMENT_LAYER:
			if (strcmp(name, "Layer") == 0) {
				data->state = E_ELEMENT_SURFACES;
				break;
			}
		case E_ELEMENT_SURFACES:
			if (strcmp(name, "LogicalAnimationSurfaces") == 0) {
				data->state = E_ELEMENT_BODYTYPE;
				break;
			}
		case E_ELEMENT_LAYER_CONFIG:
			if (strcmp(name, "LayerConfiguration") == 0) {
				data->state = E_ELEMENT_LAYER_CONFIGS;
				break;
			}
		case E_ELEMENT_LAYER_CONFIGS:
			if (strcmp(name, "LayerConfigurations") == 0) {
				data->state = E_ELEMENT_BODYTYPE;
				break;
			}
		case E_ELEMENT_BODYTYPE:
			if (strcmp(name, "LogicalBodyType") == 0) {
				data->state = E_ELEMENT_BODYTYPES;
				std::map<Layers::LayerPropertiesVector::size_type, Layers::LayerProperties>::iterator i = data->layerPropertiesConfig.begin();
				for (; i != data->layerPropertiesConfig.end(); i++) {
					data->currentBodyType->SetLayerProperties(i->first, i->second);
				}
				BodyTypeDB::Instance().Add(data->currentBodyType);
				data->currentBodyType = NULL; // in case an exception is raised between here and the start of the next bodytype element
				break;
			}
		case E_ELEMENT_BODYTYPES:
			if (strcmp(name, "LogicalBodyTypes") == 0) {
				data->state = E_NONE;
				break;
			}
		default:
			throw XMLParseException("Unexpected element! Closing tag missing?", name, data->pParser);
			return;
		}
	}

	void BodyTypeDB::CharacterDataHandle(void* userData, const XML_Char* str, int len) {
	}

	void BodyTypeDB::RegisterEnumerators() {

		/*****************************************
		  Animation states
		******************************************/
		LOGBT_ENUMDB_ADD("AnimationStates", NUMANIMATIONSTATES,
			WALKING,
			STANDING,
			KNEEL_DOWN,
			CROUCHING,
			KNEEL_UP,
			SWATTING,
			RUNNING,
			PRONE_DOWN,
			CRAWLING,
			PRONE_UP,
			PRONE,
			READY_RIFLE_STAND,
			AIM_RIFLE_STAND,
			SHOOT_RIFLE_STAND,
			END_RIFLE_STAND,
			START_SWAT,
			END_SWAT,
			FLYBACK_HIT,
			READY_RIFLE_PRONE,
			AIM_RIFLE_PRONE,
			SHOOT_RIFLE_PRONE,
			END_RIFLE_PRONE,
			FALLBACK_DEATHTWICH,
			GENERIC_HIT_STAND,
			FLYBACK_HIT_BLOOD_STAND,
			FLYBACK_HIT_DEATH,
			READY_RIFLE_CROUCH,
			AIM_RIFLE_CROUCH,
			SHOOT_RIFLE_CROUCH,
			END_RIFLE_CROUCH,
			FALLBACK_HIT_STAND,
			ROLLOVER,
			CLIMBUPROOF,
			FALLOFF,
			GETUP_FROM_ROLLOVER,
			CLIMBDOWNROOF,
			FALLFORWARD_ROOF,
			GENERIC_HIT_DEATHTWITCHNB,
			GENERIC_HIT_DEATHTWITCHB,
			FALLBACK_HIT_DEATHTWITCHNB,
			FALLBACK_HIT_DEATHTWITCHB,
			GENERIC_HIT_DEATH,
			FALLBACK_HIT_DEATH,
			GENERIC_HIT_CROUCH,
			STANDING_BURST,
			STANDING_BURST_HIT,
			FALLFORWARD_FROMHIT_STAND,
			FALLFORWARD_FROMHIT_CROUCH,
			ENDFALLFORWARD_FROMHIT_CROUCH,
			GENERIC_HIT_PRONE,
			PRONE_HIT_DEATH,
			PRONE_LAY_FROMHIT,
			PRONE_HIT_DEATHTWITCHNB,
			PRONE_HIT_DEATHTWITCHB,
			ADULTMONSTER_BREATHING,
			ADULTMONSTER_WALKING,
			ADULTMONSTER_ATTACKING,
			FLYBACK_HITDEATH_STOP,
			FALLFORWARD_HITDEATH_STOP,
			FALLBACK_HITDEATH_STOP,
			PRONE_HITDEATH_STOP,
			FLYBACKHIT_STOP,
			FALLBACKHIT_STOP,
			FALLOFF_STOP,
			FALLOFF_FORWARD_STOP,
			STAND_FALLFORWARD_STOP,
			PRONE_LAYFROMHIT_STOP,
			HOPFENCE,
			ADULTMONSTER_HIT,
			ADULTMONSTER_DYING,
			ADULTMONSTER_DYING_STOP,
			PUNCH_BREATH,
			PUNCH,
			NOTHING_STAND,
			JFK_HITDEATH,
			JFK_HITDEATH_STOP,
			JFK_HITDEATH_TWITCHB,
			FIRE_STAND_BURST_SPREAD,
			FALLOFF_DEATH,
			FALLOFF_DEATH_STOP,
			FALLOFF_TWITCHB,
			FALLOFF_TWITCHNB,
			FALLOFF_FORWARD_DEATH,
			FALLOFF_FORWARD_DEATH_STOP,
			FALLOFF_FORWARD_TWITCHB,
			FALLOFF_FORWARD_TWITCHNB,
			OPEN_DOOR,
			OPEN_STRUCT,
			PICKUP_ITEM,
			DROP_ITEM,
			SLICE,
			STAB,
			CROUCH_STAB,
			START_AID,
			GIVING_AID,
			END_AID,
			DODGE_ONE,
			FATCIV_ASS_SCRATCH,
			READY_DUAL_STAND,
			AIM_DUAL_STAND,
			SHOOT_DUAL_STAND,
			END_DUAL_STAND,
			RAISE_RIFLE,
			LOWER_RIFLE,
			BODYEXPLODING,
			THROW_ITEM,
			LOB_ITEM,
			QUEEN_MONSTER_BREATHING,
			CROUCHED_BURST,
			PRONE_BURST,
			NOTUSEDANIM1,
			BIGBUY_FLEX,
			BIGBUY_STRECH,
			BIGBUY_SHOEDUST,
			BIGBUY_HEADTURN,
			MINIGIRL_STOCKING,
			GIVE_ITEM,
			CLIMB_CLIFF,
			COW_EATING,
			COW_HIT,
			COW_DYING,
			COW_DYING_STOP,
			WATER_HIT,
			WATER_DIE,
			WATER_DIE_STOP,
			CROW_WALK,
			CROW_TAKEOFF,
			CROW_LAND,
			CROW_FLY,
			CROW_EAT,
			HELIDROP,
			FEM_CLEAN,
			FEM_KICKSN,
			FEM_LOOK,
			FEM_WIPE,
			REG_SQUISH,
			REG_PULL,
			REG_SPIT,
			HATKID_YOYO,
			KID_ARMPIT,
			MONSTER_CLOSE_ATTACK,
			MONSTER_SPIT_ATTACK,
			MONSTER_BEGIN_EATTING_FLESH,
			MONSTER_EATTING_FLESH,
			MONSTER_END_EATTING_FLESH,
			BLOODCAT_RUN,
			BLOODCAT_STARTREADY,
			BLOODCAT_READY,
			BLOODCAT_ENDREADY,
			BLOODCAT_HIT,
			BLOODCAT_DYING,
			BLOODCAT_DYING_STOP,
			BLOODCAT_SWIPE,
			NINJA_GOTOBREATH,
			NINJA_BREATH,
			NINJA_LOWKICK,
			NINJA_PUNCH,
			NINJA_SPINKICK,
			END_OPEN_DOOR,
			END_OPEN_LOCKED_DOOR,
			KICK_DOOR,
			CLOSE_DOOR,
			RIFLE_STAND_HIT,
			DEEP_WATER_TRED,
			DEEP_WATER_SWIM,
			DEEP_WATER_HIT,
			DEEP_WATER_DIE,
			DEEP_WATER_DIE_STOPPING,
			DEEP_WATER_DIE_STOP,
			LOW_TO_DEEP_WATER,
			DEEP_TO_LOW_WATER,
			GOTO_SLEEP,
			SLEEPING,
			WKAEUP_FROM_SLEEP,
			FIRE_LOW_STAND,
			FIRE_BURST_LOW_STAND,
			LARVAE_BREATH,
			LARVAE_HIT,
			LARVAE_DIE,
			LARVAE_DIE_STOP,
			LARVAE_WALK,
			INFANT_HIT,
			INFANT_DIE,
			INFANT_DIE_STOP,
			INFANT_ATTACK,
			INFANT_BEGIN_EATTING_FLESH,
			INFANT_EATTING_FLESH,
			INFANT_END_EATTING_FLESH,
			MONSTER_UP,
			MONSTER_JUMP,
			STANDING_SHOOT_UNJAM,
			CROUCH_SHOOT_UNJAM,
			PRONE_SHOOT_UNJAM,
			STANDING_SHOOT_DWEL_UNJAM,
			STANDING_SHOOT_LOW_UNJAM,
			READY_DUAL_CROUCH,
			AIM_DUAL_CROUCH,
			SHOOT_DUAL_CROUCH,
			END_DUAL_CROUCH,
			CROUCH_SHOOT_DWEL_UNJAM,
			ADJACENT_GET_ITEM,
			CUTTING_FENCE,
			CRIPPLE_BEG,
			CRIPPLE_HIT,
			CRIPPLE_DIE,
			CRIPPLE_DIE_STOP,
			CRIPPLE_DIE_FLYBACK,
			CRIPPLE_DIE_FLYBACK_STOP,
			CRIPPLE_KICKOUT,
			FROM_INJURED_TRANSITION,
			THROW_KNIFE,
			KNIFE_BREATH,
			KNIFE_GOTOBREATH,
			KNIFE_ENDBREATH,
			ROBOTNW_HIT,
			ROBOTNW_DIE,
			ROBOTNW_DIE_STOP,
			CATCH_STANDING,
			CATCH_CROUCHED,
			PLANT_BOMB,
			USE_REMOTE,
			START_COWER,
			COWERING,
			END_COWER,
			STEAL_ITEM,
			SHOOT_ROCKET,
			CIV_DIE2,
			SHOOT_MORTAR,
			CROW_DIE,
			SIDE_STEP,
			WALK_BACKWARDS,
			BEGIN_OPENSTRUCT,
			END_OPENSTRUCT,
			END_OPENSTRUCT_LOCKED,
			PUNCH_LOW,
			PISTOL_SHOOT_LOW,
			DECAPITATE,
			BLOODCAT_BITE_ANIM,
			BIGMERC_IDLE_NECK,
			BIGMERC_CROUCH_TRANS_INTO,
			BIGMERC_CROUCH_TRANS_OUTOF,
			GOTO_PATIENT,
			BEING_PATIENT,
			GOTO_DOCTOR,
			BEING_DOCTOR,
			END_DOCTOR,
			GOTO_REPAIRMAN,
			BEING_REPAIRMAN,
			END_REPAIRMAN,
			FALL_INTO_PIT,
			ROBOT_WALK,
			ROBOT_SHOOT,
			QUEEN_HIT,
			QUEEN_DIE,
			QUEEN_DIE_STOP,
			QUEEN_INTO_READY,
			QUEEN_READY,
			QUEEN_END_READY,
			QUEEN_CALL,
			QUEEN_SPIT,
			QUEEN_SWIPE,
			RELOAD_ROBOT,
			END_CATCH,
			END_CROUCH_CATCH,
			AI_RADIO,
			AI_CR_RADIO,
			TANK_SHOOT,
			TANK_BURST,
			QUEEN_SLAP,
			SLAP_HIT,
			TAKE_BLOOD_FROM_CORPSE,
			VEHICLE_DIE,
			QUEEN_FRUSTRATED_SLAP,
			CHARIOTS_OF_FIRE,
			AI_PULL_SWITCH,
			MONSTER_MELT,
			MERC_HURT_IDLE_ANIM,
			END_HURT_WALKING,
			PASS_OBJECT,
			DROP_ADJACENT_OBJECT,
			READY_DUAL_PRONE,
			AIM_DUAL_PRONE,
			SHOOT_DUAL_PRONE,
			END_DUAL_PRONE,
			PRONE_SHOOT_DWEL_UNJAM,
			PICK_LOCK,
			OPEN_DOOR_CROUCHED,
			BEGIN_OPENSTRUCT_CROUCHED,
			CLOSE_DOOR_CROUCHED,
			OPEN_STRUCT_CROUCHED,
			END_OPEN_DOOR_CROUCHED,
			END_OPENSTRUCT_CROUCHED,
			END_OPEN_LOCKED_DOOR_CROUCHED,
			END_OPENSTRUCT_LOCKED_CROUCHED,
			DRUNK_IDLE,
			CROWBAR_ATTACK,
			CIV_COWER_HIT,
			BLOODCAT_WALK_BACKWARDS,
			MONSTER_WALK_BACKWARDS,
			KID_SKIPPING,
			ROBOT_BURST_SHOOT,
			ATTACH_CAN_TO_STRING,
			SWAT_BACKWARDS,
			JUMP_OVER_BLOCKING_PERSON,
			REFUEL_VEHICLE,
			ROBOT_CAMERA_NOT_MOVING,
			CRIPPLE_OPEN_DOOR,
			CRIPPLE_CLOSE_DOOR,
			CRIPPLE_END_OPEN_DOOR,
			CRIPPLE_END_OPEN_LOCKED_DOOR,
			LOCKPICK_CROUCHED,
			THROW_ITEM_CROUCHED,
			BIGGUY_STONE,
			SHOOT_ROCKET_CROUCHED,
			SWATTING_WK,
			SWAT_BACKWARDS_WK,
			SWAT_BACKWARDS_NOTHING,
			THROW_GRENADE_STANCE,
			LOB_GRENADE_STANCE,
			ROLL_PRONE_R,
			ROLL_PRONE_L,
			JUMPUPWALL,
			JUMPDOWNWALL,
			JUMPWINDOWS,
			THROW_KNIFE_SP_BM,
			RUNNING_W_PISTOL,
			SIDE_STEP_WEAPON_RDY,
			SIDE_STEP_DUAL_RDY,
			SIDE_STEP_CROUCH_RIFLE,
			SIDE_STEP_CROUCH_PISTOL,
			SIDE_STEP_CROUCH_DUAL,
			WALKING_WEAPON_RDY,
			WALKING_DUAL_RDY,
			CROUCHEDMOVE_RIFLE_READY,
			CROUCHEDMOVE_PISTOL_READY,
			CROUCHEDMOVE_DUAL_READY,
			START_AID_PRN,
			GIVING_AID_PRN,
			END_AID_PRN,
			ADJACENT_GET_ITEM_CROUCHED,
			STEAL_ITEM_CROUCHED,
			BURST_DUAL_STAND,
			BURST_DUAL_CROUCH,
			BURST_DUAL_PRONE,
			READY_ALTERNATIVE_STAND,
			AIM_ALTERNATIVE_STAND,
			SHOOT_ALTERNATIVE_STAND,
			BURST_ALTERNATIVE_STAND,
			LOW_SHOT_ALTERNATIVE_STAND,
			LOW_BURST_ALTERNATIVE_STAND,
			UNREADY_ALTERNATIVE_STAND,
			UNJAM_ALTERNATIVE_STAND,
			LOW_UNJAM_ALTERNATIVE_STAND,
			WALKING_ALTERNATIVE_RDY,
			SIDE_STEP_ALTERNATIVE_RDY,
			START_COWER_CROUCHED,
			END_COWER_CROUCHED,
			START_COWER_PRONE,
			COWERING_PRONE,
			END_COWER_PRONE,
			BIGMERC_IDLE_NECK2,
			FOCUSED_PUNCH,
			FOCUSED_STAB,
			HTH_KICK,
			FOCUSED_HTH_KICK,
			LONG_JUMP,
			CRYO_DEATH,
			CRYO_DEATH_CROUCHED,
			BAYONET_STAB_STANDING_VS_STANDING,
			BAYONET_STAB_STANDING_VS_PRONE
		);

		/*****************************************
		  Animation states
		******************************************/
		LOGBT_ENUMDB_ADD("AnimationSurfaces", NUMANIMATIONSURFACETYPES,
			RGMBASICWALKING,
			RGMSTANDING,
			RGMCROUCHING,
			RGMSNEAKING,
			RGMRUNNING,
			RGMPRONE,
			RGMSTANDAIM,
			RGMHITHARD,
			RGMHITSTAND,
			RGMHITHARDBLOOD,
			RGMCROUCHAIM,
			RGMHITFALLBACK,
			RGMROLLOVER,
			RGMCLIMBROOF,
			RGMFALL,
			RGMFALLF,
			RGMHITCROUCH,
			RGMHITPRONE,
			RGMHOPFENCE,
			RGMPUNCH,
			RGMNOTHING_STD,
			RGMNOTHING_WALK,
			RGMNOTHING_RUN,
			RGMNOTHING_SWAT,
			RGMNOTHING_CROUCH,
			RGMHANDGUN_S_SHOT,
			RGMHANDGUN_C_SHOT,
			RGMHANDGUN_PRONE,
			RGMDIE_JFK,
			RGMOPEN,
			RGMPICKUP,
			RGMSTAB,
			RGMSLICE,
			RGMCSTAB,
			RGMMEDIC,
			RGMDODGE,
			RGMSTANDDWALAIM,
			RGMRAISE,
			RGMTHROW,
			RGMLOB,
			RGMKICKDOOR,
			RGMRHIT,
			RGM_SQUISH,
			RGM_LOOK,
			RGM_PULL,
			RGM_SPIT,
			RGMWATER_R_WALK,
			RGMWATER_R_STD,
			RGMWATER_N_WALK,
			RGMWATER_N_STD,
			RGMWATER_DIE,
			RGMWATER_N_AIM,
			RGMWATER_R_AIM,
			RGMWATER_DBLSHT,
			RGMWATER_TRANS,
			RGMDEEPWATER_TRED,
			RGMDEEPWATER_SWIM,
			RGMDEEPWATER_DIE,
			RGMMCLIMB,
			RGMHELIDROP,
			RGMLOWKICK,
			RGMNPUNCH,
			RGMSPINKICK,
			RGMSLEEP,
			RGMSHOOT_LOW,
			RGMCDBLSHOT,
			RGMHURTSTANDINGN,
			RGMHURTSTANDINGR,
			RGMHURTWALKINGN,
			RGMHURTWALKINGR,
			RGMHURTTRANS,
			RGMTHROWKNIFE,
			RGMBREATHKNIFE,
			RGMPISTOLBREATH,
			RGMCOWER,
			RGMROCKET,
			RGMMORTAR,
			RGMSIDESTEP,
			RGMDBLBREATH,
			RGMPUNCHLOW,
			RGMPISTOLSHOOTLOW,
			RGMWATERTHROW,
			RGMRADIO,
			RGMCRRADIO,
			RGMBURN,
			RGMDWPRONE,
			RGMDRUNK,
			RGMPISTOLDRUNK,
			RGMCROWBAR,
			RGMJUMPOVER,
			RGMBAYONET_S_S,
			RGMBAYONET_S_P,
			BGMWALKING,
			BGMSTANDING,
			BGMCROUCHING,
			BGMSNEAKING,
			BGMRUNNING,
			BGMPRONE,
			BGMSTANDAIM,
			BGMHITHARD,
			BGMHITSTAND,
			BGMHITHARDBLOOD,
			BGMCROUCHAIM,
			BGMHITFALLBACK,
			BGMROLLOVER,
			BGMCLIMBROOF,
			BGMFALL,
			BGMFALLF,
			BGMHITCROUCH,
			BGMHITPRONE,
			BGMHOPFENCE,
			BGMPUNCH,
			BGMNOTHING_STD,
			BGMNOTHING_WALK,
			BGMNOTHING_RUN,
			BGMNOTHING_SWAT,
			BGMNOTHING_CROUCH,
			BGMHANDGUN_S_SHOT,
			BGMHANDGUN_C_SHOT,
			BGMHANDGUN_PRONE,
			BGMDIE_JFK,
			BGMOPEN,
			BGMPICKUP,
			BGMSTAB,
			BGMSLICE,
			BGMCSTAB,
			BGMMEDIC,
			BGMDODGE,
			BGMSTANDDWALAIM,
			BGMRAISE,
			BGMTHROW,
			BGMLOB,
			BGMKICKDOOR,
			BGMRHIT,
			BGMSTANDAIM2,
			BGMFLEX,
			BGMSTRECH,
			BGMSHOEDUST,
			BGMHEADTURN,
			BGMWATER_R_WALK,
			BGMWATER_R_STD,
			BGMWATER_N_WALK,
			BGMWATER_N_STD,
			BGMWATER_DIE,
			BGMWATER_N_AIM,
			BGMWATER_R_AIM,
			BGMWATER_DBLSHT,
			BGMWATER_TRANS,
			BGMDEEPWATER_TRED,
			BGMDEEPWATER_SWIM,
			BGMDEEPWATER_DIE,
			BGMHELIDROP,
			BGMSLEEP,
			BGMSHOOT_LOW,
			BGMTHREATENSTAND,
			BGMCDBLSHOT,
			BGMHURTSTANDINGN,
			BGMHURTSTANDINGR,
			BGMHURTWALKINGN,
			BGMHURTWALKINGR,
			BGMHURTTRANS,
			BGMTHROWKNIFE,
			BGMBREATHKNIFE,
			BGMPISTOLBREATH,
			BGMCOWER,
			BGMRAISE2,
			BGMROCKET,
			BGMMORTAR,
			BGMSIDESTEP,
			BGMDBLBREATH,
			BGMPUNCHLOW,
			BGMPISTOLSHOOTLOW,
			BGMWATERTHROW,
			BGMWALK2,
			BGMRUN2,
			BGMIDLENECK,
			BGMCROUCHTRANS,
			BGMRADIO,
			BGMCRRADIO,
			BGMDWPRONE,
			BGMDRUNK,
			BGMPISTOLDRUNK,
			BGMCROWBAR,
			BGMJUMPOVER,
			BGMBAYONET_S_S,
			BGMBAYONET_S_P,
			RGFWALKING,
			RGFSTANDING,
			RGFCROUCHING,
			RGFSNEAKING,
			RGFRUNNING,
			RGFPRONE,
			RGFSTANDAIM,
			RGFHITHARD,
			RGFHITSTAND,
			RGFHITHARDBLOOD,
			RGFCROUCHAIM,
			RGFHITFALLBACK,
			RGFROLLOVER,
			RGFCLIMBROOF,
			RGFFALL,
			RGFFALLF,
			RGFHITCROUCH,
			RGFHITPRONE,
			RGFHOPFENCE,
			RGFPUNCH,
			RGFNOTHING_STD,
			RGFNOTHING_WALK,
			RGFNOTHING_RUN,
			RGFNOTHING_SWAT,
			RGFNOTHING_CROUCH,
			RGFHANDGUN_S_SHOT,
			RGFHANDGUN_C_SHOT,
			RGFHANDGUN_PRONE,
			RGFDIE_JFK,
			RGFOPEN,
			RGFPICKUP,
			RGFSTAB,
			RGFSLICE,
			RGFCSTAB,
			RGFMEDIC,
			RGFDODGE,
			RGFSTANDDWALAIM,
			RGFRAISE,
			RGFTHROW,
			RGFLOB,
			RGFKICKDOOR,
			RGFRHIT,
			RGFCLEAN,
			RGFKICKSN,
			RGFALOOK,
			RGFWIPE,
			RGFWATER_R_WALK,
			RGFWATER_R_STD,
			RGFWATER_N_WALK,
			RGFWATER_N_STD,
			RGFWATER_DIE,
			RGFWATER_N_AIM,
			RGFWATER_R_AIM,
			RGFWATER_DBLSHT,
			RGFWATER_TRANS,
			RGFDEEPWATER_TRED,
			RGFDEEPWATER_SWIM,
			RGFDEEPWATER_DIE,
			RGFHELIDROP,
			RGFSLEEP,
			RGFSHOOT_LOW,
			RGFCDBLSHOT,
			RGFHURTSTANDINGN,
			RGFHURTSTANDINGR,
			RGFHURTWALKINGN,
			RGFHURTWALKINGR,
			RGFHURTTRANS,
			RGFTHROWKNIFE,
			RGFBREATHKNIFE,
			RGFPISTOLBREATH,
			RGFCOWER,
			RGFROCKET,
			RGFMORTAR,
			RGFSIDESTEP,
			RGFDBLBREATH,
			RGFPUNCHLOW,
			RGFPISTOLSHOOTLOW,
			RGFWATERTHROW,
			RGFRADIO,
			RGFCRRADIO,
			RGFSLAP,
			RGFDWPRONE,
			RGFDRUNK,
			RGFPISTOLDRUNK,
			RGFCROWBAR,
			RGFJUMPOVER,
			RGFBAYONET_S_S,
			RGFBAYONET_S_P,
			CRYO_EXPLODE,
			CRYO_EXPLODE_CROUCHED,
			AFMONSTERSTANDING,
			AFMONSTERWALKING,
			AFMONSTERATTACK,
			AFMONSTERCLOSEATTACK,
			AFMONSTERSPITATTACK,
			AFMONSTEREATING,
			AFMONSTERDIE,
			AFMUP,
			AFMJUMP,
			AFMMELT,
			LVBREATH,
			LVDIE,
			LVWALK,
			IBREATH,
			IWALK,
			IDIE,
			IEAT,
			IATTACK,
			QUEENMONSTERSTANDING,
			QUEENMONSTERREADY,
			QUEENMONSTERSPIT_SW,
			QUEENMONSTERSPIT_E,
			QUEENMONSTERSPIT_NE,
			QUEENMONSTERSPIT_S,
			QUEENMONSTERSPIT_SE,
			QUEENMONSTERDEATH,
			QUEENMONSTERSWIPE,
			FATMANSTANDING,
			FATMANWALKING,
			FATMANRUNNING,
			FATMANDIE,
			FATMANASS,
			FATMANACT,
			FATMANCOWER,
			FATMANDIE2,
			FATMANCOWERHIT,
			MANCIVSTANDING,
			MANCIVWALKING,
			MANCIVRUNNING,
			MANCIVDIE,
			MANCIVACT,
			MANCIVCOWER,
			MANCIVDIE2,
			MANCIVSMACKED,
			MANCIVPUNCH,
			MANCIVCOWERHIT,
			MINICIVSTANDING,
			MINICIVWALKING,
			MINICIVRUNNING,
			MINICIVDIE,
			MINISTOCKING,
			MINIACT,
			MINICOWER,
			MINIDIE2,
			MINICOWERHIT,
			DRESSCIVSTANDING,
			DRESSCIVWALKING,
			DRESSCIVRUNNING,
			DRESSCIVDIE,
			DRESSCIVACT,
			DRESSCIVCOWER,
			DRESSCIVDIE2,
			DRESSCIVCOWERHIT,
			HATKIDCIVSTANDING,
			HATKIDCIVWALKING,
			HATKIDCIVRUNNING,
			HATKIDCIVDIE,
			HATKIDCIVJFK,
			HATKIDCIVYOYO,
			HATKIDCIVACT,
			HATKIDCIVCOWER,
			HATKIDCIVDIE2,
			HATKIDCIVCOWERHIT,
			HATKIDCIVSKIP,
			KIDCIVSTANDING,
			KIDCIVWALKING,
			KIDCIVRUNNING,
			KIDCIVDIE,
			KIDCIVJFK,
			KIDCIVARMPIT,
			KIDCIVACT,
			KIDCIVCOWER,
			KIDCIVDIE2,
			KIDCIVCOWERHIT,
			KIDCIVSKIP,
			CRIPCIVSTANDING,
			CRIPCIVWALKING,
			CRIPCIVRUNNING,
			CRIPCIVBEG,
			CRIPCIVDIE,
			CRIPCIVDIE2,
			CRIPCIVKICK,
			COWSTANDING,
			COWWALKING,
			COWDIE,
			COWEAT,
			CROWWALKING,
			CROWFLYING,
			CROWEATING,
			CROWDYING,
			CATBREATH,
			CATWALK,
			CATRUN,
			CATREADY,
			CATHIT,
			CATDIE,
			CATSWIPE,
			CATBITE,
			ROBOTNWBREATH,
			ROBOTNWWALK,
			ROBOTNWHIT,
			ROBOTNWDIE,
			ROBOTNWSHOOT,
			HUMVEE_BASIC,
			HUMVEE_DIE,
			TANKNW_READY,
			TANKNW_SHOOT,
			TANKNW_DIE,
			TANKNE_READY,
			TANKNE_SHOOT,
			TANKNE_DIE,
			ELDORADO_BASIC,
			ELDORADO_DIE,
			ICECREAMTRUCK_BASIC,
			ICECREAMTRUCK_DIE,
			JEEP_BASIC,
			JEEP_DIE,
			ARMED_CAR_READY,
			ARMED_CAR_SHOOT,
			ARMED_CAR_DIE,
			BODYEXPLODE,
			RGMCRTHROW,
			BGMCRTHROW,
			RGFCRTHROW,
			BGMWITHSTONE,
			RGMCRROCKET,
			BGMCRROCKET,
			RGFCRROCKET,
			BGMSWKNIFE,
			RGFSWKNIFE,
			RGMSWKNIFE,
			BGMSTHRG,
			RGFSTHRG,
			RGMSTHRG,
			BGMSLOBG,
			RGFSLOBG,
			RGMSLOBG,
			RGMROLL_PR,
			BGMROLL_PR,
			RGFROLL_PR,
			RGMWALLJUMP,
			BGMWALLJUMP,
			RGFWALLJUMP,
			RGMJUMPWINDOWS,
			BGMJUMPWINDOWS,
			RGFJUMPWINDOWS,
			RGMUSEREMOTE,
			BGMUSEREMOTE,
			RGFUSEREMOTE,
			BGMTHROWKNIFE2,
			RGMPISTOL_RUN,
			BGMPISTOL_RUN,
			RGFPISTOL_RUN,
			RGMSIDESTEP_P_RDY,
			RGMSIDESTEP_R_RDY,
			RGMSIDESTEP_D_RDY,
			BGMSIDESTEP_P_RDY,
			BGMSIDESTEP_R_RDY,
			BGMSIDESTEP_R_RDY2,
			BGMSIDESTEP_D_RDY,
			RGFSIDESTEP_P_RDY,
			RGFSIDESTEP_R_RDY,
			RGFSIDESTEP_D_RDY,
			RGMSIDESTEP_CROUCH_R_RDY,
			RGMSIDESTEP_CROUCH_P_RDY,
			RGMSIDESTEP_CROUCH_D_RDY,
			BGMSIDESTEP_CROUCH_R_RDY,
			BGMSIDESTEP_CROUCH_P_RDY,
			BGMSIDESTEP_CROUCH_D_RDY,
			RGFSIDESTEP_CROUCH_R_RDY,
			RGFSIDESTEP_CROUCH_P_RDY,
			RGFSIDESTEP_CROUCH_D_RDY,
			RGMWALK_P_RDY,
			RGMWALK_R_RDY,
			RGMWALK_D_RDY,
			RGMCROUCH_R_RDY,
			RGMCROUCH_P_RDY,
			RGMCROUCH_D_RDY,
			BGMWALK_P_RDY,
			BGMWALK_R_RDY,
			BGMWALK_R_RDY2,
			BGMWALK_D_RDY,
			BGMCROUCH_R_RDY,
			BGMCROUCH_P_RDY,
			BGMCROUCH_D_RDY,
			RGFWALK_P_RDY,
			RGFWALK_R_RDY,
			RGFWALK_D_RDY,
			RGFCROUCH_R_RDY,
			RGFCROUCH_P_RDY,
			RGFCROUCH_D_RDY,
			RGMMEDICPRN,
			BGMMEDICPRN,
			RGFMEDICPRN,
			RGM_HIP_AIM,
			RGM_HIP_LOW,
			BGM_HIP_AIM,
			RGF_HIP_AIM,
			RGM_WATER_HIP_AIM,
			BGM_WATER_HIP_AIM,
			RGF_WATER_HIP_AIM,
			RGM_COWER_PRONE,
			BGM_COWER_PRONE,
			RGF_COWER_PRONE,
			BGMIDLENECK2,
			BGMHIPAIMALT,
			BGMSRAIMALT,
			RGM_PFSHOT_AIM,
			BGM_PFSHOT_AIM,
			RGF_PFSHOT_AIM
		);

	};

}
