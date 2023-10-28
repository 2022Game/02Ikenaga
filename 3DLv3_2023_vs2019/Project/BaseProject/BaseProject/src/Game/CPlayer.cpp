//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"

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
	{ "Character\\Player\\animation\\DogPowerUp.x",	true,	143.0f	},  //攻撃力アップ
	{ "Character\\Player\\animation\\DogAttack2.x",	true,	140.0f	},  //攻撃2

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
	return sHp <= 0;
}

bool CPlayer::IsDeath2() const
{
	return sLevel <= 0;
}

bool CPlayer::IsDeath3() const
{
	return sPower <= 0;
}

int CPlayer::sHp = 0;

int CPlayer::Hp()
{
	return sHp;
}

int CPlayer::sLevel = 0;

int CPlayer::Level()
{
	return sLevel;
}

int CPlayer::sPower = 0;

int CPlayer::Power()
{
	return sPower;
}

// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mpRideObject(nullptr)
{

	sHp = HP;
	sLevel = LEVEL;
	sPower = POWER;

	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_DOG);
	Scale(10.0f, 10.0f, 10.0f);

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
		mpColliderSphere->Owner(),
		ELayer::eField,
		mpColliderSphere->Type()
	);*/
	//mpColliderSphere->SetCollisionLayers({ ELayer::eField });
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

	if (mImage != nullptr)
	{
		delete mImage;
		mImage = nullptr;
	}

	if (mImage2 != nullptr)
	{
		delete mImage2;
		mImage2 = nullptr;
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

			mMoveSpeed += move * MOVE_SPEED;

			int speed = 1.2f;
			int speed2 = 1.4f;
			int speed3 = 1.6f;
			int speed4 = 1.8f;
			int speed5 = 2.0f;
			int speed6 = 2.2f;
			int speed7 = 2.4f;
			int speed8 = 2.6f;
			int speed9 = 2.8f;
			int speed10 = 3.0f;
			switch (sLevel)
			{
			case 10:	
				mMoveSpeed += move * MOVE_SPEED* speed;
			 break;
			case 20:
				mMoveSpeed += move * MOVE_SPEED * speed2;
				break;
			case 30:
				mMoveSpeed += move * MOVE_SPEED * speed3;
				break;
			case 40:
				mMoveSpeed += move * MOVE_SPEED * speed4;
				break;
			case 50:
				mMoveSpeed += move * MOVE_SPEED * speed5;
				break;
			case 60:
				mMoveSpeed += move * MOVE_SPEED * speed6;
				break;
			case 70:
				mMoveSpeed += move * MOVE_SPEED * speed7;
				break;
			case 80:
				mMoveSpeed += move * MOVE_SPEED * speed8;
				break;
			case 90:
				mMoveSpeed += move * MOVE_SPEED * speed9;
				break;
			case 100:
				mMoveSpeed += move * MOVE_SPEED * speed10;
				break;
			}

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
			//sHp++;
			sLevel++;
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
			sHp--;
		}
		if (CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::ePowerUp;
		}
		if (CInput::PushKey(WM_LBUTTONDBLCLK))
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::eAttack2;
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

