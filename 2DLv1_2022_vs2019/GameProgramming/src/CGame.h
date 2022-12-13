#pragma once
#include "CUi.h"
#include "CPlayer2.h"
#include"CItem.h"

//定数の定義
#define TEXTURE "ゲーム制作の素材02.png" //テクスチャのファイル
#define HITO "Image.png"
#define SURAIMU "Monster2.png"
#define KODOMO "1175450110.png"
#define GOAL "890568442.png"
#define UGOKUBLOCK "6fce1c1aa2540daee360283fa78defcd_t1.png"
#define AITEM "_7814-459000.png"
#define TEKI "chara081234.png"
#define TEKI2 "DqWQjAXWsAI-YEO1.png"
#define UPUP "117545011.png"
#define HAZURE "4127255i17.png"

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
	//void Collision(CCharacter* m, CCharacter* o);
	//static int Time(); //経過時間
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
	//CUi* mpUi; //UIクラスのポインタ
	 int mTime;
private:
	int mCdx, mCdy; //カメラとプレイヤーの座標の差分
	void CameraSet(); //カメラ設定
	CPlayer2 *mpPlayer; //プレイヤーのポインタ
	CItem *mpItem;
	CUi *mpUi; //UIクラスのポインタ
	//int mTime;
	//CItem* mpItem;
	//CGoal *mpGoal;
};
