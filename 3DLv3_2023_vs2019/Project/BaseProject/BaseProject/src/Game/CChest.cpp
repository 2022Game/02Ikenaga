#include "CChest.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"
#include "CCoin.h"

// �`�F�X�g�����X�^�[�̃C���X�^���X
CChest* CChest::spInstance = nullptr;

#define ENEMY_HEIGHT  0.3f     // �����R���C�_�[
#define WITHIN_RANGE  40.0f    // �͈͓�
#define MOVE_SPEED    0.12f    // �ړ����x
#define GRAVITY       0.0625f  // �d��
#define WALK_RANGE    150.0f   // �ǐՂ���͈�
#define STOP_RANGE    32.0f    // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f   // ��]����͈�

// �`�F�X�g�����X�^�[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CChest::AnimData CChest::ANIM_DATA[] =
{
	{ "",										            true,	0.0f,	 0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Chest\\animation\\ChestIdle.x",	true,	21.0f,	 1.0f},	 // �ҋ@ 21.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestIdle2.x",	true,	23.0f,	 0.5f},	 // �ҋ@2 23.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestIdle3.x",	true,	41.0f,	 0.5f},	 // �ҋ@3 41.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestAttack.x",	true,	25.0f,	 0.5f},	 // �U�� 25.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestAttack2.x",	true,	23.0f,	 0.5f},	 // �U��2 23.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestGetHit.x",	true,	19.0f,	 0.4f},	 // �q�b�g 19.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestDie.x",	    true,	29.0f,	0.25f},  // ���� 29.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestDizzy.x",	true,	41.0f,	 0.5f},	 // �߂܂� 41.0f
	{ "Character\\Enemy\\Chest\\animation\\ChestRun.x",	    true,	17.0f,	 0.5f},	 // ���� 17.0f
};

// �R���X�g���N�^
CChest::CChest()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mStateAttackStep(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Chest");

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

	SetAnimationSpeed(1.0f);
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
		this, ELayer::eEnemy,
		0.55f, false, 5.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.27f, 0.0f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.55f, false, 5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.05f, 0.0f, 0.0f);

	// �L�����N�^�[�����߂�����(�O�̍���)
	mpColliderSphereFeet = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 5.0f
	);
	mpColliderSphereFeet->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeet->Position(0.0f, 0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(�O�̉E��)
	mpColliderSphereFeet2 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 5.0f
	);
	mpColliderSphereFeet2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeet2->Position(0.0f, 0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(���̍���)
	mpColliderSphereFeet3 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 5.0f
	);
	mpColliderSphereFeet3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeet3->Position(0.0f, 0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(���̉E��)
	mpColliderSphereFeet4 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 5.0f
	);
	mpColliderSphereFeet4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeet4->Position(0.0f, 0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.55f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[��
	//�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon });
	// �_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageColHead->Position(0.27f, 0.0f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.55f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.05f, 0.0f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�O�̍���)
	mpDamageColFeet = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.08f, false
	);
	mpDamageColFeet->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet->Position(0.0f, 0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�O�̉E��)
	mpDamageColFeet2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.08f, false
	);
	mpDamageColFeet2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet2->Position(0.0f, 0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(���̍���)
	mpDamageColFeet3 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.08f, false
	);
	mpDamageColFeet3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet3->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet3->Position(0.0f, 0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(���̉E��)
	mpDamageColFeet4 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.08f, false
	);
	mpDamageColFeet4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet4->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet4->Position(0.0f, 0.1f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.55f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.27f, 0.0f, 0.0f);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[��
	// �U���R���C�_�[���`�F�X�g�����X�^�[�̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpDamageColHead->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// �����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���`�F�X�g�����X�^�[�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);

	// �����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���`�F�X�g�����X�^�[�̑O�̍����̍s��ɃA�^�b�`
	const CMatrix* leftFeetMty = GetFrameMtx("Armature_FrontLeftLeg02");
	mpColliderSphereFeet->SetAttachMtx(leftFeetMty);
	mpDamageColFeet->SetAttachMtx(leftFeetMty);

	// �����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���`�F�X�g�����X�^�[�̑O�̉E���̍s��ɃA�^�b�`
	const CMatrix* rightFeetMty = GetFrameMtx("Armature_FrontRightLeg02");
	mpColliderSphereFeet2->SetAttachMtx(rightFeetMty);
	mpDamageColFeet2->SetAttachMtx(rightFeetMty);

	// �����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���`�F�X�g�����X�^�[�̌��̍����̍s��ɃA�^�b�`
	const CMatrix* leftFeetMty2 = GetFrameMtx("Armature_RearLeftLeg02");
	mpColliderSphereFeet3->SetAttachMtx(leftFeetMty2);
	mpDamageColFeet3->SetAttachMtx(leftFeetMty2);

	// �����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���`�F�X�g�����X�^�[�̌��̉E���̍s��ɃA�^�b�`
	const CMatrix* rightFeetMty2 = GetFrameMtx("Armature_RearRightLeg02");
	mpColliderSphereFeet4->SetAttachMtx(rightFeetMty2);
	mpDamageColFeet4->SetAttachMtx(rightFeetMty2);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColHead->SetEnable(false);
}

