#include "CDragon.h"
#include "CPlayer.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "Maths.h"
#include "CFlamethrower.h"
#include "CRoarEffect.h"

// ドラゴンのインスタンス
CDragon* CDragon::spInstance = nullptr;

#define ENEMY_HEIGHT -2.9f
#define WITHIN_RANGE 40.0f       // 範囲内
#define MOVE_SPEED 0.08f         // 移動速度
#define GRAVITY 0.0625f          // 重力
#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 24.0f         // 追跡を辞める範囲
#define ROTATE_RANGE  50.0f      // 回転する範囲

// ドラゴンのアニメーションデータのテーブル
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{ "",										                true,	0.0f },// Tポーズ
	{ "Character\\Enemy\\Dragon\\animation\\DragonSleep.x",	    true,	81.0f,  0.5f },	    // 寝る 81.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonIdle.x",	    true,	41.0f,  0.5f },	    // 待機 41.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonIdle2.x",	    true,	101.0f, 0.5f },	    // 待機 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonAttack.x",	false,	81.0f,  0.5f },	    // 攻撃 81.0f
    { "Character\\Enemy\\Dragon\\animation\\DragonAttack2.x",	false,	91.0f,  0.5f },	    // 攻撃2 91.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonAttack3.x",	false,	36.0f,	0.5f },	    // 攻撃3 36.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyFlame.x",	true,	91.0f,	0.5f },	    // フライフレーム 91.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonGetHit.x",	true,	41.0f,	0.5f },	    // ヒット 41.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonDefend.x",	true,	61.0f,	0.5f },	    // 防御 61.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonDie.x",	    true,	65.0f,	0.5f },	    // 死ぬ 65.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonScream.x",	true,	101.0f,	0.5f },	    // 雄叫び 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonTakeOff.x",	true,	121.0f, 0.5f },	    // 飛行開始する前 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyIdle.x",	true,	31.0f,	0.5f },	    // フライアイドル 31.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonLand.x",	    true,	121.0f,	0.5f },	    // 着地 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonRun.x",	    true,	21.0f,	0.5f },	    // 走る 21.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyForward.x",	true,	62.0f	},	// フライフォワード 31.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyGlide.x",	true,	51.0f	},	// フライグライド 51.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonWalk.x",	true,	82.0f	},	        // 歩く 41.0f
};

