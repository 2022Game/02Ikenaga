#include "CLich.h"
#include "CCollisionManager.h"
#include "CCane.h"
#include "CPlayer.h"
#include "Maths.h"
#include "CMagicCircle.h"
#include "CSlime.h"
#include "CBeholder.h"
#include "CInput.h"

// ���b�`�̃C���X�^���X
CLich* CLich::spInstance = nullptr;

float CLich::mElapsedTime;

#define ENEMY_HEIGHT    2.0f
#define ROTATE_RANGE  250.0f  // ��]����͈�
#define MC_ANIM_TIME    1.0f  // ���@�w�̃A�j���[�V��������
#define MC_ANIM_MOVE_Y 30.0f  // ���@�w�̍����ړ���
#define MC_ANIM_ROT_Y 360.0f  // ���@�w�̂�Y����]��

const CLich::SpawnData CLich::SPAWN_DATA[] =
{
	//  �G�̎�ށ@�@�@�@�@�@ ����   ���@�w�̃I�t�Z�b�g�ʒu    ���@�w�̐F              ���@�w�̑傫��  �G�̑傫��
	//{ EEnemyType::eBee,      40.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,1.0f), 15.0f, 15.0f },
	{ EEnemyType::eBeholder, 40.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,1.0f), 15.0f,          15.0f,     0.5f },
	/*{ EEnemyType::eBoxer,    40.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,1.0f), 15.0f, 15.0f },
	{ EEnemyType::eCactus,   40.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,1.0f), 15.0f, 15.0f },
	{ EEnemyType::eChest,    40.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,1.0f), 15.0f, 15.0f },
	{ EEnemyType::eMushroom, 40.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,1.0f), 15.0f, 15.0f },
	{ EEnemyType::eRay,      40.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,1.0f), 15.0f, 15.0f },*/
	{ EEnemyType::eSlime,    50.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,0.0f), 30.0f,           25.0f,    0.25f },
	//{ EEnemyType::eTurtle,   40.0f, CVector(0.0f,-9.6f,0.0f), CColor(1.0f,0.0f,1.0f), 15.0f, 15.0f },
};

// ���b�`�̃A�j���[�V�����f�[�^�̃e�[�u��
const CLich::AnimData CLich::ANIM_DATA[] =
{
	{ "",								                  true,	   0.0f,  0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Lich\\animation\\LichIdle.x",	  true,   41.0f,  0.4f},  // �ҋ@ 41.0f
	{ "Character\\Enemy\\Lich\\animation\\LichAttack.x",  false,  41.0f,  0.4f},  // �U�� 41.0f
	{ "Character\\Enemy\\Lich\\animation\\LichAttack2.x", false,  71.0f,  0.5f},  // �U�� 71.0f
	{ "Character\\Enemy\\Lich\\animation\\LichGetHit.x",  false,  41.0f,  0.4f},  // �q�b�g 41.0f
	{ "Character\\Enemy\\Lich\\animation\\LichDie.x",	  false,  29.0f,  0.5f},  // ���� 29.0f
	{ "Character\\Enemy\\Lich\\animation\\LichVictory.x", false,  81.0f,  0.4f},  // ���� 81.0f
	{ "Character\\Enemy\\Lich\\animation\\LichRun.x",	  true,	  21.0f,  0.5f},  // ���� 21.0f
};

// �R���X�g���N�^
CLich::CLich()
	: mState(EState::eIdle)
	, mAttackTime(0)
	, mStateStep(0)
	, mpRideObject(nullptr)
	, mIsGrounded(false)
	, mpSpawnEnemy(nullptr)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Lich");

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
		10.0f, false, 20.0f
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
		10.0f, false
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
		break;
	case EEnemyType::eBeholder:
		ret = new CBeholder();
		break;
	case EEnemyType::eBoxer:
		break;
	case EEnemyType::eCactus:
		break;
	case EEnemyType::eChest:
		break;
	case EEnemyType::eMushroom:
		break;
	case EEnemyType::eRay:
		break;
	case EEnemyType::eSlime:
		ret = new CSlime();
		break;
	case EEnemyType::eTurtle:
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
	if (vectorPos <= 50.0f)
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
}

// �U��
void CLich::UpdateAttack()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eAttack);

	if (mAnimationFrame >= 41.0f)
	{
		ChangeState(EState::eAttackWait);
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
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// �q�b�g
void CLich::UpdateHit()
{
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
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDie);
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
	
//�X�V����
void CLich::Update()
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

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState == EState::eIdle2)
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
		if (mAttackTime >= 300)
		{
			ChangeState(EState::eAttack);
			//ChangeState(EState::eAttack2);
		}
		if (mState == EState::eAttack || mState == EState::eAttack2)
		{
			mAttackTime = 0;
		}
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
	}

	mIsGrounded = false;
	CDebugPrint::Print("false %f\n",mElapsedTime);
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

	//mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	//mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CLich::TakeDamage(int damage, CObjectBase* causedObj)
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
		//Death();
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

// �`��
void CLich::Render()
{
	CXCharacter::Render();
}