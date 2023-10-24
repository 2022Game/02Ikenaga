#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
���N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CSword : public CWeapon
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

