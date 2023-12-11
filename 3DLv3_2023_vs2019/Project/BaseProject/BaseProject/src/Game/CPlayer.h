#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CImage.h"
class CHpGauge;
class CSaGauge;
class CSword;
class CShield;

#include <algorithm>

#define DEFAULT_CAMERA_POS CVector(0.0f, 50.0f, 75.0f)
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
	// �U���I���҂�
	void UpdateAttackWait();
	// �W�����v�J�n
	void UpdateJumpStart();
	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();
	//�U���̓A�b�v
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

	// 1���x���A�b�v
	void LevelUp();
	// ���x���̕ύX
	virtual void ChangeLevel(int level);

	// HP�񕜂Ɠ���U��(SA)�񐔂�������
	void AutomaticRecovery();

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

	static int mPower;
private:
	int mAttackTime;
	int mAttackcount;
	// �񕜂܂ł̃J�E���g
	int healcount;
	// ����U���̉񐔂̉񕜂܂ł̃J�E���g
	int recoverycount;
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		//eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eWalk,		// ���s
		eAttack,	// �U��
		eJumpStart,	// �W�����v�J�n
		eAttack2,	// �U��2
		eAttack3,	// �U��3
		eAttack4,	// �U��4
	    eAttack5,	// �U��5
		eAttack6,	// �U��6
		ePowerUp,   // �U���̓A�b�v
		eHit,       // �q�b�g
		eGuard,     // �K�[�h
		eGuardHit,  // �K�[�h�q�b�g
		eRolling,   // ���
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
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �v���C���[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eAttack,	// �U��
		eAttack2,	// �U��2
		eAttack3,	// �U��3
		eAttack4,	// �U��4
		eAttack5,	// �U��5
		eAttack6,	// �U��6
		eAttackWait,// �U���I���҂�
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
		ePowerUp,   // �U���̓A�b�v
		ePowerUpEnd,  // �U���̓A�b�v�I��
		eHit,       // �q�b�g
		eGuard,     // �K�[�h
		eGuardHit,  // �K�[�h�q�b�g
		eRolling,   // ���
		eStop,      // ��~
	};
	EState mState;	// �v���C���[�̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;  // �_���[�W���󂯂�R���C�_�[

	CTransform* mpRideObject;

	CHpGauge* mpHpGauge;  // HP�Q�[�W
	CSaGauge* mpSaGauge;  // SA�Q�[�W(�X�y�V�����A�^�b�N)

	CVector mDefaultPos;  // �f�t�H���g�̍��W

	CSword* mpSword;      // �E��Ɏ���
	CShield* mpShield;    // ����Ɏ���
};
