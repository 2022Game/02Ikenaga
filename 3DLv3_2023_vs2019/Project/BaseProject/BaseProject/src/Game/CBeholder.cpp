#include "CBeholder.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CLightningBallEffect.h"
#include "CElectricShockEffect.h"
#include "CInput.h"

// ���̂̃����X�^�[�̃C���X�^���X
CBeholder* CBeholder::spInstance = nullptr;

#define ENEMY_HEIGHT 0.5f
#define WITHIN_RANGE 40.0f       // �͈͓�
#define MOVE_SPEED 0.1f          // �ړ����x
#define GRAVITY 0.0625f          // �d��
#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 24.0f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f     // ��]����͈�

// ���̂̃����X�^�[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CBeholder::AnimData CBeholder::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle.x",	true,	82.0f	},	    // �ҋ@ 41.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle2.x",	true,	46.0f	},	    // �ҋ@2 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack.x",	true,	50.0f	},	    // �U�� 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack2.x",	true,	50.0f	},	    // �U��2 21.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack3.x",	true,	42.0f	},	    // �U��3 17.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack4.x",	true,	50.0f	},	    // �U��4 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderGetHit.x",	true,	46.0f	},	    // �q�b�g 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderDie.x",	true,	76.0f	},	        // ���� 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderDizzy.x",	true,	82.0f	},	    // �߂܂� 41.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderRun.x",	true,	34.0f	},	    // ���� 17.0f
};

// �R���X�g���N�^
CBeholder::CBeholder()
	: mpRideObject(nullptr)
	, mAttackTime(0)
	, mFlyingTime(0)
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

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, -0.5f),
		CVector(0.0f, 0.0f, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine->Position(0.0f, 20.0f, 0.0f);

	mpColliderLine2 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, -0.8, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine2->SetCollisionLayers({ ELayer::eField });
	mpColliderLine2->Position(0.0f, 20.0f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,0.41f, false, 2.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(����̐G��)
	mpColliderSphereTentacle = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphereTentacle->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�E��̐G��)
	mpColliderSphereTentacle2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.1f, false, 1.0f
	);
	mpColliderSphereTentacle2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�����̐G��)
	mpColliderSphereTentacle3 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphereTentacle3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�E���̐G��)
	mpColliderSphereTentacle4 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphereTentacle4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�^�񒆏�̐G��)
	mpColliderSphereTentacle5 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 1.0f
	);
	mpColliderSphereTentacle5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�^�񒆉��̐G��)
	mpColliderSphereTentacle6 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphereTentacle6->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.41f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(����̐G��)
	mpDamageColTentacle = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.087f, false
	);
	mpDamageColTentacle->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��̐G��)
	mpDamageColTentacle2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.1f, false
	);
	mpDamageColTentacle2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle2->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�����̐G��)
	mpDamageColTentacle3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageColTentacle3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle3->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�E���̐G��)
	mpDamageColTentacle4 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageColTentacle4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle4->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�^�񒆏�̐G��)
	mpDamageColTentacle5 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.08f, false
	);
	mpDamageColTentacle5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle5->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�^�񒆉��̐G��)
	mpDamageColTentacle6 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageColTentacle6->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTentacle6->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.41f, false
	);
	// �U���R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(����̐G��)
	mpAttackColTentacle = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackColTentacle->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�E��̐G��)
	mpAttackColTentacle2 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.1f, false
	);
	mpAttackColTentacle2->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle2->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�����̐G��)
	mpAttackColTentacle3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackColTentacle3->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle3->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�E���̐G��)
	mpAttackColTentacle4 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackColTentacle4->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle4->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�^�񒆏�̐G��)
	mpAttackColTentacle5 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.08f, false
	);
	mpAttackColTentacle5->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTentacle5->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�^�񒆉��̐G��)
	mpAttackColTentacle6 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
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
}

CBeholder::~CBeholder()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderLine2);

	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereTentacle);
	SAFE_DELETE(mpColliderSphereTentacle2);
	SAFE_DELETE(mpColliderSphereTentacle3);
	SAFE_DELETE(mpColliderSphereTentacle4);
	SAFE_DELETE(mpColliderSphereTentacle5);
	SAFE_DELETE(mpColliderSphereTentacle6);

	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColTentacle);
	SAFE_DELETE(mpDamageColTentacle2);
	SAFE_DELETE(mpDamageColTentacle3);
	SAFE_DELETE(mpDamageColTentacle4);
	SAFE_DELETE(mpDamageColTentacle5);
	SAFE_DELETE(mpDamageColTentacle6);

	SAFE_DELETE(mpAttackColBody);
	SAFE_DELETE(mpAttackColTentacle);
	SAFE_DELETE(mpAttackColTentacle2);
	SAFE_DELETE(mpAttackColTentacle3);
	SAFE_DELETE(mpAttackColTentacle4);
	SAFE_DELETE(mpAttackColTentacle5);
	SAFE_DELETE(mpAttackColTentacle6);
}

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

