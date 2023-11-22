#pragma once
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
class CHpGauge;

#include <algorithm>
/*
���b�h�X���C��(�G�l�~�[)�̃N���X
�L�����N�^�N���X���p��
*/
class CSlime : public CXCharacter
{
public:
	bool IsDeath() const;
	//�C���X�^���X�̃|�C���^�̎擾
	static CSlime* Instance();

	// �R���X�g���N�^
	CSlime();
	~CSlime();

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
	// �U�����[�h
	void UpdateAttackMode();

	//�q�b�g
	void UpdateHIt();
	// ���ʎ�
	void UpdateDie();
	// �߂܂�(����)
	void UpdateDizzy();
	// ���s
	void UpdateWalk();

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
	virtual void TakeDamage(int damage, CObjectBase*causedObj);

	static int mHp;

private:
	int mAttackTime;   // �U�����Ԃ̊Ԋu
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
		eWalk,		// ���s
		eLeftWalk,  // �E�ɕ��s
		eRightWalk, // ���ɕ��s
		eRun,       // ����
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// ���b�h�X���C��(�G�l�~�[)�̃C���X�^���X
	static CSlime* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// ���b�h�X���C��(�G�l�~�[)�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eIdle2,     // �ҋ@2
		eIdle3,     // �ҋ@3
		eIdleWait,  // �ҋ@�I���҂�
		eAttack,	// �U��
		eAttack2,	// �U��2
		eAttackWait,// �U���I���҂�
		eAttackMode,// �U�����[�h
		eHit,       // �q�b�g
		eDie,       // ���ʎ�
		eDizzy,     // �߂܂�(����)
		eWalk,      // ���s
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
	};
	EState mState;	// ���b�h�X���C��(�G�l�~�[)�̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;  // �_���[�W���󂯂�R���C�_�[
	CColliderSphere* mpAttackCol;  // �_���[�W��^����R���C�_�[
	CTransform* mpRideObject;

	CHpGauge* mpHpGauge;  // HP�Q�[�W
};
