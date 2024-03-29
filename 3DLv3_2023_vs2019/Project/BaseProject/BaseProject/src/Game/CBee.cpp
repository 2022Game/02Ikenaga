#include "CBee.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "Maths.h"

// 蜂のインスタンス
CBee* CBee::spInstance = nullptr;

#define ENEMY_HEIGHT 0.3f
#define WITHIN_RANGE 40.0f       // 範囲内
#define MOVE_SPEED 0.12f         // 移動速度
#define GRAVITY 0.0625f          // 重力
#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 18.0f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// 蜂のアニメーションデータのテーブル
const CBee::AnimData CBee::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Enemy\\Bee\\animation\\BeeIdle.x",	true,	40.0f	},	    // 待機 20.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeAttack.x",	true,	40.0f	},	    // 攻撃 17.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeGetHit.x",	true,	50.0f	},	    // ヒット 13.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeDie.x",	true,	90.0f	},	        // 死ぬ 20.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveBWD.x",	true,	42.0f	},	    // 移動 21.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeMoveFWD.x",	true,	42.0f	},	    // 移動2 21.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveLFT.x",	true,	42.0f	},	    // 左移動 21.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveRGT.x",	true,	42.0f	},	    // 右移動 21.0f
};

// コンストラクタ
CBee::CBee()
	: mpRideObject(nullptr)
	, mAttackTime(0)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Bee");

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
	mpColliderLine->Position(0.0f, 5.0f, 0.0f);

	// キャラクター押し戻し処理
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.3f, false, 5.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere->Position(0.0f, 0.3f, -0.2f);
	mpColliderSphere->SetEnable(false);

	// ダメージを受けるコライダーを作成(体)
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.25f, false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	// ダメージを受けるコライダーを少し上へずらす
	mpDamageCol->Position(0.0f, 0.18f, 0.0f);

	// ダメージを受けるコライダーを作成(尻尾)
	mpDamageCol2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.1f, false
	);
	mpDamageCol2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol2->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol2->Position(0.02f, 0.14f, 0.0f);

	// ダメージを与えるコライダー
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.01f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(-0.28f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpDamageCol->SetAttachMtx(bodyMty);

	// ダメージを受けるコライダーを尻尾の行列にアタッチ
	const CMatrix* tailMty = GetFrameMtx("Armature_Tail01");
	mpDamageCol2->SetAttachMtx(tailMty);

	// 攻撃コライダーを蜂の針の行列にアタッチ
	const CMatrix* needleMty = GetFrameMtx("Armature_Tail02");
	mpAttackCol->SetAttachMtx(needleMty);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackCol->SetEnable(false);
}

CBee::~CBee()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);
}

CBee* CBee::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CBee::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 待機状態
void CBee::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mState = EState::eRun;
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle);
		if (IsAnimationFinished())
		{
			mState = EState::eIdle;
		}
	}
}

// 攻撃
void CBee::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃終了待ち
void CBee::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle;
	}
}

// ヒット
void CBee::UpdateHit()
{
	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// 死ぬ
void CBee::UpdateDie()
{
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::Death();
	}
}

// 移動
void CBee::UpdateRun()
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
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// 更新処理
void CBee::Update()
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
		// 攻撃
	case EState::eAttack:
		UpdateAttack();
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
		// 移動
	case EState::eRun:
		UpdateRun();
		break;
	}

	// HPゲージの座標を更新(敵の座標の少し上の座標)
	CVector gaugePos = Position() + CVector(0.0f, 35.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eAttack && mState != EState::eAttackWait
		&& mState != EState::eHit && mState != EState::eDie && mState != EState::eRun)
	{
		mState = EState::eIdle;
	}

	if (mState == EState::eRun || mState == EState::eIdle || mState == EState::eAttack ||
		mState == EState::eDie || mState == EState::eHit || mState == EState::eAttackWait)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle && vectorp <= 40.0f || mState == EState::eHit || mState == EState::eRun
		|| mState == EState::eAttack || mState == EState::eAttackWait)
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
			mState = EState::eAttack;
		}
		if (mState == EState::eAttack)
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
		if (Position().Y() < 0.0f)
		{
			mMoveSpeed += CVector(0.0f, 1.0f, 0.0f);
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

	if (Position().Y() >= 90.0f || Position().Y() <= -90.0f)
	{
		mState = EState::eDie;
	}

	// キャラクターの更新
	CXCharacter::Update();

	mpDamageCol->Update();
	mpDamageCol2->Update();
	mpAttackCol->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CBee::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackCol && mState != EState::eIdle)
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
	else if (self == mpColliderSphere)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CBee::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
}

// 攻撃終了
void CBee::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
}

// 描画
void CBee::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CBee::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CBee::ChangeLevel(int level)
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
void CBee::TakeDamage(int damage, CObjectBase* causedObj)
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
		Position(Position() - dir * Scale().X() * 0.6f);
	}
}


// 死亡処理
void CBee::Death()
{
	// 死亡状態へ移行
	mState = EState::eDie;
}