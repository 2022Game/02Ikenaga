#pragma once
#include "CUIBase.h"

class CText;

// �G�l�~�[�̃��x��UI
class CEnemyLevelUI : public CUIBase
{
public:

	// �R���X�g���N
	CEnemyLevelUI(bool is3dGauge);

	// �f�X�g���N�^
	~CEnemyLevelUI();

	// �G�l�~�[�̃��x����ݒ�
	void SetEnemyLevel(int level);

	// ���S�ʒu�̂̊����̐ݒ�
	void SetCenterRatio(const CVector2& ratio);

	// ���[���h���W��ݒ�
	void SetWorldPos(const CVector& worldPos);

	// �`��
	void Render()override;

private:
	CText* mpLevelText;    // ���x���e�L�X�g
	CVector2 mBasePos;     // �x�[�X�ʒu
	CVector2 mCenterRatio; // ���S�ʒu�̊���
	bool mIs3dGauge;       // 3D��Ԃɔz�u����Q�[�W���ǂ���
};