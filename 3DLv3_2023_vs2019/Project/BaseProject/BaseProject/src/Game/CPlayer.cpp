//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CGameCamera2.h"
#include "CHpGauge.h"
#include "CSaGauge.h"
#include "Maths.h"
#include "CSword.h"
#include "CShield.h"
#include "CSlash.h"
#include "CBeamEffect.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;
int CPlayer::mHp;
int CPlayer::mSa;

// プレイヤーのモデルデータのパス
#define MODEL_PATH "Character\\Player\\player.x"

#define MODEL_TURTLE "Character\\Turtle\\Turtle.x"

// プレイヤーのアニメーションデータのテーブル
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	//{ "",										true,	0.0f	},	// Tポーズ
	//{ "Character\\Player\\anim\\DogAttack.x",		true,	91.0f	},	// 待機
	//{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// 歩行

	{ "Character\\Player\\animation\\DogIdle.x",	true,	221.0f	},  // 待機 221.0f
	{ "Character\\Player\\animation\\DogWalk.x",	true,	69.0f	},  // 歩行
	{ "Character\\Player\\animation\\DogAttack.x",	true,   90.0f	},  // 攻撃 91.0f
	{ "Character\\Player\\animation\\DogJump.x",	true,	49.0f	},  // ジャンプ
	{ "Character\\Player\\animation\\DogAttack2.x",	true,	140.0f	},  // 攻撃2
	{ "Character\\Player\\animation\\DogAttack3.x",	true,	91.0f	},  // 攻撃3
	{ "Character\\Player\\animation\\DogAttack4.x",	true,	105.0f	},  // 攻撃4 105.0f
	{ "Character\\Player\\animation\\DogAttack5.x",	true,	101.0f	},  // 攻撃5 101.0f
	{ "Character\\Player\\animation\\DogAttack6.x",	true,	219.0f	},  // 攻撃6
	{ "Character\\Player\\animation\\DogAttack7.x",	true,	190.0f	},  // 攻撃7 213.0f
	{ "Character\\Player\\animation\\DogPowerUp.x",	true,	143.0f	},  // 攻撃力アップ
	{ "Character\\Player\\animation\\DogHit.x",	true,	43.0f	},      // ヒット 43.0f
	{ "Character\\Player\\animation\\DogGuard.x",	false,	47.0f	},      // ガード 47.0f
	{ "Character\\Player\\animation\\DogGuardHit.x",	false,	47.0f	},  // ガードヒット 47.0f
	{ "Character\\Player\\animation\\DogRolling.x",	true,	43.0f	},      // 回避 43.0f
	//{ "Character\\Player\\animation\\DogImpact.x",	true,	43.0f	},  // 衝撃
	{ "Character\\Player\\animation\\DogDie.x",	true,	235.0f	},  // 死ぬ
};

#define PLAYER_HEIGHT 2.0f

#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

#define MOVE_SPEED 0.3f  //移動速度

//デフォルトのスケール値
#define DEFAULT_SCALE 10.0f

bool CPlayer::IsDeath() const
{
	return mCharaStatus.hp <= 0;
}

bool CPlayer::IsDeath2() const
{
	return mCharaStatus.level <= 0;
}

bool CPlayer::IsDeath3() const
{
	return mCharaStatus.power <= 0;
}

// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mpRideObject(nullptr)
	,mRollingTime(0)
	,mRollingCount(1)
	,mAttackTime(0)
	,mAttackCount(0)
	, healcount(0)
	, recoverycount(0)
	, mDefaultPos(CVector::zero)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Player");
	// デフォルトスケールの反映
	Scale(CVector::one * DEFAULT_SCALE);

	// HPゲージを作成
	mpHpGauge = new CHpGauge(false);
	mpHpGauge->SetPos(10.0f, 63.0f);
	mpHpGauge->SetShow(true);

	// SAゲージを作成
	mpSaGauge = new CSaGauge();
	mpSaGauge->SetPos(10.0f,103.5f);

	// 最初に1レベルに設定
	ChangeLevel(41);

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
		this, ELayer::ePlayer,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine->Position(0.0f, 1.0f, 0.0f)
		;
	// キャラクター同士の押し戻しコライダー
	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::ePlayer,
		0.5f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::eEnemy,ELayer::eEnemy2 });
	mpColliderSphere->Position(0.0f,0.4f,0.1f);

	///ダメージを受けるコライダーを作成
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.37f,
		false
	);
	//ダメージを受けるコライダーと
	//衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eEnemy,ETag::eFlame ,ETag::eWave,ETag::eWeapon });
	//ダメージを受けるコライダーを少し上へずらす
	mpDamageCol->Position(-0.05f, 0.3f, 0.0f);

	///ダメージを受けるコライダーを作成
	mpDamageCol2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f,
		false
	);
	//ダメージを受けるコライダーと
	//衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol2->SetCollisionTags({ ETag::eEnemy ,ETag::eFlame, ETag::eWave,ETag::eWeapon });
	//ダメージを受けるコライダーを少し上へずらす
	mpDamageCol2->Position(-0.05f, 0.8f, 0.15f);

	//デフォルト座標を設定
	mDefaultPos = Position();

	//剣を生成して右手に持たせる
	mpSword = new CSword();
	mpSword->SetAttachMtx(GetFrameMtx("Armature_mixamorig_RightHand"));
	mpSword->SetOwner(this);

	//盾を生成して左手に持たせる
	mpShield= new CShield();
	mpShield->SetAttachMtx(GetFrameMtx("Armature_mixamorig_LeftHand"));
	mpShield->SetOwner(this);

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	mpBeam = new CBeamEffect
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f)
	);
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpDamageCol2);
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}


// 待機
void CPlayer::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		if (mAttackTime >= 300 ||mAttackCount <=0)
		{
			mAttackCount = 0;
			mAttackTime = 0;
		}
		
		// スペシャル攻撃
		if (mCharaStatus.SpecialAttack >= 4)
		{
			if (CInput::PushKey(VK_MBUTTON))
			{
				mState = EState::eAttack4;
				mCharaStatus.SpecialAttack -= 4;
			}
		}

		if (CInput::Key(VK_RBUTTON))
		{
			mState = EState::eGuard;
		}
		if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
	}
}

// 歩行
void CPlayer::UpadateWalk()
{
	ChangeAnimation(EAnimType::eWalk);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// 攻撃
void CPlayer::UpdateAttack()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack);

	if (mAnimationFrame >= 35.0f)
	{
		//剣に攻撃開始を伝える
		mpSword->AttackStart();

		// 攻撃終了待ち状態へ移行
		mState = EState::eAttackWait;
	}

	// 斬撃SEの再生済みフラグを初期化
	mIsPlayedSlashSE = false;
}

// 攻撃2
void CPlayer::UpdateAttack2()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack2);

	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;

	//剣に攻撃開始を伝える
	mpSword->AttackStart();
}

// 攻撃3
void CPlayer::UpdateAttack3()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack3);
	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;

	//剣に攻撃開始を伝える
	mpSword->AttackStart();
}

// 攻撃4
void CPlayer::UpdateAttack4()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack4);

	if (mAnimationFrame >= 20.0f)
	{
		//剣に攻撃開始を伝える
		mpSword->AttackStart();
	}
	if (mAnimationFrame >= 80.0f)
	{
		mpSword->AttackEnd();
	}
	if (mAnimationFrame >= 39.0f)
	{
		// 斬撃エフェクトの生成済みフラグを初期化
		mIsSpawnedSlashEffect = false;
		// 斬撃エフェクトを生成していないかつ、アニメーションが39%以上進行したら、
		if (!mIsSpawnedSlashEffect && GetAnimationFrameRatio() >= 0.39f)
		{
			// 斬撃エフェクトを生成して、正面方向へ飛ばす
			CSlash* slash = new CSlash
			(
				this,
				Position() + CVector(0.0f, 10.0f, 0.0f),
				VectorZ(),
				100.0f,
				100.0f
			);
			// 斬撃エフェクトの色設定
			slash->SetColor(CColor(0.15f, 0.5f, 0.5f));
			slash->SetOwner(this);

			mIsSpawnedSlashEffect = true;
		}
	}
	if (mIsSpawnedSlashEffect && mAnimationFrame >=42.0f)
	{
		// 攻撃終了待ち状態へ移行
		mState = EState::eAttackWait;
	}
}

