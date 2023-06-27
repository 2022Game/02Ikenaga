#pragma once
#ifndef CENEMY2_H
#define CENEMY2_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/

class CEnemy2 : public CCharacter3 {
public:
	static CEnemy2* Instance();
	//�R���X�g���N�^
	CEnemy2();
	void Collision();
	//CEnemy2(�ʒu,��],�g�k)
	CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//�R���X�g���N�^
	//CEnemy2(���f��,�ʒu,��],�g�k)
	CEnemy2(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	static CEnemy2* spInstance;
	int mark;
	int mark2;
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
};

#endif