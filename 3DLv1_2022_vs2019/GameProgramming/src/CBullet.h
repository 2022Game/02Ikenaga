#pragma once
#ifndef CBULLET_H
#define CBULLET_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter3.h"

/*
�e�N���X
�O�p�`�N���X�̃C���N���[�h
*/

class CBullet : public CCharacter3
{
public:
	//���Ɖ��s���̐ݒ�
	//Set(��,���s)
	void Set(float w, float d);
	//�X�V
	void Update();
	void Render();
private:
	//�O�p�`
	CTriangle mT;
};

#endif