// コンストラクタ
CDragon::CDragon()
    : mState(EState::eIdle2)
	, mpRideObject(nullptr)
	, mFlyingTime(0)
	, mDefenseTime(0)
	, mAttackTime(0)
	, mFlyingAttackTime(0)
	, mStateStep(0)
	, mIsGrounded(false)
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
	mpColliderLine->Position(35.0f, 0.0f,-70.0f);

	// キャラクター押し戻し処理(前の左足)
	mpColliderSphereFeet = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.7f, false, 20.0f
	);
	mpColliderSphereFeet->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeet->Position(0.0f, 0.0f, 0.2f);

	// キャラクター押し戻し処理(前の右足)
	mpColliderSphereFeet2 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.7f, false, 20.0f
	);
	mpColliderSphereFeet2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeet2->Position(0.0f, 0.0f, 0.2f);

	// ダメージを受けるコライダーを作成(頭)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.6f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHead->Position(0.1f, 0.0f, 0.0f);

	// ダメージを受けるコライダーを作成(口)
	mpDamageColMouth = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.5f, false
	);
	mpDamageColMouth->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColMouth->SetCollisionTags({ ETag::eWeapon });
	mpDamageColMouth->Position(-0.2f, 0.1f, 0.15f);

	// ダメージを受けるコライダーを作成(口の先端)
	mpDamageColTipMouth = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.4f, false
	);
	mpDamageColTipMouth->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTipMouth->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTipMouth->Position(-0.1f, 0.15f, 0.1f);

	// ダメージを受けるコライダーを作成(首)
	mpDamageColNeck = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.6f, false
	);
	mpDamageColNeck->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColNeck->SetCollisionTags({ ETag::eWeapon });
	mpDamageColNeck->Position(0.1f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを作成(胸)
	mpDamageColChest = new CColliderSphere
	(
		this, ELayer::eDamageCol, 3.0f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColChest->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColChest->SetCollisionTags({ ETag::eWeapon });
	mpDamageColChest->Position(-0.2f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを作成(前の左足)
	mpDamageColFeet = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.8f,false,1.0f
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColFeet->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet->Position(0.0f, 0.0f, 0.2f);

	// ダメージを受けるコライダーを作成(前の右足)
	mpDamageColFeet2 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.8f, false, 1.0f
	);
	mpDamageColFeet2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet2->Position(0.0f, 0.0f, 0.2f);

	// ダメージを受けるコライダーを作成(後ろの左足)
	mpDamageColFeet3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.8f, false, 1.0f
	);
	mpDamageColFeet3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet3->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet3->Position(0.0f, 0.0f, 0.2f);

	// ダメージを受けるコライダーを作成(後ろの右足)
	mpDamageColFeet4= new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.8f, false, 1.0f
	);
	mpDamageColFeet4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet4->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet4->Position(0.0f, 0.0f, 0.2f);

	// ダメージを与えるコライダー(頭)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.6f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.1f, 0.0f, 0.0f);

	// ダメージを与えるコライダー(口)
	mpAttackColMouth = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.5f, false
	);
	mpAttackColMouth->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColMouth->SetCollisionTags({ ETag::ePlayer });
	mpAttackColMouth->Position(-0.2f, 0.1f, 0.15f);

	// ダメージを与えるコライダー(口の先端)
	mpAttackColTipMouth = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.4f, false
	);
	mpAttackColTipMouth->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTipMouth->SetCollisionTags({ ETag::ePlayer });
	mpAttackColTipMouth->Position(-0.1f, 0.15f, 0.1f);

	// ダメージを受けるコライダーと攻撃コライダーをドラゴンの頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpDamageColHead->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// ダメージを受けるコライダーと攻撃コライダーをドラゴンの口の行列にアタッチ
	const CMatrix* mouthMty = GetFrameMtx("Armature_UpperMouth01");
	mpDamageColMouth->SetAttachMtx(mouthMty);
	mpAttackColMouth->SetAttachMtx(mouthMty);

	// ダメージを受けるコライダーと攻撃コライダーをドラゴンの口の先端の行列にアタッチ
	const CMatrix* mouthMty2 = GetFrameMtx("Armature_UpperMouth02");
	mpDamageColTipMouth->SetAttachMtx(mouthMty2);
	mpAttackColTipMouth->SetAttachMtx(mouthMty2);

	// ダメージを受けるコライダーをドラゴンの首の行列にアタッチ
	const CMatrix* neckMty = GetFrameMtx("Armature_Neck03");
	mpDamageColNeck->SetAttachMtx(neckMty);

	// ダメージを受けるコライダーをドラゴンの胸の行列にアタッチ
	const CMatrix* chestMty = GetFrameMtx("Armature_Chest");
	mpDamageColChest->SetAttachMtx(chestMty);

	// ダメージを受けるコライダーをドラゴンの手の行列にアタッチ
	const CMatrix* handMty = GetFrameMtx("Armature_Hand_L");
	//mpColliderLine->SetAttachMtx(handMty);

	// ダメージを受けるコライダーをドラゴンの脊椎2の行列にアタッチ
	const CMatrix* spineMty2 = GetFrameMtx("Armature_Spine01");

	// ダメージを受けるコライダーをドラゴンの首2の行列にアタッチ
	const CMatrix* neckMty2 = GetFrameMtx("Armature_Neck02");

	// ダメージを受けるコライダーをドラゴンの首3の行列にアタッチ
	const CMatrix* neckMty3 = GetFrameMtx("Armature_Neck01");

	// ダメージを受けるコライダーをドラゴンの脊椎の行列にアタッチ
	const CMatrix* spineMty = GetFrameMtx("Armature_Spine02");

	// ダメージを受けるコライダーをドラゴンのつなぎの行列にアタッチ
	const CMatrix* jointMty = GetFrameMtx("Armature_joint19");
	
	// ダメージを受けるコライダーをドラゴンのつなぎ2の行列にアタッチ
	const CMatrix* jointMty2 = GetFrameMtx("Armature_joint20");

	// ダメージを受けるコライダーをドラゴンのつなぎ3の行列にアタッチ
	const CMatrix* jointMty3 = GetFrameMtx("Armature_joint21");

	// ダメージを受けるコライダーをドラゴンのつなぎ4の行列にアタッチ
	const CMatrix* jointMty4 = GetFrameMtx("Armature_joint22");

	// ダメージを受けるコライダーをドラゴンのつなぎ5の行列にアタッチ
	const CMatrix* jointMty5 = GetFrameMtx("Armature_joint23");

	// 押し戻しコライダーとダメージを受けるコライダーをドラゴンの前の左足の行列にアタッチ
	const CMatrix* leftFootMty = GetFrameMtx("Armature_Middle01_L");
	mpColliderSphereFeet->SetAttachMtx(leftFootMty);
	mpDamageColFeet->SetAttachMtx(leftFootMty);

	// 押し戻しコライダーとダメージを受けるコライダーをドラゴンの前の右足の行列にアタッチ
	const CMatrix* rightFootMty = GetFrameMtx("Armature_Middle01_R");
	mpColliderSphereFeet2->SetAttachMtx(rightFootMty);
	mpDamageColFeet2->SetAttachMtx(rightFootMty);

	// 押し戻しコライダーとダメージを受けるコライダーをドラゴンの後ろの左足の行列にアタッチ
	const CMatrix* leftFootMty2 = GetFrameMtx("Armature_MiddleToe01_L");
	mpDamageColFeet3->SetAttachMtx(leftFootMty2);

	// 押し戻しコライダーとダメージを受けるコライダーをドラゴンの後ろの右足の行列にアタッチ
	const CMatrix* rightFootMty2 = GetFrameMtx("Armature_MiddleToe01_R");
	mpDamageColFeet4->SetAttachMtx(rightFootMty2);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColHead->SetEnable(false);
	mpAttackColMouth->SetEnable(false);
	mpAttackColTipMouth->SetEnable(false);

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
	// キャラ押し戻しコライダーを削除
	SAFE_DELETE(mpColliderSphereFeet);
	SAFE_DELETE(mpColliderSphereFeet2);
	// ダメージを受けるコライダーを削除
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColMouth);
	SAFE_DELETE(mpDamageColTipMouth);
	SAFE_DELETE(mpDamageColNeck);
	SAFE_DELETE(mpDamageColChest);
	SAFE_DELETE(mpDamageColFeet);
	SAFE_DELETE(mpDamageColFeet2);
	SAFE_DELETE(mpDamageColFeet3);
	SAFE_DELETE(mpDamageColFeet4);
	// 攻撃コライダーを削除
	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColMouth);
	SAFE_DELETE(mpAttackColTipMouth);
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

