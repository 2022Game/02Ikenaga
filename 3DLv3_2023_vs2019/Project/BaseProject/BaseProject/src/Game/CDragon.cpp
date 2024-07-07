#include "CDragon.h"
#include "CPlayer.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "Maths.h"
#include "CHpGauge.h"
#include "CFlamethrower.h"
#include "CFlightFlamethrower.h"
#include "CRoar.h"

// ドラゴンのインスタンス
CDragon* CDragon::spInstance = nullptr;

#define ENEMY_HEIGHT  -2.9f    // 線分コライダー
#define WITHIN_RANGE  70.0f    // 範囲内
#define MOVE_SPEED    0.8f     // 移動速度
#define GRAVITY       0.0625f  // 重力
#define WALK_RANGE    500.0f   // 追跡する範囲
#define STOP_RANGE    155.0f   // 追跡を辞める範囲
#define ROTATE_RANGE  500.0f   // 回転する範囲
#define THROW_INTERVAL 1.0f    // 雄叫びの発射間隔時間

// ドラゴンのアニメーションデータのテーブル
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{ "",										                 true,	  0.0f,  0.0f},  // Tポーズ
	{ "Character\\Enemy\\Dragon\\animation\\DragonSleep.x",	     true,	 81.0f,  0.5f},  // 寝る 81.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonIdle.x",	     true,	 41.0f,  0.5f},  // 待機 41.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonIdle2.x",	     true,	101.0f,  0.5f},  // 待機2 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonAttack.x",	 false,	 81.0f,  0.5f},  // 攻撃(火炎放射) 81.0f
    { "Character\\Enemy\\Dragon\\animation\\DragonAttack2.x",	 false,	 91.0f,  0.5f},  // 攻撃2(前に飛んで後ろに下がる) 91.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonAttack3.x",	 false,	 36.0f,	 0.5f},  // 攻撃3(噛みつき) 36.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyFlame.x",	 false,	 91.0f,	 0.5f},  // フライフレーム 91.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonGetHit.x",	 false,	 41.0f,	 0.5f},  // ヒット 41.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonDefend.x",	 false,	 61.0f,	 0.5f},  // 防御 61.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonDie.x",	     false,	 65.0f,	 0.4f},  // 死ぬ 65.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonScream.x",	 false,	101.0f,  0.5f},  // 雄叫び 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonTakeOff.x",	 false,	121.0f,  0.5f},  // 飛行開始する前 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyIdle.x",	 true,	 31.0f,	 0.5f},  // フライアイドル 31.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonLand.x",	     false,	121.0f,  0.5f},  // 着地 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonRun.x",	     true,	 21.0f,	 0.5f},  // 走る 21.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyForward.x", true,	 31.0f,	 0.5f},	 // フライフォワード 31.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonBackStep.x",   false,	 24.0f,	 0.5f},	 // バックステップ
};

