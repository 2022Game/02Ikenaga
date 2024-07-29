#include "CChest.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "CCoin.h"
#include "CGameEnemyUI.h"
#include "CHit.h"
#include "Maths.h"

// チェストモンスターのインスタンス
CChest* CChest::spInstance = nullptr;

#define ENEMY_HEIGHT    0.3f  // 線分コライダー
#define WITHIN_RANGE   30.0f  // 範囲内
#define ATTACK_RANGE   50.0f  // 攻撃の範囲内
#define MOVE_SPEED     0.64f  // 移動速度
#define GRAVITY      0.0625f  // 重力
#define WALK_RANGE    150.0f  // 追跡する範囲
#define STOP_RANGE     32.0f  // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f  // 回転する範囲

// チェストモンスターのアニメーションデータのテーブル
const CChest::AnimData CChest::ANIM_DATA[] =
{
	{ "",										            true,	0.0f,	 0.0f},  // Tポーズ
	{ "Character\\Enemy\\Chest\\animation\\ChestIdle.x",	true,	21.0f,	 1.0f},	 // 待機 21.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestIdle2.x",	true,	23.0f,	 0.5f},	 // 待機2 23.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestIdle3.x",	true,	41.0f,	 0.5f},	 // 待機3 41.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestAttack.x",	false,	25.0f,	 0.5f},	 // 攻撃 25.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestAttack2.x",	false,	23.0f,	 0.5f},	 // 攻撃2 23.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestGetHit.x",	false,	19.0f,	 0.4f},	 // ヒット 19.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestDie.x",	    false,	29.0f,	0.25f},  // 死ぬ 29.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestDizzy.x",	false,	41.0f,	 0.5f},	 // めまい 41.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestRun.x",	    true,	17.0f,	 0.4f},	 // 走る 17.0f
};

// コンストラクタ
CChest::CChest()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mStateAttackStep(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
{
	//インスタンスの設定
	spInstance = this;

	// 敵の種類
	mType = EEnemyType::eChest;

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

	SetAnimationSpeed(1.0f);
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
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.55f, false, 5.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.27f, 0.0f, 0.0f);

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.55f, false, 5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.05f, 0.0f, 0.0f);

	// キャラクター押し戻し処理(前の左足)
	mpColliderSphereFeet = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 5.0f
	);
	mpColliderSphereFeet->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeet->Position(0.0f, 0.1f, 0.0f);

	// キャラクター押し戻し処理(前の右足)
	mpColliderSphereFeet2 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 5.0f
	);
	mpColliderSphereFeet2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeet2->Position(0.0f, 0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(頭)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.55f, false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon });
	// ダメージを受けるコライダーを少し上へずらす
	mpDamageColHead->Position(0.27f, 0.0f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.55f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.05f, 0.0f, 0.0f);

	// ダメージを受けるコライダーを作成(前の左足)
	mpDamageColFeet = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.2f, 0.0f),
		3.0f, false
	);
	mpDamageColFeet->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(前の右足)
	mpDamageColFeet2 = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.2f, 0.0f),
		3.0f, false
	);
	mpDamageColFeet2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet2->SetCollisionTags({ ETag::eWeapon });

	// ダメージを与えるコライダー(頭)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.55f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.27f, 0.0f, 0.0f);

	// 押し戻しコライダーとダメージを受けるコライダーと
	// 攻撃コライダーをチェストモンスターの頭の行列にアタッチ
	const CMatrix* headMtx = GetFrameMtx("Armature_Head");
	mpColliderSphereHead->SetAttachMtx(headMtx);
	mpDamageColHead->SetAttachMtx(headMtx);
	mpAttackColHead->SetAttachMtx(headMtx);

	// 押し戻しコライダーと
	// ダメージを受けるコライダーをチェストモンスターの体の行列にアタッチ
	const CMatrix* bodyMtx = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMtx);
	mpDamageColBody->SetAttachMtx(bodyMtx);

	// 押し戻しコライダーと
	// ダメージを受けるコライダーをチェストモンスターの前の左足の行列にアタッチ
	const CMatrix* leftFeetMtx = GetFrameMtx("Armature_FrontLeftLeg02");
	mpColliderSphereFeet->SetAttachMtx(leftFeetMtx);
	mpDamageColFeet->SetAttachMtx(leftFeetMtx);

	// 押し戻しコライダーと
	// ダメージを受けるコライダーをチェストモンスターの前の右足の行列にアタッチ
	const CMatrix* rightFeetMtx = GetFrameMtx("Armature_FrontRightLeg02");
	mpColliderSphereFeet2->SetAttachMtx(rightFeetMtx);
	mpDamageColFeet2->SetAttachMtx(rightFeetMtx);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColHead->SetEnable(false);

	float Size = 25.0f;   // サイズ
	float Height = 0.7f;  // 高さ
	// ヒットエフェクトを作成
	mpHitEffect = new CHit(Size, Height);
	mpHitEffect->SetOwner(this);
	mpHitEffect->Position(Position());
	mpHitEffect->SetShow(false);

	mpGameUI->SetUIoffSetPos(CVector(0.0f, 40.0f, 0.0f));

	// Lv.を設定
	mpGameUI->SetLv();
	// レベルを設定
	int level = 51;
	std::to_string(level);
	mpGameUI->SetEnemyLevel(level);

	Scale(30.0f, 30.0f, 30.0f);
}

