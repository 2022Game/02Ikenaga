#include "CMushroom.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"

// マッシュルームのインスタンス
CMushroom* CMushroom::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 40.0f       // 範囲内
#define MOVE_SPEED 0.07f         // 移動速度
#define GRAVITY 0.0625f          // 重力

#define WALK_RANGE 100.0f        // 追跡する範囲
#define STOP_RANGE 20.0f         // 追跡を辞める範囲
#define ROTATE_RANGE  250.0f     // 回転する範囲

// マッシュルームのアニメーションデータのテーブル
const CMushroom::AnimData CMushroom::ANIM_DATA[] =
{
	{ "",										                            true,	0.0f,	0.0f},// Tポーズ
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlant.x",	        true,	21.0f,	0.3f},	// 植物 21.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlantToBattle.x",	true,	21.0f,	0.3},	// 植物からきのこ 21.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle2.x",	    true,	18.0f,	0.5f},	// 待機2 18.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle.x",	    true,	18.0f,	0.5f},	// 待機 18.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack.x",	        false,	26.0f,	0.4f},	// 攻撃 26.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack2.x",	        false,	26.0f,	0.6f},	// 攻撃 26.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack3.x",	        false,	26.0f,	0.5f},	// 攻撃 26.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomGetHit.x",	        true,	23.0f,	0.4f},	// ヒット 23.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomDie.x",	            true,	26.0f,	0.25f},	//  死ぬ26.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomDizzy.x",	            true,	41.0f,  0.4f},	// めまい 41.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomRun.x",	            true,	17.0f, 	0.4f},	//走る 17.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomSenseSomethingMaintain.x",	true,	121.0f	},	//見回す 121.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomSenseSomethingStart.x",	true,	25.0f	},	//開始の見回す 25.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomTaunting.x",	true,	80.0f	},	//挑発 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomVictory.x",	true,	61.0f	},	//勝利 61.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk.x",	true,	31.0f	},	//歩く 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk2.x",	true,	31.0f	},	//歩く2 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk3.x",	true,	31.0f	},	//歩く3 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk4.x",	true,	31.0f	},	//歩く4 31.0f
};

int CMushroom::mHp;

// コンストラクタ
CMushroom::CMushroom()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mIsGrounded(false)
	, mStateAttack2Step(0)
	, mStateAttack3Step(0)
	, mMoveSpeed(CVector::zero)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Mushroom");

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

	SetAnimationSpeed(0.3f);
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
		0.5f, false, 5.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.0f, -0.08f, 0.0f);

	// キャラクター押し戻し処理(体)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.25f, false, 5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.045f, 0.2f, 0.0f);

	// キャラクター押し戻し処理(根)
	mpColliderSphereRoot = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.25f, false, 5.0f
	);
	mpColliderSphereRoot->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereRoot->Position(0.065f, 0.18f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.55f, 0.0f),
		4.5f,false
	);
	//　ダメージを受けるコライダーと衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, -4.2f, 0.0f);

	// ダメージを受けるコライダーを作成(かさ)
	mpDamageColUmbrella = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.55f, false
	);
	mpDamageColUmbrella->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColUmbrella->SetCollisionTags({ ETag::eWeapon });
	mpDamageColUmbrella->Position(0.0f, -0.1f, 0.0f);
	
	// ダメージを受けるコライダーを作成(根)
	mpDamageColRoot = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.25f, false
	);
	mpDamageColRoot->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColRoot->SetCollisionTags({ ETag::eWeapon });
	mpDamageColRoot->Position(0.05f, 0.23f, 0.0f);

	// ダメージを与えるコライダー(頭)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.55f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.0f, -0.1f, 0.0f);

	// ダメージを与えるコライダー(根)
	mpAttackColRoot = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.25f, false
	);
	mpAttackColRoot->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColRoot->SetCollisionTags({ ETag::ePlayer });
	mpAttackColRoot->Position(0.05f, 0.23f, 0.0f);

	// キャラクター押し戻しコライダーと
	// ダメージを受けるコライダーと攻撃コライダーをマッシュルームの頭の行列にアタッチ
	const CMatrix* headMty = GetFrameMtx("Armature_mushroom_spine03");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpDamageColUmbrella->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// キャラクター押し戻しコライダーと
	// ダメージを受けるコライダーをマッシュルームの体の行列にアタッチ
	const CMatrix* bodyMty = GetFrameMtx("Armature_mushroom_spine02");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);

	// キャラクター押し戻しコライダーと
	// ダメージを受けるコライダーと攻撃コライダーをマッシュルームの根の行列にアタッチ
	const CMatrix* rootMty = GetFrameMtx("Armature_mushroom_root");
	mpColliderSphereRoot->SetAttachMtx(rootMty);
	mpDamageColRoot->SetAttachMtx(rootMty);
	mpAttackColRoot->SetAttachMtx(rootMty);

	// 最初の攻撃コライダーを無効にしておく
	mpAttackColHead->SetEnable(false);
	mpAttackColRoot->SetEnable(false);
}

