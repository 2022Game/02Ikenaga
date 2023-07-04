#pragma once
#include "CVector.h"
#include <vector>//vectorクラスのインクルード(動的配列)
#include "CMatrix.h"  //マトリクスクラスのインクルード
class CAnimationKey;  //アニメーションキークラス
class CAnimation;  //アニメーションクラス
class CAnimationSet;  //アニメーションセットクラス
class CSkinWeights;  //スキンウェイトクラス
class CMaterial;  //マテリアルの宣言
class CMesh;  //CMeshクラスの宣言
class CModelXFrame;  //CModelXFrameクラスの宣言
class CModelX;  //CModelXクラスの宣言
#ifndef CMODELX_H //インクルードガード
#define CMODELX_H

//#define MODEL_FILE "res\\sample.blend.x"  //入力ファイル名
#define MODEL_FILE "res\\ラグナ.x"  //入力ファイル名

//領域解放をマクロ化
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a; a=nullptr;}
// 配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a)(sizeof(a)/sizeof(a[0]))

/*
CAnimationKey
アニメーションキークラス
*/
class CAnimationKey
{
	friend CAnimation;
	friend CAnimationSet;
	friend CModelX;
	//friend CModelX;
public:
	CAnimationKey();
private:
	//時間
	float mTime;
	//行列
	CMatrix mMatrix;
};

/*
CAnimation
アニメーションクラス
*/
class CAnimation
{
	friend CModelX;
	friend CAnimationSet;
public:
	CAnimation();
	CAnimation(CModelX* model);
	~CAnimation();
private:
	int mKeyNum;  //キー数(時間数)
	CAnimationKey* mpKey;  //キーの配列
	char* mpFrameName;  //フレーム名
	int mFrameIndex;  //フレーム番号
};

/*
CAnimationSet
アニメーションセット
*/
class CAnimationSet
{
	friend CModelX;
public:
	CAnimationSet();
	float Time();
	float MaxTime();
	std::vector<CAnimation*>& Animation();
	void AnimateMatrix(CModelX* model);
	void Time(float time);  //時間の設定
	void Weight(float weight);  //重みの設定
	CAnimationSet(CModelX* model);
	~CAnimationSet();
private:
	float mTime;  //現在時間
	float mWeight;  //重み
	float mMaxTime;  //最大時間
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
	void AnimateVertex(CMatrix*);
	//頂点にアニメーション適用
	void AnimateVertex(CModelX* model);
	//スキンウェイトにフレーム番号を設定する
	void SetSkinWeightFrameIndex(CModelX* model);
	void Render();
	//コンストラクタ
	CMesh();
	//デストラクタ
	~CMesh();
	//読み込み処理
	void Init(CModelX* model);
private:
	//テクスチャ座標データ
	float* mpTextureCoords;
	CVector* mpAnimateVertex;  //アニメーション用頂点
	CVector* mpAnimateNormal;  //アニメーション用法線
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
	friend CAnimationSet;
	friend CAnimation;
	friend CModelX;
public:
	CModelXFrame();
	const CMatrix& CombinedMatrix();
	//合成行列の作成
	void AnimateCombined(CMatrix* parent);
	int Index();
	//std::vector<CAnimation*>& Index();
	void Render();
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame();
private:
	CMatrix mCombinedMatrix;  //合成行列
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
	//friend CAnimationKey;
	friend CAnimation;
	friend CAnimationSet;
	friend CModelXFrame;
public:
	/*
アニメーションを抜き出す
idx:分割したいアニメーションの番号
start:分割したいアニメーションの開始時間
end:分割したいアニメーションの終了時間
name:追加するアニメーションの名前
*/
	void CModelX::SeparateAnimationSet(int idx,int start,int end,char*name);
	void AnimateVertex(CMatrix*);
	//マテリアル配列の取得
	std::vector<CMaterial*>& Material();
	//マテリアルの検索
	CMaterial* FindMaterial(char* name);
	//頂点にアニメーションを適用
	void AnimateVertex();
	//スキンウェイトのフレーム番号設定
	void SetSkinWeightFrameIndex();
	std::vector<CModelXFrame*>& Frames();
	//std::vector<CAnimationSet*>& CModelX::Animation();
	void AnimateFrame();
	std::vector<CAnimationSet*>& CModelX::AnimationSet();
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
	std::vector<CMaterial*> mMaterial;  //マテリアル配列
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