// デストラクタ
CChest::~CChest()
{
	// 線分コライダー
	SAFE_DELETE(mpColliderLine);
	// キャラクターの押し戻しコライダー
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereFeet);
	SAFE_DELETE(mpColliderSphereFeet2);
	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColFeet);
	SAFE_DELETE(mpDamageColFeet2);
	// 攻撃コライダー
	SAFE_DELETE(mpAttackColHead);
}

// インスタンス
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

// 状態の切り替え
void CChest::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
}

// 戦う前の待機状態
void CChest::UpdateIdle()
{
	SetAnimationSpeed(1.0f);
	ChangeAnimation(EAnimType::eIdle);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= WITHIN_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
	else
	{
		ChangeState(EState::eIdle);
	}
}

// 待機状態2
void CChest::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >= 10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE
		&& player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 28.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
}

// 待機状態3
void CChest::UpdateIdle3()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// 攻撃
void CChest::UpdateAttack()
{
	SetAnimationSpeed(0.5f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mAnimationFrame <= 30.0f)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = CPlayer::Instance()->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	// ステップごとに処理を分ける
	switch (mStateAttackStep)
	{
		// ステップ0 : 攻撃アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		mStateAttackStep++;
		break;
		// ステップ1 : 攻撃開始
	case 1:
		if (mAnimationFrame >= 5.0f && mAnimationFrame < 10.0f)
		{
			AttackStart();
			mStateAttackStep++;
		}
		break;
		// ステップ2 : 攻撃終了＆プレイヤーとの距離が45.0f以上の時、コイン生成
	case 2:
		if (mAnimationFrame >= 10.0f)
		{
			AttackEnd();
			if (vectorPos >= ATTACK_RANGE)
			{
				// コインを生成済みフラグを初期化
				mIsSpawnedCoinEffect = false;

				CVector forward = VectorZ();
				forward.Y(0.0f);
				forward.Normalize();
				CVector coinPos = Position() + forward * 15.0f + CVector(0.0f, 18.0f, 0.0f);

				// コインを生成していない
				if (!mIsSpawnedCoinEffect)
				{
					CCoin* coin = new CCoin
					(
						this,
						coinPos,
						VectorZ(),
						180.0f,
						150.0f
					);
					coin->SetColor(CColor(1.0f, 1.0f, 0.0f));
					coin->Scale(10.0f, 10.0f, 10.0f);
					coin->Rotate(-90.0f, 0.0f, 0.0f);
					coin->SetOwner(this);

					mIsSpawnedCoinEffect = true;
					mStateAttackStep++;
				}
			}
			else
			{
				mStateAttackStep++;
			}
		}
		break;
		// ステップ3 : 攻撃終了待ち
	case 3:
		if (mAnimationFrame >= 23.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃2
void CChest::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	ChangeState(EState::eAttackWait);
}

// 攻撃終了待ち
void CChest::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle2);
	}
}

