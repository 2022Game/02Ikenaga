#pragma once
#ifndef CENEMY_H
#define CENEMY_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include "CColliderMesh.h"
#include "CColliderLine.h"

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/

class CEnemy : public CCharacter3{
public:
	static CEnemy* Instance();
	//�R���X�g���N�^
	CEnemy();
	void Collision();
	//CEnemy3(�ʒu,��],�g�k)
	CEnemy(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//�R���X�g���N�^
	//CEnemy(���f��,�ʒu,��],�g�k)
	CEnemy(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CCollider mCollider;
	CCollider mCollider2;
	CCollider mCollider3;
	CColliderLine mLine3; //�����R���C�_
	CColliderLine mLine2; //�����R���C�_
	CColliderLine mLine; //�����R���C�_
	static CEnemy* spInstance;
	int mark;
	int mark2;
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
};

#endif