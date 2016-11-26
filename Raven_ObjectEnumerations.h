#ifndef RAVEN_OBJECTS_H
#define RAVEN_OBJECTS_H

#include <string>


enum 
{
  type_wall, // 0
  type_bot, // 1
  type_unused, // 2
  type_waypoint, // 3
  type_health, // 4
  type_spawn_point, // 5
  type_rail_gun, // 6
  type_rocket_launcher, // 7
  type_shotgun, // 8
  type_blaster, // 9
  type_obstacle, // 10
  type_sliding_door, // 11
  type_door_trigger, // 12
  type_medi_rifle, // 13
  type_slag_sniper // 14
};



inline std::string GetNameOfType(int w)
{
  std::string s;
  
  switch (w)
  {
  case type_wall:
    
    s = "Wall"; break; 
    
  case type_waypoint:
    
    s = "Waypoint"; break;

  case type_obstacle:
    
    s = "Obstacle"; break;

  case type_health:
    
    s = "Health"; break;

  case type_spawn_point:
    
    s = "Spawn Point"; break;

  case type_medi_rifle:

	  s = "MediRifle"; break;

  case type_slag_sniper:
	  s = "SlagSniper"; break;

  case type_rail_gun:
    
    s = "Railgun"; break;

  case type_blaster:
    
    s = "Blaster"; break;

  case type_rocket_launcher:
    
    s =  "rocket_launcher"; break;

  case type_shotgun:
    
    s =  "shotgun"; break;

  case type_unused:
    
    s =  "knife"; break;

  case type_bot:
    
    s =  "bot"; break;

  case type_sliding_door:
    
    s =  "sliding_door"; break;
    
  case type_door_trigger:
    
    s =  "door_trigger"; break;

  default:

    s = "UNKNOWN OBJECT TYPE"; break;

  }

  return s;
}


#endif