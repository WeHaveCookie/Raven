#include "Raven_TargetingSystem.h"
#include "Raven_Bot.h"
#include "Raven_SensoryMemory.h"
#include "armory/Raven_Weapon.h"
#include "Raven_WeaponSystem.h"
#include "Raven_ObjectEnumerations.h"
#include "TeamManager.h"

//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
Raven_TargetingSystem::Raven_TargetingSystem(Raven_Bot* owner):m_pOwner(owner),
                                                               m_pCurrentTarget(0)
{}



//----------------------------- Update ----------------------------------------

//-----------------------------------------------------------------------------
void Raven_TargetingSystem::Update()
{
  double ClosestDistSoFar = MaxDouble;
  m_pCurrentTarget       = 0;

  //grab a list of all the opponents the owner can sense
  std::list<Raven_Bot*> SensedBots;
  SensedBots = m_pOwner->GetSensoryMem()->GetListOfRecentlySensedOpponents();
  
  // If bot get a medirifle, select the lowest hp ally.
  if (m_pOwner->GetWeaponSys()->GetCurrentWeapon()->GetType() == type_medi_rifle)
  {
	  auto allies = TeamManager::GetSingleton()->getAlly(m_pOwner->getTeam());
	  if (allies.size() >= 2)
	  {
		  m_pCurrentTarget = allies[0];
		  if (m_pCurrentTarget == m_pOwner)
		  {
			  m_pCurrentTarget = allies[1];
		  }
			for (auto& ally : allies)
			{
				if (ally->HealthPerc() < m_pCurrentTarget->HealthPerc() && m_pOwner != ally)
				{
					m_pCurrentTarget = ally;
				}
			}
			return;
	  }
  }
  
  std::list<Raven_Bot*>::const_iterator curBot = SensedBots.begin();
  for (curBot; curBot != SensedBots.end(); ++curBot)
  {
	  
    //make sure the bot is alive and that it is not the owner
    if ((*curBot)->isAlive() && (*curBot != m_pOwner) && ((*curBot)->getTeam() != m_pOwner->getTeam()))
    {
      double dist = Vec2DDistanceSq((*curBot)->Pos(), m_pOwner->Pos());

      if (dist < ClosestDistSoFar)
      {
        ClosestDistSoFar = dist;
        m_pCurrentTarget = *curBot;
      }
    }
  }
}




bool Raven_TargetingSystem::isTargetWithinFOV()const
{
  return m_pOwner->GetSensoryMem()->isOpponentWithinFOV(m_pCurrentTarget);
}

bool Raven_TargetingSystem::isTargetShootable()const
{
  return m_pOwner->GetSensoryMem()->isOpponentShootable(m_pCurrentTarget);
}

Vector2D Raven_TargetingSystem::GetLastRecordedPosition()const
{
  return m_pOwner->GetSensoryMem()->GetLastRecordedPositionOfOpponent(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenVisible()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenVisible(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenOutOfView()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenOutOfView(m_pCurrentTarget);
}
