#include "CBoxer.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CImpactEffect.h"
#include "CInput.h"

// ボクサーのインスタンス
CBoxer* CBoxer::spInstance = nullptr;

#define ENEMY_HEIGHT   1.0f   // 線分コライダー
#define WITHIN_RANGE  60.0f   // 範囲内
#define MOVE_SPEED     0.8f   // 移動速度
#define JUMP_SPEED     1.5f   // ジャンプスピード
#define GRAVITY     0.0625f   // 重力
#define JUMP_END_Y     1.0f	   
#define WALK_RANGE    50.0f   // 追跡する範囲
#define STOP_RANGE    30.0f   // 追跡を辞める範囲
#define ROTATE_RANGE 250.0f   // 回転する範囲

// ボクサーのアニメーションデータのテーブル
const CBoxer::AnimData CBoxer::ANIM_DATA[] =
{
	{ "",										                true,	 0.0f,	 0.0f},  // Tポーズ
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDance.x",	    true,	85.0f,	 0.5f},	 // ダンス
	{ "Character\\Enemy\\Boxer\\animation\\BoxerIdle.x",	    true,	21.0f,	 0.5f},  // 待機
	{ "Character\\Enemy\\Boxer\\animation\\BoxerAttack.x",	    true,	19.0f,   0.5f},	 // 攻撃(パンチ)
	{ "Character\\Enemy\\Boxer\\animation\\BoxerAttack2.x",	    false,	26.0f,	 0.5f},	 // 攻撃2(蹴り)
	{ "Character\\Enemy\\Boxer\\animation\\BoxerGetHit.x",	    false,	17.0f,   0.4f},	 // ヒット
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDefense.x",	    true,	21.0f,	 0.5f},	 // 防御
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDefenseHit.x",	false,	21.0f,	 0.5f},  // 防御中のヒット
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDie.x",	        false,	24.0f,	 0.2f},	 // 死ぬ 24.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDizzy.x",	    false,	24.0f,	 0.4f},	 // めまい 24.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerJump.x",	    false,	23.0f,   0.5f},	 // ジャンプ 23.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerJumpEnd.x",	    false,	26.0f,	 0.5f},	 // ジャンプの終了 26.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerRun.x",	        true,	21.0f, 	 0.5f},	 // 走る 21.0
	{ "Character\\Enemy\\Boxer\\animation\\BoxerSlide.x",	    false,	36.0f,   0.5f},	 // 滑る 36.0
	//{ "Character\\Enemy\\Boxer\\animation\\BoxerDieRecover.x",	true,	48.0f	},	// 起き上がる 24.0f
};

