#pragma once
#ifndef CXPLAYER_H
#define CXPLAYER_H

#include"CXCharacter.h"
#include "CInput.h"

class CXPlayer :public CXCharacter
{
public:
	void Update();
private:
	//int mTime;
	//CXCharacter mCharacter;
	CInput mInput;
};

#endif
