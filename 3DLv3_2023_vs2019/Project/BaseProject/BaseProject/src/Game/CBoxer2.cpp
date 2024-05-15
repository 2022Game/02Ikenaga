#include "CBoxer2.h"
#include "CPlayer.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CImpactEffect.h"

// ボクサー2のインスタンス
CBoxer2* CBoxer2::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 60.0f       // 範囲内
#define MOVE_SPEED 0.13f         // 移動速度
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f          // 重力
#define JUMP_END_Y 1.0f
#define WALK_RANGE 150.0f        // 追跡する範囲
#define STOP_RANGE 30.0f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// ボクサー2のアニメーションデータのテーブル
const CBoxer2::AnimData CBoxer2::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Dance.x",	true,	170.0f	},	        // ダンス 85.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Idle.x",	true,	42.0f	},	        // 待機 21.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Attack.x",	true,	35.0f	},	    // 攻撃 19.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Attack2.x",	true,	52.0f	},	    // 攻撃2 26.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2GetHit.x",	true,	36.0f	},	    // ヒット 17.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Defense.x",	true,	42.0f	},	    // 防御 21.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2DefenseHit.x",	true,	42.0f	},  // 防御中のヒット 21.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Die.x",	true,	70.0f	},	        // 死ぬ 24.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Dizzy.x",	true,	66.0f	},	        // めまい 33.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2JumpStart.x",	true,	9.0f	},	    // ジャンプの開始 9.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Jump.x",	true,	46.0f	},	        // ジャンプ 23.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2JumpEnd.x",	true,	26.0f	},	    // ジャンプの終了 26.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Run.x",	true,	42.0f	},	        // 走る 21.0
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Slide.x",	true,	72.0f	},	        // 滑る 36.0
	//{ "Character\\Enemy\\Boxer\\animation2\\Boxer2DieRecover.x",	true,	48.0f	},	// 起き上がる 27.0f
};