// コンストラクタ
CDragon::CDragon()
    : mState(EState::eIdle3)
	, mpRideObject(nullptr)
	, mFlyingTime(0)
	, mDefenseTime(0)
	, mAttackTime(0)
	, mFlyingAttackTime(0)
	, mBackStepTime(0.0f)
	, mRoarCount(false)
	, mStateStep(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
	, mIsSpawnedRoarEffect(false)
	, mElapsedRoarTime(0.0f)
{
	//インスタンスの設定
	spInstance = this;

	// 敵の種類
	mType = EEnemyType::eDragon;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Dragon");

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
	ChangeAnimation(EAnimType::eIdle3);

	// 線分コライダー
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// キャラクター押し戻し処理(口)
	mpColSphereMouth = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, -1.0f, 0.15f),
		CVector(0.0f, 0.8f, 0.15f),
		6.0f, false, 20.0f
	);
	mpColSphereMouth->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(首)
	mpColSphereNeck = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, -1.5f, 0.15f),
		CVector(0.0f, 1.0f, 0.15f),
		5.5f, false, 20.0f
	);
	mpColSphereNeck->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(体)
	mpColSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		2.0f, false, 20.0f
	);
	mpColSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColSphereBody->Position(-0.2f, -0.5f, 0.1f);

	// キャラクター押し戻し処理(前の左足)
	mpColSphereFeet = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.8f, false, 20.0f
	);
	mpColSphereFeet->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy});
	mpColSphereFeet->Position(0.0f, 0.0f, 0.2f);

	// キャラクター押し戻し処理(前の右足)
	mpColSphereFeet2 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.8f, false, 20.0f
	);
	mpColSphereFeet2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColSphereFeet2->Position(0.0f, 0.0f, 0.2f);

	// ダメージを受けるコライダーを作成(口)
	mpDamageColMouth = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, -0.5f, 0.15f),
		CVector(0.0f, 0.5f, 0.15f),
		10.0f, false
	);
	mpDamageColMouth->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColMouth->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(首)
	mpDamageColNeck = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, -1.0f, 0.15f),
		CVector(0.0f, 1.0f, 0.15f),
		5.5f, false
	);
	mpDamageColNeck->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColNeck->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(胸)
	mpDamageColChest = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		3.0f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColChest->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColChest->SetCollisionTags({ ETag::eWeapon });
	mpDamageColChest->Position(-0.2f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを作成(前の左足)
	mpDamageColFeet = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.8f,false,1.0f
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColFeet->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet->Position(0.0f, 0.0f, 0.2f);

	// ダメージを受けるコライダーを作成(前の右足)
	mpDamageColFeet2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.8f, false, 1.0f
	);
	mpDamageColFeet2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet2->Position(0.0f, 0.0f, 0.2f);

	// ダメージを受けるコライダーを作成(後ろの左足)
	mpDamageColFeet3 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.8f, false, 1.0f
	);
	mpDamageColFeet3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet3->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet3->Position(0.0f, 0.0f, 0.2f);

	// ダメージを受けるコライダーを作成(後ろの右足)
	mpDamageColFeet4= new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.8f, false, 1.0f
	);
	mpDamageColFeet4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet4->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet4->Position(0.0f, 0.0f, 0.2f);

	// ダメージを与えるコライダー(口)
	mpAttackColMouth = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, -1.0f, 0.15f),
		CVector(0.0f, 0.8f, 0.15f),
		6.0f, false
	);
	mpAttackColMouth->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColMouth->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(体)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		2.0f, false
	);
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(前の左足)
	mpAttackColFeet = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.8f, false
	);
	mpAttackColFeet->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColFeet->SetCollisionTags({ ETag::ePlayer });
	mpAttackColFeet->Position(0.0f, 0.0f, 0.2f);

	// ダメージを与えるコライダー(前の右足)
	mpAttackColFeet2 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.8f, false
	);
	mpAttackColFeet2->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColFeet2->SetCollisionTags({ ETag::ePlayer });
	mpAttackColFeet2->Position(0.0f, 0.0f, 0.2f);

	// ダメージを受けるコライダーと攻撃コライダーをドラゴンの頭の行列にアタッチ
	const CMatrix* headMtx = GetFrameMtx("Armature_Head");

	// 押し戻しコライダーとダメージを受けるコライダーと
	// 攻撃コライダーをドラゴンの口の行列にアタッチ
	const CMatrix* mouthMtx = GetFrameMtx("Armature_UpperMouth01");
	mpColSphereMouth->SetAttachMtx(mouthMtx);
	mpDamageColMouth->SetAttachMtx(mouthMtx);
	mpAttackColMouth->SetAttachMtx(mouthMtx);

	// 押し戻しコライダーと
	// ダメージを受けるコライダーをドラゴンの首の行列にアタッチ
	const CMatrix* neckMtx = GetFrameMtx("Armature_Neck03");
	mpColSphereNeck->SetAttachMtx(neckMtx);
	mpDamageColNeck->SetAttachMtx(neckMtx);

	// 押し戻しコライダーとダメージを受けるコライダーと
	// 攻撃コライダーをドラゴンの胸の行列にアタッチ
	const CMatrix* chestMtx = GetFrameMtx("Armature_Chest");
	mpColSphereBody->SetAttachMtx(chestMtx);
	mpDamageColChest->SetAttachMtx(chestMtx);
	mpAttackColBody->SetAttachMtx(chestMtx);

	// 押し戻しコライダーとダメージを受けるコライダーと
	// 攻撃コライダーをドラゴンの前の左足の行列にアタッチ
	const CMatrix* leftFootMtx = GetFrameMtx("Armature_Middle01_L");
	mpColSphereFeet->SetAttachMtx(leftFootMtx);
	mpDamageColFeet->SetAttachMtx(leftFootMtx);
	mpAttackColFeet->SetAttachMtx(leftFootMtx);

	// 押し戻しコライダーとダメージを受けるコライダーと
	// 攻撃コライダーをドラゴンの前の右足の行列にアタッチ
	const CMatrix* rightFootMtx = GetFrameMtx("Armature_Middle01_R");
	mpColSphereFeet2->SetAttachMtx(rightFootMtx);
	mpDamageColFeet2->SetAttachMtx(rightFootMtx);
	mpAttackColFeet2->SetAttachMtx(rightFootMtx);

	// ダメージを受けるコライダーをドラゴンの後ろの左足の行列にアタッチ
	const CMatrix* leftFootMtx2 = GetFrameMtx("Armature_MiddleToe01_L");
	mpDamageColFeet3->SetAttachMtx(leftFootMtx2);

	// ダメージを受けるコライダーをドラゴンの後ろの右足の行列にアタッチ
	const CMatrix* rightFootMtx2 = GetFrameMtx("Armature_MiddleToe01_R");
	mpDamageColFeet4->SetAttachMtx(rightFootMtx2);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColMouth->SetEnable(false);
	mpAttackColBody->SetEnable(false);
	mpAttackColFeet->SetEnable(false);
	mpAttackColFeet2->SetEnable(false);

	// 火炎放射の生成
	const CMatrix* mtx = GetFrameMtx("Armature_Tongue01");
	mpFlamethrower = new CFlamethrower
	(
		this, mtx,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0,-90.f,0.0f).Matrix()
	); 

	// 飛行中の火炎放射の生成
	mpFlightFlamethrower = new CFlightFlamethrower
	(
		this, mtx,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0, -90.f, 0.0f).Matrix()
	);
}

