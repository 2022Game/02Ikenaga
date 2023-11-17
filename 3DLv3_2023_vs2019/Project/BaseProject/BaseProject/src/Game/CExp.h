#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"

/*
�o���l�N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CExp : public CObjectBase
{
public:
	CExp();
	~CExp();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	void Update();
	void Render();

private:
	CModel* mpExp;

	CColliderSphere* mpColliderSphere;
};