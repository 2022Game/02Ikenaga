#ifndef CMODEL_H
#define CMODEL_H

//Vectorのインクルード
#include<vector>
#include"CTriangle.h"
#include"CMaterial.h"
#include"CVertex.h"

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel {
public:
	std::vector<CTriangle>Triangles() const;
	//描画
	//Render(行列)
	void Render(const CMatrix& m);
	~CModel();
	//モデルファイルの入力
	//Load(モデルファイル名,マテリアルファイル名
	void Load(char* obj,char* mtl);
	//描画
	void Render();
private:
	//頂点の配列
	CVertex* mpVertexes;
	void CreateVertexBuffer();
	//マテリアルポインタの可変長配列
	std::vector<CMaterial*> mpMaterials;
	std::vector<CTriangle> mTriangles;
};


#include "CVertex.h"
#include <vector>

class CModelTest
{
public:
	CModelTest();
	void Render();

	CVertex mVertex[6];
	std::vector<CVertex> mVector;

};
#endif 

