#pragma once
#include "CUIBase.h"

class CImage;

// �X���b�V���X�L��
class CSlashSkill : public CUIBase
{
public:
	// �R���X�g���N
	CSlashSkill(float x, float y);

	// �f�X�g���N�^
	~CSlashSkill();

private:
	CImage* mpSlashSkill;
};