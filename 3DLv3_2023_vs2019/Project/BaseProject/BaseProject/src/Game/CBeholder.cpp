#include "CBeholder.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CLightningBallEffect.h"
#include "CElectricShockEffect.h"
#include "CHomingBallEffect.h"
#include "CInput.h"

// ���̂̃����X�^�[�̃C���X�^���X
CBeholder* CBeholder::spInstance = nullptr;

#define ENEMY_HEIGHT  0.5f     // �����R���C�_�[
#define WITHIN_RANGE  40.0f    // �͈͓�
#define MOVE_SPEED    0.65f    // �ړ����x
#define GRAVITY       0.0625f  // �d��
#define WALK_RANGE    150.0f   // �ǐՂ���͈�
#define STOP_RANGE    26.0f    // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f   // ��]����͈�

// ���̂̃����X�^�[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CBeholder::AnimData CBeholder::ANIM_DATA[] =
{
	{ "",										                   true,    0.0f,  0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle.x",	   true,   41.0f,  0.5f},  // �ҋ@
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle2.x",    true,   23.0f,  0.5f},  // �ҋ@2
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack.x",   false,  23.0f,  0.5f},  // �U��(�d�C�{�[��)
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack2.x",  false,  21.0f,  0.4f},  // �U��2(�d��)
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack3.x",  false,  17.0f,  0.5f},  // �U��3(�d�C�{�[���̃z�[�~���O)
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack4.x",  false,  23.0f,  0.4f},  // �U��4(��]�U���{�G�t�F�N�g)
	{ "Character\\Enemy\\Beholder\\animation\\BeholderGetHit.x",   true,   23.0f,  0.5f},  // �q�b�g
	{ "Character\\Enemy\\Beholder\\animation\\BeholderDie.x",	   false,  23.0f,  0.2f},  // ����
	{ "Character\\Enemy\\Beholder\\animation\\BeholderDizzy.x",	   true,   41.0f,  0.5f},  // �߂܂�
	{ "Character\\Enemy\\Beholder\\animation\\BeholderRun.x",	   true,   17.0f,  0.5f},  // ����
};

// �R���X�g���N�^
CBeholder::CBeholder()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
	, mStateAttackStep(0)
	, mStateAttack2Step(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Beholder");

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

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, -0.8, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine->Position(0.0f, 20.0f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.41f, false, 2.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy, ELayer::eField });

	// �L�����N�^�[�����߂�����(����̐G��)
	mpColliderSphereTentacle = new CColliderSphere
	(
		this, ELayer::eEnemy, 
		0.087f, false, 1.0f
	);
	mpColliderSphereTentacle->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�E��̐G��)
	mpColliderSphereTentacle2 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.1f, false, 1.0f
	);
	mpColliderSphereTentacle2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�����̐G��)
	mpColliderSphereTentacle3 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.087f, false, 1.0f
	);
	mpColliderSphereTentacle3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�E���̐G��)
	mpColliderSphereTentacle4 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.087f, false, 1.0f
	);
	mpColliderSphereTentacle4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�^�񒆏�̐G��)
	mpColliderSphereTentacle5 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.08f, false, 1.0f
	);
	mpColliderSphereTentacle5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�^�񒆉��̐G��)
	mpColliderSphereTentacle6 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.087f, false, 1.0f
	);
	mpColliderSphereTentacle6->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.7f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	mpColliderSphereBody->Position(0.0f, 0.2f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(����̐G��)
	mpDamageColTentacle = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.087f, false
	);
	mpDamageColTentacle->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��̐G��)
	mpDamageColTentacle2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.1f, false
	);
	mpDamageColTentacle2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle2->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�����̐G��)
	mpDamageColTentacle3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 
		0.087f, false
	);
	mpDamageColTentacle3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle3->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�E���̐G��)
	mpDamageColTentacle4 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.087f, false
	);
	mpDamageColTentacle4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle4->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�^�񒆏�̐G��)
	mpDamageColTentacle5 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.08f, false
	);
	mpDamageColTentacle5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle5->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�^�񒆉��̐G��)
	mpDamageColTentacle6 = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.087f, false
	);
	mpDamageColTentacle6->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle6->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.41f, false
	);
	// �U���R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(����̐G��)
	mpAttackColTentacle = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.087f, false
	);
	mpAttackColTentacle->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�E��̐G��)
	mpAttackColTentacle2 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.1f, false
	);
	mpAttackColTentacle2->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle2->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�����̐G��)
	mpAttackColTentacle3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.087f, false
	);
	mpAttackColTentacle3->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle3->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�E���̐G��)
	mpAttackColTentacle4 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.087f, false
	);
	mpAttackColTentacle4->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle4->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�^�񒆏�̐G��)
	mpAttackColTentacle5 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.08f, false
	);
	mpAttackColTentacle5->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle5->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�^�񒆉��̐G��)
	mpAttackColTentacle6 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.087f, false
	);
	mpAttackColTentacle6->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle6->SetCollisionTags({ ETag::ePlayer });

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);
	mpAttackColBody->SetAttachMtx(bodyMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̍���̐G��̍s��ɃA�^�b�`
	const CMatrix* tentacleMty = GetFrameMtx("Armature_TentacleA05");
	mpColliderSphereTentacle->SetAttachMtx(tentacleMty);
	mpDamageColTentacle->SetAttachMtx(tentacleMty);
	mpAttackColTentacle->SetAttachMtx(tentacleMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̉E��̐G��̍s��ɃA�^�b�`
	const CMatrix* tentacleMty2 = GetFrameMtx("Armature_TentacleE05");
	mpColliderSphereTentacle2->SetAttachMtx(tentacleMty2);
	mpDamageColTentacle2->SetAttachMtx(tentacleMty2);
	mpAttackColTentacle2->SetAttachMtx(tentacleMty2);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̍����̐G��̍s��ɃA�^�b�`
	const CMatrix * tentacleMty3 = GetFrameMtx("Armature_TentacleB05");
	mpColliderSphereTentacle3->SetAttachMtx(tentacleMty3);
	mpDamageColTentacle3->SetAttachMtx(tentacleMty3);
	mpAttackColTentacle3->SetAttachMtx(tentacleMty3);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̉E���̐G��̍s��ɃA�^�b�`
	const CMatrix* tentacleMty4 = GetFrameMtx("Armature_TentacleD05");
	mpColliderSphereTentacle4->SetAttachMtx(tentacleMty4);
	mpDamageColTentacle4->SetAttachMtx(tentacleMty4);
	mpAttackColTentacle4->SetAttachMtx(tentacleMty4);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̐^�񒆏�̐G��̍s��ɃA�^�b�`
	const CMatrix* tentacleMty5 = GetFrameMtx("Armature_TentacleF05");
	mpColliderSphereTentacle5->SetAttachMtx(tentacleMty5);
	mpDamageColTentacle5->SetAttachMtx(tentacleMty5);
	mpAttackColTentacle5->SetAttachMtx(tentacleMty5);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̐^�񒆉��̐G��̍s��ɃA�^�b�`
	const CMatrix* tentacleMty6 = GetFrameMtx("Armature_TentacleC05");
	mpColliderSphereTentacle6->SetAttachMtx(tentacleMty6);
	mpDamageColTentacle6->SetAttachMtx(tentacleMty6);
	mpAttackColTentacle6->SetAttachMtx(tentacleMty6);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColBody->SetEnable(false);
	mpAttackColTentacle->SetEnable(false);
	mpAttackColTentacle2->SetEnable(false);
	mpAttackColTentacle3->SetEnable(false);
	mpAttackColTentacle4->SetEnable(false);
	mpAttackColTentacle5->SetEnable(false);
	mpAttackColTentacle6->SetEnable(false);

	mpLightningBall = new CLightningBallEffect
	(
		this, nullptr,
		CVector(0.0f, 20.0f, 0.0f),
		CQuaternion(0.0, 0.f, 0.0f).Matrix()
	);

	mpElectricShock = new  CElectricShockEffect
	(
		this, nullptr,
		CVector(0.0f, 22.0f, 0.0f),
		CQuaternion(0.0, 0.f, 0.0f).Matrix()
	);

	mpHomingBall = new CHomingBallEffect
	(
		this, nullptr,
		CVector(0.0f, 20.0f, 0.0f),
		CQuaternion(0.0, 0.f, 0.0f).Matrix()
	);
}

