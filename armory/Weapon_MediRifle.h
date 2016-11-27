#pragma once

#include "Raven_Weapon.h"

class MediRifle : public Raven_Weapon
{
private:
	void InitializeFuzzyModule();

public:
	MediRifle(Raven_Bot* owner);
	
	void Render();
	void ShootAt(Vector2D pos);
	double GetDesirability(double DistToTarget);

};

