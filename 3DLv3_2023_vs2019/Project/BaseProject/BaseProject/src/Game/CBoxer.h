#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 �{�N�T�[
 �G�l�~�[�N���X���p��
*/
class CBoxer : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CBoxer* Instance();

	// �R���X�g���N�^
	CBoxer();
	~CBoxer();

	// �ҋ@���
	void UpdateIdle();
	// �ҋ@���2
	void UpdateIdle2();

	// �U��
	void UpdateAttack();
	// �U��2
	void UpdateAttack2();
	// �U���I���҂�
	void UpdateAttackWait();

	// �W�����v�J�n
	void UpdateJumpStart();
	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();

	//�q�b�g
	void UpdateHit();
	// �h��
	void UpdateDefense();
	// �h�䒆�̃q�b�g
	void UpdateDefenseHit();

	// ���ʎ�
	void UpdateDie();
	// �߂܂�(����)
	void UpdateDizzy();
	// �ړ�
	void UpdateRun();
	// ����
	void UpdateSlide();

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
		eAttack,	  // �U��
		eAttack2,	  // �U��2
		eHit,         // �q�b�g
		eDefense,     // �h��
		eDefenseHit,  // �h�䒆�̃q�b�g
		eDie,         // ����
		eDizzy,       // �߂܂�(����)
		eJumpStart,	  // �W�����v�J�n
		eJump,		  // �W�����v��
		eJumpEnd,	  // �W�����v�I��
		eRun,		  // �ړ�
		eSlide,       // ����

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �{�N�T�[�̃C���X�^���X
	static CBoxer* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �{�N�T�[�̏��
	enum class EState
	{
		eIdle,		  // �ҋ@
		eIdle2,		  // �ҋ@2
		eAttack,	  // �U��
		eAttack2,	  // �U��2
		eAttackWait,  // �U���I���҂�
		eHit,         // �q�b�g
		eDefense,     // �h��
		eDefenseHit,  // �h�䒆�̃q�b�g
		eDie,         // ���ʎ�
		eDizzy,       // �߂܂�(����)
		eJumpStart,	  // �W�����v�J�n
		eJump,		  // �W�����v��
		eJumpEnd,	  // �W�����v�I��
		eRun,         // �ړ�
		eSlide,       // ����
	};
	EState mState;	// �{�N�T�[�̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;   // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphere2;  // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphere3;  // �L�����N�^�[�����߂��R���C�_�[(�E��)
	CColliderSphere* mpColliderSphere4;  // �L�����N�^�[�����߂��R���C�_�[(����)
	CColliderSphere* mpColliderSphere5;  // �L�����N�^�[�����߂��R���C�_�[(�E��)
	CColliderSphere* mpColliderSphere6;  // �L�����N�^�[�����߂��R���C�_�[(����)

	CColliderSphere* mpDamageCol;   // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageCol2;  // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageCol3;  // �_���[�W���󂯂�R���C�_�[(�E��)
	CColliderSphere* mpDamageCol4;  // �_���[�W���󂯂�R���C�_�[(����)
	CColliderSphere* mpDamageCol5;  // �_���[�W���󂯂�R���C�_�[(�E��)
	CColliderSphere* mpDamageCol6;  // �_���[�W���󂯂�R���C�_�[(����)

	CColliderSphere* mpAttackCol;   // �_���[�W��^����R���C�_�[(�E��)
	CColliderSphere* mpAttackCol2;  // �_���[�W��^����R���C�_�[(�E��)
	CColliderSphere* mpAttackCol3;  // �_���[�W��^����R���C�_�[(����)
	CColliderSphere* mpAttackCol4;  // �_���[�W��^����R���C�_�[(��)
	CColliderSphere* mpAttackCol5;  // �_���[�W��^����R���C�_�[(��)

	CTransform* mpRideObject;
};