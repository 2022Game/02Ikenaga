#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 ���̂̃����X�^�[
 �G�l�~�[�N���X���p��
*/
class CBeholder : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CBeholder* Instance();

	// �R���X�g���N�^
	CBeholder();
	~CBeholder();

	// �키�O�̑ҋ@���
	void UpdateIdle();
	// �키�O�̑ҋ@���2
	void UpdateIdle2();

	// �U��
	void UpdateAttack();
	// �U��2
	void UpdateAttack2();
	// �U��3
	void UpdateAttack3();
	// �U��4
	void UpdateAttack4();
	// �U���I���҂�
	void UpdateAttackWait();

	//�q�b�g
	void UpdateHit();
	// ���ʎ�
	void UpdateDie();
	// �߂܂�(����)
	void UpdateDizzy();

	// �ړ�
	void UpdateRun();

	// �X�V����
	void Update();

	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	// �`��
	void Render();

	// 1���x���A�b�v
	void LevelUp();
	// ���x���̕ύX
	void ChangeLevel(int level);

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W</param>
	//�_���[�W��^�����I�u�W�F�N�g
	virtual void TakeDamage(int damage, CObjectBase* causedObj);

	// ���S����
	void Death() override;

private:
	int mAttackTime;   // �U�����Ԃ̊Ԋu
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eIdle2,		// �ҋ@2
		eAttack,	// �U��
		eAttack2,	// �U��2
		eAttack3,	// �U��3
		eAttack4,	// �U��4
		eHit,       // �q�b�g
		eDie,       // ����
		eDizzy,     // �߂܂�(����)
		eRun,		// �ړ�

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// ���̂̃����X�^�[�̃C���X�^���X
	static CBeholder* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// ���̂̃����X�^�[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eIdle2,		// �ҋ@2
		eAttack,	// �U��
		eAttack2,	// �U��2
		eAttack3,	// �U��3
		eAttack4,	// �U��4
		eAttackWait,// �U���I���҂�
		eHit,       // �q�b�g
		eDie,       // ���ʎ�
		eDizzy,     // �߂܂�(����)
		eRun,       // �ړ�
	};
	EState mState;	// ���̂̃����X�^�[�̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;

	CColliderSphere* mpColliderSphere;   // �L�����N�^�[�����߂��R���C�_�[ (��)
	CColliderSphere* mpColliderSphere2;  // �L�����N�^�[�����߂��R���C�_�[ (����̐G��)
	CColliderSphere* mpColliderSphere3;  // �_���[�W���󂯂�R���C�_�[(�E��̐G��)
	CColliderSphere* mpColliderSphere4;  // �_���[�W���󂯂�R���C�_�[(�����̐G��)
	CColliderSphere* mpColliderSphere5;  // �_���[�W���󂯂�R���C�_�[(�E���̐G��)
	CColliderSphere* mpColliderSphere6;  // �_���[�W���󂯂�R���C�_�[(�^�񒆏�̐G��)
	CColliderSphere* mpColliderSphere7;  // �_���[�W���󂯂�R���C�_�[(�^�񒆉��̐G��)

	CColliderSphere* mpDamageCol;   // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageCol2;  // �_���[�W���󂯂�R���C�_�[(����̐G��)
	CColliderSphere* mpDamageCol3;  // �_���[�W���󂯂�R���C�_�[(�E��̐G��)
	CColliderSphere* mpDamageCol4;  // �_���[�W���󂯂�R���C�_�[(�����̐G��)
	CColliderSphere* mpDamageCol5;  // �_���[�W���󂯂�R���C�_�[(�E���̐G��)
	CColliderSphere* mpDamageCol6;  // �_���[�W���󂯂�R���C�_�[(�^�񒆏�̐G��)
	CColliderSphere* mpDamageCol7;  // �_���[�W���󂯂�R���C�_�[(�^�񒆉��̐G��)

	CColliderSphere* mpAttackCol;   // �_���[�W��^����R���C�_�[(��)
	CColliderSphere* mpAttackCol2;  // �_���[�W��^����R���C�_�[(����̐G��)
	CColliderSphere* mpAttackCol3;  // �_���[�W��^����R���C�_�[(�E��̐G��)
	CColliderSphere* mpAttackCol4;  // �_���[�W��^����R���C�_�[(�����̐G��)
	CColliderSphere* mpAttackCol5;  // �_���[�W��^����R���C�_�[(�E���̐G��)
	CColliderSphere* mpAttackCol6;  // �_���[�W��^����R���C�_�[(�^�񒆏�̐G��)
	CColliderSphere* mpAttackCol7;  // �_���[�W��^����R���C�_�[(�^�񒆉��̐G��)
	CTransform* mpRideObject;
};