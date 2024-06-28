#include "CStarAura.h"
#include "CCharaBase.h"
#include "Easing.h"
#include "CHealCircle.h"
#include "Maths.h"

// 星のスケール値の最大値
#define SCALE 3.0f
// 星のスケール値が最大値になるまでの時間
#define SCALE_ANIM_TIME 0.2f

// コンストラクタ
CStarAura::CStarAura(const CVector& pos, const CVector& dir, float speed, float dist)
	: CBillBoardImage("Effect/star.png", ETag::eAura, ETaskPauseType::eGame)
	, mMoveDir(CVector::zero)
	, mElapsedTime(0.0f)
	, mMovedDist(0.0f)
	, mKillMoveDist(dist)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// デストラクタ
CStarAura::~CStarAura()
{
}

// カラーを設定
void CStarAura::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CStarAura::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 更新
void CStarAura::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	// スケール変更時間を経過していない
	if (mElapsedTime < SCALE_ANIM_TIME)
	{
		// 経過時間に合わせて、徐々に星を大きくする
		float per = mElapsedTime / SCALE_ANIM_TIME;
		if (per < 1.0f)
		{
			float scale = Easing::QuadOut(per, 1.0f, 1.0f, 1.0f);
			Scale(CVector::one * scale * SCALE);
		}
		else
		{
			Scale(CVector::one * SCALE);
		}

		mElapsedTime += Time::DeltaTime();
	}
	// 移動時間が経過したら、削除する
	else
	{
		Scale(CVector::one * SCALE);
	}

	// 移動ベクトルを求める
	CVector move = mMoveSpeed * Time::DeltaTime();

	float moveDist = move.Length();
	if (mMovedDist + moveDist >= mKillMoveDist)
	{
		moveDist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * moveDist;
	}
	move.Y(0.1f);
	Position(Position() + move);

	mMovedDist += moveDist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}