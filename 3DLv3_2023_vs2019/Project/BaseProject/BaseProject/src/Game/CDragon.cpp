#include "CDragon.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "Maths.h"

// ドラゴンのインスタンス
CDragon* CDragon::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 40.0f    // 範囲内

// ドラゴンのアニメーションデータのテーブル
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	//{ "Character\\Enemy\\Dragon\\animation\\DragonIdle.x",	true,	82.0f	},	    // 待機 41.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonIdle2.x",	true,	202.0f	},	    // 待機 101.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonAttack.x",	true,	162.0f	},	    // 攻撃 81.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonAttack2.x",	true,	182.0f	},	    // 攻撃2 91.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonAttack3.x",	true,	72.0f	},	    // 攻撃3 36.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonDefend.x",	true,	122.0f	},	    // 防御 61.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonDie.x",	true,	130.0f	},	        // 死ぬ 65.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyFlame.x",	true,	182.0f	},	// フライフレーム 91.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyForward.x",	true,	62.0f	},	// フライフォワード 31.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyGlide.x",	true,	51.0f	},	// フライグライド 51.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyIdle.x",	true,	62.0f	},	    // フライアイドル 31.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonGetHit.x",	true,	82.0f	},	    // フライアイドル 41.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonLand.x",	true,	242.0f	},	    // 着地 121.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonRun.x",	true,	42.0f	},	        // 走る 21.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonScream.x",	true,	202.0f	},	    // 雄叫び 101.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonSleep.x",	true,	162.0f	},	    // 寝る 81.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonTakeOff.x",	true,	242.0f	},	    // 飛行開始する前 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonWalk.x",	true,	82.0f	},	        // 歩く 41.0f
};

// コンストラクタ
CDragon::CDragon()
	: mpRideObject(nullptr)
	, mAttackTime(0)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Dragon");

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

CDragon::~CDragon()
{
	/*SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);*/
}

CDragon* CDragon::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CDragon::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 戦う前の待機状態
void CDragon::UpdateIdle()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// 戦う前の待機状態2
void CDragon::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// 待機状態3
void CDragon::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// 攻撃
void CDragon::UpdateAttack()
{
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃2
void CDragon::UpdateAttack2()
{
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}


// 攻撃3
void CDragon::UpdateAttack3()
{
	ChangeAnimation(EAnimType::eAttack3);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃終了待ち
void CDragon::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle3;
	}
}

// ヒット
void CDragon::UpdateHit()
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
			mState = EState::eIdle3;
			ChangeAnimation(EAnimType::eIdle3);
		}
	}
}

// 死ぬ
void CDragon::UpdateDie()
{
	//ChangeAnimation(EAnimType::eDie);
}

// めまい(混乱)
void CDragon::UpdateDizzy()
{
	//ChangeAnimation(EAnimType::eDizzy);
}

// 更新処理
void CDragon::Update()
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
		// 攻撃2
	case EState::eAttack2:
		UpdateAttack2();
		break;
		// 攻撃3
	case EState::eAttack3:
		UpdateAttack3();
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
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle3)
	{
		//UpdateIdle();
	}
	if (mState == EState::eIdle3)
	{
		//mAttackTime++;
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
			//	mState = EState::eAttack;
			//}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3)
		{
			mAttackTime = 0;
		}
	}

	CDebugPrint::Print(" 攻撃時間: %d\n", mAttackTime);
	CDebugPrint::Print(" HP: %d\n", mCharaStatus.hp);

	// キャラクターの更新
	CXCharacter::Update();

	//mpAttackCol->Update();

	mIsGrounded = false;

	if (CInput::PushKey('Q'))
	{
		mState = EState::eAttack;
	}
}

// 衝突処理
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CDragon::AttackStart()
{
	//CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	//mpAttackCol->SetEnable(true);
}

// 攻撃終了
void CDragon::AttackEnd()
{
	//CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	//mpAttackCol->SetEnable(false);
}

// 描画
void CDragon::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CDragon::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CDragon::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY2_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	//mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	//mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CDragon::TakeDamage(int damage, CObjectBase* causedObj)
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
void CDragon::Death()
{
	// 死亡状態へ移行
	mState = EState::eDie;
}