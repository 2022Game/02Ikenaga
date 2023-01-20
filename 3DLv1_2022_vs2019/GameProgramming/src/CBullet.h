#pragma once
#ifndef CBULLET_H
#define CBULLET_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter3.h"
#include"CCollider.h"

/*
�e�N���X
�O�p�`�N���X�̃C���N���[�h
*/

class CBullet : public CCharacter3
{
public:
	void Collision();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	/*void CBullet::Render() {
		CCharacter3::Render();
		mCollider.Render();
	}*/
	CBullet();
	//���Ɖ��s���̐ݒ�
	//Set(��,���s)
	void Set(float w, float d);
	//�X�V
	void Update();
	void Render();
private:
	CCollider mCollider;
	//��������
	int mLife;
	//�O�p�`
	CTriangle mT;
};

#endif