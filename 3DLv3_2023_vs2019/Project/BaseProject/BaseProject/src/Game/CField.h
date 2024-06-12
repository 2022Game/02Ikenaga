#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
 �t�B�[���h
*/
class CField : public CObjectBase
{
public:
	// �R���X�g���N
	CField();
	//�@�f�X�g���N�^
	~CField();

	// �R���C�_�[���擾
	CCollider* GetCollider() const;

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;
};