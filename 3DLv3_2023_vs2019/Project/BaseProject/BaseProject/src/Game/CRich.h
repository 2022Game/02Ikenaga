#ifndef CRICH_H
#define CRICH_H
#include "CXCharacter.h"
#include "CColliderLine.h"

#include "CModel.h"

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
	~CRich();

	// �ҋ@���
	void UpdateIdle();

	// �X�V����
	void Update();

	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render();
private:
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eWalk,		// ���s
		eAttack,	// �U��
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��

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
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// ���b�`�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eAttack,	// �U��
		eAttackWait,// �U���I���҂�
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
	};
	EState mState;	// �G�l�~�[�̏��

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CTransform* mpRideObject;
};

#endif