// ヒット
void CChest::UpdateHit()
{
	SetAnimationSpeed(0.4f);
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

// 死ぬ
void CChest::UpdateDie()
{
	SetAnimationSpeed(0.25f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::ChestDeath();
	}
}

// めまい(混乱)
void CChest::UpdateDizzy()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		ChangeState(EState::eIdle2);
	}
}

// 移動
void CChest::UpdateRun()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// 範囲内の時、移動し追跡する
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += newPos * MOVE_SPEED;
		// 回転する範囲であれば
		if (vectorPos <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}

	if (vectorPos <= 36.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
	// 追跡が止まった時、待機モーションへ
	else if (vectorPos <= STOP_RANGE || vectorPos > WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// 更新処理
void CChest::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
	mMoveSpeed.Z(0.0f);

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

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eHit)
	{
		if (vectorPos <= 70.0f)
		{
			mAttackTime++;
		}

		if (vectorPos <= ROTATE_RANGE)
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
			int probability2 = Math::Rand(0, 2);
			if (probability2 == 2)Attack2 = true;
			if (Attack2)
			{
				ChangeState(EState::eAttack2);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
	}

	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed);
	}

	if (Position().Y() >= 0.1f)
	{
		Position(Position().X(), Position().Y() - 1.0f, Position().Z());
	}

	// キャラクターの更新
	CXCharacter::Update();
	// キャラクターの押し戻しコライダー
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereFeet->Update();
	mpColliderSphereFeet2->Update();
	// ダメージを受けるコライダー
	mpDamageColHead->Update();
	mpDamageColBody->Update();
	mpDamageColFeet->Update();
	mpDamageColFeet2->Update();
	// 攻撃コライダー
	mpAttackColHead->Update();

	mIsGrounded = false;

	CEnemy::Update();

	if (mState == EState::eIdle)
	{
		CHpGauge* hpGauge = mpGameUI->GetHpGauge();
		hpGauge->SetShow(false);
		CLevelUI* Lv = mpGameUI->GetLv();
		Lv->SetShow(false);
		CEnemyLevelUI* Level = mpGameUI->GetLevel();
		Level->SetShow(false);
		mpGameUI->SetUIoffSetPos(CVector(0.0f, 30.0f, 0.0f));
		// 名前を設定
		std::string name = " 宝箱？";
		mpGameUI->SetEnemyName(name);
	}
	else
	{
		mpGameUI->SetUIoffSetPos(CVector(0.0f, 40.0f, 0.0f));
		// 名前を設定
		std::string name = " 箱モン";
		mpGameUI->SetEnemyName(name);
	}
}

// 衝突処理
void CChest::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColHead && mState != EState::eIdle && mState != EState::eIdle2 &&
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
				int damage = CalcDamage(1.0f,this, chara);

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
	else if (self == mpColliderSphereHead || self == mpColliderSphereBody
		|| self == mpColliderSphereFeet || self == mpColliderSphereFeet2)
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
	if (mState == EState::eAttack || mState == EState::eAttack2)
	{
		mpAttackColHead->SetEnable(true);
	}
}

// 攻撃終了
void CChest::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColHead->SetEnable(false);
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

	mpGameUI->SetMaxHp(mCharaMaxStatus.hp);
	mpGameUI->SetHp(mCharaStatus.hp);
}

// 被ダメージ処理
void CChest::TakeDamage(int damage, CObjectBase* causedObj)
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
		Position(Position() - dir * Scale().X() * 0.2f);
	}
}

// 死亡処理
void CChest::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}

// ランダムに位置を取得
CVector CChest::GetRandomSpawnPos()
{
	CVector pos = CVector::zero;
	pos.X(Math::Rand(-200.0f, 0.0f));
	pos.Z(Math::Rand(-950.0f, -800.0f));

	return CVector(pos);
}

// 描画
void CChest::Render()
{
	CXCharacter::Render();
}