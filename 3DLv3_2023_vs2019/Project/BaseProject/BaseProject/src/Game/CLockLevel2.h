#pragma once
#include "CUIBase.h"

class CText;

// ���b�N���x��2
class CLockLevel2 : public CUIBase
{
public:
	// �R���X�g���N
	CLockLevel2(float x, float y);

	// �f�X�g���N�^
	~CLockLevel2();

	// Level��ݒ�
	void SetLevel2(std::string level);

	// �X�V
	void Update();

	// �`��
	void Render();

private:
	CText* mpLockLevel;
	int mLevel;
	CVector2 mBasePos;  // �x�[�X�ʒu
};