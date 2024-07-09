#pragma once
#include "CUIBase.h"

class CText;

// �v���C���[����UI
class CPlayerNameUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerNameUI();

	// �f�X�g���N�^
	~CPlayerNameUI();

	// �v���C���[����ݒ�
	void SetPlayerName(std::string name);

	// �X�V
	void Update()override;

	// �`��
	void Render()override;
private:
	CText* mpNameText;
	CVector2 mBasePos;
};