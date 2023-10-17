#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
�ړ����x�A�b�v�̃|�[�V�����N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CPortionBlue : public CObjectBase
{
public:
	CPortionBlue();
	~CPortionBlue();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpPortionBlue;
};
