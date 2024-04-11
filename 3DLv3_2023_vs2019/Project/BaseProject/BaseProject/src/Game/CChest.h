#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 �`�F�X�g�����X�^�[
 �v���C���[�̐��背�x��(51�`60)
 �G�l�~�[�N���X���p��
*/
class CChest : public CEnemy
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CChest* Instance();

	// �R���X�g���N�^
	CChest();
	~CChest();

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

	// �q�b�g
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
	// �_���[�W��^�����I�u�W�F�N�g
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
		eIdle2,		// �ҋ@2
		eIdle3,     // �ҋ@���3
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

	// �`�F�X�g�����X�^�[�̃C���X�^���X
	static CChest* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �`�F�X�g�����X�^�[�̏��
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
		eRun        // �ړ�
	};
	EState mState;	// �`�F�X�g�����X�^�[�̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsSpawnedCoinEffect;

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphereHead;   // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereBody;   // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereFeet;   // �L�����N�^�[�����߂��R���C�_�[(�O�̍���)
	CColliderSphere* mpColliderSphereFeet2;  // �L�����N�^�[�����߂��R���C�_�[(�O�̉E��)
	CColliderSphere* mpColliderSphereFeet3;  // �L�����N�^�[�����߂��R���C�_�[(���̍���)
	CColliderSphere* mpColliderSphereFeet4;  // �L�����N�^�[�����߂��R���C�_�[(���̉E��)

	CColliderSphere* mpDamageColHead;    // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColBody;    // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColFeet;    // �_���[�W���󂯂�R���C�_�[(�O�̍���)
	CColliderSphere* mpDamageColFeet2;   // �_���[�W���󂯂�R���C�_�[(�O�̉E��)
	CColliderSphere* mpDamageColFeet3;   // �_���[�W���󂯂�R���C�_�[(���̍���)
	CColliderSphere* mpDamageColFeet4;   // �_���[�W���󂯂�R���C�_�[(���̉E��)

	CColliderSphere* mpAttackColHead;   // �_���[�W��^����R���C�_�[(��)
	CTransform* mpRideObject;
};