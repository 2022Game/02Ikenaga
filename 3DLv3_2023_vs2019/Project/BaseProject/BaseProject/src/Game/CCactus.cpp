#include "CCactus.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"
#include "CCrackEffect.h"

// �T�{�e���̃C���X�^���X
CCactus* CCactus::spInstance = nullptr;

#define ENEMY_HEIGHT  0.3f     // �����R���C�_�[
#define WITHIN_RANGE  40.0f    // �͈͓�
#define MOVE_SPEED    0.6f    // �ړ����x
#define GRAVITY       0.0625f  // �d��
#define WALK_RANGE    100.0f   // �ǐՂ���͈�
#define STOP_RANGE    24.0f    // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f   // ��]����͈�

// �T�{�e���̃A�j���[�V�����f�[�^�̃e�[�u��
const CCactus::AnimData CCactus::ANIM_DATA[] =
{
	{ "",										                        true,  0.0f,   0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdlePlant.x",	        true,  21.0f,  0.5f},  // �A�� 21.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdlePlantToBattle.x",	true,  21.0f,  0.3f},  // �A��2 21.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdleBattle.x",	    true,  18.0f,  0.5f},  // �ҋ@ 18.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusIdleNormal.x",	    true,  41.0f,  0.5f},  // �ҋ@2 41.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusAttack.x",	        true,  21.0f,  0.5f},  // �U�� 21.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusAttack2.x",	        true,  26.0f,  0.5f},  // �U��2 26.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusGetHit.x",	        true,  23.0f,  0.3f},  // �q�b�g 23.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusDie.x",	            true,  23.0f,  0.2f},  // ���� 23.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusDizzy.x",          	true,  41.0f,  0.5f},  // �߂܂� 41.0f
	{ "Character\\Enemy\\Cactus\\animation\\CactusRun.x",	            true,  17.0f,  0.4f},  // ���� 17.0f
	//{ "Character\\Enemy\\Cactus\\animation\\CactusIdle.x",	true,	121.0f	},	 // �n�܂�̑ҋ@ 121.0f
	//{ "Character\\Enemy\\Cactus\\animation\\CactusIdle2.x",	true,	46.0f	},	 // �n�܂�̑ҋ@2 23.0f
};

// �R���X�g���N�^
CCactus::CCactus()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mStateAttackStep(0)
	, mStateAttack2Step(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
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
		0.25f, false, 1.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.0f, 0.0f, -0.02f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.3f, false, 1.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.0f, -0.055f, 0.0f);

	// �L�����N�^�[�����߂�����(����)
	mpColliderSphereFeet = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.3f, false, 3.0f
	);
	mpColliderSphereFeet->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(����)
	mpColliderSphereLeftHand = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.16f, false, 1.0f
	);
	mpColliderSphereLeftHand->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereLeftHand->Position(-0.12f, 0.01f, 0.0f);

	// �L�����N�^�[�����߂�����(�E��)
	mpColliderSphereRightHand = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.16f, false, 1.0f
	);
	mpColliderSphereRightHand->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy, ELayer::eField });
	mpColliderSphereRightHand->Position(0.12f, 0.01f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.27f, false
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
		this, ELayer::eDamageCol,
		0.3f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, -0.055f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageColFeet = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.3f, false
	);
	mpDamageColFeet->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageColLeftHand = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.3f, 0.0f, 0.0f),
		4.3f, false
	);
	mpDamageColLeftHand->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColLeftHand->SetCollisionTags({ ETag::eWeapon });
	mpDamageColLeftHand->Position(0.0f, 0.0f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��)
	mpDamageColRightHand = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.3f, 0.0f, 0.0f),
		4.3f, false
	);
	mpDamageColRightHand->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColRightHand->SetCollisionTags({ ETag::eWeapon });
	mpDamageColRightHand->Position(0.12f, 0.01f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.27f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.0f, 0.0f, -0.02f);

	// �_���[�W��^����R���C�_�[(����)
	mpAttackColLeftHand = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.16f, false
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

	// �����߂��R���C�_�[�ƍU���R���C�_�[���T�{�e���̍���̍s��ɃA�^�b�`
	const CMatrix* leftHandMty = GetFrameMtx("Armature_cactus_leftHandTip");
	mpColliderSphereLeftHand->SetAttachMtx(leftHandMty);
	mpAttackColLeftHand->SetAttachMtx(leftHandMty);

	// �_���[�W���󂯂�R���C�_�[���T�{�e���̍��r�̍s��ɃA�^�b�`
    const CMatrix* leftArmMty = GetFrameMtx("Armature_cactus_leftLowerArm");
	mpDamageColLeftHand->SetAttachMtx(leftArmMty);

	// �����߂��R���C�_�[���T�{�e���̉E��̍s��ɃA�^�b�`
	const CMatrix* rightHandMty = GetFrameMtx("Armature_cactus_rightHandTip");
	mpColliderSphereRightHand->SetAttachMtx(rightHandMty);

	// ���[�W���󂯂�R���C�_�[���T�{�e���̉E�r�̍s��ɃA�^�b�`
	const CMatrix* rightArmMty = GetFrameMtx("Armature_cactus_rightLowerArm");
	mpDamageColRightHand->SetAttachMtx(rightArmMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColHead->SetEnable(false);
	mpAttackColLeftHand->SetEnable(false);

	// �Ђъ���G�t�F�N�g�쐬
	mpCrack = new CCrackEffect
	(
		this, leftHandMty,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0, -90.f, 0.0f).Matrix()
	);
}

