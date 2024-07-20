#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"

class CHit;

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

	// �f�X�g���N�^
	~CRay();

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

	int mAttackTime;   // ���̍U������
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

	// �g���̃G�t�F�N�g���쐬
	void CreateWave();

	// �G�C�̃C���X�^���X
	static CRay* spInstance;

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

	// �G�C�̏��
	enum class EState
	{
		eIdle,		  // �ҋ@
		eIdle2,		  // �ҋ@2
		eAttack,	  // �U��
		eAttackWait,  // �U���I���҂�
		eHit,         // �q�b�g
		eDie,         // ���ʎ�
		eRun,		  // �ړ�
	};
	EState mState;	  // �G�C�̏��

	int mStateAttackStep;  // State���̍U���ł̃X�e�b�v����
	int mStateWave;        // State���̔g���̃X�e�b�v����

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	// �����R���C�_�[
	CColliderLine* mpColliderLine;

	// �L�����N�^�[�̉����߂��R���C�_�[
	CColliderSphere* mpColliderSphereHead;  // ��
	CColliderSphere* mpColliderSphereBody;  // ��

	 // �_���[�W���󂯂�R���C�_�[
	CColliderCapsule* mpDamageColSpine;     // �Ғ�
	CColliderSphere* mpDamageColFinLeft;    // �q���̍�
	CColliderSphere* mpDamageColFinRight;   // �q���̉E

	// �U���R���C�_�[
	CColliderSphere* mpAttackColHead;       //��

	CTransform* mpRideObject;

	// �G�t�F�N�g�֘A
	bool mIsSpawnedWaveEffect;  // �g���G�t�F�N�g
	float mElapsedWaveTime;     // �o�ߎ��Ԍv���p(�g��)
	CHit* mpHitEffect;          // �q�b�g�G�t�F�N�g
};