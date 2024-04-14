#include "CBeholder.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CLightningBallEffect.h"
#include "CElectricShockEffect.h"
#include "CInput.h"

// 球体のモンスターのインスタンス
CBeholder* CBeholder::spInstance = nullptr;

#define ENEMY_HEIGHT 0.5f
#define WITHIN_RANGE 40.0f       // 範囲内
#define MOVE_SPEED 0.1f          // 移動速度
#define GRAVITY 0.0625f          // 重力
#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 24.0f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// 球体のモンスターのアニメーションデータのテーブル
const CBeholder::AnimData CBeholder::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle.x",	true,	82.0f	},	    // 待機 41.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle2.x",	true,	46.0f	},	    // 待機2 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack.x",	true,	50.0f	},	    // 攻撃 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack2.x",	true,	50.0f	},	    // 攻撃2 21.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack3.x",	true,	42.0f	},	    // 攻撃3 17.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack4.x",	true,	50.0f	},	    // 攻撃4 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderGetHit.x",	true,	46.0f	},	    // ヒット 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderDie.x",	true,	76.0f	},	        // 死ぬ 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderDizzy.x",	true,	82.0f	},	    // めまい 41.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderRun.x",	true,	34.0f	},	    // 走る 17.0f
};

// コンストラクタ
CBeholder::CBeholder()
	: mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Beholder");

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
		CVector(0.0f, 0.0f, -0.5f),
		CVector(0.0f, 0.0f, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine->Position(0.0f, 20.0f, 0.0f);

	mpColliderLine2 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, -0.8, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine2->SetCollisionLayers({ ELayer::eField });
	mpColliderLine2->Position(0.0f, 20.0f, 0.0f);

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,0.41f, false, 2.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(左上の触手)
	mpColliderSphereTentacle = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphereTentacle->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(右上の触手)
	mpColliderSphereTentacle2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.1f, false, 1.0f
	);
	mpColliderSphereTentacle2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(左下の触手)
	mpColliderSphereTentacle3 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphereTentacle3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(右下の触手)
	mpColliderSphereTentacle4 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphereTentacle4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(真ん中上の触手)
	mpColliderSphereTentacle5 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 1.0f
	);
	mpColliderSphereTentacle5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(真ん中下の触手)
	mpColliderSphereTentacle6 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphereTentacle6->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.41f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(左上の触手)
	mpDamageColTentacle = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.087f, false
	);
	mpDamageColTentacle->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(右上の触手)
	mpDamageColTentacle2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.1f, false
	);
	mpDamageColTentacle2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle2->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(左下の触手)
	mpDamageColTentacle3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageColTentacle3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle3->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(右下の触手)
	mpDamageColTentacle4 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageColTentacle4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle4->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(真ん中上の触手)
	mpDamageColTentacle5 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.08f, false
	);
	mpDamageColTentacle5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle5->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(真ん中下の触手)
	mpDamageColTentacle6 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageColTentacle6->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle6->SetCollisionTags({ ETag::eWeapon });

	// ダメージを与えるコライダー(体)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.41f, false
	);
	// 攻撃コライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(左上の触手)
	mpAttackColTentacle = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackColTentacle->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(右上の触手)
	mpAttackColTentacle2 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.1f, false
	);
	mpAttackColTentacle2->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle2->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(左下の触手)
	mpAttackColTentacle3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackColTentacle3->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle3->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(右下の触手)
	mpAttackColTentacle4 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackColTentacle4->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle4->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(真ん中上の触手)
	mpAttackColTentacle5 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.08f, false
	);
	mpAttackColTentacle5->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle5->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(真ん中下の触手)
	mpAttackColTentacle6 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackColTentacle6->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle6->SetCollisionTags({ ETag::ePlayer });

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);
	mpAttackColBody->SetAttachMtx(bodyMty);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの左上の触手の行列にアタッチ
	const CMatrix* tentacleMty = GetFrameMtx("Armature_TentacleA05");
	mpColliderSphereTentacle->SetAttachMtx(tentacleMty);
	mpDamageColTentacle->SetAttachMtx(tentacleMty);
	mpAttackColTentacle->SetAttachMtx(tentacleMty);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの右上の触手の行列にアタッチ
	const CMatrix* tentacleMty2 = GetFrameMtx("Armature_TentacleE05");
	mpColliderSphereTentacle2->SetAttachMtx(tentacleMty2);
	mpDamageColTentacle2->SetAttachMtx(tentacleMty2);
	mpAttackColTentacle2->SetAttachMtx(tentacleMty2);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの左下の触手の行列にアタッチ
	const CMatrix * tentacleMty3 = GetFrameMtx("Armature_TentacleB05");
	mpColliderSphereTentacle3->SetAttachMtx(tentacleMty3);
	mpDamageColTentacle3->SetAttachMtx(tentacleMty3);
	mpAttackColTentacle3->SetAttachMtx(tentacleMty3);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの右下の触手の行列にアタッチ
	const CMatrix* tentacleMty4 = GetFrameMtx("Armature_TentacleD05");
	mpColliderSphereTentacle4->SetAttachMtx(tentacleMty4);
	mpDamageColTentacle4->SetAttachMtx(tentacleMty4);
	mpAttackColTentacle4->SetAttachMtx(tentacleMty4);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの真ん中上の触手の行列にアタッチ
	const CMatrix* tentacleMty5 = GetFrameMtx("Armature_TentacleF05");
	mpColliderSphereTentacle5->SetAttachMtx(tentacleMty5);
	mpDamageColTentacle5->SetAttachMtx(tentacleMty5);
	mpAttackColTentacle5->SetAttachMtx(tentacleMty5);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーを球体のモンスターの真ん中下の触手の行列にアタッチ
	const CMatrix* tentacleMty6 = GetFrameMtx("Armature_TentacleC05");
	mpColliderSphereTentacle6->SetAttachMtx(tentacleMty6);
	mpDamageColTentacle6->SetAttachMtx(tentacleMty6);
	mpAttackColTentacle6->SetAttachMtx(tentacleMty6);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColBody->SetEnable(false);
	mpAttackColTentacle->SetEnable(false);
	mpAttackColTentacle2->SetEnable(false);
	mpAttackColTentacle3->SetEnable(false);
	mpAttackColTentacle4->SetEnable(false);
	mpAttackColTentacle5->SetEnable(false);
	mpAttackColTentacle6->SetEnable(false);

	mpLightningBall = new CLightningBallEffect
	(
		this, nullptr,
		CVector(0.0f, 20.0f, 0.0f),
		CQuaternion(0.0, 0.f, 0.0f).Matrix()
	);

	mpElectricShock = new  CElectricShockEffect
	(
		this, nullptr,
		CVector(0.0f, 22.0f, 0.0f),
		CQuaternion(0.0, 0.f, 0.0f).Matrix()
	);
}

