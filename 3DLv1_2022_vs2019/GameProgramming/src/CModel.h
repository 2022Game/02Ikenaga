#ifndef CMODEL_H
#define CMODEL_H

//Vector�̃C���N���[�h
#include<vector>
#include"CTriangle.h"
#include"CMaterial.h"

/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
class CModel {
public:
	//�`��
	//Render(�s��)
	void Render(const CMatrix& m);
	~CModel();
	//���f���t�@�C���̓���
	//Load(���f���t�@�C����,�}�e���A���t�@�C����
	void Load(char* obj,char* mtl);
	//�`��
	void Render();
private:
	//�}�e���A���|�C���^�̉ϒ��z��
	std::vector<CMaterial*> mpMaterials;
	std::vector<CTriangle> mTriangles;
};

#endif 

