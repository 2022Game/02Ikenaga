#include "CBee.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"
#include "CNeedle.h"

// 蜂のインスタンス
CBee* CBee::spInstance = nullptr;

#define ENEMY_HEIGHT  0.3f    // 線分コライダー
#define WITHIN_RANGE  40.0f   // 範囲内
#define WALK_RANGE    100.0f  // 追跡する範囲
#define STOP_RANGE    22.0f   // 追跡を辞める範囲
#define STOP_RANGE_Y  20.0f   // 追跡を辞める高さ
#define ROTATE_RANGE  250.0f  // 回転する範囲
#define MOVE_SPEED    0.12f   // 移動速度
#define MOVE_SPEED_Y  0.027f  // Yのスピード
#define HEIGHT        0.5f    // 高さ
#define PLAYER_HEIGHT 0.25f   // プレイヤーの高さ


// 蜂のアニメーションデータのテーブル
const CBee::AnimData CBee::ANIM_DATA[] =
{
	{ "",										        true,	0.0f,	0.0f},  // Tポーズ
	{ "Character\\Enemy\\Bee\\animation\\BeeIdle.x",	true,	20.0f,	0.5f},	// 待機 20.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeAttack.x",	true,	17.0f,	0.3f},	// 攻撃 17.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeGetHit.x",	true,	13.0f,	0.25f},	// ヒット 13.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeDie.x",	    true,	20.0f,	0.2f},	// 死ぬ 20.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeMoveFWD.x",	true,	21.0f,	0.5f},	// 移動2 21.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveBWD.x",	true,	42.0f	},	    // 移動 21.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveLFT.x",	true,	42.0f	},	    // 左移動 21.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveRGT.x",	true,	42.0f	},	    // 右移動 21.0f
};

// コンストラクタ
CBee::CBee()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
	, mMoveSpeed(CVector::zero)
	, mIsSpawnedNeedleEffect(false)
	, mIsGrounded(false)
	, mStateAttackStep(0)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Bee");

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

	// キャラクターの線分コライダー
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine->Position(0.0f, 5.0f, 0.0f);

	// キャラクター押し戻し処理(頭)
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 2.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer, ELayer::eEnemy, ELayer::eField});
	mpColliderSphereHead->Position(-0.1f, 0.0f, 0.03f);

	// キャラクター押し戻し処理(口ばし)
	mpColliderSphereBeak = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.04f, false, 2.0f
	);
	mpColliderSphereBeak->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBeak->Position(-0.23f, 0.01f, 0.1f);

	// キャラクター押し戻し処理(口ばし2)
	mpColliderSphereBeak2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.02f, false, 2.0f
	);
	mpColliderSphereBeak2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBeak2->Position(-0.35f, 0.01f, 0.2f);

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,0.25f, false, 2.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eField });
	mpColliderSphereBody->Position(0.0f, 0.18f, 0.0f);

	// キャラクター押し戻し処理(尻尾)
	mpColliderSphereTail = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 2.0f
	);
	mpColliderSphereTail->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// キャラクター押し戻し処理(尻尾2)
	mpColliderSphereTail2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.1f, false, 2.0f
	);
	mpColliderSphereTail2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail2->Position(0.02f, 0.14f, 0.0f);

	// キャラクター押し戻し処理(尻尾3)
	mpColliderSphereTail3 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.05f, false, 2.0f
	);
	mpColliderSphereTail3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail3->Position(-0.05f, 0.0f, 0.04f);

	// キャラクター押し戻し処理(尻尾4)
	mpColliderSphereTail4 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.03f, false, 2.0f
	);
	mpColliderSphereTail4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail4->Position(-0.15f, 0.0f, 0.1f);

	// キャラクター押し戻し処理(尻尾5)
	mpColliderSphereTail5 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.02f, false, 2.0f
	);
	mpColliderSphereTail5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail5->Position(-0.25f, 0.0f, 0.105f);

	// ダメージを受けるコライダーを作成(頭)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.08f, false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHead->Position(-0.1f, 0.0f, 0.03f);

	// ダメージを受けるコライダーを作成(口ばし)
	mpDamageColBeak = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.04f, false
	);
	mpDamageColBeak->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBeak->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBeak->Position(-0.23f, 0.01f, 0.1f);

	// ダメージを受けるコライダーを作成(口ばし2)
	mpDamageColBeak2 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.02f, false
	);
	mpDamageColBeak2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBeak2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBeak2->Position(-0.35f, 0.01f, 0.2f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.25f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, 0.18f, 0.0f);

	// ダメージを受けるコライダーを作成(尻尾)
	mpDamageColTail = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.08f, false
	);
	mpDamageColTail->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail->SetCollisionTags({ ETag::eWeapon });

	// ダメージを受けるコライダーを作成(尻尾2)
	mpDamageColTail2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.1f, false
	);
	mpDamageColTail2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail2->Position(0.02f, 0.14f, 0.0f);

	// ダメージを受けるコライダーを作成(尻尾3)
	mpDamageColTail3 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.05f, false
	);
	mpDamageColTail3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail3->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail3->Position(-0.05f, 0.0f, 0.04f);

	// ダメージを受けるコライダーを作成(尻尾4)
	mpDamageColTail4 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.03f, false
	);
	mpDamageColTail4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail4->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail4->Position(-0.15f, 0.0f, 0.1f);

	// ダメージを受けるコライダーを作成(尻尾5)
	mpDamageColTail5 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.02f, false
	);
	mpDamageColTail5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail5->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail5->Position(-0.25f, 0.0f, 0.105f);

	// ダメージを与えるコライダー
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.01f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(-0.28f, 0.0f, 0.1f);

	// キャラクター押し戻しとダメージを受けるコライダーを頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpColliderSphereBeak->SetAttachMtx(headMty);
	mpColliderSphereBeak2->SetAttachMtx(headMty);
	mpDamageColHead->SetAttachMtx(headMty);
	mpDamageColBeak->SetAttachMtx(headMty);
	mpDamageColBeak2->SetAttachMtx(headMty);

	// キャラクター押し戻しと
	// ダメージを受けるコライダーを体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);

	// キャラクター押し戻しと
	// ダメージを受けるコライダーを尻尾の行列にアタッチ
	const CMatrix* tailMty = GetFrameMtx("Armature_Tail01");
	mpColliderSphereTail->SetAttachMtx(tailMty);
	mpColliderSphereTail2->SetAttachMtx(tailMty);
	mpDamageColTail->SetAttachMtx(tailMty);
	mpDamageColTail2->SetAttachMtx(tailMty);

	// キャラクター押し戻しとダメージを受けるコライダーと
	// 攻撃コライダーを蜂の針の行列にアタッチ
	const CMatrix* needleMty = GetFrameMtx("Armature_Tail02");
	mpColliderSphereTail3->SetAttachMtx(needleMty);
	mpColliderSphereTail4->SetAttachMtx(needleMty);
	mpColliderSphereTail5->SetAttachMtx(needleMty);
	mpDamageColTail3->SetAttachMtx(needleMty);
	mpDamageColTail4->SetAttachMtx(needleMty);
	mpDamageColTail5->SetAttachMtx(needleMty);
	mpAttackCol->SetAttachMtx(needleMty);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackCol->SetEnable(false);
	mpColliderSphereBeak2->SetEnable(false);
}

