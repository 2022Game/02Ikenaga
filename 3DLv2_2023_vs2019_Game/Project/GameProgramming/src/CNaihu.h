#pragma once
#ifndef CNAIHU_H
#define CNAIHU_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter3.h"
#include"CCollider.h"

/*
�i�C�t�N���X
*/

class CNaihu : public CCharacter3 {
public:
	void Set(float w, float d);
	//�R���X�g���N�^
	CNaihu();
	void Collision();
	//(�ʒu,��],�g�kCNaihu
	CNaihu(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//�R���X�g���N�^
	//CNaihu(���f��,�ʒu,��],�g�k)
	CNaihu(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CCollider mCollider;
	//��������
	int mLife;
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
};

#endif