CMushroom::~CMushroom()
{
	SAFE_DELETE(mpColliderLine);

	// キャラの押し戻しコライダー
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereRoot);

	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColUmbrella);
	SAFE_DELETE(mpDamageColRoot);

	// 攻撃コライダー
	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColRoot);
}

// インスタンス
CMushroom* CMushroom::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CMushroom::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 状態の切り替え
void CMushroom::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttack2Step = 0;
	mStateAttack3Step = 0;
}

// 戦う前の待機状態
void CMushroom::UpdateIdle()
{
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE)
	{
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle2);
		}
	}
}

// 戦う前の待機状態2
void CMushroom::UpdateIdle2()
{
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// 待機状態3
void CMushroom::UpdateIdle3()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE && player->Position().Y() < 0.7f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorp <= 30.0f && player->Position().Y() >= 0.7f)
	{
		ChangeState(EState::eIdle3);
	}
}

// 攻撃
void CMushroom::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eAttack);
	if (mAnimationFrame >= 13.0f)
	{
		AttackStart();
		// 攻撃終了待ち状態へ移行
		ChangeState(EState::eAttackWait);
	}
}

// 攻撃2
void CMushroom::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.6f);

	// ステップごとに処理を分ける
	switch (mStateAttack2Step)
	{
		// ステップ0 : 攻撃2アニメーション開始＋攻撃開始(一回目)
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		if (mAnimationFrame >= 5.0f && mAnimationFrame <= 10.0f)
		{
			AttackStart();
			mStateAttack2Step++;
		}
		break;
		// ステップ1 : 攻撃終了
	case 1:
		if (mAnimationFrame >= 11.0f && mAnimationFrame <= 12.0f)
		{
			AttackEnd();
			mStateAttack2Step++;
		}
		break;
		// ステップ2 : 攻撃開始(二回目)
	case 2:
		if (mAnimationFrame >= 13.0f && mAnimationFrame <= 17.0f)
		{
			AttackStart();
			mStateAttack2Step++;
		}
		break;
		// ステップ3 : 攻撃終了
	case 3:
		if (mAnimationFrame >= 18.0f && mAnimationFrame <= 20.0f)
		{
			AttackEnd();
			mStateAttack2Step++;
		}
		break;
		// ステップ4 : 攻撃開始(三回目)
	case 4:
		if (mAnimationFrame >= 21.0f && mAnimationFrame < 25.0f)
		{
			AttackStart();
			mStateAttack2Step++;
		}
		break;
		// ステップ5 : 攻撃終了
	case 5:
		if (mAnimationFrame >= 25.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}


// 攻撃3
void CMushroom::UpdateAttack3()
{
	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();
	SetAnimationSpeed(0.5f);
	mpAttackColHead->SetEnable(false);

	// ステップごとに処理を分ける
	switch (mStateAttack3Step)
	{
		// ステップ0 : 攻撃3アニメーション開始
	case 0:
		ChangeAnimation(EAnimType::eAttack3);
		mStateAttack3Step++;
		break;
		// ステップ1 : 攻撃開始
	case 1:
		if (mAnimationFrame >= 10.0f)
		{
			AttackStart();
			mStateAttack3Step++;
		}
		break;
		// ステップ2 : 攻撃の時に前に移動するか止まるか
	case 2:
		if (mAnimationFrame >= 0.0f)
		{
			if (vectorp <= 25.0f)
			{
				mMoveSpeed += nowPos * 7.0f;
				mStateAttack3Step++;
			}
			else if (vectorp >= 26.0f)
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				mStateAttack3Step++;
			}
		}
		break;
		// ステップ3 : 攻撃終了待ち
	case 3:
		if (mAnimationFrame >= 25.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃終了待ち
void  CMushroom::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle3);
	}
}

// ヒット
void CMushroom::UpdateHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	SetAnimationSpeed(0.4f);
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
			ChangeState(EState::eIdle3);
		}
	}
}

// 死ぬ
void CMushroom::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.25f);
	// 死ぬ時のアニメーションを開始
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::MushroomDeath();
	}
}

