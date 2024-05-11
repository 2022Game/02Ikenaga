#include "CWave.h"
#include "CCharaBase.h"
#include "Easing.h"
#include "CRay.h"
#include "Maths.h"

// 波動のスケール値の最大値
#define FLAME_SCALE 40.0f
// 波動のスケール値が最大値になるまでの時間
#define FLAME_SCALE_ANIM_TIME 3.0f

// 波動の間隔時間
#define THROW_INTERVAL 0.2f
// 波動の方向のブレ幅
#define FLAME_DIR_RAND 0.02f
// 波動の移動速度
#define FLAME_MOVE_SPEED 25.0f
// 波動の色
#define FLAME_COLOR CColor(0.0f, 0.5f, 1.0f)


// コンストラクタ
CWave::CWave(ETag tag)
	: CBillBoardImage("Effect/ring2.png", ETag::eWave, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
	mpCollider = new CColliderSphere
	(
		this,
		ELayer::eAttackCol,
		0.2
	);
	mpCollider->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
}

// デストラクタ
CWave::~CWave()
{
	SAFE_DELETE(mpCollider);
}

// 各パラメータを設定
void CWave::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// 削除フラグが立っているかどうか
bool CWave::IsDeath() const
{
	return mIsDeath;
}

// カラーを設定
void CWave::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CWave::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 衝突処理
void CWave::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
				int damage = CalcDamage(mOwner, chara);

				// ダメージを与える
				chara->TakeDamage(damage, mOwner);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
}

// 更新
void CWave::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	// 波動のエフェクトを移動
	CVector move = mMoveSpeed * Time::DeltaTime();
	Position(Position() + move);

	// スケール変更時間を経過していない
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// 経過時間に合わせて、徐々に波動を大きくする
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

	if (mElapsedTime >= 2)
	{
		Kill();
	}
}