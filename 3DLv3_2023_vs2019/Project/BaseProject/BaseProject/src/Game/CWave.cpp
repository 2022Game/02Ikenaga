#include "CWave.h"
#include "CCharaBase.h"

// コンストラクタ
CWave::CWave(CObjectBase* owner, const CVector& pos, const CVector& dir,
	float speed, float dist)
	: mpOwner(owner)
	, mKillMoveDist(dist)
	, mMovedDist(0.0f)

{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
	Rotation(CQuaternion::LookRotation(mMoveSpeed, CVector::up));

	mpModel = CResourceManager::Get<CModel>("Wave");
	mpModel->SetupEffectSettings();

	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		9.0f,false
	);
	mpAttackCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpAttackCol->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
}

// デストラクタ
CWave::~CWave()
{
	SAFE_DELETE(mpAttackCol);
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
				int damage = CalcDamage(0.7f, mOwner, chara);

				// ダメージを与える
				chara->TakeDamage(damage, mOwner);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
}

// 攻撃開始
void CWave::AttackStart()
{
	CWeapon::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
}

// 攻撃終了
void CWave::AttackEnd()
{
	CWeapon::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
}

// 更新
void CWave::Update()
{
	CVector move = mMoveSpeed * Time::DeltaTime();
	float dist = move.Length();
	if (mMovedDist + dist >= mKillMoveDist)
	{
		dist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * dist;
	}
	Position(Position() + move);

	mMovedDist += dist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}

// 描画
void CWave::Render()
{
	mpModel->SetColor(mColor);
	mpModel->SetBlend(EBlend::eAdd);
	mpModel->Render(Matrix());
}