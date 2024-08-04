#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CImpactEffect;
class CShieldRotate2;
class CHit;
class CDizzyEffect;

/*
 �{�N�T�[
 �v���C���[�̐��背�x��(71�`80)
 �G�l�~�[�N���X���p��
*/
class CBoxer : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CBoxer* Instance();

	// �R���X�g���N�^
	CBoxer();
	// �f�X�g���N�^
	~CBoxer();

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

	/// <summary>
	/// �h��͂̋����������擾
	/// </summary>
	/// <param name="attackDir"></param>
	/// <returns></returns>
	float GetDefBuff(const CVector& attackDir) const override;

	// ���S����
	void Death() override;

	// �����_���Ɉʒu���擾
	CVector GetRandomSpawnPos()override;

	// �`��
	void Render();

private:

	float mDefenseTime;  // ���̖h�䎞��
	float mAttackTime;   // ���̍U������

	// �ҋ@���
	void UpdateIdle();
	// �ҋ@���2
	void UpdateIdle2();

	// �U��(�p���`)
	void UpdateAttack();
	// �U��2(�R��)
	void UpdateAttack2();
	// �U���I���҂�
	void UpdateAttackWait();

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

	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();

	// �ړ�
	void UpdateRun();
	// ����
	void UpdateSlide();

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		  // T�|�[�Y
		eIdle,		  // �ҋ@
		eIdle2,		  // �ҋ@2
		eAttack,	  // �U��(�p���`)
		eAttack2,	  // �U��2(�R��)
		eHit,         // �q�b�g
		eDefense,     // �h��
		eDefenseHit,  // �h�䒆�̃q�b�g
		eDie,         // ����
		eDizzy,       // �߂܂�(����)
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
		float animSpeed;    // �A�j���[�V�����̍Đ����x
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �{�N�T�[�̏��
	enum class EState
	{
		eIdle,		  // �ҋ@
		eIdle2,		  // �ҋ@2
		eAttack,	  // �U��(�p���`)
		eAttack2,	  // �U��2(�R��)
		eAttackWait,  // �U���I���҂�
		eHit,         // �q�b�g
		eDefense,     // �h��
		eDefenseHit,  // �h�䒆�̃q�b�g
		eDie,         // ���ʎ�
		eDizzy,       // �߂܂�(����)
		eJump,		  // �W�����v��
		eJumpEnd,	  // �W�����v�I��
		eRun,         // �ړ�
		eSlide,       // ����
	};
	EState mState;	// �{�N�T�[�̏��

	int mStateStep;  // State���̃X�e�b�v����

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	// �����R���C�_�[
	CColliderLine* mpColLineSide;    // �����R���C�_�[(��)
	CColliderLine* mpColLineHeight;  // �����R���C�_�[(�c)

	// �L�����N�^�[�����߂��R���C�_�[
	CColliderSphere* mpColliderSphereHead;   // ��
	CColliderSphere* mpColliderSphereBody;   // ��
	CColliderSphere* mpColliderSphereHandR;  // �E��
	CColliderSphere* mpColliderSphereHandL;  // ����
	CColliderSphere* mpColliderSphereFeetR;  // �E��
	CColliderSphere* mpColliderSphereFeetL;  // ����

	// �_���[�W���󂯂�R���C�_�[
	CColliderSphere* mpDamageColHead;        // ��
	CColliderSphere* mpDamageColBody;        // ��
	CColliderSphere* mpDamageColHandR;       // �E��
	CColliderSphere* mpDamageColHandL;       // ����
	CColliderSphere* mpDamageColArmR;        // �E�r
	CColliderSphere* mpDamageColArmL;        // ���r
	CColliderSphere* mpDamageColFeetR;       // �E��
	CColliderSphere* mpDamageColFeetL;       // ����

	// �_���[�W��^����R���C�_�[
	CColliderSphere* mpAttackColHead;         // ��
	CColliderSphere* mpAttackColBody;         // ��
	CColliderSphere* mpAttackColHandR;        // �E��
	CColliderSphere* mpAttackColFeetR;        // �E��
	CColliderSphere* mpAttackColFeetL;        // ����

	CTransform* mpRideObject;

	// ��]����V�[���h
	CShieldRotate2* mpShieldRotate;
	CShieldRotate2* mpShieldRotate2;
	CShieldRotate2* mpShieldRotate3;
	CShieldRotate2* mpShieldRotate4;

	// �G�t�F�N�g�֘A
	CImpactEffect* mpImpact;  // �Ռ��G�t�F�N�g
	CHit* mpHitEffect;        // �q�b�g�G�t�F�N�g
	// �߂܂��̃G�t�F�N�g
	CDizzyEffect* mpDizzyEffect;
};