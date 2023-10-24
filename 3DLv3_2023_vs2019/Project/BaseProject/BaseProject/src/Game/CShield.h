#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
���N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CShield : public CObjectBase
{
public:
	CShield();
	~CShield();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpShield;
};
