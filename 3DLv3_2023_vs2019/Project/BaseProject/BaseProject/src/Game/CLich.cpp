#include "CLich.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CMagicCircle.h"
#include "CBee.h"
#include "CBeholder.h"
#include "CBoxer.h"
#include "CCactus.h"
#include "CChest.h"
#include "CMushroom.h"
#include "CRay.h"
#include "CSlime.h"
#include "CTurtle.h"
#include "CDrainEffect.h"
#include "CShieldRotate.h"
#include "CHealCircle.h"
#include "CIceBreath.h"
#include "CHit.h"
#include "CGameEnemyUI.h"
#include "Maths.h"
#include "CInput.h"

// リッチのインスタンス
CLich* CLich::spInstance = nullptr;

float CLich::mElapsedTime;

#define ENEMY_HEIGHT    -4.1f  // 縦の長さ
#define MOVE_SPEED       0.3f  // 移動速度
#define WALK_RANGE     300.0f  // 追跡する範囲
#define STOP_RANGE      50.0f  // 追跡を辞める範囲
#define ROTATE_RANGE   250.0f  // 回転する範囲
#define MC_ANIM_TIME     1.0f  // 魔法陣のアニメーション時間
#define MC_ANIM_MOVE_Y  38.0f  // 魔法陣の高さ移動量
#define MC_ANIM_ROT_Y  360.0f  // 魔法陣ののY軸回転量

const CLich::SpawnData CLich::SPAWN_DATA[] =
{
	//  敵の種類　　　　　　 距離   魔法陣のオフセット位置    魔法陣の色               魔法陣の大きさ  敵の大きさ  召喚するタイミング
	{ EEnemyType::eBee,      50.0f, CVector(0.0f,-9.6f,0.0f), CColor(0.7f,0.0f,1.0f),  25.0f,          30.0f,      0.25f },
	{ EEnemyType::eBeholder, 50.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,1.0f),  25.0f,          18.0f,       0.5f },
	{ EEnemyType::eBoxer,    50.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,1.0f,0.0f),  35.0f,          25.0f,      0.25f },
	{ EEnemyType::eCactus,   50.0f, CVector(0.0f,-9.6f,0.0f), CColor(0.0f,1.0f,0.0f),  35.0f,          35.0f,      0.25f },
	{ EEnemyType::eChest,    50.0f, CVector(0.0f,-9.6f,0.0f), CColor(0.85f,0.6f,0.0f), 35.0f,          35.0f,      0.25f },
	{ EEnemyType::eMushroom, 50.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.85f,0.6f), 30.0f,          35.0f,      0.25f },
	{ EEnemyType::eRay,      50.0f, CVector(0.0f,-9.6f,0.0f), CColor(0.0f,1.0f,1.0f),  30.0f,          40.0f,      0.25f },
	//{ EEnemyType::eSlime,    50.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,0.0f),  30.0f,          30.0f,      0.25f },
	{ EEnemyType::eTurtle,   50.0f, CVector(0.0f,-9.6f,0.0f), CColor(0.0f,0.0f,1.0f),  30.0f,          30.0f,      0.25f },
};

// リッチのアニメーションデータのテーブル
const CLich::AnimData CLich::ANIM_DATA[] =
{
	{ "",								                  true,	   0.0f,  0.0f},  // Tポーズ
	{ "Character\\Enemy\\Lich\\animation\\LichIdle.x",	  true,   41.0f,  0.4f},  // 待機 41.0f
	{ "Character\\Enemy\\Lich\\animation\\LichAttack.x",  false,  41.0f,  0.4f},  // 攻撃 41.0f
	{ "Character\\Enemy\\Lich\\animation\\LichAttack2.x", false,  71.0f,  0.5f},  // 攻撃 71.0f
	{ "Character\\Enemy\\Lich\\animation\\LichGetHit.x",  false,  41.0f,  0.4f},  // ヒット 41.0f
	{ "Character\\Enemy\\Lich\\animation\\LichDie.x",	  false,  29.0f,  0.2f},  // 死ぬ 29.0f
	{ "Character\\Enemy\\Lich\\animation\\LichVictory.x", false,  81.0f,  0.4f},  // 勝利 81.0f
	{ "Character\\Enemy\\Lich\\animation\\LichRun.x",	  true,	  21.0f,  0.5f},  // 走る 21.0f
};