// 攻撃5
void CPlayer::UpdateAttack5()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack5);

	if (mAnimationFrame >= 50.0f)
	{
		//剣に攻撃開始を伝える
		mpSword->AttackStart();

		// 攻撃終了待ち状態へ移行
		mState = EState::eAttackWait;
	}
}

// 攻撃6
void CPlayer::UpdateAttack6()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack6);
	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;

	//剣に攻撃開始を伝える
	mpSword->AttackStart();
}

// 攻撃7
void CPlayer::UpdateAttack7()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack7);

	if (mAnimationFrame >= 30.0f&& mAnimationFrame <= 31.0f)
	{
		mpSword->AttackStart();
	}
	else if (mAnimationFrame == 31.0f)
	{
		//剣に攻撃終了を伝える
		mpSword->AttackEnd();
	}
	else if (mAnimationFrame == 52.0f)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
	}
	if (mAnimationFrame >= 80.0f)
	{
		// 攻撃終了待ち状態へ移行
		mState = EState::eAttackWait;

		//剣に攻撃開始を伝える
		mpSword->AttackStart();
	}
	else if (mAnimationFrame == 81.0f)
	{
		//剣に攻撃終了を伝える
		mpSword->AttackEnd();
	}
}

// 攻撃終了待ち
void CPlayer::UpdateAttackWait()
{
	// 斬撃SEを再生していないかつ、アニメーションが25%以上進行したら、
	if (!mIsPlayedSlashSE && GetAnimationFrameRatio() >= 0.25f)
	{
		// 斬撃SEを再生
		mpSlashSE->Play();
		mIsPlayedSlashSE = true;
	}

	// 攻撃アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);

		//剣に攻撃終了を伝える
		mpSword->AttackEnd();
	}
}

// ジャンプ開始
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	mState = EState::eJump;

	mMoveSpeed += CVector(0.0f, JUMP_SPEED, 0.0f);
	mIsGrounded = false;
}

// ジャンプ中
void CPlayer::UpdateJump()
{
	if (mMoveSpeed.Y() <= 0.0f)
	{
		//ChangeAnimation(EAnimType::eJumpEnd);
		mState = EState::eJumpEnd;
	}
}

// ジャンプ終了
void CPlayer::UpdateJumpEnd()
{
	// ジャンプアニメーションが終了かつ、
	// 地面に接地したら、待機状態へ戻す
	if (IsAnimationFinished() && mIsGrounded)
	{
		mState = EState::eIdle;
	}
}

// 移動の更新処理
void CPlayer::UpdateMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	int speed = 1.5f;
	int speed2 = -1.5f;

	// 移動処理
    // キーの入力ベクトルを取得
	CVector input;
	if (CInput::Key('W'))
	{
		input.Z(speed2);
		// SPACEキーで回避
		if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
		{
			RollingCount();
		}
	}
	else if (CInput::Key('S'))
	{
		input.Z(speed);
		if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
		{
			RollingCount();
		}
	}
	if (CInput::Key('A'))
	{
		input.X(speed2);
		if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
		{
			RollingCount();
		}
	}
	else if (CInput::Key('D'))
	{
		input.X(speed);
		if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
		{
			RollingCount();
		}
	}

	// 入力ベクトルの長さで入力されているか判定
	if (input.LengthSqr() > 0.0f)
	{
		// カメラの向きに合わせた移動ベクトルに変換
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		CVector camSide = CVector::Cross(CVector::up, camForward);
		CVector move = camForward * input.Z() + camSide * input.X();
		move.Y(0.0f);
		move.Normalize();

		mMoveSpeed += move * MOVE_SPEED * mCharaStatus.mobility;

		// 待機状態であれば、歩行アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			// 歩行アニメーションに切り替え
			ChangeAnimation(EAnimType::eWalk);
			mpDamageCol->SetEnable(true);
			mpDamageCol2->SetEnable(true);
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			// 待機アニメーションに切り替え
			ChangeAnimation(EAnimType::eIdle);
			mpDamageCol->SetEnable(true);
			mpDamageCol2->SetEnable(true);
		}
	}

	// 左クリックで攻撃状態へ移行
	if (CInput::PushKey(VK_LBUTTON))
	{
		mAttackCount++;
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		mState = EState::eAttack;
		if (mAttackCount == 2)
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::eAttack5;
		}
		if (mAttackCount == 3)
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::eAttack4;
		}
	}
}