CBeholder::~CBeholder()
{
	// �����R���C�_�[
	SAFE_DELETE(mpColliderLine);
	// �L�����N�^�[�̉����߂��R���C�_�[
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereTentacle);
	SAFE_DELETE(mpColliderSphereTentacle2);
	SAFE_DELETE(mpColliderSphereTentacle3);
	SAFE_DELETE(mpColliderSphereTentacle4);
	SAFE_DELETE(mpColliderSphereTentacle5);
	SAFE_DELETE(mpColliderSphereTentacle6);
	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColTentacle);
	SAFE_DELETE(mpDamageColTentacle2);
	SAFE_DELETE(mpDamageColTentacle3);
	SAFE_DELETE(mpDamageColTentacle4);
	SAFE_DELETE(mpDamageColTentacle5);
	SAFE_DELETE(mpDamageColTentacle6);
	// �U���R���C�_�[
	SAFE_DELETE(mpAttackColBody);
	SAFE_DELETE(mpAttackColTentacle);
	SAFE_DELETE(mpAttackColTentacle2);
	SAFE_DELETE(mpAttackColTentacle3);
	SAFE_DELETE(mpAttackColTentacle4);
	SAFE_DELETE(mpAttackColTentacle5);
	SAFE_DELETE(mpAttackColTentacle6);
}

