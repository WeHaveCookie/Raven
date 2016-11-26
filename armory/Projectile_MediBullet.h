#pragma once
#include "Raven_Projectile.h"

class MediBullet : public Raven_Projectile
{
private:

	//tests the trajectory of the shell for an impact
	void TestForImpact();


public:

	MediBullet(Raven_Bot* shooter, Vector2D target);

	void Render();

	void Update();
};

