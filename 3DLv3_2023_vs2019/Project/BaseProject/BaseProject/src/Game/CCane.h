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
	// �R���X�g���N
	CCane();
	// �f�X�g���N�^
	~CCane();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	// ���f���f�[�^�ǂݍ���
	CModel* mpCane;
};
