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
	CModel* mpModel;//モデルのポインタ
};

#endif
