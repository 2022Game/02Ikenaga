#include "CMushroom.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"

// マッシュルームのインスタンス
CMushroom* CMushroom::spInstance = nullptr;

#define MODEL_MUSHROOM "Character\\Enemy\\Mushroom\\Mushroom.x"

#define ENEMY_HEIGHT 2.0f
#define WITHIN_RANGE 40.0f    // 範囲内

// マッシュルームのアニメーションデータのテーブル
const CMushroom::AnimData CMushroom::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlant.x",	true,	21.0f	},	        // 植物 21.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlantToBattle.x",	true,	70.0f	},	// 植物からきのこ 21.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle2.x",	true,	41.0f	},	// 待機2 18.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle.x",	true,	41.0f	},	    // 待機 18.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack.x",	true,	80.0f	},	// 攻撃 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack2.x",	true,	70.0f	},	// 攻撃 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack3.x",	true,	70.0f	},	// 攻撃 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomDie.x",	true,	70.0f	},	// 攻撃 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomDizzy.x",	true,	70.0f	},	// めまい 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomGetHit.x",	true,	70.0f	},	// ヒット 23.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomRun.x",	true,	40.0f	},	//走る 17.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomSenseSomethingMaintain.x",	true,	121.0f	},	//見回す 121.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomSenseSomethingStart.x",	true,	25.0f	},	//開始の見回す 25.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomTaunting.x",	true,	80.0f	},	//挑発 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomVictory.x",	true,	61.0f	},	//勝利 61.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk.x",	true,	31.0f	},	//歩く 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk2.x",	true,	31.0f	},	//歩く2 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk3.x",	true,	31.0f	},	//歩く3 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk4.x",	true,	31.0f	},	//歩く4 31.0f
};

// コンストラクタ
CMushroom::CMushroom()
	: mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Mushroom");

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// キャラクター押し戻し処理
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.3f, false, 5.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy2 });
	mpColliderSphere->Position(0.0f, 0.2f, 0.0f);

	// ダメージを受けるコライダーを作成
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.4f, false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	//ダメージを受けるコライダーを少し上へずらす
	mpDamageCol->Position(0.0f, 0.3f, 0.0f);

	mpDamageCol->SetEnable(false);

	// ダメージを与えるコライダー
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.6f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.0f, -0.1f, 0.0f);

	// 攻撃コライダーをマッシュルームの頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_mushroom_spine03");
	mpAttackCol->SetAttachMtx(headMty);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackCol->SetEnable(false);
}

CMushroom::~CMushroom()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);
}

CMushroom* CMushroom::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CMushroom::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 戦う前の待機状態
void CMushroom::UpdateIdle()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// 戦う前の待機状態2
void CMushroom::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// 待機状態3
void CMushroom::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle3);
}

// 攻撃
void CMushroom::UpdateAttack()
{
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃終了待ち
void  CMushroom::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle3;
	}
}

// 更新処理
void CMushroom::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 戦う前の待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
		// 戦う前の待機状態2
	case EState::eIdle2:
		UpdateIdle2();
		break;
		// 待機状態3
	case EState::eIdle3:
		UpdateIdle3();
		break;
		// 攻撃
	case EState::eAttack:
		UpdateAttack();
		break;
		// 攻撃終了待ち
	case EState::eAttackWait:
		UpdateAttackWait();
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle3)
	{
		UpdateIdle();
	}

	// キャラクターの更新
	CXCharacter::Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	if (CInput::PushKey('Q'))
	{
		mState = EState::eAttack;
	}
}

// 衝突処理
void CMushroom::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackCol && mState != EState::eIdle && mState != EState::eIdle2 &&
		mState != EState::eIdle3)
	{
		// キャラのポインタに変換
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// 相手のコライダーの持ち主がキャラであれば、
		if (chara != nullptr)
		{
			// 既に攻撃済みのキャラでなければ
			if (!IsAttackHitObj(chara))
			{
				int damage = CalcDamage(this, chara);

				// ダメージを与える
				chara->TakeDamage(damage, this);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
	else if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;
			//mMoveSpeed.Y(0.0f);

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// キャラクター同士の衝突処理
	else if (self == mpColliderSphere)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CMushroom::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
}

// 攻撃終了
void CMushroom::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
}

// 描画
void CMushroom::Render()
{
	CXCharacter::Render();
}


// 死亡処理
void CMushroom::Death()
{
	// マッシュルームの死亡処理
	CEnemy::Death();
	// 死亡状態へ移行
	//mState = EState::eDie;
}