// 攻撃力アップ
void CPlayer::UpdatePowerUp()
{
	ChangeAnimation(EAnimType::ePowerUp);
	// 攻撃力アップ終了
	mState = EState::ePowerUpEnd;
}

void CPlayer::UpdatePowerUpEnd()
{
	// 攻撃アップのアニメーションが終了したら、
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// ヒット
void CPlayer::UpdateHit()
{
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		if (mCharaStatus.hp <= 0)
		{
			mState = EState::eDie;
		}
		else
		{
			mState = EState::eIdle;
		}
	}
}

// ガード
void CPlayer::UpdateGuard()
{
	if (CInput::Key(VK_RBUTTON))
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		ChangeAnimation(EAnimType::eGuard);
	}
	else
	{
		mState = EState::eIdle;
	}
}

// ガード中のヒット
void CPlayer::UpdateGuardHit()
{
	ChangeAnimation(EAnimType::eGuardHit);

	if (CInput::Key('G'))
	{
		mState = EState::eGuard;
	}
	else
	{
		mState = EState::eIdle;
	}
}

// 回避
void CPlayer::UpdateRolling()
{
	ChangeAnimation(EAnimType::eRolling);
	Position(Position() + mMoveSpeed * 60.0f * Time::DeltaTime());
	RollingCount();
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
	else
	{
		mpDamageCol->SetEnable(false);
		mpDamageCol2->SetEnable(false);
	}
}

// 死ぬ
void CPlayer::UpdateDei()
{
	ChangeAnimation(EAnimType::eDie);
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
}

// 現在の経験値を消費してレベルアップ
void CPlayer::LevelUp()
{
	// 現在のレベル
	int level = mCharaStatus.level;
	// 現在の最大ステータス
	CharaStatus current = PLAYER_STATUS[level - 1];
	// 次のレベルまでの経験値が溜まっていたら
	while (mCharaStatus.exp >=current.exp)
	{
		// 経験値を消費して、レベルを加算
		mCharaStatus.exp -= current.exp;
		level++;
		// 次のレベルの最大ステータスを設定
		current = PLAYER_STATUS[level - 1];
	}
	// レベルを切り替え
	ChangeLevel(level);
}

// レベルを変更
void CPlayer::ChangeLevel(int level)
{
	// ステータスのテーブルのインデックス値に変換
	int index =Math::Clamp(level-1,0, PLAYER_LEVEL_MAX);
	// 最大ステータスに設定
	mCharaMaxStatus = PLAYER_STATUS[index];
	// 残りの経験値を記憶しておく
	int remaiExp = mCharaStatus.exp;
	// 現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;
	// 残りの経験値を反映
	mCharaStatus.exp = remaiExp;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);

	mpSaGauge->SetMaxValue(mCharaStatus.SpecialAttack);
	mpSaGauge->SetValue(mCharaStatus.SpecialAttack);

	// 現在値のステータスのスケール値を反映
	Scale(CVector::one * DEFAULT_SCALE * mCharaMaxStatus.volume);

	// 現在のレベルのカメラの高さを設定
	CCamera* mainCamera = CGameCamera2::MainCamera();
	if (mainCamera != nullptr)
	{
		CVector diff = DEFAULT_CAMERA_POS - mDefaultPos;
		diff.Normalize();
		diff.Y(diff.Y() +   mCharaStatus.cameraHeight);
		diff.Z(diff.Z() + mCharaStatus.cameraHeight);
		mainCamera->SetFollowTargetOffset(diff);
	}
}

