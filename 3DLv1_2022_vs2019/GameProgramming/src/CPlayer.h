#pragma once
#ifndef CPLAYER_H
#define CPLAYER_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include "CInput.h"
#include"CBullet.h"
#include"CColliderLine.h"

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/

class CPlayer : public CCharacter3 {
public:
	void Collision();
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	//CBullet bullet;
	CPlayer();
	//CPayer(�ʒu,��],�X�P�[��)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	//�X�V����
	void Update();
private:
	CColliderLine mLine3; //�����R���C�_
	CColliderLine mLine2; //�����R���C�_
	CColliderLine mLine; //�����R���C�_
	CInput mInput;
};

#endif