#pragma once
#include "Raven_Projectile.h"

class Electrical : public Raven_Projectile
{
private:

	//tests the trajectory of the shell for an impact
	void TestForImpact();


public:

	Electrical(Raven_Bot* shooter, Vector2D target);

	void Render();

	void Update();
};

