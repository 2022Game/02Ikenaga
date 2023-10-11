#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CColliderLine.h"

#include "CModel.h"

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy : public CXCharacter
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CEnemy* Instance();

	// �R���X�g���N�^
	CEnemy();
	~CEnemy();

	// �X�V����
	void Update();

	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	void Collision(CCollider* self, CCollider* other, const CVector& adjust);

	// �`��
	void Render();
private:
	// �G�l�~�[�̃C���X�^���X
	static CEnemy* spInstance;

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���

	CColliderLine* mpColliderLine;
	CTransform* mpRideObject;
};

#endif
