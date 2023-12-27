#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CModel.h"
#include "CEnemy.h"
#include "CColliderSphere.h"

/*
 �}�b�V�����[���N���X
�L�����N�^�N���X���p��
*/
class CMushroom : public CEnemy
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CMushroom* Instance();

	// �R���X�g���N�^
	CMushroom();
	~CMushroom();

	// �키�O�̑ҋ@���
	void UpdateIdle();
	// �키�O�̑ҋ@���2
	void UpdateIdle2();
	// �ҋ@���3
	void UpdateIdle3();

	// �U��
	void UpdateAttack();
	// �U���I���҂�
	void UpdateAttackWait();

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

	// ���S����
	void Death() override;

private:
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �키�O�̑ҋ@
		eIdle2,		// �키�O�̑ҋ@2
		eIdle3,     // �ҋ@���3
		eIdle4,     // �ҋ@���4
		eAttack,	// �U��
		eWalk,		// ���s
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �}�b�V�����[���̃C���X�^���X
	static CMushroom* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �}�b�V�����[���̏��
	enum class EState
	{
		eIdle,		// �키�O�̑ҋ@
		eIdle2,		// �키�O�̑ҋ@2
		eIdle3,     // �ҋ@���3
		eAttack,	// �U��
		eAttackWait,// �U���I���҂�
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
	};
	EState mState;	// �}�b�V�����[���̏��

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CColliderSphere* mpColliderSphere;
	CColliderSphere* mpDamageCol;  // �_���[�W���󂯂�R���C�_�[
	CColliderSphere* mpAttackCol;  // �_���[�W��^����R���C�_�[
	CTransform* mpRideObject;
};