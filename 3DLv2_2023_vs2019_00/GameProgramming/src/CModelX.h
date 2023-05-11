#pragma once
#include "CVector.h"
#include <vector>//vectorクラスのインクルード(動的配列)
#include "CMatrix.h"  //マトリクスクラスのインクルード
class CAnimation;  //アニメーションクラス
class CAnimationSet;  //アニメーションセットクラス
class CSkinWeights;  //スキンウェイトクラス
class CMaterial;  //マテリアルの宣言
class CMesh;  //CMeshクラスの宣言
class CModelX;  //CModelXクラスの宣言
class CModelXFrame;  //CModelXFrameクラスの宣言
#ifndef CMODELX_H //インクルードガード
#define CMODELX_H

#define MODEL_FILE "res\\sample.blend.x"  //入力ファイル名

//領域解放をマクロ化
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a; a=nullptr;}

/*
CAnimation
アニメーションクラス
*/
class CAnimation
{
	friend CAnimationSet;
public:
	CAnimation(CModelX* model);
	~CAnimation();
private:
	char* mpFrameName;  //フレーム名
	int mFrameIndex;  //フレーム番号
};

/*
CAnimationSet
アニメーションセット
*/
class CAnimationSet
{
public:
	CAnimationSet(CModelX* model);
	~CAnimationSet();
private:
	//アニメーション
	std::vector<CAnimation*> mAnimation;
	//アニメーションセット名
	char* mpName;
};

/*
CSkinWeights
スキンウェイトクラス
*/
class CSkinWeights
{
	friend CModelX;
	friend CMesh;
public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();
	const int& FrameIndex();
	const CMatrix& Offset();
private:
	char* mpFrameName;  //フレーム名
	int mFrameIndex;  //フレーム番号
	int mIndexNum;  //頂点番号数
	int* mpIndex;  //頂点番号配列
	float* mpWeight;  //頂点ウェイト配列
	CMatrix mOffset;  //オフセットマトリックス
};

class CMesh {
public:
	void Render();
	//コンストラクタ
	CMesh();
	//デストラクタ
	~CMesh();
	//読み込み処理
	void Init(CModelX* model);
private:
	//スキンウェイト
	std::vector<CSkinWeights*>mSkinWeights;
	int mMaterialNum;  //マテリアル数
	int mMaterialIndexNum;  //マテリアル番号数(面数)
	int* mpMaterialIndex;  //マテリアル番号
	std::vector<CMaterial*>mMaterial;  //マテリアルデータ
	int mNormalNum;  //法線数
	CVector* mpNormal;  //法線ベクトル
	int mFaceNum;  //面数
	int* mpVertexIndex;  //面を構成する頂点インデックス
	int mVertexNum;  //頂点数
	CVector* mpVertex;  //頂点データ
};

//CModelXFrameクラスの定義
class CModelXFrame {
	friend CAnimation;
	friend CModelX;
public:
	bool Index();
	void Render();
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame();
private:
	CMesh* mpMesh;  //Meshデータ
	std::vector<CModelXFrame*> mChild;   //子フレームの配列
	CMatrix mTransformMatrix;  //変数行列
	char* mpName;  //フレーム名前
	int mIndex;  //フレーム番号
};

/*
 CModelX
 Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX {
	friend CAnimation;
	friend CAnimationSet;
	friend CModelXFrame;
public:
	//フレーム名に該当するフレームのアドレスを返す
	CModelXFrame* FindFrame(char* name);
	bool EOT();  //トークンがなくなったらtrue
	void Render();
	//単語の取り出し
	char* GetToken();
	char* Token();
	~CModelX();
	//ノードの読み飛ばし
	void SkipNode();
	CModelX();
	//ファイル読み込み
	void Load(char* file);
private:
	//アニメーションセットの配列
	std::vector<CAnimationSet*> mAnimationSet;
	std::vector<CModelXFrame*> mFrame;  //フレームの配列
	//単語の取り出し
	//char* GetToken();
	//cが区切り文字ならtrueを返す
	bool IsDelimiter(char c);
	char* mpPointer;     //読み込み位置
	char mToken[1024];  //取り出した単語の領域
};

#endif
