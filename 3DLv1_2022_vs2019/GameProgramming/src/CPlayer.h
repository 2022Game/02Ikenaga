#pragma once
//#ifndef CPLAYER_H
//#define CPLAYER_H

//#endif

#include "CCharacter.h"
#include "CInput.h"

class CPlayer : public CCharacter
{
private:
	float mVy;	//YŽ²‘¬“x
	CInput mInput;
public:
	void Collision();
	void Collision(CCharacter* m, CCharacter* o);
	CPlayer(float x, float y, float w, float h, CTexture* pt);
	void Update();
};