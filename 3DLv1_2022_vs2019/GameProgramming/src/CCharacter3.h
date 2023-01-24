#pragma once
#ifndef CCHARACTER3_H
#define CCHARACTER3_H
//変数行列クラスのインクルード
#include "CTransform.h"
//モデルクラスのインクルード
#include"CModel.h"
#include"CTask.h"
//#include"CCollider.h"

/*
キャラクタークラス
ゲームキャラクターの基本的な機能を定義する
*/

//コライダクラスの宣言
class CCollider;
class CCharacter3 :public CTransform,public CTask{
public:
	enum ETag
	{
		EZERO,    //初期値
		EPLAYER,  //プレイヤー
		EENEMY,   //敵
		EBULLETPLAYER,  //プレイヤー弾
		EBULLETENEMY,  //敵弾
	};
	//タグの取得
	ETag Tag();
	//コンストラクタ
	CCharacter3(int priority);
	//衝突処理
	virtual void Collision(CCollider *m, CCollider*o){}
	//コンストラクタ
	CCharacter3();
	//デストラクタ
	~CCharacter3();
	//モデルの設定
	//Model(モデルクラスのポインタ)
	void Model(CModel* m);
	//描画処理
	void Render();
protected:
	ETag mTag;  //タグ
	CModel* mpModel;//モデルのポインタ
};

#endif
