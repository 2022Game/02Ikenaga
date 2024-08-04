#pragma once
#include "CBillBoardImage.h"

// SP�̃G�t�F�N�g
class CSpEffect : public CBillBoardImage
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="dir">�ړ�����</param>
	/// <param name="speed">�ړ����x</param>
	/// <param name="dist">�ړ�����������鋗��</param>
	CSpEffect(const CVector& pos, const CVector& dir, float speed, float dist);
	// �f�X�g���N�^
	~CSpEffect();

	// �J���[��ݒ�
	void SetColor(const CColor& color) override;
	// �u�����h�^�C�v��ݒ�
	void SetBlendType(EBlend type);

	// �X�V
	void Update() override;

private:
	CVector mMoveDir;	  // �ړ���������x�N�g��
	float mElapsedTime;	  // �o�ߎ���
	CVector mMoveSpeed;	  // �ړ����x
	float mKillMoveDist;  // �ړ�����������鋗��
	float mMovedDist;     // ���݈ړ���������
};