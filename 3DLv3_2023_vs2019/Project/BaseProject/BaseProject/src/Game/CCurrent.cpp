#include "CCurrent.h"
#include "CLineEffect.h"
#include "CCharaBase.h"
#include "CColliderLine.h"
#include "CInput.h"

// コンストラ
CCurrent::CCurrent(CObjectBase* owner, const CVector& pos, const CVector& dir)
	: mEffectAnimData(1, 11, true, 11, 0.03f)
	, mElapsedTime(0.0f)
{
	Position(pos);

	mpLineEffect = new CLineEffect(ETag::eCurrent);
	mpLineEffect->SetTexture("Current");
	mpLineEffect->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	mpLineEffect->SetColor(CColor::yellow);
	mpLineEffect->SetAnimData(&mEffectAnimData);

	CVector forward = dir;
	forward.Y(0.0f);
	forward.Normalize();
	CVector Pos = pos + forward * 40.0f;

	CVector startPos = pos;
	CVector endPos = Pos;
	int div = 3;
	float width = 5.0f;
	mpLineEffect->AddPoint(startPos, width, width);
	for (int i = 0; i < div; i++)
	{
		float alpha = (float)(i + 1) / div;
		CVector pos = CVector::Lerp(startPos, endPos, alpha);
		mpLineEffect->AddPoint(pos, width, width);
	}

	mpAttackCol = new CColliderLine
	(
		this,ELayer::eAttackCol,
		CVector::zero, forward * 40.0f,
		true
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
}

// デストラクタ
CCurrent::~CCurrent()
{
	if (mpLineEffect != nullptr)
	{
		mpLineEffect->Kill();
	}

	SAFE_DELETE(mpAttackCol);
}

// ヒットした時のコールバック関数を設定
void CCurrent::SetCollisionCallback(std::function<void()>callback)
{
	mColCallback = callback;
}

// 攻撃開始
void CCurrent::AttackStart()
{
	CWeapon::AttackStart();
	mpAttackCol->SetEnable(true);;
	SetEnableCol(true);
}

// 攻撃終了
void CCurrent::AttackEnd()
{
	CWeapon::AttackEnd();
	mpAttackCol->SetEnable(false);
	SetEnableCol(false);
}

// 衝突処理
void CCurrent::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
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
				int damage = 0;
				damage = 30;
				// ダメージを与える(30の固定ダメージ+防御力無視)
				chara->TakeDamage(damage, mOwner);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);

				// ヒット時に呼び出す、
				// コールバック関数が設定されていたら、実行する
				if (mColCallback != nullptr)
				{
					mColCallback();
				}
			}
		}
	}
}

// 更新
void CCurrent::Update()
{
	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= 0.8f)
	{
		Kill();
	}
}