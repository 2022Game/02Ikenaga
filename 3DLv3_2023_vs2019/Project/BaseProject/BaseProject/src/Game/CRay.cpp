#include "CRay.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "CWave.h"
#include "CGameEnemyUI.h"
#include "CHit.h"
#include "Maths.h"

// エイのインスタンス
CRay* CRay::spInstance = nullptr;

#define ENEMY_SIDE     0.75f  // 線分コライダー(横)
#define ENEMY_HEIGHT    0.6f  // 線分コライダー(縦)
#define WITHIN_RANGE   40.0f  // 範囲内
#define MOVE_SPEED      0.4f  // 移動速度
#define WALK_RANGE    100.0f  // 追跡する範囲
#define STOP_RANGE     28.0f  // 追跡を辞める範囲
#define STOP_RANGE_Y   20.0f  // 追跡を辞める高さ
#define ROTATE_RANGE  250.0f  // 回転する範囲
#define THROW_INTERVAL 0.07f  // 波動の発射間隔時間

// エイのアニメーションデータのテーブル
const CRay::AnimData CRay::ANIM_DATA[] =
{
	{ "",										        true,	0.0f,	 0.0f},  // Tポーズ
	{ "Character\\Enemy\\Ray\\animation\\RayIdle.x",	true,	21.0f,	 0.5f},	 // 待機
	{ "Character\\Enemy\\Ray\\animation\\RayAttack.x",	false,	17.0f,	 0.4f},  // 攻撃
	{ "Character\\Enemy\\Ray\\animation\\RayGetHit.x",	false,	13.0f,	0.25f},	 // ヒット
	{ "Character\\Enemy\\Ray\\animation\\RayDie.x",	    false,	20.0f,	0.15f},	 // 死ぬ
	{ "Character\\Enemy\\Ray\\animation\\RayMoveBWD.x",	true,	21.0f,	 0.5f},	 // 移動
};

// コンストラクタ
CRay::CRay()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0.0f)
	, mFlyingTime(0.0f)
	, mIsGrounded(false)
	, mMoveSpeed(CVector::zero)
	, mStateStep(0)
	, mIsSpawnedWaveEffect(false)
	, mElapsedWaveTime(0.0f)
{
	//インスタンスの設定
	spInstance = this;

	//	敵の種類
	mType = EEnemyType::eRay;

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

	// 線分コライダー(横)
	mpColLineSide = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, ENEMY_SIDE)
	);
	mpColLineSide->SetCollisionLayers({ ELayer::eField });
	mpColLineSide->Position(0.0f, 10.0f, 0.0f);

	// 線分コライダー(縦)
	mpColLineHeight = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColLineHeight->SetCollisionLayers({ ELayer::eField });

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
	const CMatrix* headMtx = GetFrameMtx("Armature_Head");
	mpColliderSphereHead->SetAttachMtx(headMtx);
	mpAttackColHead->SetAttachMtx(headMtx);

	// キャラの押し戻しコライダーをエイの体の行列にアタッチ
	const CMatrix* bodyMtx = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMtx);

	// ダメージを受けるコライダーをエイの脊椎の行列にアタッチ
	const CMatrix* spineMtx = GetFrameMtx("Armature_Spine");
	mpDamageColSpine->SetAttachMtx(spineMtx);

	// キャラの押し戻しコライダーと
	// ダメージを受けるコライダーをエイのヒレの左の行列にアタッチ
	const CMatrix* finLeftMtx = GetFrameMtx("Armature_WingLeft01");
	mpDamageColFinLeft->SetAttachMtx(finLeftMtx);

	// キャラの押し戻しコライダーと
	// ダメージを受けるコライダーをエイのヒレの右の行列にアタッチ
	const CMatrix* finRightMtx = GetFrameMtx("Armature_WingRight01");
	mpDamageColFinRight->SetAttachMtx(finRightMtx);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColHead->SetEnable(false);

	float Size = 15.0f;   // サイズ
	float Height = 0.4f;  // 高さ
	// ヒットエフェクトを作成
	mpHitEffect = new CHit(Size, Height);
	mpHitEffect->SetOwner(this);
	mpHitEffect->Position(Position());
	mpHitEffect->SetShow(false);

	mpGameUI->SetUIoffSetPos(CVector(0.0f, 27.0f, 0.0f));

	// Lv.を設定
	mpGameUI->SetLv();
	// レベルを設定
	int level = 21;
	std::to_string(level);
	mpGameUI->SetEnemyLevel(level);
	// 名前を設定
	std::string name = "マンタ";
	mpGameUI->SetEnemyName(name);

	Scale(35.0f, 35.0f, 35.0f);
}

