#include "CLevelUI.h"
#include "CText.h"
#include "CCamera.h"
#include "Maths.h"

// スケール値計算時のカメラとの距離の最小
#define SCALE_DIST_MIN 0.0f
// スケール値計算時のカメラとの距離の最大
#define SCALE_DIST_MAX 300.0f
// スケール値の最小値
#define SCALE_MIN 0.7f
// スケール値の最大値
#define SCALE_MAX 1.5f

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CLevelUI::CLevelUI(bool is3dGauge)
	: mBasePos(15.0f + SHADOW_WIDTH, 32.0f)
	, mCenterRatio(0.0f, 0.0f)
	, mIs3dGauge(is3dGauge)
{
	CVector2 size = CVector2(WINDOW_WIDTH*0.5, WINDOW_HEIGHT*0.5f);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// レベルのテキストを作成
	mpLevelText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpLevelText->SetTextAlignH(textAlignH);
}

// デストラクタ
CLevelUI::~CLevelUI()
{
	SAFE_DELETE(mpLevelText);
}

// Levelを設定
void CLevelUI::SetLevel(std::string level)
{
	mpLevelText->SetText(level.c_str());
}

// 中心位置のの割合の設定
void CLevelUI::SetCenterRatio(const CVector2& ratio)
{
	mCenterRatio = ratio;
}

// ワールド座標を設定
void CLevelUI::SetWorldPos(const CVector& worldPos)
{
	// 現在のカメラを取得
	CCamera* cam = CCamera::CurrentCamera();
	if (cam == nullptr)return;

	// 指定されたワールド座標をスクリーン座標に変換
	CVector screenPos = cam->WorldToScreenPos(worldPos);

	// 設定ワールド座標がカメラの背後であれば、
	// 表示しない
	if (screenPos.Z() < 0.0f)
	{
		SetShow(false);
		return;
	}

	// もとめたスクリーン座標を自身の位置に設定
	mPosition = (CVector2)screenPos + CVector2(-120.0f,-32.0f);

	// 設定されたワールド座標とカメラの座標を求める
	float dist = (worldPos - cam->Position()).Length();

	if (dist <= SCALE_DIST_MAX)
	{
		// カメラから離れるごとにスケール値を小さくする
		float ratio = 0.3f - Math::Clamp01((dist - SCALE_DIST_MIN) / (SCALE_DIST_MAX - SCALE_DIST_MIN));
		mScale = Math::Lerp(SCALE_MIN, SCALE_MAX, ratio);
		// 表示
		SetShow(true);
	}
	// カメラの距離が遠い場合は、非表示
	else
	{
		SetShow(false);
	}
}

// 更新
void CLevelUI::Update()
{
}

// 描画
void CLevelUI::Render()
{
	if (mIs3dGauge)
	{
		mpLevelText->SetColor(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < SHADOW_COUNT; i++)
		{
			CVector2 v = CVector2(1.0f, 0.0f);
			float alpha = (float)i / SHADOW_COUNT;
			CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);
			mpLevelText->SetPos(mPosition + (CVector2)(rot * v) * SHADOW_WIDTH);
			mpLevelText->Render();
		}
		mpLevelText->SetColor(1.0f, 1.0f, 1.0f);
		mpLevelText->SetPos(mPosition);
		mpLevelText->Render();
	}
	else
	{
		mpLevelText->SetColor(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < SHADOW_COUNT; i++)
		{
			CVector2 v = CVector2(1.0f, 0.0f);
			float alpha = (float)i / SHADOW_COUNT;
			CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);
			mpLevelText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
			mpLevelText->Render();
		}
		mpLevelText->SetColor(1.0f, 1.0f, 1.0f);
		mpLevelText->SetPos(mBasePos);
		mpLevelText->Render();
	}
}