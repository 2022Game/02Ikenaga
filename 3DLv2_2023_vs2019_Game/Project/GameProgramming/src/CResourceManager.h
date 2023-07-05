#pragma once
#include <string>
#include <map>
#include "CModel.h"

//リソース管理クラス
class CResourceManager
{
public:
	//インスタンスを取得
	static CResourceManager* Instance();

	//コンストラクタ
	CResourceManager();
	//デストラクタ
	~CResourceManager();

	//モデルデータ読み込み
	//(リソース管理用の名前、モデルデータのパス、マテリアルデータのパス)
	CModel* LoadModel(std::string name, std::string objPath, std::string mtlPath);
	//モデルデータを読み込んでいるかどうか
	bool IsLoadedModel(std::string name) const;
	//読み込み済みのモデルデータを取得
	CModel* GetModel(std::string name) const;

private:
	//CResourceManagerのインスタンス
	static CResourceManager* mpInstance;

	//読み込み済みのモデルデータのリスト
	std::map<std::string, CModel*> mModelList;
};

#define LOAD_MODEL(name, obj, mtl) CResourceManager::Instance()->LoadModel(name, obj, mtl)
#define GET_MODEL(name) CResourceManager::Instance()->GetModel(name)