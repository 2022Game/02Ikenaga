//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CHpGauge.h"
#include "Maths.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

// プレイヤーのモデルデータのパス
#define MODEL_PATH "Character\\Player\\player.x"
#define MODEL_DOG "Character\\Player\\Dog.x"
//#define MODEL_SLIME "Character\\Slime\\Slime.x"
#define MODEL_TURTLE "Character\\Turtle\\Turtle.x"
//#define MODEL_MUSHROOM "Character\\Mushroom\\Mushroom.x"
#define HP_FRAME_IMAGE "Character\\Player\\HP\\HPframe.png"  //HPフレーム画像
#define HP_IMAGE "Character\\Player\\HP\\HP.png"//HP画像

// プレイヤーのアニメーションデータのテーブル
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	//{ "",										true,	0.0f	},	// Tポーズ
	//{ "Character\\Player\\anim\\DogAttack.x",		true,	91.0f	},	// 待機
	//{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// 歩行

	{ "Character\\Player\\animation\\DogIdle.x",	true,	221.0f	},  //待機 221.0f
	{ "Character\\Player\\animation\\DogWalk.x",	true,	69.0f	},  //歩行
	{ "Character\\Player\\animation\\DogAttack.x",	true,	91.0f	},  //攻撃
	{ "Character\\Player\\animation\\DogJump.x",	true,	49.0f	},  //ジャンプ
	//{ "Character\\Player\\animation\\DogPowerUp.x",	true,	143.0f	},  //攻撃力アップ
	{ "Character\\Player\\animation\\DogAttack2.x",	true,	140.0f	},  //攻撃2
	//{ "Character\\Player\\animation\\DogImpact.x",	true,	43.0f	},  //衝撃
	//{ "Character\\Player\\animation\\DogDie.x",	true,	235.0f	},  //死ぬ

	//{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// ジャンプ開始
	//{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// ジャンプ中
	//{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// ジャンプ終了
};

#define PLAYER_HEIGHT 2.0f

#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

#define HP 10  //HP
#define LEVEL 1  //レベル
#define SA 5   //特殊攻撃
#define POWER 5  //攻撃力
#define MOVE_SPEED 0.3f  //移動速度

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
	,time(0)
{

	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_DOG);
	Scale(10.0f, 10.0f, 10.0f);

	//HPゲージを作成
	mpHpGauge = new CHpGauge();
	mpHpGauge->SetPos(10.0f, 690.f);

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
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	/*mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eField,
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::eField });*/
}

CPlayer::~CPlayer()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}

	if (mpColliderSphere != nullptr)
	{
		delete mpColliderSphere;
		mpColliderSphere = nullptr;
	}

	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
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
		if (CInput::Key('W'))		input.Z(1.0f);
		else if (CInput::Key('S'))	input.Z(-1.0f);
		if (CInput::Key('A'))		input.X(-1.0f);
		else if (CInput::Key('D'))	input.X(1.0f);

		// 入力ベクトルの長さで入力されているか判定
		if (input.LengthSqr() > 0.0f)
		{
			// カメラの向きに合わせた移動ベクトルに変換
			CVector move = CCamera::MainCamera()->Rotation() * input;
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * MOVE_SPEED* mCharaStatus.mobility;

			// 歩行アニメーションに切り替え
			ChangeAnimation(EAnimType::eWalk);
		}
		// 移動キーを入力していない
		else
		{
			// 待機アニメーションに切り替え
			ChangeAnimation(EAnimType::eIdle);
		}

		// 左クリックで攻撃状態へ移行
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::eAttack;
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
		if (CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
		//	mState = EState::ePowerUp;
		}
		time++;
		if (mCharaStatus.SpecialAttack > 0)
		{
			if (CInput::PushKey('K'))
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				mState = EState::eAttack2;
				mCharaStatus.SpecialAttack--;
			}
		}
		if (mCharaStatus.SpecialAttack < 2)
		{
			if (time > 500)
			{
				mCharaStatus.SpecialAttack++;
				time = 0;
			}
			else if (mCharaStatus.SpecialAttack == 2)
			{
				time = 0;
			}
		}
		else if (mCharaStatus.SpecialAttack < 4)
		{
			if (time > 500)
			{
				mCharaStatus.SpecialAttack++;
				time = 0;
			}
			else if (mCharaStatus.SpecialAttack == 4)
			{
				time = 0;
			}
		}
	}
	else
	{
		// 待機アニメーションに切り替え
		ChangeAnimation(EAnimType::eIdle);
	}
}

// 攻撃
void CPlayer::UpdateAttack()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack);
	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;
}

// 攻撃
void CPlayer::UpdateAttack2()
{
	// 攻撃アニメーションを開始
	ChangeAnimation(EAnimType::eAttack2);
	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;
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

//攻撃力アップ
void CPlayer::UpdatePowerUp()
{
	ChangeAnimation(EAnimType::ePowerUp);
	//攻撃力アップ終了
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

//1レベルアップ
void CPlayer::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

//レベルを変更
void CPlayer::ChangeLevel(int level)
{
	//ステータスのテーブルのインデックス値に変換
	int index =Math::Clamp(level-1,0, PLAYER_LEVEL_MAX);
	//最大ステータスに設定
	mCharaMaxStatus = PLAYER_STATUS[index];
	//現在のステータスを最大値にすることで、HP回復
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 更新
void CPlayer::Update()
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
	    // 攻撃力アップ
		case EState::ePowerUp:
			UpdatePowerUp();
			break;
	    // 攻撃力アップ終了
		case EState::ePowerUpEnd:
			UpdatePowerUpEnd();
			break;
	}

	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// 移動
	Position(Position() + mMoveSpeed);

	// プレイヤーを移動方向へ向ける
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;

	//プレイヤーのデバック表示
	static bool debug = false;
	if (CInput::PushKey('R'))
	{
		debug = !debug;
	}
	if(debug)
	{
		CDebugPrint::Print("  レベル     %d\n", mCharaStatus.level);
		CDebugPrint::Print("   HP        %d /%d\n", mCharaStatus.hp,mCharaMaxStatus.hp);
		CDebugPrint::Print("  攻撃値     %d\n",mCharaStatus.power);
		CVector scale = Scale();
		CDebugPrint::Print("  スケール値 %f,%f,%f \n", scale.X(), scale.Y(), scale.Z());
		CDebugPrint::Print("  特殊攻撃(SA)   %d\n", mCharaStatus.SpecialAttack);
	}
	if (CInput::Key('1'))
	{
		if (CInput::PushKey(VK_UP)) mCharaStatus.hp++;
		else if (CInput::PushKey(VK_DOWN)) mCharaStatus.hp--;
	}
	else if (CInput::PushKey('2'))
	{
		LevelUp();
	}
	//CDebugPrint::Print(" ステータス確認  Rボタン");

	//HPゲージに現在のHPを設定
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			Position(Position() + hit.adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}

	if (self == mpColliderSphere)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			Position(Position() + hit.adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
}

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}
