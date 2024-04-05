#pragma once
#include "CSceneBase.h"
class CPlayer;
class CSlime;
class CSlime2;
class CSlime3;
class CMushroom;
class CMushroom2;
class CMushroom3;
class CTurtle;
class CTurtle2;
class CTurtle3;
class CGameMenu;
class CSound;

// タイトルシーン
class CGameScene :public CSceneBase
{
public:
	// コンストラクタ
	CGameScene();
	// デストラクタ
	~CGameScene();
	// シーン読み込み
	void Load();

	// ゲームクリア判定
	bool IsClear();
	// ゲームクリア処理
	void Clear();

	// シーンの更新処理
	void Update();

private:
	// 復活時間
	int SlimeReTime;      // SlimeRevivalTime(レッドスライムの復活時間)
	int Slime2ReTime;     // Slime2RevivalTime(オレンジスライムの復活時間)
	int Slime3ReTime;     // Slime3RevivalTime(ブルースライムの復活時間)
	int MushroomReTime;   // MushroomRevivalTime(マッシュルームの復活時間)
	int Mushroom2ReTime;  // Mushroom2RevivalTime(マッシュルーム2の復活時間)
	int Mushroom3ReTime;  // Mushroom3RevivalTime(マッシュルーム3の復活時間)
	int TurtleReTime;     // TurtleRevivalTime(亀の復活時間)
	int Turtle2ReTime;     // Turtle2RevivalTime(亀2の復活時間)
	int Turtle3ReTime;     // Turtle3RevivalTime(亀3の復活時間)

	// プレイヤー
	CPlayer* mpPlayer;
	// スライム
	CSlime* mpSlime;
	CSlime2* mpSlime2;
	CSlime3* mpSlime3;
	// マッシュルーム
	CMushroom* mpMushroom;
	CMushroom2* mpMushroom2;
	CMushroom3* mpMushroom3;
	// 亀
	CTurtle* mpTurtle;
	CTurtle2* mpTurtle2;
	CTurtle3* mpTurtle3;

	CGameMenu* mpGameMenu;
};