// デストラクタ
CDragon::~CDragon()
{
	// 線分コライダーを削除
	SAFE_DELETE(mpColliderLine);
	// キャラ押し戻しコライダーを削除
	SAFE_DELETE(mpColSphereMouth);
	SAFE_DELETE(mpColSphereBody);
	SAFE_DELETE(mpColSphereNeck);
	SAFE_DELETE(mpColSphereFeet);
	SAFE_DELETE(mpColSphereFeet2);
	// ダメージを受けるコライダーを削除
	SAFE_DELETE(mpDamageColMouth);
	SAFE_DELETE(mpDamageColNeck);
	SAFE_DELETE(mpDamageColChest);
	SAFE_DELETE(mpDamageColFeet);
	SAFE_DELETE(mpDamageColFeet2);
	SAFE_DELETE(mpDamageColFeet3);
	SAFE_DELETE(mpDamageColFeet4);
	// 攻撃コライダーを削除
	SAFE_DELETE(mpAttackColMouth);
	SAFE_DELETE(mpAttackColBody);
	SAFE_DELETE(mpAttackColFeet);
	SAFE_DELETE(mpAttackColFeet2);
}

// ドラゴンのインスタンス
CDragon* CDragon::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CDragon::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
	SetAnimationSpeed(data.animSpeed);
}

// 雄叫びエフェクトを生成
void CDragon::CreateRoar()
{
	// 雄叫びの生成
	CRoar* roar = new CRoar
	(
		this,
		Position() + CVector(0.0f, 0.0f, 0.0f),
		VectorZ(),
		30.0f,
		50.0f
	);
	roar->Scale(100.0f, 100.0f, 100.0f);
	// 雄叫びエフェクトの色設定
	roar->SetColor(CColor(1.0f, 1.0f, 1.0f));
}

