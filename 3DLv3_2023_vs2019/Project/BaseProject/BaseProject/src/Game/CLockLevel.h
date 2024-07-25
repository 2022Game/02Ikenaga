#pragma once
#include "CUIBase.h"

class CText;

// ���b�N���x��
class CLockLevel : public CUIBase
{
public:
	// �R���X�g���N
	CLockLevel(float x, float y);

	// �f�X�g���N�^
	~CLockLevel();

	// Level��ݒ�
	void SetLevel(std::string level);

	// �X�V
	void Update();

	// �`��
	void Render();

private:
	CText* mpLockLevel;
	int mLevel;
	CVector2 mBasePos;  // �x�[�X�ʒu
};