#include "CImpact.h"
#include "CCharaBase.h"
#include "Easing.h"

// 衝撃のスケール値の最大値
#define FLAME_SCALE 8.0f
// 衝撃のスケール値が最大値になるまでの時間
#define FLAME_SCALE_ANIM_TIME 3.0f

// アニメーションの1コマ表示時間
#define ANIM_TIME 0.0625f
// 衝撃のエフェクトのアニメーションデータ
TexAnimData CImpact::msAnimData = TexAnimData(3, 5, false, 64, ANIM_TIME);

// コンストラクタ
CImpact::CImpact(ETag tag)
	: CBillBoardImage("Effect/Impact1.png", ETag::eImpact, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
	SetAnimData(&msAnimData);

	mpCollider = new CColliderSphere
	(
		this,
		ELayer::eAttackCol,
		2.0
	);
	mpCollider->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
}

// デストラクタ
CImpact::~CImpact()
{
	SAFE_DELETE(mpCollider);
}

// 各パラメータを設定
void CImpact::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// 削除フラグが立っているかどうか
bool CImpact::IsDeath() const
{
	return mIsDeath;
}

// カラーを設定
void CImpact::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CImpact::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 衝突処理
void CImpact::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eField)
	{
		float length = mMoveSpeed.Length();
		CVector n = hit.adjust.Normalized();
		float d = CVector::Dot(n, mMoveSpeed);
		mMoveSpeed = (mMoveSpeed - n * d).Normalized() * length;
		Position(Position() + hit.adjust * hit.weight);
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
				int damage = CalcDamage(0.6f, mOwner, chara);

				// ダメージを与える
				chara->TakeDamage(damage, mOwner);

				// 攻撃済みリストに追加
			    AddAttackHitObj(chara);
			}
		}
	}
}

// 更新
void CImpact::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	// 衝撃のエフェクトを移動
	CVector move = mMoveSpeed * Time::DeltaTime();
	Position(Position() + move);

	// スケール変更時間を経過していない
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// 経過時間に合わせて、徐々に衝撃を大きくする
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
	// アニメーションが終わったら、削除フラグを立てる
	if (IsEndAnim())
	{
		mIsDeath = true;
	}
	if (mElapsedTime >= 0.5)
	{
		Kill();
	}
}