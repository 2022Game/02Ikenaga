#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
�o���l�N���X
�I�u�W�F�N�g�x�[�X�p��
*/
class CBall : public CObjectBase
{
public:
	CBall();
	~CBall();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpBall;
};