// コンストラクタ
CBoxer2::CBoxer2()
	: mpRideObject(nullptr)
	, mAttackTime(0)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Boxer2");

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

	// キャラクター押し戻し処理(頭)
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.4f, false, 2.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.04f, 0.1f, 0.0f);

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.5f, false, 4.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.07f, 0.1f, 0.0f);

	// キャラクター押し戻し処理(右手)
	mpColliderSphereHandR = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.27f, false, 3.0f
	);
	mpColliderSphereHandR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHandR->Position(0.0f, -0.1f, 0.0f);

	// キャラクター押し戻し処理(左手)
	mpColliderSphereHandL = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.27f, false, 3.0f
	);
	mpColliderSphereHandL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHandL->Position(0.0f, -0.1f, 0.0f);

	// キャラクター押し戻し処理(右足)
	mpColliderSphereFeetR = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.06f, false, 3.0f
	);
	mpColliderSphereFeetR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeetR->Position(0.0f, 0.03f, 0.0f);

	// キャラクター押し戻し処理(左足)
	mpColliderSphereFeetL = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.06f, false, 3.0f
	);
	mpColliderSphereFeetL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeetL->Position(0.0f, 0.03f, 0.0f);

	// ダメージを受けるコライダーを作成(頭)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.4f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon,ETag::eFlame });
	//ダメージを受けるコライダーを少し上へずらす
	mpDamageColHead->Position(0.04f, 0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.5f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon,ETag::eFlame });
	mpDamageColBody->Position(0.07f, 0.0f, 0.0f);

	// ダメージを受けるコライダーを作成(右手)
	mpDamageColHandR = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.27f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHandR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHandR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHandR->Position(0.0f, -0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(左手)
	mpDamageColHandL = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.27f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHandL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHandL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHandL->Position(0.0f, -0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(右足)
	mpDamageColFeetR = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.1f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColFeetR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeetR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeetR->Position(0.0f, 0.25f, 0.0f);

	// ダメージを受けるコライダーを作成(左足)
	mpDamageColFeetL = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.1f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColFeetL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeetL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeetL->Position(0.0f, 0.25f, 0.0f);

	// ダメージを与えるコライダー(頭)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.4f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.04f, 0.1f, 0.0f);

	// ダメージを与えるコライダー(体)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.5f, false
	);
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });
	mpAttackColBody->Position(0.07f, 0.0f, 0.0f);

	// ダメージを与えるコライダー(右手)
	mpAttackColHandR = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.27f, false
	);
	mpAttackColHandR->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHandR->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHandR->Position(0.0f, -0.1f, 0.0f);

	// ダメージを与えるコライダー(右足)
	mpAttackColFeetR = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.06f, false
	);
	mpAttackColFeetR->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColFeetR->SetCollisionTags({ ETag::ePlayer });
	mpAttackColFeetR->Position(0.0f, 0.03f, 0.0f);

	// ダメージを与えるコライダー(左足)
	mpAttackColFeetL = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.06f, false
	);
	mpAttackColFeetL->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColFeetL->SetCollisionTags({ ETag::ePlayer });
	mpAttackColFeetL->Position(0.0f, 0.03f, 0.0f);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーをボクサーの頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_neck_01");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpDamageColHead->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーをボクサーの体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_spine_02");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);
	mpAttackColBody->SetAttachMtx(bodyMty);

	// 押し戻しコライダーとダメージを受けるコライダーと攻撃コライダーをボクサーの右手の行列にアタッチ
	const CMatrix* rightHandMty = GetFrameMtx("Armature_drill_r");
	mpColliderSphereHandR->SetAttachMtx(rightHandMty);
	mpDamageColHandR->SetAttachMtx(rightHandMty);
	mpAttackColHandR->SetAttachMtx(rightHandMty);

	// 押し戻しコライダーとダメージを受けるコライダーをボクサーの左手の行列にアタッチ
	const CMatrix* leftHandMty = GetFrameMtx("Armature_drill_l");
	mpColliderSphereHandL->SetAttachMtx(leftHandMty);
	mpDamageColHandL->SetAttachMtx(leftHandMty);

	// ダメージを受けるコライダーをボクサーの右足の行列にアタッチ
	const CMatrix* rightFootMty = GetFrameMtx("Armature_thigh_r");
	mpDamageColFeetR->SetAttachMtx(rightFootMty);

	// ダメージを受けるコライダーをボクサーの左足の行列にアタッチ
	const CMatrix* leftFootMty = GetFrameMtx("Armature_thigh_l");
	mpDamageColFeetL->SetAttachMtx(leftFootMty);

	// 押し戻しコライダーと攻撃コライダーをボクサーの右足の行列にアタッチ
	const CMatrix* rightFootMty2 = GetFrameMtx("Armature_foot_r");
	mpColliderSphereFeetR->SetAttachMtx(rightFootMty2);
	mpAttackColFeetR->SetAttachMtx(rightFootMty2);

	// 押し戻しコライダーと攻撃コライダーをボクサーの左足の行列にアタッチ
	const CMatrix* leftFootMty2 = GetFrameMtx("Armature_foot_l");
	mpColliderSphereFeetL->SetAttachMtx(leftFootMty2);
	mpAttackColFeetL->SetAttachMtx(leftFootMty2);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColHead->SetEnable(false);
	mpAttackColBody->SetEnable(false);
	mpAttackColHandR->SetEnable(false);
	mpAttackColFeetR->SetEnable(false);
	mpAttackColFeetL->SetEnable(false);

	const CMatrix* rightHandMty2 = GetFrameMtx("Armature_saw_r");
	mpImpact = new  CImpactEffect
	(
		this, rightHandMty2,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0, 90.f, 0.0f).Matrix()
	);
}

CBoxer2::~CBoxer2()
{
	SAFE_DELETE(mpColliderLine);

	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereHandR);
	SAFE_DELETE(mpColliderSphereHandL);
	SAFE_DELETE(mpColliderSphereFeetR);
	SAFE_DELETE(mpColliderSphereFeetL);

	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColHandR);
	SAFE_DELETE(mpDamageColHandL);
	SAFE_DELETE(mpDamageColFeetR);
	SAFE_DELETE(mpDamageColFeetL);

	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColBody);
	SAFE_DELETE(mpAttackColHandR);
	SAFE_DELETE(mpAttackColFeetR);
	SAFE_DELETE(mpAttackColFeetL);
}

