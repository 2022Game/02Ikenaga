#include "CDizzy.h"
#include "CCharaBase.h"
#include "Easing.h"

// アニメーションの1コマ表示時間
#define ANIM_TIME 0.0825f

// エフェクトのアニメーションデータ
TexAnimData CDizzy::msAnimData = TexAnimData(1, 5, true, 64, ANIM_TIME);

// コンストラクタ
CDizzy::CDizzy()
	: CBillBoardImage("Effect/Dizzy.png", ETag::eDizzy,ETaskPauseType::eGame)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
	SetAnimData(&msAnimData);
}

// デストラクタ
CDizzy::~CDizzy()
{
}

// 各パラメータを設定
void CDizzy::Setup(const CVector& pos)
{
	Position(pos);
}

// 削除フラグが立っているかどうか
bool CDizzy::IsDeath() const
{
	return mIsDeath;
}

// カラーを設定
void CDizzy::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CDizzy::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 更新
void CDizzy::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	mElapsedTime += Time::DeltaTime();

	if (mElapsedTime >= 1.3)
	{
		Kill();
	}
}