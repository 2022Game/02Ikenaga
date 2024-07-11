#pragma once
#include "CUIBase.h"

class CText;

// Lv��UI
class CLevelUI : public CUIBase
{
public:

	// �R���X�g���N
	CLevelUI();

	// �f�X�g���N�^
	~CLevelUI();

	// Level��ݒ�
	void SetLevel(std::string level);

	// �X�V
	void Update()override;

	// �`��
	void Render()override;
private:
	CText* mpLevelText;
	CVector2 mBasePos;
};