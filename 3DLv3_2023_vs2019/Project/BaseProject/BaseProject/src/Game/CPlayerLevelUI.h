#pragma once
#include "CUIBase.h"

class CText;

// �v���C���[���x����UI
class CPlayerLevelUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerLevelUI();

	// �f�X�g���N�^
	~CPlayerLevelUI();

	// �v���C���[���x����ݒ�
	void SetPlayerLevel(int level);

	// �X�V
	void Update()override;

	// �`��
	void Render()override;
private:
	CText* mpLevelText;
	CVector2 mBasePos;
};