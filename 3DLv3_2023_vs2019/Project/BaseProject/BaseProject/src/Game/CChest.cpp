#include "CChest.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"
#include "CCoin.h"

// チェストモンスターのインスタンス
CChest* CChest::spInstance = nullptr;

#define ENEMY_HEIGHT 0.3f
#define WITHIN_RANGE 40.0f       // 範囲内
#define MOVE_SPEED 0.12f         // 移動速度
#define GRAVITY 0.0625f          // 重力
#define WALK_RANGE 150.0f        // 追跡する範囲
#define STOP_RANGE 32.0f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// チェストモンスターのアニメーションデータのテーブル
const CChest::AnimData CChest::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Enemy\\Chest\\animation\\ChestIdle.x",	true,	21.0f	},	    // 待機 21.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestIdle2.x",	true,	46.0f	},	    // 待機2 23.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestIdle3.x",	true,	82.0f	},	    // 待機3 41.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestAttack.x",	true,	50.0f	},	    // 攻撃 25.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestAttack2.x",	true,	46.0f	},	    // 攻撃 23.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestGetHit.x",	true,	60.0f	},	    // ヒット 19.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestDie.x",	true,	90.0f	},	        // 死ぬ 29.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestDizzy.x",	true,	82.0f	},	    // めまい 41.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestRun.x",	true,	34.0f	},	        // 走る 17.0f
};

// コンストラクタ
CChest::CChest()
	: mpRideObject(nullptr)
	, mAttackTime(0)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Chest");

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

	// キャラクター押し戻し処理(頭)
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.55f, false, 5.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere->Position(0.27f, 0.0f, 0.0f);

	// キャラクター押し戻し処理(体)
	mpColliderSphere2 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.55f, false, 5.0f
	);
	mpColliderSphere2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere2->Position(0.05f, 0.0f, 0.0f);

	// キャラクター押し戻し処理(前の左足)
	mpColliderSphere3 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 5.0f
	);
	mpColliderSphere3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere3->Position(0.0f, 0.1f, 0.0f);

	// キャラクター押し戻し処理(前の右足)
	mpColliderSphere4 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 5.0f
	);
	mpColliderSphere4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere4->Position(0.0f, 0.1f, 0.0f);

	// キャラクター押し戻し処理(後ろの左足)
	mpColliderSphere5 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 5.0f
	);
	mpColliderSphere5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere5->Position(0.0f, 0.1f, 0.0f);

	// キャラクター押し戻し処理(後ろの右足)
	mpColliderSphere6 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 5.0f
	);
	mpColliderSphere6->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere6->Position(0.0f, 0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(頭)
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.55f, false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	// ダメージを受けるコライダーを少し上へずらす
	mpDamageCol->Position(0.27f, 0.0f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageCol2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.55f, false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol2->SetCollisionTags({ ETag::eWeapon });
	// ダメージを受けるコライダーを少し上へずらす
	mpDamageCol2->Position(0.05f, 0.0f, 0.0f);

	// ダメージを与えるコライダー(頭)
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.55f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.27f, 0.0f, 0.0f);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーをチェストモンスターの頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpColliderSphere->SetAttachMtx(headMty);
	mpDamageCol->SetAttachMtx(headMty);
	mpAttackCol->SetAttachMtx(headMty);

	// 押し戻しコライダーとダメージを受けるコライダーをチェストモンスターの体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphere2->SetAttachMtx(bodyMty);
	mpDamageCol2->SetAttachMtx(bodyMty);

	// 押し戻しコライダーをチェストモンスターの前の左足の行列にアタッチ
	const CMatrix* leftFeetMty = GetFrameMtx("Armature_FrontLeftLeg02");
	mpColliderSphere3->SetAttachMtx(leftFeetMty);

	// 押し戻しコライダーをチェストモンスターの前の右足の行列にアタッチ
	const CMatrix* rightFeetMty = GetFrameMtx("Armature_FrontRightLeg02");
	mpColliderSphere4->SetAttachMtx(rightFeetMty);

	// 押し戻しコライダーをチェストモンスターの後ろの左足の行列にアタッチ
	const CMatrix* leftFeetMty2 = GetFrameMtx("Armature_RearLeftLeg02");
	mpColliderSphere5->SetAttachMtx(leftFeetMty2);

	// 押し戻しコライダーをチェストモンスターの後ろの右足の行列にアタッチ
	const CMatrix* rightFeetMty2 = GetFrameMtx("Armature_RearRightLeg02");
	mpColliderSphere5->SetAttachMtx(rightFeetMty2);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackCol->SetEnable(false);
}

