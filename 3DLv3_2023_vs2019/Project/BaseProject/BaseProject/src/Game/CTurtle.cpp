#include "CTurtle.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"

// 亀のインスタンス
CTurtle* CTurtle::spInstance = nullptr;

int CTurtle::mHp;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 40.0f       // 範囲内
#define MOVE_SPEED 0.07f         // 移動速度
#define GRAVITY 0.0625f          // 重力
#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 24.5f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// 亀のアニメーションデータのテーブル
const CTurtle::AnimData CTurtle::ANIM_DATA[] =
{
	{ "",										                    true,	 0.0f,	0.0f},  // Tポーズ
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdleNormal.x",	true,	51.0f,	0.5f},  // 待機 51.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdleBattle.x",	true,	25.0f,	0.5f},  // 待機2 25.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdle.x",	        true,	71.0f,	0.5f},  // 見回す待機 71.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdle2.x",     	true,	61.0f,	0.5f},  // 見回す待機2 61.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleAttack.x",	    false,	26.0f,	0.5f},	// 攻撃 26.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleAttack2.x",	    false,  26.0f,	0.5f},	// 攻撃2 26.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleGetHit.x",	    true,	26.0f,	0.5f},	// ヒット 26.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDefend.x",	    false,	18.0f,	0.5f},	// 防御 18.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDefendHit.x",	    true,	 8.0f,	0.3f},	// 防御中のヒット 8.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDefendIdle.x",	true,	 8.0f,	0.3f},	// 防御中の待機 8.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDie.x",	        true,	61.0f,	0.5f},	// 死ぬ 61.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDizzy.x",	        true,	41.0f,	0.5f},	// めまい 41.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleRun.x",	        true,	17.0f,	0.45f}, // 走る 17.0f

};

// コンストラクタ
CTurtle::CTurtle()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mDefenseTime(0)
	, mIsGrounded(false)
	, mMoveSpeed(CVector::zero)
	, mStateAttack2Step(0)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Turtle");

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
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.65f, false, 5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy});
	mpColliderSphereBody->Position(0.3f, 0.5f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.65f, false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	// ダメージを受けるコライダーを少し上へずらす
	mpDamageColBody->Position(0.3f, 0.5f, 0.0f);

	// ダメージを与えるコライダー(体)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.65f, false
	);
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });
	mpAttackColBody->Position(0.3f, 0.5f, 0.0f);

	// キャラクター押し戻しコライダーと
	// ダメージを受けるコライダーと攻撃コライダーを亀の体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);
	mpAttackColBody->SetAttachMtx(bodyMty);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColBody->SetEnable(false);
}

CTurtle::~CTurtle()
{
	SAFE_DELETE(mpColliderLine);
	//　キャラの押し戻しコライダー
	SAFE_DELETE(mpColliderSphereBody);
	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColBody);
	// 攻撃コライダー
	SAFE_DELETE(mpAttackColBody);
}

// インスタンス
CTurtle* CTurtle::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CTurtle::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 状態の切り替え
void CTurtle::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttack2Step = 0;
}

// 待機状態
void CTurtle::UpdateIdle()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// 待機状態2
void CTurtle::UpdateIdle2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE && player->Position().Y() < 0.7f)
	{
		ChangeState(EState::eRun);
	}

	if (vectorp <= 30.0f && player->Position().Y() >= 0.7f)
	{
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// 待機状態3
void CTurtle::UpdateIdle3()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// 攻撃
void CTurtle::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// 攻撃終了待ち状態へ移行
	ChangeState(EState::eAttackWait);
}

// 攻撃2
void CTurtle::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateAttack2Step)
	{
		// ステップ0 : 攻撃2アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		AttackStart();
		mStateAttack2Step++;
		break;
	case 1:
		if (mAnimationFrame >= 10.0f)
		{
			mpColliderSphereBody->SetEnable(false);
			mStateAttack2Step++;
		}
		break;
	case 2:
		if (mAnimationFrame >= 20.0f)
		{
			// 攻撃2終了待ち状態へ移行
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃終了待ち
void CTurtle::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpColliderSphereBody->SetEnable(true);
		ChangeState(EState::eIdle2);
	}
}

// ヒット
void CTurtle::UpdateHit()
{
	SetAnimationSpeed(0.5f);
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
			ChangeState(EState::eDizzy);
		}
		else
		{
			// プレイヤーの攻撃がヒットした時の待機状態へ移行
			ChangeState(EState::eIdle2);
		}
	}
}

// 防御
void CTurtle::UpdateDefense()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDefense);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eDefenseIdle);
	}
}

// 防御中のヒット
void CTurtle::UpdateDefenseHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eDefenseHit);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eDefenseIdle);
	}
}

// 防御中の待機
void CTurtle::UpdateDefenseIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eDefenseIdle);

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= ROTATE_RANGE)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (IsAnimationFinished())
	{
		if (mDefenseTime >= 800)
		{
			ChangeState(EState::eAttack2);
		}
		else
		{
			ChangeState(EState::eDefenseIdle);
		}
	}
}

