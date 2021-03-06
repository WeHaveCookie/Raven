#pragma once

#include "Goal_Evaluator.h"
#include "../Raven_Bot.h"


class FollowLeader_Evaluator : public Goal_Evaluator
{ 
public:

  FollowLeader_Evaluator(double bias):Goal_Evaluator(bias){}
  
  double CalculateDesirability(Raven_Bot* pBot);

  void  SetGoal(Raven_Bot* pEnt);

  void RenderInfo(Vector2D Position, Raven_Bot* pBot);
};