// HP回復と特殊攻撃(SA)の自動回復
void CPlayer::AutomaticRecovery()
{
	healcount++;
	recoverycount++;
	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		if (healcount > 700)
		{
			mCharaStatus.hp++;
			healcount = 0;
		}
	}
	if (mCharaStatus.hp <= 0)
	{
		healcount = 0;
	}
	if (mCharaStatus.hp == mCharaMaxStatus.hp)
	{
		healcount = 0;
	}
	if (mCharaStatus.SpecialAttack < mCharaMaxStatus.SpecialAttack)
	{
		if (recoverycount > 400)
		{
			mCharaStatus.SpecialAttack++;
			recoverycount = 0;
		}
	}
	if (mCharaStatus.SpecialAttack == mCharaMaxStatus.SpecialAttack)
	{
		recoverycount = 0;
	}
}

// 攻撃が当たったら特殊攻撃(SA)を回復
void CPlayer::AttackRecovery()
{
	if (mCharaStatus.SpecialAttack < mCharaMaxStatus.SpecialAttack)
	{
		mCharaStatus.SpecialAttack++;
	}
}

// 回避カウント
void CPlayer::RollingCount()
{
	if (mRollingCount >= 1)
	{
		mRollingCount--;
		mState = EState::eRolling;
	}
	if (mRollingCount <= 0)
	{
		mRollingCount = 0;
	}
}

// 経験値を加算
void CPlayer::AddExp(int exp)
{
	// 現在のステータスに経験値を加算
	mCharaStatus.exp += exp;
	// 次のレベルまでの経験値を獲得していたら
	if (mCharaStatus.exp >= mCharaMaxStatus.exp)
	{
		// レベルアップ
		LevelUp();
	}
}