CBeholder::~CBeholder()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderLine2);

	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereTentacle);
	SAFE_DELETE(mpColliderSphereTentacle2);
	SAFE_DELETE(mpColliderSphereTentacle3);
	SAFE_DELETE(mpColliderSphereTentacle4);
	SAFE_DELETE(mpColliderSphereTentacle5);
	SAFE_DELETE(mpColliderSphereTentacle6);

	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColTentacle);
	SAFE_DELETE(mpDamageColTentacle2);
	SAFE_DELETE(mpDamageColTentacle3);
	SAFE_DELETE(mpDamageColTentacle4);
	SAFE_DELETE(mpDamageColTentacle5);
	SAFE_DELETE(mpDamageColTentacle6);

	SAFE_DELETE(mpAttackColBody);
	SAFE_DELETE(mpAttackColTentacle);
	SAFE_DELETE(mpAttackColTentacle2);
	SAFE_DELETE(mpAttackColTentacle3);
	SAFE_DELETE(mpAttackColTentacle4);
	SAFE_DELETE(mpAttackColTentacle5);
	SAFE_DELETE(mpAttackColTentacle6);
}

CBeholder* CBeholder::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CBeholder::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 待機状態
void CBeholder::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// 待機状態2
void CBeholder::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mState = EState::eRun;
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle2);
		if (IsAnimationFinished())
		{
			mState = EState::eIdle2;
		}
	}
}

