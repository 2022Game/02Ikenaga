#pragma once
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CEnemy.h"
#include "CSound.h"

class CHit;

/*
 �I�����W�X���C���̃N���X
 �v���C���[�̐��背�x��(1�`5)
 �G�l�~�[�N���X���p��
*/
class CSlime2 : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CSlime2* Instance();

	// �R���X�g���N�^
	CSlime2();
	// �f�X�g���N�^
	~CSlime2();

	// �X�V����
	void Update();

	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	virtual void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

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

	// �����_���Ɉʒu���擾
	CVector GetRandomSpawnPos()override;

	// �`��
	void Render();

private:

	// �ҋ@���
	void UpdateIdle();
	// �ҋ@2���
	void UpdateIdle2();
	// �v���C���[�̍U�����q�b�g�������̑ҋ@���
	void UpdateIdle3();
	// �ҋ@2�̏I���҂�
	void UpdateIdleWait();

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

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eIdle2,		// �ҋ@2
		eIdle3,		// �ҋ@3
		eIdle4,		// �ҋ@4
		eAttack,	// �U��
		eAttack2,	// �U��2
		eHit,       // �q�b�g
		eDie,       // ���ʎ�
		eDizzy,     // �߂܂�(����)
		eRun,		// �ړ�

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �C���X�^���X
	static CSlime2* spInstance;

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

	// �I�����W�X���C���̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eIdle2,     // �ҋ@2
		eIdle3,     // �ҋ@3
		eIdleWait,  // �ҋ@�I���҂�
		eAttack,	// �U��
		eAttack2,	// �U��2
		eAttackWait,// �U���I���҂�
		eHit,       // �q�b�g
		eDie,       // ���ʎ�
		eDizzy,     // �߂܂�(����)
		eRun,       // �ړ�
	};
	EState mState;	// �I�����W�X���C���̏��
	int mStateStep; // State���̃X�e�b�v����

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	float mAttackTime;  // �U�����Ԃ̊Ԋu

	CColliderLine* mpColliderLine;          // �L�����N�^�[�̐����R���C�_�[
	CColliderSphere* mpColliderSphereBody;  // �L�����N�^�[�̉����߂��R���C�_�[(��)
	CColliderSphere* mpDamageColBody;       // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpAttackColBody;       // �_���[�W��^����R���C�_�[(��)
	CTransform* mpRideObject;

	CHit* mpHitEffect;        // �q�b�g�G�t�F�N�g 

	// �T�E���h�֘A
	CSound* mpSlimeAttackSE;  // �U���̉�
	CSound* mpSlimeDizzySE;   // �����̉�
	CSound* mpSlimeHitSE;     // �q�b�g�̉�
	CSound* mpSlimeDieSE;     // ���S�̉�

	bool mIsSlimeAttackSE;
	bool mIsSlimeDizzySE;
	bool mIsSlimeHitSE;
	bool mIsSlimeDieSE;
};
