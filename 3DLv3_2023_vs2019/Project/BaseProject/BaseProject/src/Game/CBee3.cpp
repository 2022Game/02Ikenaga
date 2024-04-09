#include "CBee3.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"
#include "CNeedle.h"

// �I3�̃C���X�^���X
CBee3* CBee3::spInstance = nullptr;

#define ENEMY_HEIGHT 0.3f
#define WITHIN_RANGE 40.0f       // �͈͓�
#define MOVE_SPEED 0.12f         // �ړ����x
#define GRAVITY 0.0625f          // �d��
#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 18.0f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f     // ��]����͈�

// �I3�̃A�j���[�V�����f�[�^�̃e�[�u��
const CBee3::AnimData CBee3::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	{ "Character\\Enemy\\Bee\\animation\\BeeIdle.x",	true,	40.0f	},	    // �ҋ@ 20.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeAttack.x",	true,	40.0f	},	    // �U�� 17.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeGetHit.x",	true,	50.0f	},	    // �q�b�g 13.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeDie.x",	true,	90.0f	},	        // ���� 20.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveBWD.x",	true,	42.0f	},	    // �ړ� 21.0f
	{ "Character\\Enemy\\Bee\\animation\\BeeMoveFWD.x",	true,	42.0f	},	    // �ړ�2 21.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveLFT.x",	true,	42.0f	},	    // ���ړ� 21.0f
	//{ "Character\\Enemy\\Bee\\animation\\BeeMoveRGT.x",	true,	42.0f	},	    // �E�ړ� 21.0f
};

// �R���X�g���N�^
CBee3::CBee3()
	: mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
	, mIsSpawnedNeedleEffect(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Bee");

	//�ŏ���1���x���ɐݒ�
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
	mpColliderLine->Position(0.0f, 5.0f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 2.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
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
		this, ELayer::eEnemy, 0.25f, false, 2.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
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
		this, ELayer::eDamageCol, 0.25f, false
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
		this, ELayer::eDamageCol, 0.1f, false
	);
	mpDamageColTail2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail2->Position(0.02f, 0.14f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�K��3)
	mpDamageColTail3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.05f, false
	);
	mpDamageColTail3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail3->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail3->Position(-0.05f, 0.0f, 0.04f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�K��4)
	mpDamageColTail4 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.03f, false
	);
	mpDamageColTail4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail4->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail4->Position(-0.15f, 0.0f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�K��5)
	mpDamageColTail5 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.02f, false
	);
	mpDamageColTail5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTail5->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTail5->Position(-0.25f, 0.0f, 0.105f);

	// �_���[�W��^����R���C�_�[
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.01f, false
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

	// �L�����N�^�[�����߂��ƃ_���[�W���󂯂�R���C�_�[��̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);

	// �L�����N�^�[�����߂��ƃ_���[�W���󂯂�R���C�_�[��K���̍s��ɃA�^�b�`
	const CMatrix* tailMty = GetFrameMtx("Armature_Tail01");
	mpColliderSphereTail->SetAttachMtx(tailMty);
	mpColliderSphereTail2->SetAttachMtx(tailMty);
	mpDamageColTail->SetAttachMtx(tailMty);
	mpDamageColTail2->SetAttachMtx(tailMty);

	// �L�����N�^�[�����߂��ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[��I�̐j�̍s��ɃA�^�b�`
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
}

CBee3::~CBee3()
{
	SAFE_DELETE(mpColliderLine);
	
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBeak);
	SAFE_DELETE(mpColliderSphereBeak2);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereTail);
	SAFE_DELETE(mpColliderSphereTail2);
	SAFE_DELETE(mpColliderSphereTail3);
	SAFE_DELETE(mpColliderSphereTail4);
	SAFE_DELETE(mpColliderSphereTail5);

	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBeak);
	SAFE_DELETE(mpDamageColBeak2);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColTail);
	SAFE_DELETE(mpDamageColTail2);
	SAFE_DELETE(mpDamageColTail3);
	SAFE_DELETE(mpDamageColTail4);
	SAFE_DELETE(mpDamageColTail5);
}

CBee3* CBee3::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CBee3::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �ҋ@���
void CBee3::UpdateIdle()
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
void CBee3::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	
	if (mAnimationFrame >= 5.0f)
	{
		AttackStart();
	}
	if (mAnimationFrame >= 15.0f)
	{
		AttackEnd();
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (mAnimationFrame >= 14.0f)
	{
		if (vectorp >= 30.0f)
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
					150.0f,
					100.0f
				);
				needle->SetColor(CColor(1.0f, 0.0f, 1.0f));
				needle->Scale(5.0f, 5.0f, 5.0f);
				needle->Rotate(-90.0f, 0.0f, 0.0f);
				needle->SetOwner(this);

				mIsSpawnedNeedleEffect = true;
			}
		}

		// �U���I���҂���Ԃֈڍs
		mState = EState::eAttackWait;
	}
}

// �U���I���҂�
void CBee3::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle;
	}
}

// �q�b�g
void CBee3::UpdateHit()
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
void CBee3::UpdateDie()
{
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::Death();
	}
}

// �ړ�
void CBee3::UpdateRun()
{
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// �ǐՂ���߂Ď~�܂�
	if (vectorp <= 20.0f && vectorp >= 23.0f)
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
void CBee3::Update()
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
	CVector gaugePos = Position() + CVector(0.0f, 35.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eAttack && mState != EState::eAttackWait
		&& mState != EState::eHit && mState != EState::eDie && mState != EState::eRun)
	{
		mState = EState::eIdle;
	}

	if (mState == EState::eRun || mState == EState::eIdle || mState == EState::eAttack ||
		mState == EState::eDie || mState == EState::eHit || mState == EState::eAttackWait)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle && vectorp <= 40.0f || mState == EState::eHit || mState == EState::eRun
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
		if (mFlyingTime <= 200 && Position().Y() <= 0.0f)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + 0.02f);
		}

		if (mFlyingTime >= 200 && Position().Y() >= 0.1f)
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

	mpColliderSphereHead->Update();
	mpColliderSphereBeak->Update();
	mpColliderSphereBeak2->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereTail->Update();
	mpColliderSphereTail2->Update();
	mpColliderSphereTail3->Update();
	mpColliderSphereTail4->Update();
	mpColliderSphereTail5->Update();

	mpDamageColHead->Update();
	mpDamageColBeak->Update();
	mpDamageColBeak2->Update();
	mpDamageColBody->Update();
	mpDamageColTail->Update();
	mpDamageColTail2->Update();
	mpDamageColTail3->Update();
	mpDamageColTail4->Update();
	mpDamageColTail5->Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// �Փˏ���
void CBee3::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CBee3::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
}

// �U���I��
void CBee3::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
}

// �`��
void CBee3::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CBee3::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CBee3::ChangeLevel(int level)
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
void CBee3::TakeDamage(int damage, CObjectBase* causedObj)
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
		Position(Position() - dir * Scale().X() * 0.6f);
	}
}

// ���S����
void CBee3::Death()
{
	// ���S��Ԃֈڍs
	mState = EState::eDie;
}