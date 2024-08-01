#pragma once
#include "CBillBoardImage.h"

// �{�XUI
class CBossUI : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CBossUI(float height);

	// �f�X�g���N�^
	~CBossUI();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �X�V
	void Update() override;

private:

	float mBaseScale;    // ������̃x�[�X�X�P�[���l
	float mHeight;       // ����
};