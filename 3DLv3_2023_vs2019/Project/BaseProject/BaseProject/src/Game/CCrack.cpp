#include "CCrack.h"
#include "CCharaBase.h"
#include "Easing.h"

// ひび割れのスケール値の最大値
#define FLAME_SCALE 10.0f
// ひび割れのスケール値が最大値になるまでの時間
#define FLAME_SCALE_ANIM_TIME 3.0f

// コンストラクタ
CCrack::CCrack(ETag tag)
	: CBillBoardImage("Effect/Crack.png", ETag::eNone, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
}

// デストラクタ
CCrack::~CCrack()
{
}

// 各パラメータを設定
void CCrack::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// 削除フラグが立っているかどうか
bool CCrack::IsDeath() const
{
	return mIsDeath;
}

// カラーを設定
void CCrack::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CCrack::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 更新
void CCrack::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	// ひび割れのエフェクトを移動
	//CVector move = mMoveSpeed * Time::DeltaTime();
	//Position(Position() + move);

	// スケール変更時間を経過していない
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// 経過時間に合わせて、徐々にひび割れを大きくする
		float per = mElapsedTime / FLAME_SCALE_ANIM_TIME;
		if (per < 1.0f)
		{
			float scale = Easing::QuadOut(per, 1.0f, 1.0f, 1.0f);
			Scale(CVector::one * scale * FLAME_SCALE);
		}
		else
		{
			Scale(CVector::one * FLAME_SCALE);
		}

		mElapsedTime += Time::DeltaTime();
	}
	// 移動時間が経過したら、削除する
	else
	{
		Scale(CVector::one * FLAME_SCALE);
	}

	if (mElapsedTime >= 1)
	{
		Kill();
	}
}