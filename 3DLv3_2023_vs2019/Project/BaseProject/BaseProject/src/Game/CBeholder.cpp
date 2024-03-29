#include "CBeholder.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"

// 球体のモンスターのインスタンス
CBeholder* CBeholder::spInstance = nullptr;

#define ENEMY_HEIGHT 0.3f
#define WITHIN_RANGE 40.0f       // 範囲内
#define MOVE_SPEED 0.1f          // 移動速度
#define GRAVITY 0.0625f          // 重力
#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 24.0f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// 球体のモンスターのアニメーションデータのテーブル
const CBeholder::AnimData CBeholder::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle.x",	true,	82.0f	},	    // 待機 41.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle2.x",	true,	46.0f	},	    // 待機2 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack.x",	true,	50.0f	},	    // 攻撃 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack2.x",	true,	50.0f	},	    // 攻撃2 21.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack3.x",	true,	42.0f	},	    // 攻撃3 17.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack4.x",	true,	50.0f	},	    // 攻撃4 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderGetHit.x",	true,	46.0f	},	    // ヒット 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderDie.x",	true,	76.0f	},	        // 死ぬ 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderDizzy.x",	true,	82.0f	},	    // めまい 41.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderRun.x",	true,	34.0f	},	    // 走る 17.0f
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

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// キャラクター押し戻し処理(体)
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eEnemy,0.41f, false, 2.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(左上の触手)
	mpColliderSphere2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphere2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(右上の触手)
	mpColliderSphere3 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.1f, false, 1.0f
	);
	mpColliderSphere3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(左下の触手)
	mpColliderSphere4 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphere4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(右下の触手)
	mpColliderSphere5 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphere5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(真ん中上の触手)
	mpColliderSphere6 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 1.0f
	);
	mpColliderSphere6->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(真ん中下の触手)
	mpColliderSphere7 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphere7->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// ダメージを受けるコライダーを作成(体)
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.41f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(左上の触手)
	mpDamageCol2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.087f, false
	);
	mpDamageCol2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol2->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(右上の触手)
	mpDamageCol3 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.1f, false
	);
	mpDamageCol3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol3->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(左下の触手)
	mpDamageCol4 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageCol4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol4->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(右下の触手)
	mpDamageCol5 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageCol5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol5->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(真ん中上の触手)
	mpDamageCol6 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.08f, false
	);
	mpDamageCol6->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol6->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(真ん中下の触手)
	mpDamageCol7 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageCol7->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol7->SetCollisionTags({ ETag::eWeapon });

	// ダメージを与えるコライダー(体)
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.41f, false
	);
	// 攻撃コライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(左上の触手)
	mpAttackCol2 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackCol2->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol2->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(右上の触手)
	mpAttackCol3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.1f, false
	);
	mpAttackCol3->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol3->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(左下の触手)
	mpAttackCol4 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackCol4->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol4->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(右下の触手)
	mpAttackCol5 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackCol5->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol5->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(真ん中上の触手)
	mpAttackCol6 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.08f, false
	);
	mpAttackCol6->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol6->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(真ん中下の触手)
	mpAttackCol7 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackCol7->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol7->SetCollisionTags({ ETag::ePlayer });

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphere->SetAttachMtx(bodyMty);
	mpDamageCol->SetAttachMtx(bodyMty);
	mpAttackCol->SetAttachMtx(bodyMty);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの左上の触手の行列にアタッチ
	const CMatrix* tentacleMty = GetFrameMtx("Armature_TentacleA05");
	mpColliderSphere2->SetAttachMtx(tentacleMty);
	mpDamageCol2->SetAttachMtx(tentacleMty);
	mpAttackCol2->SetAttachMtx(tentacleMty);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの右上の触手の行列にアタッチ
	const CMatrix* tentacleMty2 = GetFrameMtx("Armature_TentacleE05");
	mpColliderSphere3->SetAttachMtx(tentacleMty2);
	mpDamageCol3->SetAttachMtx(tentacleMty2);
	mpAttackCol3->SetAttachMtx(tentacleMty2);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの左下の触手の行列にアタッチ
	const CMatrix * tentacleMty3 = GetFrameMtx("Armature_TentacleB05");
	mpColliderSphere4->SetAttachMtx(tentacleMty3);
	mpDamageCol4->SetAttachMtx(tentacleMty3);
	mpAttackCol4->SetAttachMtx(tentacleMty3);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの右下の触手の行列にアタッチ
	const CMatrix* tentacleMty4 = GetFrameMtx("Armature_TentacleD05");
	mpColliderSphere5->SetAttachMtx(tentacleMty4);
	mpDamageCol5->SetAttachMtx(tentacleMty4);
	mpAttackCol5->SetAttachMtx(tentacleMty4);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの真ん中上の触手の行列にアタッチ
	const CMatrix* tentacleMty5 = GetFrameMtx("Armature_TentacleF05");
	mpColliderSphere6->SetAttachMtx(tentacleMty5);
	mpDamageCol6->SetAttachMtx(tentacleMty5);
	mpAttackCol6->SetAttachMtx(tentacleMty5);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの真ん中下の触手の行列にアタッチ
	const CMatrix* tentacleMty6 = GetFrameMtx("Armature_TentacleC05");
	mpColliderSphere7->SetAttachMtx(tentacleMty6);
	mpDamageCol7->SetAttachMtx(tentacleMty6);
	mpAttackCol7->SetAttachMtx(tentacleMty6);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackCol->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
	mpAttackCol4->SetEnable(false);
	mpAttackCol5->SetEnable(false);
	mpAttackCol6->SetEnable(false);
	mpAttackCol7->SetEnable(false);
}

