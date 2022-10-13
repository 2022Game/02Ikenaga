#ifndef CTRIANGLE_H
#define CTRIANGLE_H

#include"CVector.h"
/*
三角形
*/
class CTriangle{
public:
	//Normal(法線ベクトル1,法線ベクトル2,法線ベクトル3)
	void Normal(const CVector& v0, const CVector& v1, const CVector& v2);
	void Vertex(const CVector &v0, const CVector &v1, const CVector &v2);
	//法線設定
	//Normal(法線ベクトル)
	void Normal(const CVector& n);
		//描画
		void Render();
private:
	//三角形の可変長配列
	CVector mV[3];//頂点座標
	CVector mN[3];   //法線
};
#endif