#include "CResourceManager.h"

//CResourceManager�̃C���X�^���X
CResourceManager* CResourceManager::mpInstance = nullptr;

//�C���X�^���X�擾
CResourceManager* CResourceManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CResourceManager();
	}
	return mpInstance;
}

//�R���X�g���N�^
CResourceManager::CResourceManager()
{
}

//�f�X�g���N�^
CResourceManager::~CResourceManager()
{
	//�ǂݍ��ݍς݂̃��f���f�[�^��j��
	for (auto& m : mModelList)
	{
		delete m.second;
	}
	mModelList.clear();
}

//���f���f�[�^�ǂݍ���
CModel* CResourceManager::LoadModel(std::string name, std::string objPath, std::string mtlPath)
{
	//�ǂݍ��ރ��f���f�[�^�����ɓǂݍ��ݍς݂ł���΁A
	//�ǂݍ��ݍς݂̃��f���f�[�^��Ԃ��Ă��
	auto find = mModelList.find(name);
	if (find != mModelList.end()) return find->second;

	//�ǂݍ��ݍς݂łȂ���΁A���f���f�[�^�ǂݍ���
	CModel* model = new CModel();
	if (!model->Load(objPath.c_str(), mtlPath.c_str()))
	{
		//���f���f�[�^���ǂݍ��߂Ȃ�����
		delete model;
		return nullptr;
	}

	//�ǂݍ��񂾃��f���f�[�^�����X�g�ɒǉ�
	mModelList.insert(make_pair(name, model));

	//�ǂݍ��񂾃��f���f�[�^��Ԃ�
	return model;
}

//���f���f�[�^��ǂݍ���ł��邩�ǂ���
bool CResourceManager::IsLoadedModel(std::string name) const
{
	return GetModel(name) != nullptr;
}

//�ǂݍ��ݍς݂̃��f���f�[�^���擾
CModel* CResourceManager::GetModel(std::string name) const
{
	//�ǂݍ��ݍς݂̃��f���f�[�^�̃��X�g������
	auto find = mModelList.find(name);

	//���f���f�[�^���ǂݍ��ݍς݂ł���ΕԂ�
	if (find != mModelList.end()) return find->second;

	//���f���f�[�^���ǂݍ��܂�ĂȂ���΁Anullptr��Ԃ�
	return nullptr;
}
