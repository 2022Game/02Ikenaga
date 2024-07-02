#include "CBee.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"
#include "CNeedle.h"

// �I�̃C���X�^���X
CBee* CBee::spInstance = nullptr;

#define ENEMY_HEIGHT   0.3f  // �����R���C�_�[
#define WITHIN_RANGE  40.0f  // �͈͓�
#define WALK_RANGE   100.0f  // �ǐՂ���͈�
#define STOP_RANGE    22.0f  // �ǐՂ����߂�͈�
#define STOP_RANGE_Y  20.0f  // �ǐՂ����߂鍂��
#define ROTATE_RANGE 250.0f  // ��]����͈�
#define MOVE_SPEED     0.6f  // �ړ����x
#define MOVE_SPEED_Y 0.027f  // Y�̃X�s�[�h
#define HEIGHT         0.5f  // ����
#define PLAYER_HEIGHT 0.25f  // �v���C���[�̍���

// �I�̃A�j���[�V�����f�[�^�̃e�[�u��
const CBee::AnimData CBee::ANIM_DATA[] =
{
	{ "",										        true,	 0.0f,	 0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Bee\\animation\\BeeIdle.x",	true,	20.0f,	 0.5f},	 // �ҋ@
	{ "Character\\Enemy\\Bee\\animation\\BeeAttack.x",	false,	17.0f,	 0.3f},	 // �U��
	{ "Character\\Enemy\\Bee\\animation\\BeeGetHit.x",	false,	13.0f,	0.25f},	 // �q�b�g
	{ "Character\\Enemy\\Bee\\animation\\BeeDie.x",	    false,	20.0f,	0.15f},	 // ����
	{ "Character\\Enemy\\Bee\\animation\\BeeMoveFWD.x",	true,	21.0f,	 0.5f},	 // �ړ�
};

// �R���X�g���N�^
CBee::CBee()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
	, mMoveSpeed(CVector::zero)
	, mIsSpawnedNeedleEffect(false)
	, mIsGrounded(false)
	, mStateStep(0)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// �G�̎��
	mType = EEnemyType::eBee;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Bee");

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
	mpColliderLine->Position(0.0f, 5.0f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 2.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer, ELayer::eEnemy, ELayer::eField});
	mpColliderSphereHead->Position(-0.1f, 0.0f, 0.03f);

	// �L�����N�^�[�����߂�����(���΂�)
	mpColliderSphereBeak = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.04f, false, 2.0f
	);
	mpColliderSphereBeak->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBeak->Position(-0.23f, 0.01f, 0.1f);

	// �L�����N�^�[�����߂�����(���΂�2)
	mpColliderSphereBeak2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.02f, false, 2.0f
	);
	mpColliderSphereBeak2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBeak2->Position(-0.35f, 0.01f, 0.2f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,0.25f, false, 2.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eField });
	mpColliderSphereBody->Position(0.0f, 0.18f, 0.0f);

	// �L�����N�^�[�����߂�����(�K��)
	mpColliderSphereTail = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 2.0f
	);
	mpColliderSphereTail->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�K��2)
	mpColliderSphereTail2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.1f, false, 2.0f
	);
	mpColliderSphereTail2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail2->Position(0.02f, 0.14f, 0.0f);

	// �L�����N�^�[�����߂�����(�K��3)
	mpColliderSphereTail3 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.05f, false, 2.0f
	);
	mpColliderSphereTail3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail3->Position(-0.05f, 0.0f, 0.04f);

	// �L�����N�^�[�����߂�����(�K��4)
	mpColliderSphereTail4 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.03f, false, 2.0f
	);
	mpColliderSphereTail4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail4->Position(-0.15f, 0.0f, 0.1f);

	// �L�����N�^�[�����߂�����(�K��5)
	mpColliderSphereTail5 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.02f, false, 2.0f
	);
	mpColliderSphereTail5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereTail5->Position(-0.25f, 0.0f, 0.105f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.08f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHead->Position(-0.1f, 0.0f, 0.03f);

	// �_���[�W���󂯂�R���C�_�[���쐬(���΂�)
	mpDamageColBeak = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.04f, false
	);
	mpDamageColBeak->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBeak->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBeak->Position(-0.23f, 0.01f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(���΂�2)
	mpDamageColBeak2 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.02f, false
	);
	mpDamageColBeak2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBeak2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBeak2->Position(-0.35f, 0.01f, 0.2f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.25f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, 0.18f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�K��)
	mpDamageColTail = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.08f, false
	);
	mpDamageColTail->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�K��2)
	mpDamageColTail2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.1f, false
	);
	mpDamageColTail2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail2->Position(0.02f, 0.14f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�K��3)
	mpDamageColTail3 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.05f, false
	);
	mpDamageColTail3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail3->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail3->Position(-0.05f, 0.0f, 0.04f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�K��4)
	mpDamageColTail4 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.03f, false
	);
	mpDamageColTail4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail4->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail4->Position(-0.15f, 0.0f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�K��5)
	mpDamageColTail5 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.02f, false
	);
	mpDamageColTail5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail5->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail5->Position(-0.25f, 0.0f, 0.105f);

	// �_���[�W��^����R���C�_�[
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.01f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(-0.28f, 0.0f, 0.1f);

	// �L�����N�^�[�����߂��ƃ_���[�W���󂯂�R���C�_�[�𓪂̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpColliderSphereBeak->SetAttachMtx(headMty);
	mpColliderSphereBeak2->SetAttachMtx(headMty);
	mpDamageColHead->SetAttachMtx(headMty);
	mpDamageColBeak->SetAttachMtx(headMty);
	mpDamageColBeak2->SetAttachMtx(headMty);

	// �L�����N�^�[�����߂���
	// �_���[�W���󂯂�R���C�_�[��̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);

	// �L�����N�^�[�����߂���
	// �_���[�W���󂯂�R���C�_�[��K���̍s��ɃA�^�b�`
	const CMatrix* tailMty = GetFrameMtx("Armature_Tail01");
	mpColliderSphereTail->SetAttachMtx(tailMty);
	mpColliderSphereTail2->SetAttachMtx(tailMty);
	mpDamageColTail->SetAttachMtx(tailMty);
	mpDamageColTail2->SetAttachMtx(tailMty);

	// �L�����N�^�[�����߂��ƃ_���[�W���󂯂�R���C�_�[��
	// �U���R���C�_�[��I�̐j�̍s��ɃA�^�b�`
	const CMatrix* needleMty = GetFrameMtx("Armature_Tail02");
	mpColliderSphereTail3->SetAttachMtx(needleMty);
	mpColliderSphereTail4->SetAttachMtx(needleMty);
	mpColliderSphereTail5->SetAttachMtx(needleMty);
	mpDamageColTail3->SetAttachMtx(needleMty);
	mpDamageColTail4->SetAttachMtx(needleMty);
	mpDamageColTail5->SetAttachMtx(needleMty);
	mpAttackCol->SetAttachMtx(needleMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackCol->SetEnable(false);
	mpColliderSphereBeak2->SetEnable(false);
}

