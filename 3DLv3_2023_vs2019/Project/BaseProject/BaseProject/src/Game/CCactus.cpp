#include "CCactus.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"
#include "CCrackEffect.h"

// �T�{�e���̃C���X�^���X
CCactus* CCactus::spInstance = nullptr;

#define ENEMY_HEIGHT 0.3f
#define WITHIN_RANGE 40.0f       // �͈͓�
#define MOVE_SPEED 0.12f         // �ړ����x
#define GRAVITY 0.0625f          // �d��
#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 24.0f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f     // ��]����͈�

// �T�{�e���̃A�j���[�V�����f�[�^�̃e�[�u��
const CCactus::AnimData CCactus::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdlePlant.x",	true,	42.0f	},	         // �A�� 21.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdlePlantToBattle.x",	true,	63.0f	},	 // �A��2 21.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdleBattle.x",	true,	36.0f	},	// �ҋ@ 18.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdleNormal.x",	true,	82.0f	},	// �ҋ@2 41.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusAttack.x",	true,	42.0f	},	    // �U�� 21.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusAttack2.x",	true,	52.0f	},	    // �U��2 26.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusGetHit.x",	true,	46.0f	},	    // �q�b�g 23.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusDie.x",	true,	90.0f	},	        // ���� 23.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusDizzy.x",	true,	82.0f	},	        // �߂܂� 41.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusRun.x",	true,	34.0f	},	        // ���� 17.0f
	//{ "Character\\Enemy\\Cactus\\animation\\CactusIdle.x",	true,	121.0f	},	 // �n�܂�̑ҋ@ 121.0f
	//{ "Character\\Enemy\\Cactus\\animation\\CactusIdle2.x",	true,	46.0f	},	 // �n�܂�̑ҋ@2 23.0f
};

