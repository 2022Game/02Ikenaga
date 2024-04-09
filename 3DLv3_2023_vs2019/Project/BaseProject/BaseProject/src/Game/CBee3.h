#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 �I3
 �v���C���[�̐��背�x��(40)
 �G�l�~�[�N���X���p��
*/
class CBee3 : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CBee3* Instance();

	// �R���X�g���N�^
	CBee3();
	~CBee3();

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
	int mFlyingTime;   // ��s����
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

	// �I3�̃C���X�^���X
	static CBee3* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �I3�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eAttack,	// �U��
		eAttackWait,// �U���I���҂�
		eHit,       // �q�b�g
		eDie,       // ���ʎ�
		eRun,       // �ړ�
	};
	EState mState;	// �I2�̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsSpawnedNeedleEffect;

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;   // �_���[�W���󂯂�R���C�_�[ (��)
	CColliderSphere* mpDamageCol2;  // �_���[�W���󂯂�R���C�_�[2 (�K��)
	CColliderSphere* mpAttackCol;   // �_���[�W��^����R���C�_�[
	CTransform* mpRideObject;
};