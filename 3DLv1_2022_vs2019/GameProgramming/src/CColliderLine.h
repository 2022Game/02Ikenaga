#pragma once
#ifndef CCOLLIDERLINE_H
#define CCOLLIDERLINE_H
#include"CCollider.h"

/*
線分コライダ
*/

class CColliderLine :public CCollider
{
public:
	CColliderLine(){}
	//コンストラクタ(線分コライダ)
	//CColliderLine(親,親行列,頂点1,頂点2)
	CColliderLine(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1);
	//線分コライダの設定
	//Set(親,親行列,頂点1,頂点2)
	void Set(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1);
	//描画
	void Render();
};

#endif