// �R���X�g���N�^
CCactus::CCactus()
	: mpRideObject(nullptr)
	, mAttackTime(0)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Cactus");

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
		this, ELayer::eEnemy,0.27f, false, 1.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.0f, 0.0f, -0.02f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,0.3f, false, 1.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.0f, -0.055f, 0.0f);

	// �L�����N�^�[�����߂�����(����)
	mpColliderSphereFeet = new CColliderSphere
	(
		this, ELayer::eEnemy,0.3f, false, 3.0f
	);
	mpColliderSphereFeet->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(����)
	mpColliderSphereLeftHand = new CColliderSphere
	(
		this, ELayer::eEnemy,0.16f, false, 1.0f
	);
	mpColliderSphereLeftHand->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereLeftHand->Position(-0.12f, 0.01f, 0.0f);

	// �L�����N�^�[�����߂�����(�E��)
	mpColliderSphereRightHand = new CColliderSphere
	(
		this, ELayer::eEnemy,0.16f, false, 1.0f
	);
	mpColliderSphereRightHand->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereRightHand->Position(0.12f, 0.01f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.27f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[��
	//�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon });
	// �_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageColHead->Position(0.0f, 0.0f, -0.02f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.3f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, -0.055f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageColFeet = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.3f, false
	);
	mpDamageColFeet->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageColLeftHand = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.16f, false
	);
	mpDamageColLeftHand->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColLeftHand->SetCollisionTags({ ETag::eWeapon });
	mpDamageColLeftHand->Position(-0.12f, 0.01f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��)
	mpDamageColRightHand = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.16f, false
	);
	mpDamageColRightHand->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColRightHand->SetCollisionTags({ ETag::eWeapon });
	mpDamageColRightHand->Position(0.12f, 0.01f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.27f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.0f, 0.0f, -0.02f);

	// �_���[�W��^����R���C�_�[(����)
	mpAttackColLeftHand = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.16f, false
	);
	mpAttackColLeftHand->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColLeftHand->SetCollisionTags({ ETag::ePlayer });
	mpAttackColLeftHand->Position(-0.12f, 0.01f, 0.0f);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[���T�{�e���̑̂̍s��ɃA�^�b�`
	const CMatrix* hitBodyMty = GetFrameMtx("Armature_cactus_spine03");
	mpColliderSphereBody->SetAttachMtx(hitBodyMty);
	mpDamageColBody->SetAttachMtx(hitBodyMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[���T�{�e���̑����̍s��ɃA�^�b�`
	const CMatrix* hitFeetMty = GetFrameMtx("Armature_cactus_spine01");
	mpColliderSphereFeet->SetAttachMtx(hitFeetMty);
	mpDamageColFeet->SetAttachMtx(hitFeetMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���T�{�e���̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_cactus_headDeco");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpDamageColHead->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���T�{�e���̍���̍s��ɃA�^�b�`
	const CMatrix* leftHandMty = GetFrameMtx("Armature_cactus_leftHandTip");
	mpColliderSphereLeftHand->SetAttachMtx(leftHandMty);
	mpDamageColLeftHand->SetAttachMtx(leftHandMty);
	mpAttackColLeftHand->SetAttachMtx(leftHandMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[���T�{�e���̉E��̍s��ɃA�^�b�`
	const CMatrix* rightHandMty = GetFrameMtx("Armature_cactus_rightHandTip");
	mpColliderSphereRightHand->SetAttachMtx(rightHandMty);
	mpDamageColRightHand->SetAttachMtx(rightHandMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColHead->SetEnable(false);
	mpAttackColLeftHand->SetEnable(false);

	mpCrack = new CCrackEffect
	(
		this, leftHandMty,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0, -90.f, 0.0f).Matrix()
	);
}

CCactus::~CCactus()
{
	SAFE_DELETE(mpColliderLine);

	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereFeet);
	SAFE_DELETE(mpColliderSphereLeftHand);
	SAFE_DELETE(mpColliderSphereRightHand);

	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColFeet);
	SAFE_DELETE(mpDamageColLeftHand);
	SAFE_DELETE(mpDamageColRightHand);

	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColLeftHand);
}

CCactus* CCactus::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CCactus::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �키�O�̑ҋ@���
void CCactus::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// �키�O�̑ҋ@���2
void CCactus::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// �ҋ@���3
void CCactus::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle3);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mState = EState::eRun;
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle3);
		if (IsAnimationFinished())
		{
			mState = EState::eIdle3;
		}
	}
}

// �U��
void CCactus::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	if (!mpCrack->IsThrowing())
	{
		mpCrack->Start();
	}
	if (mAnimationFrame >= 10.0f && mAnimationFrame < 20.0f)
	{
		AttackStart();
		mpAttackColHead->SetEnable(false);
	}
	if (mAnimationFrame >=30.0f && mAnimationFrame < 31.0f)
	{
		AttackEnd();
	}

	if (IsAnimationFinished())
	{
		mpCrack->Stop();
		// �U���I���҂���Ԃֈڍs
		mState = EState::eAttackWait;
	}
}

// �U��2
void CCactus::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	mpAttackColLeftHand->SetEnable(false);
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U���I���҂�
void CCactus::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpCrack->Stop();
		mState = EState::eIdle3;
	}
}

// �q�b�g
void CCactus::UpdateHit()
{
	mpCrack->Stop();
	// �q�b�g�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// �߂܂���false�ɂ���
		bool stan = false;
		// �m�����ŏ���0�ő�40
		int probability = Math::Rand(0, 20);
		if (probability == 1)stan = true;
		if (stan)
		{
			mState = EState::eDizzy;
		}
		else
		{
			// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
			mState = EState::eIdle3;
			ChangeAnimation(EAnimType::eIdle3);
		}
	}
}

// ����
void CCactus::UpdateDie()
{
	mpCrack->Stop();
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::CactusDeath();
	}
}

// �߂܂�(����)
void CCactus::UpdateDizzy()
{
	mpCrack->Stop();
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle3);
	}
}

// �ړ�
void CCactus::UpdateRun()
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
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle3);
	}
}

// �X�V����
void CCactus::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �키�O�̑ҋ@���
	case EState::eIdle:
		UpdateIdle();
		break;
		// �키�O�̑ҋ@���2
	case EState::eIdle2:
		UpdateIdle2();
		break;
		// �ҋ@���3
	case EState::eIdle3:
		UpdateIdle3();
		break;
		// �U��
	case EState::eAttack:
		UpdateAttack();
		break;
		// �U��2
	case EState::eAttack2:
		UpdateAttack2();
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
		// �߂܂�(����)
	case EState::eDizzy:
		UpdateDizzy();
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

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eIdle3
		&& mState != EState::eAttack && mState != EState::eAttackWait && mState != EState::eHit && mState != EState::eDie
		&& mState != EState::eDizzy && mState != EState::eRun)
	{
		UpdateIdle();
	}

	if (mState == EState::eRun || mState == EState::eIdle3 || mState == EState::eAttack || mState == EState::eAttack2 ||
		mState == EState::eHit || mState == EState::eDie || mState == EState::eDizzy || mState == EState::eAttackWait)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle3 && mState != EState::eDizzy || mState == EState::eRun)
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
			// �U��2
			bool Attack2 = false;
			// �m�����ŏ���3�ő�6
			int probability2 = Math::Rand(2, 5);
			if (probability2 == 2)Attack2 = true;
			if (Attack2)
			{
				mState = EState::eAttack2;
			}
			else
			{
				mState = EState::eAttack;
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2)
		{
			mAttackTime = 0;
		}
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (Position().Y() >= 0.1f)
	{
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereFeet->Update();
	mpColliderSphereLeftHand->Update();
	mpColliderSphereRightHand->Update();

	mpDamageColHead->Update();
	mpDamageColBody->Update();
	mpDamageColFeet->Update();
	mpDamageColLeftHand->Update();
	mpDamageColRightHand->Update();

	mpAttackColHead->Update();
	mpAttackColLeftHand->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// �Փˏ���
void CCactus::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColLeftHand && mState != EState::eIdle
		&& mState != EState::eIdle2 && mState != EState::eIdle3)
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
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColHead && mState != EState::eIdle
		&& mState != EState::eIdle2 && mState != EState::eIdle3)
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
				damage = damage * 2;

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
	else if (self == mpColliderSphereHead || self == mpColliderSphereBody || self == mpColliderSphereFeet
		|| self == mpColliderSphereLeftHand || self == mpColliderSphereRightHand)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CCactus::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackColHead->SetEnable(true);
	mpAttackColLeftHand->SetEnable(true);
}

// �U���I��
void CCactus::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackColHead->SetEnable(false);
	mpAttackColLeftHand->SetEnable(false);
}

// �`��
void CCactus::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CCactus::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CCactus::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY6_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CCactus::TakeDamage(int damage, CObjectBase* causedObj)
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
void CCactus::Death()
{
	// ���S��Ԃֈڍs
	mState = EState::eDie;
}