CBeholder::~CBeholder()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpColliderSphere2);
	SAFE_DELETE(mpColliderSphere3);
	SAFE_DELETE(mpColliderSphere4);
	SAFE_DELETE(mpColliderSphere5);
	SAFE_DELETE(mpColliderSphere6);
	SAFE_DELETE(mpColliderSphere7);

	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpDamageCol2);
	SAFE_DELETE(mpDamageCol3);
	SAFE_DELETE(mpDamageCol4);
	SAFE_DELETE(mpDamageCol5);
	SAFE_DELETE(mpDamageCol6);
	SAFE_DELETE(mpDamageCol7);

	SAFE_DELETE(mpAttackCol);
	SAFE_DELETE(mpAttackCol2);
	SAFE_DELETE(mpAttackCol3);
	SAFE_DELETE(mpAttackCol4);
	SAFE_DELETE(mpAttackCol5);
	SAFE_DELETE(mpAttackCol6);
	SAFE_DELETE(mpAttackCol7);
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
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mState = EState::eRun;
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle2);
		if (IsAnimationFinished())
		{
			mState = EState::eIdle2;
		}
	}
}

// 攻撃
void CBeholder::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃2
void CBeholder::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃3
void CBeholder::UpdateAttack3()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack3);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃4
void CBeholder::UpdateAttack4()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack4);
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
			mState = EState::eDizzy;
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
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::Death();
	}
}