// めまい(混乱)
void CMushroom::UpdateDizzy()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.4f);
	// めまい(混乱)アニメーションを開始
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// プレイヤーの攻撃がヒットした時の待機状態へ移行
		ChangeState(EState::eIdle3);
		ChangeAnimation(EAnimType::eIdle4);
	}
}

// 走る
void CMushroom::UpdateRun()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eRun);
	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// 範囲内の時、移動し追跡する
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE && player->Position().Y() < 0.7f)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
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
	if (vectorp <= 30.0f && player->Position().Y() >= 0.7f)
	{
		ChangeState(EState::eIdle3);
	}
	// 追跡が止まった時、攻撃用の待機モーションへ
	else if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		ChangeState(EState::eIdle3);
	}
}

// 更新処理
void CMushroom::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mHp = mCharaStatus.hp;
	mMoveSpeed.Y(0.0f);

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 戦う前の待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
		// 戦う前の待機状態2
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
		// 歩行
	case EState::eRun:
		UpdateRun();
		break;
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eIdle3 && mState != EState::eAttack &&
		mState != EState::eAttack2 && mState != EState::eAttack3 && mState != EState::eAttackWait && mState != EState::eHit
		&& mState != EState::eDizzy && mState != EState::eDie && mState != EState::eRun)
	{
		ChangeState(EState::eIdle);
	}

	if (mState == EState::eRun || mState == EState::eIdle3 || mState == EState::eAttack || mState == EState::eAttack2 ||
		mState == EState::eAttack3 || mState == EState::eHit || mState == EState::eDizzy|| mState == EState::eAttackWait)
	{
		// HPゲージの座標を更新(敵の座標の少し上の座標)
		CVector gaugePos = Position() + CVector(0.0f, 30.0f, 0.0f);

		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle3|| mState == EState::eRun)
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
			// 確率を最小に3最大6
			int probability2 = Math::Rand(2, 5);
			int probability3 = Math::Rand(6, 10);
			if (probability2 == 2)Attack2 = true;
			if (probability3 == 6)Attack3 = true;
			if (Attack2)
			{
				ChangeState(EState::eAttack2);
			}
			else if (Attack3)
			{
				ChangeState(EState::eAttack3);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3 || mState == EState::eAttackWait)
		{
			mAttackTime = 0;
		}
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE || mState==EState::eAttack3)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (Position().Y() >= 0.5f)
	{
		Position(Position().X(), Position().Y() * 0.5f, Position().Z());
	}

	if (player->Position().Y() >= 16.0f)
	{
		mpDamageColUmbrella->SetEnable(true);
	}
	if (player->Position().Y() >= 10.0f)
	{
		mpColliderSphereHead->SetEnable(true);
	}
	if (player->Position().Y() <= 0.5f)
	{
		mpDamageColUmbrella->SetEnable(false);
	}
	if (player->Position().Y() <= 5.0f)
	{
 		mpColliderSphereHead->SetEnable(false);
	}

	// キャラクターの更新
	CXCharacter::Update();
	
	// キャラの押し戻しコライダー
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereRoot->Update();

	// ダメージを受けるコライダー
	mpDamageColBody->Update();
	mpDamageColUmbrella->Update();
	mpDamageColRoot->Update();

	// 攻撃コライダー
	mpAttackColHead->Update();
	mpAttackColRoot->Update();

	mIsGrounded = false;

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
	CDebugPrint::Print("攻撃 %d\n",mAttackTime);
	CDebugPrint::Print("HP %d\n", mCharaStatus.hp);
}

// 衝突処理
void CMushroom::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば、
	if (self == mpAttackColHead || self == mpAttackColRoot&& mState != EState::eIdle && mState != EState::eIdle2 &&
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
	else if (self == mpColliderSphereHead || self == mpColliderSphereBody || self == mpColliderSphereRoot)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 攻撃開始
void CMushroom::AttackStart()
{
	CXCharacter::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	if (mState == EState::eAttack || mState==EState::eAttack2)
	{
		mpAttackColHead->SetEnable(true);
	}
	if (mState == EState::eAttack3)
	{
		mpAttackColRoot->SetEnable(true);
	}
}

// 攻撃終了
void CMushroom::AttackEnd()
{
	CXCharacter::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackColHead->SetEnable(false);
	mpAttackColRoot->SetEnable(false);
}

// 描画
void CMushroom::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CMushroom::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CMushroom::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY2_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 被ダメージ処理
void CMushroom::TakeDamage(int damage, CObjectBase* causedObj)
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
void CMushroom::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}