// �f�X�g���N�^
CCactus::~CCactus()
{
	// �L�����N�^�[�̐����R���C�_�[
	SAFE_DELETE(mpColliderLine);
	// �L�����N�^�[�̉����߂��R���C�_�[
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereFeet);
	SAFE_DELETE(mpColliderSphereLeftHand);
	SAFE_DELETE(mpColliderSphereRightHand);
	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColFeet);
	SAFE_DELETE(mpDamageColLeftHand);
	SAFE_DELETE(mpDamageColRightHand);
	// �U���R���C�_�[
	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColLeftHand);
}

// �C���X�^���X
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

// ��Ԃ̐؂�ւ�
void CCactus::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
	mStateAttack2Step = 0;
}

// �키�O�̑ҋ@���
void CCactus::UpdateIdle()
{
	SetAnimationSpeed(0.5f);
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

// �키�O�̑ҋ@���2
void CCactus::UpdateIdle2()
{
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// �ҋ@���3
void CCactus::UpdateIdle3()
{
	mpColliderSphereHead->SetEnable(true);
	mpColliderSphereBody->SetEnable(true);
	mpColliderSphereFeet->SetEnable(true);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >= 10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE
		&& player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 33.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle3);
	}
}

// �U��
void CCactus::UpdateAttack()
{
	SetAnimationSpeed(0.5f);
	mpAttackColHead->SetEnable(false);
	CPlayer* player = CPlayer::Instance();

	// �Ђъ��ꂵ�Ă��邩�ǂ���
	if (!mpCrack->IsThrowing())
	{
		mpCrack->Start();
	}

	// �p���`�J�n����p���`�Ŏ��L�΂�����܂ł́A
	// �v���C���[�̕����Ɍ���
	if (mAnimationFrame <= 30.0f)
	{
		// �v���C���[�̂�������֌���
		CVector dir = CPlayer::Instance()->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

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
		if (mAnimationFrame >= 5.0f && mAnimationFrame < 15.0f)
		{
			AttackStart();
			mStateAttackStep++;
		}
		break;
		// �X�e�b�v2 : ����̃L�����N�^�[�̉����߂��R���C�_�[�I�t
	case 2:
		if (mAnimationFrame >= 11.0f)
		{
			mpColliderSphereLeftHand->SetEnable(false);
			mStateAttackStep++;
		}
		break;
		// �X�e�b�v3 : �U���I��
	case 3:
		if (mAnimationFrame >= 15.0f)
		{
			AttackEnd();
			mStateAttackStep++;
		}
		break;
		// �X�e�b�v4 : �U���I���҂��{�Ђъ�������߂�
	case 4:
		if (mAnimationFrame >= 19.0f)
		{
			mpCrack->Stop();
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U��2
void CCactus::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);
	mpAttackColLeftHand->SetEnable(false);

	if (mAnimationFrame <= 30.0f)
	{
		// �v���C���[�̂�������֌���
		CVector dir = CPlayer::Instance()->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateAttack2Step)
	{
		// �X�e�b�v0 : �U���A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		mStateAttack2Step++;
		break;
	case 1:
		if (mAnimationFrame >= 10.0f)
		{
			AttackStart();
			mStateAttack2Step++;
		}
		break;
	case 2:
		if (mAnimationFrame >= 24.0f)
		{
			// �U��2�I���҂���Ԃֈڍs
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U���I���҂�
void CCactus::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpCrack->Stop();
		mpColliderSphereLeftHand->SetEnable(true);
		ChangeState(EState::eIdle3);
	}
}

// �q�b�g
void CCactus::UpdateHit()
{
	mpCrack->Stop();
	mpColliderSphereHead->SetEnable(false);
	mpColliderSphereBody->SetEnable(false);
	mpColliderSphereFeet->SetEnable(false);
	SetAnimationSpeed(0.3f);
	// �q�b�g�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eHit);

	// �߂܂���false�ɂ���
	bool stan = false;
	if (GetAnimationFrameRatio() >= 0.8f)
	{
		// �m�����ŏ���0�ő�20
		int probability = Math::Rand(0, 20);
		if (probability == 1)stan = true;
		if (stan)
		{
			ChangeState(EState::eDizzy);
		}
		else 
		{
			ChangeState(EState::eIdle3);
		}
	}
}

// ����
void CCactus::UpdateDie()
{
	mpCrack->Stop();
	SetAnimationSpeed(0.2f);
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
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle3);
	}
}

