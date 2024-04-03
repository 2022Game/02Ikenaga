#include "CDragon.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "Maths.h"
#include "CFlamethrower.h"
#include "CRoarEffect.h"

// ドラゴンのインスタンス
CDragon* CDragon::spInstance = nullptr;

#define ENEMY_HEIGHT -3.0f
#define WITHIN_RANGE 40.0f       // 範囲内
#define MOVE_SPEED 0.08f         // 移動速度
#define GRAVITY 0.0625f          // 重力
#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 24.0f         // 追跡を辞める範囲
#define ROTATE_RANGE  50.0f      // 回転する範囲

// ドラゴンのアニメーションデータのテーブル
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// Tポーズ
	{ "Character\\Enemy\\Dragon\\animation\\DragonSleep.x",	true,	162.0f	},	    // 寝る 81.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonIdle.x",	true,	82.0f	},	    // 待機 41.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonIdle2.x",	true,	202.0f	},	    // 待機 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonAttack.x",	true,	162.0f	},	    // 攻撃 81.0f
    { "Character\\Enemy\\Dragon\\animation\\DragonAttack2.x",	true,	182.0f	},	    // 攻撃2 91.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonAttack3.x",	true,	72.0f	},	    // 攻撃3 36.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyFlame.x",	true,	182.0f	},	    // フライフレーム 91.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonGetHit.x",	true,	82.0f	},	    // ヒット 41.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonDefend.x",	true,	122.0f	},	    // 防御 61.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonDie.x",	true,	150.0f	},	        // 死ぬ 65.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonScream.x",	true,	202.0f	},	    // 雄叫び 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonTakeOff.x",	true,	242.0f	},	    // 飛行開始する前 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyIdle.x",	true,	62.0f	},	    // フライアイドル 31.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonLand.x",	true,	242.0f	},	        // 着地 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonRun.x",	true,	42.0f	},	        // 走る 21.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyForward.x",	true,	62.0f	},	// フライフォワード 31.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyGlide.x",	true,	51.0f	},	// フライグライド 51.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonWalk.x",	true,	82.0f	},	        // 歩く 41.0f
};

