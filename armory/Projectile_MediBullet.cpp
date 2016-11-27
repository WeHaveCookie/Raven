#include "Projectile_MediBullet.h"
#include "../lua/Raven_Scriptor.h"
#include "misc/cgdi.h"
#include "../Raven_Bot.h"
#include "../Raven_Game.h"
#include "../constants.h"
#include "2d/WallIntersectionTests.h"
#include "../Raven_Map.h"

#include "../Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"


MediBullet::MediBullet(Raven_Bot* shooter, Vector2D target)
	:Raven_Projectile(target,
	shooter->GetWorld(),
	shooter->ID(),
	shooter->Pos(),
	shooter->Facing(),
	script->GetFloat("MediBullet_Damage"),
	script->GetDouble("MediBullet_Scale"),
	script->GetDouble("MediBullet_MaxSpeed"),
	script->GetDouble("MediBullet_Mass"),
	script->GetDouble("MediBullet_MaxForce"),
	script->GetInt("MediBullet_Element"), 
	script->GetDouble("MediBullet_Duration"))
{
	assert(target != Vector2D());

}

void MediBullet::Render()
{
	gdi->BlackPen();
	gdi->GreenBrush();
	gdi->Circle(Pos(), 2);
}

void MediBullet::Update()
{
	if (!m_bImpacted)
	{
		m_vVelocity = MaxSpeed() * Heading();

		//make sure vehicle does not exceed maximum velocity
		m_vVelocity.Truncate(m_dMaxSpeed);

		//update the position
		m_vPosition += m_vVelocity;

		TestForImpact();
	}
	else
	{
		m_bDead = true;
	}
}

void MediBullet::TestForImpact()
{
	
	Raven_Bot* hit = GetClosestIntersectingBot(m_vPosition - m_vVelocity, m_vPosition);

	//if hit
	if (hit)
	{
		m_bImpacted = true;

		//send a message to the bot to let it know it's been hit, and who the
		//shot came from
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			m_iShooterID,
			hit->ID(),
			Msg_TakeThatMF,
			(void*)&m_info);

	}

	//test for impact with a wall
	double dist;
	if (FindClosestPointOfIntersectionWithWalls(m_vPosition - m_vVelocity,
		m_vPosition,
		dist,
		m_vImpactPoint,
		m_pWorld->GetMap()->GetWalls()))
	{
		m_bImpacted = true;

		m_vPosition = m_vImpactPoint;

		return;
	}

	//test to see if rocket has reached target position. If so, test for
	//all bots in vicinity
	const double tolerance = 5.0;
	if (Vec2DDistanceSq(Pos(), m_vTarget) < tolerance*tolerance)
	{
		m_bImpacted = true;
	}
}