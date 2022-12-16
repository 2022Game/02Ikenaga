#pragma once
#ifndef CCOLLIDER_H
#define CCOLLIDER_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"

/*
�R���C�_�N���X
�Փ˔���f�[�^
*/

class CCollisionManager;
class CCollider :public CTransform,public CTask {
	friend CCollisionManager;
public:
	//�Փˏ���
	//CCollision(�R���C�_1,�R���C�_2)
	//retrun:true(�Փ˂��Ă���)false(�Փ˂��Ă��Ȃ�)
	static bool CCollision(CCollider* m, CCollider* o);
	~CCollider();
	//�R���X�g���N�^
	//CCollider(�e,�V�s��,�ʒu,���a)
	CCollider(CCharacter3* parent, CMatrix* matrix, const CVector& position, float radius);
	//�e�|�C���^�̎擾
	CCharacter3* Parent();
	//�`��
	void Render();
protected:
	CCharacter3* mpParent;//�e
	CMatrix* mpMatrix;//�V�s��
	float mRadius; //���a
};

#endif 