// 状態の切り替え
void CDragon::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// 待機状態
void CDragon::UpdateIdle()
{ 
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
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
		ChangeState(EState::eRun);
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle2);
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle2);
		}
	}
}

// 待機状態3
void CDragon::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// 攻撃
void CDragon::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
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

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
	// ステップ0 : 攻撃アニメーション開始
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
	// ステップ2 : 火炎放射終了待ち
	case 2:
		if (mAnimationFrame >= 70.0f)
		{
			mpFlamethrower->Stop();
			SetAnimationSpeed(0.5f);
			// 攻撃終了待ち状態へ移行
			ChangeState(EState::eAttackWait);
		}
		break;
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
	ChangeState(EState::eAttackWait);
}


// 攻撃3
void CDragon::UpdateAttack3()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack3);
	AttackStart();
	// 攻撃3終了待ち状態へ移行
	ChangeState(EState::eAttackWait);
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
	ChangeAnimation(EAnimType::eFlyingAttack);
	if (mAnimationFrame >= 35.0f)
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
	}
	if (mAnimationFrame >= 140.0f)
	{
		mpFlamethrower->Stop();
	}
	if (IsAnimationFinished())
	{
		mpFlamethrower->Stop();
		// 飛行時の攻撃終了待ち状態へ移行
		ChangeState(EState::eFlyingAttackWait);
	}
}

