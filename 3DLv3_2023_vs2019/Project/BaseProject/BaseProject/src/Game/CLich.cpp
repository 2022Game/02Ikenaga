#include "CLich.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CMagicCircle.h"
#include "CBee.h"
#include "CBeholder.h"
#include "CBoxer.h"
#include "CCactus.h"
#include "CChest.h"
#include "CMushroom.h"
#include "CRay.h"
#include "CSlime.h"
#include "CTurtle.h"
#include "CDrainEffect.h"
#include "CShieldRotate.h"
#include "CInput.h"

// ���b�`�̃C���X�^���X
CLich* CLich::spInstance = nullptr;

float CLich::mElapsedTime;

#define ENEMY_HEIGHT   -4.1f  // �c�̒���
#define MOVE_SPEED      0.3f  // �ړ����x
#define WALK_RANGE    300.0f  // �ǐՂ���͈�
#define STOP_RANGE     50.0f  // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f  // ��]����͈�
#define MC_ANIM_TIME    1.0f  // ���@�w�̃A�j���[�V��������
#define MC_ANIM_MOVE_Y 38.0f  // ���@�w�̍����ړ���
#define MC_ANIM_ROT_Y 360.0f  // ���@�w�̂�Y����]��

const CLich::SpawnData CLich::SPAWN_DATA[] =
{
	//  �G�̎�ށ@�@�@�@�@�@ ����   ���@�w�̃I�t�Z�b�g�ʒu    ���@�w�̐F               ���@�w�̑傫��  �G�̑傫��  ��������^�C�~���O
	{ EEnemyType::eBee,      50.0f, CVector(0.0f,-9.6f,0.0f), CColor(0.7f,0.0f,1.0f),  25.0f,          30.0f,      0.25f },
	{ EEnemyType::eBeholder, 50.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,1.0f),  25.0f,          18.0f,       0.5f },
	{ EEnemyType::eBoxer,    50.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,1.0f,0.0f),  35.0f,          25.0f,      0.25f },
	{ EEnemyType::eCactus,   50.0f, CVector(0.0f,-9.6f,0.0f), CColor(0.0f,1.0f,0.0f),  35.0f,          35.0f,      0.25f },
	{ EEnemyType::eChest,    50.0f, CVector(0.0f,-9.6f,0.0f), CColor(0.85f,0.6f,0.0f), 35.0f,          35.0f,      0.25f },
	{ EEnemyType::eMushroom, 50.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.85f,0.6f), 30.0f,          35.0f,      0.25f },
	{ EEnemyType::eRay,      50.0f, CVector(0.0f,-9.6f,0.0f), CColor(0.0f,1.0f,1.0f),  30.0f,          40.0f,      0.25f },
	{ EEnemyType::eSlime,    50.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,0.0f),  30.0f,          30.0f,      0.25f },
	{ EEnemyType::eTurtle,   50.0f, CVector(0.0f,-9.6f,0.0f), CColor(0.0f,0.0f,1.0f),  30.0f,          30.0f,      0.25f },
};

