#include "CBeholder.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CLightningBallEffect.h"
#include "CElectricShockEffect.h"
#include "CHomingBallEffect.h"
#include "CTornado.h"
#include "CInput.h"

// 球体のモンスターのインスタンス
CBeholder* CBeholder::spInstance = nullptr;

#define ENEMY_HEIGHT  0.5f     // 線分コライダー
#define WITHIN_RANGE  40.0f    // 範囲内
#define MOVE_SPEED    0.65f    // 移動速度
#define GRAVITY       0.0625f  // 重力
#define WALK_RANGE    150.0f   // 追跡する範囲
#define STOP_RANGE    26.0f    // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f   // 回転する範囲

// 球体のモンスターのアニメーションデータのテーブル
const CBeholder::AnimData CBeholder::ANIM_DATA[] =
{
	{ "",										                   true,    0.0f,  0.0f},  // Tポーズ
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle.x",	   true,   41.0f,  0.5f},  // 待機
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle2.x",    true,   23.0f,  0.5f},  // 待機2
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack.x",   false,  23.0f,  0.5f},  // 攻撃(電気ボール)
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack2.x",  false,  21.0f,  0.4f},  // 攻撃2(電流)
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack3.x",  false,  17.0f,  0.5f},  // 攻撃3(電気ボールのホーミング)
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack4.x",  false,  23.0f,  0.4f},  // 攻撃4(回転攻撃＋エフェクト)
	{ "Character\\Enemy\\Beholder\\animation\\BeholderGetHit.x",   true,   23.0f,  0.5f},  // ヒット
	{ "Character\\Enemy\\Beholder\\animation\\BeholderDie.x",	   false,  23.0f,  0.2f},  // 死ぬ
	{ "Character\\Enemy\\Beholder\\animation\\BeholderDizzy.x",	   true,   41.0f,  0.5f},  // めまい
	{ "Character\\Enemy\\Beholder\\animation\\BeholderRun.x",	   true,   17.0f,  0.5f},  // 走る
};

// コンストラクタ
CBeholder::CBeholder()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
	, mStateStep(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
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

	SetAnimationSpeed(0.5f);
	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, -0.8, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine->Position(0.0f, 20.0f, 0.0f);

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.41f, false, 2.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy, ELayer::eField });

	// キャラクター押し戻し処理(左上の触手)
	mpColliderSphereTentacle = new CColliderSphere
	(
		this, ELayer::eEnemy, 
		0.087f, false, 1.0f
	);
	mpColliderSphereTentacle->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(右上の触手)
	mpColliderSphereTentacle2 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.1f, false, 1.0f
	);
	mpColliderSphereTentacle2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(左下の触手)
	mpColliderSphereTentacle3 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.087f, false, 1.0f
	);
	mpColliderSphereTentacle3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(右下の触手)
	mpColliderSphereTentacle4 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.087f, false, 1.0f
	);
	mpColliderSphereTentacle4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(真ん中上の触手)
	mpColliderSphereTentacle5 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 1.0f
	);
	mpColliderSphereTentacle5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(真ん中下の触手)
	mpColliderSphereTentacle6 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.087f, false, 1.0f
	);
	mpColliderSphereTentacle6->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.7f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpColliderSphereBody->Position(0.0f, 0.2f, 0.0f);

	// ダメージを受けるコライダーを作成(左上の触手)
	mpDamageColTentacle = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.087f, false
	);
	mpDamageColTentacle->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(右上の触手)
	mpDamageColTentacle2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.1f, false
	);
	mpDamageColTentacle2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle2->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(左下の触手)
	mpDamageColTentacle3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 
		0.087f, false
	);
	mpDamageColTentacle3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle3->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(右下の触手)
	mpDamageColTentacle4 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.087f, false
	);
	mpDamageColTentacle4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle4->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(真ん中上の触手)
	mpDamageColTentacle5 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.08f, false
	);
	mpDamageColTentacle5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle5->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(真ん中下の触手)
	mpDamageColTentacle6 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.087f, false
	);
	mpDamageColTentacle6->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle6->SetCollisionTags({ ETag::eWeapon });

	// ダメージを与えるコライダー(体)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.41f, false
	);
	// 攻撃コライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(左上の触手)
	mpAttackColTentacle = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.087f, false
	);
	mpAttackColTentacle->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(右上の触手)
	mpAttackColTentacle2 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.1f, false
	);
	mpAttackColTentacle2->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle2->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(左下の触手)
	mpAttackColTentacle3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.087f, false
	);
	mpAttackColTentacle3->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle3->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(右下の触手)
	mpAttackColTentacle4 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.087f, false
	);
	mpAttackColTentacle4->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle4->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(真ん中上の触手)
	mpAttackColTentacle5 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.08f, false
	);
	mpAttackColTentacle5->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle5->SetCollisionTags({ ETag::ePlayer });

	// ダメージを与えるコライダー(真ん中下の触手)
	mpAttackColTentacle6 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.087f, false
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

	mpHomingBall = new CHomingBallEffect
	(
		this, nullptr,
		CVector(0.0f, 20.0f, 0.0f),
		CQuaternion(0.0, 0.f, 0.0f).Matrix()
	);
}

