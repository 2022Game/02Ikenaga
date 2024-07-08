#include "CAvoidanceGauge.h"
#include "CImage.h"
#include "Maths.h"
#include "CCamera.h"
#include "CTransform.h"

// フレームの横のサイズ
#define FRAME_SIZE_X (20.0f)
// フレームの縦のサイズ
#define FRAME_SIZE_Y (150.0f)
// 白色の幅
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
CAvoidanceGauge::CAvoidanceGauge(bool is3dGauge)
	: mMaxValue(100)
	, mValue(100)
	, mCenterRatio(0.0f, 0.0f)
	, mScale(1.0f)
	, mIs3dGauge(is3dGauge)
{
	mpFrameImage = new CImage("Frame");
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpBarImage = new CImage("Gauge");
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpEdgeImage = new CImage("FrameEdge");
	mpEdgeImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	// 最初は非表示
	SetShow(false);
}

// デストラクタ
CAvoidanceGauge::~CAvoidanceGauge()
{
}

// 回避ゲージを削除
void CAvoidanceGauge::Kill()
{
	CTask::Kill();
	mpFrameImage->Kill();
	mpBarImage->Kill();
	mpEdgeImage->Kill();
}

// 表示するかどうか設定
void CAvoidanceGauge::SetShow(bool isShow)
{
	// ベースクラスの表示設定処理
	CTask::SetShow(isShow);
	mpFrameImage->SetShow(isShow);
	mpBarImage->SetShow(isShow);
	mpEdgeImage->SetShow(isShow);
}

// 最大値を設定
void CAvoidanceGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// 現在値
void CAvoidanceGauge::SetValue(int value)
{
	mValue = value;
}

// 中心位置のの割合の設定
void CAvoidanceGauge::SetCenterRatio(const CVector2& ratio)
{
	mCenterRatio = ratio;
}

// ワールド座標を設定
void CAvoidanceGauge::SetWorldPos(const CVector& worldPos)
{
	// 現在のカメラを取得
	CCamera* cam = CCamera::CurrentCamera();
	if (cam == nullptr)return;

	// 指定されたワールド座標をスクリーン座標に変換
	CVector screenPos = cam->WorldToScreenPos(worldPos);

	// 設定ワールド座標がカメラの背後であれば、
	// ゲージを表示しない
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

		// ゲージが表示
		SetShow(true);
	}
	// カメラの距離が遠い場合は、非表示
	else
	{
		SetShow(false);
	}
}

// 更新
void CAvoidanceGauge::Update()
{
	// ゲージのフレームバーの位置を設定
	mpFrameImage->SetPos(mPosition);
	CVector2 barPos = mPosition;
	barPos.Y(barPos.Y() - FRAME_SIZE_Y * mCenterRatio.Y() * mScale);
	mpBarImage->SetPos(barPos + CVector2(FRAME_BORDER, FRAME_BORDER) * mScale);
	mpEdgeImage->SetPos(mPosition);

	// フレームサイズを変更
	mpFrameImage->SetSize(CVector2(FRAME_SIZE_X, -FRAME_SIZE_Y) * mScale);
	mpEdgeImage->SetSize(CVector2(FRAME_SIZE_X, -FRAME_SIZE_Y) * mScale);

	// バーのサイズを最大値と現在値から求める
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X , -BAR_SIZE_Y * percent) * mScale;
	mpBarImage->SetSize(size);

	// フレームの中心位置を設定
	mpFrameImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	// バーの中心位置を設定
	mpBarImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		1.5f
	);

	// ふちの中心位置を設定
	mpEdgeImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	CColor color;
	// 白色
	if (percent <= 1.0f) color = CColor::white;
	// バーに色を設定
	mpBarImage->SetColor(color);
}