// コンストラクタ
CLich::CLich()
	: mState(EState::eIdle)
	, mAttackTime(0)
	, mStateStep(0)
	, mpRideObject(nullptr)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
	, mpSpawnEnemy(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// 敵の種類
	mType = EEnemyType::eRich;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Lich");

	//最初に1レベルに設定
	ChangeLevel(1);

	mElapsedTime = 0.0f;

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

	SetAnimationSpeed(0.4f);
	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// キャラクターの押し戻しコライダーを作成(体)
	mpColCapsuleBody = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, 1.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f),
		13.0f, false, 20.0f
	);
	mpColCapsuleBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColCapsuleBody->Position(0.0f, -5.5f, -1.0f);

	// キャラクターの押し戻しコライダーを作成(左腕)
	mpColSphereArmL = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.35f, false, 20.0f
	);
	mpColSphereArmL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColSphereArmL->Position(0.0f, -0.15f, 0.0f);

	// キャラクターの押し戻しコライダーを作成(右腕)
	mpColSphereArmR = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.35f, false, 20.0f
	);
	mpColSphereArmR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColSphereArmR->Position(0.0f, -0.15f, 0.0f);

	// ダメージを受けるコライダーを作成(体)
	mpDamageColBody = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, 1.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f),
		13.0f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, -5.5f, -1.0f);

	// ダメージを受けるコライダーを作成(左腕)
	mpDamageColArmL = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f, false
	);
	mpDamageColArmL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColArmL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColArmL->Position(0.0f, -0.15f, 0.0f);

	// ダメージを受けるコライダーを作成(右腕)
	mpDamageColArmR = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f, false
	);
	mpDamageColArmR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColArmR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColArmR->Position(0.0f, -0.15f, 0.0f);

	// 押し戻しコライダーとダメージを受けるコライダーをリッチの体の行列にアタッチ
	const CMatrix* bodyMtx = GetFrameMtx("Armature_Spine");
	mpColCapsuleBody->SetAttachMtx(bodyMtx);
	mpDamageColBody->SetAttachMtx(bodyMtx);

	// 押し戻しコライダーとダメージを受けるコライダーをリッチの左腕の行列にアタッチ
	const CMatrix* armLeftMtx = GetFrameMtx("Armature_Hand_L");
	mpColSphereArmL->SetAttachMtx(armLeftMtx);
	mpDamageColArmL->SetAttachMtx(armLeftMtx);

	// 押し戻しコライダーとダメージを受けるコライダーをリッチの右腕の行列にアタッチ
	const CMatrix* armRightMtx = GetFrameMtx("Armature_Hand_R");
	mpColSphereArmR->SetAttachMtx(armRightMtx);
	mpDamageColArmR->SetAttachMtx(armRightMtx);

	CVector forward = VectorZ();
	forward.Y(0.0f);
	forward.Normalize();
	CVector DrainPos = Position() + forward * 55.0f + CVector(0.0f, 15.0f, 0.0f);
	// ドレイン
	mpDrain = new CDrainEffect
	(
		this, nullptr,
		DrainPos,
		CQuaternion(0.0, 0.f, 0.0f).Matrix()

	);

	// シールドとの距離
	float ShieldDist = 10.0f;
	// 回転するシールド
	mpShieldRotate = new CShieldRotate(0.0f, ShieldDist);
	mpShieldRotate->SetOwner(this);

	// 回転するシールド2
	mpShieldRotate2 = new CShieldRotate(180.0, ShieldDist);
	mpShieldRotate2->SetOwner(this);

	// 回転するシールド3
	mpShieldRotate3 = new CShieldRotate(-270.0f, ShieldDist);
	mpShieldRotate3->SetOwner(this);

	// 回転するシールド4
	mpShieldRotate4 = new CShieldRotate(270.0f, ShieldDist);
	mpShieldRotate4->SetOwner(this);

	// 回復サークル
	mpHealCircle = new CHealCircle();
	mpHealCircle->SetColor(CColor(0.0f, 1.0f, 0.0f));
	mpHealCircle->Position(0.0f, -2.0f, 0.0f);
	mpHealCircle->SetShow(false);
	mpHealCircle->SetOwner(this);

	const CMatrix* mtx = GetFrameMtx("Armature_Lower_Arm_L");
	CVector IceBreathPos = CVector(0.0f, 60.0f, 0.0f);
	// アイスブレスの生成
	mpIceBreath = new CIceBreath
	(
		this, mtx,
		IceBreathPos,
		CQuaternion(0.0, 90.f, 0.0f).Matrix()
	);

	float Size = 70.0f;
	float Height = 6.0f;
	mpHitEffect = new CHit(Size, Height);
	mpHitEffect->SetOwner(this);
	mpHitEffect->Position(Position());

	mpGameUI->SetUIoffSetPos(CVector(0.0f, 40.0f, 0.0f));

	// Lv.を設定
	mpGameUI->SetLv();
	// レベルを設定
	std::string level = "81";
	mpGameUI->SetEnemyLevel(level);
	// 名前を設定
	std::string name = "召喚師 リッチ";
	mpGameUI->SetEnemyName(name);
}

