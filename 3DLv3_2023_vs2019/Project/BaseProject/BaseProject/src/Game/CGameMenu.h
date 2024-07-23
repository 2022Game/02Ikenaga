#pragma once
#include "CTask.h"
#include "CImage.h"
#include "COperationIns.h"
#include "CPlayerStatusUI.h"

class CGameMenu : public CTask
{
public:
	// ゲームメニューのインスタンスを取得
	static CGameMenu* Instance();
	// ゲームメニューのインスタンスを削除
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
	CGameMenu();
	// デストラクタ
	~CGameMenu();

	static CGameMenu* spInstance;

	CImage* mpBackground;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	CImage* mpTextOperation;
	CImage* mpStatus;
	CImage* mpTextReturn;

	int mSelectIndex;
	bool mIsOpened;

	COperationIns* mpOperationIns;
	CPlayerStatusUI* mpPlayerStatusUI;
};