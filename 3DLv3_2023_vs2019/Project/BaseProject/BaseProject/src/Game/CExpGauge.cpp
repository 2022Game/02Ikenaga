#include "CExpGauge.h"
#include "CImage.h"
#include "Maths.h"

// フレームの横のサイズ
#define FRAME_SIZE_X (600.0f)
// フレームの縦のサイズ
#define FRAME_SIZE_Y (20.0f)
// 緑色の幅
#define FRAME_BORDER (2.0f)
// バーの横のサイズ
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
// バーの縦のサイズ
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)


// コンストラクタ
CExpGauge::CExpGauge()
	: mMaxValue(100)
	, mValue(100)
{
	mpBarImage = new CImage("Gauge");
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpEdgeImage = new CImage("FrameEdge");
	mpEdgeImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
}

// デストラクタ
CExpGauge::~CExpGauge()
{
	CTask::Kill();
	mpBarImage->Kill();
	mpEdgeImage->Kill();
}

// 最大値を設定
void CExpGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// 現在値
void CExpGauge::SetValue(int value)
{
	mValue = value;
}

// 表示するかどうか設定
void CExpGauge::SetShow(bool isShow)
{
	// ベースクラスの表示設定処理
	CTask::SetShow(isShow);
	mpBarImage->SetShow(isShow);
	mpEdgeImage->SetShow(isShow);
}

// 更新
void CExpGauge::Update()
{
	// ゲージのバーと縁の位置を設定
	mpBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));
	mpEdgeImage->SetPos(mPosition);

	// バーのサイズを最大値と現在値から求める
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X, BAR_SIZE_Y);
	size.X(BAR_SIZE_X * percent);
	mpBarImage->SetSize(size);

	// SAの割合でバーの色を変更
	CColor color;
	// ピンク色
	if (percent <= 1.0f) color = CColor(0.0f, 0.5f, 0.0f, 0.8f);
	// バーに色を設定
	mpBarImage->SetColor(color);
}