// 状態の切り替え
void CDragon::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// 待機状態(寝ている時)
void CDragon::UpdateIdle()
{ 
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= WITHIN_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// 待機状態2
void CDragon::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (GetAnimationFrame() >= 10.0f && vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		ChangeState(EState::eRun);
	}
	else
	{
		ChangeState(EState::eIdle2);
	}
}

// 待機状態3
void CDragon::UpdateIdle3()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= WITHIN_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// 攻撃(火炎放射) 
void CDragon::UpdateAttack()
{
	SetAnimationSpeed(0.5f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= ROTATE_RANGE && vectorPos >= STOP_RANGE)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃アニメーション開始+攻撃開始
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		AttackStart();
		mStateStep++;
		break;
		// ステップ1　: 火炎放射開始
	case 1:
		if (mAnimationFrame >= 17.5f)
		{
			if (!mpFlamethrower->IsThrowing())
			{
				mpFlamethrower->Start();
				SetAnimationSpeed(0.25f);
				mStateStep++;
			}
		}
		break;
		// ステップ2 : 火炎放射アニメーション終了待ち
	case 2:
		if (mAnimationFrame >= 70.0f)
		{
			mpFlamethrower->Stop();
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃2(前に飛んで後ろに下がる)
void CDragon::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 37.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
	case 2:
		if (mAnimationFrame >= 55.0f)
		{
			AttackEnd();
			mStateStep++;
		}
		break;
	case 3:
		if (mAnimationFrame >= 90.0f)
		{
			// 攻撃2終了待ち状態へ移行
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}


// 攻撃3(嚙みつき)
void CDragon::UpdateAttack3()
{
	SetAnimationSpeed(0.5f);
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack3);
		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
	case 2:
		if (mAnimationFrame >= 20.0f)
		{
			AttackEnd();
			mStateStep++;
		}
		break;
	case 3:
		if (mAnimationFrame >= 36.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃終了待ち
void CDragon::UpdateAttackWait()
{
	mpFlamethrower->Stop();
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle2);
	}
}

// 飛行中の攻撃
void CDragon::UpdateFlyingAttack()
{
	SetAnimationSpeed(0.5f);

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eFlyingAttack);
		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 30.0f)
		{
			if (!mpFlightFlamethrower->IsThrowing())
			{
				mpFlightFlamethrower->Start();
				mStateStep++;
			}
		}
		break;
	case 2:
		if (mAnimationFrame >= 85.0f)
		{
			mpFlightFlamethrower->Stop();
			mStateStep++;
		}
		break;
	case 3:
		if (mAnimationFrame >= 89.0f)
		{
			ChangeState(EState::eFlyingAttackWait);
		}
		break;
	}
}

// 飛行中の攻撃終了待ち
void CDragon::UpdateFlyingAttackWait()
{
	mpFlightFlamethrower->Stop();
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eFlyingIdle);
	}
}

// ヒット
void CDragon::UpdateHit()
{
	SetAnimationSpeed(0.5f);
	mpFlamethrower->Stop();
	mpFlightFlamethrower->Stop();
	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		ChangeState(EState::eIdle2);
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// 防御
void CDragon::UpdateDefense()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDefense);

	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// 死ぬ
void CDragon::UpdateDie()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理へ
		CEnemy::DragonDeath();
	}
}

// 雄叫び
void CDragon::UpdateRoar()
{
	mRoarCount = true;
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eRoar);
	mIsSpawnedRoarEffect = false;
	if (mAnimationFrame >= 1.0f)
	{
		if (!mIsSpawnedRoarEffect)
		{
			mIsSpawnedRoarEffect = true;
		}
	}
	if (IsAnimationFinished())
	{
		mIsSpawnedRoarEffect = false;
		ChangeState(EState::eFlyingStart);
	}
}

