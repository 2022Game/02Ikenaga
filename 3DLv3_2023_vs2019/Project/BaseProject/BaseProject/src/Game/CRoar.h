#pragma once
#include "CObjectBase.h"
#include "CWeapon.h"
#include "CModel.h"

// �Y���уG�t�F�N�g
class CRoar : public CWeapon
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�G�t�F�N�g�𔭐��������I�u�W�F�N�g</param>
	/// <param name="pos">�����ʒu</param>
	/// <param name="dir">�ړ�����</param>
	/// <param name="speed">�ړ����x</param>
	/// <param name="dist">�ړ�����������鋗��</param>
	CRoar(CObjectBase* owner, const CVector& pos, const CVector& dir, float speed, float dist);

	// �f�X�g���N�^
	~CRoar();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CObjectBase* mpOwner;	// ���̃G�t�F�N�g�̎�����
	CModel* mpModel;		// �G�t�F�N�g�̃��f���f�[�^
	CVector mMoveSpeed;		// �ړ����x
	float mKillMoveDist;	// �ړ�����������鋗��
	float mMovedDist;		// ���݈ړ���������
};