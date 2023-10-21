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
#define MODEL_MUSHROOM "Character\\Mushroom\\Mushroom.x"
#define HP_FRAME_IMAGE "Character\\Player\\HP\\HPframe.png"  //HPフレーム画像
#define HP_IMAGE "Character\\Player\\HP\\HP.png"//HP画像

// プレイヤーのアニメーションデータのテーブル
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	//{ "",										true,	0.0f	},	// Tポーズ
	//{ "Character\\Player\\anim\\DogAttack.x",		true,	91.0f	},	// 待機
	//{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// 歩行
	{ "Character\\Player\\anim\\DogIdle.x",	true,	221.0f	},  //待機
	{ "Character\\Player\\anim\\DogWalk.x",	true,	69.0f	},  //歩行
	{ "Character\\Player\\anim\\DogAttack.x",	true,	91.0f	}  //攻撃
	//{ "Character\\Mushroom\\anim\\attack.x",	false,	26.0f	},	// 攻撃
	//{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// ジャンプ開始
	//{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// ジャンプ中
	//{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// ジャンプ終了
};

#define PLAYER_HEIGHT 2.0f

#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

#define HP 10  //HP
#define SA 5   //特殊攻撃
#define POWER 5  //攻撃力
#define MOVE_SPEED 0.375f  //移動速度

bool CPlayer::IsDeath() const
{
	return sHp <= 0;
}

int CPlayer::sHp = 0;

int CPlayer::Hp()
{
	return sHp;
}

// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mpRideObject(nullptr)
{

	sHp = HP;
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_DOG);

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
			sHp++;
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
			sHp--;
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
		ChangeAnimation(EAnimType::eJumpEnd);
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
	}

	if (sHp == 10)
	{

		mImage2 = new CImage(HP_FRAME_IMAGE);
		mImage2->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage2->SetPos(22.0, 24.0f);
		mImage2->SetSize(1017.0f, 27.0f);
		mImage2->Kill();

		mImage = new CImage(HP_IMAGE);
		mImage->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage->SetPos(30.0, 30.0f);
		mImage->SetSize(1000.0f, 15.0f);
		mImage->Kill();
	}

	if (sHp == 9)
	{
		mImage2 = new CImage(HP_FRAME_IMAGE);
		mImage2->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage2->SetPos(22.0, 24.0f);
		mImage2->SetSize(917.0f, 27.0f);
		mImage2->Kill();

		mImage = new CImage(HP_IMAGE);
		mImage->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		mImage->SetPos(30.0, 30.0f);
		mImage->SetSize(900.0f, 15.0f);
		mImage->Kill();
	}

	if (sHp == 8)
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
