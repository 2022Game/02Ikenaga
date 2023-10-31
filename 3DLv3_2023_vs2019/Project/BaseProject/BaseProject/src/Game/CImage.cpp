#include "CImage.h"

//コンストラクタ
CImage::CImage()
	: mpTexture(nullptr)
	, mUV(0.0f, 0.0f, 1.0f, 1.0f)
{
	mPosition = CVector2(0.0f, 0.0f);
	mSize = CVector2(128.0f, 128.0f);
	mColor = CColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//コンストラクタ（テクスチャ指定版）
CImage::CImage(const char* path)
	: CImage()
{
	Load(path);
}

//デストラクタ
CImage::~CImage()
{
	//テクスチャを読み込んでいたら破棄
	if (mpTexture != nullptr)
	{
		delete mpTexture;
	}
}

//テクスチャの読み込み
void CImage::Load(const char* path)
{
	mpTexture = new CTexture();
	bool success = mpTexture->Load(path);
	//読み込み成功
	if (success)
	{
		//表示サイズをテクスチャのサイズで初期化
		const STgamHeader& header = mpTexture->Header();
		SetSize(header.width, header.height);
	}
	//読み込み失敗
	else
	{
		//テクスチャを破棄
		delete mpTexture;
		mpTexture = nullptr;
	}
}

//表示する範囲を設定（CVector4版）
void CImage::SetUV(const CVector4& uv)
{
	mUV = uv;
}

//表示する範囲を設定
void CImage::SetUV(const float& left, const float& top, const float& right, const float& bottom)
{
	SetUV(CVector4(left, top, right, bottom));
}

//表示する範囲を取得
const CVector4& CImage::GetUV() const
{
	return mUV;
}

//描画
void CImage::Render()
{
	//テクスチャが読み込まれてなければ、描画処理は行わない
	if (mpTexture == nullptr) return;

	glPushMatrix();

	glLoadIdentity();

	//テクスチャを有効にする
	glEnable(GL_TEXTURE_2D);
	//アルファブレンドを有効にする
	glEnable(GL_BLEND);
	//ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//テクスチャを指定
	glBindTexture(GL_TEXTURE_2D, mpTexture->Id());

	glBegin(GL_TRIANGLES);
	// 表示カラーを設定
	glColor4fv((GLfloat*)&mColor);

	float uvLeft = mUV.X();
	float uvTop = mUV.Y();
	float uvRight = mUV.Z();
	float uvBottom = mUV.W();

	float vtxLeft = mPosition.X();
	float vtxRight = vtxLeft + mSize.X();
	//OpenGLは左下原点なので、左上が原点になるよう調整
	float vtxBottom = WINDOW_HEIGHT - mPosition.Y();
	float vtxTop = vtxBottom - mSize.Y();

	glTexCoord2f(uvLeft, uvTop);
	glVertex2d(vtxLeft, vtxTop);

	glTexCoord2f(uvLeft, uvBottom);
	glVertex2d(vtxLeft, vtxBottom);

	glTexCoord2f(uvRight, uvBottom);
	glVertex2d(vtxRight, vtxBottom);

	glTexCoord2f(uvLeft, uvTop);
	glVertex2d(vtxLeft, vtxTop);

	glTexCoord2f(uvRight, uvBottom);
	glVertex2d(vtxRight, vtxBottom);

	glTexCoord2f(uvRight, uvTop);
	glVertex2d(vtxRight, vtxTop);

	glEnd();

	//テクスチャを解放
	glBindTexture(GL_TEXTURE_2D, 0);
	//アルファブレンドを無効
	glDisable(GL_BLEND);
	//テクスチャを無効
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