// �C���X�^���X
CBeholder* CBeholder::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CBeholder::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CBeholder::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateAttackStep = 0;
	mStateAttack2Step = 0;
}

// �ҋ@���
void CBeholder::UpdateIdle()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= WITHIN_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
	else if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// �ҋ@���2
void CBeholder::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >=10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE
		&& player->Position().Y() < 1.0f)
	{
		ChangeState(EState::eRun);
	}
	if (vectorPos <= 30.0f && player->Position().Y() >= 1.0f)
	{
		ChangeState(EState::eIdle2);
	}
}

// �U��(�d�C�{�[��)
void CBeholder::UpdateAttack()
{
	SetAnimationSpeed(0.5f);

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
		// �X�e�b�v1 : �U���J�n�{�d�C�{�[������
	case 1:
		if (mAnimationFrame >= 3.0f)
		{
			if (!mpLightningBall->IsThrowing() && !mpElectricShock->IsThrowing())
			{
				mpLightningBall->Start();
				mpElectricShock->Start();
				mStateAttackStep++;
			}
		}
		break;
		// �X�e�b�v2 : �U���I���{�d�C�{�[���폜�{�U���A�j���[�V�����I���҂�
	case 2:
		if (mAnimationFrame >=23.0f)
		{
			mpLightningBall->Stop();
			mpElectricShock->Stop();
			// �U���I���҂���Ԃֈڍs
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// // �U��2(�d��)
void CBeholder::UpdateAttack2()
{
	SetAnimationSpeed(0.4f);

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateAttack2Step)
	{
		// �X�e�b�v0 : �U���A�j���[�V�����J�n�{�U���J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		if (mAnimationFrame >= 3.0f)
		{
			mStateAttack2Step++;
		}
		break;
		// �X�e�b�v1 : �U���A�j���[�V�����I���҂�
	case 1:
		if (mAnimationFrame >= 20.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U��3(�d�C�{�[���̃z�[�~���O)
void CBeholder::UpdateAttack3()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack3);
	if (!mpHomingBall->IsThrowing())
	{
		mpHomingBall->Start();
	}
	// �U���I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);
}

// �U��4(��]�U���{�G�t�F�N�g)
void CBeholder::UpdateAttack4()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eAttack4);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);
}

// �U���I���҂�
void CBeholder::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpLightningBall->Stop();
		mpElectricShock->Stop();
		mpHomingBall->Stop();
		ChangeState(EState::eIdle2);
	}
}

// �q�b�g
void CBeholder::UpdateHit()
{
	SetAnimationSpeed(0.5f);
	mpElectricShock->Stop();
	mpLightningBall->Stop();
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
void CBeholder::UpdateDie()
{
	SetAnimationSpeed(0.2f);
	mpElectricShock->Stop();
	mpLightningBall->Stop();
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::BeholderDeath();
	}
}

// �߂܂�(����)
void CBeholder::UpdateDizzy()
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
void CBeholder::UpdateRun()
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