// コンストラクタ
CBoxer::CBoxer()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mDefenseTime(0)
	, mStateStep(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
{
	//インスタンスの設定
	spInstance = this;

	// 敵の種類
	mType = EEnemyType::eBoxer;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Boxer");

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
		0.4f, false, 2.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.04f, 0.1f, 0.0f);

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.5f, false, 4.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.07f, 0.1f, 0.0f);

	// キャラクター押し戻し処理(右手)
	mpColliderSphereHandR = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.27f, false, 3.0f
	);
	mpColliderSphereHandR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHandR->Position(0.0f, -0.1f, 0.0f);

	// キャラクター押し戻し処理(左手)
	mpColliderSphereHandL = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.27f, false, 3.0f
	);
	mpColliderSphereHandL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHandL->Position(0.0f, -0.1f, 0.0f);

	// キャラクター押し戻し処理(右足)
	mpColliderSphereFeetR = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.06f, false, 3.0f
	);
	mpColliderSphereFeetR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeetR->Position(0.0f, 0.03f, 0.0f);

	// キャラクター押し戻し処理(左足)
	mpColliderSphereFeetL = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.06f, false, 3.0f
	);
	mpColliderSphereFeetL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeetL->Position(0.0f, 0.03f, 0.0f);

	// ダメージを受けるコライダーを作成(頭)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.4f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon,ETag::eFlame});
	//ダメージを受けるコライダーを少し上へずらす
	mpDamageColHead->Position(0.04f, 0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.5f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon,ETag::eFlame });
	mpDamageColBody->Position(0.07f, 0.0f, 0.0f);

	// ダメージを受けるコライダーを作成(右手)
	mpDamageColHandR = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.27f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHandR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHandR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHandR->Position(0.0f, -0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(左手)
	mpDamageColHandL = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.27f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHandL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHandL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHandL->Position(0.0f, -0.1f, 0.0f);

	// ダメージを受けるコライダーを作成(右腕)
	mpDamageColArmR = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColArmR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColArmR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColArmR->Position(0.0f, -0.04f, 0.0f);

	// ダメージを受けるコライダーを作成(左腕)
	mpDamageColArmL = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColArmL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColArmL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColArmL->Position(0.0f, -0.04f, 0.0f);

	// ダメージを受けるコライダーを作成(右足)
	mpDamageColFeetR = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.1f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColFeetR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeetR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeetR->Position(0.0f, 0.25f, 0.0f);

	// ダメージを受けるコライダーを作成(左足)
	mpDamageColFeetL = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.1f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColFeetL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeetL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeetL->Position(0.0f, 0.25f, 0.0f);

	// ダメージを与えるコライダー(頭)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.4f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.04f, 0.1f, 0.0f);

	// ダメージを与えるコライダー(体)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.5f, false
	);
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });
	mpAttackColBody->Position(0.07f, 0.0f, 0.0f);

	// ダメージを与えるコライダー(右手)
	mpAttackColHandR = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.27f, false
	);
	mpAttackColHandR->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHandR->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHandR->Position(0.0f, -0.1f, 0.0f);

	// ダメージを与えるコライダー(右足)
	mpAttackColFeetR = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.06f, false
	);
	mpAttackColFeetR->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColFeetR->SetCollisionTags({ ETag::ePlayer });
	mpAttackColFeetR->Position(0.0f, 0.03f, 0.0f);

	// ダメージを与えるコライダー(左足)
	mpAttackColFeetL = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.06f, false
	);
	mpAttackColFeetL->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColFeetL->SetCollisionTags({ ETag::ePlayer });
	mpAttackColFeetL->Position(0.0f, 0.03f, 0.0f);

	// 押し戻しコライダーとダメージを受けるコライダーと
	// 攻撃コライダーをボクサーの頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_neck_01");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpDamageColHead->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// 押し戻しコライダーとダメージを受けるコライダーと
	// 攻撃コライダーをボクサーの体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_spine_02");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);
	mpAttackColBody->SetAttachMtx(bodyMty);

	// 押し戻しコライダーとダメージを受けるコライダーと
	// 攻撃コライダーをボクサーの右手の行列にアタッチ
	const CMatrix* rightHandMty = GetFrameMtx("Armature_drill_r");
	mpColliderSphereHandR->SetAttachMtx(rightHandMty);
	mpDamageColHandR->SetAttachMtx(rightHandMty);
	mpAttackColHandR->SetAttachMtx(rightHandMty);

	// 押し戻しコライダーとダメージを受けるコライダーをボクサーの左手の行列にアタッチ
	const CMatrix* leftHandMty = GetFrameMtx("Armature_drill_l");
	mpColliderSphereHandL->SetAttachMtx(leftHandMty);
	mpDamageColHandL->SetAttachMtx(leftHandMty);

	// ダメージを受けるコライダーをボクサーの右腕の行列にアタッチ
	const CMatrix* rightArmMty = GetFrameMtx("Armature_lowerarm_r");
	mpDamageColArmR->SetAttachMtx(rightArmMty);

	// ダメージを受けるコライダーをボクサーの左腕の行列にアタッチ
	const CMatrix* leftArmMty = GetFrameMtx("Armature_lowerarm_l");
	mpDamageColArmL->SetAttachMtx(leftArmMty);

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

	// 衝撃を作成
	const CMatrix* rightHandMty2 = GetFrameMtx("Armature_saw_r");
	mpImpact = new  CImpactEffect
	(
		this, rightHandMty2,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0, 90.f, 0.0f).Matrix()
	);
}

