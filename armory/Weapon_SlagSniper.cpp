#include "Weapon_SlagSniper.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


SlagSniper::SlagSniper(Raven_Bot* owner)
	:Raven_Weapon(type_slag_sniper,
	script->GetInt("SlagSniper_DefaultRounds"),
	script->GetInt("SlagSniper_MaxRoundsCarried"),
	script->GetDouble("SlagSniper_FiringFreq"),
	script->GetDouble("SlagSniper_IdealRange"),
	script->GetDouble("Slag_MaxSpeed"),
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

void SlagSniper::Render()
{
	m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
		m_pOwner->Pos(),
		m_pOwner->Facing(),
		m_pOwner->Facing().Perp(),
		m_pOwner->Scale());

	gdi->PurplePen();

	gdi->ClosedShape(m_vecWeaponVBTrans);
}

void SlagSniper::ShootAt(Vector2D pos)
{
	if (isReadyForNextShot())
	{
		//fire!
		m_pOwner->GetWorld()->AddSlag(m_pOwner, pos);
		m_iNumRoundsLeft--;

		UpdateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("SlagSniper_SoundRange"));
	}
}

double SlagSniper::GetDesirability(double DistToTarget)
{
	if (m_iNumRoundsLeft == 0 || m_pOwner->GetTargetBot()->Slagged())
	{
		m_dLastDesirabilityScore = 0;
	}
	else
	{
		//fuzzify distance and amount of ammo
		m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);
		m_FuzzyModule.Fuzzify("TeamSize", 1);

		m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);

	}
	
	return m_dLastDesirabilityScore;
}

void SlagSniper::InitializeFuzzyModule()
{
	FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");

	FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close", 0, 25, 150);
	FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
	FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far", 150, 300, 1000);

	FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability");
	FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
	FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
	FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);

	FuzzyVariable& TeamSize = m_FuzzyModule.CreateFLV("TeamSize");
	FzSet& SmallTeam = TeamSize.AddRightShoulderSet("SmallTeam", 0, 1, 3);
	FzSet& MediumTeam = TeamSize.AddTriangularSet("MediumTeam", 1, 3, 5);
	FzSet& HugeTeam = TeamSize.AddLeftShoulderSet("HugeTeam", 3, 5, 100);

	m_FuzzyModule.AddRule(FzAND(Target_Close, SmallTeam), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, MediumTeam), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, HugeTeam), VeryDesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Medium, SmallTeam), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, MediumTeam), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, HugeTeam), VeryDesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Far, SmallTeam), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, MediumTeam), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, HugeTeam), VeryDesirable);

}