// デストラクタ
CBee::~CBee()
{
	// キャラクターの線分コライダー
	SAFE_DELETE(mpColliderLine);
	// キャラクター押し戻しコライダー
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBeak);
	SAFE_DELETE(mpColliderSphereBeak2);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereTail);
	SAFE_DELETE(mpColliderSphereTail2);
	SAFE_DELETE(mpColliderSphereTail3);
	SAFE_DELETE(mpColliderSphereTail4);
	SAFE_DELETE(mpColliderSphereTail5);
	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBeak);
	SAFE_DELETE(mpDamageColBeak2);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColTail);
	SAFE_DELETE(mpDamageColTail2);
	SAFE_DELETE(mpDamageColTail3);
	SAFE_DELETE(mpDamageColTail4);
	SAFE_DELETE(mpDamageColTail5);
	// 攻撃コライダー
	SAFE_DELETE(mpAttackCol);
}

// インスタンス
CBee* CBee::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CBee::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 状態の切り替え
void CBee::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
}

// 待機状態
void CBee::UpdateIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// 待機状態2
void CBee::UpdateIdle2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	mFlyingTime++;
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		ChangeState(EState::eRun);
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
void CBee::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.3f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	// ステップごとに処理を分ける
	switch (mStateAttackStep)
	{
		// ステップ0 : 攻撃アニメーション開始＋攻撃開始
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		if (mAnimationFrame >= 5.0f)
		{
			AttackStart();
			mStateAttackStep++;
		}
		break;
		// ステップ1 : 
	case 1:
		if (vectorPos >= 30.0f)
		{
			if (mAnimationFrame >= 7.5f)
			{
				// 針を生成済みフラグを初期化
				mIsSpawnedNeedleEffect = false;
				// 針を生成していない
				if (!mIsSpawnedNeedleEffect)
				{
					CNeedle* needle = new CNeedle
					(
						this,
						Position() + CVector(0.0f, 8.0f, 0.0f),
						VectorZ(),
						150.0f,
						100.0f
					);
					needle->SetColor(CColor(1.0f, 0.0f, 1.0f));
					needle->Scale(5.0f, 5.0f, 5.0f);
					needle->Rotate(-90.0f, 0.0f, 0.0f);
					needle->SetOwner(this);

					mIsSpawnedNeedleEffect = true;
					mStateAttackStep++;
				}
			}
		}
		else
		{
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
}

// 攻撃終了待ち
void CBee::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle2);
	}
}

