#pragma once
#include "CTask.h"

class CLevelUI;

// �Q�[�����̃G�l�~�[UI
class CGameEnemyUI : public CTask
{
public:

	// �R���X�g���N
	CGameEnemyUI();

	// �f�X�g���N�^
	~CGameEnemyUI();

	// Lv��ݒ�
	void SetLv(std::string lv);

	// Lv���擾
	CLevelUI* GetLv()const;

private:
	// UI�֘A
	CLevelUI* mpLvUI;  // ���x����UI(Lv)
};