#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
���N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CSword : public CObjectBase
{
public:
	CSword();
	~CSword();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpSword;
};

