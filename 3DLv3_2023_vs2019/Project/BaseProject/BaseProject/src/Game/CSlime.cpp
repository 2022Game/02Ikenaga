#include "CSlime.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CPlayer.h"
#include "CCamera.h"

// ���b�h�X���C��(�G�l�~�[)�̃C���X�^���X
CSlime* CSlime::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define MOVE_SPEED 0.07f         // �ړ����x
#define GRAVITY 0.0625f          // �d��

#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 21.0f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f     // ��]����͈�

// ���b�h�X���C��(�G�l�~�[)�̃A�j���[�V�����f�[�^�̃e�[�u��
const CSlime::AnimData CSlime::ANIM_DATA[] =
{
	{ "",										                         true,	0.0f,	0.0f},	// T�|�[�Y
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleNormal.x",	         true,	51.0f,	0.7f},  // �A�C�h���ʏ� 51.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingStart.x",	 true,	63.0f,  0.5f},  // �J�n�̌��� 63.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingRoutine.x",true,	71.0f,  0.5f},  // ���� 71.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleBattle.x",	         true,	25.0f,	0.8f},  // �A�C�h���o�g�� 25.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack.x",	             false,	26.0f,	0.5f},  // �U�� 26.0ff
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack2.x",	             false,	26.0f,	0.5f},  // �U��2 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeGetHit.x",	             true,	26.0f,	0.5f},  // �q�b�g 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeDie.x",	                 true,	41.0f,	0.5f},  // ���� 41.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeDizzy.x",	             true,	41.0f,	0.5f},  // �߂܂� 41.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeRun.x",	                 true,	21.0f,	0.5f},  // �ړ� 21.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeWalk.x",	             true,	31.0f,	0.5f},  // ���� 31.0f
//	{ "Character\\Enemy\\Slime\\animation\\SlimeWalkRight.x",	true,	31.0f	},  // �E�Ɉړ�
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkLeft.x",	true,	31.0f	},  // ���Ɉړ�
	//{ "Character\\Enemy\\Slime\\animation\\SlimeTaunt.x",	true,	21.0f	},  // ����
	//{ "Character\\Enemy\\Slime\\animation\\SlimeVictory.x",	true,	81.0f	},  // ����
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkBack.x",	true,	31.0f	},  // ���ɕ���
};

bool CSlime::IsDeath() const
{
	return mCharaStatus.hp <= 0;
}

int CSlime::mHp;

// �R���X�g���N�^
CSlime::CSlime()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mIsGrounded(false)
	, mStateAttackStep(0)
	, mStateAttack2Step(0)
	, mStateDizzyStep(0)
	, mStateHitStep(0)
	, mStateDieStep(0)
	, mIsSlimeRunSE(false)
	, mIsSlimeAttackSE(false)
	, mIsSlimeDizzySE(false)
	, mIsSlimeHitSE(false)
	, mIsSlimeDieSE(false)
	, mMoveSpeed(CVector::zero)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Slime");

	// �ŏ���1���x���ɐݒ�
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

	SetAnimationSpeed(0.7f);
	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	// �L�����N�^�[�̐����R���C�_�[
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this,ELayer::eEnemy,
		0.5f,false,5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.06f, 0.3f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.5f,false
	);
	//�@�_���[�W���󂯂�R���C�_�[��
	//�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon,ETag::eSlash });
	//�_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageColBody->Position(0.1f, 0.3f,0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.5f,false
	);
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });
	mpAttackColBody->Position(0.1f, 0.3f, 0.0f);
	
	// �L�����N�^�[�����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���X���C���̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);
	mpAttackColBody->SetAttachMtx(bodyMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColBody->SetEnable(false);

	mpSlimeRunSE = CResourceManager::Get<CSound>("SlimeRun");
	mpSlimeAttackSE = CResourceManager::Get<CSound>("SlimeAttack");
	mpSlimeDizzySE = CResourceManager::Get<CSound>("SlimeDizzy");
	mpSlimeHitSE = CResourceManager::Get<CSound>("SlimeHit");
	mpSlimeDieSE = CResourceManager::Get<CSound>("SlimeDie");
}

// �f�X�g���N�^
CSlime::~CSlime()
{
	// �L�����N�^�[�̐����R���C�_�[
	SAFE_DELETE(mpColliderLine);
	// �L�����N�^�[�̉����߂��R���C�_�[���폜
	SAFE_DELETE(mpColliderSphereBody);
	// �_���[�W���󂯂�R���C�_�[���폜
	SAFE_DELETE(mpDamageColBody);
	// �U���R���C�_�[���폜
	SAFE_DELETE(mpAttackColBody);
}