// �ҋ@���
void CBeholder::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �ҋ@���2
void CBeholder::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mState = EState::eRun;
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle2);
		if (IsAnimationFinished())
		{
			mState = EState::eIdle2;
		}
	}
}

// �U��
void CBeholder::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	if (mAnimationFrame >= 0.0f && mAnimationFrame < 5.0f)
	{
		AttackStart();
	}

	if (mAnimationFrame >= 10.0f)
	{
		if (!mpLightningBall->IsThrowing()&& !mpElectricShock->IsThrowing())
		{
			mpLightningBall->Start();
			mpElectricShock->Start();
		}
	}

	if (IsAnimationFinished())
	{
		mpLightningBall->Stop();
		mpElectricShock->Stop();
		// �U���I���҂���Ԃֈڍs
		mState = EState::eAttackWait;
	}
}

// �U��2
void CBeholder::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack2);
	if (mAnimationFrame >= 0.0f && mAnimationFrame < 5.0f)
	{
		AttackStart();
	}

	if (IsAnimationFinished())
	{
		// �U��2�I���҂���Ԃֈڍs
		mState = EState::eAttackWait;
	}
}

// �U��3
void CBeholder::UpdateAttack3()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack3);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U��4
void CBeholder::UpdateAttack4()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack4);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U���I���҂�
void CBeholder::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpElectricShock->Stop();
		mState = EState::eIdle2;
	}
}

// �q�b�g
void CBeholder::UpdateHit()
{
	mpElectricShock->Stop();
	mpLightningBall->Stop();
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
			mState = EState::eIdle2;
			ChangeAnimation(EAnimType::eIdle2);
		}
	}
}

// ����
void CBeholder::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
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
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		mState = EState::eIdle2;
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// �ړ�
void CBeholder::UpdateRun()
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
		mState = EState::eIdle2;
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// �X�V����
void CBeholder::Update()
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
		// �U��3
	case EState::eAttack3:
		UpdateAttack3();
		break;
		// �U��4
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
	float vectorp = (player->Position() - Position()).Length();

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eAttack 
		&& mState != EState::eAttack2 && mState != EState::eAttack3 && mState != EState::eAttack4  && mState != EState::eAttackWait
		&& mState != EState::eHit && mState != EState::eDie && mState != EState::eDizzy && mState != EState::eRun)
	{
		mState = EState::eIdle2;
	}

	if (mState == EState::eRun || mState == EState::eIdle2 || mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3
		|| mState == EState::eAttack4 || mState == EState::eHit || mState == EState::eDie || mState == EState::eDizzy || mState == EState::eAttackWait)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
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
				mState = EState::eAttack2;
			}
			else if (Attack3)
			{
				//mState = EState::eAttack3;
			}
			else if (Attack4)
			{
				//mState = EState::eAttack4;
			}
			else
			{
				mState = EState::eAttack;
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3 || mState == EState::eAttack4)
		{
			mAttackTime = 0;
		}
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (mState == EState::eIdle2 || mState == EState::eRun)
	{
		if (mFlyingTime <= 200 && Position().Y() <= 0.0f)
		{
			mMoveSpeed.Y(mMoveSpeed.Y() + 0.02f);
		}

		if (mFlyingTime >= 200 && Position().Y() >= 0.1f)
		{
			Position(Position().X(), Position().Y() - 0.5f, Position().Z());
		}
	}

	if (Position().Y() >= 0.1f || vectorp >= 24.0f && vectorp <= WALK_RANGE)
	{
		mFlyingTime++;
	}

	if (Position().Y() <= 0.0f)
	{
		mFlyingTime = 0;
	}
	if (mState == EState::eHit)
	{
		Position(Position().X(), Position().Y() - 0.5f, Position().Z());
	}

	CDebugPrint::Print(" �U������: %d\n", mAttackTime);
	//CDebugPrint::Print(" HP: %d\n", mCharaStatus.hp);

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
				int damage = CalcDamage(this, chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, this);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
	else if (self == mpColliderLine || self == mpColliderLine2)
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
	mpAttackColBody->SetEnable(true);
	mpAttackColTentacle->SetEnable(true);
	mpAttackColTentacle2->SetEnable(true);
	mpAttackColTentacle3->SetEnable(true);
	mpAttackColTentacle4->SetEnable(true);
	mpAttackColTentacle5->SetEnable(true);
	mpAttackColTentacle6->SetEnable(true);
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
		Position(Position() - dir * Scale().X() * 0.4f);
	}
}


// ���S����
void CBeholder::Death()
{
	// ���S��Ԃֈڍs
	mState = EState::eDie;
}