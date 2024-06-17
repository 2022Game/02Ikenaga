#ifndef CRICH_H
#define CRICH_H
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CModel.h"
class CCane;

/*
���b�`�N���X
�L�����N�^�N���X���p��
*/
class CRich : public CXCharacter
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CRich* Instance();

	// �R���X�g���N�^
	CRich();
	// �f�X�g���N�^
	~CRich();

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

	// �X�V����
	void Update();

	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

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

	// �`��
	void Render();
private:
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
		eWalk,	   // ���s

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// ���b�`�̃C���X�^���X
	static CRich* spInstance;

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
	};
	EState mState;	  // �G�l�~�[�̏��

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	bool mIsGrounded; // �ڒn���Ă��邩�ǂ���

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
	CCane* mpCane;  // ��
};

#endif