// 死ぬ
void CTurtle::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::TurtleDeath();
	}
}

// めまい(混乱)
void CTurtle::UpdateDizzy()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		ChangeState(EState::eIdle2);
	}
}

// 走る
void CTurtle::UpdateRun()
{
	SetAnimationSpeed(0.45f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// 範囲内の時、移動し追跡する
	if (vectorp >= 24.5f && vectorp <= WALK_RANGE && player->Position().Y() < 0.7f)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
		// 回転する範囲であれば
		if (vectorp <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	if (vectorp <= 30.0f && player->Position().Y() >= 0.7f)
	{
		ChangeState(EState::eIdle2);
	}
	// 追跡が止まった時、攻撃用の待機モーションへ
	else if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// 更新処理
void CTurtle::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mHp = mCharaStatus.hp;
	mMoveSpeed.Y(0.0f);

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
		// 防御
	case EState::eDefense:
		UpdateDefense();
		break;
		// 防御中のヒット
	case EState::eDefenseHit:
		UpdateDefenseHit();
		break;
		// 防御中のヒット
	case EState::eDefenseIdle:
		UpdateDefenseIdle();
		break;
		// 死ぬ
	case EState::eDie:
		UpdateDie();
		break;
		// めまい(混乱)
	case EState::eDizzy:
		UpdateDizzy();
		break;
		// 走る
	case EState::eRun:
		UpdateRun();
		break;
	}

	// HPゲージの座標を更新(敵の座標の少し上の座標)
	CVector gaugePos = Position() + CVector(0.0f, 32.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle2 && mState != EState::eAttack && mState != EState::eAttack2)
	{
		if (mState != EState::eAttackWait && mState != EState::eHit && mState != EState::eDefense && mState != EState::eDefenseHit)
		{
			if (mState != EState::eDefenseIdle && mState != EState::eDie && mState != EState::eDizzy && mState != EState::eRun)
			{
				ChangeState(EState::eIdle2);
			}
		}
	}

	if (mState == EState::eRun || mState == EState::eIdle2 || mState == EState::eAttack || mState == EState::eAttack2 ||
		mState == EState::eDefense || mState == EState::eHit || mState == EState::eDizzy || mState == EState::eAttackWait
		|| mState == EState::eDefenseHit || mState == EState::eDefenseIdle)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState != EState::eDefense || mState == EState::eDefenseIdle)
	{
		if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eDefense
			|| mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttackWait)
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
				// 防御
				bool Defense = false;
				// 攻撃2の確率を最小に2最大5
				int probability2 = Math::Rand(2, 4);
				// 防御の確率を最小に5最大7
				int probability3 = Math::Rand(4, 6);

				if (probability2 == 2)Attack2 = true;
				if (probability3 == 4)Defense = true;

				if (Attack2)
				{
					ChangeState(EState::eAttack2);
				}
				else if (Defense && mState != EState::eDefenseIdle)
				{
					ChangeState(EState::eDefense);
				}
				else
				{
					ChangeState(EState::eAttack);
				}
			}
			if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eDefense)
			{
				mAttackTime = 0;
			}
		}
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (Position().Y() >= 0.5f)
	{
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	}

	// 防御時間計測
	if (mState == EState::eDefense || mState == EState::eDefenseHit || mState == EState::eDefenseIdle)
	{
		mDefenseTime++;
	}
	else
	{
		mDefenseTime = 0;
	}

	// キャラクターの更新
	CXCharacter::Update();

	// キャラの押し戻しコライダー
	mpColliderSphereBody->Update();
	// ダメージを受けるコライダー
	mpDamageColBody->Update();
	// 攻撃コライダー
	mpAttackColBody->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
	CDebugPrint::Print("HP %d\n", mCharaStatus.hp);
}

// 衝突処理
void CTurtle::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColBody && mState != EState::eIdle && mState != EState::eIdle2 &&
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
	else if (self == mpColliderSphereBody)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CTurtle::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	if (mState == EState::eAttack || mState == EState::eAttack2)
	{
		mpAttackColBody->SetEnable(true);
	}
}

// 攻撃終了
void CTurtle::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColBody->SetEnable(false);
}

// 描画
void CTurtle::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CTurtle::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CTurtle::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY3_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CTurtle::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		if (mState == EState::eDefense || mState == EState::eDefenseIdle)
		{
			ChangeState(EState::eDefenseHit);
		}
		else
		{
			ChangeState(EState::eHit);
		}
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
		Position(Position() - dir * Scale().X() * 0.01f);
	}
}

// 防御力の強化割合を取得
float CTurtle::GetDefBuff(const CVector& attackDir)const
{
	// 防御状態であれば、防御2倍
	if (mState == EState::eDefense|| mState == EState::eDefenseIdle) return 2.0f;

	// 通常時の防御の割合
	return mBaseDefenseBuffRatio;
}


// 死亡処理
void CTurtle::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}