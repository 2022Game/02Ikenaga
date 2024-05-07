#include "CMushroom.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"

// �}�b�V�����[���̃C���X�^���X
CMushroom* CMushroom::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 40.0f       // �͈͓�
#define MOVE_SPEED 0.07f         // �ړ����x
#define GRAVITY 0.0625f          // �d��

#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 20.0f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f     // ��]����͈�

// �}�b�V�����[���̃A�j���[�V�����f�[�^�̃e�[�u��
const CMushroom::AnimData CMushroom::ANIM_DATA[] =
{
	{ "",										                            true,	0.0f,	0.0f},// T�|�[�Y
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlant.x",	        true,	21.0f,	0.3f},	// �A�� 21.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlantToBattle.x",	true,	21.0f,	0.3},	// �A�����炫�̂� 21.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle2.x",	    true,	18.0f,	0.5f},	// �ҋ@2 18.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle.x",	    true,	18.0f,	0.5f},	// �ҋ@ 18.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack.x",	        false,	26.0f,	0.4f},	// �U�� 26.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack2.x",	        false,	26.0f,	0.6f},	// �U�� 26.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack3.x",	        false,	26.0f,	0.5f},	// �U�� 26.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomGetHit.x",	        true,	23.0f,	0.4f},	// �q�b�g 23.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomDie.x",	            true,	26.0f,	0.25f},	//  ����26.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomDizzy.x",	            true,	41.0f,  0.4f},	// �߂܂� 41.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomRun.x",	            true,	17.0f, 	0.4f},	//���� 17.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomSenseSomethingMaintain.x",	true,	121.0f	},	//���� 121.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomSenseSomethingStart.x",	true,	25.0f	},	//�J�n�̌��� 25.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomTaunting.x",	true,	80.0f	},	//���� 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomVictory.x",	true,	61.0f	},	//���� 61.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk.x",	true,	31.0f	},	//���� 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk2.x",	true,	31.0f	},	//����2 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk3.x",	true,	31.0f	},	//����3 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk4.x",	true,	31.0f	},	//����4 31.0f
};

int CMushroom::mHp;

// �R���X�g���N�^
CMushroom::CMushroom()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mStateAttack2Step(0)
	, mStateAttack3Step(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

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
	const CMatrix* headMty = GetFrameMtx("Armature_mushroom_spine03");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpDamageColUmbrella->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// �L�����N�^�[�����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���}�b�V�����[���̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_mushroom_spine02");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);

	// �L�����N�^�[�����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���}�b�V�����[���̍��̍s��ɃA�^�b�`
	const CMatrix* rootMty = GetFrameMtx("Armature_mushroom_root");
	mpColliderSphereRoot->SetAttachMtx(rootMty);
	mpDamageColRoot->SetAttachMtx(rootMty);
	mpAttackColRoot->SetAttachMtx(rootMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColHead->SetEnable(false);
	mpAttackColRoot->SetEnable(false);
}

// �f�X�g���N�^
CMushroom::~CMushroom()
{
	// �L�����N�^�[�̐����R���C�_�[
	SAFE_DELETE(mpColliderLine);

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
	mStateAttack2Step = 0;
	mStateAttack3Step = 0;
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
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
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
	switch (mStateAttack2Step)
	{
		// �X�e�b�v0 : �U��2�A�j���[�V�����J�n�{�U���J�n(����)
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		if (mAnimationFrame >= 5.0f && mAnimationFrame <= 10.0f)
		{
			AttackStart();
			mStateAttack2Step++;
		}
		break;
		// �X�e�b�v1 : �U���I��
	case 1:
		if (mAnimationFrame >= 11.0f && mAnimationFrame <= 12.0f)
		{
			AttackEnd();
			mStateAttack2Step++;
		}
		break;
		// �X�e�b�v2 : �U���J�n(����)
	case 2:
		if (mAnimationFrame >= 13.0f && mAnimationFrame <= 17.0f)
		{
			AttackStart();
			mStateAttack2Step++;
		}
		break;
		// �X�e�b�v3 : �U���I��
	case 3:
		if (mAnimationFrame >= 18.0f && mAnimationFrame <= 20.0f)
		{
			AttackEnd();
			mStateAttack2Step++;
		}
		break;
		// �X�e�b�v4 : �U���J�n(�O���)
	case 4:
		if (mAnimationFrame >= 21.0f && mAnimationFrame < 25.0f)
		{
			AttackStart();
			mStateAttack2Step++;
		}
		break;
		// �X�e�b�v5 : �U���I��
	case 5:
		if (mAnimationFrame >= 25.0f)
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
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();
	SetAnimationSpeed(0.5f);
	mpAttackColHead->SetEnable(false);

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateAttack3Step)
	{
		// �X�e�b�v0 : �U��3�A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack3);
		mStateAttack3Step++;
		break;
		// �X�e�b�v1 : �U���J�n
	case 1:
		if (mAnimationFrame >= 10.0f)
		{
			AttackStart();
			mStateAttack3Step++;
		}
		break;
		// �X�e�b�v2 : �U���̎��ɑO�Ɉړ����邩�~�܂邩
	case 2:
		if (mAnimationFrame >= 0.0f)
		{
			if (vectorPos <= 25.0f)
			{
				mMoveSpeed += nowPos * 7.0f;
				mStateAttack3Step++;
			}
			else if (vectorPos >= 26.0f)
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				mStateAttack3Step++;
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
			ChangeState(EState::eIdle3);
		}
	}
}

// ����
void CMushroom::UpdateDie()
{
	SetAnimationSpeed(0.25f);
	// ���ʎ��̃A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::MushroomDeath();
	}
}

// �߂܂�(����)
void CMushroom::UpdateDizzy()
{
	SetAnimationSpeed(0.4f);
	// �߂܂�(����)�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle3);
	}
}

// ����
void CMushroom::UpdateRun()
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
	mHp = mCharaStatus.hp;
	mMoveSpeed.Y(0.0f);

	if (mState != EState::eRun && mState != EState::eAttack3)
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
		// ���s
	case EState::eRun:
		UpdateRun();
		break;
	}

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState !=EState::eIdle && mState != EState::eIdle2 && mState != EState::eDie)
	{
		// HP�Q�[�W�̍��W���X�V(�G�̍��W�̏�����̍��W)
		CVector gaugePos = Position() + CVector(0.0f, 30.0f, 0.0f);

		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle3 || mState == EState::eRun || mState == EState::eHit)
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
			mAttackTime = 0;
		}
		if (vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
	}

	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE || mState == EState::eAttack3)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
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

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
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

// �`��
void CMushroom::Render()
{
	CXCharacter::Render();
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

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CMushroom::TakeDamage(int damage, CObjectBase* causedObj)
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
void CMushroom::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}