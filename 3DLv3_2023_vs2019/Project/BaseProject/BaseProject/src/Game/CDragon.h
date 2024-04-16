#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
class CFlamethrower;
class CRoarEffect;

/*
 �{�X�h���S��
 �v���C���[�̐��背�x��(91�`100)
 �G�l�~�[�N���X���p��
*/
class CDragon : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CDragon* Instance();

	// �R���X�g���N�^
	CDragon();
	~CDragon();

	// �ҋ@���
	void UpdateIdle();
	// �ҋ@���2
	void UpdateIdle2();
	// �ҋ@���3
	void UpdateIdle3();

	// �U��
	void UpdateAttack();
	// �U��2
	void UpdateAttack2();
	// �U��3
	void UpdateAttack3();
	// �U���I���҂�
	void UpdateAttackWait();
	// ��s���̍U��
	void UpdateFlyingAttack();
	// ��s���̍U���I���҂�
	void UpdateFlyingAttackWait();

	//�q�b�g
	void UpdateHit();
	// �h��
	void UpdateDefense();
	// ���ʎ�
	void UpdateDie();

	// �Y����
	void UpdateRoar();

	// ��s�J�n
	void UpdateFlyingStart();
	// ��s�̑ҋ@
	void UpdateFlyingIdle();
	// ��s�I��
	void UpdateFlyingEnd();

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

	/// <summary>
	/// �h��͂̋����������擾
	/// </summary>
	/// <param name="attackDir"></param>
	/// <returns></returns>
	float GetDefBuff(const CVector& attackDir) const override;

	// ���S����
	void Death() override;

private:
	int mFlyingTime;   // ��s����
	int mDefenseTime;  // �h�䎞�Ԃ̊Ԋu
	int mAttackTime;   // �U�����Ԃ̊Ԋu
	int mFlyingAttackTime;  // ��s���̍U�����Ԃ̊Ԋu
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		   // T�|�[�Y
		eIdle,		   // �ҋ@
		eIdle2,		   // �ҋ@2
		eIdle3,        // �ҋ@���3
		eAttack,	   // �U��
		eAttack2,	   // �U��2
		eAttack3,	   // �U��3
		eFlyingAttack, // ��s���̍U��
		eHit,          // �q�b�gWait
		eDefense,      // �h��
		eDie,          // ����
		eRoar,         // �Y����
		eFlyingStart,  // ��s�J�n
		eFlyingIdle,   // ��s�̑ҋ@
		eFlyingEnd,    // ��s�I��
		eRun,		   // �ړ�

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �h���S���̃C���X�^���X
	static CDragon* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �h���S���̏��
	enum class EState
	{
		eIdle,		       // �ҋ@
		eIdle2,		       // �ҋ@2
		eIdle3,            // �ҋ@���3
		eAttack,	       // �U��
		eAttack2,	       // �U��2
		eAttack3,	       // �U��3
		eAttackWait,       // �U���I���҂�
		eFlyingAttack,     // ��s���̍U��
		eFlyingAttackWait, // ��s���̍U���I���҂�
		eHit,              // �q�b�g
		eDefense,          // �h��
		eDie,              // ���ʎ�
		eRoar,             // �Y����
		eFlyingStart,      // ��s�J�n
		eFlyingIdle,       // ��s�̑ҋ@
		eFlyingEnd,        // ��s�I��
		eRun,              // �ړ�
	};
	EState mState;	// �h���S���̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphereFeet;   // �L�����N�^�[�����߂��R���C�_�[(�O�̍���)
	CColliderSphere* mpColliderSphereFeet2;  // �L�����N�^�[�����߂��R���C�_�[(�O�̉E��)

	CColliderSphere* mpDamageColHead;       // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColMouth;      // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColTipMouth;   // �_���[�W���󂯂�R���C�_�[(���̐�[)
	CColliderSphere* mpDamageColNeck;       // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColChest;      // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColFeet;       // �_���[�W���󂯂�R���C�_�[(�O�̍���)
	CColliderSphere* mpDamageColFeet2;      // �_���[�W���󂯂�R���C�_�[(�O�̉E��)
	CColliderSphere* mpDamageColFeet3;      // �_���[�W���󂯂�R���C�_�[(���̍���)
	CColliderSphere* mpDamageColFeet4;      // �_���[�W���󂯂�R���C�_�[(���̉E��)

	CColliderSphere* mpAttackColHead;     // �_���[�W��^����R���C�_�[(��)
	CColliderSphere* mpAttackColMouth;    // �_���[�W��^����R���C�_�[(��)
	CColliderSphere* mpAttackColTipMouth; // �_���[�W��^����R���C�_�[(���̐�[)

	CTransform* mpRideObject;

	CFlamethrower* mpFlamethrower;  // �Ή����˃G�t�F�N�g
	CRoarEffect* mpRoar;  // �Y���уG�t�F�N�g
};