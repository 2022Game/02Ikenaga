#pragma once
#pragma once
#include "CCharacter.h"

class CGoal2 : public CCharacter
{
public:
	static void Goal(int goal);
	static int Goal();
	//Õ“Ëˆ—4
	void Collision(CCharacter* m, CCharacter* o);
	//static int Goal();
	CGoal2(float x, float y, float w, float h, CTexture* pt);
	void Update() {};
	//void Render() 
private:
	//static int sGoal;
};
