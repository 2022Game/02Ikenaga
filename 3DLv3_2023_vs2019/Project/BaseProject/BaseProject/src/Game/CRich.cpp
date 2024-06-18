#include "CRich.h"
#include "CCollisionManager.h"
#include "CCane.h"
#include "CPlayer.h"
#include "Maths.h"
#include "CMagicCircle.h"
#include "CSlime.h"
#include "CBeholder.h"
#include "CInput.h"

// ���b�`�̃C���X�^���X
CRich* CRich::spInstance = nullptr;

float CRich::mElapsedTime;

#define ENEMY_HEIGHT    2.0f
#define ROTATE_RANGE  250.0f  // ��]����͈�

// ���b�`�̃A�j���[�V�����f�[�^�̃e�[�u��
const CRich::AnimData CRich::ANIM_DATA[] =
{
	{ "",								                  true,	   0.0f,  0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Rich\\animation\\RichIdle.x",	  true,   41.0f,  0.4f},  // �ҋ@ 41.0f
	{ "Character\\Enemy\\Rich\\animation\\RichAttack.x",  false,  41.0f,  0.4f},  // �U�� 41.0f
	{ "Character\\Enemy\\Rich\\animation\\RichAttack2.x", false,  71.0f,  0.5f},  // �U�� 71.0f
	{ "Character\\Enemy\\Rich\\animation\\RichGetHit.x",  false,  41.0f,  0.4f},  // �q�b�g 41.0f
	{ "Character\\Enemy\\Rich\\animation\\RichDie.x",	  false,  29.0f,  0.5f},  // ���� 29.0f
	{ "Character\\Enemy\\Rich\\animation\\RichVictory.x", false,  81.0f,  0.4f},  // ���� 81.0f
	{ "Character\\Enemy\\Rich\\animation\\RichRun.x",	  true,	  21.0f,  0.5f},  // ���� 21.0f
};

// �R���X�g���N�^
CRich::CRich()
	: mState(EState::eIdle)
	, mAttackTime(0)
	, mStateStep(0)
	, mpRideObject(nullptr)
	, mIsGrounded(false)
	, mIsSummoning(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Rich");

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
CRich::~CRich()
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
}

// �C���X�^���X
CRich* CRich::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CRich::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CRich::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// �ҋ@���
void CRich::UpdateIdle()
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
void CRich::UpdateIdle2()
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
void CRich::UpdateAttack()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eAttack);

	if (mAnimationFrame >= 41.0f)
	{
		ChangeState(EState::eAttackWait);
	}
}

// �U��2
void CRich::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack2);
	if (mAnimationFrame >= 71.0f)
	{
		ChangeState(EState::eAttackWait);
	}
}

// �U���I���҂�
void CRich::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// �q�b�g
void CRich::UpdateHit()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// ����
void CRich::UpdateDie()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDie);
}

// ����
void CRich::UpdateSummon()
{
	SetAnimationSpeed(0.4f);
	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eSummon);
		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			if (mIsSummoning == false)
			{
				RandomSummon();
				mStateStep++;
			}
			else
			{
				mStateStep++;
			}
		}
		break;
	case 2:
		if (mAnimationFrame >= 81.0f)
		{
			ChangeState(EState::eIdle2);
		}
		break;
	}
}

// �ړ�
void CRich::UpdateRun()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eRun);
}

// �����_������
void CRich::RandomSummon()
{

	bool Beholder = false;
	int Random = Math::Rand(0, 2);

	if (Random == 2) Beholder = true;
	if (Beholder)
	{
		mpMagicCircle = new CMagicCircle
		(
			this,
			Position() + CVector(0.0f, -9.6f, 40.0f)
		);
		mpMagicCircle->SetColor(CColor(1.0f, 0.0f, 1.0f));
		mpMagicCircle->Scale(15.0f, 15.0f, 15.0f);

		CVector magicPos = mpMagicCircle->Position();

		// ���̂̃����X�^�[
		CBeholder* enemy22 = new CBeholder();
		enemy22->Position(magicPos);
		enemy22->Scale(15.0f, 15.0f, 15.0f);
	}
	else
	{
		mpMagicCircle = new CMagicCircle
		(
			this,
			Position() + CVector(0.0f, -9.6f, 50.0f)
		);
		mpMagicCircle->SetColor(CColor::red);
		mpMagicCircle->Scale(30.0f, 30.0f, 30.0f);

		CVector magicPos = mpMagicCircle->Position() + CVector(0.0f, 0.0f, 0.0f);
		// ���b�h�X���C��
		mpSlime = new CSlime();
		mpSlime->Position(magicPos);
		mpSlime->Scale(25.0f, 25.0f, 25.0f);
	}
}

//�X�V����
void CRich::Update()
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

	if (mpSlime->mHp <= 0)
	{
		mIsSummoning = false;
	}
	if (mpSlime->mHp > 0)
	{
		mIsSummoning = true;
	}

	mIsGrounded = false;
	CDebugPrint::Print("false %f\n",mElapsedTime);
}

// �Փˏ���
void CRich::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CRich::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CRich::ChangeLevel(int level)
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
void CRich::TakeDamage(int damage, CObjectBase* causedObj)
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

// �`��
void CRich::Render()
{
	CXCharacter::Render();
}