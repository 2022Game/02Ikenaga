#pragma once
#include "CUIBase.h"

class CImage;

// ���b�N2
class CLock2 : public CUIBase
{
public:
	// �R���X�g���N
	CLock2(float x, float y);

	// �f�X�g���N�^
	~CLock2();

	// �X�V
	void Update();

private:
	CImage* mpLock;
	int mLevel;
};