#pragma once
#include "CCharacter.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
class CEnemy2 : public CCharacter
{
public:
	/*CItem* mItem3;
	 CItem2* mItem2;
	CItem* mItem;*/
	//char ch;
	//char item[3][]{
	//"CItem"; //アイテム名
	//"CItem2"; //アイテム名
	//"CItem3"; //アイテム名
	//};
	//int i;
	static int mItem3;
	static int mItem2;
	static int mItem;
	//敵の数を設定
	static void Num(int num);
	//敵の数を取得
	static int Num();
	//衝突処理2
	void Collision();
	//衝突処理4
	void Collision(CCharacter* m, CCharacter* o);
	//CEnemy2(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CEnemy2(float x, float y, float w, float h, CTexture* pt);
	//更新処理
	void Update();
private:
	static int sNum;	//敵の数
};