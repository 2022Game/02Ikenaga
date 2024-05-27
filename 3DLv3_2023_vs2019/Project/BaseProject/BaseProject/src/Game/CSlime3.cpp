#include "CSlime3.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CPlayer.h"
#include "CCamera.h"

// ブルースライム(エネミー)のインスタンス
CSlime3* CSlime3::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define MOVE_SPEED 0.07f    // 移動速度
#define GRAVITY 0.0625f     // 重力

#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 24.5f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// ブルースライム(エネミー)のアニメーションデータのテーブル
const CSlime3::AnimData CSlime3::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// Tポーズ
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleNormal.x",	true,	90.0f	},  // アイドル通常 51.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingStart.x",	true,	150.0f	},  // 開始の見回す 63.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingRoutine.x",	true,160.0f	},  // 見回す 71.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleBattle.x",	true,	50.0f	},  // アイドルバトル 25.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack.x",	true,	52.0f	},  // 攻撃 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack2.x",	true,	80.0f	},  // 攻撃2 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeGetHit.x",	true,	70.0f	},  // ヒット 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeDie.x",	true,	90.0f	},  // 死ぬ 41.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeDizzy.x",	true,	100.0f	},  // めまい 41.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeRun.x",	true,	30.0f	},  // 走る
	{ "Character\\Enemy\\Slime\\animation\\SlimeWalk.x",	true,	31.0f	},  // 歩く
	//{ "Character\\Enemy\\Slime\\animation\\SlimeTaunt.x",	true,	21.0f	},  // 挑発
	//{ "Character\\Enemy\\Slime\\animation\\SlimeVictory.x",	true,	81.0f	},  // 勝利
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkBack.x",	true,	31.0f	},  // 後ろに歩く
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkLeft.x",	true,	31.0f	},  // 左に移動
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkRight.x",	true,	31.0f	},  // 右に移動
};

bool CSlime3::IsDeath() const
{
	return mCharaStatus.hp <= 0;
}

int CSlime3::mHp;

// コンストラクタ
CSlime3::CSlime3()
	:mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Slime3");

	//最初に5レベルに設定
	ChangeLevel(5);

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
		this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// キャラクター押し戻し処理
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.5f, false, 5.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere->Position(0.0f, 0.3f, 0.0f);

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

	// ダメージを与えるコライダー
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.5f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.0f, 0.3f, 0.0f);

	// 攻撃コライダーをスライムのBodyの行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpAttackCol->SetAttachMtx(bodyMty);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackCol->SetEnable(false);
}

CSlime3::~CSlime3()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	// ダメージを受けるコライダーを削除
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);
}

CSlime3* CSlime3::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CSlime3::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 待機状態
void CSlime3::UpdateIdle()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// 待機2状態
void CSlime3::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdleWait;
	}
}

// 攻撃した時の待機状態
void CSlime3::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle4);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mState = EState::eRun;
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle4);
		if (IsAnimationFinished())
		{
			mState = EState::eIdle3;
		}
	}
}

// 待機2の終了待ち
void CSlime3::UpdateIdleWait()
{
	// 待機3アニメーションに切り替え
	ChangeAnimation(EAnimType::eIdle3);
	// 待機3アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// 攻撃
void CSlime3::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃2
void CSlime3::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// 攻撃2アニメーションを開始
	ChangeAnimation(EAnimType::eAttack2);
	if (mAnimationFrame >= 35.0f)
	{
		// 攻撃2終了待ち状態へ移行
		mState = EState::eAttackWait;
		AttackStart();
	}
}

// 攻撃と攻撃2終了待ち
void CSlime3::UpdateAttackWait()
{
	// 攻撃と攻撃2アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		AttackEnd();
		CPlayer* player = CPlayer::Instance();
		float vectorp = (player->Position() - Position()).Length();
		if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
		{
			mState = EState::eRun;
		}
		else
		{
			// プレイヤーの攻撃がヒットした時の待機状態へ移行
			mState = EState::eIdle3;
			ChangeAnimation(EAnimType::eIdle4);
		}
	}
}

// ヒット
void CSlime3::UpdateHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// めまいをfalseにする
		bool stan = false;
		// 確率を最小に0最大20
		int probability = Math::Rand(0, 20);
		if (probability == 1)stan = true;
		if (stan)
		{
			mState = EState::eDizzy;
		}
		else
		{
			// プレイヤーの攻撃がヒットした時の待機状態へ移行
			mState = EState::eIdle3;
			ChangeAnimation(EAnimType::eIdle4);
		}
	}
}

// 死ぬ時
void CSlime3::UpdateDie()
{
	// 死ぬ時のアニメーションを開始
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::SlimeBigDeath();
	}
}

// めまい(混乱)
void CSlime3::UpdateDizzy()
{
	// めまい(混乱)アニメーションを開始
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle4);
	}
}

// 走る
void CSlime3::UpdateRun()
{
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();
	// 追跡をやめて止まる
	if (vectorp <= 22.0f && vectorp >= 24.0f)
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
	// 追跡が止まった時、攻撃用の待機モーションへ
	if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle4);
	}
}

// 更新処理
void CSlime3::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mHp = mCharaStatus.hp;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eIdle2:
		UpdateIdle2();
		break;
	case EState::eIdle3:
		UpdateIdle3();
		break;
		// 待機終了待ち
	case EState::eIdleWait:
		UpdateIdleWait();
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
		// 死ぬ時
	case EState::eDie:
		UpdateDie();
		break;
		// めまい(混乱)
	case EState::eDizzy:
		UpdateDizzy();
		break;
		// 歩行
	case EState::eRun:
		UpdateRun();
		break;
	}

	// HPゲージの座標を更新(敵の座標の少し上の座標)
	CVector gaugePos = Position() + CVector(0.0f, 40.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		mpHpGauge->SetWorldPos(gaugePos);

		mAttackTime++;
		if (mAttackTime > 230)
		{
			// 大きい攻撃
			bool BigAttack = false;
			// 確率を最小に8最大10
			int probability2 = Math::Rand(8, 11);
			if (probability2 == 8)BigAttack = true;
			if (BigAttack)
			{
				mState = EState::eAttack2;
			}
			else
			{
				mState = EState::eAttack;
			}
		}
		else if (mCharaStatus.hp <= 0)
		{
			mAttackTime = 0;
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eDizzy)
		{
			mAttackTime = 0;
		}
	}
	else
	{
		mpHpGauge->SetPos(-1000.0f, -1000.0f);
	}

	if (mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eIdleWait)
	{
		float vectorp = (player->Position() - Position()).Length();
		if (vectorp <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	}

	// キャラクターの更新
	CXCharacter::Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CSlime3::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackCol && mState != EState::eIdle && mState != EState::eIdle2 &&
		mState != EState::eIdle3 && mState != EState::eIdleWait)
	{
		// キャラのポインタに変換
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// 相手のコライダーの持ち主がキャラであれば、
		if (chara != nullptr)
		{
			// 既に攻撃済みのキャラでなければ
			if (!IsAttackHitObj(chara))
			{
				int damage = CalcDamage(1.0f,this, chara);

				// ダメージを与える
				chara->TakeDamage(damage, this);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
	// フィールドとの接地判定
	else if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;
			mMoveSpeed.Y(0.0f);

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
void CSlime3::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
}

// 攻撃終了
void CSlime3::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
}

// 描画
void CSlime3::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CSlime3::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CSlime3::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CSlime3::TakeDamage(int damage, CObjectBase* causedObj)
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
void CSlime3::Death()
{
	// 死亡状態へ移行
	mState = EState::eDie;
}