// �ړ�
void CCactus::UpdateRun()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// �͈͓��̎��A�ړ����ǐՂ���
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
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
	// �ړ��A�j���[�V�����ŃT�{�e�����n�ʂɕt���Ă��邾��
	// �ҋ@���[�V�����ɐ؂�ւ���
	if (!(3.0f <= mAnimationFrame && mAnimationFrame <= 15.0f))
	{
		if (vectorPos <= 33.0f && player->Position().Y() >= 1.0f)
		{
			ChangeState(EState::eIdle3);
		}
		// �ǐՂ��~�܂������A�ҋ@���[�V������
		else if (vectorPos <= STOP_RANGE || vectorPos > WALK_RANGE)
		{
			ChangeState(EState::eIdle3);
		}
	}
}

// �X�V����
void CCactus::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
	mMoveSpeed.Z(0.0f);

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
	CVector gaugePos = Position() + CVector(0.0f, 37.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState != EState::eIdle && mState != EState::eIdle2)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle3 || mState == EState::eRun || mState == EState::eHit)
	{
		if (vectorPos <= WITHIN_RANGE)
		{
			mAttackTime++;
		}

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
			// �U��2
			bool Attack2 = false;
			// �m�����ŏ���3�ő�6
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

	if (mState == EState::eRun)
	{
		if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
		{
			Position(Position() + mMoveSpeed);
		}
	}

	if (Position().Y() >= 0.1f)
	{
		Position(Position().X(), Position().Y() - 1.0f, Position().Z());
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �L�����N�^�[�̉����߂��R���C�_�[
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereFeet->Update();
	mpColliderSphereLeftHand->Update();
	mpColliderSphereRightHand->Update();
	// �_���[�W���󂯂�R���C�_�[
	mpDamageColHead->Update();
	mpDamageColBody->Update();
	mpDamageColFeet->Update();
	mpDamageColLeftHand->Update();
	mpDamageColRightHand->Update();
	// �U���R���C�_�[
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
		// �L�����N�^�[�̃|�C���^�ɕϊ�
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
		// �L�����N�^�[�̃|�C���^�ɕϊ�
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
	else if (self == mpColliderLine || self == mpColliderSphereRightHand)
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
	if (mState == EState::eAttack2)
	{
		mpAttackColHead->SetEnable(true);
	}
	if (mState == EState::eAttack)
	{
		mpAttackColLeftHand->SetEnable(true);
	}
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
		Position(Position() - dir * Scale().X() * 0.1f);
	}
}

// ���S����
void CCactus::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}