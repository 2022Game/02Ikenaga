#include "CHomingBall.h"
#include "CCharaBase.h"
#include "Easing.h"
#include "Maths.h"
#include "CPlayer.h"

// ホーミングボールのスケール値の最大値
#define SCALE 30.0f
// ホーミングボールのスケール値が最大値になるまでの時間
#define SCALE_ANIM_TIME 3.0f
// ホーミングボールの発射の移動速度
#define MOVE_SPEED 0.8 *60.0f
// プレイヤーまでの距離
#define DISTANCE 20.0f

// コンストラクタ
CHomingBall::CHomingBall(ETag tag)
	: CBillBoardImage("Effect/Ball.png", ETag::eHomingBall, ETaskPauseType::eGame)
	, mMoveDir(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
	, mMovedDist(0.0f)
	, mIsHoming(true)
{
	mpAttackCollider = new CColliderSphere
	(
		this,
		ELayer::eAttackCol,
		0.14
	);
	mpAttackCollider->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpAttackCollider->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
}

// デストラクタ
CHomingBall::~CHomingBall()
{
	SAFE_DELETE(mpAttackCollider);
}

// 各パラメータを設定
void CHomingBall::Setup(const CVector& pos, const CVector& dir, float dist)
{
	Position(pos);
	mKillMoveDist = dist;
}

// 削除フラグが立っているかどうか
bool CHomingBall::IsDeath() const
{
	return mIsDeath;
}

// カラーを設定
void CHomingBall::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CHomingBall::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 衝突処理
void CHomingBall::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eField)
	{
		Kill();
	}

	if (other->Layer() == ELayer::eDamageCol)
	{
		// キャラのポインタに変換
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// 相手のコライダーの持ち主がキャラであれば、
		if (chara != nullptr)
		{
			// 既に攻撃済みのキャラでなければ
			if (!IsAttackHitObj(chara))
			{
				// 与えるダメージを計算
				int damage = CalcDamage(0.7f, mOwner, chara);

				// ダメージを与える
				chara->TakeDamage(damage, mOwner);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
				Kill();
			}
		}
	}
}

// 更新
void CHomingBall::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	// スケール変更時間を経過していない
	if (mElapsedTime < SCALE_ANIM_TIME)
	{
		// 経過時間に合わせて、徐々にホーミングボールを大きくする
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

	if (mIsHoming)
	{
		CPlayer* player = CPlayer::Instance();
		bool IsRolling = player->mRolling;

		// 自身からプレイヤーまでのベクトル
		CVector targetPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
		CVector vec = targetPos - Position();

		float playerDist = vec.Length();
		if (IsRolling && playerDist <= DISTANCE)
		{
			// ホーミング終了
			mIsHoming = false;
		}
		else
		{
			mMoveDir = CVector::Slerp(mMoveDir, vec.Normalized(), 0.05f);
			//mMoveDir = vec.Normalized();
		}
	}

	// 移動ベクトルを求める
	CVector move = mMoveDir * MOVE_SPEED * Time::DeltaTime();
	
	float moveDist = move.Length();
	if (mMovedDist + moveDist >= mKillMoveDist)
	{
		moveDist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * moveDist;
	}

	Position(Position() + move);
	
	if (mIsHoming == false)
	{
		mMovedDist += moveDist;
		if (mMovedDist >= mKillMoveDist)
		{
			Kill();
		}
	}
}