#pragma once
#include "CUIBase.h"

class CImage;

// Q�L�[
class CKeyQ : public CUIBase
{
public:
	// �R���X�g���N
	CKeyQ(float x, float y);

	// �f�X�g���N�^
	~CKeyQ();

private:
	CImage* mpKeyQ;
};