#include "CSaGauge.h"
#include "CImage.h"
#include "Maths.h"

//ゲージのフレーム画像のファイルパス
#define FRAME_IMAGE "Character\\Player\\HP\\HPframe.png"  //HPフレーム画像
//ゲージのバー画像のファイルパス
#define BAR_IMAGE "UI\\white.png"

//フレームの横のサイズ
#define FRAME_SIZE_X (230.0f)
//#define FRAME_SIZE_XX (430.0f)
//フレームの縦のサイズ
#define FRAME_SIZE_Y (25.0f)
//緑の幅
#define FRAME_BORDER (2.0f)
//バーの横のサイズ
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
//バーの縦のサイズ
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)


//コンストラクタ
CSaGauge::CSaGauge()
	:mMaxValue(100)
	, mValue(100)
{
	mpFrameImage = new CImage(FRAME_IMAGE);
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpBarImage = new CImage(BAR_IMAGE);
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);
}

//デストラクタ
CSaGauge::~CSaGauge()
{

}

//最大値を設定
void CSaGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

//現在値
void CSaGauge::SetValue(int value)
{
	mValue = value;
}

//更新
void CSaGauge::Update()
{
	//ゲージのフレームｔｐバーの位置を設定
	mpFrameImage->SetPos(mPosition);
	mpBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));

	//バーのサイズを最大値と現在値から求める
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X, BAR_SIZE_Y);
	size.X(BAR_SIZE_X * percent);
	mpBarImage->SetSize(size);

	//SAの割合でバーの色を変更
	CColor color;
	//青色
	if (percent <= 1.0f) color = CColor(0.0f, 0.4f, 1.0f);
	//バーに色を設定
	mpBarImage->SetColor(color);
}