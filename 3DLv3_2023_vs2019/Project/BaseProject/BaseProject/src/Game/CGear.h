#pragma once
#include "CUIBase.h"

class CImage;

// ����
class CGear : public CUIBase
{
public:
	// �R���X�g���N
	CGear(float x, float y);

	// �f�X�g���N�^
	~CGear();

private:
	CImage* mpGear;
};