// デストラクタ
CBoxer::~CBoxer()
{
	// 線分コライダー
	SAFE_DELETE(mpColliderLine);
	// キャラクターの押し戻しコライダー
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereHandR);
	SAFE_DELETE(mpColliderSphereHandL);
	SAFE_DELETE(mpColliderSphereFeetR);
	SAFE_DELETE(mpColliderSphereFeetL);
	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColHandR);
	SAFE_DELETE(mpDamageColHandL);
	SAFE_DELETE(mpDamageColArmR);
	SAFE_DELETE(mpDamageColArmL);
	SAFE_DELETE(mpDamageColFeetR);
	SAFE_DELETE(mpDamageColFeetL);
	// 攻撃コライダー
	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColBody);
	SAFE_DELETE(mpAttackColHandR);
	SAFE_DELETE(mpAttackColFeetR);
	SAFE_DELETE(mpAttackColFeetL);
}

// インスタンス
CBoxer* CBoxer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CBoxer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 状態の切り替え
void CBoxer::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// 待機状態
void CBoxer::UpdateIdle()
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
void CBoxer::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >=10.0f && vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
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

// 攻撃(パンチ)
void CBoxer::UpdateAttack()
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
	switch (mStateStep)
	{
		// ステップ0 : 攻撃アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		mStateStep++;
		break;
		// ステップ1 : 攻撃開始＋インパクト生成
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			AttackStart();
			if (vectorPos >= 40.0f)
			{
				if (!mpImpact->IsThrowing())
				{
					mpImpact->Start();
					mStateStep++;
				}
			}
			else
			{
				mStateStep++;
			}
		}
		break;
		// ステップ2 : 攻撃終了
	case 2:
		if (mAnimationFrame >= 15.0f)
		{
			AttackEnd();
			mStateStep++;
		}
		break;
		// ステップ3 : 攻撃アニメーション終了待ち
	case 3:
		if (mAnimationFrame >= 17.0f)
		{
			AttackEnd();
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃2(蹴り)
void CBoxer::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);
	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	if (mAnimationFrame <= 30.0f)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = CPlayer::Instance()->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (vectorPos >= 0.0f && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += newPos * 1.0f;
	}

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃2アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
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
		// ステップ2 : 攻撃2アニメーション終了待ち
	case 2:
		if (mAnimationFrame >= 24.0f)
		{
			AttackEnd();
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃終了待ち
void CBoxer::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpImpact->Stop();

		// 連続攻撃するかどうか
		bool continuousz = false;
		// 確率
		int percent = Math::Rand(0, 5);
		if (percent == 1) continuousz = true;
		if (continuousz)
		{
			ChangeState(EState::eAttack);
		}
		else
		{
			ChangeState(EState::eIdle2);
		}
	}
}

// ヒット
void CBoxer::UpdateHit()
{
	SetAnimationSpeed(0.4f);
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
			ChangeState(EState::eDizzy);
		}
		else
		{
			// プレイヤーの攻撃がヒットした時の待機状態へ移行
			ChangeState(EState::eIdle2);
		}
	}
}

// 防御
void CBoxer::UpdateDefense()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDefense);

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= ROTATE_RANGE)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (IsAnimationFinished())
	{
		if (mDefenseTime >= 200)
		{
			ChangeState(EState::eAttack);
		}
		else
		{
			ChangeState(EState::eDefense);
		}
	}
}

// 防御中のヒット
void CBoxer::UpdateDefenseHit()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDefenseHit);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eDefense);
	}
}

// 死ぬ
void CBoxer::UpdateDie()
{
	SetAnimationSpeed(0.2f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::BoxerDeath();
	}
}

// めまい(混乱)
void CBoxer::UpdateDizzy()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		ChangeState(EState::eIdle2);
	}
}

// ジャンプ中
void CBoxer::UpdateJump()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eJump);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eJumpEnd);
	}
}

