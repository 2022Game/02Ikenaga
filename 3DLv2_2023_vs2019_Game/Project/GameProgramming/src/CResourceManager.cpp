#include "CResourceManager.h"

//CResourceManagerのインスタンス
CResourceManager* CResourceManager::mpInstance = nullptr;

//インスタンス取得
CResourceManager* CResourceManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CResourceManager();
	}
	return mpInstance;
}

//コンストラクタ
CResourceManager::CResourceManager()
{
}

//デストラクタ
CResourceManager::~CResourceManager()
{
	//読み込み済みのモデルデータを破棄
	for (auto& m : mModelList)
	{
		delete m.second;
	}
	mModelList.clear();
}

//モデルデータ読み込み
CModel* CResourceManager::LoadModel(std::string name, std::string objPath, std::string mtlPath)
{
	//読み込むモデルデータが既に読み込み済みであれば、
	//読み込み済みのモデルデータを返してやる
	auto find = mModelList.find(name);
	if (find != mModelList.end()) return find->second;

	//読み込み済みでなければ、モデルデータ読み込み
	CModel* model = new CModel();
	if (!model->Load(objPath.c_str(), mtlPath.c_str()))
	{
		//モデルデータが読み込めなかった
		delete model;
		return nullptr;
	}

	//読み込んだモデルデータをリストに追加
	mModelList.insert(make_pair(name, model));

	//読み込んだモデルデータを返す
	return model;
}

//モデルデータを読み込んでいるかどうか
bool CResourceManager::IsLoadedModel(std::string name) const
{
	return GetModel(name) != nullptr;
}

//読み込み済みのモデルデータを取得
CModel* CResourceManager::GetModel(std::string name) const
{
	//読み込み済みのモデルデータのリストを検索
	auto find = mModelList.find(name);

	//モデルデータが読み込み済みであれば返す
	if (find != mModelList.end()) return find->second;

	//モデルデータが読み込まれてなければ、nullptrを返す
	return nullptr;
}