CBeholder::~CBeholder()
{
	// 線分コライダー
	SAFE_DELETE(mpColliderLine);
	// キャラクターの押し戻しコライダー
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereTentacle);
	SAFE_DELETE(mpColliderSphereTentacle2);
	SAFE_DELETE(mpColliderSphereTentacle3);
	SAFE_DELETE(mpColliderSphereTentacle4);
	SAFE_DELETE(mpColliderSphereTentacle5);
	SAFE_DELETE(mpColliderSphereTentacle6);
	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColTentacle);
	SAFE_DELETE(mpDamageColTentacle2);
	SAFE_DELETE(mpDamageColTentacle3);
	SAFE_DELETE(mpDamageColTentacle4);
	SAFE_DELETE(mpDamageColTentacle5);
	SAFE_DELETE(mpDamageColTentacle6);
	// 攻撃コライダー
	SAFE_DELETE(mpAttackColBody);
	SAFE_DELETE(mpAttackColTentacle);
	SAFE_DELETE(mpAttackColTentacle2);
	SAFE_DELETE(mpAttackColTentacle3);
	SAFE_DELETE(mpAttackColTentacle4);
	SAFE_DELETE(mpAttackColTentacle5);
	SAFE_DELETE(mpAttackColTentacle6);
}

// インスタンス
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

// 状態の切り替え
void CBeholder::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// トルネードエフェクトを作成
void CBeholder::CreateTornado()
{
	// トルネードエフェクトを生成して、正面方向へ飛ばす
	CTornado* tornado = new CTornado
	(
		this,
		Position() + CVector(0.0f, 8.0f, 0.0f),
		VectorZ(),
		40.0f,
		80.0f
	);
	tornado->SetColor(CColor(1.0f, 1.0f, 1.0f));
	tornado->SetOwner(this);
}

// 待機状態
void CBeholder::UpdateIdle()
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
void CBeholder::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >=10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE
		&& player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 30.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
}

// 攻撃(電気ボール)
void CBeholder::UpdateAttack()
{
	SetAnimationSpeed(0.5f);

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
		// ステップ1 : 攻撃開始＋電気ボール生成
	case 1:
		if (mAnimationFrame >= 3.0f)
		{
			if (!mpLightningBall->IsThrowing() && !mpElectricShock->IsThrowing())
			{
				mpLightningBall->Start();
				mpElectricShock->Start();
				mStateStep++;
			}
		}
		break;
		// ステップ2 : 攻撃終了＋電気ボール削除＋攻撃アニメーション終了待ち
	case 2:
		if (mAnimationFrame >=23.0f)
		{
			mpLightningBall->Stop();
			mpElectricShock->Stop();
			// 攻撃終了待ち状態へ移行
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// // 攻撃2(電流)
void CBeholder::UpdateAttack2()
{
	SetAnimationSpeed(0.4f);

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0 : 攻撃アニメーション開始＋攻撃開始
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		if (mAnimationFrame >= 3.0f)
		{
			mStateStep++;
		}
		break;
		// ステップ1 : 攻撃アニメーション終了待ち
	case 1:
		if (mAnimationFrame >= 20.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃3(電気ボールのホーミング)
void CBeholder::UpdateAttack3()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack3);
	if (!mpHomingBall->IsThrowing())
	{
		mpHomingBall->Start();
	}
	// 攻撃終了待ち状態へ移行
	ChangeState(EState::eAttackWait);
}

// 攻撃4(回転攻撃＋トルネード)
void CBeholder::UpdateAttack4()
{
	SetAnimationSpeed(0.4f);
	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eAttack4);
		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 13.0f)
		{
			AttackStart();
			CreateTornado();
			mStateStep++;
		}
		break;
	case 2:
		if (mAnimationFrame >= 17.0f)
		{
			AttackEnd();
			mStateStep++;
		}
		break;
		// 攻撃アニメーション終了待ち
	case 3:
		if (mAnimationFrame >= 23.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃終了待ち
void CBeholder::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpLightningBall->Stop();
		mpElectricShock->Stop();
		mpHomingBall->Stop();
		ChangeState(EState::eIdle2);
	}
}

