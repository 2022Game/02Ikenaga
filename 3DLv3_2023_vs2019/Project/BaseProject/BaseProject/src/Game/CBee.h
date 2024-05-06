#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 �I
 �v���C���[�̐��背�x��(31�`40)
 �G�l�~�[�N���X���p��
*/
class CBee : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CBee* Instance();

	// �R���X�g���N�^
	CBee();
	~CBee();

	// �ҋ@���
	void UpdateIdle();
	// �ҋ@���2
	void UpdateIdle2();

	// �U��
	void UpdateAttack();
	// �U���I���҂�
	void UpdateAttackWait();

	//�q�b�g
	void UpdateHit();
	// ���ʎ�
	void UpdateDie();

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
	int mFlyingTime;   // ��s����

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eAttack,	// �U��
		eHit,       // �q�b�g
		eDie,       // ����
		eRun,		// �ړ�

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �I�̃C���X�^���X
	static CBee* spInstance;

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

	// �I�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eIdle2,		// �ҋ@2
		eAttack,	// �U��
		eAttackWait,// �U���I���҂�
		eHit,       // �q�b�g
		eDie,       // ���ʎ�
		eRun,       // �ړ�
	};
	EState mState;	// �I�̏��
	int mStateAttackStep;  // State���̍U���ł̃X�e�b�v����

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsSpawnedNeedleEffect;

	CColliderLine* mpColliderLine;

	CColliderSphere* mpColliderSphereHead;   // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereBeak;   // �L�����N�^�[�����߂��R���C�_�[(���΂�)
	CColliderSphere* mpColliderSphereBeak2;  // �L�����N�^�[�����߂��R���C�_�[(���΂�2)
	CColliderSphere* mpColliderSphereBody;   // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereTail;   // �L�����N�^�[�����߂��R���C�_�[(�K��)
	CColliderSphere* mpColliderSphereTail2;  // �L�����N�^�[�����߂��R���C�_�[(�K��2)
	CColliderSphere* mpColliderSphereTail3;  // �L�����N�^�[�����߂��R���C�_�[(�K��3)
	CColliderSphere* mpColliderSphereTail4;  // �L�����N�^�[�����߂��R���C�_�[(�K��4)
	CColliderSphere* mpColliderSphereTail5;  // �L�����N�^�[�����߂��R���C�_�[(�K��5)

	CColliderSphere* mpDamageColHead;    // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColBeak;    // �_���[�W���󂯂�R���C�_�[(���΂�)
	CColliderSphere* mpDamageColBeak2;   // �_���[�W���󂯂�R���C�_�[(���΂�2)
	CColliderSphere* mpDamageColBody;    // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColTail;    // �_���[�W���󂯂�R���C�_�[(�K��)
	CColliderSphere* mpDamageColTail2;   // �_���[�W���󂯂�R���C�_�[(�K��2)
	CColliderSphere* mpDamageColTail3;   // �_���[�W���󂯂�R���C�_�[(�K��3)
	CColliderSphere* mpDamageColTail4;   // �_���[�W���󂯂�R���C�_�[(�K��4)
	CColliderSphere* mpDamageColTail5;   // �_���[�W���󂯂�R���C�_�[(�K��5)

	CColliderSphere* mpAttackCol;        // �_���[�W��^����R���C�_�[(�j)

	CTransform* mpRideObject;
};