#pragma once
#include "CTask.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CField :public CTask
{
public:
	//�R���X�g���N�^
	CField();
	//�f�X�g���N�^
	~CField();

	void Update();
	void Render();

private:
	CModel* mModel;  //�t�B�[���h�̃��f���f�[�^
	CColliderMesh mColliderMesh;
};