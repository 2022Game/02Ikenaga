#include "CSlashSkill.h"
#include "CImage.h"

// �R���X�g���N
CSlashSkill::CSlashSkill(float x, float y)
{
	mpSlashSkill = new CImage("BuffSkill");
	mpSlashSkill->SetPos(x, y);
	mpSlashSkill->SetSize(100.0f, 100.0f);
	mpSlashSkill->SetColor(1.0f, 1.0f, 1.0f, 0.9f);
}

// �f�X�g���N�^
CSlashSkill::~CSlashSkill()
{
}