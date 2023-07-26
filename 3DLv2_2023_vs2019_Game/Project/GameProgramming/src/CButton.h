#pragma once
#ifndef CBUTTON_H
#define CBUTTON_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"
#include "CColliderLine.h"

/*
�{�^���N���X
�L�����N�^�N���X���p��
*/

class CButton : public CCharacter3 {
public:
	static CButton* Instance();
	//�R���X�g���N�^
	CButton();
	void Collision();
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//�R���X�g���N�^
	//CButton(���f��,�ʒu,��],�g�k)
	CButton(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CColliderLine mLine3; //�����R���C�_
	CColliderLine mLine2; //�����R���C�_
	CColliderLine mLine; //�����R���C�_
	static CButton* spInstance;
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
	//CCollider mCollider;
	CColliderMesh mColliderMesh1;
};

#endif
