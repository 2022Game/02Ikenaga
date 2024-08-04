#include "CMushroom.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CGameEnemyUI.h"
#include "CHit.h"
#include "CDizzyEffect.h"
#include "Maths.h"

// �}�b�V�����[���̃C���X�^���X
CMushroom* CMushroom::spInstance = nullptr;

#define ENEMY_SIDE      0.7f  // �����R���C�_�[(��)
#define ENEMY_HEIGHT    1.0f  // �����R���C�_�[(�c)
#define WITHIN_RANGE   40.0f  // �͈͓�
#define MOVE_SPEED      0.4f  // �ړ����x
#define GRAVITY         0.3f  // �d��
#define WALK_RANGE    100.0f  // �ǐՂ���͈�
#define STOP_RANGE     20.0f  // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f  // ��]����͈�

// �}�b�V�����[���̃A�j���[�V�����f�[�^�̃e�[�u��
const CMushroom::AnimData CMushroom::ANIM_DATA[] =
{
	{ "",										                            true,	0.0f,	0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlant.x",	        true,	21.0f,	0.3f},	// �A��
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlantToBattle.x",	false,	21.0f,	0.3f},  // �A�����炫�̂�
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle2.x",	    true,	18.0f,	0.5f},	// �ҋ@2
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle.x",	    true,	18.0f,	0.5f},	// �ҋ@
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack.x",	        false,	26.0f,	0.4f},	// �U��
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack2.x",	        false,	26.0f,	0.6f},	// �U��
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack3.x",	        false,	26.0f,	0.5f},	// �U��
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomGetHit.x",	        false,	23.0f,	0.4f},	// �q�b�g
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomDie.x",	            false,	26.0f, 0.25f},	// ����
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomDizzy.x",	            false,	41.0f,  0.5f},	// �߂܂�
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomRun.x",	            true,	17.0f, 	0.4f},	// �ړ�
};

// �R���X�g���N�^
CMushroom::CMushroom()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mStateStep(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
	, mAttackTime(0.0f)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// �G�̎��
	mType = EEnemyType::eMushroom;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Mushroom");

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

	SetAnimationSpeed(0.3f);
	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	// �����R���C�_�[(��)
	mpColLineSide = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, ENEMY_SIDE)
	);
	mpColLineSide->SetCollisionLayers({ ELayer::eField });
	mpColLineSide->Position(0.0f, 10.0f, 0.0f);

	// �����R���C�_�[(�c)
	mpColLineHeight = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColLineHeight->SetCollisionLayers({ ELayer::eField });
	mpColLineHeight->Position(0.0f, 0.2f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.5f, false, 5.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.0f, -0.08f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.25f, false, 5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.045f, 0.2f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereRoot = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.25f, false, 5.0f
	);
	mpColliderSphereRoot->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereRoot->Position(0.065f, 0.18f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.55f, 0.0f),
		4.5f,false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, -4.2f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageColUmbrella = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.55f, false
	);
	mpDamageColUmbrella->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColUmbrella->SetCollisionTags({ ETag::eWeapon });
	mpDamageColUmbrella->Position(0.0f, -0.1f, 0.0f);
	
	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColRoot = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.25f, false
	);
	mpDamageColRoot->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColRoot->SetCollisionTags({ ETag::eWeapon });
	mpDamageColRoot->Position(0.05f, 0.23f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.55f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.0f, -0.1f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColRoot = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.25f, false
	);
	mpAttackColRoot->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColRoot->SetCollisionTags({ ETag::ePlayer });
	mpAttackColRoot->Position(0.05f, 0.23f, 0.0f);

	// �L�����N�^�[�����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���}�b�V�����[���̓��̍s��ɃA�^�b�`
	const CMatrix* headMtx = GetFrameMtx("Armature_mushroom_spine03");
	mpColliderSphereHead->SetAttachMtx(headMtx);
	mpDamageColUmbrella->SetAttachMtx(headMtx);
	mpAttackColHead->SetAttachMtx(headMtx);

	// �L�����N�^�[�����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���}�b�V�����[���̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMtx = GetFrameMtx("Armature_mushroom_spine02");
	mpColliderSphereBody->SetAttachMtx(bodyMtx);
	mpDamageColBody->SetAttachMtx(bodyMtx);

	// �L�����N�^�[�����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���}�b�V�����[���̍��̍s��ɃA�^�b�`
	const CMatrix* rootMtx = GetFrameMtx("Armature_mushroom_root");
	mpColliderSphereRoot->SetAttachMtx(rootMtx);
	mpDamageColRoot->SetAttachMtx(rootMtx);
	mpAttackColRoot->SetAttachMtx(rootMtx);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColHead->SetEnable(false);
	mpAttackColRoot->SetEnable(false);

	float Size = 17.0f;   // �T�C�Y
	float Height = 0.6f;  // ����
	// �q�b�g�G�t�F�N�g���쐬
	mpHitEffect = new CHit(Size, Height);
	mpHitEffect->SetOwner(this);
	mpHitEffect->Position(Position());
	mpHitEffect->SetShow(false);

	// �߂܂��G�t�F�N�g�쐬
	mpDizzyEffect = new CDizzyEffect
	(
		this, nullptr,
		CVector(0.0f, 37.0f, 0.0f)
	);

	mpGameUI->SetUIoffSetPos(CVector(0.0f, 30.0f, 0.0f));

	// Lv.��ݒ�
	mpGameUI->SetLv();
	// ���x����ݒ�
	int level = 6;
	std::to_string(level);
	mpGameUI->SetEnemyLevel(level);
	// ���O��ݒ�
	std::string name = "�U���̂�";
	mpGameUI->SetEnemyName(name);

	Scale(25.0f, 25.0f, 25.0f);
}

