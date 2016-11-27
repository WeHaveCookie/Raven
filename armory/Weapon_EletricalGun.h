#pragma once

#include "Raven_Weapon.h"

class ElectricalGun : public Raven_Weapon
{
private:
	void InitializeFuzzyModule();

public:
	ElectricalGun(Raven_Bot* owner);
	
	void Render();
	void ShootAt(Vector2D pos);
	double GetDesirability(double DistToTarget);

};