// 更新
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mHp = mCharaStatus.hp;
	mSa = mCharaStatus.SpecialAttack;

	if (mAttackCount >= 1)
	{
		mAttackTime++;
	}

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
		case EState::eIdle:
			UpdateIdle();
			break;
		// 歩行
		case EState::eWalk:
			UpadateWalk();
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
		// 攻撃5
		case EState::eAttack5:
			UpdateAttack5();
			break;
		// 攻撃6
		case EState::eAttack6:
			UpdateAttack6();
			break;
		// 攻撃7
		case EState::eAttack7:
			UpdateAttack7();
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
	    // 攻撃力アップ
		case EState::ePowerUp:
			UpdatePowerUp();
			break;
	    // 攻撃力アップ終了
		case EState::ePowerUpEnd:
			UpdatePowerUpEnd();
			break;
	    // ヒット
		case EState::eHit:
			UpdateHit();
			break;
		// ガード
		case EState::eGuard:
			UpdateGuard();
			break;
		// ガードヒット
		case EState::eGuardHit:
			UpdateGuardHit();
			break;
		// 回避
		case EState::eRolling:
			UpdateRolling();
			break;
		// 死ぬ
		case EState::eDie:
			UpdateDei();
			break;
	}

	// 待機中とジャンプ中は、移動処理を行う
	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	//CDebugPrint::Print(" 回避回数: %d\n", mRollingCount);
	//CDebugPrint::Print(" プレイヤーのHP回復時間 %d\n", healcount);
	//CDebugPrint::Print(" 攻撃計測 %d\n", mAttackTime);
	//CDebugPrint::Print(" 攻撃した回数 %d\n", mAttackcount);

	// 移動
	Position(Position() + mMoveSpeed * 60.0f * Time::DeltaTime());

	// プレイヤーを移動方向へ向ける
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	if (mRollingCount < 1)
	{
		mRollingTime++;
	}
	if (mRollingTime >= 300)
	{
		mRollingCount++;
		mRollingTime = 0;
	}

	AutomaticRecovery();

	// 「E」キーで炎の発射をオンオフする
	if (CInput::PushKey('E'))
	{

		// 斬撃エフェクトを生成して、正面方向へ飛ばす
		CSlash* slash = new CSlash
		(
			this,
			Position() + CVector(0.0f, 10.0f, 0.0f),
			VectorZ(),
			100.0f,
			100.0f
		);
		// 斬撃エフェクトの色設定
		slash->SetColor(CColor(0.15f, 0.5f, 0.5f));
		slash->SetOwner(this);
	}

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;

	if (mState == EState::eHit)
	{
		SetColor(CColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		SetColor(CColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (mState == EState::eGuard)
	{
		if (mCharaStatus.hp <= 0)
		{
			mState = EState::eDie;
		}
	}
	if (mCharaStatus.hp <= 0)
	{
		mState = EState::eDie;
	}

	// プレイヤーのデバック表示
	static bool debug = false;
	static bool debug2 = true;
	if (CInput::PushKey('R'))
	{
		debug = !debug;
		debug2 = !debug2;
	}

	if(debug)
	{
		CDebugPrint::Print("    Lv:      %d\n", mCharaStatus.level);
		CDebugPrint::Print("   Exp:     %d  / %d\n", mCharaStatus.exp, mCharaMaxStatus.exp);
		CDebugPrint::Print("    Hp:    %d / %d\n", mCharaStatus.hp,mCharaMaxStatus.hp);
		CDebugPrint::Print(" 攻撃力:    %d\n",mCharaStatus.power/2);
		CDebugPrint::Print(" 防御力:    %d\n", mCharaStatus.defense);
		CDebugPrint::Print("    SA:    %d / %d\n", mCharaStatus.SpecialAttack, mCharaMaxStatus.SpecialAttack);
		CVector scale = Scale();
		CDebugPrint::Print(" スケール値 %f,%f,%f \n", scale.X(), scale.Y(), scale.Z());
		CDebugPrint::Print(" 回避回数: %d\n", mRollingCount);
	}
	if (debug2)
	{
		CDebugPrint::Print(" R: ステータス表示\n");
		CDebugPrint::Print(" 2: レベルアップ\n");
		CDebugPrint::Print(" WASD: 移動\n");
		CDebugPrint::Print(" 右クリック: ガード\n");
		CDebugPrint::Print(" 左クリック: 攻撃\n");
		CDebugPrint::Print(" マウスホイール\n");
		CDebugPrint::Print(" (レベル5以降:使用可) : スペシャル攻撃\n");
		CDebugPrint::Print(" スペース: ジャンプ\n");
		CDebugPrint::Print(" WASD+Sfift: 回避\n");
		CDebugPrint::Print(" 回避のクールタイム: %d\n", mRollingTime);
		CDebugPrint::Print(" %.1fFPS( Delta:%f)\n", Time::FPS(), Time::DeltaTime());
	}

	if (CInput::Key('1'))
	{
		if (CInput::PushKey(VK_UP)) mCharaStatus.hp++;
		else if (CInput::PushKey(VK_DOWN))
		{
			mCharaStatus.hp--;
			mState = EState::eHit;
		}
	}
	else if (CInput::Key('2'))
	{
		mCharaStatus.exp++;
		if (mCharaStatus.exp == mCharaMaxStatus.exp)
		{
			LevelUp();
		}
	}
	else if (CInput::PushKey('3'))
	{
		ChangeLevel(100);
	}

	// HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
	// SAゲージに現在のSAを設定
	mpSaGauge->SetValue(mCharaStatus.SpecialAttack);
}

// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// 他のキャラクターとの押し戻し処理
	else if (self == mpColliderSphere)
	{
		if (other->Layer() == ELayer::eEnemy || other->Layer() == ELayer::eEnemy2)
		{
			CVector pushBack = hit.adjust * hit.weight;
			pushBack.Y(0.0f);
			Position(Position() + pushBack);
		}
	}
}

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}

//被ダメージ処理
void CPlayer::TakeDamage(int damage, CObjectBase* causedObj)
{
	if (mCharaStatus.hp -= damage)
	{
		if (mState == EState::eGuard)
		{
			mState = EState::eGuardHit;
		}
		else
		{
			mState = EState::eHit;
		}
	}

	// ダメージを受けたら、移動を停止
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// 攻撃も中止
	mpSword->AttackEnd();
}


float CPlayer::GetAtkBuff()const
{
	return mBaseAttackBuffRatio;
}

float CPlayer::GetDefBuff(const CVector& attackDir)const
{
	// ガード状態であれば、防御2倍
	if (mState == EState::eGuard)
	{
		float dot = CVector::Dot(attackDir.Normalized(),VectorZ());
		if (dot >= cosf(Math::DegreeToRadian(30.0f)))
		{
			return 2.0f;
		}
	}

	// 通常時の防御の割合
	return mBaseDefenseBuffRatio;
}