#include "UtilityAI.h"
#include "UtilityAI_ResponseCurve.h"
#include "Overhead Types.h"
#include "Soldier Control.h"
#include "Isometric Utils.h"
#include "soldier profile type.h"
#include "AIInternals.h"
#include "Render Fun.h"

namespace UtilityAI
{

    typedef enum DecisionInput
    {
        MyHealth,
        MyStamina,
        MyStance,
        MyMorale,
        MyCurrentWeaponRange,
        DistanceFromMe,
        HealthOfTarget,
        AllyCount,
        EnemyCount,
        AllyEnemyRatio,
        EnemiesNearTarget,
        AlliesNearTarget,
        HaveGasmaskInInventory,
        HaveNVGInInventory,
        HasKnife,
        HasThrowingKnife,
        HasSmokeGrenade,
        IamInSmoke,
        IamInTearGas,
        IamInMustardGas, // Also fire & creature gas
        NearSmoke,
        NearTearGas,
        NearMustardGas,
        InShock,
        InsideRoom,
        NightTime,
        InLightAtNight,
        TakenLargeHit,
        HasTrait_AutoWeapons,
        HasTrait_HeavyWeapons,
        HasTrait_Sniper,
        HasTrait_Ranger,
        HasTrait_Gunslinger,
        HasTrait_MartialArts,
        HasTrait_SquadLeader,
        HasTrait_Technician,
        HasTrait_Doctor,
        HasTrait_Ambidextrous,
        HasTrait_Melee,
        HasTrait_Throwing,
        HasTrait_NightOps,
        HasTrait_Stealthy,
        HasTrait_Athletics,
        HasTrait_Bodybuilding,
        HasTrait_Demolitions,
        HasTrait_Teaching,
        HasTrait_Scouting,
        HasTrait_Covert,
        HasTrait_RadioOperator,
        HasTrait_Snitch,
        HasTrait_Survival,
        HasTraitOld_Lockpicking,
        HasTraitOld_HandToHand,
        HasTraitOld_Electronics,
        HasTraitOld_NightOps,
        HasTraitOld_Throwing,
        HasTraitOld_Teaching,
        HasTraitOld_HeavyWeapons,
        HasTraitOld_AutoWeapons,
        HasTraitOld_Stealthy,
        HasTraitOld_Ambidextrous,
        HasTraitOld_Thief,
        HasTraitOld_MartialArts,
        HasTraitOld_Knifing,
        HasTraitOld_Sniper,
        HasTraitOld_Camouflaged,

    };

    struct Consideration
    {
        DecisionInput input;
        ResponseCurve curve;
        SoldierID target;
        INT32 targetLocation;
    };

    struct DecisionScoreEvaluator
    {
        ActionType Action;
        float priorityWeight;
        std::string description;
        std::vector<Consideration> considerations;
    };

    struct DecisionScore
    {
        float score;
        size_t idx; // Index into DM::decisions

        bool operator<(const DecisionScore& a) const
        {
            return score < a.score;
        }

        bool operator>(const DecisionScore& a) const
        {
            return score > a.score;
        }

        bool operator==(const DecisionScore& a) const
        {
            return score == a.score;
        }

    };

    struct DecisionMaker
    {
        std::vector<DecisionScoreEvaluator> decisions;
        std::vector<DecisionScore> scores;
    };

