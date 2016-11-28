#include "Goal_FollowLeader.h"
#include "..\Raven_Bot.h"
#include "..\Raven_SteeringBehaviors.h"
#include "../armory/Raven_Weapon.h"



Goal_FollowLeader::Goal_FollowLeader(Raven_Bot* pBot):Goal<Raven_Bot>(pBot, goal_follow_leader),
                                                    m_dIdealRange(0)
{}

//---------------------------- Initialize -------------------------------------
//-----------------------------------------------------------------------------  
void Goal_FollowLeader::Activate()
{
	auto botLeader = m_pOwner->GetLeader();
	if (botLeader != m_pOwner && botLeader != NULL)
	{
		m_pOwner->GetSteering()->SetTarget(botLeader->Pos());
		m_pOwner->GetSteering()->SeekOn();
	}
}

//------------------------------ Process --------------------------------------
//-----------------------------------------------------------------------------
int Goal_FollowLeader::Process()
{
  //if status is inactive, call Activate()
  ActivateIfInactive();

  /*
  if (m_pOwner->GetCurrentWeapon()->isInIdealWeaponRange())
  {
    m_iStatus = completed;
  }
*/
  return m_iStatus;
}

//---------------------------- Terminate --------------------------------------
//-----------------------------------------------------------------------------
void Goal_FollowLeader::Terminate()
{




}