// 飛行中の攻撃終了待ち
void CDragon::UpdateFlyingAttackWait()
{
	mpFlamethrower->Stop();
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eFlyingIdle);
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
		ChangeState(EState::eIdle2);
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
		ChangeState(EState::eIdle2);
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
		// エネミーの死亡処理へ
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
		ChangeState(EState::eFlyingStart);
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
		ChangeState(EState::eFlyingIdle);
	}
}

// 飛行中の待機
void CDragon::UpdateFlyingIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eFlyingIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eFlyingIdle);
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
		ChangeState(EState::eIdle2);
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
		ChangeState(EState::eIdle2);
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
		ChangeState(EState::eIdle2);
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
				ChangeState(EState::eAttack2);
			}
			else if (Attack)
			{
				ChangeState(EState::eAttack);
			}
			else if (Defense)
			{
				ChangeState(EState::eDefense);
			}
			else
			{
				ChangeState(EState::eAttack3);
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
			ChangeState(EState::eRoar);
		}
		if (mCharaStatus.hp <= mCharaMaxStatus.hp * 0.5 && mFlyingTime >=400)
		{
			ChangeState(EState::eFlyingStart);
		}
	}

	if (mState == EState::eFlyingIdle)
	{
		mFlyingTime++;
		mFlyingAttackTime++;

		if (mFlyingAttackTime > 400)
		{
			ChangeState(EState::eFlyingAttack);
		}
	}
	if (mState == EState::eFlyingAttack || mState == EState::eFlyingEnd)
	{
		mFlyingAttackTime = 0;
	}

	if (mFlyingTime >= 600)
	{
		ChangeState(EState::eFlyingEnd);
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

	 // キャラ押し戻しコライダー
	mpColliderSphereFeet->Update();
	mpColliderSphereFeet2->Update();

	// ダメージを受けるコライダー
	mpDamageColHead->Update();
	mpDamageColMouth->Update();
	mpDamageColTipMouth->Update();
	mpDamageColNeck->Update();
	mpDamageColChest->Update();
	mpDamageColFeet->Update();
	mpDamageColFeet2->Update();
	mpDamageColFeet3->Update();
	mpDamageColFeet4->Update();

	// 攻撃コライダー
	mpAttackColHead->Update();
	mpAttackColMouth->Update();
	mpAttackColTipMouth->Update();

	mIsGrounded = false;

	if (CInput::PushKey(('Q')))
	{
		ChangeState(EState::eAttack2);
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
	if (self == mpAttackColHead || self == mpAttackColMouth || self == mpAttackColTipMouth
		&& mState != EState::eIdle && mState != EState::eIdle2 &&
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

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// キャラクター同士の衝突処理
	else if (self == mpColliderSphereFeet || self == mpColliderSphereFeet2)
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

	}
	else if (mState == EState::eAttack3)
	{
		mpAttackColHead->SetEnable(true);
		mpAttackColMouth->SetEnable(true);
		mpAttackColTipMouth->SetEnable(true);
	}
}

// 攻撃終了
void CDragon::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColHead->SetEnable(false);
	mpAttackColMouth->SetEnable(false);
	mpAttackColTipMouth->SetEnable(false);
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
	mCharaMaxStatus = ENEMY10_STATUS[index];
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
		ChangeState(EState::eHit);
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
	ChangeState(EState::eDie);
}