CBoxer2* CBoxer2::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CBoxer2::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 待機状態
void CBoxer2::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// 待機状態2
void CBoxer2::UpdateIdle2()
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
void CBoxer2::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	if (mAnimationFrame >= 0.0f && mAnimationFrame < 5.0f)
	{
		AttackStart();
	}

	if (mAnimationFrame >= 8.0f)
	{
		if (!mpImpact->IsThrowing())
		{
			mpImpact->Start();
		}
	}
	if (IsAnimationFinished())
	{
		// 攻撃終了待ち状態へ移行
		mState = EState::eAttackWait;
	}
}

// 攻撃2
void CBoxer2::UpdateAttack2()
{
	ChangeAnimation(EAnimType::eAttack2);
	if (mAnimationFrame >= 0.0f && mAnimationFrame < 5.0f)
	{
		AttackStart();
	}
	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= 0.0f && vectorp <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * 0.2f;
	}
	if (IsAnimationFinished())
	{
		// 攻撃2終了待ち状態へ移行
		mState = EState::eAttackWait;
	}
}

// 攻撃終了待ち
void CBoxer2::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpImpact->Stop();
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);

		// 連続攻撃するかどうか
		bool continuousz = false;
		// 確率
		int percent = Math::Rand(0, 3);
		if (percent == 1) continuousz = true;
		if (continuousz)
		{
			mState = EState::eAttack;
		}
		else
		{
			mState = EState::eIdle2;
		}
	}
}

// ジャンプ開始
void CBoxer2::UpdateJumpStart()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eJumpStart);
	if (IsAnimationFinished())
	{
		mState = EState::eJump;

		mMoveSpeed += CVector(0.0f, JUMP_SPEED, 0.0f);
		mIsGrounded = false;
	}
}

// ジャンプ中
void CBoxer2::UpdateJump()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eJump);
	if (mMoveSpeed.Y() <= 0.0f || IsAnimationFinished())
	{
		mState = EState::eJumpEnd;
	}
}

// ジャンプ終了
void CBoxer2::UpdateJumpEnd()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// ジャンプアニメーションが待機状態へ戻す
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// ヒット
void CBoxer2::UpdateHit()
{
	mpImpact->Stop();
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
			mState = EState::eIdle2;
			ChangeAnimation(EAnimType::eIdle2);
		}
	}
}

// 防御
void CBoxer2::UpdateDefense()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefense);

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
		if (mDefenseTime >= 300)
		{
			mState = EState::eAttack;
		}
		else
		{
			mState = EState::eDefense;
		}
	}
}

// 防御中のヒット
void CBoxer2::UpdateDefenseHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefenseHit);
	if (IsAnimationFinished())
	{
		mState = EState::eDefense;
	}
}

// 死ぬ
void CBoxer2::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::BoxerDeath();
	}
}

// めまい(混乱)
void CBoxer2::UpdateDizzy()
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
void CBoxer2::UpdateRun()
{
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// 追跡をやめて止まる
	if (vectorp <= STOP_RANGE && vectorp >= 33.0f)
	{
		//mMoveSpeed.X(0.0f);
		//mMoveSpeed.Z(0.0f);

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
	// 範囲内の時、移動し追跡する
	else if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
	}
	// 追跡が止まった時、待機モーションへ
	if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		if (mState != EState::eAttack2)
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			ChangeAnimation(EAnimType::eIdle2);
			mState = EState::eIdle2;
		}
	}
}

// 滑る
void CBoxer2::UpdateSlide()
{
	ChangeAnimation(EAnimType::eSlide);
	if (mAnimationFrame >= 0.0f && mAnimationFrame < 5.0f)
	{
		AttackStart();
	}
	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= 0.0f && vectorp <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
	}
	if (IsAnimationFinished())
	{
		// 滑る終了待ち状態へ移行
		mState = EState::eAttackWait;
	}
}