CChest::~CChest()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpColliderSphere2);
	SAFE_DELETE(mpColliderSphere3);
	SAFE_DELETE(mpColliderSphere4);
	SAFE_DELETE(mpColliderSphere5);
	SAFE_DELETE(mpColliderSphere6);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpDamageCol2);
	SAFE_DELETE(mpAttackCol);
}

CChest* CChest::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CChest::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 戦う前の待機状態
void CChest::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// 待機状態2
void CChest::UpdateIdle2()
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

// 待機状態3
void CChest::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// 攻撃
void CChest::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	if (mAnimationFrame >= 5.0f && mAnimationFrame < 10.0f)
	{
		AttackStart();
	}

	if (mAnimationFrame >= 20.0f && mAnimationFrame < 30.0f)
	{
		AttackEnd();
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (mAnimationFrame >= 10.0f)
	{
		if (vectorp >= 45.0f)
		{
			// コインを生成済みフラグを初期化
			mIsSpawnedCoinEffect = false;
			// コインを生成していない
			if (!mIsSpawnedCoinEffect)
			{
				CCoin* coin = new CCoin
				(
					this,
					Position() + CVector(0.0f, 18.0f, 0.0f),
					VectorZ(),
					180.0f,
					150.0f
				);
				coin->SetColor(CColor(1.0f, 1.0f, 0.0f));
				coin->Scale(10.0f, 10.0f, 10.0f);
				coin->Rotate(-90.0f, 0.0f, 0.0f);
				coin->SetOwner(this);

				mIsSpawnedCoinEffect = true;
			}
		}
		// 攻撃終了待ち状態へ移行
		mState = EState::eAttackWait;
	}
}

// 攻撃2
void CChest::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃終了待ち
void CChest::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle2;
	}
}

// ヒット
void CChest::UpdateHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
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
void CChest::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::Death2();
	}
}

// めまい(混乱)
void CChest::UpdateDizzy()
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
void CChest::UpdateRun()
{
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// 追跡をやめて止まる
	if (vectorp <= STOP_RANGE && vectorp >= 35.0f)
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
void CChest::Update()
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
	CVector gaugePos = Position() + CVector(0.0f, 40.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eAttack
		&& mState != EState::eAttack2 && mState != EState::eAttackWait && mState != EState::eHit
		&& mState != EState::eDie && mState != EState::eDizzy && mState != EState::eRun)
	{
		UpdateIdle();
	}

	if (mState == EState::eRun || mState == EState::eIdle2 || mState == EState::eAttack || mState == EState::eAttack2 ||
		mState == EState::eHit || mState == EState::eDie || mState == EState::eDizzy || mState == EState::eAttackWait)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 && mState != EState::eDizzy || mState == EState::eRun)
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
			// 確率を最小に2最大5
			int probability2 = Math::Rand(2, 5);
			if (probability2 == 2)Attack2 = true;
			if (Attack2)
			{
				mState = EState::eAttack2;
			}
			else
			{
				mState = EState::eAttack;
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2)
		{
			mAttackTime = 0;
		}
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	// キャラクターの更新
	CXCharacter::Update();

	mpColliderSphere->Update();
	mpColliderSphere2->Update();
	mpColliderSphere3->Update();
	mpColliderSphere4->Update();
	mpColliderSphere5->Update();
	mpColliderSphere6->Update();

	mpDamageCol->Update();
	mpDamageCol2->Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CChest::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// キャラクター同士の衝突処理
	else if (self == mpColliderSphere || self == mpColliderSphere2 || self == mpColliderSphere3
		|| self == mpColliderSphere4 || self == mpColliderSphere5 || self == mpColliderSphere6)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CChest::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
}

// 攻撃終了
void CChest::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
}

// 描画
void CChest::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CChest::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CChest::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY7_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CChest::TakeDamage(int damage, CObjectBase* causedObj)
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
		Position(Position() - dir * Scale().X() * 0.2f);
	}
}

// 死亡処理
void CChest::Death()
{
	// 死亡状態へ移行
	mState = EState::eDie;
}