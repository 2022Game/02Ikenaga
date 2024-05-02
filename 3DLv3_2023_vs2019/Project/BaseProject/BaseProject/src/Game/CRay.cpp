#include "CRay.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CWaveEffect.h"
#include "Maths.h"

// �G�C�̃C���X�^���X
CRay* CRay::spInstance = nullptr;

#define ENEMY_HEIGHT 0.5f
#define WITHIN_RANGE 40.0f       // �͈͓�
#define MOVE_SPEED 0.1f         // �ړ����x
#define GRAVITY 0.06f            // �d��
#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 28.0f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f     // ��]����͈�

// �G�C�̃A�j���[�V�����f�[�^�̃e�[�u��
const CRay::AnimData CRay::ANIM_DATA[] =
{
	{ "",										        true,	0.0f,	0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Ray\\animation\\RayIdle.x",	true,	21.0f,	0.5f},	// �ҋ@ 21.0f
	{ "Character\\Enemy\\Ray\\animation\\RayAttack.x",	true,	17.0f,	0.4f},  // �U�� 17.0f
	{ "Character\\Enemy\\Ray\\animation\\RayGetHit.x",	true,	13.0f,	0.25f},	// �q�b�g 13.0f
	{ "Character\\Enemy\\Ray\\animation\\RayDie.x",	    true,	20.0f,	0.2f},	// ���� 20.0f
	{ "Character\\Enemy\\Ray\\animation\\RayMoveBWD.x",	true,	21.0f,	0.5f},	// �ړ� 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveFWD.x",	true,	42.0f	},	    // �ړ�2 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveLFT.x",	true,	42.0f	},	    // ���ړ� 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveRGT.x",	true,	42.0f	},	    // �E�ړ� 21.0f
};

// �R���X�g���N�^
CRay::CRay()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
	, mIsGrounded(false)
	, mMoveSpeed(CVector::zero)
	, mStateAttackStep(0)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Ray");

	//�ŏ���1���x���ɐݒ�
	ChangeLevel(1);

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

	SetAnimationSpeed(0.5f);
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

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, -0.1f, 0.0f),
		CVector(0.0f, 0.32f, 0.0f),
		6.0f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[��
	//�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(��2)
	mpDamageColBody2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.4f, false, 5.0f
	);
	mpDamageColBody2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody2->Position(0.0f, 0.3f, 0.14f);

	// �U���R���C�_�[(��)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.19f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });

	// �U���R���C�_�[���G�C�̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpAttackColHead->SetAttachMtx(headMty);

	// �L�����̉����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���G�C�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Spine");
	mpDamageColBody->SetAttachMtx(bodyMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColHead->SetEnable(false);
	//mpColliderSphere->SetEnable(false);
	
	mpWave = new CWaveEffect
	(
		this, nullptr,
		CVector(0.0f, 12.0f, 20.0f),
		CQuaternion(0.0, 0.f, 0.0f).Matrix()
	);
	//mpWave->SetOwner(this);
}

CRay::~CRay()
{
	SAFE_DELETE(mpColliderLine);
	// �L�����̉����߂��R���C�_�[
	SAFE_DELETE(mpColliderSphere);
	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColBody2);
	// �U���R���C�_�[
	SAFE_DELETE(mpAttackColHead);
}

CRay* CRay::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CRay::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CRay::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
}

// �ҋ@���
void CRay::UpdateIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
}

// �ҋ@���2
void CRay::UpdateIdle2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		ChangeState(EState::eRun);
	}
	else if (vectorp < 26.0f)
	{
		//ChangeState(EState::eIdle2);
	}
}

// �U��
void CRay::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.4f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (!mpWave->IsThrowing() && vectorp >= 30.0f)
	{
		mpWave->Start();
	}

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateAttackStep)
	{
		// �X�e�b�v0 : �U���A�j���[�V�����J�n�{�U���R���C�_�[�J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		if (mAnimationFrame >= 10.0f && mAnimationFrame <= 11.0f)
		{
			AttackStart();
			mStateAttackStep++;
		}
		break;
	case 1:  // �X�e�b�v1 : �U���R���C�_�[�I��
		if (mAnimationFrame >= 12.0f && mAnimationFrame < 15.0f)
		{
			AttackEnd();
			mStateAttackStep++;
		}
		break;
	case 2:  // �X�e�b�v2 : �U���I���҂�
		if (mAnimationFrame >= 15.0f)
		{
			// �U���I���҂���Ԃֈڍs
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U���I���҂�
void CRay::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		mpWave->Stop();
		AttackEnd();
		ChangeState(EState::eIdle2);
	}
}

// �q�b�g
void CRay::UpdateHit()
{
	SetAnimationSpeed(0.25f);
	// �q�b�g�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle2);
	}
}

// ����
void CRay::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.2f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::RayDeath();
	}
}

// �ړ�
void CRay::UpdateRun()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// �͈͓��̎��A�ړ����ǐՂ���
	if (vectorp > 28.0f && vectorp < WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
		// ��]����͈͂ł����
		if (vectorp <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	// �ǐՂ��~�܂������A�ҋ@���[�V������
	else if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// �X�V����
void CRay::Update()
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
		// �ҋ@���2
	case EState::eIdle2:
		UpdateIdle2();
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

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle2 && mState != EState::eAttack
		&& mState != EState::eAttackWait && mState != EState::eHit && mState != EState::eDie && mState !=EState::eRun)
	{
		ChangeState(EState::eIdle2);
	}

	if (mState == EState::eIdle2 || mState == EState::eAttack || mState == EState::eAttackWait
		|| mState == EState::eHit || mState == EState::eRun)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 && vectorp <= 70.0f || mState == EState::eHit || mState == EState::eRun
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
			ChangeState(EState::eAttack);
		}
		if (mState == EState::eAttack)
		{
			mAttackTime = 0;
		}
	}

	if (vectorp > STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
	{
		mFlyingTime++;
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
	{
		if (mFlyingTime < 100 && mFlyingTime >= 10)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + 0.04f);
		}

		else if (mFlyingTime > 100)
		{
			Position(Position().X(), Position().Y() - 0.5f, Position().Z());
		}
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

	// �_���[�W���󂯂�R���C�_�[
	mpDamageColBody->Update();

	// �U���R���C�_�[
	mpAttackColHead->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
	CDebugPrint::Print("��s %d", mFlyingTime);
	CDebugPrint::Print("���� %f", vectorp);
	float y = Position().Y();
	CDebugPrint::Print("���� %f", y);
}

// �Փˏ���
void CRay::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColHead && mState != EState::eIdle)
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
void CRay::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	if (mState == EState::eAttack)
	{
		mpAttackColHead->SetEnable(true);
	}
}

// �U���I��
void CRay::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackColHead->SetEnable(false);
}

// �`��
void CRay::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CRay::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CRay::ChangeLevel(int level)
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
void CRay::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HP����_���[�W������
	if (mCharaStatus.hp -= damage)
	{
		ChangeState(EState::eHit);
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
void CRay::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}