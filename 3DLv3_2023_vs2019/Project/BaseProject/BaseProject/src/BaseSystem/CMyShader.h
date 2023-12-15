#ifndef CMYSHADER_H
#define CMYSHADER_H

#include "CShader.h"

class CModelX;
class CMaterial;
class CMesh;
class CMatrix;
class CModel;

class CMyShader : public CShader {
private:
	//マテリアルの設定
	void SetShader(CModelX* model, CMaterial* material);
	//描画処理
	void Render(CModelX* model, CMesh* mesh, CMatrix* pCombinedMatrix);
public:
	//描画処理
	void Render(CModelX* model, CMatrix* combinedMatrix);
	//	void Render(CModel *mesh);
};

#endif