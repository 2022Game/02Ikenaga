#ifndef CMODEL_H
#define CMODEL_H
//vectorのインクルード
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"
#include "CVertex.h"
#include "CResource.h"
#include "CColor.h"

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel : public CResource
{
	friend CResourceManager;

public:
	std::vector<CTriangle> Triangles() const;

	// カラーを設定
	void SetColor(const CColor& color);
	// カラーを取得
	const CColor& GetColor() const;
	// アルファ値設定
	void SetAlpha(float alpha);
	// アルファ値取得
	float GetAlpha() const;

	//描画
	void Render();
	//描画
	//Render(行列)
	void Render(const CMatrix& m);
private:
	CModel();
	~CModel();
	//モデル読み込み
	bool Load(std::string path, bool dontDelete) override;
	//マテリアル読み込み
	bool LoadMaterial(std::string path, bool dontDelete);
	void CreateVertexBuffer();

	//マテリアルポインタの可変長配列
	std::vector<CMaterial*> mpMaterials;
	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;
	//頂点の配列
	CVertex* mpVertexes;
	//モデルのカラー
	CColor mColor;
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
