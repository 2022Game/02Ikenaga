#include "CRay3.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "CWaveEffect.h"
#include "Maths.h"

// �G�C3�̃C���X�^���X
CRay3* CRay3::spInstance = nullptr;

#define ENEMY_HEIGHT 0.5f
#define WITHIN_RANGE 40.0f       // �͈͓�
#define MOVE_SPEED 0.1f         // �ړ����x
#define GRAVITY 0.06f            // �d��
#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 24.5f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f     // ��]����͈�

// �G�C3�̃A�j���[�V�����f�[�^�̃e�[�u��
const CRay3::AnimData CRay3::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	{ "Character\\Enemy\\Ray\\animation\\RayIdle.x",	true,	42.0f	},	    // �ҋ@ 21.0f
	{ "Character\\Enemy\\Ray\\animation\\RayAttack.x",	true,	60.0f	},	    // �U�� 17.0f
	{ "Character\\Enemy\\Ray\\animation\\RayGetHit.x",	true,	60.0f	},	    // �q�b�g 13.0f
	{ "Character\\Enemy\\Ray\\animation\\RayDie.x",	true,	90.0f	},	        // ���� 20.0f
	{ "Character\\Enemy\\Ray\\animation\\RayMoveBWD.x",	true,	42.0f	},	    // �ړ� 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveFWD.x",	true,	42.0f	},	    // �ړ�2 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveLFT.x",	true,	42.0f	},	    // ���ړ� 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveRGT.x",	true,	42.0f	},	    // �E�ړ� 21.0f
};

// �R���X�g���N�^
CRay3::CRay3()
	: mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Ray");

	//�ŏ���5���x���ɐݒ�
	ChangeLevel(5);

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
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[�����߂�����
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.4f, false, 5.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere->Position(0.0f, 0.3f, 0.15f);

	// �_���[�W���󂯂�R���C�_�[���쐬
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.3f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[��
	//�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	// �_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageCol->Position(0.0f, 0.35f, 0.2f);

	// �_���[�W��^����R���C�_�[
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.19f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });

	// �U���R���C�_�[���G�C�̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpAttackCol->SetAttachMtx(headMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackCol->SetEnable(false);

	mpWave = new CWaveEffect
	(
		this, nullptr,
		CVector(0.0f, 12.0f, 20.0f),
		CQuaternion(0.0, 0.f, 0.0f).Matrix()
	);
}

CRay3::~CRay3()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);
}

CRay3* CRay3::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CRay3::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �ҋ@���
void CRay3::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mState = EState::eRun;
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle);
		if (IsAnimationFinished())
		{
			mState = EState::eIdle;
		}
	}
}

// �U��
void CRay3::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	if (mAnimationFrame >= 20.0f && mAnimationFrame <= 21.0f)
	{
		AttackStart();
	}
	if (mAnimationFrame >= 30.0f && mAnimationFrame <= 31.0f)
	{
		AttackEnd();
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (!mpWave->IsThrowing() && vectorp >= 30.0f)
	{
		mpWave->Start();
	}
	if (IsAnimationFinished())
	{
		// �U���I���҂���Ԃֈڍs
		mState = EState::eAttackWait;
	}
}

// �U���I���҂�
void CRay3::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		mpWave->Stop();
		AttackEnd();
		mState = EState::eIdle;
	}
}

// �q�b�g
void CRay3::UpdateHit()
{
	// �q�b�g�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// ����
void CRay3::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::Death();
	}
}

// �ړ�
void CRay3::UpdateRun()
{
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// �ǐՂ���߂Ď~�܂�
	if (vectorp <= 22.0f && vectorp >= 24.0f)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);

		// ��]����͈͂ł����
		if (vectorp <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));

			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
		}
	}
	// �͈͓��̎��A�ړ����ǐՂ���
	else if (vectorp >= 24.0f && vectorp <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
	}
	// �ǐՂ��~�܂������A�ҋ@���[�V������
	if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// �X�V����
void CRay3::Update()
{
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
		// �q�b�g
	case EState::eHit:
		UpdateHit();
		break;
		// ����
	case EState::eDie:
		UpdateDie();
		break;
		// �ړ�
	case EState::eRun:
		UpdateRun();
		break;
	}

	// HP�Q�[�W�̍��W���X�V(�G�̍��W�̏�����̍��W)
	CVector gaugePos = Position() + CVector(0.0f, 27.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eAttack && mState != EState::eAttackWait
		&& mState != EState::eHit && mState != EState::eDie && mState != EState::eRun)
	{
		mState = EState::eIdle;
	}

	if (mState == EState::eAttack || mState == EState::eAttackWait || mState == EState::eDie
		|| mState == EState::eHit || mState == EState::eIdle || mState == EState::eRun)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle && vectorp <= 60.0f || mState == EState::eHit || mState == EState::eRun
		|| mState == EState::eAttack || mState == EState::eAttackWait)
	{
		mAttackTime++;

		if (vectorp <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}

		if (mAttackTime > 200)
		{
			mState = EState::eAttack;
		}
		if (mState == EState::eAttack)
		{
			mAttackTime = 0;
		}
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (mState == EState::eIdle || mState == EState::eRun)
	{
		if (mFlyingTime <= 200 || mFlyingTime <= 0)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + 0.02f);
		}

		if (mFlyingTime >= 200)
		{
			Position(Position().X(), Position().Y() - 0.5f, Position().Z());
		}
	}

	if (Position().Y() >= 0.1f || vectorp >= 24.0f && vectorp <= WALK_RANGE)
	{
		mFlyingTime++;
	}

	if (Position().Y() <= 0.0f)
	{
		mFlyingTime = 0;
	}
	if (mState == EState::eHit)
	{
		Position(Position().X(), Position().Y() - 0.5f, Position().Z());
	}
	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// �Փˏ���
void CRay3::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackCol && mState != EState::eIdle)
	{
		// �L�����̃|�C���^�ɕϊ�
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// ����̃R���C�_�[�̎����傪�L�����ł���΁A
		if (chara != nullptr)
		{
			// ���ɍU���ς݂̃L�����łȂ����
			if (!IsAttackHitObj(chara))
			{
				int damage = CalcDamage(this, chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, this);

				// �U���ς݃��X�g�ɒǉ�
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
	// �L�����N�^�[���m�̏Փˏ���
	else if (self == mpColliderSphere)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CRay3::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
}

// �U���I��
void CRay3::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
}

// �`��
void CRay3::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CRay3::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CRay3::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY4_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CRay3::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HP����_���[�W������
	if (mCharaStatus.hp -= damage)
	{
		mState = EState::eHit;
	}
	// HP��0�ȉ��ɂȂ�����A
	if (mCharaStatus.hp <= 0)
	{
		// ���S����
		Death();
	}

	if (causedObj != nullptr)
	{
		// �_���[�W��^��������̕����֌���
		CVector dir = causedObj->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));

		// �m�b�N�o�b�N�Ń_���[�W��^��������̕���������ɃY����
		Position(Position() - dir * Scale().X() * 0.2f);
	}
}

// ���S����
void CRay3::Death()
{
	// ���S��Ԃֈڍs
	mState = EState::eDie;
}