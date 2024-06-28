#pragma once
#include "CBillBoardImage.h"

// �p���[�A�b�v�I�[��
class CPowerUpAura : public CBillBoardImage
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="dir">�ړ�����</param>
	/// <param name="speed">�ړ����x</param>
	/// <param name="dist">�ړ�����������鋗��</param>
	CPowerUpAura(const CVector& pos, const CVector& dir, float speed, float dist);

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
	void Update() override;

private:

	// ���g�̃x�[�X�X�P�[���l���Z�o
	float CalcScale() const;

	CVector mMoveDir;	 // �ړ���������x�N�g��
	CVector mMoveSpeed;	 // �ړ����x
	float mKillMoveDist; // �ړ�����������鋗��
	float mMovedDist;    // ���݈ړ���������
	float mElapsedTime;  // �o�ߎ���
	float mBaseScale;    // ������̃x�[�X�X�P�[���l
};