// デストラクタ
CLich::~CLich()
{
	// 線分コライダー
	SAFE_DELETE(mpColliderLine);
	// キャラクターの押し戻しコライダー
	SAFE_DELETE(mpColCapsuleBody);
	SAFE_DELETE(mpColSphereArmL);
	SAFE_DELETE(mpColSphereArmR);
	// ダメージを受けるコライダー
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColArmL);
	SAFE_DELETE(mpDamageColArmR);

	// 自身が召喚した敵が存在すれば
	if (mpSpawnEnemy)
	{
		mpSpawnEnemy->SetSummoner(nullptr);
	}
}

// インスタンス
CLich* CLich::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CLich::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 状態の切り替え
void CLich::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

CEnemy* CLich::SpawnEnemy(EEnemyType type) const
{
	CEnemy* ret = nullptr;

	switch (type)
	{
	case EEnemyType::eBee:
		ret = new CBee();
		break;
	case EEnemyType::eBeholder:
		ret = new CBeholder();
		break;
	case EEnemyType::eBoxer:
		ret = new CBoxer();
		break;
	case EEnemyType::eCactus:
		ret = new CCactus();
		break;
	case EEnemyType::eChest:
		ret = new CChest();
		break;
	case EEnemyType::eMushroom:
		ret = new CMushroom();
		break;
	case EEnemyType::eRay:
		ret = new CRay();
		break;
	case EEnemyType::eSlime:
		//ret = new CSlime();
		break;
	case EEnemyType::eTurtle:
		ret = new CTurtle();
		break;
	}
	return ret;
}

// 待機状態
void CLich::UpdateIdle()
{
	SetAnimationSpeed(0.4f);
	// 待機アニメーションに切り替え
	ChangeAnimation(EAnimType::eIdle);

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= 70.0f)
	{
		ChangeState(EState::eIdle2);
	}
	else if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// 待機状態2
void CLich::UpdateIdle2()
{
	SetAnimationSpeed(0.4f);
	// 待機アニメーションに切り替え
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >= 10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE
		&& player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 30.0f)
	{
		ChangeState(EState::eIdle2);
	}
}

// 攻撃
void CLich::UpdateAttack()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eAttack);

	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			if (!mpDrain->IsThrowing())
			{
				mpDrain->Start();
				mStateStep++;
			}
		}
		break;
	case 2:
		if (mAnimationFrame >= 39.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// 攻撃2
void CLich::UpdateAttack2()
{
	CPlayer* player = CPlayer::Instance();
	CVector dir = player->Position() - Position();
	dir.Y(0.0f);
	dir.Normalize();
	Rotation(CQuaternion::LookRotation(dir));

	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack2);
	if (mAnimationFrame >= 25.0f)
	{
		if (!mpIceBreath->IsThrowing())
		{
			mpIceBreath->Start();
		}
	}
	if (mAnimationFrame >= 50.0f)
	{
		mpIceBreath->Stop();
	}
	if (mAnimationFrame >= 71.0f)
	{
		ChangeState(EState::eAttackWait);
	}
}

// 攻撃終了待ち
void CLich::UpdateAttackWait()
{
	mpDrain->Stop();
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// ヒット
void CLich::UpdateHit()
{
	mpDrain->Stop();
	mpIceBreath->Stop();
	SetAnimationSpeed(0.4f);

	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eHit);
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle2);
		}
		break;
	}
}

