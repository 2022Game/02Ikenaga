#include "CEnemy.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CModel.h"

class CMagicCircle;
class CDrainEffect;
class CShieldRotate;
class CHealCircle;

/*
 ���b�`�N���X
 �G�l�~�[�N���X���p��
*/
class CLich : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CLich* Instance();

	// �R���X�g���N�^
	CLich();
	// �f�X�g���N�^
	~CLich();

	// HP�z���܂��͉�
	virtual void DrainHp();

	// �X�V����
	void Update();

	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W</param>
	//�_���[�W��^�����I�u�W�F�N�g
	virtual void TakeDamage(int damage, CObjectBase* causedObj);

	// ���������G�̎��S����
	void DeathSummonEnemy(CEnemy* enemy) override;

	// ���S����
	void Death() override;

	/// <summary>
	/// �h��͂̋����������擾
	/// </summary>
	/// <param name="attackDir"></param>
	/// <returns></returns>
	float GetDefBuff(const CVector& attackDir) const override;

	// �`��
	void Render();

	// 1���x���A�b�v
	void LevelUp();
	// ���x���̕ύX
	void ChangeLevel(int level);

	static float mElapsedTime;  // �o�ߎ���

private:
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

	// �q�b�g
	void UpdateHit();
	// ����
	void UpdateDie();
	// ����
	void UpdateSummon();
	// �ړ�
	void UpdateRun();

	// �����_����������G�̃C���f�b�N�X�l���擾
	int GetRandomSummonIndex() const;
	// �����_������
	void RandomSummon();

	// �G�̏����f�[�^
	struct SpawnData
	{
		EEnemyType type;     // �G�̎��
		float dist;          // ��������
		CVector offsetPos;   // �����ʒu�̃I�t�Z�b�g�ʒu
		CColor circleColor;  // ���@�w�̐F
		float circleScale;   // ���@�w�̃X�P�[���l
		float monsterScale;  // ��������G�̃X�P�[���l
		float spawnRatio;    // ��������^�C�~���O
	};
	// �G�̏����f�[�^�̃e�[�u��
	static const SpawnData SPAWN_DATA[];

	CEnemy* SpawnEnemy(EEnemyType type) const;

	int mAttackTime;  // �U�����Ԃ̊Ԋu

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,	   // T�|�[�Y
		eIdle,	   // �ҋ@
		eAttack,   // �U��
		eAttack2,  // �U��2
		eHit,      // �q�b�g
		eDie,      // ����
		eSummon,   // ����
		eRun,	   // �ړ�

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// ���b�`�̃C���X�^���X
	static CLich* spInstance;

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

	// ���b�`�̏��
	enum class EState
	{
		eIdle,		  // �ҋ@
		eIdle2,		  // �ҋ@2
		eAttack,	  // �U��
		eAttack2,	  // �U��2
		eAttackWait,  // �U���I���҂�
		eHit,         // �q�b�g
		eDie,         // ����
		eSummon,      // ����
		eRun,         // �ړ�
	};
	EState mState;	  // �G�l�~�[�̏��
	int mStateStep;   // State���̃X�e�b�v����

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;	 // �ړ����x
	bool mIsGrounded;    // �ڒn���Ă��邩�ǂ���

	// �����R���C�_�[
	CColliderLine* mpColliderLine;

	// �L�����N�^�[�̉����߂��R���C�_�[
	CColliderCapsule* mpColCapsuleBody;  // ��
	CColliderSphere* mpColSphereArmL;    // ���r
	CColliderSphere* mpColSphereArmR;    // �E�r

	// �_���[�W���󂯂�R���C�_�[
	CColliderCapsule* mpDamageColBody;  // ��
	CColliderSphere* mpDamageColArmL;   // ���r
	CColliderSphere* mpDamageColArmR;   // �E�r
	CTransform* mpRideObject;

	// ��]����V�[���h
	CShieldRotate* mpShieldRotate;
	CShieldRotate* mpShieldRotate2;
	CShieldRotate* mpShieldRotate3;
	CShieldRotate* mpShieldRotate4;

	CMagicCircle* mpMagicCircle;  // ���@�w
	CEnemy* mpSpawnEnemy;         // ���g�����������G�̃|�C���^
	CVector mMCStartPos;          // ���@�w�̃A�j���[�V�����J�n�ʒu
	CQuaternion mMCStartRot;      // ���@�w�̃A�j���[�V�����J�n���̉�]�l
	int mRandomSummonIndex;       // �G���������郉���_���C���f�b�N�X�l

	CDrainEffect* mpDrain;      // �h���C��
	CHealCircle* mpHealCircle;  // �񕜃T�[�N��
};
