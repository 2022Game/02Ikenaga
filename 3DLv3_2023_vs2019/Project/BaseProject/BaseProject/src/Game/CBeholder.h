#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CLightningBallEffect;
class CElectricShockEffect;

/*
 ���̂̃����X�^�[
 �v���C���[�̐��背�x��(61�`70)
 �G�l�~�[�N���X���p��
*/
class CBeholder : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CBeholder* Instance();

	// �R���X�g���N�^
	CBeholder();
	~CBeholder();

	// �키�O�̑ҋ@���
	void UpdateIdle();
	// �키�O�̑ҋ@���2
	void UpdateIdle2();

	// �U��
	void UpdateAttack();
	// �U��2
	void UpdateAttack2();
	// �U��3
	void UpdateAttack3();
	// �U��4
	void UpdateAttack4();
	// �U���I���҂�
	void UpdateAttackWait();

	//�q�b�g
	void UpdateHit();
	// ���ʎ�
	void UpdateDie();
	// �߂܂�(����)
	void UpdateDizzy();

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
		eIdle2,		// �ҋ@2
		eAttack,	// �U��
		eAttack2,	// �U��2
		eAttack3,	// �U��3
		eAttack4,	// �U��4
		eHit,       // �q�b�g
		eDie,       // ����
		eDizzy,     // �߂܂�(����)
		eRun,		// �ړ�

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// ���̂̃����X�^�[�̃C���X�^���X
	static CBeholder* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// ���̂̃����X�^�[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eIdle2,		// �ҋ@2
		eAttack,	// �U��
		eAttack2,	// �U��2
		eAttack3,	// �U��3
		eAttack4,	// �U��4
		eAttackWait,// �U���I���҂�
		eHit,       // �q�b�g
		eDie,       // ���ʎ�
		eDizzy,     // �߂܂�(����)
		eRun,       // �ړ�
	};
	EState mState;	// ���̂̃����X�^�[�̏��

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CColliderLine* mpColliderLine2;

	CColliderSphere* mpColliderSphereBody;       // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpColliderSphereTentacle;   // �L�����N�^�[�����߂��R���C�_�[(����̐G��)
	CColliderSphere* mpColliderSphereTentacle2;  // �L�����N�^�[�����߂��R���C�_�[(�E��̐G��)
	CColliderSphere* mpColliderSphereTentacle3;  // �L�����N�^�[�����߂��R���C�_�[(�����̐G��)
	CColliderSphere* mpColliderSphereTentacle4;  // �L�����N�^�[�����߂��R���C�_�[(�E���̐G��)
	CColliderSphere* mpColliderSphereTentacle5;  // �L�����N�^�[�����߂��R���C�_�[(�^�񒆏�̐G��)
	CColliderSphere* mpColliderSphereTentacle6;  // �L�����N�^�[�����߂��R���C�_�[(�^�񒆉��̐G��)

	CColliderSphere* mpDamageColBody;       // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpDamageColTentacle;   // �_���[�W���󂯂�R���C�_�[(����̐G��)
	CColliderSphere* mpDamageColTentacle2;  // �_���[�W���󂯂�R���C�_�[(�E��̐G��)
	CColliderSphere* mpDamageColTentacle3;  // �_���[�W���󂯂�R���C�_�[(�����̐G��)
	CColliderSphere* mpDamageColTentacle4;  // �_���[�W���󂯂�R���C�_�[(�E���̐G��)
	CColliderSphere* mpDamageColTentacle5;  // �_���[�W���󂯂�R���C�_�[(�^�񒆏�̐G��)
	CColliderSphere* mpDamageColTentacle6;  // �_���[�W���󂯂�R���C�_�[(�^�񒆉��̐G��)

	CColliderSphere* mpAttackColBody;       // �_���[�W��^����R���C�_�[(��)
	CColliderSphere* mpAttackColTentacle;   // �_���[�W��^����R���C�_�[(����̐G��)
	CColliderSphere* mpAttackColTentacle2;  // �_���[�W��^����R���C�_�[(�E��̐G��)
	CColliderSphere* mpAttackColTentacle3;  // �_���[�W��^����R���C�_�[(�����̐G��)
	CColliderSphere* mpAttackColTentacle4;  // �_���[�W��^����R���C�_�[(�E���̐G��)
	CColliderSphere* mpAttackColTentacle5;  // �_���[�W��^����R���C�_�[(�^�񒆏�̐G��)
	CColliderSphere* mpAttackColTentacle6;  // �_���[�W��^����R���C�_�[(�^�񒆉��̐G��)
	CTransform* mpRideObject;

	CLightningBallEffect* mpLightningBall;  // ����
	CElectricShockEffect* mpElectricShock;  // �d��
};