// �X�V����
void CBeholder::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.X(0.0f);
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
		// �U��(�d�C�{�[��)
	case EState::eAttack:
		UpdateAttack();
		break;
		// �U��2(�d��)
	case EState::eAttack2:
		UpdateAttack2();
		break;
		// �U��3(�d�C�{�[���̃z�[�~���O)
	case EState::eAttack3:
		UpdateAttack3();
		break;
		// �U��4(��]�U���{�G�t�F�N�g)
	case EState::eAttack4:
		UpdateAttack4();
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
	float vectorPos = (player->Position() - Position()).Length();

	if (mState != EState::eIdle && mState != EState::eDie)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eHit)
	{
		if (vectorPos <= 70.0f)
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

		if (mAttackTime > 160)
		{
			// �U��2
			bool Attack2 = false;
			// �U��3
			bool Attack3 = false;
			// �U��4
			bool Attack4 = false;
			// �m�����ŏ���2�ő�5
			int probability2 = Math::Rand(2, 5);
			// �m�����ŏ���6�ő�10
			int probability3 = Math::Rand(6, 10);
			// �m�����ŏ���11�ő�15
			int probability4 = Math::Rand(11, 15);

			if (probability2 == 2)Attack2 = true;
			if (probability3 == 6)Attack3 = true;
			if (probability4 == 11)Attack4 = true;
			if (Attack2)
			{
				//ChangeState(EState::eAttack2);
			}
			else if (Attack3)
			{
				ChangeState(EState::eAttack3);
			}
			else if (Attack4)
			{
				//ChangeState(EState::eAttack4);
			}
			else
			{
				//ChangeState(EState::eAttack);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3 || mState == EState::eAttack4)
		{
			mAttackTime = 0;
		}
	}

	if (mState == EState::eRun)
	{
		mFlyingTime++;
		if (mFlyingTime < 200 && mFlyingTime > 0 && player->Position().Y() < 1.0f)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + 0.05f);
		}
		else if(player->Position().Y() < 1.0f)
		{
			//Position(Position().X(), Position().Y() - 0.1f, Position().Z());
		}
	}
	if (mState == EState::eIdle2 && vectorPos > WALK_RANGE)
	{
		Position(Position().X(), Position().Y() - 0.05f, Position().Z());
	}
	else if(mState == EState::eIdle2 && vectorPos <= STOP_RANGE)
	{
		Position(Position().X(), Position().Y() - 0.05f, Position().Z());
	}

	if (mState == EState::eRun)
	{
		if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
		{
			Position(Position() + mMoveSpeed);
		}
	}

	if (Position().Y() <= 0.0f)
	{
		mFlyingTime = 0;
	}
	if (mState == EState::eHit)
	{
		Position(Position().X(), Position().Y() - 0.5f, Position().Z());
	}

	CDebugPrint::Print(" ��s: %d\n", mFlyingTime);
	float y = Position().Y();
	CDebugPrint::Print(" ����: %f\n", y);

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mpColliderSphereBody->Update();
	mpColliderSphereTentacle->Update();
	mpColliderSphereTentacle2->Update();
	mpColliderSphereTentacle3->Update();
	mpColliderSphereTentacle4->Update();
	mpColliderSphereTentacle5->Update();
	mpColliderSphereTentacle6->Update();

	mpDamageColBody->Update();
	mpDamageColTentacle->Update();
	mpDamageColTentacle2->Update();
	mpDamageColTentacle3->Update();
	mpDamageColTentacle4->Update();
	mpDamageColTentacle5->Update();
	mpDamageColTentacle6->Update();

	mpAttackColBody->Update();
	mpAttackColTentacle->Update();
	mpAttackColTentacle2->Update();
	mpAttackColTentacle3->Update();
	mpAttackColTentacle4->Update();
	mpAttackColTentacle5->Update();
	mpAttackColTentacle6->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);

	if (CInput::PushKey('Q'))
	{
		if (!mpLightningBall->IsThrowing())
		{
			mpLightningBall->Start();
		}
		else
		{
			mpLightningBall->Stop();
		}
	}
}

// �Փˏ���
void CBeholder::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColBody || self == mpAttackColTentacle || self == mpAttackColTentacle2 || self == mpAttackColTentacle3 || self == mpAttackColTentacle4
		|| self == mpAttackColTentacle5 || self == mpAttackColTentacle6 && mState != EState::eIdle && mState != EState::eIdle2)
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
	else if (self == mpColliderLine || self == mpColliderSphereBody)
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
	else if (self == mpColliderSphereBody || self == mpColliderSphereTentacle || self == mpColliderSphereTentacle2
		|| self == mpColliderSphereTentacle3 || self == mpColliderSphereTentacle4 || self == mpColliderSphereTentacle5
		|| self == mpColliderSphereTentacle6)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CBeholder::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	if (mState == EState::eAttack4)
	{
		mpAttackColBody->SetEnable(true);
		mpAttackColTentacle->SetEnable(true);
		mpAttackColTentacle2->SetEnable(true);
		mpAttackColTentacle3->SetEnable(true);
		mpAttackColTentacle4->SetEnable(true);
		mpAttackColTentacle5->SetEnable(true);
		mpAttackColTentacle6->SetEnable(true);
	}
}

// �U���I��
void CBeholder::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackColBody->SetEnable(false);
	mpAttackColTentacle->SetEnable(false);
	mpAttackColTentacle2->SetEnable(false);
	mpAttackColTentacle3->SetEnable(false);
	mpAttackColTentacle4->SetEnable(false);
	mpAttackColTentacle5->SetEnable(false);
	mpAttackColTentacle6->SetEnable(false);
}

// �`��
void CBeholder::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CBeholder::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CBeholder::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY8_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CBeholder::TakeDamage(int damage, CObjectBase* causedObj)
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
void CBeholder::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}