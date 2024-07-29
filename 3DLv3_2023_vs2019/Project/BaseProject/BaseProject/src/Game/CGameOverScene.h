#pragma once
#include "CTask.h"
#include "CImage.h"

class CTexk;

class CGameOverScene : public CTask
{
public:
	// インスタンスを取得
	static CGameOverScene* Instance();
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
	CGameOverScene();
	// デストラクタ
	~CGameOverScene();

	static CGameOverScene* spInstance;

	CImage* mpBackground;
	CImage* mpBackground2;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	CImage* mpUpUI;
	CImage* mpDownUI;

	CImage* mpEnterKey;

	int mSelectIndex;
	bool mIsOpened;
	CText* mpGameOver;  // ゲームオーバー
	CText* mpRevival;
	CText* mpEnd;
};