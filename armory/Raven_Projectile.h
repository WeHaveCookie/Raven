#ifndef PROJECTILE_H
#define PROJECTILE_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Raven_Projectile.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Base class to define a projectile type. A projectile of the correct
//          type is created whnever a weapon is fired. In Raven there are four
//          types of projectile: Slugs (railgun), Pellets (shotgun), Rockets
//          (rocket launcher ) and Bolts (Blaster) 
//-----------------------------------------------------------------------------
#include "game/MovingEntity.h"
#include "2d/Vector2D.h"
#include "time/CrudeTimer.h"
#include <list>

class Raven_Bot;

class Raven_Game;

namespace Element
{
	enum Enum
	{
		neutral = 0,
		start_enum = neutral,
		fire, // 1
		frost, // 2
		poison, // 3
		electric, // 4
		slag, // 5
		end_enum = slag
	};

	static void operator+=(Enum& a1, const int a2)
	{
		a1 = static_cast<Enum>(a1 + a2);
		if (a1 > end_enum)
		{
			a1 = start_enum;
		}
	}
}


struct ProjectileInfo
{
	float damage;
	Element::Enum element;
	double duration;
};

class Raven_Projectile : public MovingEntity
{
protected:

  //the ID of the entity that fired this
  int           m_iShooterID;

  //the place the projectile is aimed at
  Vector2D      m_vTarget;

  //a pointer to the world data
  Raven_Game*   m_pWorld;

  //where the projectile was fired from
  Vector2D      m_vOrigin;


  //is it dead? A dead projectile is one that has come to the end of its
  //trajectory and cycled through any explosion sequence. A dead projectile
  //can be removed from the world environment and deleted.
  bool          m_bDead;

  //this is set to true as soon as a projectile hits something
  bool          m_bImpacted;

  //the position where this projectile impacts an object
  Vector2D      m_vImpactPoint;

  //this is stamped with the time this projectile was instantiated. This is
  //to enable the shot to be rendered for a specific length of time
  double       m_dTimeOfCreation;

  //how much damage the projectile inflicts, type and term element
  ProjectileInfo		m_info;

  Raven_Bot*            GetClosestIntersectingBot(Vector2D From,
                                                  Vector2D To)const;

  std::list<Raven_Bot*> GetListOfIntersectingBots(Vector2D From,
                                                  Vector2D To)const;


public:

  Raven_Projectile(Vector2D  target,   //the target's position
                   Raven_Game* world,  //a pointer to the world data
                   int      ShooterID, //the ID of the bot that fired this shot
                   Vector2D origin,  //the start position of the projectile
                   Vector2D heading,   //the heading of the projectile
                   float      damage,    //how much damage it inflicts
                   double    scale,    
                   double    MaxSpeed, 
                   double    mass,
                   double    MaxForce,
				   unsigned int ElementID,
				   double		ElementDuration) : MovingEntity(origin,
                                                     scale,
                                                     Vector2D(0,0),
                                                     MaxSpeed,
                                                     heading,
                                                     mass,
                                                     Vector2D(scale, scale),
                                                     0, //max turn rate irrelevant here, all shots go straight
                                                     MaxForce),

                                        m_vTarget(target),
                                        m_bDead(false),
                                        m_bImpacted(false),
                                        m_pWorld(world),
                                        m_vOrigin(origin),
                                        m_iShooterID(ShooterID)
                

  {
	  m_dTimeOfCreation = Clock->GetCurrentTime();
	  m_info.damage = damage;
	  m_info.element = static_cast<Element::Enum>(ElementID);
	  m_info.duration = ElementDuration;
  }

  Raven_Projectile(Vector2D  target,   //the target's position
				 Raven_Game* world,  //a pointer to the world data
				  int      ShooterID, //the ID of the bot that fired this shot
				  Vector2D origin,  //the start position of the projectile
				  Vector2D heading,   //the heading of the projectile
				  float      damage,    //how much damage it inflicts
				  double    scale,
				  double    MaxSpeed,
				  double    mass,
				  double    MaxForce) : 
				  Raven_Projectile(target, world, ShooterID, origin, heading, damage, scale, MaxSpeed, mass, MaxForce, Element::neutral, 0)


  {
  }

  //unimportant for this class unless you want to implement a full state 
  //save/restore (which can be useful for debugging purposes)
  void Write(std::ostream&  os)const{}
  void Read (std::ifstream& is){}

  //must be implemented
  virtual void Update() = 0;
  virtual void Render() = 0;
  
  //set to true if the projectile has impacted and has finished any explosion 
  //sequence. When true the projectile will be removed from the game
  bool isDead()const{return m_bDead;}
  
  //true if the projectile has impacted but is not yet dead (because it
  //may be exploding outwards from the point of impact for example)
  bool HasImpacted()const{return m_bImpacted;}



};






#endif