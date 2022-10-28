#pragma once
#include "CCharacter.h"

class CGoal : public CCharacter
{
public:
	CGoal(float x, float y, float w, float h, CTexture* pt);
	void Update() {};
	//void Render() {};
};