// コンストラクタ
CDragon::CDragon()
	: mpRideObject(nullptr)
	,mFlyingTime(0)
	,mDefenseTime(0)
	, mAttackTime(0)
	,mFlyingAttackTime(0)
{
	//インスタンスの設定
	spInstance = this;

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

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle2);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	//mpColliderLine->Position(0.0f, 0.0f,0.0f);

	//// キャラクター押し戻し処理
	//mpColliderSphere = new CColliderSphere
	//(
	//	this, ELayer::eEnemy,
	//	0.3f, false, 5.0f
	//);
	//mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	//mpColliderSphere->Position(0.0f, 0.2f, 0.0f);

	// ダメージを受けるコライダーを作成(頭)
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.6f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol->Position(0.1f, 0.0f, 0.0f);

	// ダメージを受けるコライダーを作成(口)
	mpDamageCol2 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.4f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol2->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol2->Position(-0.2f, 0.1f, 0.15f);

	// ダメージを受けるコライダーを作成(口の先端)
	mpDamageCol3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.3f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol3->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol3->Position(-0.1f, 0.15f, 0.1f);

	// ダメージを受けるコライダーを作成(首)
	mpDamageCol4 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.6f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol4->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol4->Position(0.1f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを作成(首2)
	mpDamageCol5 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.6f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol5->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol5->Position(0.1f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを作成(首3)
	mpDamageCol6 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.7f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol6->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol6->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol6->Position(0.1f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを作成(胸)
	mpDamageCol7 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.9f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol7->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol7->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol7->Position(-0.2f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを作成(脊椎)
	mpDamageCol8 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 1.0f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol8->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol8->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol8->Position(-0.15f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを作成(脊椎2)
	mpDamageCol9 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.8f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol9->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol9->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol9->Position(0.0f, -0.4f, 0.0f);

	// ダメージを受けるコライダーを作成(つなぎ)
	mpDamageCol10 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.6f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol10->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol10->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol10->Position(0.0f, -0.4f, 0.0f);

	// ダメージを受けるコライダーを作成(つなぎ2)
	mpDamageCol11 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.4f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol11->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol11->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol11->Position(0.1f, 0.0f, -0.1f);

	// ダメージを受けるコライダーを作成(つなぎ3)
	mpDamageCol12 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.35f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol12->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol12->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol12->Position(0.1f, 0.0f, -0.1f);

	// ダメージを受けるコライダーを作成(つなぎ4)
	mpDamageCol13 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.25f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol13->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol13->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol13->Position(0.2f, 0.0f, -0.1f);

	// ダメージを受けるコライダーを作成(つなぎ5)
	mpDamageCol14 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.2f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol14->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol14->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol14->Position(0.2f, 0.1f, 0.0f);

	// カプセルコライダー
	// ダメージを受けるコライダーを作成(前の左足)
	mpDamageCol15 = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(-2.0f,1.0f, 0.0f),CVector(2.0f, 1.0f, 0.0f),
		2.0f,false,1.0f
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol15->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol15->SetCollisionTags({ ETag::eWeapon });
	
	//mpDamageCol15->Position(0.1f, 0.0f, 0.15f);

	// ダメージを受けるコライダーを作成(前の左足2)
	mpDamageCol16 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.3f,false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol16->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol16->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol16->Position(0.1f, 0.0f, 0.15f);

	// ダメージを与えるコライダー(頭)
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.6f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.1f, 0.0f, 0.0f);

	// ダメージを受けるコライダーと攻撃コライダーをドラゴンの頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpDamageCol->SetAttachMtx(headMty);
	mpAttackCol->SetAttachMtx(headMty);

	// ダメージを受けるコライダーと攻撃コライダーをドラゴンの口の行列にアタッチ
	const CMatrix* mouthMty = GetFrameMtx("Armature_UpperMouth01");
	mpDamageCol2->SetAttachMtx(mouthMty);

	// ダメージを受けるコライダーと攻撃コライダーをドラゴンの口の先端の行列にアタッチ
	const CMatrix* mouthMty2 = GetFrameMtx("Armature_UpperMouth02");
	mpDamageCol3->SetAttachMtx(mouthMty2);

	// ダメージを受けるコライダーをドラゴンの首の行列にアタッチ
	const CMatrix* neckMty = GetFrameMtx("Armature_Neck03");
	mpDamageCol4->SetAttachMtx(neckMty);

	// ダメージを受けるコライダーをドラゴンの首2の行列にアタッチ
	const CMatrix* neckMty2 = GetFrameMtx("Armature_Neck02");
	mpDamageCol5->SetAttachMtx(neckMty2);

	// ダメージを受けるコライダーをドラゴンの首3の行列にアタッチ
	const CMatrix* neckMty3 = GetFrameMtx("Armature_Neck01");
	mpDamageCol6->SetAttachMtx(neckMty3);

	// ダメージを受けるコライダーをドラゴンの胸の行列にアタッチ
	const CMatrix* chestMty = GetFrameMtx("Armature_Chest");
	mpDamageCol7->SetAttachMtx(chestMty);

	// ダメージを受けるコライダーをドラゴンの脊椎の行列にアタッチ
	const CMatrix* spineMty = GetFrameMtx("Armature_Spine02");
	mpDamageCol8->SetAttachMtx(spineMty);

	// ダメージを受けるコライダーをドラゴンの脊椎2の行列にアタッチ
	const CMatrix* spineMty2 = GetFrameMtx("Armature_Spine01");
	mpDamageCol9->SetAttachMtx(spineMty2);

	// ダメージを受けるコライダーをドラゴンのつなぎの行列にアタッチ
	const CMatrix* jointMty = GetFrameMtx("Armature_joint19");
	mpDamageCol10->SetAttachMtx(jointMty);

	// ダメージを受けるコライダーをドラゴンのつなぎ2の行列にアタッチ
	const CMatrix* jointMty2 = GetFrameMtx("Armature_joint20");
	mpDamageCol11->SetAttachMtx(jointMty2);

	// ダメージを受けるコライダーをドラゴンのつなぎ3の行列にアタッチ
	const CMatrix* jointMty3 = GetFrameMtx("Armature_joint21");
	mpDamageCol12->SetAttachMtx(jointMty3);

	// ダメージを受けるコライダーをドラゴンのつなぎ4の行列にアタッチ
	const CMatrix* jointMty4 = GetFrameMtx("Armature_joint22");
	mpDamageCol13->SetAttachMtx(jointMty4);

	// ダメージを受けるコライダーをドラゴンのつなぎ5の行列にアタッチ
	const CMatrix* jointMty5 = GetFrameMtx("Armature_joint23");
	mpDamageCol14->SetAttachMtx(jointMty5);

	// ダメージを受けるコライダーをドラゴンの前の左足の行列にアタッチ
	const CMatrix* leftFootMty = GetFrameMtx("Armature_Hand_L");
	mpDamageCol15->SetAttachMtx(leftFootMty);

	// ダメージを受けるコライダーをドラゴンの前の左足2の行列にアタッチ
	const CMatrix* leftFootMty2 = GetFrameMtx("Armature_Middle02_L");
	mpDamageCol16->SetAttachMtx(leftFootMty2);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackCol->SetEnable(false);

	const CMatrix* mtx = GetFrameMtx("Armature_Tongue01");
	mpFlamethrower = new CFlamethrower
	(
		this, mtx,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0,-90.f,0.0f).Matrix()
	); 

	mpRoar = new CRoarEffect
	(
		this,nullptr,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0, 0.f, 0.0f).Matrix()
	);
}

CDragon::~CDragon()
{
	SAFE_DELETE(mpColliderLine);
	//SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpDamageCol2);
	SAFE_DELETE(mpDamageCol3);
	SAFE_DELETE(mpDamageCol4);
	SAFE_DELETE(mpDamageCol5);
	SAFE_DELETE(mpDamageCol6);
	SAFE_DELETE(mpDamageCol7);
	SAFE_DELETE(mpDamageCol8);
	SAFE_DELETE(mpDamageCol9);
	SAFE_DELETE(mpDamageCol10);
	SAFE_DELETE(mpDamageCol11);
	SAFE_DELETE(mpDamageCol12);
	SAFE_DELETE(mpDamageCol13);
	SAFE_DELETE(mpDamageCol14);
	SAFE_DELETE(mpDamageCol15);
	SAFE_DELETE(mpDamageCol16);

	SAFE_DELETE(mpAttackCol);
}

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
}

