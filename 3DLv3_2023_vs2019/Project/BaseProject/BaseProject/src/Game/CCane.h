#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
��N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CCane : public CWeapon
{
public:
	CCane();
	~CCane();

	void Update();
	void Render();

private:

	CModel* mpCane;
};
