#pragma once
#ifndef CWATER_H
#define CWATER_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
���N���X
�L�����N�^�N���X���p��
*/

class CWater : public CCharacter3 {
public:
	void Set(float w, float d);
	//�R���X�g���N�^
	CWater();
	void Collision();
	//CWater(�ʒu,��],�g�k)
	CWater(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//�R���X�g���N�^
	//CWater(���f��,�ʒu,��],�g�k)
	CWater(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
};

#endif

