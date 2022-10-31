#pragma once
#include "CUi.h"
#include "CPlayer2.h"

//定数の定義
#define TEXTURE "ゲーム制作の素材02.png" //テクスチャのファイル
#define HITO "Image.png"
#define SURAIMU "Monster2.png"
#define KODOMO "117545.png"
#define GOAL "890568442.png"
#define UGOKUBLOCK "6fce1c1aa2540daee360283fa78defcd_t1.png"
#define AITEM "_7814-459000.png"

#define TIPSIZE 20.0f //マップ要素のサイズ

/*
* CGameクラス
* ゲームクラス
* ゲームのマップを作成し、
* キャラクタを配置する
*/
class CGame
{
public:
	//デストラクタ
	~CGame();
	//ゲームクリア判定
	bool IsClear();
	//ゲームクリア処理
	void Clear();
	//ゲームオーバー判定
	bool IsOver();
	//ゲームオーバー処理
	void Over();
	//スタート処理
	void Start();
	CGame(); //デフォルトコンストラクタ（初期処理）
	void Update(); //更新処理
private:
	int mCdx, mCdy; //カメラとプレイヤーの座標の差分
	void CameraSet(); //カメラ設定
	CPlayer2 *mpPlayer; //プレイヤーのポインタ
	int mTime; //経過時間
	CUi *mpUi; //UIクラスのポインタ
};
