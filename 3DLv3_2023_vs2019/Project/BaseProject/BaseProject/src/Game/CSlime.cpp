#include "CSlime.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CPlayer.h"
#include "CCamera.h"

// レッドスライム(エネミー)のインスタンス
CSlime* CSlime::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define MOVE_SPEED 0.07f         // 移動速度
#define GRAVITY 0.0625f          // 重力

#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 21.0f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// レッドスライム(エネミー)のアニメーションデータのテーブル
const CSlime::AnimData CSlime::ANIM_DATA[] =
{
	{ "",										                         true,	0.0f,	0.0f},	// Tポーズ
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleNormal.x",	         true,	51.0f,	0.7f},  // アイドル通常 51.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingStart.x",	 true,	63.0f,  0.5f},  // 開始の見回す 63.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingRoutine.x",true,	71.0f,  0.5f},  // 見回す 71.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleBattle.x",	         true,	25.0f,	0.8f},  // アイドルバトル 25.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack.x",	             false,	26.0f,	0.5f},  // 攻撃 26.0ff
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack2.x",	             false,	26.0f,	0.5f},  // 攻撃2 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeGetHit.x",	             true,	26.0f,	0.5f},  // ヒット 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeDie.x",	                 true,	41.0f,	0.5f},  // 死ぬ 41.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeDizzy.x",	             true,	41.0f,	0.5f},  // めまい 41.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeRun.x",	                 true,	21.0f,	0.5f},  // 走る 21.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeWalk.x",	             true,	31.0f,	0.5f},  // 歩く 31.0f
//	{ "Character\\Enemy\\Slime\\animation\\SlimeWalkRight.x",	true,	31.0f	},  // 右に移動
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkLeft.x",	true,	31.0f	},  // 左に移動
	//{ "Character\\Enemy\\Slime\\animation\\SlimeTaunt.x",	true,	21.0f	},  // 挑発
	//{ "Character\\Enemy\\Slime\\animation\\SlimeVictory.x",	true,	81.0f	},  // 勝利
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkBack.x",	true,	31.0f	},  // 後ろに歩く
};

bool CSlime::IsDeath() const
{
	return mCharaStatus.hp <= 0;
}

int CSlime::mHp;

// コンストラクタ
CSlime::CSlime()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mIsGrounded(false)
	, mStateAttackStep(0)
	, mStateAttack2Step(0)
	, mStateDizzyStep(0)
	, mStateHitStep(0)
	, mStateDieStep(0)
	, mIsSlimeRunSE(false)
	, mIsSlimeAttackSE(false)
	, mIsSlimeDizzySE(false)
	, mIsSlimeHitSE(false)
	, mIsSlimeDieSE(false)
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Slime");

	// 最初に1レベルに設定
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

	SetAnimationSpeed(0.7f);
	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this,ELayer::eEnemy,
		0.5f,false,5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.06f, 0.3f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.5f,false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon,ETag::eSlash });
	//ダメージを受けるコライダーを少し上へずらす
	mpDamageColBody->Position(0.1f, 0.3f,0.0f);

	// ダメージを与えるコライダー(体)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.5f,false
	);
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });
	mpAttackColBody->Position(0.1f, 0.3f, 0.0f);
	
	// キャラクター押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーをスライムの体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);
	mpAttackColBody->SetAttachMtx(bodyMty);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColBody->SetEnable(false);

	mpSlimeRunSE = CResourceManager::Get<CSound>("SlimeRun");
	mpSlimeAttackSE = CResourceManager::Get<CSound>("SlimeAttack");
	mpSlimeDizzySE = CResourceManager::Get<CSound>("SlimeDizzy");
	mpSlimeHitSE = CResourceManager::Get<CSound>("SlimeHit");
	mpSlimeDieSE = CResourceManager::Get<CSound>("SlimeDie");
}

