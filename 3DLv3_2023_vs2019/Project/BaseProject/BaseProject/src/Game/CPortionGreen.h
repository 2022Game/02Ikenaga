#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"

/*
�񕜂̃|�[�V�����N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CPortionGreen : public CObjectBase
{
public:
	// �R���X�g���N
	CPortionGreen();
	// �f�X�g���N�^
	~CPortionGreen();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	CModel* mpPortionGreen;  // ���f���f�[�^�ǂݍ���
	CColliderSphere* mpColliderSphere;  // ��(�Փˏ����p)
};
