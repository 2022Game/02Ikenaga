#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 �G�C
 �v���C���[�̐��背�x��(21�`30)
 �G�l�~�[�N���X���p��
*/
class CRay : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CRay* Instance();

	// �R���X�g���N�^
	CRay();
	~CRay();

	// �ҋ@���
	void UpdateIdle();

	// �U��
	void UpdateAttack();
	// �U���I���҂�
	void UpdateAttackWait();

	//�q�b�g
	void UpdateHit();
	// ���ʎ�
	void UpdateDie();

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

	// ���S����
	void Death() override;

private:
	int mAttackTime;   // �U�����Ԃ̊Ԋu
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eAttack,	// �U��
		eHit,       // �q�b�g
		eDie,       // ����
		eRun,		// �ړ�

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �G�C�̃C���X�^���X
	static CRay* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �G�C�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eAttack,	// �U��
		eAttackWait,// �U���I���҂�
		eHit,       // �q�b�g
		eDie,       // ���ʎ�
		eRun,		// �ړ�
	};
	EState mState;	// �G�C�̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;  // �_���[�W���󂯂�R���C�_�[
	CColliderSphere* mpAttackCol;  // �_���[�W��^����R���C�_�[
	CTransform* mpRideObject;
};