// �f�X�g���N�^
CMushroom::~CMushroom()
{
	// �L�����N�^�[�̐����R���C�_�[
	SAFE_DELETE(mpColLineSide);
	SAFE_DELETE(mpColLineHeight);

	// �L�����N�^�[�̉����߂��R���C�_�[
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereRoot);

	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColUmbrella);
	SAFE_DELETE(mpDamageColRoot);

	// �U���R���C�_�[
	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColRoot);
}

// �C���X�^���X
CMushroom* CMushroom::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CMushroom::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CMushroom::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// �키�O�̑ҋ@���
void CMushroom::UpdateIdle()
{
	SetAnimationSpeed(0.3f);
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
void CMushroom::UpdateIdle2()
{
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// �ҋ@���3
void CMushroom::UpdateIdle3()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >= 10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 30.0f && player->Position().Y() >= 0.7f)
	{
		ChangeState(EState::eIdle3);
	}
}

// �U��
void CMushroom::UpdateAttack()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eAttack);
	if (mAnimationFrame >= 13.0f)
	{
		AttackStart();
		// �U���I���҂���Ԃֈڍs
		ChangeState(EState::eAttackWait);
	}
}

// �U��2
void CMushroom::UpdateAttack2()
{
	SetAnimationSpeed(0.6f);

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0 : �U��2�A�j���[�V�����J�n�{�U���J�n(����)
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		if (mAnimationFrame >= 5.0f && mAnimationFrame <= 10.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// �X�e�b�v1 : �U���I��
	case 1:
		if (mAnimationFrame >= 11.0f && mAnimationFrame <= 12.0f)
		{
			AttackEnd();
			mStateStep++;
		}
		break;
		// �X�e�b�v2 : �U���J�n(����)
	case 2:
		if (mAnimationFrame >= 13.0f && mAnimationFrame <= 17.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// �X�e�b�v3 : �U���I��
	case 3:
		if (mAnimationFrame >= 18.0f && mAnimationFrame <= 20.0f)
		{
			AttackEnd();
			mStateStep++;
		}
		break;
		// �X�e�b�v4 : �U���J�n(�O���)
	case 4:
		if (mAnimationFrame >= 21.0f && mAnimationFrame < 24.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// �X�e�b�v5 : �U���I��
	case 5:
		if (mAnimationFrame >= 24.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}


// �U��3
void CMushroom::UpdateAttack3()
{
	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();
	SetAnimationSpeed(0.5f);
	mpAttackColHead->SetEnable(false);

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0 : �U��3�A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack3);
		mStateStep++;
		break;
		// �X�e�b�v1 : �U���J�n
	case 1:
		if (mAnimationFrame >= 10.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// �X�e�b�v2 : �U���̎��ɑO�Ɉړ����邩�~�܂邩
	case 2:
		if (mAnimationFrame >= 0.0f)
		{
			if (vectorPos <= 25.0f)
			{
				mMoveSpeed += newPos * MOVE_SPEED;
				mStateStep++;
			}
			else if (vectorPos >= 26.0f)
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				mStateStep++;
			}
		}
		break;
		// �X�e�b�v3 : �U���I���҂�
	case 3:
		if (mAnimationFrame >= 25.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U���I���҂�
void  CMushroom::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle3);
	}
}

// �q�b�g
void CMushroom::UpdateHit()
{
	mpDizzyEffect->Stop();

	SetAnimationSpeed(0.4f);
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
			ChangeState(EState::eIdle3);
		}
	}
}

// ����
void CMushroom::UpdateDie()
{
	mpDizzyEffect->Stop();

	SetAnimationSpeed(0.25f);

	switch (mStateStep)
	{
	// �X�e�b�v0 : �A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eDie);
		mpHitEffect->StartHitEffect();
		mStateStep++;
		break;
	// �X�e�b�v1 : �A�j���[�V�����I���҂�
	case 1:
		if (IsAnimationFinished())
		{
			Kill();
			// �G�l�~�[�̎��S����
			CEnemy::MushroomDeath();
		}
		break;
	}
}

// �߂܂�(����)
void CMushroom::UpdateDizzy()
{
	SetAnimationSpeed(0.5f);

	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eDizzy);
		if (!mpDizzyEffect->IsDizzy())
		{
			mpDizzyEffect->Start();
		}
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mpDizzyEffect->Stop();
			ChangeState(EState::eIdle3);
		}
		break;
	}
}