CChest::~CChest()
{
	SAFE_DELETE(mpColliderLine);
	// �L�����N�^�[�̉����߂��R���C�_�[
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereFeet);
	SAFE_DELETE(mpColliderSphereFeet2);
	SAFE_DELETE(mpColliderSphereFeet3);
	SAFE_DELETE(mpColliderSphereFeet4);
	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColFeet);
	SAFE_DELETE(mpDamageColFeet2);
	SAFE_DELETE(mpDamageColFeet3);
	SAFE_DELETE(mpDamageColFeet4);
	// �U���R���C�_�[
	SAFE_DELETE(mpAttackColHead);
}

// �C���X�^���X
CChest* CChest::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CChest::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CChest::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
}

// �키�O�̑ҋ@���
void CChest::UpdateIdle()
{
	SetAnimationSpeed(1.0f);
	ChangeAnimation(EAnimType::eIdle);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= WITHIN_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
	else
	{
		ChangeState(EState::eIdle);
	}
}

// �ҋ@���2
void CChest::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		ChangeState(EState::eRun);
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle2);
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle2);
		}
	}
}

// �ҋ@���3
void CChest::UpdateIdle3()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// �U��
void CChest::UpdateAttack()
{
	SetAnimationSpeed(0.5f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

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
		if (mAnimationFrame >= 5.0f && mAnimationFrame < 10.0f)
		{
			AttackStart();
			mStateAttackStep++;
		}
		break;
		// �X�e�b�v2 : �U���I�����v���C���[�Ƃ̋�����45.0f�ȏ�̎��A�R�C������
	case 2:
		if (mAnimationFrame >= 10.0f)
		{
			AttackEnd();
			if (vectorp >= 50.0f)
			{
				// �R�C���𐶐��ς݃t���O��������
				mIsSpawnedCoinEffect = false;
				// �R�C���𐶐����Ă��Ȃ�
				if (!mIsSpawnedCoinEffect)
				{
					CCoin* coin = new CCoin
					(
						this,
						Position() + CVector(0.0f, 18.0f, 0.0f),
						VectorZ(),
						180.0f,
						150.0f
					);
					coin->SetColor(CColor(1.0f, 1.0f, 0.0f));
					coin->Scale(10.0f, 10.0f, 10.0f);
					coin->Rotate(-90.0f, 0.0f, 0.0f);
					coin->SetOwner(this);

					mIsSpawnedCoinEffect = true;
					mStateAttackStep++;
				}
			}
			else
			{
				mStateAttackStep++;
			}
		}
		break;
		// �X�e�b�v3 : �U���I���҂�
	case 3:
		if (mAnimationFrame >= 23.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U��2
void CChest::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);
}

// �U���I���҂�
void CChest::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle2);
	}
}

// �q�b�g
void CChest::UpdateHit()
{
	SetAnimationSpeed(0.4f);
	// �q�b�g�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// �߂܂���false�ɂ���
		bool stan = false;
		// �m�����ŏ���0�ő�20
		int probability = Math::Rand(0, 20);
		if (probability == 1)stan = true;
		if (stan)
		{
			ChangeState(EState::eDizzy);
		}
		else
		{
			// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
			ChangeState(EState::eIdle2);
		}
	}
}

// ����
void CChest::UpdateDie()
{
	SetAnimationSpeed(0.25f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::ChestDeath();
	}
}

// �߂܂�(����)
void CChest::UpdateDizzy()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle2);
	}
}

// �ړ�
void CChest::UpdateRun()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// �ǐՂ���߂Ď~�܂�
	if (vectorp <= STOP_RANGE && vectorp >= 35.0f)
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
		ChangeState(EState::eIdle2);
	}
}

// �X�V����
void CChest::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.Y(0.0f);

	if (mState != EState::eRun)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
	}

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
	CVector gaugePos = Position() + CVector(0.0f, 40.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (mState != EState::eIdle && mState != EState::eDie)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 && mState != EState::eDizzy || mState == EState::eRun)
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
			// �m�����ŏ���2�ő�5
			int probability2 = Math::Rand(2, 5);
			if (probability2 == 2)Attack2 = true;
			if (Attack2)
			{
				ChangeState(EState::eAttack2);
			}
			else
			{
				ChangeState(EState::eAttack);
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
	// �L�����N�^�[�̉����߂��R���C�_�[
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereFeet->Update();
	mpColliderSphereFeet2->Update();
	mpColliderSphereFeet3->Update();
	mpColliderSphereFeet4->Update();
	// �_���[�W���󂯂�R���C�_�[
	mpDamageColHead->Update();
	mpDamageColBody->Update();
	mpDamageColFeet->Update();
	mpDamageColFeet2->Update();
	mpDamageColFeet3->Update();
	mpDamageColFeet4->Update();
	// �U���R���C�_�[
	mpAttackColHead->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// �Փˏ���
void CChest::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColHead && mState != EState::eIdle && mState != EState::eIdle2 &&
		mState != EState::eIdle3)
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
	else if (self == mpColliderSphereHead || self == mpColliderSphereBody || self == mpColliderSphereFeet
		|| self == mpColliderSphereFeet2 || self == mpColliderSphereFeet3 || self == mpColliderSphereFeet4)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CChest::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	if (mState == EState::eAttack || mState == EState::eAttack2)
	{
		mpAttackColHead->SetEnable(true);
	}
}

// �U���I��
void CChest::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackColHead->SetEnable(false);
}

// �`��
void CChest::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CChest::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CChest::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY7_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CChest::TakeDamage(int damage, CObjectBase* causedObj)
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
void CChest::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}