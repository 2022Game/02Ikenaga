#pragma once
#include "CUIBase.h"

class CText;

// Lv��UI
class CLevelUI : public CUIBase
{
public:

	// �R���X�g���N
	CLevelUI(bool is3dGauge);

	// �f�X�g���N�^
	~CLevelUI();

	// Level��ݒ�
	void SetLevel(std::string level);

	// ���S�ʒu�̂̊����̐ݒ�
	void SetCenterRatio(const CVector2& ratio);

	// ���[���h���W��ݒ�
	void SetWorldPos(const CVector& worldPos);

	// �X�V
	void Update()override;

	// �`��
	void Render()override;
private:
	CText* mpLevelText;    // ���x���e�L�X�g
	CVector2 mBasePos;     // �x�[�X�ʒu
	CVector2 mCenterRatio; // ���S�ʒu�̊���
	bool mIs3dGauge;       // 3D��Ԃɔz�u����Q�[�W���ǂ���
};