#include "CSword.h"
#include "CCollisionManager.h"
#include "CCharaBase.h"
#include "Maths.h"

CSword::CSword()
{
	mpSword = new CModel();
	mpSword->Load("Item\\Equipment\\Sword\\Sword.obj", "Item\\Equipment\\Sword\\Sword.mtl");

	//攻撃判定用のコライダーを作成
	mpAttackCol = new CColliderLine
	(
		this, ELayer::eAttackCol,
		CVector(0.15f, 0.15f, 0.0f),
		CVector(0.8f,0.8f, 0.0f)
	);
	ChangeLevel(1);

	// 攻撃判定用のコライダーと衝突判定を行う
	//レイヤーとタグを設定
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::eEnemy });

	// 最初は攻撃判定用のコライダーをオフにしておく
	mpAttackCol->SetEnable(false);
}

CSword::~CSword()
{
	SAFE_DELETE(mpSword);
}

void CSword::Update()
{
}

void CSword::Render()
{
	mpSword->Render(Matrix());
}

// 衝突処理
void CSword::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackCol)
	{
		// キャラのポインタに変換
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// 相手のコライダーの持ち主がキャラであれば、
		if (chara != nullptr)
		{
			//既に攻撃済みのキャラでなければ
			if (!IsAttackHitObj(chara))
			{
				// ダメージを与える
				chara->TakeDamage(mCharaMaxStatus.power,mOwner);

				//攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
}

//武器の行列取得
CMatrix CSword::Matrix() const
{
	//手に持っていないときは自分自身の行列を返す
	if (mpAttachMtx == nullptr)
	{
		return CTransform::Matrix();
	}
	//手に持っているときは、アタッチしている行列を返す
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		return sm * (*mpAttachMtx);
	}
}

// 攻撃開始
void CSword::AttackStart()
{
	CWeapon::AttackStart();
	//攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
}

//攻撃終了
void CSword:: AttackEnd()
{
	CWeapon::AttackEnd();
	//攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
}

//1レベルアップ
void CSword::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

//レベルを変更
void CSword::ChangeLevel(int level)
{
	//ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, PLAYER_LEVEL_MAX);
	//最大ステータスに設定
	mCharaMaxStatus = PLAYER_STATUS[index];
}