// 攻撃
void CBeholder::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	if (mAnimationFrame >= 0.0f && mAnimationFrame < 5.0f)
	{
		AttackStart();
	}

	if (mAnimationFrame >= 10.0f)
	{
		if (!mpLightningBall->IsThrowing()&& !mpElectricShock->IsThrowing())
		{
			mpLightningBall->Start();
			mpElectricShock->Start();
		}
	}

	if (IsAnimationFinished())
	{
		mpLightningBall->Stop();
		mpElectricShock->Stop();
		// 攻撃終了待ち状態へ移行
		mState = EState::eAttackWait;
	}
}

// 攻撃2
void CBeholder::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack2);
	if (mAnimationFrame >= 0.0f && mAnimationFrame < 5.0f)
	{
		AttackStart();
	}

	if (mAnimationFrame >= 1.0f)
	{
		if (!mpElectricShock->IsThrowing())
		{
			mpElectricShock->Start();
		}
	}

	if (IsAnimationFinished())
	{
		mpElectricShock->Stop();
		// 攻撃2終了待ち状態へ移行
		mState = EState::eAttackWait;
	}
}

// 攻撃3
void CBeholder::UpdateAttack3()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack3);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃4
void CBeholder::UpdateAttack4()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack4);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃終了待ち
void CBeholder::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpElectricShock->Stop();
		mState = EState::eIdle2;
	}
}

// ヒット
void CBeholder::UpdateHit()
{
	mpElectricShock->Stop();
	mpLightningBall->Stop();
	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// めまいをfalseにする
		bool stan = false;
		// 確率を最小に0最大40
		int probability = Math::Rand(0, 20);
		if (probability == 1)stan = true;
		if (stan)
		{
			mState = EState::eDizzy;
		}
		else
		{
			// プレイヤーの攻撃がヒットした時の待機状態へ移行
			mState = EState::eIdle2;
			ChangeAnimation(EAnimType::eIdle2);
		}
	}
}

// 死ぬ
void CBeholder::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::BeholderDeath();
	}
}

