#pragma once
#ifndef CPLAYER_H
#define CPLAYER_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include "CInput.h"
#include"CBullet.h"
#include"CColliderLine.h"
#include "CColliderMesh.h"

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/

class CPlayer : public CCharacter3 {
public:
	bool IsDeath() const;
	static int Hp();
	//CModel* model;
	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();
	void Collision();
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	//CBullet bullet;
	CPlayer();
	//CPayer(�ʒu,��],�X�P�[��)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	CPlayer(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
private:
	static int sHp;	//HP
	int jump;
	CCollider mCollider;
	//CColliderMesh mColliderMesh1;
	//�v���C���[�̃C���X�^���X
	static CPlayer* spInstance;
	//CColliderLine mLine4; //�����R���C�_
	CColliderLine mLine3; //�����R���C�_
	CColliderLine mLine2; //�����R���C�_
	CColliderLine mLine; //�����R���C�_
	CInput mInput;
};

#endif