#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CHit;

/*
 �I
 �v���C���[�̐��背�x��(31�`40)
 �G�l�~�[�N���X���p��
*/
class CBee : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CBee* Instance();

	// �R���X�g���N�^
	CBee();

	// �f�X�g���N�^
	~CBee();

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
	//�_���[�W��^�����I�u�W�F�N�g
	virtual void TakeDamage(int damage, CObjectBase* causedObj);

	// ���S����
	void Death() override;

	// �`��
	void Render();

private:

	// �ҋ@���
	void UpdateIdle();
	// �ҋ@���2
	void UpdateIdle2();

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

	int mAttackTime;   // �U�����Ԃ̊Ԋu
	int mFlyingTime;   // ��s����

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,	  // T�|�[�Y
		eIdle,	  // �ҋ@
		eAttack,  // �U��
		eHit,     // �q�b�g
		eDie,     // ����
		eRun,	  // �ړ�

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �I�̃C���X�^���X
	static CBee* spInstance;

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

	// �I�̏��
	enum class EState
	{
		eIdle,		  // �ҋ@
		eIdle2,		  // �ҋ@2
		eAttack,	  // �U��
		eAttackWait,  // �U���I���҂�
		eHit,         // �q�b�g
		eDie,         // ���ʎ�
		eRun,         // �ړ�
	};
	EState mState;	  // �I�̏��

	int mStateStep;  // State���̃X�e�b�v����

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsSpawnedNeedleEffect;

	// �L�����N�^�[�̐����R���C�_�[
	CColliderLine* mpColliderLine;

	// �L�����N�^�[�����߂��R���C�_�[
	CColliderSphere* mpColliderSphereHead;   // ��
	CColliderSphere* mpColliderSphereBeak;   // ���΂�
	CColliderSphere* mpColliderSphereBeak2;  // ���΂�2
	CColliderSphere* mpColliderSphereBody;   // ��
	CColliderSphere* mpColliderSphereTail;   // �K��
	CColliderSphere* mpColliderSphereTail2;  // �K��2
	CColliderSphere* mpColliderSphereTail3;  // �K��3
	CColliderSphere* mpColliderSphereTail4;  // �K��4
	CColliderSphere* mpColliderSphereTail5;  // �K��5

	 // �_���[�W���󂯂�R���C�_�[
	CColliderSphere* mpDamageColHead;    // ��
	CColliderSphere* mpDamageColBeak;    // ���΂�
	CColliderSphere* mpDamageColBeak2;   // ���΂�2
	CColliderSphere* mpDamageColBody;    // ��
	CColliderSphere* mpDamageColTail;    // �K��
	CColliderSphere* mpDamageColTail2;   // �K��2
	CColliderSphere* mpDamageColTail3;   // �K��3
	CColliderSphere* mpDamageColTail4;   // �K��4
	CColliderSphere* mpDamageColTail5;   // �K��5

	 // �_���[�W��^����R���C�_�[
	CColliderSphere* mpAttackCol;        // �j

	CTransform* mpRideObject;

	// �G�t�F�N�g�֘A
	CHit* mpHitEffect;        // �q�b�g�G�t�F�N�g
};