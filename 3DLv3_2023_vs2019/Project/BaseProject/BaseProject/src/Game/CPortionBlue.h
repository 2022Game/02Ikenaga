#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"

/*
 �h��̓A�b�v�̃|�[�V�����N���X
 �I�u�W�F�N�g�x�[�X�p��
*/
class CPortionBlue : public CObjectBase
{
public:

	// �R���X�g���N
	CPortionBlue();

	// �f�X�g���N�^
	~CPortionBlue();

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

	// ���f���f�[�^�ǂݍ���
	CModel* mpPortionBlue;

	// �����R���C�_�[
	CColliderLine* mpColliderLine;

	// ��(�Փˏ����p)
	CColliderSphere* mpColliderSphere;

	// �h��̓A�b�v
	bool mDefenseUp;
};