// �X���C���̃C���X�^���X
CSlime* CSlime::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CSlime::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CSlime::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
	mStateAttack2Step = 0;
	mStateDizzyStep = 0;
	mStateHitStep = 0;
	mStateDieStep = 0;
}

// �ҋ@���
void CSlime::UpdateIdle()
{
	SetAnimationSpeed(0.7f);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// �ҋ@2���
void CSlime::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeAnimation(EAnimType::eIdle3);
		ChangeState(EState::eIdleWait);
	}
}

// �U���������̑ҋ@���
void CSlime::UpdateIdle3()
{
	SetAnimationSpeed(0.8f);
	ChangeAnimation(EAnimType::eIdle4);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE && player->Position().Y() < 0.5f)
	{
		ChangeState(EState::eRun);
	}
	else 
	{
		if (vectorPos <= 25.0f && player->Position().Y() >= 6.0f)
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
		}
	}
}

// �ҋ@2�̏I���҂�
void CSlime::UpdateIdleWait()
{
	SetAnimationSpeed(0.5f);
	// �ҋ@3�A�j���[�V�����ɐ؂�ւ�
	ChangeAnimation(EAnimType::eIdle3);
	// �ҋ@3�A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle);
	}
}

// �U��
void CSlime::UpdateAttack()
{
	SetAnimationSpeed(0.5f);

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateAttackStep)
	{
		// �X�e�b�v0 : �U���A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		AttackStart();
		mStateAttackStep++;
		break;
		// �X�e�b�v1�@: ���ʉ��J�n	
	case 1:
		if (mAnimationFrame >= 1.0f)
		{
			mpSlimeAttackSE->Play();
			mIsSlimeAttackSE = true;
			mStateAttackStep++;
		}
		break;
		// �X�e�b�v2 : �U���I���҂�
	case 2:
		if (mAnimationFrame >= 25.0f)
		{
			// �U���I���҂���Ԃֈڍs
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U��2
void CSlime::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateAttack2Step)
	{
		// �X�e�b�v0 : �U��2�A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		if (mAnimationFrame >= 10.0f)
		{
			AttackStart();
			mpColliderSphereBody->SetEnable(false);
			mStateAttack2Step++;
		}
		break;
		// �X�e�b�v1�@: ���ʉ��J�n	
	case 1:
		if (mAnimationFrame >= 1.0f)
		{
			mpSlimeAttackSE->Play();
			mIsSlimeAttackSE = true;
			mStateAttack2Step++;
		}
		break;
		// �X�e�b�v2 : �U���I���҂�
	case 2:
		if (mAnimationFrame >= 25.0f)
		{
			// �U���I���҂���Ԃֈڍs
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U���ƍU��2�I���҂�
void CSlime::UpdateAttackWait()
{
	// �U���ƍU��2�A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpColliderSphereBody->SetEnable(true);
		CPlayer* player = CPlayer::Instance();
		float vectorPos = (player->Position() - Position()).Length();
		if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
		{
			ChangeState(EState::eRun);
		}
		else
		{
			SetAnimationSpeed(0.5f);
			// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
			ChangeState(EState::eIdle3);
			ChangeAnimation(EAnimType::eIdle4);
		}
	}
}

// �q�b�g
void CSlime::UpdateHit()
{
	SetAnimationSpeed(0.5f);

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateHitStep)
	{
		// �X�e�b�v0 :�q�b�g�A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eHit);
		mStateHitStep++;
		break;
		// �X�e�b�v1�@: ���ʉ��J�n	
	case 1:
		if (mAnimationFrame >= 0.0f)
		{
			mpSlimeHitSE->Play(3.0f);
			mIsSlimeHitSE = true;
			mStateHitStep++;
		}
		break;
		// �X�e�b�v2 : �q�b�g���[�V�����I���҂�
	case 2:
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
				ChangeAnimation(EAnimType::eIdle4);
			}
		}
		break;
	}
}

// ���ʎ�
void CSlime::UpdateDie()
{
	SetAnimationSpeed(0.5f);

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateDieStep)
	{
		// �X�e�b�v0 :���S�A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eDie);
		mStateDieStep++;
		break;
		// �X�e�b�v1�@: ���ʉ��J�n	
	case 1:
		if (mAnimationFrame >= 0.0f)
		{
			mpSlimeHitSE->Play(3.0f);
			mIsSlimeHitSE = true;
		}
		if (mAnimationFrame >= 15.0f)
		{
			mpSlimeDieSE->Play(5.0f);
			mIsSlimeDieSE = true;
			mStateDieStep++;
		}
		break;
		// �X�e�b�v2 : ���S���[�V�����I���҂�
	case 2:
		if (IsAnimationFinished())
		{
			Kill();
			// �G�l�~�[�̎��S����
			CEnemy::SlimeDeath();
		}
		break;
	}
}