// めまい(混乱)
void CBeholder::UpdateDizzy()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		mState = EState::eIdle2;
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// 移動
void CBeholder::UpdateRun()
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
		mState = EState::eIdle2;
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// 更新処理
void CBeholder::Update()
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
		// 待機状態2
	case EState::eIdle2:
		UpdateIdle2();
		break;
		// 攻撃
	case EState::eAttack:
		UpdateAttack();
		break;
		// 攻撃2
	case EState::eAttack2:
		UpdateAttack2();
		break;
		// 攻撃3
	case EState::eAttack3:
		UpdateAttack3();
		break;
		// 攻撃4
	case EState::eAttack4:
		UpdateAttack4();
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
	CVector gaugePos = Position() + CVector(0.0f, 35.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eAttack 
		&& mState != EState::eAttack2 && mState != EState::eAttack3 && mState != EState::eAttack4  && mState != EState::eAttackWait
		&& mState != EState::eHit && mState != EState::eDie && mState != EState::eDizzy && mState != EState::eRun)
	{
		mState = EState::eIdle2;
	}

	if (mState == EState::eRun || mState == EState::eIdle2 || mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3
		|| mState == EState::eAttack4 || mState == EState::eHit || mState == EState::eDie || mState == EState::eDizzy || mState == EState::eAttackWait)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
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
			// 攻撃3
			bool Attack3 = false;
			// 攻撃4
			bool Attack4 = false;
			// 確率を最小に2最大5
			int probability2 = Math::Rand(2, 5);
			// 確率を最小に6最大10
			int probability3 = Math::Rand(6, 10);
			// 確率を最小に11最大15
			int probability4 = Math::Rand(11, 15);

			if (probability2 == 2)Attack2 = true;
			if (probability3 == 6)Attack3 = true;
			if (probability4 == 11)Attack4 = true;
			if (Attack2)
			{
				//mState = EState::eAttack2;
			}
			else if (Attack3)
			{
				//mState = EState::eAttack3;
			}
			else if (Attack4)
			{
				//mState = EState::eAttack4;
			}
			else
			{
				mState = EState::eAttack;
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3 || mState == EState::eAttack4)
		{
			mAttackTime = 0;
		}
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
	{
		if (mFlyingTime <= 200 && Position().Y() <= 0.0f)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + 0.02f);
		}

		if (mFlyingTime >= 200 && Position().Y() >= 0.1f)
		{
			Position(Position().X(), Position().Y() - 0.5f, Position().Z());
		}
	}

	if (Position().Y() >= 0.1f || vectorp >= 24.0f && vectorp <= WALK_RANGE)
	{
		mFlyingTime++;
	}

	if (Position().Y() <= 0.0f)
	{
		mFlyingTime = 0;
	}
	if (mState == EState::eHit)
	{
		Position(Position().X(), Position().Y() - 0.5f, Position().Z());
	}

	CDebugPrint::Print(" 攻撃時間: %d\n", mAttackTime);
	//CDebugPrint::Print(" HP: %d\n", mCharaStatus.hp);

	// キャラクターの更新
	CXCharacter::Update();

	mpColliderSphereBody->Update();
	mpColliderSphereTentacle->Update();
	mpColliderSphereTentacle2->Update();
	mpColliderSphereTentacle3->Update();
	mpColliderSphereTentacle4->Update();
	mpColliderSphereTentacle5->Update();
	mpColliderSphereTentacle6->Update();

	mpDamageColBody->Update();
	mpDamageColTentacle->Update();
	mpDamageColTentacle2->Update();
	mpDamageColTentacle3->Update();
	mpDamageColTentacle4->Update();
	mpDamageColTentacle5->Update();
	mpDamageColTentacle6->Update();

	mpAttackColBody->Update();
	mpAttackColTentacle->Update();
	mpAttackColTentacle2->Update();
	mpAttackColTentacle3->Update();
	mpAttackColTentacle4->Update();
	mpAttackColTentacle5->Update();
	mpAttackColTentacle6->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);

	if (CInput::PushKey('Q'))
	{
		if (!mpLightningBall->IsThrowing())
		{
			mpLightningBall->Start();
		}
		else
		{
			mpLightningBall->Stop();
		}
	}
}

// 衝突処理
void CBeholder::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColBody || self == mpAttackColTentacle || self == mpAttackColTentacle2 || self == mpAttackColTentacle3 || self == mpAttackColTentacle4
		|| self == mpAttackColTentacle5 || self == mpAttackColTentacle6 && mState != EState::eIdle && mState != EState::eIdle2)
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
	else if (self == mpColliderLine || self == mpColliderLine2)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;
			//mMoveSpeed.Y(0.0f);

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// キャラクター同士の衝突処理
	else if (self == mpColliderSphereBody || self == mpColliderSphereTentacle || self == mpColliderSphereTentacle2
		|| self == mpColliderSphereTentacle3 || self == mpColliderSphereTentacle4 || self == mpColliderSphereTentacle5
		|| self == mpColliderSphereTentacle6)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CBeholder::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackColBody->SetEnable(true);
	mpAttackColTentacle->SetEnable(true);
	mpAttackColTentacle2->SetEnable(true);
	mpAttackColTentacle3->SetEnable(true);
	mpAttackColTentacle4->SetEnable(true);
	mpAttackColTentacle5->SetEnable(true);
	mpAttackColTentacle6->SetEnable(true);
}

// 攻撃終了
void CBeholder::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColBody->SetEnable(false);
	mpAttackColTentacle->SetEnable(false);
	mpAttackColTentacle2->SetEnable(false);
	mpAttackColTentacle3->SetEnable(false);
	mpAttackColTentacle4->SetEnable(false);
	mpAttackColTentacle5->SetEnable(false);
	mpAttackColTentacle6->SetEnable(false);
}

// 描画
void CBeholder::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CBeholder::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CBeholder::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY8_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CBeholder::TakeDamage(int damage, CObjectBase* causedObj)
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
void CBeholder::Death()
{
	// 死亡状態へ移行
	mState = EState::eDie;
}