// 飛行開始
void CDragon::UpdateFlyingStart()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eFlyingStart);

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (vectorPos <= ROTATE_RANGE && vectorPos >= STOP_RANGE)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (IsAnimationFinished())
	{
		ChangeState(EState::eFlyingIdle);
	}
}

// 飛行中の待機
void CDragon::UpdateFlyingIdle()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eFlyingIdle);

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (vectorPos <= ROTATE_RANGE && vectorPos >= STOP_RANGE)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (GetAnimationFrame() >= 10.0f && vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		ChangeState(EState::eFlyForward);
	}
	else if (IsAnimationFinished())
	{
		ChangeState(EState::eFlyingIdle);
	}
}

// 飛行終了(着地)
void CDragon::UpdateFlyingEnd()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eFlyingEnd);

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (vectorPos <= ROTATE_RANGE && vectorPos >= STOP_RANGE)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// 移動
void CDragon::UpdateRun()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	if (vectorPos <= ROTATE_RANGE && vectorPos >= STOP_RANGE)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	// 範囲内の時、移動し追跡する
	if (vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += newPos * MOVE_SPEED;
	}
	// 追跡が止まった時、待機モーションへ
	if (vectorPos <= STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// 飛行中の移動
void CDragon::UpdateFlyForward()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eFlyForward);

	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	if (vectorPos <= ROTATE_RANGE && vectorPos >= STOP_RANGE)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	// 範囲内の時、移動し追跡する
	if (vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += newPos * MOVE_SPEED;
	}
	// 追跡が止まった時、待機モーションへ
	if (vectorPos <= STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		ChangeState(EState::eFlyingIdle);
	}
}

// バックステップ
void CDragon::UpdateBackStep()
{
	mMoveSpeed = CVector::zero;

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	switch (mStateStep)
	{
	case 0:
		SetAnimationSpeed(0.5f);
		ChangeAnimation(EAnimType::eBackStep);

		Position(Position() - VectorZ() * 150.0f);

		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 24.0f)
		{
			bool mAttack = false;   // 攻撃(火炎放射)
			bool mAttack2 = false;  // 攻撃2(前に飛んで後ろに下がる)
			// 確率を最小に2最大5
			int probability2 = Math::Rand(2, 6);

			if (probability2 == 2)mAttack2 = true;
			if (mAttack2)
			{
				ChangeState(EState::eAttack2);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}
		break;
	}
}

