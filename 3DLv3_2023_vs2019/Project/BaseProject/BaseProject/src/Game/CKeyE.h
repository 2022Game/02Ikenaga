#pragma once
#include "CUIBase.h"

class CImage;

// E�L�[
class CKeyE : public CUIBase
{
public:
	// �R���X�g���N
	CKeyE(float x, float y);

	// �f�X�g���N�^
	~CKeyE();

private:
	CImage* mpKeyE;
};