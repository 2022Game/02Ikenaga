#pragma once
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

class CShieldRotate2;
class CHit;

/*
 �T
 �v���C���[�̐��背�x��(11�`20)
 �G�l�~�[�N���X���p��
*/
class CTurtle : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CTurtle* Instance();

	// �R���X�g���N�^
	CTurtle();
	// �f�X�g���N�^
	~CTurtle();

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
	// �ҋ@
	void UpdateIdle();
	// �ҋ@2
	void UpdateIdle2();
	// �ҋ@3
	void UpdateIdle3();

	// �U��
	void UpdateAttack();
	// �U��2
	void UpdateAttack2();
	// �U���I���҂�
	void UpdateAttackWait();

	// �q�b�g
	void UpdateHit();
	// �h��
	void UpdateDefense();
	// �h�䒆�̃q�b�g
	void UpdateDefenseHit();
	// �h�䒆�̑ҋ@
	void UpdateDefenseIdle();
	// ���ʎ�
	void UpdateDie();
	// �߂܂�(����)
	void UpdateDizzy();
	// �ړ�
	void UpdateRun();

	int mDefenseTime;  // �h�䎞��
	int mAttackTime;   // ���̍U������

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		   // T�|�[�Y
		eIdle,		   // �ҋ@
		eIdle2,		   // �ҋ@2
		eIdle3,        // �ҋ@3
		eIdle4,        // �ҋ@4
		eAttack,	   // �U��
		eAttack2,	   // �U��2
		eHit,          // �q�b�g
		eDefense,      // �h��
		eDefenseHit,   // �h�䒆�̃q�b�g
		eDefenseIdle,  // �h�䒆�̑ҋ@
		eDie,          // ����
		eDizzy,        // �߂܂�(����)
		eRun,		   // �ړ�

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �T�̃C���X�^���X
	static CTurtle* spInstance;

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

	// �T�̏��
	enum class EState
	{
		eIdle,		   // �ҋ@
		eIdle2,		   // �ҋ@2
		eIdle3,        // �ҋ@3
		eAttack,	   // �U��
		eAttack2,	   // �U��2
		eAttackWait,   // �U���I���҂�
		eHit,          // �q�b�g
		eDefense,      // �h��
		eDefenseHit,   // �h�䒆�̃q�b�g
		eDefenseIdle,  // �h�䒆�̑ҋ@
		eDie,          // ���ʎ�
		eDizzy,        // �߂܂�(����)
		eRun,          // �ړ�
	};
	EState mState;	 // �T�̏��
	int mStateStep;  // State���̃X�e�b�v

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	CVector mMoveSpeed;	    // �ړ����x
	bool mIsGrounded;       // �ڒn���Ă��邩�ǂ���

	 // �L�����N�^�[�̐����R���C�_�[
	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphereBody;  // �L�����N�^�[�����߂��R���C�_�[(��)
	CColliderSphere* mpDamageColBody;       // �_���[�W���󂯂�R���C�_�[(��)
	CColliderSphere* mpAttackColBody;       // �_���[�W��^����R���C�_�[(��)
	CTransform* mpRideObject;

	// ��]����V�[���h
	CShieldRotate2* mpShieldRotate;
	CShieldRotate2* mpShieldRotate2;
	CShieldRotate2* mpShieldRotate3;
	CShieldRotate2* mpShieldRotate4;

	CHit* mpHitEffect;  // �q�b�g�G�t�F�N�g 
};
