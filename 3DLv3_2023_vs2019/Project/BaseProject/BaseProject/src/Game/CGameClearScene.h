#pragma once
#include "CTask.h"
#include "CImage.h"

class CTexk;

class CGameClearScene : public CTask
{
public:
	// インスタンスを取得
	static CGameClearScene* Instance();
	// インスタンスを削除
	static void ClearInstance();

	// オープン
	void Open();
	// クローズ
	void Close();
	// オープンするかどうか
	bool IsOpened() const;

	// 決める
	void Decide(int select);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// コンストラク
	CGameClearScene();
	// デストラクタ
	~CGameClearScene();

	static CGameClearScene* spInstance;

	CImage* mpBackground;
	CImage* mpBackground2;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	CImage* mpUpUI;
	CImage* mpDownUI;

	CImage* mpEnterKey;

	int mSelectIndex;
	bool mIsOpened;      // オープンするかどうか
	CText* mpGameClear;  // ゲームクリア
	CText* mpTitle;      // タイトル
};