#include "FollowLeader_Evaluator.h"
#include "../navigation/Raven_PathPlanner.h"
#include "../Raven_ObjectEnumerations.h"
#include "../lua/Raven_Scriptor.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"

#include "Goal_Think.h"
#include "Raven_Goal_Types.h"
#include "../TeamManager.h"

#include "../Raven_SteeringBehaviors.h"

//---------------- CalculateDesirability -------------------------------------
//-----------------------------------------------------------------------------
double FollowLeader_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
	
  double Desirability = 0.05;

  auto teamSize = TeamManager::GetSingleton()->getSizeTeam(pBot->getTeam());
  if (teamSize <= 1 || pBot->isLeader())
  {
	  return 0;
  }
  Desirability *= m_dCharacterBias;

  return Desirability;
}

//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void FollowLeader_Evaluator::SetGoal(Raven_Bot* pBot)
{
  pBot->GetBrain()->AddGoal_MoveToPosition(pBot->GetLeader()->Pos());
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void FollowLeader_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
  gdi->TextAtPos(Position, "EX: " + ttos(CalculateDesirability(pBot), 2));
}