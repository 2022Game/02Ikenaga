#pragma once
#pragma once
#ifndef CEXCLAMATIONMARK_H
#define CEXCLAMATIONMARK_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/

class CExclamationMark : public CCharacter3 {
public:
	void Set(float w, float d);
	//�R���X�g���N�^
	CExclamationMark();
	void Collision();
	//CEnemy3(�ʒu,��],�g�k)
	CExclamationMark(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	////�m�F�p���\�b�h�@�폜�\��
	//void CEnemy::Render() {
	//	CCharacter3::Render();
	//	mCollider1.Render();
	//	mCollider2.Render();
	//	mCollider3.Render();
	//}
	//�R���X�g���N�^
	//CEnemy(���f��,�ʒu,��],�g�k)
	CExclamationMark(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	int mark;
	CVector mPoint;  //�ڕW�n�_
	//���f���f�[�^
	static CModel sModel;
	CCollider mCollider;
	CColliderMesh mColliderMesh1;
	//�R���C�_
	CCollider mCollider1;
	CCollider mCollider2;
	CCollider mCollider3;
};

#endif