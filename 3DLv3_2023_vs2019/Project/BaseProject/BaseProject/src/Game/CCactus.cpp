#include "CCactus.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"
#include "CCrackEffect.h"

// サボテンのインスタンス
CCactus* CCactus::spInstance = nullptr;

#define ENEMY_HEIGHT 0.3f     // 線分コライダー
#define WITHIN_RANGE 40.0f    // 範囲内
#define MOVE_SPEED 0.12f      // 移動速度
#define GRAVITY 0.0625f       // 重力
#define WALK_RANGE 100.0f     // 追跡する範囲
#define STOP_RANGE 24.0f      // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f  // 回転する範囲

// サボテンのアニメーションデータのテーブル
const CCactus::AnimData CCactus::ANIM_DATA[] =
{
	{ "",										                        true,  0.0f,   0.0f},  // Tポーズ
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdlePlant.x",	        true,  21.0f,  0.5f},  // 植物 21.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdlePlantToBattle.x",	true,  21.0f,  0.3f},  // 植物2 21.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdleBattle.x",	    true,  18.0f,  0.4f},  // 待機 18.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdleNormal.x",	    true,  41.0f,  0.5f},  // 待機2 41.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusAttack.x",	        true,  21.0f,  0.5f},  // 攻撃 21.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusAttack2.x",	        true,  26.0f,  0.5f},  // 攻撃2 26.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusGetHit.x",	        true,  23.0f,  0.3f},  // ヒット 23.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusDie.x",	            true,  23.0f,  0.2f},  // 死ぬ 23.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusDizzy.x",          	true,  41.0f,  0.5f},  // めまい 41.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusRun.x",	            true,  17.0f,  0.4f},  // 走る 17.0f
	//{ "Character\\Enemy\\Cactus\\animation\\CactusIdle.x",	true,	121.0f	},	 // 始まりの待機 121.0f
	//{ "Character\\Enemy\\Cactus\\animation\\CactusIdle2.x",	true,	46.0f	},	 // 始まりの待機2 23.0f
};

// コンストラクタ
CCactus::CCactus()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mStateAttackStep(0)
	, mStateAttack2Step(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Cactus");

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

	SetAnimationSpeed(0.5f);
	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	// キャラクターの線分コライダー
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
		this, ELayer::eEnemy,0.27f, false, 1.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.0f, 0.0f, -0.02f);

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,0.3f, false, 1.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.0f, -0.055f, 0.0f);

	// キャラクター押し戻し処理(足元)
	mpColliderSphereFeet = new CColliderSphere
	(
		this, ELayer::eEnemy,0.3f, false, 3.0f
	);
	mpColliderSphereFeet->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(左手)
	mpColliderSphereLeftHand = new CColliderSphere
	(
		this, ELayer::eEnemy,0.16f, false, 1.0f
	);
	mpColliderSphereLeftHand->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereLeftHand->Position(-0.12f, 0.01f, 0.0f);

	// キャラクター押し戻し処理(右手)
	mpColliderSphereRightHand = new CColliderSphere
	(
		this, ELayer::eEnemy,0.16f, false, 1.0f
	);
	mpColliderSphereRightHand->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy, ELayer::eField });
	mpColliderSphereRightHand->Position(0.12f, 0.01f, 0.0f);

	// ダメージを受けるコライダーを作成(頭)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.27f, false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon });
	// ダメージを受けるコライダーを少し上へずらす
	mpDamageColHead->Position(0.0f, 0.0f, -0.02f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.3f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, -0.055f, 0.0f);

	// ダメージを受けるコライダーを作成(足元)
	mpDamageColFeet = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.3f, false
	);
	mpDamageColFeet->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(左手)
	mpDamageColLeftHand = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.16f, false
	);
	mpDamageColLeftHand->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColLeftHand->SetCollisionTags({ ETag::eWeapon });
	mpDamageColLeftHand->Position(-0.12f, 0.01f, 0.0f);

	// ダメージを受けるコライダーを作成(右手)
	mpDamageColRightHand = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.16f, false
	);
	mpDamageColRightHand->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColRightHand->SetCollisionTags({ ETag::eWeapon });
	mpDamageColRightHand->Position(0.12f, 0.01f, 0.0f);

	// ダメージを与えるコライダー(頭)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.27f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.0f, 0.0f, -0.02f);

	// ダメージを与えるコライダー(左手)
	mpAttackColLeftHand = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.16f, false
	);
	mpAttackColLeftHand->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColLeftHand->SetCollisionTags({ ETag::ePlayer });
	mpAttackColLeftHand->Position(-0.12f, 0.01f, 0.0f);

	// 押し戻しコライダーとダメージを受けるコライダーをサボテンの体の行列にアタッチ
	const CMatrix* hitBodyMty = GetFrameMtx("Armature_cactus_spine03");
	mpColliderSphereBody->SetAttachMtx(hitBodyMty);
	mpDamageColBody->SetAttachMtx(hitBodyMty);

	// 押し戻しコライダーとダメージを受けるコライダーをサボテンの足元の行列にアタッチ
	const CMatrix* hitFeetMty = GetFrameMtx("Armature_cactus_spine01");
	mpColliderSphereFeet->SetAttachMtx(hitFeetMty);
	mpDamageColFeet->SetAttachMtx(hitFeetMty);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーをサボテンの頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_cactus_headDeco");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpDamageColHead->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーをサボテンの左手の行列にアタッチ
	const CMatrix* leftHandMty = GetFrameMtx("Armature_cactus_leftHandTip");
	mpColliderSphereLeftHand->SetAttachMtx(leftHandMty);
	mpDamageColLeftHand->SetAttachMtx(leftHandMty);
	mpAttackColLeftHand->SetAttachMtx(leftHandMty);

	// 押し戻しコライダーとダメージを受けるコライダーをサボテンの右手の行列にアタッチ
	const CMatrix* rightHandMty = GetFrameMtx("Armature_cactus_rightHandTip");
	mpColliderSphereRightHand->SetAttachMtx(rightHandMty);
	mpDamageColRightHand->SetAttachMtx(rightHandMty);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColHead->SetEnable(false);
	mpAttackColLeftHand->SetEnable(false);

	// ひび割れエフェクト作成
	mpCrack = new CCrackEffect
	(
		this, leftHandMty,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0, -90.f, 0.0f).Matrix()
	);
}