    static float GetTrait(DecisionInput input, SOLDIERTYPE* soldier)
    {
        switch ( input )
        {
            case HasTrait_AutoWeapons:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, AUTO_WEAPONS_NT));
            case HasTrait_HeavyWeapons:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, HEAVY_WEAPONS_NT));
            case HasTrait_Sniper:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, SNIPER_NT));
            case HasTrait_Ranger:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, RANGER_NT));
            case HasTrait_Gunslinger:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, GUNSLINGER_NT));
            case HasTrait_MartialArts:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, MARTIAL_ARTS_NT));
            case HasTrait_SquadLeader:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, SQUADLEADER_NT));
            case HasTrait_Technician:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, TECHNICIAN_NT));
            case HasTrait_Doctor:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, DOCTOR_NT));
            case HasTrait_Ambidextrous:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, AMBIDEXTROUS_NT));
            case HasTrait_Melee:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, MELEE_NT));
            case HasTrait_Throwing:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, THROWING_NT));
            case HasTrait_NightOps:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, NIGHT_OPS_NT));
            case HasTrait_Stealthy:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, STEALTHY_NT));
            case HasTrait_Athletics:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, ATHLETICS_NT));
            case HasTrait_Bodybuilding:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, BODYBUILDING_NT));
            case HasTrait_Demolitions:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, DEMOLITIONS_NT));
            case HasTrait_Teaching:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, TEACHING_NT));
            case HasTrait_Scouting:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, SCOUTING_NT));
            case HasTrait_Covert:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, COVERT_NT));
            case HasTrait_RadioOperator:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, RADIO_OPERATOR_NT));
            case HasTrait_Snitch:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, SNITCH_NT));
            case HasTrait_Survival:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, SURVIVAL_NT));
            case HasTraitOld_Lockpicking:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, LOCKPICKING_OT));
            case HasTraitOld_HandToHand:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, HANDTOHAND_OT));
            case HasTraitOld_Electronics:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, ELECTRONICS_OT));
            case HasTraitOld_NightOps:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, NIGHTOPS_OT));
            case HasTraitOld_Throwing:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, THROWING_OT));
            case HasTraitOld_Teaching:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, TEACHING_OT));
            case HasTraitOld_HeavyWeapons:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, HEAVY_WEAPS_OT));
            case HasTraitOld_AutoWeapons:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, AUTO_WEAPS_OT));
            case HasTraitOld_Stealthy:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, STEALTHY_OT));
            case HasTraitOld_Ambidextrous:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, AMBIDEXT_OT));
            case HasTraitOld_Thief:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, THIEF_OT));
            case HasTraitOld_MartialArts:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, MARTIALARTS_OT));
            case HasTraitOld_Knifing:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, KNIFING_OT));
            case HasTraitOld_Sniper:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, PROF_SNIPER_OT));
            case HasTraitOld_Camouflaged:
                return static_cast<float>(HAS_SKILL_TRAIT(soldier, CAMOUFLAGED_OT));
        }

        return 0.0f;
    }

    float GetInputValue(DecisionInput input, SOLDIERTYPE* me, SoldierID target, INT32 targetLocation)
    {
        switch ( input )
        {
            case MyHealth:
            {
                const auto health = static_cast<float>(me->stats.bLife);
                const auto min = 0.0f;
                const auto max = static_cast<float>(me->stats.bLifeMax);
                return NormalizeInput(health, min, max);
            }
            case MyMorale:
            {
                const auto morale = static_cast<float>(me->aiData.bAIMorale);
                const auto min = 0.0f;
                const auto max = static_cast<float>(MORALE_FEARLESS);
                return NormalizeInput(morale, min, max);
            }
            case HaveGasmaskInInventory:
            {
                const auto gasmaskInvSlot = FindGasMask(me);
                if ( gasmaskInvSlot != NO_SLOT ) { return 1.0f; }
                else { return 0.0f; }
            }
            case HasKnife:
            {
                const auto bWeaponIn = FindAIUsableObjClass(me, (IC_BLADE));
                if ( bWeaponIn != NO_SLOT ) { return 1.0f; }
                else { return 0.0f; }
            }
            case HasThrowingKnife:
            {
                const auto bWeaponIn = FindAIUsableObjClass(me, (IC_THROWING_KNIFE));
                if ( bWeaponIn != NO_SLOT ) { return 1.0f; }
                else { return 0.0f; }
            }
            case HasSmokeGrenade:
            {
                const auto bGrenadeIn = FindThrowableGrenade(me, EXPLOSV_SMOKE);
                if ( bGrenadeIn != NO_SLOT ) { return 1.0f; }
                else { return 0.0f; }
            }
            case DistanceFromMe:
            {
                const auto distance = PythSpacesAway(me->sGridNo, targetLocation);
            }
            case IamInSmoke:
                return static_cast<float>(InSmoke(me, me->sGridNo));
            case IamInTearGas:
                return static_cast<float>(InTearGas(me, me->sGridNo));
            case IamInMustardGas:
                return static_cast<float>(InMustardGas(me, me->sGridNo));
            case NearSmoke:

            case NearTearGas:

            case NearMustardGas:

            case InShock:

            case InsideRoom:
                return static_cast<float>(InARoom(me->sGridNo, NULL));
            case NightTime:

            case InLightAtNight:

            case TakenLargeHit:
                return static_cast<float>(me->TakenLargeHit());

            case HasTrait_AutoWeapons:
            case HasTrait_HeavyWeapons:
            case HasTrait_Sniper:
            case HasTrait_Ranger:
            case HasTrait_Gunslinger:
            case HasTrait_MartialArts:
            case HasTrait_SquadLeader:
            case HasTrait_Technician:
            case HasTrait_Doctor:
            case HasTrait_Ambidextrous:
            case HasTrait_Melee:
            case HasTrait_Throwing:
            case HasTrait_NightOps:
            case HasTrait_Stealthy:
            case HasTrait_Athletics:
            case HasTrait_Bodybuilding:
            case HasTrait_Demolitions:
            case HasTrait_Teaching:
            case HasTrait_Scouting:
            case HasTrait_Covert:
            case HasTrait_RadioOperator:
            case HasTrait_Snitch:
            case HasTrait_Survival:
            case HasTraitOld_Lockpicking:
            case HasTraitOld_HandToHand:
            case HasTraitOld_Electronics:
            case HasTraitOld_NightOps:
            case HasTraitOld_Throwing:
            case HasTraitOld_Teaching:
            case HasTraitOld_HeavyWeapons:
            case HasTraitOld_AutoWeapons:
            case HasTraitOld_Stealthy:
            case HasTraitOld_Ambidextrous:
            case HasTraitOld_Thief:
            case HasTraitOld_MartialArts:
            case HasTraitOld_Knifing:
            case HasTraitOld_Sniper:
            case HasTraitOld_Camouflaged:
                return GetTrait(input, me);
        }
    }

    float ScoreDSE(DecisionScoreEvaluator DSE, SOLDIERTYPE* me)
    {
        float finalScore = 1.0f;

        for ( const auto& consideration : DSE.considerations )
        {
            if ( finalScore <= 0.0f ) { break; }

            float curveInput = GetInputValue(consideration.input, me, consideration.target, consideration.targetLocation);
            float considerationScore = CalculateResponse(curveInput, consideration.curve);

            finalScore *= considerationScore;
        }

        // Add consideration compensation
        const auto modificationFactor = 1 - 1 / (DSE.considerations.size());
        const auto makeUpValue = (1 - finalScore) * modificationFactor;
        finalScore += finalScore * makeUpValue;

        // Add DSE priority weight
        finalScore *= DSE.priorityWeight;

        return finalScore;
    }

    void ScoreAllDecisions(DecisionMaker DM, SOLDIERTYPE* me)
    {
        size_t idx = 0;
        for ( const auto& decision : DM.decisions )
        {
            DM.scores[idx].score = ScoreDSE(decision, me);
            DM.scores[idx].idx = idx;
            ++idx;
        }

        // Sort all scores from best to last
        std::sort(DM.scores.begin(), DM.scores.end(), std::greater<DecisionScore>());
    }

    ActionType SelectScoredAction(DecisionMaker DM)
    {
        // Select highest for now. TODO weighted random choice
        const auto idx = DM.scores[0].idx;
        const auto action = DM.decisions[idx].Action;

        return action;
    }

}
