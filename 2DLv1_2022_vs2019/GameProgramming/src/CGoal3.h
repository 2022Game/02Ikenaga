#pragma once
#include "CCharacter.h"

class CGoal3 : public CCharacter
{
public:
	/*static void Goal(int goal);
	static int Goal();*/
	//Õ“Ëˆ—4
	void Collision(CCharacter* m, CCharacter* o);
	//static int Goal();
	CGoal3(float x, float y, float w, float h, CTexture* pt);
	void Update() {};
private:
	//static int sGoal;
};

