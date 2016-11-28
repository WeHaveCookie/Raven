#ifndef GOAL_ENUMERATIONS_H
#define GOAL_ENUMERATIONS_H

#include <string>
#include "misc/TypeToString.h"

enum
{
  goal_think, // 0
  goal_explore, // 1
  goal_arrive_at_position, // 2
  goal_seek_to_position, // 3
  goal_follow_path, // 4
  goal_traverse_edge, // 5
  goal_move_to_position, // 6
  goal_get_health, // 7
  goal_get_shotgun, // 8
  goal_get_rocket_launcher, // 9
  goal_get_railgun, // 10
  goal_get_medi_rifle, // 11
  goal_get_slag_sniper, // 12
  goal_get_frost_smg, // 13
  goal_get_electrical_gun, // 14
  goal_wander,
  goal_negotiate_door,
  goal_attack_target,
  goal_hunt_target,
  goal_strafe,
  goal_adjust_range,
  goal_follow_leader,
  goal_say_phrase
  
};

class GoalTypeToString : public TypeToString
{

  GoalTypeToString(){}

public:

  static GoalTypeToString* Instance();
  
  std::string Convert(int gt);
};

#endif
