#include "Weapon_FrostSMG.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


FrostSMG::FrostSMG(Raven_Bot* owner)
	:Raven_Weapon(type_frost_smg,
	script->GetInt("FrostSMG_DefaultRounds"),
	script->GetInt("FrostSMG_MaxRoundsCarried"),
	script->GetDouble("FrostSMG_FiringFreq"),
	script->GetDouble("FrostSMG_IdealRange"),
	script->GetDouble("Frost_MaxSpeed"),
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

void FrostSMG::Render()
{
	m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
		m_pOwner->Pos(),
		m_pOwner->Facing(),
		m_pOwner->Facing().Perp(),
		m_pOwner->Scale());

	gdi->BlackPen();
	gdi->WhiteBrush();

	gdi->ClosedShape(m_vecWeaponVBTrans);
}

void FrostSMG::ShootAt(Vector2D pos)
{
	if (isReadyForNextShot())
	{
		//fire!
		m_pOwner->GetWorld()->AddFrost(m_pOwner, pos);
		m_iNumRoundsLeft--;

		UpdateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("FrostSMG_SoundRange"));
	}
}

double FrostSMG::GetDesirability(double DistToTarget)
{
	if (m_iNumRoundsLeft == 0 || m_pOwner->GetTargetBot()->Slagged())
	{
		m_dLastDesirabilityScore = 0;
	}
	else
	{
		//fuzzify distance and amount of ammo
		m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);
		m_FuzzyModule.Fuzzify("EnemyLife", m_pOwner->GetTargetBot()->HealthPerc());

		m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);

	}
	
	return m_dLastDesirabilityScore;
}

void FrostSMG::InitializeFuzzyModule()
{
	FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");

	FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close", 0, 25, 150);
	FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
	FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far", 150, 300, 1000);

	FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability");
	FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
	FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
	FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);

	FuzzyVariable& EnemyLife = m_FuzzyModule.CreateFLV("EnemyLife");
	FzSet& LowLife = EnemyLife.AddRightShoulderSet("LowLife", 0, 25, 50);
	FzSet& MediumLife = EnemyLife.AddTriangularSet("MediumLife", 25, 50, 75);
	FzSet& HightLife = EnemyLife.AddLeftShoulderSet("HightLife", 50, 75, 100);

	m_FuzzyModule.AddRule(FzAND(Target_Close, LowLife), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, MediumLife), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, HightLife), Undesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Medium, LowLife), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, MediumLife), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, HightLife), Desirable);

	m_FuzzyModule.AddRule(FzAND(Target_Far, LowLife), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, MediumLife), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, HightLife), Desirable);

}