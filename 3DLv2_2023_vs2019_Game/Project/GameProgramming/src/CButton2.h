#pragma once
#ifndef CBUTTON2_H
#define CBUTTON2_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include"CCollider.h"
#include"CColliderLine.h"
#include"CColliderMesh.h"

/*
�{�^���N���X
�L�����N�^�N���X���p��
*/

class CButton2 : public CCharacter3 {
public:
	void Set(float w, float d);
	//�R���X�g���N�^
	CButton2();
	void Collision();
	//CWater(�ʒu,��],�g�k)
	CButton2(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//�R���X�g���N�^
	//CButton2(���f��,�ʒu,��],�g�k)
	CButton2(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CColliderMesh mColliderMesh1;
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
};

#endif