// ヒット
void CBeholder::UpdateHit()
{
	SetAnimationSpeed(0.5f);
	mpElectricShock->Stop();
	mpLightningBall->Stop();
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
void CBeholder::UpdateDie()
{
	SetAnimationSpeed(0.2f);
	mpElectricShock->Stop();
	mpLightningBall->Stop();
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
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		ChangeState(EState::eIdle2);
	}
}

// 移動
void CBeholder::UpdateRun()
{
	SetAnimationSpeed(0.5f);
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

	if (vectorPos <= 30.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
	// 追跡が止まった時、待機モーションへ
	if (vectorPos <= STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// 更新処理
void CBeholder::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.X(0.0f);
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
		// 攻撃(電気ボール)
	case EState::eAttack:
		UpdateAttack();
		break;
		// 攻撃2(電流)
	case EState::eAttack2:
		UpdateAttack2();
		break;
		// 攻撃3(電気ボールのホーミング)
	case EState::eAttack3:
		UpdateAttack3();
		break;
		// 攻撃4(回転攻撃＋エフェクト)
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
	float vectorPos = (player->Position() - Position()).Length();

	if (mState != EState::eIdle && mState != EState::eDie)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

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

		if (mAttackTime > 160)
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
				//ChangeState(EState::eAttack2);
			}
			else if (Attack3)
			{
				//ChangeState(EState::eAttack3);
			}
			else if (Attack4)
			{
				ChangeState(EState::eAttack4);
			}
			else
			{
				//ChangeState(EState::eAttack);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3 || mState == EState::eAttack4)
		{
			mAttackTime = 0;
		}
	}

	if (mState == EState::eRun)
	{
		mFlyingTime++;
		if (mFlyingTime < 200 && mFlyingTime > 0 && player->Position().Y() < 1.0f)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + 0.05f);
		}
		else if(player->Position().Y() < 1.0f)
		{
			//Position(Position().X(), Position().Y() - 0.1f, Position().Z());
		}
	}
	if (mState == EState::eIdle2 && vectorPos > WALK_RANGE)
	{
		Position(Position().X(), Position().Y() - 0.05f, Position().Z());
	}
	else if(mState == EState::eIdle2 && vectorPos <= STOP_RANGE)
	{
		Position(Position().X(), Position().Y() - 0.05f, Position().Z());
	}

	if (mState == EState::eRun)
	{
		if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
		{
			Position(Position() + mMoveSpeed);
		}
	}

	if (Position().Y() <= 0.0f)
	{
		mFlyingTime = 0;
	}
	if (mState == EState::eHit)
	{
		Position(Position().X(), Position().Y() - 0.5f, Position().Z());
	}

	CDebugPrint::Print(" 飛行: %d\n", mFlyingTime);
	float y = Position().Y();
	CDebugPrint::Print(" 高さ: %f\n", y);

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
				int damage = CalcDamage(1.0f,this, chara);

				// ダメージを与える
				chara->TakeDamage(damage, this);

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
	else if (self == mpColliderLine || self == mpColliderSphereBody)
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
	if (mState == EState::eAttack4)
	{
		mpAttackColBody->SetEnable(true);
		mpAttackColTentacle->SetEnable(true);
		mpAttackColTentacle2->SetEnable(true);
		mpAttackColTentacle3->SetEnable(true);
		mpAttackColTentacle4->SetEnable(true);
		mpAttackColTentacle5->SetEnable(true);
		mpAttackColTentacle6->SetEnable(true);
	}
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
		Position(Position() - dir * Scale().X() * 0.4f);
	}
}

// 死亡処理
void CBeholder::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}