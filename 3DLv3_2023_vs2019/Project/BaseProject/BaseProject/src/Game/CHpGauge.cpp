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
#define SCALE_DIST_MAX 300.0f
// スケール値の最小値
#define SCALE_MIN 0.7f
// スケール値の最大値
#define SCALE_MAX 1.5f

// ダメージバーが減少を始めるまでの待ち時間
#define DAMAGE_WAIT_TIME 1.0f
// ダメージポイントが1減少するのにかかる時間
#define DAMAGE_SUB_TIME 0.001f

// コンストラクタ
CHpGauge::CHpGauge(bool is3dGauge)
	: mMaxValue(100)
	, mValue(100)
	, mDamageValue(100)
	, mCenterRatio(0.0f, 0.0f)
	, mIs3dGauge(is3dGauge)
	, mScale(1.0f)
	, mDamageWaitTime(0.0f)
	, mDamageElapsedTime(0.0f)
{
	mpFrameImage = new CImage("Frame");
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpDamagaBarImage = new CImage("Gauge");
	mpDamagaBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpHpBarImage = new CImage("Gauge");
	mpHpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpEdgeImage = new CImage("FrameEdge");
	mpEdgeImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	if (!mIs3dGauge)
	{
		mScale = 1.0f;
	}

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
	mpHpBarImage->Kill();
	mpEdgeImage->Kill();
}

// 表示するかどうか設定
void CHpGauge::SetShow(bool isShow)
{
	// ベースクラスの表示設定処理
	CTask::SetShow(isShow);
	mpFrameImage->SetShow(isShow);
	mpDamagaBarImage->SetShow(isShow);
	mpHpBarImage->SetShow(isShow);
	mpEdgeImage->SetShow(isShow);
}

// 大きさを設定
void CHpGauge::SetMaxScale(float maxScale)
{
	mMaxScale = maxScale;
}

// 最小の大きさを設定
void CHpGauge::SetMinScale(float minScale)
{
	mMinScale = minScale;
}

// 最大値を設定
void CHpGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// 現在値
void CHpGauge::SetValue(int value)
{
	// ダメージを受けた場合は、ダメージ関連のタイマーをリセット
	if (value < mValue)
	{
		mDamageWaitTime = DAMAGE_WAIT_TIME;
		mDamageElapsedTime = 0.0f;
	}
	else
	{
		if (value > mDamageValue)
		{
			mDamageValue = value;
		}
	}

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
		mScale = Math::Lerp(mMinScale, mMaxScale, ratio);
	
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
	// 現在HPがダメージバー表示用のHPより小さい場合は
	if (mValue < mDamageValue)
	{
		// ダメージポイント減少までの待ち時間
		if (mDamageWaitTime > 0.0f)
		{
			mDamageWaitTime -= Time::DeltaTime();
		}
		// ダメージポイント減少中
		else
		{
			// DAMAGE_SUB_TIMEの時間を経過する度に、ダメージポイント1減少させる
			if (mDamageElapsedTime >= DAMAGE_SUB_TIME)
			{
				mDamageValue -= 5;
				if (mDamageValue < mValue)
				{
					mDamageValue = mValue;
				}
				mDamageElapsedTime -= DAMAGE_SUB_TIME;
			}
			mDamageElapsedTime += Time::DeltaTime();
		}
	}

	// ダメージバーのサイズをダメージポイントとHPの最大値で判断する
	float damageBer = Math::Clamp01((float)mDamageValue / mMaxValue);
	CVector2 damageSize = CVector2(BAR_SIZE_X * damageBer, BAR_SIZE_Y) * mScale;	
	mpDamagaBarImage->SetSize(damageSize);

	// ゲージのフレームバーの位置を設定
	mpFrameImage->SetPos(mPosition);
	CVector2 barPos = mPosition;
	barPos.X(barPos.X() - FRAME_SIZE_X * mCenterRatio.X() * mScale);
	mpDamagaBarImage->SetPos(barPos + CVector2(FRAME_BORDER, FRAME_BORDER) * mScale);
	mpHpBarImage->SetPos(barPos + CVector2(FRAME_BORDER, FRAME_BORDER) * mScale);
	mpEdgeImage->SetPos(mPosition);

	// フレームサイズを変更
	mpFrameImage->SetSize(CVector2(FRAME_SIZE_X, FRAME_SIZE_Y) * mScale);
	mpEdgeImage->SetSize(CVector2(FRAME_SIZE_X, FRAME_SIZE_Y) * mScale);

	// HPバーのサイズを最大値と現在値から求める
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X * percent, BAR_SIZE_Y) * mScale;
	mpHpBarImage->SetSize(size);

    // フレームの中心位置を設定
	mpFrameImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X()* mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	// ダメージバーの中心位置を設定
	mpDamagaBarImage->SetCenter
	(
		0.0f,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	// Hpバーの中心位置を設定
	mpHpBarImage->SetCenter
	(
		0.0f,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	// ふちの中心位置を設定
	mpEdgeImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);
	
	// ダメージバーを赤色に設定
	mpDamagaBarImage->SetColor(CColor::red);

	// HPの割合でバーの色を変更
	CColor color;
	// 10%以下は橙色
	if (percent <= 0.1f) color = CColor(1.0f, 0.6f, 0.0f);
	// 30%以下は黄色
	else if (percent <= 0.3f)color = CColor(1.0f, 1.0f, 0.0f);
	// それ以外は緑色
	else color = CColor(0.0f, 1.0f,0.0f);
	// バーに色を設定
	mpHpBarImage->SetColor(color);

	// 3D空間に配置するゲージは、残りのHPが0なら非表示にする
	if (mIs3dGauge && mValue <= 0)
	{
		SetShow(false);
	}
}