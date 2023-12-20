//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CHpGauge.h"
#include "CSaGauge.h"
#include "Maths.h"
#include "CSword.h"
#include "CShield.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

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
	{ "Character\\Player\\animation\\DogAttack.x",	true,	91.0f	},  // 攻撃
	{ "Character\\Player\\animation\\DogJump.x",	true,	49.0f	},  // ジャンプ
	{ "Character\\Player\\animation\\DogAttack2.x",	true,	140.0f	},  // 攻撃2
	{ "Character\\Player\\animation\\DogAttack3.x",	true,	91.0f	},  // 攻撃3
	{ "Character\\Player\\animation\\DogAttack4.x",	true,	105.0f	},  // 攻撃4
	{ "Character\\Player\\animation\\DogAttack5.x",	true,	101.0f	},  // 攻撃5 101.0f
	{ "Character\\Player\\animation\\DogAttack6.x",	true,	219.0f	},  // 攻撃6
	{ "Character\\Player\\animation\\DogAttack7.x",	true,	140.0f	},  // 攻撃7 213.0f
	{ "Character\\Player\\animation\\DogPowerUp.x",	true,	143.0f	},  // 攻撃力アップ
	{ "Character\\Player\\animation\\DogHit.x",	true,	60.0f	},      // ヒット 43.0f
	{ "Character\\Player\\animation\\DogGuard.x",	false,	47.0f	},      // ガード 47.0f
	{ "Character\\Player\\animation\\DogGuardHit.x",	false,	47.0f	},  // ガードヒット 47.0f
	{ "Character\\Player\\animation\\DogRolling.x",	true,	71.0f	},      // ヒット 43.0f
	//{ "Character\\Player\\animation\\DogImpact.x",	true,	43.0f	},  // 衝撃
	//{ "Character\\Player\\animation\\DogDie.x",	true,	235.0f	},  // 死ぬ

	//{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// ジャンプ開始
	//{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// ジャンプ中
	//{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// ジャンプ終了
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
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Player");
	// デフォルトスケールの反映
	Scale(CVector::one * DEFAULT_SCALE);

	// HPゲージを作成
	mpHpGauge = new CHpGauge();
	mpHpGauge->SetPos(10.0f, 63.0f);

	// SAゲージを作成
	mpSaGauge = new CSaGauge();
	mpSaGauge->SetPos(10.0f,105.0f);

	// 最初に1レベルに設定
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
		this, ELayer::ePlayer,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// キャラクター同士の押し戻しコライダー
	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::ePlayer,
		0.4f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::eEnemy,ELayer::eEnemy2 });

	///ダメージを受けるコライダーを作成
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.4f,
		false
	);
	//ダメージを受けるコライダーと
	//衝突判定を行うコライダーのレイヤーとタグを設定
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eEnemy });
	//ダメージを受けるコライダーを少し上へずらす
	mpDamageCol->Position(-0.05f, 0.7f, 0.0f);

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
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
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
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	
	if (mIsGrounded)
	{
		// 移動処理
		// キーの入力ベクトルを取得
		CVector input;
		if (CInput::Key('W'))
		{
			input.Z(-1.0f);
			// SPACEキーで回避
			if (CInput::Key(VK_SPACE))
			{
				RollingCount();
			}
		}
		else if (CInput::Key('S'))
		{
			input.Z(1.0f);
			if (CInput::Key(VK_SPACE))
			{
				RollingCount();
			}
		}
		if (CInput::Key('A'))
		{
			input.X(-1.0f);
			if (CInput::Key(VK_SPACE))
			{
				RollingCount();
			}
		}
		else if (CInput::Key('D'))
		{
			input.X(1.0f);
			if (CInput::Key(VK_SPACE))
			{
				RollingCount();
			}
		}

		// 入力ベクトルの長さで入力されているか判定
		if (input.LengthSqr() > 0.0f)
		{
			// カメラの向きに合わせた移動ベクトルに変換
			CVector move = CCamera::MainCamera()->Rotation() * input;
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * MOVE_SPEED * mCharaStatus.mobility;

			// 歩行アニメーションに切り替え
			ChangeAnimation(EAnimType::eWalk);
			mpDamageCol->SetEnable(true);
		}
		// 移動キーを入力していない
		else
		{
			// 待機アニメーションに切り替え
			ChangeAnimation(EAnimType::eIdle);
			mpDamageCol->SetEnable(true);
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
		if (mAttackTime >= 300 ||mAttackCount <=0)
		{
			mAttackCount = 0;
			mAttackTime = 0;
		}
		if (CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			//	mState = EState::ePowerUp;
		}
		if (mCharaStatus.SpecialAttack > 0)
		{
			if (CInput::PushKey('K'))
			{
				//mMoveSpeed.X(0.0f);
				//mMoveSpeed.Z(0.0f);
				mState = EState::eAttack7;
				//mCharaStatus.SpecialAttack--;
			}
		}

		if (CInput::Key('G'))
		{
			mState = EState::eGuard;
		}
		if (CInput::PushKey('J'))
		{
			mState = EState::eJumpStart;
		}
	}
	else
	{
		// 待機アニメーションに切り替え
		ChangeAnimation(EAnimType::eIdle);
		mpDamageCol->SetEnable(true);
	}
}

