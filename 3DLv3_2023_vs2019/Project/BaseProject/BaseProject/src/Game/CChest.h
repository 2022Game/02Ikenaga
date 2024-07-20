#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"

class CHit;

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
	// �f�X�g���N�^
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

	// �`��
	void Render();

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
		float animSpeed;    // �A�j���[�V�����̍Đ����x
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �`�F�X�g�����X�^�[�̏��
	enum class EState
	{
		eIdle,		  // �키�O�̑ҋ@
		eIdle2,		  // �키�O�̑ҋ@2
		eIdle3,       // �ҋ@���3
		eAttack,	  // �U��
		eAttack2,	  // �U��2
		eAttackWait,  // �U���I���҂�
		eHit,         // �q�b�g
		eDie,         // ���ʎ�
		eDizzy,       // �߂܂�(����)
		eRun          // �ړ�
	};
	EState mState;	  // �`�F�X�g�����X�^�[�̏��

	int mStateAttackStep;   // State���̍U���ł̃X�e�b�v����

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;  // �ړ����x
	bool mIsGrounded;	 // �ڒn���Ă��邩�ǂ���
	bool mIsSpawnedCoinEffect;  // �R�C������

	// �����R���C�_�[
	CColliderLine* mpColliderLine;

	// �L�����N�^�[�̉����߂��R���C�_�[
	CColliderSphere* mpColliderSphereHead;   // ��
	CColliderSphere* mpColliderSphereBody;   // ��
	CColliderSphere* mpColliderSphereFeet;   // �O�̍���
	CColliderSphere* mpColliderSphereFeet2;  // �O�̉E��

	// �_���[�W���󂯂�R���C�_�[
	CColliderSphere* mpDamageColHead;        // ��
	CColliderSphere* mpDamageColBody;        // ��
	CColliderCapsule* mpDamageColFeet;       // �O�̍���
	CColliderCapsule* mpDamageColFeet2;      // �O�̉E��

	// �_���[�W��^����R���C�_�[
	CColliderSphere* mpAttackColHead;        // ��
	CTransform* mpRideObject;

	// �G�t�F�N�g�֘A
	CHit* mpHitEffect;          // �q�b�g�G�t�F�N�g 
};