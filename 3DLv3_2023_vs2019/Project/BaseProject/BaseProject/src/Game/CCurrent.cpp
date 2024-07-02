#include "CCurrent.h"
#include "CLineEffect.h"
#include "CCharaBase.h"

// コンストラ
CCurrent::CCurrent(CObjectBase* owner, const CVector& pos, const CVector& dir)
	: mEffectAnimData(1, 11, true, 11, 0.03f)
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
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, 10.0f),
		CVector(0.0f, 0.0f, 0.0f)
	);
	mpAttackCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpAttackCol->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
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
				// 与えるダメージを計算
				int damage = CalcDamage(1.5f, mOwner, chara);

				// ダメージを与える
				chara->TakeDamage(damage, mOwner);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
}

// 更新
void CCurrent::Update()
{
}