// ヒット
void CBee::UpdateHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
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
void CBee::UpdateDie()
{
	SetAnimationSpeed(0.2f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::BeeDeath();
	}
}

// 移動
void CBee::UpdateRun()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eRun);
	mFlyingTime++;

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
	// 追跡が止まった時、待機モーションへ
	else if (vectorPos <= STOP_RANGE || vectorPos > WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// 更新処理
void CBee::Update()
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
	CVector gaugePos = Position() + CVector(0.0f, 35.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	
	if (vectorPos <= WITHIN_RANGE && mState != EState::eIdle2 && mState != EState::eAttack 
		&& mState != EState::eAttackWait && mState != EState::eHit
		&& mState != EState::eDie && mState != EState::eRun)
	{
		ChangeState(EState::eIdle2);
	}

	if (mState == EState::eRun || mState == EState::eIdle2 || mState == EState::eAttack ||
		mState == EState::eDie || mState == EState::eHit || mState == EState::eAttackWait)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 && vectorPos <= WITHIN_RANGE || mState == EState::eHit || mState == EState::eRun
		|| mState == EState::eAttack || mState == EState::eAttackWait)
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
		if (mState == EState::eAttack)
		{
			mAttackTime = 0;
		}
	}

	if (mState == EState::eRun)
	{
		if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
		{
			Position(Position() + mMoveSpeed * MOVE_SPEED);
		}
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
	{
		if (mFlyingTime <= 200 && mFlyingTime > 0)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + MOVE_SPEED_Y);
		}
	}

	if (mFlyingTime >= 200 && Position().Y() >= 0.1f)
	{
		Position(Position().X(), Position().Y() - HEIGHT, Position().Z());
	}

	if (mState == EState::eHit)
	{
		Position(Position().X(), Position().Y() - HEIGHT, Position().Z());
	}

	if (vectorPos <= STOP_RANGE_Y && player->Position().Y() > PLAYER_HEIGHT)
	{
		ChangeState(EState::eIdle2);
	}

	if (Position().Y() <= 0.0f)
	{
		mFlyingTime = 0;
	}

	// キャラクターの更新
	CXCharacter::Update();

	// キャラクター押し戻しコライダー
	mpColliderSphereHead->Update();
	mpColliderSphereBeak->Update();
	mpColliderSphereBeak2->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereTail->Update();
	mpColliderSphereTail2->Update();
	mpColliderSphereTail3->Update();
	mpColliderSphereTail4->Update();
	mpColliderSphereTail5->Update();
	// ダメージを受けるコライダー
	mpDamageColHead->Update();
	mpDamageColBeak->Update();
	mpDamageColBeak2->Update();
	mpDamageColBody->Update();
	mpDamageColTail->Update();
	mpDamageColTail2->Update();
	mpDamageColTail3->Update();
	mpDamageColTail4->Update();
	mpDamageColTail5->Update();
	// 攻撃コライダー
	mpAttackCol->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
	CDebugPrint::Print(" 飛行 %d\n", mFlyingTime);
	float y = Position().Y();
	CDebugPrint::Print(" 高さ %f\n", y);
	CDebugPrint::Print(" 長さ %f\n", vectorPos);
}

// 衝突処理
void CBee::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackCol && mState != EState::eIdle)
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
	else if (self == mpColliderSphereHead || self == mpColliderSphereBeak || self == mpColliderSphereBeak2
		|| self == mpColliderSphereBody || self == mpColliderSphereTail || self == mpColliderSphereTail2
		|| self == mpColliderSphereTail3 || self == mpColliderSphereTail4 || self == mpColliderSphereTail5)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CBee::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	if (mState == EState::eAttack)
	{
		mpAttackCol->SetEnable(true);
	}
}

// 攻撃終了
void CBee::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackCol->SetEnable(false);
}

// 描画
void CBee::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CBee::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CBee::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY5_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CBee::TakeDamage(int damage, CObjectBase* causedObj)
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
		Position(Position() - dir * Scale().X() * 0.6f);
	}
}

// 死亡処理
void CBee::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}