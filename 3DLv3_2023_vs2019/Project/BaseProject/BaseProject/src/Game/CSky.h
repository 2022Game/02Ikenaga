#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
 ��
*/
class CSky : public CObjectBase
{
public:
	// �R���X�g���N
	CSky();
	//�@�f�X�g���N�^
	~CSky();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	CModel* mpModel;
};