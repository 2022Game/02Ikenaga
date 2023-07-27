#pragma once
#ifndef CMEAT_H
#define CMEAT_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"
#include "CColliderLine.h"

/*
�Θg�N���X
�L�����N�^�N���X���p��
*/

class CMeat : public CCharacter3 {
public:
	void Set(float w, float d);
	//�R���X�g���N�^
	CMeat();
	void Collision();
	//CMeat(�ʒu,��],�g�k)
	CMeat(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//�R���X�g���N�^
	//CFrame(���f��,�ʒu,��],�g�k)
	CMeat(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
	CColliderLine mLine; //�����R���C�_
	CCollider mCollider;
	//CColliderMesh mColliderMesh1;
};

#endif
