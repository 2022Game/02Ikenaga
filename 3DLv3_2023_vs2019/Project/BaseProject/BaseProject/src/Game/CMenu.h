#pragma once
#include "CUIBase.h"

class CImage;

// ���j���[�A�C�R��
class CMenu : public CUIBase
{
public:
	// �R���X�g���N
	CMenu(float x, float y);

	// �f�X�g���N�^
	~CMenu();

private:
	CImage* mpMenu;
};