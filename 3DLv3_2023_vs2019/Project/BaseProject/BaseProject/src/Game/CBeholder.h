#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CLightningBallEffect;
class CElectricShockEffect;
class CHomingBallEffect;
class CCurrent;
class CHit;
class CDizzyEffect;
class CGameEnemyUI;

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

	// �f�X�g���N�^
	~CBeholder();

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

	// �����_���Ɉʒu���擾
	CVector GetRandomSpawnPos()override;

	// �`��
	void Render();

private:

	// �키�O�̑ҋ@���
	void UpdateIdle();
	// �키�O�̑ҋ@���2
	void UpdateIdle2();

	// �U��(�d�C�{�[��)
	void UpdateAttack();
	// �U��2(�d��)
	void UpdateAttack2();
	// �U��3(�d�C�{�[���̃z�[�~���O)
	void UpdateAttack3();
	// �U��4(��]�U���{�G�t�F�N�g)
	void UpdateAttack4();
	// �U���I���҂�
	void UpdateAttackWait();

	//�q�b�g
	void UpdateHit();
	// ���ʎ�
	void UpdateDie();
	// �߂܂�(����)
	void UpdateDizzy();

	float mAttackTime;  // ���̍U������
	float mFlyingTime;  // ��s����  

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eIdle2,		// �ҋ@2
		eAttack,	// �U��(�d�C�{�[��)
		eAttack2,	// �U��2(�d��)
		eAttack3,	// �U��3(�d�C�{�[���̃z�[�~���O)
		eAttack4,	// �U��4(��]�U���{�G�t�F�N�g)
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
		float animSpeed;    // �A�j���[�V�����̍Đ����x
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// ���̂̃����X�^�[�̏��
	enum class EState
	{
		eIdle,		  // �ҋ@
		eIdle2,		  // �ҋ@2
		eAttack,	  // �U��(�d�C�{�[��)
		eAttack2,	  // �U��2(�d��)
		eAttack3,	  // �U��3(�d�C�{�[���̃z�[�~���O)
		eAttack4,	  // �U��4(��]�U���{�G�t�F�N�g)
		eAttackWait,  // �U���I���҂�
		eHit,         // �q�b�g
		eDie,         // ���ʎ�
		eDizzy,       // �߂܂�(����)
		eRun,         // �ړ�
	};
	EState mState;	  // ���̂̃����X�^�[�̏��
	int mStateStep;   // State���̃X�e�b�v����

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	// �g���l�[�h�G�t�F�N�g�𐶐�
	void CreateTornado();

	// �d���G�t�F�N�g�𐶐�
	void CreateCurrent();
	// �d���G�t�F�N�g�Ƀq�b�g����
	void CollisionCurrent();

	void CreateHit();

	CVector mMoveSpeed;	  // �ړ����x
	bool mIsGrounded;	  // �ڒn���Ă��邩�ǂ���

	 // �����R���C�_�[
	CColliderLine* mpColLineSide;    // �����R���C�_�[(��)
	CColliderLine* mpColLineHeight;  // �����R���C�_�[(�c)

	// �L�����N�^�[�����߂��R���C�_�[
	CColliderSphere* mpColliderSphereBody;       // ��
	CColliderSphere* mpColliderSphereTentacle;   // ����̐G��
	CColliderSphere* mpColliderSphereTentacle2;  // �E��̐G��
	CColliderSphere* mpColliderSphereTentacle3;  // �����̐G��
	CColliderSphere* mpColliderSphereTentacle4;  // �E���̐G��
	CColliderSphere* mpColliderSphereTentacle5;  // �^�񒆏�̐G��
	CColliderSphere* mpColliderSphereTentacle6;  // �^�񒆉��̐G��

	 // �_���[�W���󂯂�R���C�_�[
	CColliderSphere* mpDamageColBody;            // ��
	CColliderSphere* mpDamageColTentacle;        // ����̐G��
	CColliderSphere* mpDamageColTentacle2;       // �E��̐G��
	CColliderSphere* mpDamageColTentacle3;       // �����̐G��
	CColliderSphere* mpDamageColTentacle4;       // �E���̐G��
	CColliderSphere* mpDamageColTentacle5;       // �^�񒆏�̐G��
	CColliderSphere* mpDamageColTentacle6;       // �^�񒆉��̐G��

	// �_���[�W��^����R���C�_�[
	CColliderSphere* mpAttackColBody;            // ��
	CColliderSphere* mpAttackColTentacle;        // ����̐G��
	CColliderSphere* mpAttackColTentacle2;       // �E��̐G��
	CColliderSphere* mpAttackColTentacle3;       // �����̐G��
	CColliderSphere* mpAttackColTentacle4;       // �E���̐G��
	CColliderSphere* mpAttackColTentacle5;       // �^�񒆏�̐G��
	CColliderSphere* mpAttackColTentacle6;       // �^�񒆉��̐G��

	CTransform* mpRideObject;

	CLightningBallEffect* mpLightningBall;  // ����
	CElectricShockEffect* mpElectricShock;  // �d��
	CHomingBallEffect* mpHomingBall;  // �z�[�~���O�{�[��

	// �d���G�t�F�N�g
	CCurrent* mpCurrent;
	CCurrent* mpCurrent2;
	CCurrent* mpCurrent3;
	CCurrent* mpCurrent4;
	CCurrent* mpCurrent5;
	CCurrent* mpCurrent6;

	// �q�b�g�G�t�F�N�g
	CHit* mpHitEffect;
	// �߂܂��̃G�t�F�N�g
	CDizzyEffect* mpDizzyEffect;
};