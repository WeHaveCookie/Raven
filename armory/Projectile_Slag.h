#pragma once
#include "Raven_Projectile.h"

class Slag : public Raven_Projectile
{
private:

	//tests the trajectory of the shell for an impact
	void TestForImpact();


public:

	Slag(Raven_Bot* shooter, Vector2D target);

	void Render();

	void Update();
};

