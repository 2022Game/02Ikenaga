#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CImage.h"
#include "CSound.h"
#include "CText.h"

class CSword;
class CShield;
class CSlash;
class CShieldRotate;
class CHealCircle;
class CBuffCircle;
class CBuffAura;
class CPowerUpAura;
class CGamePlayerUI;

#include <algorithm>

#define DEFAULT_CAMERA_POS CVector(0.0f, 10.0f, 50.0f)

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer : public CXCharacter
{
public:
	bool IsDeath() const;
	bool IsDeath2() const;
	bool IsDeath3() const;

	// �C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();

	// �R���X�g���N�^
	CPlayer();
	// �f�X�g���N�^
	~CPlayer();

	// �ҋ@���
	void UpdateIdle();

	// ����
	void UpdateDei();

	// ����
	void UpdateRevival();

	// 1���x���A�b�v
	void LevelUp();
	// ���x���̕ύX
	virtual void ChangeLevel(int level);

	// HP�񕜂Ɠ���U��(SA)�񐔂�������
	void AutomaticRecovery();
	// �U�����������������U��(SA)����
	virtual void AttackRecovery();

	//����J�E���g	
	void RollingCount();

	// �o���l�����Z
	void AddExp(int exp);

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render();

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W</param>
	virtual void TakeDamage(int damage, CObjectBase* causedObj);

	/// <summary>
	/// �U���͂̋����������擾
	/// </summary>
	/// <returns></returns>
	float GetAtkBuff() const override;

    /// <summary>
    /// �h��͂̋����������擾
    /// </summary>
    /// <param name="attackDir"></param>
    /// <returns></returns>
    float GetDefBuff(const CVector& attackDir) const override;

	static int mHp;
	static int mMaxHp;
	static int mSp;
	static int mRecoveryCount;  // ����U���̉񐔂̉񕜂܂ł̃J�E���g
	static bool mHeal;
	static bool mRolling;

private:
	// ���s
	void UpadateWalk();
	// �U��
	void UpdateAttack();
	// �U��2
	void UpdateAttack2();
	// �U��3
	void UpdateAttack3();
	// �U��4
	void UpdateAttack4();
	// �U��5
	void UpdateAttack5();
	// �U��6
	void UpdateAttack6();
	// �U��7
	void UpdateAttack7();
	// �U���I���҂�
	void UpdateAttackWait();
	// �W�����v�J�n
	void UpdateJumpStart();
	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();
	// �ړ��̍X�V����
	void UpdateMove();
	// �U���̓A�b�v
	void UpdatePowerUp();
	// �U���̓A�b�v�I��
	void UpdatePowerUpEnd();
	// �q�b�g
	void UpdateHit();
	// �K�[�h
	void UpdateGuard();
	// �K�[�h���̃q�b�g
	void UpdateGuardHit();
	// ���
	void UpdateRolling();
	// �W�����v�U��
	void UpdateJumpAttack();
	// �W�����v�U���I���҂�
	void UpdateJumpAttackWait();

	// ����񐔂𑝂₷����
	int mRollingTime;
	// �����
	int mRollingCount;
	// �U������
	int mAttackTime;
	//	�U����
	int mAttackCount;
	// �񕜂܂ł̃J�E���g
	int mHealCount;
	// �񕜗�
	int mRecoveryAmount;

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eIdle,		// �ҋ@
		eWalk,		// ���s
		eAttack,	// �U��
		eJumpStart,	// �W�����v�J�n
		eAttack2,	// �U��2
		eAttack3,	// �U��3
		eAttack4,	// �U��4
	    eAttack5,	// �U��5
		eAttack6,	// �U��6
		eAttack7,	// �U��7
		ePowerUp,   // �U���̓A�b�v
		eHit,       // �q�b�g
		eGuard,     // �K�[�h
		eGuardHit,  // �K�[�h�q�b�g
		eRolling,   // ���
		eDie,       // ����
		eJumpAttack,// �W�����v�U��
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �v���C���[�̃C���X�^���X
	static CPlayer* spInstance;

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

	// �v���C���[�̏��
	enum class EState
	{
		eIdle,		      // �ҋ@
		eWalk,            // ���s
		eAttack,	      // �U��
		eAttack2,	      // �U��2
		eAttack3,	      // �U��3
		eAttack4,	      // �U��4
		eAttack5,	      // �U��5
		eAttack6,	      // �U��6
		eAttack7,	      // �U��7
		eAttackWait,      // �U���I���҂�
		eJumpStart,	      // �W�����v�J�n
		eJump,		      // �W�����v��
		eJumpEnd,	      // �W�����v�I��
		ePowerUp,         // �U���̓A�b�v
		ePowerUpEnd,      // �U���̓A�b�v�I��
		eHit,             // �q�b�g
		eGuard,           // �K�[�h
		eGuardHit,        // �K�[�h�q�b�g
		eRolling,         // ���
		eDie,             // ����
		eJumpAttack,      // �W�����v�U��
		eJumpAttackWait,  // �W�����v�U���I���҂�
	};
	// �v���C���[�̏��
	EState mState;
	int mStateStep;
	int mStateJumpAttackStep;
	int mBuffStep;

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;      // �ړ����x
	bool mIsGrounded;        // �ڒn���Ă��邩�ǂ���

	// �����R���C�_�[
	CColliderLine* mpColliderLine;
	// �L�����N�^�[�̉����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereHead;
	// �L�����N�^�[�̉����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereBody;

	// �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColHead;
	// �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColBody;

	CTransform* mpRideObject;

	CVector mDefaultPos;  // �f�t�H���g�̍��W
	CVector current;

	// ����֘A
	CSword* mpSword;      // �E��Ɏ���
	CShield* mpShield;    // ����Ɏ���
	CSlash* mpSlash;      // �X���b�V��

	// ��]����V�[���h
	CShieldRotate* mpShieldRotate;
	CShieldRotate* mpShieldRotate2;
	CShieldRotate* mpShieldRotate3;
	CShieldRotate* mpShieldRotate4;

	// �G�t�F�N�g�֘A
	CHealCircle* mpHealCircle;    // �񕜗p�̃T�[�N��
	CBuffCircle* mpBuffCircle;    // �o�t�T�[�N��
	CBuffAura* mpBuffAura;        // �o�t�I�[��
	CPowerUpAura* mpPowerUpAura;  // �p���[�A�b�v�I�[��

	// SE�֘A
	CSound* mpSlashSE;            // �X���b�V���̌��ʉ�
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	CGamePlayerUI* mpGameUI;
};
