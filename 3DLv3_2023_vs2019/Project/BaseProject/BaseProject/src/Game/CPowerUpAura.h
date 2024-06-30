#pragma once
#include "CBillBoardImage.h"

// �p���[�A�b�v�I�[��
class CPowerUpAura : public CBillBoardImage
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <param name="width">��</param>
	/// <param name="dire">�ړ�����</param>
	/// <param name="speed">�ړ����x</param>
	/// <param name="dist">�ړ�����������鋗��</param>
	CPowerUpAura(float angle, float width, const CVector& dire, float speed, float dist);

	// �f�X�g���N�^
	~CPowerUpAura();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �J���[��ݒ�
	void SetColor(const CColor& color) override;

	// �u�����h�^�C�v��ݒ�
	void SetBlendType(EBlend type);

	// �p���[�A�b�v�I�[���J�n
	void StartAura();
	// �p���[�A�b�v�I�[���I��
	void EndAura();

	// �X�V
	void Update();

private:

	// �U���̓A�b�v
	bool mPowerUp;
	// �o�ߎ���(�U���̓A�b�v�p)
	float mElapsedPowerUpTime;

	float mAngle;        // ��]�p�x
	float mDistance;     // �����傩��̋���
	CVector mMoveDir;	 // �ړ���������x�N�g��
	CVector mMoveSpeed;	 // �ړ����x
	float mKillMoveDist; // �ړ�����������鋗��
	float mMovedDist;    // ���݈ړ���������
	float mBaseScale;    // ������̃x�[�X�X�P�[���l
};