// 死ぬ
void CLich::UpdateDie()
{
	mpDrain->Stop();
	mpIceBreath->Stop();

	CHpGauge* hpGauge = mpGameUI->GetHpGauge();
	hpGauge->SetShow(false);

	SetAnimationSpeed(0.2f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// エネミーの死亡処理
		CEnemy::BoxerDeath();
	}
}

// 召喚
void CLich::UpdateSummon()
{
	SetAnimationSpeed(0.4f);
	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eSummon);
		mStateStep++;
		break;
	// ランダム召喚開始
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			if (mpSpawnEnemy == nullptr)
			{
				mRandomSummonIndex = GetRandomSummonIndex();
				if (mRandomSummonIndex < 0)
				{
					mStateStep = 4;
				}
				else
				{
					SpawnData data = SPAWN_DATA[mRandomSummonIndex];

					CVector forward = VectorZ();
					forward.Y(0.0f);
					forward.Normalize();
					CVector magicPos = Position() + forward * data.dist + data.offsetPos;
					mpMagicCircle = new CMagicCircle
					(
						this,
						magicPos
					);
					mpMagicCircle->SetColor(data.circleColor);
					mpMagicCircle->Scale(CVector::one * data.circleScale);
					
					mMCStartPos = mpMagicCircle->Position();
					mMCStartRot = mpMagicCircle->Rotation();

					mStateStep++;
					mElapsedTime = 0.0f;
				}
			}
			else
			{
				mStateStep = 4;
			}
		}
		break;
		// 魔法陣のアニメーション
	case 2:
		if (mElapsedTime < MC_ANIM_TIME)
		{
			// 時間の経過に合わせて位置の回転値を変更
			float percent = mElapsedTime / MC_ANIM_TIME;
			
			SpawnData data = SPAWN_DATA[mRandomSummonIndex];
			if (mpSpawnEnemy == nullptr && percent >= data.spawnRatio)
			{
				RandomSummon();
			}

			// 位置
			CVector targetPos = mMCStartPos + CVector(0.0f, MC_ANIM_MOVE_Y, 0.0f);
			CVector pos = CVector::Lerp(mMCStartPos, targetPos, percent);
			mpMagicCircle->Position(pos);

			// 回転値
			CVector startAngle = CVector(0.0f, 0.0f, 0.0f);
			CVector targetAngle = CVector(0.0f, MC_ANIM_ROT_Y, 0.0f);
			CQuaternion targetRot = mMCStartRot * CQuaternion(0.0f, MC_ANIM_ROT_Y, 0.0f);
			CVector angle = CVector::Lerp(startAngle, targetAngle, percent);
			CQuaternion rot = mMCStartRot * CQuaternion(angle);
			mpMagicCircle->Rotation(rot);

			mElapsedTime += Time::DeltaTime();
		}
		// アニメーション終了
		else
		{
			if (mpSpawnEnemy == nullptr)
			{
				RandomSummon();
			}

			// 終了時の位置と回転値を設定
			CVector targetPos = mMCStartPos + CVector(0.0f, MC_ANIM_MOVE_Y, 0.0f);
			mpMagicCircle->Position(targetPos);
			CVector targetAngle = CVector(0.0f, MC_ANIM_ROT_Y, 0.0f);
			CQuaternion rot = mMCStartRot * CQuaternion(targetAngle);
			mpMagicCircle->Rotation(rot);

			mStateStep++;
			mElapsedTime = 0.0f;
		}
		break;
	// 魔法陣の待ち時間
	case 3:
		if (mElapsedTime < 1.0f)
		{
			mElapsedTime += Time::DeltaTime();
		}
		else
		{
			mpMagicCircle->Kill();
			mpMagicCircle = nullptr;

			mStateStep++;
			mElapsedTime = 0.0f;
		}
		break;
	case 4:
		if (mAnimationFrame >= 81.0f)
		{
			ChangeState(EState::eIdle2);
		}
		break;
	}
}

// 移動
void CLich::UpdateRun()
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

// ランダム召喚する敵のインデックス値を取得
int CLich::GetRandomSummonIndex() const
{
	int tableSize = ARRAY_SIZE(SPAWN_DATA);
	int index = Math::Rand(0, tableSize - 1);
	
	if (!(0 <= index && index < tableSize)) return -1;
	return index;
}

