#pragma once
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 �}�b�V�����[��2�N���X
 �G�l�~�[�N���X���p��
*/
class CMushroom2 : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CMushroom2* Instance();

	// �R���X�g���N�^
	CMushroom2();
	~CMushroom2();

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
	// �U��3
	void UpdateAttack3();
	// �U���I���҂�
	void UpdateAttackWait();

	//�q�b�g
	void UpdateHit();
	// ���ʎ�
	void UpdateDie();
	// �߂܂�(����)
	void UpdateDizzy();
	// ����
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

	static int mHp;

private:
	int mAttackTime;   // �U�����Ԃ̊Ԋu
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
		eAttack3,	// �U��3
		eHit,       // �q�b�g
		eDie,       // ����
		eDizzy,     // �߂܂�(����)
		eRun,		// ����
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �}�b�V�����[��2�̃C���X�^���X
	static CMushroom2* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �}�b�V�����[��2�̏��
	enum class EState
	{
		eIdle,		// �키�O�̑ҋ@
		eIdle2,		// �키�O�̑ҋ@2
		eIdle3,     // �ҋ@���3
		eAttack,	// �U��
		eAttack2,	// �U��2
		eAttack3,	// �U��3
		eAttackWait,// �U���I���҂�
		eHit,       // �q�b�g
		eDie,       // ���ʎ�
		eDizzy,     // �߂܂�(����)
		eRun,       // ����
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
	};
	EState mState;	// �}�b�V�����[��2�̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;  // ��
	CColliderSphere* mpColliderSphere2; // ��
	CColliderSphere* mpDamageCol;  // �_���[�W���󂯂�R���C�_�[
	CColliderSphere* mpAttackCol;  // �_���[�W��^����R���C�_�[
	CTransform* mpRideObject;
};