// �߂܂�(����)
void CSlime::UpdateDizzy()
{
	SetAnimationSpeed(0.5f);

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateDizzyStep)
	{
		// �X�e�b�v0 :�߂܂��A�j���[�V�����J�n
	case 0:
		// �߂܂��A�j���[�V�������J�n
		ChangeAnimation(EAnimType::eDizzy);
		mStateDizzyStep++;
		break;
		// �X�e�b�v1�@: ���ʉ��J�n	
	case 1:
		if (mAnimationFrame >= 0.0f)
		{
			mpSlimeDizzySE->Play();
			mIsSlimeDizzySE = true;
			mStateDizzyStep++;
		}
		break;
		// �X�e�b�v2 : �߂܂����[�V�����I���҂�
	case 2:
		if (IsAnimationFinished())
		{
			// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
			ChangeState(EState::eIdle3);
			ChangeAnimation(EAnimType::eIdle4);
		}
		break;
	}
}

// �ړ�
void CSlime::UpdateRun()
{
	SetAnimationSpeed(0.6f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	if (mAnimationFrame >= 5.0f)
	{
		mpSlimeRunSE->Play();
		mIsSlimeRunSE = true;
	}

	// �͈͓��̎��A�ړ����ǐՂ���
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += newPos * MOVE_SPEED;
		if (vectorPos <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	// �ǐՂ��~�܂������A�U���p�̑ҋ@���[�V������
	else if (vectorPos <= STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		ChangeState(EState::eIdle3);
	}
    if (vectorPos <= 25.0f && player->Position().Y() >= 6.0f)
	{
		ChangeState(EState::eIdle3);
	}
}



// �X�V����
void CSlime::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.Y(0.0f);
	mHp = mCharaStatus.hp;

	if (mState != EState::eRun)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
	}
	
	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eIdle2:
		UpdateIdle2();
		break;
	case EState::eIdle3:
		UpdateIdle3();
		break;
		// �ҋ@�I���҂�
	case EState::eIdleWait:
		UpdateIdleWait();
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
		// ���ʎ�
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
	CVector gaugePos = Position() + CVector(0.0f, 30.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	// HP����������U���J�n
	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		mpHpGauge->SetWorldPos(gaugePos);

		(mAttackTime++ / 100);

		if (mAttackTime > 230)
		{
			// ��U��
			bool BigAttack = false;
			// �m�����ŏ���8�ő�10
			int probability2 = Math::Rand(8, 11);
			if (probability2 == 8)BigAttack = true;

			if (BigAttack)
			{
				ChangeState(EState::eAttack2);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}
		else if (mCharaStatus.hp <= 0)
		{
			mAttackTime = 0;
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eDizzy)
		{
			mAttackTime = 0;
		}
		if (vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
	}

	if (mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eIdleWait)
	{
		if (vectorPos <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	if (vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �L�����N�^�[�̉����߂��R���C�_�[
	mpColliderSphereBody->Update();
	// �_���[�W���󂯂�R���C�_�[
	mpDamageColBody->Update();
	// �U���R���C�_�[
	mpAttackColBody->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// �Փˏ���
void CSlime::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
    // �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColBody && mState != EState::eIdle && mState != EState::eIdle2 &&
		mState != EState::eIdle3 && mState != EState::eIdleWait)
	{
		// �L�����̃|�C���^�ɕϊ�
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// ����̃R���C�_�[�̎����傪�L�����ł���΁A
		if (chara != nullptr)
		{
			// ���ɍU���ς݂̃L�����łȂ����
			if (!IsAttackHitObj(chara))
			{
				int damage = CalcDamage(1.0f,this,chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, this);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
	// �t�B�[���h�Ƃ̐ڒn����
	else if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;
			mMoveSpeed.Y(0.0f);

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// �L�����N�^�[���m�̏Փˏ���
	else if (self == mpColliderSphereBody)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CSlime::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackColBody->SetEnable(true);
}

// �U���I��
void CSlime::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackColBody->SetEnable(false);
}

// �`��
void CSlime::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CSlime::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CSlime::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CSlime::TakeDamage(int damage, CObjectBase* causedObj)
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
		Position(Position() - dir * Scale().X() * 0.4f);
	}
}

// ���S����
void CSlime::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}