// ジャンプ終了
void CBoxer::UpdateJumpEnd()
{
	// ジャンプアニメーションが待機状態へ戻す
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// 移動
void CBoxer::UpdateRun()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// 範囲内の時、移動し追跡する
	if (vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
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
	if (vectorPos <= STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		if (mState != EState::eAttack2)
		{
			ChangeState(EState::eIdle2);
		}
	}
}

// 滑る
void CBoxer::UpdateSlide()
{
	SetAnimationSpeed(0.5f);
	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	if (mAnimationFrame <= 30.0f)
	{
		// プレイヤーのいる方向へ向く
		CVector dir = CPlayer::Instance()->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (vectorPos >= 0.0f && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += newPos * 1.0f;
	}

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 滑るアニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eSlide);
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
		if (mAnimationFrame >= 34.0f)
		{
			// 滑る終了待ち状態へ移行
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 更新処理
void CBoxer::Update()
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
		// ジャンプ中
	case EState::eJump:
		UpdateJump();
		break;
		// ジャンプ終了
	case EState::eJumpEnd:
		UpdateJumpEnd();
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
	CVector gaugePos = Position() + CVector(0.0f, 35.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState != EState::eIdle && mState != EState::eDie)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eHit)
	{
		if (vectorPos <= 50.0f)
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
				ChangeState(EState::eAttack2);
			}
			else if (Defense)
			{
				ChangeState(EState::eDefense);
			}
			else if (Jump)
			{
				ChangeState(EState::eJump);
			}
			else if (Slide)
			{
				ChangeState(EState::eSlide);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eSlide
			|| mState == EState::eJump || mState == EState::eDefense || vectorPos >= WALK_RANGE)
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

	if (vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE ||mState == EState::eAttack2 ||mState ==EState::eSlide)
	{
		Position(Position() + mMoveSpeed);
	}

	if (Position().Y() >= 0.1f)
	{
		Position(Position().X(), Position().Y() - 0.5f, Position().Z());
	}

	// キャラクターの更新
	CXCharacter::Update();

	// キャラクターの押し戻しコライダー
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereHandR->Update();
	mpColliderSphereHandL->Update();
	mpColliderSphereFeetR->Update();
	mpColliderSphereFeetL->Update();
	// ダメージを受けるコライダー
	mpDamageColHead->Update();
	mpDamageColBody->Update();
	mpDamageColHandR->Update();
	mpDamageColHandL->Update();
	mpDamageColArmR->Update();
	mpDamageColArmL->Update();
	mpDamageColFeetR->Update();
	mpDamageColFeetL->Update();
	// 攻撃コライダー
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
void CBoxer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CBoxer::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	if (mState == EState::eAttack)
	{
		mpAttackColHandR->SetEnable(true);
	}

	if (mState == EState::eAttack2)
	{
		mpAttackColFeetR->SetEnable(true);
		mpAttackColFeetL->SetEnable(true);
	}

	if (mState == EState::eSlide)
	{
		mpAttackColHead->SetEnable(true);
		mpAttackColBody->SetEnable(true);
	}
}

// 攻撃終了
void CBoxer::AttackEnd()
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
void CBoxer::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CBoxer::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CBoxer::ChangeLevel(int level)
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
void CBoxer::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		if (mState == EState::eDefense)
		{
			ChangeState(EState::eDefenseHit);
		}
		else
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

	if (causedObj != nullptr)
	{
		// ダメージを与えた相手の方向へ向く
		CVector dir = causedObj->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));

		// ノックバックでダメージを与えた相手の方向から後ろにズラす
		Position(Position() - dir * Scale().X() * 0.1f);
	}
}

// 防御力の強化割合を取得
float CBoxer::GetDefBuff(const CVector& attackDir)const
{
	// 防御状態であれば、防御2倍
	if (mState == EState::eDefense) return 2.0f;

	// 通常時の防御の割合
	return mBaseDefenseBuffRatio;
}

// 死亡処理
void CBoxer::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}