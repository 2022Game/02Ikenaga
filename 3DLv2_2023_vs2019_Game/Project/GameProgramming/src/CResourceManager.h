#pragma once
#include <string>
#include <map>
#include "CModel.h"

//���\�[�X�Ǘ��N���X
class CResourceManager
{
public:
	//�C���X�^���X���擾
	static CResourceManager* Instance();

	//�R���X�g���N�^
	CResourceManager();
	//�f�X�g���N�^
	~CResourceManager();

	//���f���f�[�^�ǂݍ���
	//(���\�[�X�Ǘ��p�̖��O�A���f���f�[�^�̃p�X�A�}�e���A���f�[�^�̃p�X)
	CModel* LoadModel(std::string name, std::string objPath, std::string mtlPath);
	//���f���f�[�^��ǂݍ���ł��邩�ǂ���
	bool IsLoadedModel(std::string name) const;
	//�ǂݍ��ݍς݂̃��f���f�[�^���擾
	CModel* GetModel(std::string name) const;

private:
	//CResourceManager�̃C���X�^���X
	static CResourceManager* mpInstance;

	//�ǂݍ��ݍς݂̃��f���f�[�^�̃��X�g
	std::map<std::string, CModel*> mModelList;
};

#define LOAD_MODEL(name, obj, mtl) CResourceManager::Instance()->LoadModel(name, obj, mtl)
#define GET_MODEL(name) CResourceManager::Instance()->GetModel(name)