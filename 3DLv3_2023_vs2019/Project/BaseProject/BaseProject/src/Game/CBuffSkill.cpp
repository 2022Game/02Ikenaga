#include "CBuffSkill.h"
#include "CImage.h"

// �R���X�g���N
CBuffSkill::CBuffSkill(float x, float y)
{
	mpBuffSkill = new CImage("BuffSkill");
	mpBuffSkill->SetPos(x, y);
	mpBuffSkill->SetSize(100.0f, 100.0f);
}

// �f�X�g���N�^
CBuffSkill::~CBuffSkill()
{
}