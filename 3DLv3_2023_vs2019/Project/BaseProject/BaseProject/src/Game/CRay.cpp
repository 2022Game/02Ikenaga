#include "CRay.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CWave.h"
#include "Maths.h"

// エイのインスタンス
CRay* CRay::spInstance = nullptr;

#define ENEMY_HEIGHT    0.5f  // 線分コライダー
#define WITHIN_RANGE   40.0f  // 範囲内
#define MOVE_SPEED      0.1f  // 移動速度
#define GRAVITY        0.06f  // 重力
#define WALK_RANGE    100.0f  // 追跡する範囲
#define STOP_RANGE     28.0f  // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f  // 回転する範囲
#define THROW_INTERVAL 0.07f  // 波動の発射間隔時間

// エイのアニメーションデータのテーブル
const CRay::AnimData CRay::ANIM_DATA[] =
{
	{ "",										        true,	0.0f,	 0.0f},  // Tポーズ
	{ "Character\\Enemy\\Ray\\animation\\RayIdle.x",	true,	21.0f,	 0.5f},	 // 待機 21.0f
	{ "Character\\Enemy\\Ray\\animation\\RayAttack.x",	false,	17.0f,	 0.4f},  // 攻撃 17.0f
	{ "Character\\Enemy\\Ray\\animation\\RayGetHit.x",	false,	13.0f,	0.25f},	 // ヒット 13.0f
	{ "Character\\Enemy\\Ray\\animation\\RayDie.x",	    false,	20.0f,	0.15f},	 // 死ぬ 20.0f
	{ "Character\\Enemy\\Ray\\animation\\RayMoveBWD.x",	true,	21.0f,	 0.5f},	 // 移動 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveFWD.x",	true,	42.0f	 },	    // 移動2 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveLFT.x",	true,	42.0f	 },	    // 左移動 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveRGT.x",	true,	42.0f	 },	    // 右移動 21.0f
};

// コンストラクタ
CRay::CRay()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
	, mIsGrounded(false)
	, mMoveSpeed(CVector::zero)
	, mStateAttackStep(0)
	, mStateWave(0)
	, mIsSpawnedWaveEffect(false)
	, mElapsedWaveTime(0.0f)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Ray");

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
		this, ELayer::eEnemy,
		0.18f, false, 5.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eField });

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.2f, false, 5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eField });
	mpColliderSphereBody->Position(0.0f, 0.3f, 0.0f);

	// ダメージを受けるコライダーを作成(脊椎)
	mpDamageColSpine = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, -0.1f, 0.0f),
		CVector(0.0f, 0.32f, 0.0f),
		6.0f, false
	);
	//　ダメージを受けるコライダーと
	//　衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColSpine->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColSpine->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(ヒレの左)
	mpDamageColFinLeft = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.13f, false, 5.0f
	);
	mpDamageColFinLeft->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFinLeft->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFinLeft->Position(-0.05f, 0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(ヒレの右)
	mpDamageColFinRight = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.13f, false, 5.0f
	);
	mpDamageColFinRight->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFinRight->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFinRight->Position(-0.05f, 0.1f, 0.0f);

	// 攻撃コライダー(頭)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.2f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });

	// 攻撃コライダーをエイの頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// キャラの押し戻しコライダーをエイの体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);

	// ダメージを受けるコライダーをエイの脊椎の行列にアタッチ
	const CMatrix* spineMty = GetFrameMtx("Armature_Spine");
	mpDamageColSpine->SetAttachMtx(spineMty);

	// キャラの押し戻しコライダーと
	// ダメージを受けるコライダーをエイのヒレの左の行列にアタッチ
	const CMatrix* finLeftMty = GetFrameMtx("Armature_WingLeft01");
	mpDamageColFinLeft->SetAttachMtx(finLeftMty);

	// キャラの押し戻しコライダーと
	// ダメージを受けるコライダーをエイのヒレの右の行列にアタッチ
	const CMatrix* finRightMty = GetFrameMtx("Armature_WingRight01");
	mpDamageColFinRight->SetAttachMtx(finRightMty);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColHead->SetEnable(false);
}

// デストラクタ
CRay::~CRay()
{
	// キャラクターの線分コライダー
	SAFE_DELETE(mpColliderLine);
	// キャラクターの押し戻しコライダー
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColSpine);
	SAFE_DELETE(mpDamageColFinLeft);
	SAFE_DELETE(mpDamageColFinRight);
	// 攻撃コライダー
	SAFE_DELETE(mpAttackColHead);
}