// デストラクタ
CRay::~CRay()
{
	// キャラクターの線分コライダー
	SAFE_DELETE(mpColLineSide);
	SAFE_DELETE(mpColLineHeight);
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

// インスタンス
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

// 波動エフェクトを作成
void CRay::CreateWave()
{
	CVector forward = VectorZ();
	forward.Y(0.0f);
	forward.Normalize();
	CVector wavePos = Position() + forward * 15.0f + CVector(0.0f, 10.0f, 0.0f);

	// 波動エフェクトを生成して、正面方向へ飛ばす
	CWave* wave = new CWave
	(
		this,
		wavePos,
		VectorZ(),
		30.0f,
		80.0f
	);
	// 波動エフェクトの色設定
	wave->SetColor(CColor(0.0f, 0.1f, 1.0f));
	wave->SetOwner(this);
}

// 状態の切り替え
void CRay::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
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
	if (GetAnimationFrame() >= 10.0f && vectorPos > STOP_RANGE && vectorPos < WALK_RANGE && player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 33.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
	else if (vectorPos > WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
		if (vectorPos <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
}

// 攻撃
void CRay::UpdateAttack()
{
	SetAnimationSpeed(0.4f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	mIsSpawnedWaveEffect = false;
	if (!mIsSpawnedWaveEffect && vectorPos >= 40.0f && mAnimationFrame <= 7.0f)
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

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		mStateStep++;
		break;
		// ステップ1 : 攻撃開始
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// ステップ2 : 攻撃終了
	case 2:
		if (mAnimationFrame >= 13.0f)
		{
			AttackEnd();
			mStateStep++;
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

	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eDie);
		mpHitEffect->StartHitEffect();
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			Kill();
			// エネミーの死亡処理
			CEnemy::RayDeath();
		}
		break;
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
	if (vectorPos <= 33.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
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
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
	mMoveSpeed.Z(0.0f);

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

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eHit)
	{
		mFlyingTime += Time::DeltaTime();
		mAttackTime += Time::DeltaTime();

		if (vectorPos <= ROTATE_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}

		if (mAttackTime >= 3.0f)
		{
			ChangeState(EState::eAttack);
		}
		if (mState == EState::eAttack || mState == EState::eAttackWait)
		{
			mAttackTime = 0.0f;
		}
		if (vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0.0f;
		}
	}

	if (mState == EState::eRun)
	{
		if (vectorPos > STOP_RANGE && vectorPos < WALK_RANGE)
		{
			Position(Position() + mMoveSpeed);
		}
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
	{
		if (mFlyingTime < 4.0f && mFlyingTime >= 0.1f && Position().Y() <= 10.0f)
		{
			if (mState != EState::eAttack)
			{
				Position(Position().X(), Position().Y() + 0.15f, Position().Z());
			}
		}
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
	{
		if (mFlyingTime >= 4.0f)
		{
			if (mState != EState::eAttack)
			{
				Position(Position().X(), Position().Y() - 0.08f, Position().Z());
			}
			if (mFlyingTime >= 6.0f)
			{
				mFlyingTime = 0.0;
			}
		}
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

	CEnemy::Update();

	if (mState == EState::eIdle)
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
				int damage = CalcDamage(1.0f, this, chara);

				// ダメージを与える
				chara->TakeDamage(damage, this);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
	else if (self == mpColLineSide || self == mpColLineHeight
		|| self == mpColliderSphereHead || self == mpColliderSphereBody)
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

	mpGameUI->SetMaxHp(mCharaMaxStatus.hp);
	mpGameUI->SetHp(mCharaStatus.hp);
}

// 被ダメージ処理
void CRay::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		ChangeState(EState::eHit);
		if (mState == EState::eHit)
		{
			mpHitEffect->StartHitEffect();
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
		Position(Position() - dir * Scale().X() * 0.2f);
	}
}

// 死亡処理
void CRay::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}

// ランダムに位置を取得
CVector CRay::GetRandomSpawnPos()
{
	CVector pos = CVector::zero;
	pos.X(Math::Rand(50.0f, 150.0f));
	pos.Z(Math::Rand(390.0f, 490.0f));

	return CVector(pos);
}

// 描画
void CRay::Render()
{
	CXCharacter::Render();
}