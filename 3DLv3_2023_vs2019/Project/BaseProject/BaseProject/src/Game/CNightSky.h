#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
 ���
*/
class CNightSky : public CObjectBase
{
public:
	// �R���X�g���N
	CNightSky();
	//�@�f�X�g���N�^
	~CNightSky();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	CModel* mpModel;
	float mElapsedTime;	  // �o�ߎ���
};