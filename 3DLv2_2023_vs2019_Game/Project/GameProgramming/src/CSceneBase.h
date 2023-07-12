#pragma once
#include "CSceneType.h"
class CTask;

//シーンのベースクラス
class CSceneBase
{
public:
	//コンストラクタ
	CSceneBase(EScene scene);
	//デストラクタ
	virtual ~CSceneBase();

	virtual void Load() = 0;

	virtual void Update() = 0;

	EScene GetSceneType() const;

	void AddTask(CTask* task);

private:
	EScene mSceneType;
};