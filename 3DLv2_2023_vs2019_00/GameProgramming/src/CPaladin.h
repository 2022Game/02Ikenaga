#pragma once
#ifndef CPALADIN_H
#define CPALADIN_H

#include "CXCharacter.h"
//#include "CModelX.h"

class CPaladin :public CXCharacter
{
public:
	CPaladin();
private:
	static CModelX sModel;
};

#endif
