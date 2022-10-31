#ifndef CMODEL_H
#define CMODEL_H

//Vectorのインクルード
#include<vector>
#include"CTriangle.h"
#include"CMaterial.h"

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel {
public:
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
	//マテリアルポインタの可変長配列
	std::vector<CMaterial*> mpMaterials;
	std::vector<CTriangle> mTriangles;
};

#endif 

