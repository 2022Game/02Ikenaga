#include "CSlime2.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "CGameEnemyUI.h"
#include "CHit.h"
#include "Maths.h"

// オレンジスライムのインスタンス
CSlime2* CSlime2::spInstance = nullptr;

#define ENEMY_HEIGHT   1.0f  // 線分コライダー
#define MOVE_SPEED     0.4f  // 移動速度
#define WITHIN_RANGE  30.0f  // 範囲内
#define GRAVITY        0.3f  // 重力
#define WALK_RANGE   100.0f  // 追跡する範囲
#define STOP_RANGE    21.0f  // 追跡を辞める範囲
#define ROTATE_RANGE 250.0f  // 回転する範囲

// オレンジスライム(エネミー)のアニメーションデータのテーブル
const CSlime2::AnimData CSlime2::ANIM_DATA[] =
{
	{ "",										                         true,	0.0f,	0.0f},	// Tポーズ
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleNormal.x",	         true,	51.0f,	0.7f},  // アイドル通常
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingStart.x",	 true,	63.0f,  0.5f},  // 開始の見回す
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingRoutine.x",true,	71.0f,  0.5f},  // 見回す
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleBattle.x",	         true,	25.0f,	0.8f},  // アイドルバトル
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack.x",	             false,	26.0f,	0.5f},  // 攻撃
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack2.x",	             false,	26.0f,	0.5f},  // 攻撃2
	{ "Character\\Enemy\\Slime\\animation\\SlimeGetHit.x",	             false,	26.0f,	0.5f},  // ヒット
	{ "Character\\Enemy\\Slime\\animation\\SlimeDie.x",	                 false,	41.0f,	0.5f},  // 死ぬ
	{ "Character\\Enemy\\Slime\\animation\\SlimeDizzy.x",	             false,	41.0f,	0.5f},  // めまい
	{ "Character\\Enemy\\Slime\\animation\\SlimeRun.x",	                 true,	21.0f,	0.5f},  // 移動
	{ "Character\\Enemy\\Slime\\animation\\SlimeWalk.x",	             true,	31.0f,	0.5f},  // 歩く
};

// コンストラクタ
CSlime2::CSlime2()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mIsGrounded(false)
	, mStateStep(0)
	, mIsSlimeAttackSE(false)
	, mIsSlimeDizzySE(false)
	, mIsSlimeHitSE(false)
	, mIsSlimeDieSE(false)
	, mMoveSpeed(CVector::zero)
{
	// インスタンスの設定
	spInstance = this;

	// 敵の種類
	mType = EEnemyType::eSlime2;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Slime2");

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

	// キャラクターの線分コライダー
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
		this, ELayer::eEnemy,
		0.5f, false, 5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.06f, 0.3f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.5f, false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon,ETag::eSlash });
	//ダメージを受けるコライダーを少し上へずらす
	mpDamageColBody->Position(0.1f, 0.3f, 0.0f);

	// ダメージを与えるコライダー(体)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.5f, false
	);
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });
	mpAttackColBody->Position(0.1f, 0.3f, 0.0f);

	// キャラクター押し戻しコライダーと
	// ダメージを受けるコライダーと攻撃コライダーをスライムの体の行列にアタッチ
	const CMatrix* bodyMtx = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMtx);
	mpDamageColBody->SetAttachMtx(bodyMtx);
	mpAttackColBody->SetAttachMtx(bodyMtx);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColBody->SetEnable(false);

	float Size = 17.0f;   // サイズ
	float Height = 0.6f;  // 高さ
	// ヒットエフェクトを作成
	mpHitEffect = new CHit(Size, Height);
	mpHitEffect->SetOwner(this);
	mpHitEffect->Position(Position());
	mpHitEffect->SetShow(false);

	mpGameUI->SetUIoffSetPos(CVector(0.0f, 30.0f, 0.0f));

	// Lv.を設定
	mpGameUI->SetLv();
	// レベルを設定
	std::string level = "1";
	mpGameUI->SetEnemyLevel(level);
	// 名前を設定
	std::string name = "スライム";
	mpGameUI->SetEnemyName(name);

	mpSlimeAttackSE = CResourceManager::Get<CSound>("SlimeAttack");
	mpSlimeDizzySE = CResourceManager::Get<CSound>("SlimeDizzy");
	mpSlimeHitSE = CResourceManager::Get<CSound>("SlimeHit");
	mpSlimeDieSE = CResourceManager::Get<CSound>("SlimeDie");

	Scale(25.0f, 25.0f, 25.0f);
}

// デストラクタ
CSlime2::~CSlime2()
{
	// キャラクターの線分コライダー
	SAFE_DELETE(mpColliderLine);
	// キャラクターの押し戻しコライダーを削除
	SAFE_DELETE(mpColliderSphereBody);
	// ダメージを受けるコライダーを削除
	SAFE_DELETE(mpDamageColBody);
	// 攻撃コライダーを削除
	SAFE_DELETE(mpAttackColBody);
}

// スライムのインスタンス
CSlime2* CSlime2::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CSlime2::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 状態の切り替え
void CSlime2::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// 待機
void CSlime2::UpdateIdle()
{
	SetAnimationSpeed(0.7f);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// 待機2
void CSlime2::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeAnimation(EAnimType::eIdle3);
		ChangeState(EState::eIdleWait);
	}
}

// ダメージを受けた時の待機3
void CSlime2::UpdateIdle3()
{
	SetAnimationSpeed(0.8f);
	ChangeAnimation(EAnimType::eIdle4);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >= 10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE
		&& player->Position().Y() < 0.5f)
	{
		ChangeState(EState::eRun);
	}
	else
	{
		ChangeState(EState::eIdle3);
	}
}

