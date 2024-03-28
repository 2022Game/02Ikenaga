#include "CBeholder.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "Maths.h"

// 球体のモンスターのインスタンス
CBeholder* CBeholder::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 40.0f    // 範囲内

// 球体のモンスターのアニメーションデータのテーブル
const CBeholder::AnimData CBeholder::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle2.x",	true,	46.0f	},	    // 待機 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle.x",	true,	82.0f	},	    // 待機2 41.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack.x",	true,	46.0f	},	    // 攻撃 23.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack2.x",	true,	42.0f	},	    // 攻撃2 21.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack3.x",	true,	42.0f	},	    // 攻撃3 17.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack4.x",	true,	46.0f	},	    // 攻撃4 23.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderDie.x",	true,	76.0f	},	        // 死ぬ 23.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderDizzy.x",	true,	82.0f	},	    // めまい 41.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderGetHit.x",	true,	46.0f	},	    // ヒット 23.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderRun.x",	true,	34.0f	},	    // 走る 17.0f
};

// コンストラクタ
CBeholder::CBeholder()
	: mpRideObject(nullptr)
	, mAttackTime(0)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Beholder");

	//最初に1レベルに設定
	ChangeLevel(1);

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

	//mpColliderLine = new CColliderLine
	//(
	//	this, ELayer::eField,
	//	CVector(0.0f, 0.0f, 0.0f),
	//	CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	//);
	//mpColliderLine->SetCollisionLayers({ ELayer::eField });

	//// キャラクター押し戻し処理
	//mpColliderSphere = new CColliderSphere
	//(
	//	this, ELayer::eEnemy,
	//	0.3f, false, 5.0f
	//);
	//mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy2 });
	//mpColliderSphere->Position(0.0f, 0.2f, 0.0f);

	//// ダメージを受けるコライダーを作成
	//mpDamageCol = new CColliderSphere
	//(
	//	this, ELayer::eDamageCol,
	//	0.4f, false
	//);
	////　ダメージを受けるコライダーと
	////　衝突判定を行うコライダーのレイヤーとタグを設定
	//mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	//mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	////ダメージを受けるコライダーを少し上へずらす
	//mpDamageCol->Position(0.0f, 0.3f, 0.0f);

	//// ダメージを与えるコライダー
	//mpAttackCol = new CColliderSphere
	//(
	//	this, ELayer::eAttackCol,
	//	0.55f, false
	//);
	//mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	//mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	//mpAttackCol->Position(0.0f, -0.1f, 0.0f);

	//// 攻撃コライダーをマッシュルームの頭の行列にアタッチ
	//const CMatrix* headMty = GetFrameMtx("Armature_mushroom_spine03");
	//mpAttackCol->SetAttachMtx(headMty);

	//// 最初の攻撃コライダーを無効にしておく
	//mpAttackCol->SetEnable(false);
}

CBeholder::~CBeholder()
{
	/*SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);*/
}

CBeholder* CBeholder::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CBeholder::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 待機状態
void CBeholder::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// 待機状態2
void CBeholder::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// 攻撃
void CBeholder::UpdateAttack()
{
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃2
void CBeholder::UpdateAttack2()
{
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃終了待ち
void CBeholder::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle2;
	}
}

// ヒット
void CBeholder::UpdateHit()
{
	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// めまいをfalseにする
		bool stan = false;
		// 確率を最小に0最大40
		int probability = Math::Rand(0, 20);
		if (probability == 1)stan = true;
		if (stan)
		{
			//mState = EState::eDizzy;
		}
		else
		{
			// プレイヤーの攻撃がヒットした時の待機状態へ移行
			mState = EState::eIdle2;
			ChangeAnimation(EAnimType::eIdle2);
		}
	}
}

// 死ぬ
void CBeholder::UpdateDie()
{
	//ChangeAnimation(EAnimType::eDie);
}

// めまい(混乱)
void CBeholder::UpdateDizzy()
{
	//ChangeAnimation(EAnimType::eDizzy);
}