CSlime::~CSlime()
{
	SAFE_DELETE(mpColliderLine);
	// キャラ押し戻しコライダーを削除
	SAFE_DELETE(mpColliderSphereBody);
	// ダメージを受けるコライダーを削除
	SAFE_DELETE(mpDamageColBody);
	// 攻撃コライダーを削除
	SAFE_DELETE(mpAttackColBody);
}

// スライムのインスタンス
CSlime* CSlime::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CSlime::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 状態の切り替え
void CSlime::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
	mStateAttack2Step = 0;
	mStateDizzyStep = 0;
	mStateHitStep = 0;
	mStateDieStep = 0;
}

// 待機状態
void CSlime::UpdateIdle()
{
	SetAnimationSpeed(0.7f);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// 待機2状態
void CSlime::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeAnimation(EAnimType::eIdle3);
		ChangeState(EState::eIdleWait);
	}
}

// 攻撃した時の待機状態
void CSlime::UpdateIdle3()
{
	SetAnimationSpeed(0.8f);
	ChangeAnimation(EAnimType::eIdle4);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		if (player->Position().Y() <= Position().Y())
		{
			ChangeState(EState::eRun);
		}
	}
	else
	{
		SetAnimationSpeed(0.8f);
		ChangeAnimation(EAnimType::eIdle4);
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle3);
		}
	}
}

// 待機2の終了待ち
void CSlime::UpdateIdleWait()
{
	SetAnimationSpeed(0.5f);
	// 待機3アニメーションに切り替え
	ChangeAnimation(EAnimType::eIdle3);
	// 待機3アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		ChangeState(EState::eIdle);
	}
}

// 攻撃
void CSlime::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateAttackStep)
	{
		// ステップ0 : 攻撃アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		AttackStart();
		mStateAttackStep++;
		break;
		// ステップ1　: 効果音開始	
	case 1:
		if (mAnimationFrame >= 1.0f)
		{
			mpSlimeAttackSE->Play();
			mIsSlimeAttackSE = true;
			mStateAttackStep++;
		}
		break;
		// ステップ2 : 攻撃終了待ち
	case 2:
		if (mAnimationFrame >= 25.0f)
		{
			// 攻撃終了待ち状態へ移行
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃2
void CSlime::UpdateAttack2()
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
		if (mAnimationFrame >= 10.0f)
		{
			AttackStart();
			mpColliderSphereBody->SetEnable(false);
			mStateAttack2Step++;
		}
		break;
		// ステップ1　: 効果音開始	
	case 1:
		if (mAnimationFrame >= 1.0f)
		{
			mpSlimeAttackSE->Play();
			mIsSlimeAttackSE = true;
			mStateAttack2Step++;
		}
		break;
		// ステップ2 : 攻撃終了待ち
	case 2:
		if (mAnimationFrame >= 25.0f)
		{
			// 攻撃終了待ち状態へ移行
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃と攻撃2終了待ち
void CSlime::UpdateAttackWait()
{
	// 攻撃と攻撃2アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpColliderSphereBody->SetEnable(true);
		CPlayer* player = CPlayer::Instance();
		float vectorp = (player->Position() - Position()).Length();
		if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
		{
			if (player->Position().Y() <= Position().Y())
			{
				ChangeState(EState::eRun);
			}
		}
		else
		{
			SetAnimationSpeed(0.5f);
			// プレイヤーの攻撃がヒットした時の待機状態へ移行
			ChangeState(EState::eIdle3);
			ChangeAnimation(EAnimType::eIdle4);
		}
	}
}

// ヒット
void CSlime::UpdateHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateHitStep)
	{
		// ステップ0 :ヒットアニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eHit);
		mStateHitStep++;
		break;
		// ステップ1　: 効果音開始	
	case 1:
		if (mAnimationFrame >= 0.0f)
		{
			mpSlimeHitSE->Play(3.0f);
			mIsSlimeHitSE = true;
			mStateHitStep++;
		}
		break;
		// ステップ2 : ヒットモーション終了待ち
	case 2:
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
				ChangeState(EState::eIdle3);
				ChangeAnimation(EAnimType::eIdle4);
			}
		}
		break;
	}
}

