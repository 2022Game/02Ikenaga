#pragma once
#include "CUIBase.h"

class CImage;

// �o�t�X�L��
class CBuffSkill : public CUIBase
{
public:
	// �R���X�g���N
	CBuffSkill(float x, float y);

	// �f�X�g���N�^
	~CBuffSkill();

private:
	CImage* mpBuffSkill;
};