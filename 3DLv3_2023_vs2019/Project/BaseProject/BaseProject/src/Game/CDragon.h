#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
class CFlamethrower;

/*
 �{�X�h���S��
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

	//�q�b�g
	void UpdateHit();
	// �h��
	void UpdateDefense();
	// ���ʎ�
	void UpdateDie();

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
		eIdle3,       // �ҋ@���3
		eAttack,	  // �U��
		eAttack2,	  // �U��2
		eAttack3,	  // �U��3
		eHit,         // �q�b�g
		eDefense,     // �h��
		eDie,         // ����
		eWalk,		  // ���s
		eJumpStart,	  // �W�����v�J�n
		eJump,		  // �W�����v��
		eJumpEnd,	  // �W�����v�I��

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
		eIdle,		  // �ҋ@
		eIdle2,		  // �ҋ@2
		eIdle3,       // �ҋ@���3
		eAttack,	  // �U��
		eAttack2,	  // �U��2
		eAttack3,	  // �U��3
		eAttackWait,  // �U���I���҂�
		eHit,         // �q�b�g
		eDefense,     // �h��
		eDie,         // ���ʎ�
		eJumpStart,	  // �W�����v�J�n
		eJump,		  // �W�����v��
		eJumpEnd,	  // �W�����v�I��
	};
	EState mState;	// �h���S���̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	//CColliderLine* mpColliderLine;
	//CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;    // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageCol2;   // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageCol3;   // �_���[�W���󂯂�R���C�_�[(���̐�[)
	CColliderSphere* mpDamageCol4;   // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageCol5;   // �_���[�W���󂯂�R���C�_�[(��2)
	CColliderSphere* mpDamageCol6;   // �_���[�W���󂯂�R���C�_�[(��3)
	CColliderSphere* mpDamageCol7;   // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageCol8;   // �_���[�W���󂯂�R���C�_�[(�Ғ�)
	CColliderSphere* mpDamageCol9;   // �_���[�W���󂯂�R���C�_�[(�Ғ�2)
	CColliderSphere* mpDamageCol10;  // �_���[�W���󂯂�R���C�_�[(�Ȃ�)
	CColliderSphere* mpDamageCol11;  // �_���[�W���󂯂�R���C�_�[(�Ȃ�2)
	CColliderSphere* mpDamageCol12;  // �_���[�W���󂯂�R���C�_�[(�Ȃ�3)
	CColliderSphere* mpDamageCol13;  // �_���[�W���󂯂�R���C�_�[(�Ȃ�4)
	CColliderSphere* mpDamageCol14;  // �_���[�W���󂯂�R���C�_�[(�Ȃ�5)
	CColliderSphere* mpDamageCol15;  // �_���[�W���󂯂�R���C�_�[(�O�̍���)
	CColliderSphere* mpDamageCol16;  // �_���[�W���󂯂�R���C�_�[(�O�̍���2)

	//CColliderSphere* mpAttackCol;  // �_���[�W��^����R���C�_�[
	CTransform* mpRideObject;

	// �Ή����˃G�t�F�N�g
	CFlamethrower* mpFlamethrower;
};