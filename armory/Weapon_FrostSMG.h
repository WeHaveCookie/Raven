#pragma once

#include "Raven_Weapon.h"

class FrostSMG : public Raven_Weapon
{
private:
	void InitializeFuzzyModule();

public:
	FrostSMG(Raven_Bot* owner);
	
	void Render();
	void ShootAt(Vector2D pos);
	double GetDesirability(double DistToTarget);

};