// 待機状態
void CDragon::UpdateIdle()
{ 
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// 待機状態2
void CDragon::UpdateIdle2()
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

// 待機状態3
void CDragon::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// 攻撃
void CDragon::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
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
	if (mAnimationFrame >= 35.0f)
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
	}
	if (mAnimationFrame >=140.0f)
	{
		mpFlamethrower->Stop();
	}
	if (IsAnimationFinished())
	{
		//mpFlamethrower->Stop();
		// 攻撃終了待ち状態へ移行
		mState = EState::eAttackWait;
	}
}

// 攻撃2
void CDragon::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// 攻撃2終了待ち状態へ移行
	mState = EState::eAttackWait;
}


// 攻撃3
void CDragon::UpdateAttack3()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack3);
	AttackStart();
	// 攻撃3終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃終了待ち
void CDragon::UpdateAttackWait()
{
	mpFlamethrower->Stop();
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle2;
	}
}

// 飛行時の攻撃
void CDragon::UpdateFlyingAttack()
{
	ChangeAnimation(EAnimType::eFlyingAttack);
	if (mAnimationFrame >= 30.0f)
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
	}
	if (IsAnimationFinished())
	{
		mpFlamethrower->Stop();
		// 飛行時の攻撃終了待ち状態へ移行
		mState = EState::eFlyingAttackWait;
	}
}

// 飛行時の攻撃終了待ち
void CDragon::UpdateFlyingAttackWait()
{
	mpFlamethrower->Stop();
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eFlyingIdle;
	}
}

// ヒット
void CDragon::UpdateHit()
{
	mpFlamethrower->Stop();
	// ヒットアニメーションを開始
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		mState = EState::eIdle2;
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// 防御
void CDragon::UpdateDefense()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefense);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// 死ぬ
void CDragon::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::Death();
	}
}

// 雄叫び
void CDragon::UpdateRoar()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eRoar);
	if (mAnimationFrame >= 1.0f)
	{
		if (!mpRoar->IsThrowing())
		{
			mpRoar->Start();
		}
	}
	if (IsAnimationFinished())
	{
		mpRoar->Stop();
		mState = EState::eFlyingStart;
	}
}

// 飛行開始
void CDragon::UpdateFlyingStart()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eFlyingStart);
	if (IsAnimationFinished())
	{
		mState = EState::eFlyingIdle;
	}
}

// 飛行時の待機
void CDragon::UpdateFlyingIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eFlyingIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eFlyingIdle;
	}
}