// �f�X�g���N�^
CBee::~CBee()
{
	// �L�����N�^�[�̐����R���C�_�[
	SAFE_DELETE(mpColliderLine);
	// �L�����N�^�[�����߂��R���C�_�[
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBeak);
	SAFE_DELETE(mpColliderSphereBeak2);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereTail);
	SAFE_DELETE(mpColliderSphereTail2);
	SAFE_DELETE(mpColliderSphereTail3);
	SAFE_DELETE(mpColliderSphereTail4);
	SAFE_DELETE(mpColliderSphereTail5);
	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBeak);
	SAFE_DELETE(mpDamageColBeak2);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColTail);
	SAFE_DELETE(mpDamageColTail2);
	SAFE_DELETE(mpDamageColTail3);
	SAFE_DELETE(mpDamageColTail4);
	SAFE_DELETE(mpDamageColTail5);
	// �U���R���C�_�[
	SAFE_DELETE(mpAttackCol);
}

// �C���X�^���X
CBee* CBee::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CBee::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CBee::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// �ҋ@���
void CBee::UpdateIdle()
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
void CBee::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);

	mFlyingTime++;
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >= 10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE && player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 33.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
	else if (vectorPos > WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
		if (vectorPos <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
}

// �U��
void CBee::UpdateAttack()
{
	SetAnimationSpeed(0.3f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0 : �U���A�j���[�V�����J�n�{�U���J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		if (mAnimationFrame >= 5.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// �X�e�b�v1 : 
	case 1:
		if (vectorPos >= 30.0f)
		{
			if (mAnimationFrame >= 7.5f)
			{
				// �j�𐶐��ς݃t���O��������
				mIsSpawnedNeedleEffect = false;
				// �j�𐶐����Ă��Ȃ�
				if (!mIsSpawnedNeedleEffect)
				{
					CNeedle* needle = new CNeedle
					(
						this,
						Position() + CVector(0.0f, 8.0f, 0.0f),
						VectorZ(),
						200.0f,
						150.0f
					);
					needle->SetColor(CColor(1.0f, 0.0f, 1.0f));
					needle->Scale(5.0f, 5.0f, 5.0f);
					needle->Rotate(-90.0f, 0.0f, 0.0f);
					needle->SetOwner(this);

					mIsSpawnedNeedleEffect = true;
					mStateStep++;
				}
			}
		}
		else
		{
			mStateStep++;
		}
		break;
		// �X�e�b�v2 : �U���I��
	case 2:
		if (mAnimationFrame >= 13.0f)
		{
			AttackEnd();
			mStateStep++;
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
}

// �U���I���҂�
void CBee::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle2);
	}
}

// �q�b�g
void CBee::UpdateHit()
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
void CBee::UpdateDie()
{
	SetAnimationSpeed(0.15f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::BeeDeath();
	}
}

// �ړ�
void CBee::UpdateRun()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// �͈͓��̎��A�ړ����ǐՂ���
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
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
	if (vectorPos <= 33.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
	// �ǐՂ��~�܂������A�ҋ@���[�V������
	else if (vectorPos <= STOP_RANGE || vectorPos > WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// �X�V����
void CBee::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

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
	CVector gaugePos = Position() + CVector(0.0f, 35.0f, 0.0f);
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

		if (mAttackTime > 150)
		{
			ChangeState(EState::eAttack);
		}
		if (mState == EState::eAttack)
		{
			mAttackTime = 0;
		}
		if (vectorPos >= WALK_RANGE)
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

	if (mState == EState::eRun)
	{
		mFlyingTime++;
		if (mFlyingTime <= 200 && mFlyingTime > 0)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + MOVE_SPEED_Y);
		}
	}

	if (mFlyingTime >= 200 && Position().Y() >= 0.1f)
	{
		Position(Position().X(), Position().Y() - HEIGHT, Position().Z());
	}

	if (mState == EState::eHit)
	{
		Position(Position().X(), Position().Y() - HEIGHT, Position().Z());
	}

	if (vectorPos <= STOP_RANGE_Y && player->Position().Y() > PLAYER_HEIGHT)
	{
		ChangeState(EState::eIdle2);
	}

	if (Position().Y() <= 0.0f)
	{
		mFlyingTime = 0;
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �L�����N�^�[�����߂��R���C�_�[
	mpColliderSphereHead->Update();
	mpColliderSphereBeak->Update();
	mpColliderSphereBeak2->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereTail->Update();
	mpColliderSphereTail2->Update();
	mpColliderSphereTail3->Update();
	mpColliderSphereTail4->Update();
	mpColliderSphereTail5->Update();

	// �_���[�W���󂯂�R���C�_�[
	mpDamageColHead->Update();
	mpDamageColBeak->Update();
	mpDamageColBeak2->Update();
	mpDamageColBody->Update();
	mpDamageColTail->Update();
	mpDamageColTail2->Update();
	mpDamageColTail3->Update();
	mpDamageColTail4->Update();
	mpDamageColTail5->Update();

	// �U���R���C�_�[
	mpAttackCol->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// �Փˏ���
void CBee::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackCol && mState != EState::eIdle)
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
	else if (self == mpColliderSphereHead || self == mpColliderSphereBeak || self == mpColliderSphereBeak2
		|| self == mpColliderSphereBody || self == mpColliderSphereTail || self == mpColliderSphereTail2
		|| self == mpColliderSphereTail3 || self == mpColliderSphereTail4 || self == mpColliderSphereTail5)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CBee::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	if (mState == EState::eAttack)
	{
		mpAttackCol->SetEnable(true);
	}
}

// �U���I��
void CBee::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
}

// �`��
void CBee::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CBee::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CBee::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY5_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CBee::TakeDamage(int damage, CObjectBase* causedObj)
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
		Position(Position() - dir * Scale().X() * 0.6f);
	}
}

// ���S����
void CBee::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}