#include "CRay.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CWave.h"
#include "Maths.h"

// �G�C�̃C���X�^���X
CRay* CRay::spInstance = nullptr;

#define ENEMY_HEIGHT    0.5f  // �����R���C�_�[
#define WITHIN_RANGE   40.0f  // �͈͓�
#define MOVE_SPEED      0.1f  // �ړ����x
#define GRAVITY        0.06f  // �d��
#define WALK_RANGE    100.0f  // �ǐՂ���͈�
#define STOP_RANGE     28.0f  // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f  // ��]����͈�
#define THROW_INTERVAL 0.07f  // �g���̔��ˊԊu����

// �G�C�̃A�j���[�V�����f�[�^�̃e�[�u��
const CRay::AnimData CRay::ANIM_DATA[] =
{
	{ "",										        true,	0.0f,	 0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Ray\\animation\\RayIdle.x",	true,	21.0f,	 0.5f},	 // �ҋ@ 21.0f
	{ "Character\\Enemy\\Ray\\animation\\RayAttack.x",	false,	17.0f,	 0.4f},  // �U�� 17.0f
	{ "Character\\Enemy\\Ray\\animation\\RayGetHit.x",	false,	13.0f,	0.25f},	 // �q�b�g 13.0f
	{ "Character\\Enemy\\Ray\\animation\\RayDie.x",	    false,	20.0f,	0.15f},	 // ���� 20.0f
	{ "Character\\Enemy\\Ray\\animation\\RayMoveBWD.x",	true,	21.0f,	 0.5f},	 // �ړ� 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveFWD.x",	true,	42.0f	 },	    // �ړ�2 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveLFT.x",	true,	42.0f	 },	    // ���ړ� 21.0f
	//{ "Character\\Enemy\\Ray\\animation\\RayMoveRGT.x",	true,	42.0f	 },	    // �E�ړ� 21.0f
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
	, mStateWave(0)
	, mIsSpawnedWaveEffect(false)
	, mElapsedWaveTime(0.0f)
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

	// �L�����N�^�[�̐����R���C�_�[
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.18f, false, 5.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eField });

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.2f, false, 5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eField });
	mpColliderSphereBody->Position(0.0f, 0.3f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�Ғ�)
	mpDamageColSpine = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, -0.1f, 0.0f),
		CVector(0.0f, 0.32f, 0.0f),
		6.0f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[��
	//�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColSpine->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColSpine->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�q���̍�)
	mpDamageColFinLeft = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.13f, false, 5.0f
	);
	mpDamageColFinLeft->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFinLeft->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFinLeft->Position(-0.05f, 0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�q���̉E)
	mpDamageColFinRight = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.13f, false, 5.0f
	);
	mpDamageColFinRight->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFinRight->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFinRight->Position(-0.05f, 0.1f, 0.0f);

	// �U���R���C�_�[(��)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.2f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });

	// �U���R���C�_�[���G�C�̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// �L�����̉����߂��R���C�_�[���G�C�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);

	// �_���[�W���󂯂�R���C�_�[���G�C�̐Ғł̍s��ɃA�^�b�`
	const CMatrix* spineMty = GetFrameMtx("Armature_Spine");
	mpDamageColSpine->SetAttachMtx(spineMty);

	// �L�����̉����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���G�C�̃q���̍��̍s��ɃA�^�b�`
	const CMatrix* finLeftMty = GetFrameMtx("Armature_WingLeft01");
	mpDamageColFinLeft->SetAttachMtx(finLeftMty);

	// �L�����̉����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���G�C�̃q���̉E�̍s��ɃA�^�b�`
	const CMatrix* finRightMty = GetFrameMtx("Armature_WingRight01");
	mpDamageColFinRight->SetAttachMtx(finRightMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColHead->SetEnable(false);
}

// �f�X�g���N�^
CRay::~CRay()
{
	// �L�����N�^�[�̐����R���C�_�[
	SAFE_DELETE(mpColliderLine);
	// �L�����N�^�[�̉����߂��R���C�_�[
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColSpine);
	SAFE_DELETE(mpDamageColFinLeft);
	SAFE_DELETE(mpDamageColFinRight);
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

