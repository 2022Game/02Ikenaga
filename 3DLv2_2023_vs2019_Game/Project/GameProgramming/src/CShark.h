#pragma once
#ifndef CSHARK_H
#define CSHARK_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include"CCollider.h"
#include"CColliderLine.h"

/*
�Θg�N���X
�L�����N�^�N���X���p��
*/

class CShark : public CCharacter3 {
public:
	void Set(float w, float d);
	//�R���X�g���N�^
	CShark();
	void Collision();
	//CWater(�ʒu,��],�g�k)
	CShark(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//�R���X�g���N�^
	//CShark(���f��,�ʒu,��],�g�k)
	CShark(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
	CCollider mCollider;
	CColliderLine mLine3; //�����R���C�_
	CColliderLine mLine2; //�����R���C�_
	CColliderLine mLine; //�����R���C�_
};

#endif


