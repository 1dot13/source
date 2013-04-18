#include "../include/AbstractPlanFactory.h"
#include "../../Tactical/Soldier Control.h"

#include <iostream>

namespace AI
{
    namespace tactical
    {
        AIInputData::AIInputData(Auditive , int noise_maker, int grid_no, int level, int volume, int type)
            : noise_maker_(noise_maker),
              grid_no_(grid_no),
              level_(level),
              volume_(volume),
              type_(type)
        {
            event_type_ = auditive_event;
        }

        AIInputData::AIInputData(Visual, SOLDIERTYPE* opponent, int grid_no, int level, int caller1, int caller2)
            : opponent_(opponent),
              grid_no_(grid_no),
              level_(level),
              caller1_(caller1),
              caller2_(caller2)
        {
            event_type_ = visual_event;
        }

        AIInputData::AIInputData()
            : event_type_(no_event)
        {
        }

        std::ostream& operator<<(std::ostream& os, const AIInputData& i) 
        {
            if(i.event_type_ == AIInputData::no_event)
                return os<<"no_event";
            if(i.event_type_ ==  AIInputData::auditive_event)
                return os<<"sound: "<<i.noise_maker_<<" made a sound at "<<i.grid_no_<<", lvl "<<i.level_<<" vol "<<i.volume_<<" type "<<i.type_;
            if(i.event_type_ == AIInputData::visual_event)
                return os<<"visual: "<<(int)i.opponent_->ubID<<" was seen at "<<i.grid_no_<<", lvl "<<i.level_<<" caller1 "<<i.caller1_<<" caller2 "<<i.caller2_;
            return os;
        }

    }
}