//レベル(HP・攻撃力・大きさ)の更新
void CPlayer::UpdateLevel()
{
	switch (sLevel)
	{
	case 2:
		sHp = 20;
		sPower = 10;
		Scale(20.0f, 20.0f, 20.0f);
		break;
	case 3:
		sPower = 15;
		break;
	case 4:
		sPower = 20;
		break;
	case 5:
		sPower = 25;
		break;
	case 6:
		sPower = 30;
		break;
	case 7:
		sPower = 35;
		break;
	case 8:
		sPower = 40;
		break;
	case 9:
		sPower = 45;
		break;
	case 10:
		sPower = 50;
		break;
	case 11:
		sPower = 55;
		break;
	case 12:
		sPower = 60;
		break;
	case 13:
		sPower = 65;
		break;
	case 14:
		sPower = 70;
		break;
	case 15:
		sPower = 75;
		break;
	case 16:
		sPower = 80;
		break;
	case 17:
		sPower = 85;
		break;
	case 18:
		sPower = 90;
		break;
	case 19:
		sPower = 95;
		break;
	case 20:
		sPower = 100;
		break;
	case 21:
		sPower = 105;
		break;
	case 22:
		sPower = 110;
		break;
	case 23:
		sPower = 115;
		break;
	case 24:
		sPower = 120;
		break;
	case 25:
		sPower = 125;
		break;
	case 26:
		sPower = 130;
		break;
	case 27:
		sPower = 135;
		break;
	case 28:
		sPower = 140;
		break;
	case 29:
		sPower = 145;
		break;
	case 30:
		sPower = 150;
		break;
	case 31:
		sPower = 155;
		break;
	case 32:
		sPower = 160;
		break;
	case 33:
		sPower = 165;
		break;
	case 34:
		sPower = 170;
		break;
	case 35:
		sPower = 175;
		break;
	case 36:
		sPower = 180;
		break;
	case 37:
		sPower = 185;
		break;
	case 38:
		sPower = 190;
		break;
	case 39:
		sPower = 195;
		break;
	case 40:
		sPower = 200;
		break;
	case 41:
		sPower = 205;
		break;
	case 42:
		sPower = 210;
		break;
	case 43:
		sPower = 215;
		break;
	case 44:
		sPower = 220;
		break;
	case 45:
		sPower = 225;
		break;
	case 46:
		sPower = 230;
		break;
	case 47:
		sPower = 235;
		break;
	case 48:
		sPower = 240;
		break;
	case 49:
		sPower = 245;
		break;
	case 50:
		sPower = 250;
		break;
	case 51:
		sPower = 255;
		break;
	case 52:
		sPower = 260;
		break;
	case 53:
		sPower = 265;
		break;
	case 54:
		sPower = 270;
		break;
	case 55:
		sPower = 275;
		break;
	case 56:
		sPower = 280;
		break;
	case 57:
		sPower = 285;
		break;
	case 58:
		sPower = 290;
		break;
	case 59:
		sPower = 295;
		break;
	case 60:
		sPower = 300;
		break;
	case 61:
		sPower = 305;
		break;
	case 62:
		sPower = 310;
		break;
	case 63:
		sPower = 315;
		break;
	case 64:
		sPower = 320;
		break;
	case 65:
		sPower = 325;
		break;
	case 66:
		sPower = 330;
		break;
	case 67:
		sPower = 335;
		break;
	case 68:
		sPower = 340;
		break;
	case 69:
		sPower = 345;
		break;
	case 70:
		sPower = 350;
		break;
	case 71:
		sPower = 355;
		break;
	case 72:
		sPower = 360;
		break;
	case 73:
		sPower = 365;
		break;
	case 74:
		sPower = 370;
		break;
	case 75:
		sPower = 375;
		break;
	case 76:
		sPower = 380;
		break;
	case 77:
		sPower = 385;
		break;
	case 78:
		sPower = 390;
		break;
	case 79:
		sPower = 395;
		break;
	case 80:
		sPower = 400;
		break;
	case 81:
		sPower = 405;
		break;
	case 82:
		sPower = 410;
		break;
	case 83:
		sPower = 415;
		break;
	case 84:
		sPower = 420;
		break;
	case 85:
		sPower = 425;
		break;
	case 86:
		sPower = 430;
		break;
	case 87:
		sPower = 435;
		break;
	case 88:
		sPower = 440;
		break;
	case 89:
		sPower = 445;
		break;
	case 90:
		sPower = 450;
		break;
	case 91:
		sPower = 455;
		break;
	case 92:
		sPower = 460;
		break;
	case 93:
		sPower = 465;
		break;
	case 94:
		sPower = 470;
		break;
	case 95:
		sPower = 475;
		break;
	case 96:
		sPower = 480;
		break;
	case 97:
		sPower = 485;
		break;
	case 98:
		sPower = 490;
		break;
	case 99:
		sPower = 495;
		break;
	case 100:
		sPower = 500;
		break;
	}
}

// 更新
void CPlayer::Update()
{
	printf("%d", sLevel);
	//printf("%d", sPower);
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
	
	UpdateLevel();

	if (sHp == 10)
	{

		mImage2 = new CImage(HP_FRAME_IMAGE);
		mImage2->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage2->SetPos(10.0, 670.0f);
		mImage2->SetSize(1017.0f, 27.0f);
		mImage2->Kill();

		mImage = new CImage(HP_IMAGE);
		mImage->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage->SetPos(18.0, 676.0f);
		mImage->SetSize(10.0f, 15.0f);
		mImage->Kill();
	}

	if (sHp == 19)
	{
		mImage2 = new CImage(HP_FRAME_IMAGE);
		mImage2->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage2->SetPos(22.0, 24.0f);
		mImage2->SetSize(917.0f, 27.0f);
		mImage2->Kill();

		mImage = new CImage(HP_IMAGE);
		mImage->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage->SetPos(30.0, 30.0f);
		mImage->SetSize(19.0f, 15.0f);
		mImage->Kill();
	}

	if (sHp == 20)
	{
		mImage2 = new CImage(HP_FRAME_IMAGE);
		mImage2->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage2->SetPos(10.0, 670.0f);
		mImage2->SetSize(917.0f, 27.0f);
		mImage2->Kill();

		mImage = new CImage(HP_IMAGE);
		mImage->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage->SetPos(18.0, 676.0f);
		mImage->SetSize(20.0f, 15.0f);
		mImage->Kill();
	}

	/*if (sHp == 8)
	{
		mImage2 = new CImage(HP_FRAME_IMAGE);
		mImage2->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage2->SetPos(22.0, 24.0f);
		mImage2->SetSize(817.0f, 27.0f);
		mImage2->Kill();

		mImage = new CImage(HP_IMAGE);
		mImage->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage->SetPos(30.0, 30.0f);
		mImage->SetSize(800.0f, 15.0f);
		mImage->Kill();
	}

	if (sHp == 7)
	{
		mImage2 = new CImage(HP_FRAME_IMAGE);
		mImage2->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage2->SetPos(22.0, 24.0f);
		mImage2->SetSize(717.0f, 27.0f);
		mImage2->Kill();

		mImage = new CImage(HP_IMAGE);
		mImage->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage->SetPos(30.0, 30.0f);
		mImage->SetSize(700.0f, 15.0f);
		mImage->Kill();
	}

	if (sHp == 6)
	{
		mImage2 = new CImage(HP_FRAME_IMAGE);
		mImage2->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage2->SetPos(22.0, 24.0f);
		mImage2->SetSize(617.0f, 27.0f);
		mImage2->Kill();

		mImage = new CImage(HP_IMAGE);
		mImage->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage->SetPos(30.0, 30.0f);
		mImage->SetSize(600.0f, 15.0f);
		mImage->Kill();
	}*/

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

	if (CInput::Key('R'))
	{
		CVector scale = Scale();
		CDebugPrint::Print(" スケール値(%f,%f,%f)\n", scale.X(), scale.Y(), scale.Z());
	}
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
