//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;

// �v���C���[�̃��f���f�[�^�̃p�X
#define MODEL_PATH "Character\\Player\\player.x"
#define MODEL_DOG "Character\\Player\\Dog.x"
#define MODEL_SLIME "Character\\Slime\\Slime.x"
#define MODEL_TURTLE "Character\\Turtle\\Turtle.x"
#define MODEL_MUSHROOM "Character\\Mushroom\\Mushroom.x"
#define HP_FRAME_IMAGE "Character\\Player\\HP\\HPframe.png"  //HP�t���[���摜
#define HP_IMAGE "Character\\Player\\HP\\HP.png"//HP�摜

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	//{ "",										true,	0.0f	},	// T�|�[�Y
	//{ "Character\\Player\\anim\\DogAttack.x",		true,	91.0f	},	// �ҋ@
	//{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// ���s

	{ "Character\\Player\\animation\\DogIdle.x",	true,	221.0f	},  //�ҋ@ 221.0f
	{ "Character\\Player\\animation\\DogWalk.x",	true,	69.0f	},  //���s
	{ "Character\\Player\\animation\\DogAttack.x",	true,	91.0f	},  //�U��
	{ "Character\\Player\\animation\\DogJump.x",	true,	49.0f	},  //�W�����v

	//{ "Character\\Mushroom\\anim\\attack.x",	false,	26.0f	},	// �U��
	//{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// �W�����v�J�n
	//{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// �W�����v��
	//{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// �W�����v�I��
};

#define PLAYER_HEIGHT 2.0f

#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

#define HP 10  //HP
#define LEVEL 1  //���x��
#define SA 5   //����U��
#define POWER 5  //�U����
#define MOVE_SPEED 0.3f  //�ړ����x
#define MOVE_SPEED2 0.31f  //�ړ����x

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

// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mpRideObject(nullptr)
{

	sHp = HP;
	sLevel = LEVEL;
	sPower = POWER;

	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = new CModelX();
	model->Load(MODEL_DOG);

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
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

// �A�j���[�V�����؂�ւ�
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �ҋ@
void CPlayer::UpdateIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	
	if (mIsGrounded)
	{
		// �ړ�����
		// �L�[�̓��̓x�N�g�����擾
		CVector input;
		if (CInput::Key('W'))		input.Z(1.0f);
		else if (CInput::Key('S'))	input.Z(-1.0f);
		if (CInput::Key('A'))		input.X(-1.0f);
		else if (CInput::Key('D'))	input.X(1.0f);

		// ���̓x�N�g���̒����œ��͂���Ă��邩����
		if (input.LengthSqr() > 0.0f)
		{
			// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
			CVector move = CCamera::MainCamera()->Rotation() * input;
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * MOVE_SPEED;
			if (sLevel == 2)
			{
				mMoveSpeed += move * MOVE_SPEED2;
			}

			// ���s�A�j���[�V�����ɐ؂�ւ�
			ChangeAnimation(EAnimType::eWalk);
		}
		// �ړ��L�[����͂��Ă��Ȃ�
		else
		{
			// �ҋ@�A�j���[�V�����ɐ؂�ւ�
			ChangeAnimation(EAnimType::eIdle);
		}

		// ���N���b�N�ōU����Ԃֈڍs
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::eAttack;
			//sHp++;
			sLevel++;
		}
		// SPACE�L�[�ŃW�����v�J�n�ֈڍs
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
			sHp--;
		}
	}
	else
	{
		// �ҋ@�A�j���[�V�����ɐ؂�ւ�
		ChangeAnimation(EAnimType::eIdle);
	}
}

// �U��
void CPlayer::UpdateAttack()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack);
	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U���I���҂�
void CPlayer::UpdateAttackWait()
{
	// �U���A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �ҋ@��Ԃֈڍs
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// �W�����v�J�n
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	mState = EState::eJump;

	mMoveSpeed += CVector(0.0f, JUMP_SPEED, 0.0f);
	mIsGrounded = false;
}

// �W�����v��
void CPlayer::UpdateJump()
{
	if (mMoveSpeed.Y() <= 0.0f)
	{
		//ChangeAnimation(EAnimType::eJumpEnd);
		mState = EState::eJumpEnd;
	}
}

// �W�����v�I��
void CPlayer::UpdateJumpEnd()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

void CPlayer::UpdateLevel()
{
	switch (sLevel)
	{
	case 2:
		sHp = 20;
		sPower = 10;
		break;
	case 3:
		sHp = 30;
		sPower = 15;
		break;
	case 4:
		sPower = 20;
		break;
	case 5:
		sPower = 25;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		break;
	}
}

// �X�V
void CPlayer::Update()
{
	printf("%d", sHp);
	//printf("%d", sPower);
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
		case EState::eIdle:
			UpdateIdle();
			break;
		// �U��
		case EState::eAttack:
			UpdateAttack();
			break;
		// �U���I���҂�
		case EState::eAttackWait:
			UpdateAttackWait();
			break;
		// �W�����v�J�n
		case EState::eJumpStart:
			UpdateJumpStart();
			break;
		// �W�����v��
		case EState::eJump:
			UpdateJump();
			break;
		// �W�����v�I��
		case EState::eJumpEnd:
			UpdateJumpEnd();
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

	// �ړ�
	Position(Position() + mMoveSpeed);

	// �v���C���[���ړ������֌�����
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;

	if (CInput::Key('R'))
	{
		CVector scale = Scale();
		CDebugPrint::Print(" �X�P�[���l(%f,%f,%f)\n", scale.X(), scale.Y(), scale.Z());
	}
}

// �Փˏ���
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

// �`��
void CPlayer::Render()
{
	CXCharacter::Render();
}
