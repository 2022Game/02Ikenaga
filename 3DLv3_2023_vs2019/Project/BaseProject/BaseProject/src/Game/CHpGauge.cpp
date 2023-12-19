#include "CHpGauge.h"
#include "CImage.h"
#include "Maths.h"
#include "CCamera.h"

//ゲージのフレーム画像のファイルパス
#define FRAME_IMAGE "Character\\Player\\HP\\Frame.png"  //HPフレーム画像
//ゲージのバー画像のファイルパス
#define BAR_IMAGE "UI\\white.png"

//フレームの横のサイズ
#define FRAME_SIZE_X (250.0f)
//#define FRAME_SIZE_XX (430.0f)
//フレームの縦のサイズ
#define FRAME_SIZE_Y (30.0f)
//緑の幅
#define FRAME_BORDER (2.0f)
//バーの横のサイズ
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
//バーの縦のサイズ
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)

// スケール値計算時のカメラとの距離の最小
#define SCALE_DIST_MIN 0.0f
// スケール値計算時のカメラとの距離の最大
#define SCALE_DIST_MAX 200.0f
// スケール値の最小値
#define SCALE_MIN 0.2f
// スケール値の最大値
#define SCALE_MAX 1.0f

//コンストラクタ
CHpGauge::CHpGauge()
	:mMaxValue(100)
	,mValue(100)
	,mCenterRatio(0.0f,0.0f)
	,mScale(1.0f)
	,mIsShow(true)
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

// HPゲージを削除
void CHpGauge::Kill()
{
	CTask::Kill();
	mpFrameImage->Kill();
	mpBarImage->Kill();
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

// 中心位置のの割合の設定
void CHpGauge::SetCenterRatio(const CVector2& ratio)
{
	mCenterRatio = ratio;
}

// ワールド座標を設定
void CHpGauge::SetWorldPos(const CVector& worldPos)
{
	// 現在のカメラを取得
	CCamera* cam = CCamera::CurrentCamera();
	if (cam == nullptr)return;

	// 指定されたワールド座標をスクリーン座標に変換
	CVector screenPos = cam->WorldToScreenPos(worldPos);
	
	// 設定ワールド座標がカメラの背後であれば、
	// HPゲージを表示しない
	if (screenPos.Z() < 0.0f)
	{
		mIsShow = false;
		return;
	}
	
	// HPゲージを表示
	mIsShow = true;
	// もとめたスクリーン座標を自身の位置に設定
	mPosition = screenPos;

	// 設定されたワールド座標とカメラの座標を求める
	float dist = (worldPos - cam->Position()).Length();

	// カメラから離れるごとにスケール値を小さくする
	float ratio = Math::Clamp01(dist - SCALE_DIST_MIN) / (SCALE_DIST_MAX - SCALE_DIST_MIN);
	mScale = Math::Lerp(SCALE_MIN, SCALE_MAX, ratio);
}

//更新
void CHpGauge::Update()
{
	//ゲージのフレームバーの位置を設定
	mpFrameImage->SetPos(mPosition);
	CVector2 barPos = mPosition;
	barPos.X(barPos.X() - FRAME_SIZE_X * mCenterRatio.X() * mScale);
	mpBarImage->SetPos(barPos + CVector2(FRAME_BORDER, FRAME_BORDER) * mScale);

	// フレームサイズを変更
	mpFrameImage->SetSize(CVector2(FRAME_SIZE_X, FRAME_SIZE_Y) * mScale);

	//バーのサイズを最大値と現在値から求める
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X * percent, BAR_SIZE_Y) * mScale;
	mpBarImage->SetSize(size);

    // フレームとバーの中心位置を設定
	mpFrameImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X()*mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);
	mpBarImage->SetCenter
	(
		0.0f,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

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