// めまい(混乱)
void CBeholder::UpdateDizzy()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		mState = EState::eIdle2;
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// 移動
void CBeholder::UpdateRun()
{
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// 追跡をやめて止まる
	if (vectorp <= 20.0f && vectorp >= 23.0f)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);

		// 回転する範囲であれば
		if (vectorp <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));

			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
		}
	}
	// 範囲内の時、移動し追跡する
	else if (vectorp >= 24.0f && vectorp <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
	}
	// 追跡が止まった時、待機モーションへ
	if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		mState = EState::eIdle2;
		ChangeAnimation(EAnimType::eIdle2);
	}
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
		// 攻撃3
	case EState::eAttack3:
		UpdateAttack3();
		break;
		// 攻撃4
	case EState::eAttack4:
		UpdateAttack4();
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
		// 移動
	case EState::eRun:
		UpdateRun();
		break;
	}

	// HPゲージの座標を更新(敵の座標の少し上の座標)
	CVector gaugePos = Position() + CVector(0.0f, 35.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eAttack 
		&& mState != EState::eAttack2 && mState != EState::eAttack3 && mState != EState::eAttack4  && mState != EState::eAttackWait
		&& mState != EState::eHit && mState != EState::eDie && mState != EState::eDizzy && mState != EState::eRun)
	{
		mState = EState::eIdle2;
	}

	if (mState == EState::eRun || mState == EState::eIdle2 || mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3
		|| mState == EState::eAttack4 || mState == EState::eHit || mState == EState::eDie || mState == EState::eDizzy || mState == EState::eAttackWait)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
	{
		mAttackTime++;

		if (vectorp <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}

		if (mAttackTime > 200)
		{
			// 攻撃2
			bool Attack2 = false;
			// 攻撃3
			bool Attack3 = false;
			// 攻撃4
			bool Attack4 = false;
			// 確率を最小に2最大5
			int probability2 = Math::Rand(2, 5);
			// 確率を最小に6最大10
			int probability3 = Math::Rand(6, 10);
			// 確率を最小に11最大15
			int probability4 = Math::Rand(11, 15);

			if (probability2 == 2)Attack2 = true;
			if (probability3 == 6)Attack3 = true;
			if (probability4 == 11)Attack4 = true;
			if (Attack2)
			{
				mState = EState::eAttack2;
			}
			else if (Attack3)
			{
				mState = EState::eAttack3;
			}
			else if (Attack4)
			{
				mState = EState::eAttack4;
			}
			else
			{
				mState = EState::eAttack;
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3 || mState == EState::eAttack4)
		{
			mAttackTime = 0;
		}
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (mState == EState::eRun)
	{
		if (Position().Y() < 2.0f)
		{
			mMoveSpeed += CVector(0.0f, 0.5f, 0.0f);
		}

		if (Position().Y() >= 10.0f && Position().Y() < 12.0f)
		{
			mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
		}

		if (Position().Y() >= 20.0f)
		{
			mMoveSpeed -= CVector(0.0f, 1.0f, 0.0f);
		}
	}

	CDebugPrint::Print(" 攻撃時間: %d\n", mAttackTime);
	//CDebugPrint::Print(" HP: %d\n", mCharaStatus.hp);

	// キャラクターの更新
	CXCharacter::Update();

	mpColliderSphere->Update();
	mpColliderSphere2->Update();
	mpColliderSphere3->Update();
	mpColliderSphere4->Update();
	mpColliderSphere5->Update();
	mpColliderSphere6->Update();
	mpColliderSphere7->Update();

	mpDamageCol->Update();
	mpDamageCol2->Update();
	mpDamageCol3->Update();
	mpDamageCol4->Update();
	mpDamageCol5->Update();
	mpDamageCol6->Update();
	mpDamageCol7->Update();

	mpAttackCol->Update();
	mpAttackCol2->Update();
	mpAttackCol3->Update();
	mpAttackCol4->Update();
	mpAttackCol5->Update();
	mpAttackCol6->Update();
	mpAttackCol7->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CBeholder::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackCol || self == mpAttackCol2 || self == mpAttackCol3 || self == mpAttackCol4 || self == mpAttackCol5
		|| self == mpAttackCol6 || self == mpAttackCol7 && mState != EState::eIdle && mState != EState::eIdle2)
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
	else if (self == mpColliderSphere || self == mpColliderSphere2 || self == mpColliderSphere3
		|| self == mpColliderSphere4 || self == mpColliderSphere5 || self == mpColliderSphere6 || self == mpColliderSphere7)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CBeholder::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
	mpAttackCol2->SetEnable(true);
	mpAttackCol3->SetEnable(true);
	mpAttackCol4->SetEnable(true);
	mpAttackCol5->SetEnable(true);
	mpAttackCol6->SetEnable(true);
	mpAttackCol7->SetEnable(true);
}

// 攻撃終了
void CBeholder::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
	mpAttackCol4->SetEnable(false);
	mpAttackCol5->SetEnable(false);
	mpAttackCol6->SetEnable(false);
	mpAttackCol7->SetEnable(false);
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

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
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