// �ړ�
void CMushroom::UpdateRun()
{
	SetAnimationSpeed(0.4f);
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
	if (vectorPos <= 30.0f && player->Position().Y() >= 0.7f)
	{
		ChangeState(EState::eIdle3);
	}
	// �U���p�̑ҋ@���[�V������
	else if (vectorPos <= STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		ChangeState(EState::eIdle3);
	}
}

// �X�V����
void CMushroom::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.Y(-GRAVITY);

	if (mState != EState::eAttack3)
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
		// �U��3
	case EState::eAttack3:
		UpdateAttack3();
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

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState == EState::eIdle3 || mState == EState::eRun || mState == EState::eHit)
	{
		mAttackTime += Time::DeltaTime();

		if (vectorPos <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}

		if (mAttackTime >= 2.0f)
		{
			// �U��2
			bool Attack2 = false;
			// �U��3
			bool Attack3 = false;
			// �m�����ŏ���3�ő�6
			int probability2 = Math::Rand(2, 5);
			int probability3 = Math::Rand(6, 10);
			if (probability2 == 2)Attack2 = true;
			if (probability3 == 6)Attack3 = true;
			if (Attack2)
			{
				ChangeState(EState::eAttack2);
			}
			else if (Attack3)
			{
				ChangeState(EState::eAttack3);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3
			|| mState == EState::eAttackWait)
		{
			mAttackTime = 0.0f;
		}
		if (vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0.0f;
		}
	}

	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE || mState == EState::eAttack3)
	{
		Position(Position() + mMoveSpeed);
	}

	if (player->Position().Y() >= 16.0f)
	{
		mpDamageColUmbrella->SetEnable(true);
	}
	if (player->Position().Y() >= 10.0f)
	{
		mpColliderSphereHead->SetEnable(true);
	}
	if (player->Position().Y() <= 0.5f)
	{
		mpDamageColUmbrella->SetEnable(false);
	}
	if (player->Position().Y() <= 5.0f)
	{
 		mpColliderSphereHead->SetEnable(false);
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();
	
	// �L�����̉����߂��R���C�_�[
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereRoot->Update();

	// �_���[�W���󂯂�R���C�_�[
	mpDamageColBody->Update();
	mpDamageColUmbrella->Update();
	mpDamageColRoot->Update();

	// �U���R���C�_�[
	mpAttackColHead->Update();
	mpAttackColRoot->Update();

	mIsGrounded = false;

	CEnemy::Update();

	if (mState == EState::eIdle || mState == EState::eIdle2)
	{
		CHpGauge* hpGauge = mpGameUI->GetHpGauge();
		hpGauge->SetShow(false);
		CLevelUI* Lv = mpGameUI->GetLv();
		Lv->SetShow(false);
		CEnemyLevelUI* Level = mpGameUI->GetLevel();
		Level->SetShow(false);
		CEnemyNameUI* Name = mpGameUI->GetName();
		Name->SetShow(false);
	}
}

// �Փˏ���
void CMushroom::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColHead || self == mpAttackColRoot&& mState != EState::eIdle && mState != EState::eIdle2 &&
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
				int damage = CalcDamage(1.0f,this, chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, this);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
	else if (self == mpColLineHeight || self == mpColLineHeight)
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
	else if (self == mpColliderSphereHead || self == mpColliderSphereBody || self == mpColliderSphereRoot)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CMushroom::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	if (mState == EState::eAttack || mState==EState::eAttack2)
	{
		mpAttackColHead->SetEnable(true);
	}
	if (mState == EState::eAttack3)
	{
		mpAttackColRoot->SetEnable(true);
	}
}

// �U���I��
void CMushroom::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackColHead->SetEnable(false);
	mpAttackColRoot->SetEnable(false);
}

// 1���x���A�b�v
void CMushroom::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CMushroom::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY2_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpGameUI->SetMaxHp(mCharaMaxStatus.hp);
	mpGameUI->SetHp(mCharaStatus.hp);
}

// ��_���[�W����
void CMushroom::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HP����_���[�W������
	if (mCharaStatus.hp -= damage)
	{
		ChangeState(EState::eHit);
		if (mState == EState::eHit)
		{
			mpHitEffect->StartHitEffect();
		}
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
void CMushroom::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}

// �����_���Ɉʒu���擾
CVector CMushroom::GetRandomSpawnPos()
{
	CVector pos = CVector::zero;
	pos.X(Math::Rand(50.0f, 150.0f));
	pos.Z(Math::Rand(690.0f, 790.0f));

	return CVector(pos);
}

// �`��
void CMushroom::Render()
{
	CXCharacter::Render();
}