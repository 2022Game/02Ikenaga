#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
���N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CShield : public CWeapon
{
public:
	CShield();
	~CShield();

	void Update();
	void Render();

private:

	CModel* mpShield;
};
