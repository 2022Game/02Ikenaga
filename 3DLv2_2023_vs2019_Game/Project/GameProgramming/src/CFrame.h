#pragma once
#ifndef CFRAME_H
#define CFRAME_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
�Θg�N���X
�L�����N�^�N���X���p��
*/

class CFrame : public CCharacter3 {
public:
	void Set(float w, float d);
	//�R���X�g���N�^
	CFrame();
	void Collision();
	//CFrame(�ʒu,��],�g�k)
	CFrame(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//�R���X�g���N�^
	//CFrame(���f��,�ʒu,��],�g�k)
	CFrame(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
};

#endif