// �g�����쐬
void CRay::CreateWave()
{
	// �g���G�t�F�N�g�𐶐����āA���ʕ����֔�΂�
	CWave* wave = new CWave
	(
		this,
		Position() + CVector(0.0f, 10.0f, 0.0f),
		VectorZ(),
		30.0f,
		80.0f
	);
	wave->Scale(1.0f, 1.0f, 1.0f);
	// �g���G�t�F�N�g�̐F�ݒ�
	wave->SetColor(CColor(0.0f, 0.1f, 1.0f));
	wave->SetOwner(this);
}

// ��Ԃ̐؂�ւ�
void CRay::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
	mStateWave = 0;
}

// �ҋ@���
void CRay::UpdateIdle()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= WITHIN_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
	else if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// �ҋ@���2
void CRay::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos > STOP_RANGE && vectorPos < WALK_RANGE && player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 33.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
}

// �U��
void CRay::UpdateAttack()
{
	SetAnimationSpeed(0.4f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateAttackStep)
	{
		// �X�e�b�v0 : �U���A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		mStateAttackStep++;
		break;
		// �X�e�b�v1 : �U���J�n
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			AttackStart();
			mStateAttackStep++;
		}
		break;
		// �X�e�b�v2 : �U���I��
	case 2:
		if (mAnimationFrame >= 13.0f)
		{
			AttackEnd();
			mStateAttackStep++;
		}
		break;
		// �X�e�b�v3 : �U���I���҂�
	case 3:
		if (mAnimationFrame >= 15.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}

	mIsSpawnedWaveEffect = false;
	if (!mIsSpawnedWaveEffect && vectorPos >= 30.0f && mAnimationFrame <= 5.0f)
	{
		mIsSpawnedWaveEffect = true;
		if (mIsSpawnedWaveEffect)
		{
			mElapsedWaveTime += Time::DeltaTime();
			// �o�ߎ��Ԃɉ����āA�g���̃G�t�F�N�g���쐬
			if (mElapsedWaveTime >= THROW_INTERVAL)
			{
				CreateWave();
				mElapsedWaveTime -= THROW_INTERVAL;
			}
		}
	}
}

// �U���I���҂�
void CRay::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
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
	SetAnimationSpeed(0.15f);
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
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// �͈͓��̎��A�ړ����ǐՂ���
	if (vectorPos > STOP_RANGE && vectorPos < WALK_RANGE)
	{
		mMoveSpeed += newPos * MOVE_SPEED;
		// ��]����͈͂ł����
		if (vectorPos <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	// �ǐՂ��~�܂������A�ҋ@���[�V������
	else if (vectorPos < STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// �X�V����
void CRay::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	if (mState != EState::eRun)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
	}

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
	float vectorPos = (player->Position() - Position()).Length();

	if (mState != EState::eIdle)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eHit)
	{
		mAttackTime++;

		if (vectorPos <= ROTATE_RANGE)
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
		if (mState == EState::eAttack || mState == EState::eAttackWait)
		{
			mAttackTime = 0;
		}
		if (vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
	}

	if (vectorPos > STOP_RANGE && vectorPos < WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
	{
		mFlyingTime++;
		if (mFlyingTime < 100 && mFlyingTime >= 10)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + 0.04f);
		}
		else if (mFlyingTime > 100)
		{
			Position(Position().X(), Position().Y() - 0.5f, Position().Z());
		}
	}

	if (Position().Y() <= -0.4f)
	{
		mFlyingTime = 0;
	}

	if (mState == EState::eHit || mState ==EState::eDie)
	{
		Position(Position().X(), Position().Y() - 0.5f, Position().Z());
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �L�����N�^�[�̉����߂��R���C�_-
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();

	// �_���[�W���󂯂�R���C�_�[
	mpDamageColSpine->Update();
	mpDamageColFinLeft->Update();
	mpDamageColFinRight->Update();

	// �U���R���C�_�[
	mpAttackColHead->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// �Փˏ���
void CRay::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColHead && mState != EState::eIdle && mState != EState::eIdle2)
	{
		// �L�����N�^�[�̃|�C���^�ɕϊ�
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// ����̃R���C�_�[�̎����傪�L�����ł���΁A
		if (chara != nullptr)
		{
			// ���ɍU���ς݂̃L�����łȂ����
			if (!IsAttackHitObj(chara))
			{
				int damage = CalcDamage(1.0f,this, chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, this);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
	else if (self == mpColliderLine || self == mpColliderSphereHead || self == mpColliderSphereBody)
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
	else if ( self == mpColliderSphereHead || self == mpColliderSphereBody)
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