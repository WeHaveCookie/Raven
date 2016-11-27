#pragma once

#include "Raven_Weapon.h"

class SlagSniper : public Raven_Weapon
{
private:
	void InitializeFuzzyModule();

public:
	SlagSniper(Raven_Bot* owner);
	
	void Render();
	void ShootAt(Vector2D pos);
	double GetDesirability(double DistToTarget);

};