// 死ぬ時
void CSlime::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateDieStep)
	{
		// ステップ0 :死亡アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eDie);
		mStateDieStep++;
		break;
		// ステップ1　: 効果音開始	
	case 1:
		if (mAnimationFrame >= 0.0f)
		{
			mpSlimeHitSE->Play(3.0f);
			mIsSlimeHitSE = true;
		}
		if (mAnimationFrame >= 15.0f)
		{
			mpSlimeDieSE->Play(5.0f);
			mIsSlimeDieSE = true;
			mStateDieStep++;
		}
		break;
		// ステップ2 : 死亡モーション終了待ち
	case 2:
		if (IsAnimationFinished())
		{
			Kill();
			// エネミーの死亡処理
			CEnemy::SlimeDeath();
		}
		break;
	}
}

// めまい(混乱)
void CSlime::UpdateDizzy()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateDizzyStep)
	{
		// ステップ0 :めまいアニメーション開始
	case 0:
		// めまいアニメーションを開始
		ChangeAnimation(EAnimType::eDizzy);
		mStateDizzyStep++;
		break;
		// ステップ1　: 効果音開始	
	case 1:
		if (mAnimationFrame >= 0.0f)
		{
			mpSlimeDizzySE->Play();
			mIsSlimeDizzySE = true;
			mStateDizzyStep++;
		}
		break;
		// ステップ2 : めまいモーション終了待ち
	case 2:
		if (IsAnimationFinished())
		{
			// プレイヤーの攻撃がヒットした時の待機状態へ移行
			ChangeState(EState::eIdle3);
			ChangeAnimation(EAnimType::eIdle4);
		}
		break;
	}
}

// 走る
void CSlime::UpdateRun() 
{
	SetAnimationSpeed(0.6f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();
	
	// 範囲内の時、移動し追跡する
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE && player->Position().Y() <= Position().Y())
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
		if (vectorp <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	// 追跡が止まった時、攻撃用の待機モーションへ
	else if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		SetAnimationSpeed(0.8f);
		ChangeState(EState::eIdle3);
		ChangeAnimation(EAnimType::eIdle4);
	}
	if (mAnimationFrame >= 5.0f)
	{
		mpSlimeRunSE->Play();
		mIsSlimeRunSE = true;
	}
}



// 更新処理
void CSlime::Update()
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
	CVector gaugePos = Position() + CVector(0.0f, 30.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	// HPが減ったら攻撃開始
	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		mpHpGauge->SetWorldPos(gaugePos);

		(mAttackTime++ / 100);

		if (mAttackTime > 230)
		{
			// 大攻撃
			bool BigAttack = false;
			// 確率を最小に8最大10
			int probability2 = Math::Rand(8, 11);
			if (probability2 == 8)BigAttack = true;

			if (BigAttack)
			{
				ChangeState(EState::eAttack2);
			}
			else
			{
				ChangeState(EState::eAttack);
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
		CPlayer* player = CPlayer::Instance();
		float vectorp = (player->Position() - Position()).Length();
		if (vectorp >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
	}

	if (mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eIdleWait)
	{
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

	mpColliderSphereBody->Update();
	mpDamageColBody->Update();
	mpAttackColBody->Update();

	mIsGrounded = false;
	CDebugPrint::Print("HP %d\n", mCharaStatus.hp);

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CSlime::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
    // 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColBody && mState != EState::eIdle && mState != EState::eIdle2 &&
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
				int damage = CalcDamage(this,chara);

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
	else if (self == mpColliderSphereBody)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CSlime::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackColBody->SetEnable(true);
}

// 攻撃終了
void CSlime::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColBody->SetEnable(false);
}

// 描画
void CSlime::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CSlime::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CSlime::ChangeLevel(int level)
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
void CSlime::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		ChangeState(EState::eHit);
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
void CSlime::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}