// デストラクタ
CCactus::~CCactus()
{
	// キャラクターの線分コライダー
	SAFE_DELETE(mpColliderLine);
	// キャラクターの押し戻しコライダー
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereFeet);
	SAFE_DELETE(mpColliderSphereLeftHand);
	SAFE_DELETE(mpColliderSphereRightHand);
	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColFeet);
	SAFE_DELETE(mpDamageColLeftHand);
	SAFE_DELETE(mpDamageColRightHand);
	// 攻撃コライダー
	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColLeftHand);
}

// インスタンス
CCactus* CCactus::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CCactus::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 状態の切り替え
void CCactus::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
	mStateAttack2Step = 0;
}

// 戦う前の待機状態
void CCactus::UpdateIdle()
{
	SetAnimationSpeed(0.5f);
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

// 戦う前の待機状態2
void CCactus::UpdateIdle2()
{
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// 待機状態3
void CCactus::UpdateIdle3()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE && player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 33.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle3);
	}
}

// 攻撃
void CCactus::UpdateAttack()
{
	SetAnimationSpeed(0.5f);
	mpAttackColHead->SetEnable(false);
	if (!mpCrack->IsThrowing())
	{
		mpCrack->Start();
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
		if (mAnimationFrame >= 5.0f && mAnimationFrame < 15.0f)
		{
			AttackStart();
			mStateAttackStep++;
		}
		break;
		// ステップ2 : 攻撃終了
	case 2:
		if (mAnimationFrame >= 15.0f)
		{
			AttackEnd();
			mStateAttackStep++;
		}
		break;
		// ステップ3 : 攻撃終了待ち＋ひび割れを辞める
	case 3:
		if (mAnimationFrame >= 19.0f)
		{
			mpCrack->Stop();
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃2
void CCactus::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);
	mpAttackColLeftHand->SetEnable(false);

	// ステップごとに処理を分ける
	switch (mStateAttack2Step)
	{
		// ステップ0 : 攻撃アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		mStateAttack2Step++;
		break;
	case 1:
		if (mAnimationFrame >= 10.0f)
		{
			AttackStart();
			mStateAttack2Step++;
		}
		break;
	case 2:
		if (mAnimationFrame >= 24.0f)
		{
			// 攻撃2終了待ち状態へ移行
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃終了待ち
void CCactus::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpCrack->Stop();
		ChangeState(EState::eIdle3);
	}
}

// ヒット
void CCactus::UpdateHit()
{
	mpCrack->Stop();
	SetAnimationSpeed(0.3f);
	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);

	// めまいをfalseにする
	bool stan = false;
	if (GetAnimationFrameRatio() >= 0.8f)
	{
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
}

// 死ぬ
void CCactus::UpdateDie()
{
	mpCrack->Stop();
	SetAnimationSpeed(0.2f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::CactusDeath();
	}
}

// めまい(混乱)
void CCactus::UpdateDizzy()
{
	mpCrack->Stop();
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		ChangeState(EState::eIdle3);
	}
}

// 移動
void CCactus::UpdateRun()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// 範囲内の時、移動し追跡する
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
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
	if (vectorPos <= 33.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle3);
	}
	// 追跡が止まった時、待機モーションへ
	else if (vectorPos <= STOP_RANGE || vectorPos > WALK_RANGE)
	{
		ChangeState(EState::eIdle3);
	}
}

