#pragma once

#include "Goals/Goal.h"
#include "Raven_Goal_Types.h"
#include "../Raven_Bot.h"





class Goal_DodgeGetItem : public Goal<Raven_Bot>
{
private:

	Vector2D    m_vStrafeTarget;

	bool        m_bClockwise;

	Vector2D  GetStrafeTarget()const;


public:

	Goal_DodgeGetItem(Raven_Bot* pBot) :Goal<Raven_Bot>(pBot, goal_strafe),
		m_bClockwise(RandBool())
	{}


	void Activate();

	int  Process();

	void Render();

	void Terminate();

};