// ���b�`�̃A�j���[�V�����f�[�^�̃e�[�u��
const CLich::AnimData CLich::ANIM_DATA[] =
{
	{ "",								                  true,	   0.0f,  0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Lich\\animation\\LichIdle.x",	  true,   41.0f,  0.4f},  // �ҋ@ 41.0f
	{ "Character\\Enemy\\Lich\\animation\\LichAttack.x",  false,  41.0f,  0.4f},  // �U�� 41.0f
	{ "Character\\Enemy\\Lich\\animation\\LichAttack2.x", false,  71.0f,  0.5f},  // �U�� 71.0f
	{ "Character\\Enemy\\Lich\\animation\\LichGetHit.x",  false,  41.0f,  0.4f},  // �q�b�g 41.0f
	{ "Character\\Enemy\\Lich\\animation\\LichDie.x",	  false,  29.0f,  0.3f},  // ���� 29.0f
	{ "Character\\Enemy\\Lich\\animation\\LichVictory.x", false,  81.0f,  0.4f},  // ���� 81.0f
	{ "Character\\Enemy\\Lich\\animation\\LichRun.x",	  true,	  21.0f,  0.5f},  // ���� 21.0f
};

// �R���X�g���N�^
CLich::CLich()
	: mState(EState::eIdle)
	, mAttackTime(0)
	, mStateStep(0)
	, mpRideObject(nullptr)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
	, mpSpawnEnemy(nullptr)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Lich");

	//�ŏ���1���x���ɐݒ�
	ChangeLevel(1);

	mElapsedTime = 0.0f;

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

	SetAnimationSpeed(0.4f);
	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[�̉����߂��R���C�_�[���쐬(��)
	mpColCapsuleBody = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, 1.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f),
		13.0f, false, 20.0f
	);
	mpColCapsuleBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColCapsuleBody->Position(0.0f, -5.5f, -1.0f);

	// �L�����N�^�[�̉����߂��R���C�_�[���쐬(���r)
	mpColSphereArmL = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.35f, false, 20.0f
	);
	mpColSphereArmL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColSphereArmL->Position(0.0f, -0.15f, 0.0f);

	// �L�����N�^�[�̉����߂��R���C�_�[���쐬(�E�r)
	mpColSphereArmR = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.35f, false, 20.0f
	);
	mpColSphereArmR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColSphereArmR->Position(0.0f, -0.15f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(0.0f, 1.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f),
		13.0f, false
	);
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpDamageColBody->Position(0.0f, -5.5f, -1.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(���r)
	mpDamageColArmL = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f, false
	);
	mpDamageColArmL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColArmL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColArmL->Position(0.0f, -0.15f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�E�r)
	mpDamageColArmR = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f, false
	);
	mpDamageColArmR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColArmR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColArmR->Position(0.0f, -0.15f, 0.0f);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�����b�`�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Spine");
	mpColCapsuleBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�����b�`�̍��r�̍s��ɃA�^�b�`
	const CMatrix* armLeftMty = GetFrameMtx("Armature_Hand_L");
	mpColSphereArmL->SetAttachMtx(armLeftMty);
	mpDamageColArmL->SetAttachMtx(armLeftMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�����b�`�̉E�r�̍s��ɃA�^�b�`
	const CMatrix* armRightMty = GetFrameMtx("Armature_Hand_R");
	mpColSphereArmR->SetAttachMtx(armRightMty);
	mpDamageColArmR->SetAttachMtx(armRightMty);

	CVector forward = VectorZ();
	forward.Y(0.0f);
	forward.Normalize();
	CVector DrainPos = Position() + forward * 55.0f + CVector(0.0f, 15.0f, 0.0f);
	// �h���C��
	mpDrain = new CDrainEffect
	(
		this, nullptr,
		DrainPos,
		CQuaternion(0.0, 0.f, 0.0f).Matrix()

	);

	// �V�[���h�Ƃ̋���
	float ShieldDist = 10.0f;
	// ��]����V�[���h
	mpShieldRotate = new CShieldRotate(0.0f, ShieldDist);
	mpShieldRotate->SetOwner(this);

	// ��]����V�[���h2
	mpShieldRotate2 = new CShieldRotate(180.0, ShieldDist);
	mpShieldRotate2->SetOwner(this);

	// ��]����V�[���h3
	mpShieldRotate3 = new CShieldRotate(-270.0f, ShieldDist);
	mpShieldRotate3->SetOwner(this);

	// ��]����V�[���h4
	mpShieldRotate4 = new CShieldRotate(270.0f, ShieldDist);
	mpShieldRotate4->SetOwner(this);
}

// �f�X�g���N�^
CLich::~CLich()
{
	// �����R���C�_�[
	SAFE_DELETE(mpColliderLine);
	// �L�����N�^�[�̉����߂��R���C�_�[
	SAFE_DELETE(mpColCapsuleBody);
	SAFE_DELETE(mpColSphereArmL);
	SAFE_DELETE(mpColSphereArmR);
	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColArmL);
	SAFE_DELETE(mpDamageColArmR);

	// ���g�����������G�����݂����
	if (mpSpawnEnemy)
	{
		mpSpawnEnemy->SetSummoner(nullptr);
	}
}

// �C���X�^���X
CLich* CLich::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CLich::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CLich::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

CEnemy* CLich::SpawnEnemy(EEnemyType type) const
{
	CEnemy* ret = nullptr;

	switch (type)
	{
	case EEnemyType::eBee:
		ret = new CBee();
		break;
	case EEnemyType::eBeholder:
		ret = new CBeholder();
		break;
	case EEnemyType::eBoxer:
		ret = new CBoxer();
		break;
	case EEnemyType::eCactus:
		ret = new CCactus();
		break;
	case EEnemyType::eChest:
		ret = new CChest();
		break;
	case EEnemyType::eMushroom:
		ret = new CMushroom();
		break;
	case EEnemyType::eRay:
		ret = new CRay();
		break;
	case EEnemyType::eSlime:
		ret = new CSlime();
		break;
	case EEnemyType::eTurtle:
		ret = new CTurtle();
		break;
	}
	return ret;
}

// �ҋ@���
void CLich::UpdateIdle()
{
	SetAnimationSpeed(0.4f);
	// �ҋ@�A�j���[�V�����ɐ؂�ւ�
	ChangeAnimation(EAnimType::eIdle);

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= 70.0f)
	{
		ChangeState(EState::eIdle2);
	}
	else if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// �ҋ@���2
void CLich::UpdateIdle2()
{
	SetAnimationSpeed(0.4f);
	// �ҋ@�A�j���[�V�����ɐ؂�ւ�
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >= 10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE
		&& player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 30.0f)
	{
		ChangeState(EState::eIdle2);
	}
}

// �U��
void CLich::UpdateAttack()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eAttack);

	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			if (!mpDrain->IsThrowing())
			{
				mpDrain->Start();
				mStateStep++;
			}
		}
		break;
	case 2:
		if (mAnimationFrame >= 39.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U��2
void CLich::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack2);
	if (mAnimationFrame >= 71.0f)
	{
		ChangeState(EState::eAttackWait);
	}
}

// �U���I���҂�
void CLich::UpdateAttackWait()
{
	mpDrain->Stop();
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// �q�b�g
void CLich::UpdateHit()
{
	mpDrain->Stop();
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// ����
void CLich::UpdateDie()
{
	mpDrain->Stop();
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::BoxerDeath();
	}
}

// ����
void CLich::UpdateSummon()
{
	SetAnimationSpeed(0.4f);
	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eSummon);
		mStateStep++;
		break;
	// �����_�������J�n
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			if (mpSpawnEnemy == nullptr)
			{
				mRandomSummonIndex = GetRandomSummonIndex();
				if (mRandomSummonIndex < 0)
				{
					mStateStep = 4;
				}
				else
				{
					SpawnData data = SPAWN_DATA[mRandomSummonIndex];

					CVector forward = VectorZ();
					forward.Y(0.0f);
					forward.Normalize();
					CVector magicPos = Position() + forward * data.dist + data.offsetPos;
					mpMagicCircle = new CMagicCircle
					(
						this,
						magicPos
					);
					mpMagicCircle->SetColor(data.circleColor);
					mpMagicCircle->Scale(CVector::one * data.circleScale);
					
					mMCStartPos = mpMagicCircle->Position();
					mMCStartRot = mpMagicCircle->Rotation();

					mStateStep++;
					mElapsedTime = 0.0f;
				}
			}
			else
			{
				mStateStep = 4;
			}
		}
		break;
		// ���@�w�̃A�j���[�V����
	case 2:
		if (mElapsedTime < MC_ANIM_TIME)
		{
			// ���Ԃ̌o�߂ɍ��킹�Ĉʒu�̉�]�l��ύX
			float percent = mElapsedTime / MC_ANIM_TIME;
			
			SpawnData data = SPAWN_DATA[mRandomSummonIndex];
			if (mpSpawnEnemy == nullptr && percent >= data.spawnRatio)
			{
				RandomSummon();
			}

			// �ʒu
			CVector targetPos = mMCStartPos + CVector(0.0f, MC_ANIM_MOVE_Y, 0.0f);
			CVector pos = CVector::Lerp(mMCStartPos, targetPos, percent);
			mpMagicCircle->Position(pos);

			// ��]�l
			CVector startAngle = CVector(0.0f, 0.0f, 0.0f);
			CVector targetAngle = CVector(0.0f, MC_ANIM_ROT_Y, 0.0f);
			CQuaternion targetRot = mMCStartRot * CQuaternion(0.0f, MC_ANIM_ROT_Y, 0.0f);
			CVector angle = CVector::Lerp(startAngle, targetAngle, percent);
			CQuaternion rot = mMCStartRot * CQuaternion(angle);
			mpMagicCircle->Rotation(rot);

			mElapsedTime += Time::DeltaTime();
		}
		// �A�j���[�V�����I��
		else
		{
			if (mpSpawnEnemy == nullptr)
			{
				RandomSummon();
			}

			// �I�����̈ʒu�Ɖ�]�l��ݒ�
			CVector targetPos = mMCStartPos + CVector(0.0f, MC_ANIM_MOVE_Y, 0.0f);
			mpMagicCircle->Position(targetPos);
			CVector targetAngle = CVector(0.0f, MC_ANIM_ROT_Y, 0.0f);
			CQuaternion rot = mMCStartRot * CQuaternion(targetAngle);
			mpMagicCircle->Rotation(rot);

			mStateStep++;
			mElapsedTime = 0.0f;
		}
		break;
	// ���@�w�̑҂�����
	case 3:
		if (mElapsedTime < 1.0f)
		{
			mElapsedTime += Time::DeltaTime();
		}
		else
		{
			mpMagicCircle->Kill();
			mpMagicCircle = nullptr;

			mStateStep++;
			mElapsedTime = 0.0f;
		}
		break;
	case 4:
		if (mAnimationFrame >= 81.0f)
		{
			ChangeState(EState::eIdle2);
		}
		break;
	}
}

// �ړ�
void CLich::UpdateRun()
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

	if (vectorPos <= 30.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
	// �ǐՂ��~�܂������A�ҋ@���[�V������
	if (vectorPos <= STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// �����_����������G�̃C���f�b�N�X�l���擾
int CLich::GetRandomSummonIndex() const
{
	int tableSize = ARRAY_SIZE(SPAWN_DATA);
	int index = Math::Rand(0, tableSize - 1);
	
	if (!(0 <= index && index < tableSize)) return -1;
	return index;
}

// �����_������
void CLich::RandomSummon()
{
	SpawnData data = SPAWN_DATA[mRandomSummonIndex];

	CVector forward = VectorZ();
	forward.Y(0.0f);
	forward.Normalize();
	CVector magicPos = Position() + forward * data.dist + data.offsetPos;

	// ��������G�𐶐�
	mpSpawnEnemy = SpawnEnemy(data.type);
	if (mpSpawnEnemy != nullptr)
	{
		mpSpawnEnemy->Position(magicPos);
		mpSpawnEnemy->Scale(CVector::one * data.monsterScale);
		mpSpawnEnemy->Rotation(CQuaternion::LookRotation(forward));
		mpSpawnEnemy->SetSummoner(this);
	}
}

// HP�z���܂��͉�
void CLich::DrainHp()
{
	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		int Heal = 0;
		Heal = mCharaMaxStatus.hp * 0.25;
		mCharaStatus.hp += Heal;
		if (mCharaStatus.hp > mCharaMaxStatus.hp)
		{
			mCharaStatus.hp = mCharaMaxStatus.hp;
		}
	}
}
	
//�X�V����
void CLich::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
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
		// ����
	case EState::eSummon:
		UpdateSummon();
		break;
		// �ړ�
	case EState::eRun:
		UpdateRun();
		break;
	}

	// HP�Q�[�W�̍��W���X�V(�G�̍��W�̏�����̍��W)
	CVector gaugePos = Position() + CVector(0.0f, 40.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState != EState::eIdle && mState != EState::eDie)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eHit)
	{
		if (vectorPos <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}

		mAttackTime++;
		if (mAttackTime >= 250 && mpSpawnEnemy == nullptr)
		{
			ChangeState(EState::eSummon);
			//ChangeState(EState::eAttack2);
		}
		else if(mAttackTime >= 300 && mpSpawnEnemy != nullptr)
		{
			bool Attack2 = false;
			int random = Math::Rand(0, 2);
			if (random == 2)Attack2 = true;
			if (Attack2)
			{
				ChangeState(EState::eAttack2);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}

		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eSummon)
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

	if (mpSpawnEnemy != nullptr)
	{
		mDefenseUp = true;
	}
	else
	{
		mDefenseUp = false;
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �L�����N�^�[�̉����߂��R���C�_�[
	mpColCapsuleBody->Update();
	mpColSphereArmL->Update();
	mpColSphereArmR->Update();

	// �_���[�W���󂯂�R���C�_�[
	mpDamageColBody->Update();
	mpDamageColArmL->Update();
	mpDamageColArmR->Update();

	if (CInput::PushKey('Z'))
	{
		ChangeState(EState::eSummon);
		int Heal = 0;
		Heal = mCharaMaxStatus.hp * 0.25;
		mCharaStatus.hp -= Heal;
	}

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
	CDebugPrint::Print("UP %d\n", mDefenseUp);
}

// �Փˏ���
void CLich::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// �L�����N�^�[���m�̏Փˏ���
	else if (self == mpColCapsuleBody || self == mpColSphereArmL || self == mpColSphereArmR)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// ��_���[�W����
void CLich::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HP����_���[�W������
	if (mCharaStatus.hp -= damage)
	{
		if (mState != EState::eSummon)
		{
			ChangeState(EState::eHit);
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
		Position(Position() - dir * Scale().X() * 0.1f);
	}
}

// ���������G�̎��S����
void CLich::DeathSummonEnemy(CEnemy* enemy)
{
	// ���������������G�łȂ���΁A�������Ȃ�
	if (enemy != mpSpawnEnemy) return;

	// ���������������G�ł���΁A�|�C���^�[����ɂ���
	mpSpawnEnemy = nullptr;
}

// ���S����
void CLich::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}

// �h��͂̋����������擾
float CLich::GetDefBuff(const CVector& attackDir)const
{
	if (mpSpawnEnemy !=nullptr ) return 20.0f;

	// �ʏ펞�̖h��̊���
	return mBaseDefenseBuffRatio;
}

// �`��
void CLich::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CLich::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CLich::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY10_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}