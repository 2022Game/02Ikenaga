#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CCrackEffect;

/*
 �T�{�e��3
 �v���C���[�̐��背�x��(50)
 �G�l�~�[�N���X���p��
*/
class CCactus3 : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CCactus3* Instance();

	// �R���X�g���N�^
	CCactus3();
	~CCactus3();

	// �키�O�̑ҋ@���
	void UpdateIdle();
	// �키�O�̑ҋ@���2
	void UpdateIdle2();
	// �ҋ@���3
	void UpdateIdle3();

	// �U��
	void UpdateAttack();
	// �U��2
	void UpdateAttack2();
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
		eIdle,		// �키�O�̑ҋ@
		eIdle2,		// �키�O�̑ҋ@2
		eIdle3,     // �ҋ@���3
		eIdle4,     // �ҋ@���4
		eAttack,	// �U��
		eAttack2,	// �U��2
		eHit,       // �q�b�g
		eDie,       // ����
		eDizzy,     // �߂܂�(����)
		eRun,		// �ړ�

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �T�{�e��3�̃C���X�^���X
	static CCactus3* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �T�{�e��3�̏��
	enum class EState
	{
		eIdle,		// �키�O�̑ҋ@
		eIdle2,		// �키�O�̑ҋ@2
		eIdle3,     // �ҋ@���3
		eAttack,	// �U��
		eAttack2,	// �U��2
		eAttackWait,// �U���I���҂�
		eHit,       // �q�b�g
		eDie,       // ���ʎ�
		eDizzy,     // �߂܂�(����)
		eRun,       // �ړ�
	};
	EState mState;	// �T�{�e��3�̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphereHead;      // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereBody;      // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereFeet;      // �L�����N�^�[�����߂��R���C�_�[(����)
	CColliderSphere* mpColliderSphereLeftHand;  // �L�����N�^�[�����߂��R���C�_�[(����)
	CColliderSphere* mpColliderSphereRightHand; // �L�����N�^�[�����߂��R���C�_�[(�E��)

	CColliderSphere* mpDamageColHead;       // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColBody;       // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColFeet;       // �_���[�W���󂯂�R���C�_�[(����)
	CColliderSphere* mpDamageColLeftHand;   // �_���[�W���󂯂�R���C�_�[(����)
	CColliderSphere* mpDamageColRightHand;  // �_���[�W���󂯂�R���C�_�[(�E��)

	CColliderSphere* mpAttackColHead;      // �_���[�W��^����R���C�_�[(��)
	CColliderSphere* mpAttackColLeftHand;  // �_���[�W��^����R���C�_�[(����)

	CTransform* mpRideObject;

	CCrackEffect* mpCrack;  // �Ђъ���
};