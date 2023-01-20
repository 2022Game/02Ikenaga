#pragma once
#ifndef CCOLLIDERTRIANGLE_H
#define CCOLLIDERTRIANGLE_H
#include"CCollider.h"

/*
�O�p�`�R���C�_�̒�`
*/

class CColliderTriangle :public CCollider
{
public:
	CColliderTriangle(){}
	void ChangePriority();
	//�R���X�g���N�^(�O�p�R���C�_)
	//CColliderTriangle(�e,�V�s��,���_1,���_2,���_3)
	CColliderTriangle(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1, const CVector& v2);
	//�O�p�R���C�_�̐ݒ�
	//Set(�e,�V�s��,���_1,���_2,���_3)
	void Set(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1, const CVector& v2);
	//�`��
	void Render();
};

#endif
