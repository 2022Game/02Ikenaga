#ifndef CMODEL_H
#define CMODEL_H

//Vectorのインクルード
#include<vector>
#include"CTriangle.h"

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel {
public:
	//モデルファイルの入力
	//Load(モデルファイル名,マテリアルファイル名
	void Load(char* obj,char* mtl);
	//描画
	void Render();
private:
	std::vector<CTriangle> mTriangles;
};

#endif 