// ランダム召喚
void CLich::RandomSummon()
{
	SpawnData data = SPAWN_DATA[mRandomSummonIndex];

	CVector forward = VectorZ();
	forward.Y(0.0f);
	forward.Normalize();
	CVector magicPos = Position() + forward * data.dist + data.offsetPos;

	// 召喚する敵を生成
	mpSpawnEnemy = SpawnEnemy(data.type);
	if (mpSpawnEnemy != nullptr)
	{
		mpSpawnEnemy->Position(magicPos);
		mpSpawnEnemy->Scale(CVector::one * data.monsterScale);
		mpSpawnEnemy->Rotation(CQuaternion::LookRotation(forward));
		mpSpawnEnemy->SetSummoner(this);
	}
}

// HP吸収または回復
void CLich::DrainHp()
{
	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		int Heal = 0;
		Heal = mCharaMaxStatus.hp * 0.25;
		mCharaStatus.hp += Heal;
		mpHealCircle->StartCircle();
		
		if (mCharaStatus.hp > mCharaMaxStatus.hp)
		{
			mCharaStatus.hp = mCharaMaxStatus.hp;
		}
	}
}
	
//更新処理
void CLich::Update()
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
		// 死ぬ
	case EState::eDie:
		UpdateDie();
		break;
		// 召喚
	case EState::eSummon:
		UpdateSummon();
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
		if (vectorPos <= ROTATE_RANGE)
		{
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}

		mAttackTime++;
		if (mAttackTime >= 250 && mpSpawnEnemy == nullptr)
		{
			ChangeState(EState::eSummon);
		}
		else if(mAttackTime >= 300 && mpSpawnEnemy != nullptr)
		{
			bool Attack2 = false;
			int random = Math::Rand(0, 2);
			if (random == 2)Attack2 = true;
			if (Attack2)
			{
				ChangeState(EState::eAttack2);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}

		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eSummon)
		{
			mAttackTime = 0;
		}
	}

	if (mState == EState::eRun)
	{
		if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
		{
			Position(Position() + mMoveSpeed);
		}
	}

	if (mpSpawnEnemy != nullptr)
	{
		mDefenseUp = true;
	}
	else
	{
		mDefenseUp = false;
	}

	// キャラクターの更新
	CXCharacter::Update();

	// キャラクターの押し戻しコライダー
	mpColCapsuleBody->Update();
	mpColSphereArmL->Update();
	mpColSphereArmR->Update();

	// ダメージを受けるコライダー
	mpDamageColBody->Update();
	mpDamageColArmL->Update();
	mpDamageColArmR->Update();

	if (CInput::PushKey('Z'))
	{
		ChangeState(EState::eAttack2);
	}

	mIsGrounded = false;

	CEnemy::Update();

	if (mState == EState::eIdle || mState == EState::eDie)
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
void CLich::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// キャラクター同士の衝突処理
	else if (self == mpColCapsuleBody || self == mpColSphereArmL || self == mpColSphereArmR)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// 被ダメージ処理
void CLich::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HPからダメージを引く
	if (mCharaStatus.hp -= damage)
	{
		if (mState != EState::eDie)
		{
			mpHitEffect->StartHitEffect();
		}
		if (mState != EState::eSummon)
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

// 召喚した敵の死亡処理
void CLich::DeathSummonEnemy(CEnemy* enemy)
{
	// 自分が召喚した敵でなければ、処理しない
	if (enemy != mpSpawnEnemy) return;

	// 自分が召喚した敵であれば、ポインターを空にする
	mpSpawnEnemy = nullptr;
}

// 死亡処理
void CLich::Death()
{
	// 死亡状態へ移行
	ChangeState(EState::eDie);
}

// 防御力の強化割合を取得
float CLich::GetDefBuff(const CVector& attackDir)const
{
	if (mpSpawnEnemy != nullptr) return 20.0f;

	else if (mState == EState::eSummon) return 2.0f;

	// 通常時の防御の割合
	return mBaseDefenseBuffRatio;
}

// 描画
void CLich::Render()
{
	CXCharacter::Render();
}

// 1レベルアップ
void CLich::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// レベルを変更
void CLich::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = ENEMY10_STATUS[index];
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpGameUI->SetMaxHp(mCharaMaxStatus.hp);
	mpGameUI->SetHp(mCharaStatus.hp);
}