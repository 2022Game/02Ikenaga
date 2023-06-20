#pragma once
#ifndef CCLEAR_H
#define CCLEAR_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
�N���A�N���X
�L�����N�^�N���X���p��
*/

class CClear : public CCharacter3 {
public:
	void Set(float w, float d);
	//�R���X�g���N�^
	CClear();
	void Collision();
	//CClear(�ʒu,��],�g�k)
	CClear(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//�R���X�g���N�^
	//CClear(���f��,�ʒu,��],�g�k)
	CClear(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	int mark;
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
	//CCollider mCollider;
	CColliderMesh mColliderMesh1;
};

#endif
