#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 ���N���X
 ����N���X�p��
*/
class CShield : public CWeapon
{
public:
	// �R���X�g���N
	CShield();
	// �f�X�g���N�^
	~CShield();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	// ���f���f�[�^�ǂݍ���
	CModel* mpShield;
};