#include "Trigger_WeaponGiver.h"
#include "misc/Cgdi.h"
#include "misc/Stream_Utility_Functions.h"
#include <fstream>
#include "../lua/Raven_Scriptor.h"
#include "../constants.h"
#include "../Raven_ObjectEnumerations.h"
#include "../Raven_WeaponSystem.h"


///////////////////////////////////////////////////////////////////////////////

Trigger_WeaponGiver::Trigger_WeaponGiver(std::ifstream& datafile) :
	Trigger_Respawning<Raven_Bot>(GetValueFromStream<int>(datafile))
{
	m_fromDeadBot = false;
	m_team = -1;

	Read(datafile);

	//create the vertex buffer for the rocket shape
	const int NumRocketVerts = 8;
	const Vector2D rip[NumRocketVerts] = { Vector2D(0, 3),
										 Vector2D(1, 2),
										 Vector2D(1, 0),
										 Vector2D(2, -2),
										 Vector2D(-2, -2),
										 Vector2D(-1, 0),
										 Vector2D(-1, 2),
										 Vector2D(0, 3) };

	for (int i = 0; i < NumRocketVerts; ++i)
	{
		m_vecRLVB.push_back(rip[i]);
	}
}


Trigger_WeaponGiver::Trigger_WeaponGiver(Vector2D pos, double r, int GraphNodeIndex, int team) :
	Trigger_Respawning<Raven_Bot>(BaseGameEntity::GetNextValidID())
{
	m_fromDeadBot = true;
	m_team = team;

	SetPos(pos);
	SetBRadius(r);
	SetGraphNodeIndex(GraphNodeIndex);

	//create this trigger's region of fluence
	AddCircularTriggerRegion(Pos(), script->GetDouble("DefaultGiverTriggerRange"));

	SetRespawnDelay((unsigned int)(0 * FrameRate));


	//create the vertex buffer for the rocket shape
	const int NumRocketVerts = 8;
	const Vector2D rip[NumRocketVerts] = { Vector2D(0, 3),
		Vector2D(1, 2),
		Vector2D(1, 0),
		Vector2D(2, -2),
		Vector2D(-2, -2),
		Vector2D(-1, 0),
		Vector2D(-1, 2),
		Vector2D(0, 3) };

	for (int i = 0; i < NumRocketVerts; ++i)
	{
		m_vecRLVB.push_back(rip[i]);
	}
}


void Trigger_WeaponGiver::Try(Raven_Bot* pBot)
{
	if (m_fromDeadBot)
	{
// 		if (this->isActive() && this->isTouchingTrigger(pBot->Pos(), pBot->BRadius()) && m_team == pBot->getTeam())
		if (this->isActive() && this->isTouchingTrigger(pBot->Pos(), pBot->BRadius()))
		{
			pBot->GetWeaponSys()->AddWeapon(EntityType());

			Deactivate();
			deleteTrigger();
		}
	} 
	else
	{
		if (this->isActive() && this->isTouchingTrigger(pBot->Pos(), pBot->BRadius()))
		{
			pBot->GetWeaponSys()->AddWeapon(EntityType());

			Deactivate();
		}
	}
}


void Trigger_WeaponGiver::Read(std::ifstream& in)
{
	double x, y, r;
	int GraphNodeIndex;

	in >> x >> y >> r >> GraphNodeIndex;

	SetPos(Vector2D(x, y));
	SetBRadius(r);
	SetGraphNodeIndex(GraphNodeIndex);

	//create this trigger's region of fluence
	AddCircularTriggerRegion(Pos(), script->GetDouble("DefaultGiverTriggerRange"));


	SetRespawnDelay((unsigned int)(script->GetDouble("Weapon_RespawnDelay") * FrameRate));
}


void Trigger_WeaponGiver::Render()
{
	if (isActive())
	{
		switch (EntityType())
		{
		case type_rail_gun:
		{
			gdi->BluePen();
			gdi->BlueBrush();
			gdi->Circle(Pos(), 3);
			gdi->ThickBluePen();
			gdi->Line(Pos(), Vector2D(Pos().x, Pos().y - 9));
		}

		break;

		case type_shotgun:
		{

			gdi->BlackBrush();
			gdi->BrownPen();
			const double sz = 3.0;
			gdi->Circle(Pos().x - sz, Pos().y, sz);
			gdi->Circle(Pos().x + sz, Pos().y, sz);
		}

		break;

		case type_rocket_launcher:
		{

			Vector2D facing(-1, 0);

			m_vecRLVBTrans = WorldTransform(m_vecRLVB,
				Pos(),
				facing,
				facing.Perp(),
				Vector2D(2.5, 2.5));

			gdi->RedPen();
			gdi->ClosedShape(m_vecRLVBTrans);
		}

		break;

		case type_medi_rifle:
		{
			gdi->GreenPen();
			gdi->GreenBrush();
			const double sz = 2.0;
			gdi->Circle(Pos().x - sz, Pos().y, sz / 2);
			gdi->Circle(Pos().x + sz, Pos().y, sz / 2);
			gdi->Circle(Pos().x, Pos().y + sz, sz / 2);
			gdi->Circle(Pos().x, Pos().y - sz, sz / 2);
			break;
		}

		case type_slag_sniper:
		{
			gdi->BlackPen();
			// 		  gdi->PurpleBrush();
			gdi->Circle(Pos(), 3);
			// 		  gdi->ThickPurplePen();
			gdi->Line(Pos(), Vector2D(Pos().x, Pos().y - 9));
			break;
		}

		case type_frost_smg:
		{
			Vector2D facing(-1, 0);

			m_vecRLVBTrans = WorldTransform(m_vecRLVB,
				Pos(),
				facing,
				facing.Perp(),
				Vector2D(2.5, 2.5));

			gdi->BlackPen();
			gdi->WhiteBrush();
			gdi->ClosedShape(m_vecRLVBTrans);
			break;
		}

		case type_electrical_gun:
		{

			gdi->BlackBrush();
			gdi->BluePen();
			const double sz = 3.0;
			gdi->Circle(Pos().x, Pos().y - sz, sz);
			gdi->Circle(Pos().x, Pos().y + sz, sz);
		}

		break;



		}//end switch
	}
}