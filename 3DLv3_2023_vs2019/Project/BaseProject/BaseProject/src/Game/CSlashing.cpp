#include "CSlashing.h"
#include "CCharaBase.h"
#include "Easing.h"

// 斬撃のスケール値の最大値
#define FLAME_SCALE 15.0f
// 斬撃のスケール値が最大値になるまでの時間
#define FLAME_SCALE_ANIM_TIME 3.0f

// コンストラクタ
CSlashing::CSlashing(ETag tag)
	: CBillBoardImage("Effect/Slash.png", ETag::eFlame, ETaskPauseType::eGame)
	,LifeTime(0)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{

	mpColliderLine = new CColliderLine
	(
		this,ELayer::eAttackCol,
		CVector(1.0f,0.0f,0.0f),
		CVector(0.0f, -1.0f, 0.0f),
		false,1.0f
	);
	mpColliderLine->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::eEnemy });
	mpColliderLine->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
	//mpCollider->Position(-10.0f, -15.0f, 0.0f);
	//mpCollider->Rotation(-20.0f, 0.0f, 0.0f);
}

// デストラクタ
CSlashing::~CSlashing()
{
	SAFE_DELETE(mpColliderLine);
}

// 各パラメータを設定
void CSlashing::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// 削除フラグが立っているかどうか
bool CSlashing::IsDeath() const
{
	return mIsDeath;
}

// カラーを設定
void CSlashing::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CSlashing::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 衝突処理
void CSlashing::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
			// 与えるダメージを計算
			int damage = CalcDamage(0, chara);

			// ダメージを与える
			chara->TakeDamage(damage, 0);
		}
	}
}

// 更新
void CSlashing::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	// 斬撃のエフェクトを移動
	CVector move = mMoveSpeed * Time::DeltaTime();
	Position(Position() + move);

	// スケール変更時間を経過していない
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// 経過時間に合わせて、徐々にスラッシュを大きくする
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

	if (mElapsedTime >= 0.7)
	{
		Kill();
	}

	CDebugPrint::Print(" 経過時間 %f", mElapsedTime);
}