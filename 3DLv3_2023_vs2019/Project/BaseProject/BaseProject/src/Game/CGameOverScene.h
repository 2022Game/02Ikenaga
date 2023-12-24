#pragma once
#include "CSceneBase.h"
class CText;

// ゲームオーバー画面とメニューに戻る選択
class CGameOverScene :public CSceneBase
{
public:
	// コンストラクタ
	CGameOverScene();
	// デストラクタ
	~CGameOverScene();

	// シーンの読み込み
	void Load()override;
	// 更新
	void Update()override;

private:
	// 次の項目を選択
	void SelectNextMenu();
	// 前の項目を選択
	void SelectPrevMenu();

	CText* mpText;	// 文字列描画用

	// 選択情報
	struct ChoiceInfo
	{
		std::string name;	// 表示名
		EScene scene;		// 遷移先のシーン
	};
	// 選択情報のリスト
	std::vector<ChoiceInfo> mChoiceInfoList;

	int mSelectIndex;	// 選択中のインデックス値
};