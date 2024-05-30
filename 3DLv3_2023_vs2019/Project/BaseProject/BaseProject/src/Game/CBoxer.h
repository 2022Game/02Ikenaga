#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CImpactEffect;

/*
 �{�N�T�[
 �v���C���[�̐��背�x��(71�`80)
 �G�l�~�[�N���X���p��
*/
class CBoxer : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CBoxer* Instance();

	// �R���X�g���N�^
	CBoxer();
	// �f�X�g���N�^
	~CBoxer();

	// �ҋ@���
	void UpdateIdle();
	// �ҋ@���2
	void UpdateIdle2();

	// �U��(�p���`)
	void UpdateAttack();
	// �U��2(�R��)
	void UpdateAttack2();
	// �U���I���҂�
	void UpdateAttackWait();

	//�q�b�g
	void UpdateHit();
	// �h��
	void UpdateDefense();
	// �h�䒆�̃q�b�g
	void UpdateDefenseHit();

	// ���ʎ�
	void UpdateDie();
	// �߂܂�(����)
	void UpdateDizzy();

	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();

	// �ړ�
	void UpdateRun();
	// ����
	void UpdateSlide();

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

	/// <summary>
	/// �h��͂̋����������擾
	/// </summary>
	/// <param name="attackDir"></param>
	/// <returns></returns>
	float GetDefBuff(const CVector& attackDir) const override;

	// ���S����
	void Death() override;

private:
	int mDefenseTime;  // �h�䎞�Ԃ̊Ԋu
	int mAttackTime;   // �U�����Ԃ̊Ԋu

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		  // T�|�[�Y
		eIdle,		  // �ҋ@
		eIdle2,		  // �ҋ@2
		eAttack,	  // �U��(�p���`)
		eAttack2,	  // �U��2(�R��)
		eHit,         // �q�b�g
		eDefense,     // �h��
		eDefenseHit,  // �h�䒆�̃q�b�g
		eDie,         // ����
		eDizzy,       // �߂܂�(����)
		eJump,		  // �W�����v��
		eJumpEnd,	  // �W�����v�I��
		eRun,		  // �ړ�
		eSlide,       // ����

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �{�N�T�[�̃C���X�^���X
	static CBoxer* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
		float animSpeed;    // �A�j���[�V�����̍Đ����x
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �{�N�T�[�̏��
	enum class EState
	{
		eIdle,		  // �ҋ@
		eIdle2,		  // �ҋ@2
		eAttack,	  // �U��(�p���`)
		eAttack2,	  // �U��2(�R��)
		eAttackWait,  // �U���I���҂�
		eHit,         // �q�b�g
		eDefense,     // �h��
		eDefenseHit,  // �h�䒆�̃q�b�g
		eDie,         // ���ʎ�
		eDizzy,       // �߂܂�(����)
		eJump,		  // �W�����v��
		eJumpEnd,	  // �W�����v�I��
		eRun,         // �ړ�
		eSlide,       // ����
	};
	EState mState;	// �{�N�T�[�̏��

	int mStateAttackStep;     // State���̍U���̃X�e�b�v����
	int mStateAttack2Step;    // State���̍U��2�̃X�e�b�v����
	int mStateSlideStep;      // State���̊���X�e�b�v����

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;

	CColliderSphere* mpColliderSphereHead;    // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereBody;    // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereHandR;   // �L�����N�^�[�����߂��R���C�_�[(�E��)
	CColliderSphere* mpColliderSphereHandL;   // �L�����N�^�[�����߂��R���C�_�[(����)
	CColliderSphere* mpColliderSphereFeetR;   // �L�����N�^�[�����߂��R���C�_�[(�E��)
	CColliderSphere* mpColliderSphereFeetL;   // �L�����N�^�[�����߂��R���C�_�[(����)

	CColliderSphere* mpDamageColHead;         // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColBody;         // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColHandR;        // �_���[�W���󂯂�R���C�_�[(�E��)
	CColliderSphere* mpDamageColHandL;        // �_���[�W���󂯂�R���C�_�[(����)
	CColliderSphere* mpDamageColFeetR;        // �_���[�W���󂯂�R���C�_�[(�E��)
	CColliderSphere* mpDamageColFeetL;        // �_���[�W���󂯂�R���C�_�[(����)

	CColliderSphere* mpAttackColHead;         // �_���[�W��^����R���C�_�[(��)
	CColliderSphere* mpAttackColBody;         // �_���[�W��^����R���C�_�[(��)
	CColliderSphere* mpAttackColHandR;        // �_���[�W��^����R���C�_�[(�E��)
	CColliderSphere* mpAttackColFeetR;        // �_���[�W��^����R���C�_�[(�E��)
	CColliderSphere* mpAttackColFeetL;        // �_���[�W��^����R���C�_�[(����)

	CTransform* mpRideObject;

	CImpactEffect* mpImpact;
};