// 更新処理
void CDragon::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
	mMoveSpeed.Z(0.0f);

	if (CInput::PushKey(('T')))
	{
		ChangeState(EState::eBackStep);
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
		// 待機状態3
	case EState::eIdle3:
		UpdateIdle3();
		break;
		// 攻撃(火炎放射)
	case EState::eAttack:
		UpdateAttack();
		break;
		// 攻撃2(前に飛んで後ろに下がる)
	case EState::eAttack2:
		UpdateAttack2();
		break;
		// 攻撃3(嚙みつき)
	case EState::eAttack3:
		UpdateAttack3();
		break;
		// 攻撃終了待ち
	case EState::eAttackWait:
		UpdateAttackWait();
		break;
		// 飛行中の攻撃(火炎放射)
	case EState::eFlyingAttack:
		UpdateFlyingAttack();
		break;
		// 飛行中の攻撃終了待ち
	case EState::eFlyingAttackWait:
		UpdateFlyingAttackWait();
		break;
		// ヒット
	case EState::eHit:
		UpdateHit();
		break;
		// 防御
	case EState::eDefense:
		UpdateDefense();
		break;
		// 死ぬ
	case EState::eDie:
		UpdateDie();
		break;
		// 雄叫び
	case EState::eRoar:
		UpdateRoar();
		break;
		// 飛行開始
	case EState::eFlyingStart:
		UpdateFlyingStart();
		break;
		// 飛行の待機
	case EState::eFlyingIdle:
		UpdateFlyingIdle();
		break;
		// 飛行終了(着地)
	case EState::eFlyingEnd:
		UpdateFlyingEnd();
		break;
		// 移動
	case EState::eRun:
		UpdateRun();
		break;
		// 飛行中の移動
	case EState::eFlyForward:
		UpdateFlyForward();
		break;
		// バックステップ
	case EState::eBackStep:
		UpdateBackStep();
		break;
	}

	// HPゲージの座標を更新(敵の座標の少し上の座標)
	CVector gaugePos = Position() + CVector(0.0f, 35.0f, 0.0f);
	CVector gaugePos2 = Position() + CVector(0.0f, 150.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	
	if (mState != EState::eIdle3 && mState != EState::eDie)
	{
		if (mState == EState::eFlyingIdle)
		{
			mpHpGauge->SetWorldPos(gaugePos2);
		}
		else
		{
			mpHpGauge->SetWorldPos(gaugePos);
		}
	}

	if (mState == EState::eIdle2 && vectorPos <= 110.0f 
		|| mState == EState::eHit && vectorPos <= 110.0f)
	{
		mBackStepTime += Time::DeltaTime();
	}
	if (mBackStepTime >= 3)
	{
		mBackStepTime = 0;
		mAttackTime = 0;
		ChangeState(EState::eBackStep);
	}

	//if (mState != EState::eIdle3 && vectorPos > -110.0f)
	//{
	//	if (vectorPos <= ROTATE_RANGE)
	//	{
	//		// プレイヤーのいる方向へ向く
	//		CVector dir = player->Position() - Position();
	//		dir.Y(0.0f);
	//		dir.Normalize();
	//		Rotation(CQuaternion::LookRotation(dir));
	//	}
	//}
	
	if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eHit)
	{
		mAttackTime++;

		if (vectorPos <= ROTATE_RANGE && vectorPos >= STOP_RANGE)
		{
			// プレイヤーのいる方向へ向く
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}

		if (mAttackTime > 200)
		{
			bool mAttack = false;   // 攻撃(火炎放射)
			bool mAttack2 = false;  // 攻撃2(前に飛んで後ろに下がる)
			bool mDefense = false;  // 防御
			// 確率を最小に2最大5
			int probability2 = Math::Rand(2, 4);
			// 確率を最小に5最大7
			int probability3 = Math::Rand(5, 7);
			// 確率を最小に8最大10
			int probability4 = Math::Rand(8, 10);

			if (probability2 == 2)mAttack2 = true;
			if (probability3 == 5)mAttack = true;
			if (probability4 == 8)mDefense = true;
			if (mAttack2)
			{
				ChangeState(EState::eAttack2);
			}
			else if (mAttack)
			{
				//ChangeState(EState::eAttack);
			}
			else if (mDefense)
			{
				//ChangeState(EState::eDefense);
			}
			else
			{
				ChangeState(EState::eAttack3);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3
			|| mState == EState::eDefense || mState == EState::eFlyingIdle
			|| mState == EState::eBackStep)
		{
			mAttackTime = 0;
		}

		if (mCharaStatus.hp <= mCharaMaxStatus.hp * 0.5 && mFlyingTime >= 1)
		{
			mFlyingTime++;
		}
		if (mState == EState::eFlyingStart)
		{
			mFlyingTime = 0;
		}
	}

	// 防御時間計測
	if (mState == EState::eDefense)
	{
		mDefenseTime++;
	}
	else
	{
		mDefenseTime = 0;
	}

	if (mState != EState::eFlyingStart && mState != EState::eFlyingIdle && mState != EState::eFlyingAttack
		&& mState != EState::eFlyingAttackWait && mState != EState::eFlyingEnd && mState != EState::eFlyForward)
	{
		if (mCharaStatus.hp <= mCharaMaxStatus.hp * 0.5 && mFlyingTime == 0 && mRoarCount != true)
		{
			ChangeState(EState::eRoar);
		}
		if (mCharaStatus.hp <= mCharaMaxStatus.hp * 0.5 && mFlyingTime >= 400)
		{
			ChangeState(EState::eFlyingStart);
		}
	}

	if (mState == EState::eFlyingIdle || mState == EState::eFlyForward)
	{
		mFlyingTime++;
		mFlyingAttackTime++;

		if (mFlyingAttackTime > 200)
		{
			ChangeState(EState::eFlyingAttack);
		}
	}
	if (mState == EState::eFlyingAttack || mState == EState::eFlyingEnd)
	{
		mFlyingAttackTime = 0;
	}

	if (mFlyingTime >= 550)
	{
		ChangeState(EState::eFlyingEnd);
	}
	if (mState == EState::eFlyingEnd || mState == EState::eFlyingStart && mFlyingTime >= 400)
	{
		mFlyingTime = 1;
	}

	if (mState == EState::eRun || mState == EState::eFlyForward)
	{
		if (vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
		{
			Position(Position() + mMoveSpeed);
		}
	}

	if (mState == EState::eBackStep)
	{
		Position(Position() - mMoveSpeed);
	}
	CDebugPrint::Print(" 攻撃時間: %d", mAttackTime);
	CDebugPrint::Print(" HP: %d", mCharaStatus.hp);
	CDebugPrint::Print(" 距離: %f", vectorPos);
	CDebugPrint::Print(" 後ろ: %f", mBackStepTime);

	if (mIsSpawnedRoarEffect)
	{
		mElapsedRoarTime += Time::DeltaTime();

		// 経過時間に応じて、雄叫びのエフェクトを作成
		if (mElapsedRoarTime >= THROW_INTERVAL)
		{
			CreateRoar();
			mElapsedRoarTime -= THROW_INTERVAL;
		}
	}

	// キャラクターの更新
	CXCharacter::Update();

	 // キャラ押し戻しコライダー
	mpColSphereMouth->Update();
	mpColSphereBody->Update();
	mpColSphereNeck->Update();
	mpColSphereFeet->Update();
	mpColSphereFeet2->Update();

	// ダメージを受けるコライダー
	mpDamageColMouth->Update();
	mpDamageColNeck->Update();
	mpDamageColChest->Update();
	mpDamageColFeet->Update();
	mpDamageColFeet2->Update();
	mpDamageColFeet3->Update();
	mpDamageColFeet4->Update();

	// 攻撃コライダー
	mpAttackColMouth->Update();
	mpAttackColBody->Update();
	mpAttackColFeet->Update();
	mpAttackColFeet2->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColMouth || self == mpAttackColBody 
		|| self == mpAttackColFeet || self == mpAttackColFeet2
		&& mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eIdle3)
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
	if (self == mpColliderLine)
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
	else if (self == mpColSphereMouth || self == mpColSphereBody || self == mpColSphereNeck
		|| self == mpColSphereFeet || self == mpColSphereFeet2)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CDragon::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	if (mState == EState::eAttack2)
	{
		mpAttackColBody->SetEnable(true);
		mpAttackColMouth->SetEnable(true);
		mpAttackColFeet->SetEnable(true);
		mpAttackColFeet2->SetEnable(true);
	}
	else if (mState == EState::eAttack3)
	{
		mpAttackColMouth->SetEnable(true);
	}
}

// 攻撃終了
void CDragon::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColMouth->SetEnable(false);
	mpAttackColBody->SetEnable(false);
	mpAttackColFeet->SetEnable(false);
	mpAttackColFeet2->SetEnable(false);

}

// 描画
void CDragon::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CDragon::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CDragon::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY11_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CDragon::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		mpFlamethrower->Stop();
		mpFlightFlamethrower->Stop();
		if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eAttack3)
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
}

// 防御力の強化割合を取得
float CDragon::GetDefBuff(const CVector& attackDir)const
{
	// 防御状態であれば、防御2倍
	if (mState == EState::eDefense) return 2.0f;
	if (mState == EState::eRoar || mState == EState::eFlyingStart) return 20.0f;

	// 通常時の防御の割合
	return mBaseDefenseBuffRatio;
}

// 死亡処理
void CDragon::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}