#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
�񕜂̃|�[�V�����N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CPortionGreen : public CObjectBase
{
public:
	CPortionGreen();
	~CPortionGreen();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpPortionGreen;
};
