#include "CHpGauge.h"
#include "CImage.h"
#include "Maths.h"
#include "CCamera.h"
#include "CPlayer.h"

// フレームの横のサイズ
#define FRAME_SIZE_X (450.0f)
// フレームの縦のサイズ
#define FRAME_SIZE_Y (40.0f)
// ゲージの幅
#define FRAME_BORDER (2.0f)
// バーの横のサイズ
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
// バーの縦のサイズ
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)

// スケール値計算時のカメラとの距離の最小
#define SCALE_DIST_MIN 0.0f
// スケール値計算時のカメラとの距離の最大
#define SCALE_DIST_MAX 200.0f
// スケール値の最小値
#define SCALE_MIN 0.7f
// スケール値の最大値
#define SCALE_MAX 1.5f

// コンストラクタ
CHpGauge::CHpGauge(bool is3dGauge)
	:mMaxValue(100)
	, mValue(100)
	, mCenterRatio(0.0f, 0.0f)
	, mScale(1.0f)
	, mIs3dGauge(is3dGauge)
	, mDamageBarTime(0)
{
	mpFrameImage = new CImage("HpFrame");
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpDamagaBarImage = new CImage("HpGauge");
	mpDamagaBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpBarImage = new CImage("HpGauge");
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpEdgeImage = new CImage("FrameEdge");
	mpEdgeImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	// 最初は非表示
	SetShow(false);
}

// デストラクタ
CHpGauge::~CHpGauge()
{
}

// HPゲージを削除
void CHpGauge::Kill()
{
	CTask::Kill();
	mpFrameImage->Kill();
	mpDamagaBarImage->Kill();
	mpBarImage->Kill();
	mpEdgeImage->Kill();
}

// 表示するかどうか設定
void CHpGauge::SetShow(bool isShow)
{
	// ベースクラスの表示設定処理
	CTask::SetShow(isShow);
	mpFrameImage->SetShow(isShow);
	mpDamagaBarImage->SetShow(isShow);
	mpBarImage->SetShow(isShow);
	mpEdgeImage->SetShow(isShow);
}

// 最大値を設定
void CHpGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// 現在値
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
		SetShow(false);
		return;
	}
	
	// もとめたスクリーン座標を自身の位置に設定
	mPosition = screenPos;

	// 設定されたワールド座標とカメラの座標を求める
	float dist = (worldPos - cam->Position()).Length();

	if (dist <= SCALE_DIST_MAX)
	{
		// カメラから離れるごとにスケール値を小さくする
		float ratio = 0.3f - Math::Clamp01((dist - SCALE_DIST_MIN) / (SCALE_DIST_MAX - SCALE_DIST_MIN));
		mScale = Math::Lerp(SCALE_MIN, SCALE_MAX, ratio);
	
		// HPゲージが表示
		SetShow(true);
	}
	// カメラの距離が遠い場合は、非表示
	else
	{
		SetShow(false);
	}
}

// 更新
void CHpGauge::Update()
{
	// ゲージのフレームバーの位置を設定
	mpFrameImage->SetPos(mPosition);
	CVector2 barPos = mPosition;
	barPos.X(barPos.X() - FRAME_SIZE_X * mCenterRatio.X() * mScale);
	mpDamagaBarImage->SetPos(barPos + CVector2(FRAME_BORDER, FRAME_BORDER) * mScale);
	mpBarImage->SetPos(barPos + CVector2(FRAME_BORDER, FRAME_BORDER) * mScale);
	mpEdgeImage->SetPos(mPosition);

	// フレームサイズを変更
	mpFrameImage->SetSize(CVector2(FRAME_SIZE_X, FRAME_SIZE_Y) * mScale);
	mpEdgeImage->SetSize(CVector2(FRAME_SIZE_X, FRAME_SIZE_Y) * mScale);

	// バーのサイズを最大値と現在値から求める
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X * percent, BAR_SIZE_Y) * mScale;

	// プレイヤー
	if (mValue < mMaxValue)
	{
		mDamageBarTime++;
	}

	if (mDamageBarTime > 100)
	{
		mpDamagaBarImage->SetSize(size);
		mDamageBarTime = 0;
	}

	// 敵
	if (mIs3dGauge)
	{
		mpDamagaBarImage->SetSize(size);
	}
	mpBarImage->SetSize(size);

	CDebugPrint::Print("時間 %d\n", mDamageBarTime);

    // フレームとバーの中心位置を設定
	mpFrameImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X()* mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	mpDamagaBarImage->SetCenter
	(
		0.0f,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	mpBarImage->SetCenter
	(
		0.0f,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	mpEdgeImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);
	
	mpDamagaBarImage->SetColor(CColor::red);

	// HPの割合でバーの色を変更
	CColor color;
	// 10%以下は赤色
	if (percent <= 0.1f) color = CColor(1.0f, 0.0f, 0.0f);
	// 30%以下は黄色
	else if (percent <= 0.3f)color = CColor(1.0f, 1.0f, 0.0f);
	// それ以外は緑色
	else color = CColor(0.0f, 1.0f,0.0f);
	// バーに色を設定
	mpBarImage->SetColor(color);

	// 3D空間に配置するゲージは、残りのHPが0なら非表示にする
	if (mIs3dGauge && mValue <= 0)
	{
		SetShow(false);
	}
}