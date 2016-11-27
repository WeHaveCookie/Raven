#pragma once
#include "Raven_Projectile.h"

class Frost : public Raven_Projectile
{
private:

	//tests the trajectory of the shell for an impact
	void TestForImpact();


public:

	Frost(Raven_Bot* shooter, Vector2D target);

	void Render();

	void Update();
};

