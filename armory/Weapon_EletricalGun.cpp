#include "Weapon_EletricalGun.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


ElectricalGun::ElectricalGun(Raven_Bot* owner)
	:Raven_Weapon(type_electrical_gun,
	script->GetInt("ElectricalGun_DefaultRounds"),
	script->GetInt("ElectricalGun_MaxRoundsCarried"),
	script->GetDouble("ElectricalGun_FiringFreq"),
	script->GetDouble("ElectricalGun_IdealRange"),
	script->GetDouble("Electrical_MaxSpeed"),
	owner)

{
	//setup the vertex buffer
	const int NumWeaponVerts = 4;
	const Vector2D weapon[NumWeaponVerts] = { Vector2D(0, -1),
		Vector2D(10, -1),
		Vector2D(10, 1),
		Vector2D(0, 1)
	};


	for (int vtx = 0; vtx < NumWeaponVerts; ++vtx)
	{
		m_vecWeaponVB.push_back(weapon[vtx]);
	}

	//setup the fuzzy module
	InitializeFuzzyModule();
}

void ElectricalGun::Render()
{
	m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
		m_pOwner->Pos(),
		m_pOwner->Facing(),
		m_pOwner->Facing().Perp(),
		m_pOwner->Scale());

	gdi->BluePen();
	gdi->BlueBrush();

	gdi->ClosedShape(m_vecWeaponVBTrans);
}

void ElectricalGun::ShootAt(Vector2D pos)
{
	if (isReadyForNextShot())
	{
		//fire!
		m_pOwner->GetWorld()->AddElectrical(m_pOwner, pos);
		m_iNumRoundsLeft--;

		UpdateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("FrostSMG_SoundRange"));
	}
}

double ElectricalGun::GetDesirability(double DistToTarget)
{
	if (m_iNumRoundsLeft == 0 || m_pOwner->GetTargetBot()->Slagged())
	{
		m_dLastDesirabilityScore = 0;
	}
	else
	{
		//fuzzify distance and amount of ammo
		m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);
		m_FuzzyModule.Fuzzify("EnemyAffect", m_pOwner->GetTargetBot()->NumberAffectedElements());

		m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);

	}
	
	return m_dLastDesirabilityScore;
}

void ElectricalGun::InitializeFuzzyModule()
{
	FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");

	FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close", 0, 25, 150);
	FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
	FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far", 150, 300, 1000);

	FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability");
	FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
	FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
	FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);

	FuzzyVariable& EnemyAffect = m_FuzzyModule.CreateFLV("EnemyAffect");
	FzSet& LowAffect = EnemyAffect.AddRightShoulderSet("LowAffect", 0, 1, 2);
	FzSet& MediumAffect = EnemyAffect.AddTriangularSet("MediumAffect", 2, 3, 4);
	FzSet& HighAffect = EnemyAffect.AddLeftShoulderSet("HighAffect", 3, 4, 5);

	m_FuzzyModule.AddRule(FzAND(Target_Close, LowAffect), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, MediumAffect), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, HighAffect), VeryDesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Medium, LowAffect), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, MediumAffect), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, HighAffect), VeryDesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Far, LowAffect), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, MediumAffect), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, HighAffect), VeryDesirable);

}