// 更新処理
void CBoxer2::Update()
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
		// 攻撃終了待ち
	case EState::eAttackWait:
		UpdateAttackWait();
		break;
		// ジャンプ開始
	case EState::eJumpStart:
		UpdateJumpStart();
		break;
		// ジャンプ中
	case EState::eJump:
		UpdateJump();
		break;
		// ジャンプ終了
	case EState::eJumpEnd:
		UpdateJumpEnd();
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
		// 滑る
	case EState::eSlide:
		UpdateSlide();
		break;
	}

	// HPゲージの座標を更新(敵の座標の少し上の座標)
	CVector gaugePos = Position() + CVector(0.0f, 30.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eAttack && mState != EState::eAttack2
		&& mState != EState::eAttackWait && mState != EState::eHit && mState != EState::eDefense && mState != EState::eDefenseHit
		&& mState != EState::eDie && mState != EState::eDizzy && mState != EState::eJumpStart && mState != EState::eJump
		&& mState != EState::eJumpEnd && mState != EState::eRun && mState != EState::eSlide)
	{
		mState = EState::eIdle2;
	}

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle)
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
			// 防御
			bool Defense = false;
			// ジャンプ
			bool Jump = false;
			// 滑る
			bool Slide = false;

			// 確率を最小に2最大4
			int probability2 = Math::Rand(2, 4);
			// 確率を最小に5最大7
			int probability3 = Math::Rand(5, 7);
			// 確率を最小に8最大10
			int probability4 = Math::Rand(8, 10);
			//確率を最小に11最大13
			int probability5 = Math::Rand(11, 13);

			if (probability2 == 2)Attack2 = true;
			if (probability3 == 5)Defense = true;
			if (probability4 == 8)Jump = true;
			if (probability5 == 11)Slide = true;
			if (Attack2)
			{
				mState = EState::eAttack2;
			}
			else if (Defense)
			{
				mState = EState::eDefense;
			}
			else if (Jump)
			{
				mState = EState::eJump;
			}
			else if (Slide)
			{
				mState = EState::eSlide;
			}
			else
			{
				mState = EState::eAttack;
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eSlide
			|| mState == EState::eJumpStart || mState == EState::eDefense)
		{
			mAttackTime = 0;
		}
	}

	// 防御時間計測
	if (mState == EState::eDefense || mState == EState::eDefenseHit)
	{
		mDefenseTime++;
	}
	else
	{
		mDefenseTime = 0;
	}

	//CDebugPrint::Print(" HP: %f\n", boxer->Position().Y());

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE || mState == EState::eAttack2 || mState == EState::eSlide)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (Position().Y() >= 0.1f)
	{
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	}

	// キャラクターの更新
	CXCharacter::Update();

	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereHandR->Update();
	mpColliderSphereHandL->Update();
	mpColliderSphereFeetR->Update();
	mpColliderSphereFeetL->Update();

	mpDamageColHead->Update();
	mpDamageColBody->Update();
	mpDamageColHandR->Update();
	mpDamageColHandL->Update();
	mpDamageColFeetR->Update();
	mpDamageColFeetL->Update();

	mpAttackColHead->Update();
	mpAttackColBody->Update();
	mpAttackColHandR->Update();
	mpAttackColFeetR->Update();
	mpAttackColFeetL->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CBoxer2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColHead || self == mpAttackColBody || self == mpAttackColHandR || self == mpAttackColFeetR
		|| self == mpAttackColFeetL && mState != EState::eIdle && mState != EState::eIdle2)
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
			//mMoveSpeed.Y(0.0f);

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// キャラクター同士の衝突処理
	else if (self == mpColliderSphereHead || self == mpColliderSphereBody || self == mpColliderSphereHandR
		|| self == mpColliderSphereHandL || self == mpColliderSphereFeetR || self == mpColliderSphereFeetL)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CBoxer2::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackColHead->SetEnable(true);
	mpAttackColBody->SetEnable(true);
	mpAttackColHandR->SetEnable(true);
	mpAttackColFeetR->SetEnable(true);
	mpAttackColFeetL->SetEnable(true);
}

// 攻撃終了
void CBoxer2::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColHead->SetEnable(false);
	mpAttackColBody->SetEnable(false);
	mpAttackColHandR->SetEnable(false);
	mpAttackColFeetR->SetEnable(false);
	mpAttackColFeetL->SetEnable(false);
}

// 描画
void CBoxer2::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CBoxer2::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CBoxer2::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY9_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CBoxer2::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		if (mState == EState::eDefense)
		{
			mState = EState::eDefenseHit;
		}
		else
		{
			mState = EState::eHit;
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

// 防御力の強化割合を取得
float CBoxer2::GetDefBuff(const CVector& attackDir)const
{
	// 防御状態であれば、防御2倍
	if (mState == EState::eDefense) return 2.0f;

	// 通常時の防御の割合
	return mBaseDefenseBuffRatio;
}

// 死亡処理
void CBoxer2::Death()
{
	// 死亡状態へ移行
	mState = EState::eDie;
}