// 更新処理
void CCactus::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.Y(0.0f);

	if (mState != EState::eRun)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
	}

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
	CVector gaugePos = Position() + CVector(0.0f, 37.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (mState != EState::eIdle && mState != EState::eIdle2)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle3 || mState == EState::eRun || mState == EState::eHit)
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
			// 確率を最小に3最大6
			int probability2 = Math::Rand(2, 5);
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
		if (mState == EState::eAttack || mState == EState::eAttack2)
		{
			mAttackTime = 0;
		}
	}

	if (mState == EState::eRun)
	{
		if (vectorp > STOP_RANGE && vectorp <= WALK_RANGE)
		{
			Position(Position() + mMoveSpeed * MOVE_SPEED);
		}
	}

	if (Position().Y() >= 0.1f)
	{
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	}

	// キャラクターの更新
	CXCharacter::Update();

	// キャラクターの押し戻しコライダー
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereFeet->Update();
	mpColliderSphereLeftHand->Update();
	mpColliderSphereRightHand->Update();
	// ダメージを受けるコライダー
	mpDamageColHead->Update();
	mpDamageColBody->Update();
	mpDamageColFeet->Update();
	mpDamageColLeftHand->Update();
	mpDamageColRightHand->Update();
	// 攻撃コライダー
	mpAttackColHead->Update();
	mpAttackColLeftHand->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CCactus::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColLeftHand && mState != EState::eIdle
		&& mState != EState::eIdle2 && mState != EState::eIdle3)
	{
		// キャラクターのポインタに変換
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
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColHead && mState != EState::eIdle
		&& mState != EState::eIdle2 && mState != EState::eIdle3)
	{
		// キャラクターのポインタに変換
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// 相手のコライダーの持ち主がキャラであれば、
		if (chara != nullptr)
		{
			// 既に攻撃済みのキャラでなければ
			if (!IsAttackHitObj(chara))
			{
				int damage = CalcDamage(this, chara);
				damage = damage * 2;

				// ダメージを与える
				chara->TakeDamage(damage, this);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
	else if (self == mpColliderLine || self == mpColliderSphereRightHand)
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
	else if (self == mpColliderSphereHead || self == mpColliderSphereBody || self == mpColliderSphereFeet
		|| self == mpColliderSphereLeftHand || self == mpColliderSphereRightHand)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CCactus::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	if (mState == EState::eAttack2)
	{
		mpAttackColHead->SetEnable(true);
	}
	if (mState == EState::eAttack)
	{
		mpAttackColLeftHand->SetEnable(true);
	}
}

// 攻撃終了
void CCactus::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColHead->SetEnable(false);
	mpAttackColLeftHand->SetEnable(false);
}

// 描画
void CCactus::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CCactus::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CCactus::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY6_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CCactus::TakeDamage(int damage, CObjectBase* causedObj)
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
		Position(Position() - dir * Scale().X() * 0.2f);
	}
}

// 死亡処理
void CCactus::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}