// 攻撃
void CPlayer::UpdateAttack()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack);
	
    // 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;

	//剣に攻撃開始を伝える
	mpSword->AttackStart();
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
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack4);
	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;

	//剣に攻撃開始を伝える
	mpSword->AttackStart();
}

// 攻撃5
void CPlayer::UpdateAttack5()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack5);
	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;

	//剣に攻撃開始を伝える
	mpSword->AttackStart();
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
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack7);

	if (mAnimationFrame >= 40.0f&& mAnimationFrame <= 41.0f)
	{
		mpSword->AttackStart();
	}
	else if (mAnimationFrame == 41.0f)
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
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
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
		mState = EState::eIdle;
	}
}

// ガード
void CPlayer::UpdateGuard()
{
	if (CInput::Key('G'))
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
		//mRollingTime--;
		mState = EState::eIdle;
	}
	else
	{
		mpDamageCol->SetEnable(false);
	}
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
	CCamera* mainCamera = CCamera::MainCamera();
	if (mainCamera != nullptr)
	{
		CVector diff = DEFAULT_CAMERA_POS - mDefaultPos;
		diff.Y(diff.Y() +   mCharaStatus.cameraHeight);
		//diff.Z(diff.Z() + mCharaStatus.cameraHeight);
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

// 回避カウント
void CPlayer::RollingCount()
{
	mRollingCount--;
	if (mRollingCount >= 0)
	{
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
	}

	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	CDebugPrint::Print(" %.1fFPS( Delta:%f)\n", Time::FPS(), Time::DeltaTime());
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

	CDebugPrint::Print(" 回避回数: %d\n", mRollingCount);
	CDebugPrint::Print(" 回避回数を増やす時間: %d\n", mRollingTime);

	if (mRollingCount < 1)
	{
		mRollingTime++;
	}
	if (mRollingTime >= 500)
	{
		mRollingCount++;
		mRollingTime = 0;
	}


	AutomaticRecovery();
	//RollingCount();

	// キャラクターの更新
	CXCharacter::Update();

	/*mpSword->Update();
	mpShield->Update();*/

	mIsGrounded = false;

	if (mState == EState::eHit)
	{
		SetColor(CColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		SetColor(CColor(1.0f, 1.0f, 1.0f, 1.0f));
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
		CDebugPrint::Print(" G: ガード\n");
		CDebugPrint::Print(" WASD+スペース: 回避\n");
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
	//死亡していたら、ダメージは受けない
	//if (mCharaStatus.hp <= 0)return;

	//HPからダメージを引く
	//mCharaStatus.hp = max(mCharaStatus.hp - damage, 0);
	//mCharaStatus.hp -= damage;
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