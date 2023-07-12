#ifndef CIMAGE_H
#define CIMAGE_H
#include "CVector.h"
#include "CTexture.h"

//2Dイメージ描画用クラス
//読み込んだCTextureを2D表示する
class CImage
{
private:
	CTexture* mpTexture;	//表示するイメージのテクスチャ
	CVector2 mPosition;		//表示座標
	CVector2 mSize;			//表示サイズ
	CVector4 mUV;			//表示するテクスチャ上の範囲（UV）
	CVector4 mColor;		//表示カラー

public:
	//コンストラクタ
	CImage();
	//コンストラクタ（テクスチャ指定版）
	CImage(const char* path);
	//デストラクタ
	~CImage();

	//テクスチャの読み込み
	void Load(const char* path);

	//表示座標を設定（CVector2版）
	void SetPos(const CVector2& pos);
	//表示座標を設定
	void SetPos(const float& x, const float& y);
	//表示座標を取得
	const CVector2& GetPos() const;

	//表示サイズを設定（CVector2版）
	void SetSize(const CVector2& size);
	//表示サイズを設定
	void SetSize(const float& x, const float& y);
	//表示サイズを取得
	const CVector2& GetSize() const;

	//表示する範囲を設定（CVector4版）
	void SetUV(const CVector4& uv);
	//表示する範囲を設定
	void SetUV(const float& left, const float& top, const float& right, const float& bottom);
	//表示する範囲を取得
	const CVector4& GetUV() const;

	//表示カラーを設定（CVector4版）
	void SetColor(const CVector4& color);
	//表示カラーを設定（CVector版）
	void SetColor(const CVector& color);
	//表示カラーを設定（アルファ値有り版）
	void SetColor(const float& r, const float& g, const float& b, const float& a);
	//表示カラーを設定
	void SetColor(const float& r, const float& g, const float& b);
	//表示カラーを取得
	const CVector4& GetColor() const;

	//アルファ値を設定
	void SetAlpha(const float& alpha);
	//アルファ値を取得
	const float& GetAlpha() const;

	//描画
	void Render();
};

#endif