// 飛行終了
void CDragon::UpdateFlyingEnd()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eFlyingEnd);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// 移動
void CDragon::UpdateRun()
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
void CDragon::Update()
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
		// 攻撃3
	case EState::eAttack3:
		UpdateAttack3();
		break;
		// 攻撃終了待ち
	case EState::eAttackWait:
		UpdateAttackWait();
		break;
		// 飛行中の攻撃
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
		// 飛行終了
	case EState::eFlyingEnd:
		UpdateFlyingEnd();
		break;
		// 移動
	case EState::eRun:
		UpdateRun();
		break;
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eIdle3
		&& mState != EState::eAttack && mState != EState::eAttack2 && mState != EState::eAttack3 && mState != EState::eAttackWait
		&& mState != EState::eHit && mState != EState::eDefense && mState != EState::eDie && mState != EState::eRoar
		&& mState != EState::eFlyingStart && mState != EState::eFlyingIdle && mState != EState::eFlyingAttack && mState != EState::eFlyingAttackWait
		&& mState != EState::eFlyingEnd && mState != EState::eRun)
	{
		mState = EState::eIdle2;
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
			// 攻撃
			bool Attack = false;
			// 防御
			bool Defense = false;
			// 確率を最小に2最大5
			int probability2 = Math::Rand(2, 4);
			// 確率を最小に5最大7
			int probability3 = Math::Rand(5, 7);
			// 確率を最小に8最大10
			int probability4 = Math::Rand(8, 10);

			if (probability2 == 2)Attack2 = true;
			if (probability3 == 5)Attack = true;
			if (probability4 == 8)Defense = true;
			if (Attack2)
			{
				mState = EState::eAttack2;
			}
			else if (Attack)
			{
				mState = EState::eAttack;
			}
			else if (Defense)
			{
				mState = EState::eDefense;
			}
			else
			{
				mState = EState::eAttack3;
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3
			|| mState == EState::eDefense || mState == EState::eFlyingIdle)
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
		&& mState != EState::eFlyingAttackWait && mState != EState::eFlyingEnd)
	{
		if (mCharaStatus.hp <= mCharaMaxStatus.hp * 0.5 && mFlyingTime ==0)
		{
			mState = EState::eRoar;
		}
		if (mCharaStatus.hp <= mCharaMaxStatus.hp * 0.5 && mFlyingTime >=400)
		{
			mState = EState::eFlyingStart;
		}
	}

	if (mState == EState::eFlyingIdle)
	{
		mFlyingTime++;
		mFlyingAttackTime++;

		if (mFlyingAttackTime > 400)
		{
			mState = EState::eFlyingAttack;
		}
	}
	if (mState == EState::eFlyingAttack || mState == EState::eFlyingEnd)
	{
		mFlyingAttackTime = 0;
	}

	if (mFlyingTime >= 600)
	{
		mState = EState::eFlyingEnd;
	}
	if (mState == EState::eFlyingEnd || mState == EState::eFlyingStart && mFlyingTime >=400)
	{
		mFlyingTime = 1;
	}

	// HPが0以下になったら、
	if (mCharaStatus.hp <= 0)
	{
		// 死亡処理
		Death();
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	CDebugPrint::Print(" (地)攻撃時間: %d\n", mAttackTime);
	CDebugPrint::Print(" 攻撃時間: %d\n",mFlyingAttackTime);
	CDebugPrint::Print(" 空中時間: %d\n", mFlyingTime);
	CDebugPrint::Print(" HP: %d\n", mCharaStatus.hp);

	// キャラクターの更新
	CXCharacter::Update();

	mpDamageCol->Update();
	mpDamageCol2->Update();
	mpDamageCol3->Update();
	mpDamageCol4->Update();
	mpDamageCol5->Update();
	mpDamageCol6->Update();
	mpDamageCol7->Update();
	mpDamageCol8->Update();
	mpDamageCol9->Update();
	mpDamageCol10->Update();
	mpDamageCol11->Update();
	mpDamageCol12->Update();
	mpDamageCol13->Update();
	mpDamageCol14->Update();
	mpDamageCol15->Update();
	mpDamageCol16->Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	if (CInput::PushKey(('Q')))
	{
		mState = EState::eAttack;
	}

	// 「E」キーで炎の発射をオンオフする
	if (CInput::PushKey('Z'))
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
		else
		{
			mpFlamethrower->Stop();
		}
	}
	if (CInput::PushKey('C'))
	{
		if (!mpRoar->IsThrowing())
		{
			mpRoar->Start();
		}
		else
		{
			mpRoar->Stop();
		}
	}
}

// 衝突処理
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackCol && mState != EState::eIdle && mState != EState::eIdle2 &&
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
	if (self == mpColliderLine)
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
	/*else if (self == mpColliderSphere)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}*/
}

// 攻撃開始
void CDragon::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpAttackCol->SetEnable(true);
}

// 攻撃終了
void CDragon::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
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
	mCharaMaxStatus = ENEMY4_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	//mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	//mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CDragon::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		mpFlamethrower->Stop();
		mState = EState::eHit;
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

	// 通常時の防御の割合
	return mBaseDefenseBuffRatio;
}

// 死亡処理
void CDragon::Death()
{
	// 死亡状態へ移行
	mState = EState::eDie;
}