// 待機2の終了待ち
void CSlime2::UpdateIdleWait()
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
void CSlime2::UpdateAttack()
{
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		AttackStart();
		mStateStep++;
		break;
		// ステップ1　: 効果音開始	
	case 1:
		if (mAnimationFrame >= 1.0f)
		{
			mpSlimeAttackSE->Play();
			mIsSlimeAttackSE = true;
			mStateStep++;
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
void CSlime2::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃2アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		if (mAnimationFrame >= 10.0f)
		{
			AttackStart();
			mpColliderSphereBody->SetEnable(false);
			mStateStep++;
		}
		break;
		// ステップ1　: 効果音開始	
	case 1:
		if (mAnimationFrame >= 1.0f)
		{
			mpSlimeAttackSE->Play();
			mIsSlimeAttackSE = true;
			mStateStep++;
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
void CSlime2::UpdateAttackWait()
{
	// 攻撃と攻撃2アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpColliderSphereBody->SetEnable(true);
		CPlayer* player = CPlayer::Instance();
		float vectorPos = (player->Position() - Position()).Length();
		if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
		{
			ChangeState(EState::eRun);
		}
		else
		{
			ChangeState(EState::eIdle3);
		}
	}
}

// ヒット
void CSlime2::UpdateHit()
{
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 :ヒットアニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eHit);
		mStateStep++;
		break;
		// ステップ1　: 効果音開始	
	case 1:
		if (mAnimationFrame >= 0.0f)
		{
			mpSlimeHitSE->Play(3.0f);
			//mpHitEffect->StartHitEffect();
			mIsSlimeHitSE = true;
			mStateStep++;
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
				ChangeState(EState::eIdle3);
			}
		}
		break;
	}
}

// 死ぬ時
void CSlime2::UpdateDie()
{
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 :死亡アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eDie);
		mStateStep++;
		break;
		// ステップ1　: 効果音開始	
	case 1:
		if (mAnimationFrame >= 0.0f)
		{
			mpSlimeHitSE->Play(2.0f);
			mIsSlimeHitSE = true;
		}
		if (mAnimationFrame >= 15.0f)
		{
			mpSlimeDieSE->Play(5.0f);
			mIsSlimeDieSE = true;
			mStateStep++;
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
void CSlime2::UpdateDizzy()
{
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 :めまいアニメーション開始
	case 0:
		// めまいアニメーションを開始
		ChangeAnimation(EAnimType::eDizzy);
		mStateStep++;
		break;
		// ステップ1　: 効果音開始	
	case 1:
		if (mAnimationFrame >= 0.0f)
		{
			mpSlimeDizzySE->Play();
			mIsSlimeDizzySE = true;
			mStateStep++;
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

// 移動
void CSlime2::UpdateRun()
{
	SetAnimationSpeed(0.6f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// 範囲内の時、移動し追跡する
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += newPos * MOVE_SPEED;
		if (vectorPos <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	// 追跡が止まった時、攻撃用の待機モーションへ
	else if (vectorPos <= STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		ChangeState(EState::eIdle3);
	}
	if (vectorPos <= 25.0f && player->Position().Y() >= 6.0f)
	{
		ChangeState(EState::eIdle3);
	}
}



// 更新処理
void CSlime2::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(-GRAVITY);
	mMoveSpeed.Z(0.0f);

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機
	case EState::eIdle:
		UpdateIdle();
		break;
		// 待機2
	case EState::eIdle2:
		UpdateIdle2();
		break;
		// 待機3
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
		// 移動
	case EState::eRun:
		UpdateRun();
		break;
	}

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eIdleWait)
	{
		if (vectorPos <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	if (mState == EState::eRun)
	{
		if (vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
		{
			Position(Position() + mMoveSpeed);
		}
	}

	// キャラクターの更新
	CXCharacter::Update();

	// キャラクターの押し戻しコライダー
	mpColliderSphereBody->Update();
	// ダメージを受けるコライダー
	mpDamageColBody->Update();
	// 攻撃コライダー
	mpAttackColBody->Update();

	mIsGrounded = false;

	CEnemy::Update();

	// HPが減ったら攻撃開始
	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		if (vectorPos <= WITHIN_RANGE)
		{
			mAttackTime++;
		}

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
		if (vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
	}
	else
	{
		CHpGauge* hpGauge = mpGameUI->GetHpGauge();
		hpGauge->SetShow(false);
		CLevelUI* Lv = mpGameUI->GetLv();
		Lv->SetShow(false);
		CEnemyLevelUI* Level = mpGameUI->GetLevel();
		Level->SetShow(false);
		CEnemyNameUI* Name = mpGameUI->GetName();
		Name->SetShow(false);
	}
}

// 衝突処理
void CSlime2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
				int damage = CalcDamage(1.0f, this, chara);

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
void CSlime2::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackColBody->SetEnable(true);
}

// 攻撃終了
void CSlime2::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColBody->SetEnable(false);
}
// 1レベルアップ
void CSlime2::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CSlime2::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpGameUI->SetMaxHp(mCharaMaxStatus.hp);
	mpGameUI->SetHp(mCharaStatus.hp);
}

// 被ダメージ処理
void CSlime2::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		if (mState != EState::eDie)
		{
			mpHitEffect->StartHitEffect();
		}
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
void CSlime2::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}

// ランダムに位置を取得
CVector CSlime2::GetRandomSpawnPos()
{
	CVector pos = CVector::zero;
	pos.X(Math::Rand(-100.0f, 0.0f));
	pos.Z(Math::Rand(-140.0f, 0.0f));

	return CVector(pos);
}

// 描画
void CSlime2::Render()
{
	CXCharacter::Render();
}