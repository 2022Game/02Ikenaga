#include "CHpGauge.h"
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
CHpGauge::CHpGauge()
	:mMaxValue(100)
	,mValue(100)
{
	mpFrameImage = new CImage(FRAME_IMAGE);
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpBarImage = new CImage(BAR_IMAGE);
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);
}

//デストラクタ
CHpGauge::~CHpGauge()
{

}

//最大値を設定
void CHpGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

//現在値
void CHpGauge::SetValue(int value)
{
	mValue = value;
}

//更新
void CHpGauge::Update()
{
	//ゲージのフレームｔｐバーの位置を設定
	mpFrameImage->SetPos(mPosition);
	mpBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));

	//バーのサイズを最大値と現在値から求める
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X, BAR_SIZE_Y);
	size.X(BAR_SIZE_X * percent);
	mpBarImage->SetSize(size);

	//HPの割合でバーの色を変更
	CColor color;
	//10%以下は赤色
	if (percent <= 0.1f) color = CColor(1.0f, 0.0f, 0.0f);
	//30%以下は黄色
	else if (percent <= 0.3f)color = CColor(1.0f, 1.0f, 0.0f);
	//それ以外は緑色
	else color = CColor(0.0f, 1.0f,0.0f);
	//バーに色を設定
	mpBarImage->SetColor(color);
}