CRay* CRay::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CRay::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 波動を作成
void CRay::CreateWave()
{
	// 波動エフェクトを生成して、正面方向へ飛ばす
	CWave* wave = new CWave
	(
		this,
		Position() + CVector(0.0f, 10.0f, 0.0f),
		VectorZ(),
		30.0f,
		80.0f
	);
	//wave->Scale(1.0f, 1.0f, 1.0f);
	// 波動エフェクトの色設定
	wave->SetColor(CColor(0.0f, 0.1f, 1.0f));
	wave->SetOwner(this);
}

// 状態の切り替え
void CRay::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
	mStateWave = 0;
}

// 待機状態
void CRay::UpdateIdle()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= WITHIN_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
	else if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// 待機状態2
void CRay::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos > STOP_RANGE && vectorPos < WALK_RANGE && player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 33.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
}

// 攻撃
void CRay::UpdateAttack()
{
	SetAnimationSpeed(0.4f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

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
		if (mAnimationFrame >= 5.0f)
		{
			AttackStart();
			mStateAttackStep++;
		}
		break;
		// ステップ2 : 攻撃終了
	case 2:
		if (mAnimationFrame >= 13.0f)
		{
			AttackEnd();
			mStateAttackStep++;
		}
		break;
		// ステップ3 : 攻撃終了待ち
	case 3:
		if (mAnimationFrame >= 15.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}

	mIsSpawnedWaveEffect = false;
	if (!mIsSpawnedWaveEffect && vectorPos >= 30.0f && mAnimationFrame <= 5.0f)
	{
		mIsSpawnedWaveEffect = true;
		if (mIsSpawnedWaveEffect)
		{
			mElapsedWaveTime += Time::DeltaTime();
			// 経過時間に応じて、波動のエフェクトを作成
			if (mElapsedWaveTime >= THROW_INTERVAL)
			{
				CreateWave();
				mElapsedWaveTime -= THROW_INTERVAL;
			}
		}
	}
}

// 攻撃終了待ち
void CRay::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle2);
	}
}

// ヒット
void CRay::UpdateHit()
{
	SetAnimationSpeed(0.25f);
	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		ChangeState(EState::eIdle2);
	}
}

// 死ぬ
void CRay::UpdateDie()
{
	SetAnimationSpeed(0.15f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::RayDeath();
	}
}

// 移動
void CRay::UpdateRun()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// 範囲内の時、移動し追跡する
	if (vectorPos > STOP_RANGE && vectorPos < WALK_RANGE)
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
	// 追跡が止まった時、待機モーションへ
	else if (vectorPos < STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// 更新処理
void CRay::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	if (mState != EState::eRun)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
	}

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
	CVector gaugePos = Position() + CVector(0.0f, 27.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState != EState::eIdle)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eHit)
	{
		mAttackTime++;

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
			ChangeState(EState::eAttack);
		}
		if (mState == EState::eAttack || mState == EState::eAttackWait)
		{
			mAttackTime = 0;
		}
		if (vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
	}

	if (vectorPos > STOP_RANGE && vectorPos < WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
	{
		mFlyingTime++;
		if (mFlyingTime < 100 && mFlyingTime >= 10)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + 0.04f);
		}
		else if (mFlyingTime > 100)
		{
			Position(Position().X(), Position().Y() - 0.5f, Position().Z());
		}
	}

	if (Position().Y() <= -0.4f)
	{
		mFlyingTime = 0;
	}

	if (mState == EState::eHit || mState ==EState::eDie)
	{
		Position(Position().X(), Position().Y() - 0.5f, Position().Z());
	}

	// キャラクターの更新
	CXCharacter::Update();

	// キャラクターの押し戻しコライダ-
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();

	// ダメージを受けるコライダー
	mpDamageColSpine->Update();
	mpDamageColFinLeft->Update();
	mpDamageColFinRight->Update();

	// 攻撃コライダー
	mpAttackColHead->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CRay::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColHead && mState != EState::eIdle && mState != EState::eIdle2)
	{
		// キャラクターのポインタに変換
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
	else if (self == mpColliderLine || self == mpColliderSphereHead || self == mpColliderSphereBody)
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
	else if ( self == mpColliderSphereHead || self == mpColliderSphereBody)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CRay::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	if (mState == EState::eAttack)
	{
		mpAttackColHead->SetEnable(true);
	}
}

// 攻撃終了
void CRay::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColHead->SetEnable(false);
}

// 描画
void CRay::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CRay::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CRay::ChangeLevel(int level)
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
void CRay::TakeDamage(int damage, CObjectBase* causedObj)
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
void CRay::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}