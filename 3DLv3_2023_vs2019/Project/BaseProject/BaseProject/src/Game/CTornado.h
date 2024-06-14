#pragma once
#include "CObjectBase.h"
#include "CWeapon.h"
#include "CModel.h"
#include "CColliderCapsule.h"

// �g���l�[�h�G�t�F�N�g
class CTornado : public CWeapon
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
	CTornado(CObjectBase* owner, const CVector& pos, const CVector& dir, float speed, float dist);

	// �f�X�g���N�^
	~CTornado();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CObjectBase* mpOwner;  // ���̃G�t�F�N�g�̎�����
	CModel* mpModel;	   // �G�t�F�N�g�̃��f���f�[�^
	CVector mMoveSpeed;	   // �ړ����x
	float mKillMoveDist;   // �ړ�����������鋗��
	float mMovedDist;	   // ���݈ړ���������
	float mElapsedTime;	   // �o�ߎ���
    CColliderCapsule* mpAttackCol;
};