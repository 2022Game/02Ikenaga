#pragma once
#include "CVector.h"
#include "CTexture.h"
#include "CUIBase.h"

//2Dイメージ描画用クラス
//読み込んだCTextureを2D表示する
class CImage : public CUIBase
{
private:
	CTexture* mpTexture;	//表示するイメージのテクスチャ
	CVector4 mUV;			//表示するテクスチャ上の範囲（UV）

public:
	//コンストラクタ
	CImage();
	//コンストラクタ（テクスチャ指定版）
	CImage(const char* path);
	//デストラクタ
	~CImage();

	//テクスチャの読み込み
	void Load(const char* path);

	//表示する範囲を設定（CVector4版）
	void SetUV(const CVector4& uv);
	//表示する範囲を設定
	void SetUV(const float& left, const float& top, const float& right, const float& bottom);
	//表示する範囲を取得
	const CVector4& GetUV() const;


	//描画
	void Render();
};
