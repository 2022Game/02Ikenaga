#pragma once
#include "CUIBase.h"

class CText;

// �v���C���[���x����UI
class CPlayerLevelUI : public CUIBase
{
public:

	// �R���X�g���N
	CPlayerLevelUI(float x,float y);

	// �f�X�g���N�^
	~CPlayerLevelUI();

	// �v���C���[���x����ݒ�
	void SetLevel(int level);

	// �`��
	void Render()override;
private:
	CText* mpLevelText;
	CVector2 mBasePos;
};