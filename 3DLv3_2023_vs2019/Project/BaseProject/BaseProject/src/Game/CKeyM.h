#pragma once
#include "CUIBase.h"

class CImage;

// M�L�[
class CKeyM : public CUIBase
{
public:
	// �R���X�g���N
	CKeyM(float x, float y);

	// �f�X�g���N�^
	~CKeyM();

private:
	CImage* mpKeyM;
};