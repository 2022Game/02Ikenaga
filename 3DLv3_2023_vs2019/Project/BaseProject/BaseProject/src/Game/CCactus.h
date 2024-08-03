#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"

class CCrackEffect;
class CHit;

/*
 �T�{�e��
 �v���C���[�̐��背�x��(41�`50)
 �G�l�~�[�N���X���p��
*/
class CCactus : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CCactus* Instance();

	// �R���X�g���N�^
	CCactus();
	// �f�X�g���N�^
	~CCactus();

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

	float mAttackTime;  // ���̍U������

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

	// �T�{�e���̃C���X�^���X
	static CCactus* spInstance;

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

	// �T�{�e���̏��
	enum class EState
	{
		eHit,         // �q�b�g
		eIdle,		  // �키�O�̑ҋ@
		eIdle2,		  // �키�O�̑ҋ@2
		eIdle3,       // �ҋ@���3
		eAttack,	  // �U��
		eAttack2,	  // �U��2
		eAttackWait,  // �U���I���҂�
		eDie,         // ���ʎ�
		eDizzy,       // �߂܂�(����)
		eRun,         // �ړ�
	};				  
	EState mState;	  // �T�{�e���̏��

	int mStateStep;   // State���̃X�e�b�v����

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	// �����R���C�_�[
	CColliderLine* mpColLineSide;    // �����R���C�_�[(��)
	CColliderLine* mpColLineHeight;  // �����R���C�_�[(�c)

	 // �L�����N�^�[�̉����߂��R���C�_�[
	CColliderSphere* mpColliderSphereHead;      // ��
	CColliderSphere* mpColliderSphereBody;      // ��
	CColliderSphere* mpColliderSphereFeet;      // ����
	CColliderSphere* mpColliderSphereLeftHand;  // ����
	CColliderSphere* mpColliderSphereRightHand; // �E��

	// �_���[�W���󂯂�R���C�_�[
	CColliderSphere* mpDamageColHead;           // ��
	CColliderSphere* mpDamageColBody;           // ��
	CColliderSphere* mpDamageColFeet;           // ����
	CColliderCapsule* mpDamageColLeftHand;      // ����
	CColliderCapsule* mpDamageColRightHand;     // �E��

	// �_���[�W��^����R���C�_�[
	CColliderSphere* mpAttackColHead;           // ��
	CColliderSphere* mpAttackColLeftHand;       // ����

	CTransform* mpRideObject;

	// �G�t�F�N�g�֘A
	CCrackEffect* mpCrack;  // �Ђъ���
	CHit* mpHitEffect;      // �q�b�g�G�t�F�N�g
};