// 更新処理
void CBeholder::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
		// 待機状態2
	case EState::eIdle2:
		UpdateIdle2();
		break;
		// 攻撃
	case EState::eAttack:
		UpdateAttack();
		break;
		// 攻撃2
	case EState::eAttack2:
		UpdateAttack2();
		break;
		// 攻撃終了待ち
	case EState::eAttackWait:
		UpdateAttackWait();
		break;
		// ヒット
	case EState::eHit:
		UpdateHit();
		break;
		// 死ぬ
	case EState::eDie:
		UpdateDie();
		break;
		// めまい(混乱)
	case EState::eDizzy:
		UpdateDizzy();
		break;
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2
		&& mState != EState::eAttack && mState != EState::eAttackWait)
	{
		mState = EState::eIdle2;
	}
	if (mState == EState::eIdle2)
	{
		mAttackTime++;

		if (mAttackTime > 200)
		{
			// 攻撃2
			//bool Attack2 = false;
			//// 攻撃3
			//bool Attack3 = false;
			//// 確率を最小に3最大6
			//int probability2 = Math::Rand(2, 5);
			//int probability3 = Math::Rand(6, 10);
			//if (probability2 == 2)Attack2 = true;
			//if (probability3 == 6)Attack3 = true;
			//if (Attack2)
			//{
			//	mState = EState::eAttack2;
			//}
			//else if (Attack3)
			//{
			//	mState = EState::eAttack3;
			//}
			//else
			//{
				mState = EState::eAttack;
			//}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2)
		{
			mAttackTime = 0;
		}
	}

	//CDebugPrint::Print(" 攻撃時間: %d\n", mAttackTime);
	//CDebugPrint::Print(" HP: %d\n", mCharaStatus.hp);

	// キャラクターの更新
	CXCharacter::Update();

	//mpAttackCol->Update();

	mIsGrounded = false;
}

// 衝突処理
void CBeholder::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	//if (self == mpAttackCol && mState != EState::eIdle && mState != EState::eIdle2 &&
	//	mState != EState::eIdle3)
	//{
	//	// キャラのポインタに変換
	//	CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
	//	// 相手のコライダーの持ち主がキャラであれば、
	//	if (chara != nullptr)
	//	{
	//		// 既に攻撃済みのキャラでなければ
	//		if (!IsAttackHitObj(chara))
	//		{
	//			int damage = CalcDamage(this, chara);

	//			// ダメージを与える
	//			chara->TakeDamage(damage, this);

	//			// 攻撃済みリストに追加
	//			AddAttackHitObj(chara);
	//		}
	//	}
	//}
	//else if (self == mpColliderLine)
	//{
	//	if (other->Layer() == ELayer::eField)
	//	{
	//		Position(Position() + hit.adjust * hit.weight);
	//		mIsGrounded = true;
	//		//mMoveSpeed.Y(0.0f);

	//		if (other->Tag() == ETag::eRideableObject)
	//		{
	//			mpRideObject = other->Owner();
	//		}
	//	}
	//}
	// キャラクター同士の衝突処理
	/*else if (self == mpColliderSphere)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}*/
}

// 攻撃開始
void CBeholder::AttackStart()
{
	//CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	//mpAttackCol->SetEnable(true);
}

// 攻撃終了
void CBeholder::AttackEnd()
{
	//CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	//mpAttackCol->SetEnable(false);
}

// 描画
void CBeholder::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CBeholder::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CBeholder::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY4_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	//mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	//mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CBeholder::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		mState = EState::eHit;
	}
	// HPが0以下になったら、
	if (mCharaStatus.hp <= 0)
	{
		// 死亡処理
		Death();
	}

	if (causedObj != nullptr)
	{
		// ダメージを与えた相手の方向へ向く
		CVector dir = causedObj->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));

		// ノックバックでダメージを与えた相手の方向から後ろにズラす
		Position(Position() - dir * Scale().X() * 0.4f);
	}
}


// 死亡処理
void CBeholder::Death()
{
	// 死亡状態へ移行
	mState = EState::eDie;
}