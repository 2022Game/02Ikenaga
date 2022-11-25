#pragma once
#ifndef CBULLET_H
#define CBULLET_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"

/*
弾クラス
三角形クラスのインクルード
*/

class CBullet : public CCharacter3
{
public:
	//幅と奥行きの設定
	//Set(幅,奥行)
	void Set(float w, float d);
	//更新
	void Update();
	void Render();
private:
	//三角形
	CTriangle mT;
};

#endif