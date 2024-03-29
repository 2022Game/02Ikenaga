#include "CBeholder.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"

// ���̂̃����X�^�[�̃C���X�^���X
CBeholder* CBeholder::spInstance = nullptr;

#define ENEMY_HEIGHT 0.3f
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
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eEnemy,0.41f, false, 2.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(����̐G��)
	mpColliderSphere2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphere2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�E��̐G��)
	mpColliderSphere3 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.1f, false, 1.0f
	);
	mpColliderSphere3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�����̐G��)
	mpColliderSphere4 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphere4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�E���̐G��)
	mpColliderSphere5 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphere5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�^�񒆏�̐G��)
	mpColliderSphere6 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.08f, false, 1.0f
	);
	mpColliderSphere6->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �L�����N�^�[�����߂�����(�^�񒆉��̐G��)
	mpColliderSphere7 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.087f, false, 1.0f
	);
	mpColliderSphere7->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.41f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(����̐G��)
	mpDamageCol2 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.087f, false
	);
	mpDamageCol2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol2->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��̐G��)
	mpDamageCol3 = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.1f, false
	);
	mpDamageCol3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol3->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�����̐G��)
	mpDamageCol4 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageCol4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol4->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�E���̐G��)
	mpDamageCol5 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageCol5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol5->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�^�񒆏�̐G��)
	mpDamageCol6 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.08f, false
	);
	mpDamageCol6->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol6->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W���󂯂�R���C�_�[���쐬(�^�񒆉��̐G��)
	mpDamageCol7 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.087f, false
	);
	mpDamageCol7->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol7->SetCollisionTags({ ETag::eWeapon });

	// �_���[�W��^����R���C�_�[(��)
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.41f, false
	);
	// �U���R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(����̐G��)
	mpAttackCol2 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackCol2->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol2->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�E��̐G��)
	mpAttackCol3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.1f, false
	);
	mpAttackCol3->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol3->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�����̐G��)
	mpAttackCol4 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackCol4->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol4->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�E���̐G��)
	mpAttackCol5 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackCol5->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol5->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�^�񒆏�̐G��)
	mpAttackCol6 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.08f, false
	);
	mpAttackCol6->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol6->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W��^����R���C�_�[(�^�񒆉��̐G��)
	mpAttackCol7 = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.087f, false
	);
	mpAttackCol7->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol7->SetCollisionTags({ ETag::ePlayer });

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpColliderSphere->SetAttachMtx(bodyMty);
	mpDamageCol->SetAttachMtx(bodyMty);
	mpAttackCol->SetAttachMtx(bodyMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̍���̐G��̍s��ɃA�^�b�`
	const CMatrix* tentacleMty = GetFrameMtx("Armature_TentacleA05");
	mpColliderSphere2->SetAttachMtx(tentacleMty);
	mpDamageCol2->SetAttachMtx(tentacleMty);
	mpAttackCol2->SetAttachMtx(tentacleMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̉E��̐G��̍s��ɃA�^�b�`
	const CMatrix* tentacleMty2 = GetFrameMtx("Armature_TentacleE05");
	mpColliderSphere3->SetAttachMtx(tentacleMty2);
	mpDamageCol3->SetAttachMtx(tentacleMty2);
	mpAttackCol3->SetAttachMtx(tentacleMty2);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̍����̐G��̍s��ɃA�^�b�`
	const CMatrix * tentacleMty3 = GetFrameMtx("Armature_TentacleB05");
	mpColliderSphere4->SetAttachMtx(tentacleMty3);
	mpDamageCol4->SetAttachMtx(tentacleMty3);
	mpAttackCol4->SetAttachMtx(tentacleMty3);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̉E���̐G��̍s��ɃA�^�b�`
	const CMatrix* tentacleMty4 = GetFrameMtx("Armature_TentacleD05");
	mpColliderSphere5->SetAttachMtx(tentacleMty4);
	mpDamageCol5->SetAttachMtx(tentacleMty4);
	mpAttackCol5->SetAttachMtx(tentacleMty4);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̐^�񒆏�̐G��̍s��ɃA�^�b�`
	const CMatrix* tentacleMty5 = GetFrameMtx("Armature_TentacleF05");
	mpColliderSphere6->SetAttachMtx(tentacleMty5);
	mpDamageCol6->SetAttachMtx(tentacleMty5);
	mpAttackCol6->SetAttachMtx(tentacleMty5);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[�����̂̃����X�^�[�̐^�񒆉��̐G��̍s��ɃA�^�b�`
	const CMatrix* tentacleMty6 = GetFrameMtx("Armature_TentacleC05");
	mpColliderSphere7->SetAttachMtx(tentacleMty6);
	mpDamageCol7->SetAttachMtx(tentacleMty6);
	mpAttackCol7->SetAttachMtx(tentacleMty6);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackCol->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
	mpAttackCol4->SetEnable(false);
	mpAttackCol5->SetEnable(false);
	mpAttackCol6->SetEnable(false);
	mpAttackCol7->SetEnable(false);
}

CBeholder::~CBeholder()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpColliderSphere2);
	SAFE_DELETE(mpColliderSphere3);
	SAFE_DELETE(mpColliderSphere4);
	SAFE_DELETE(mpColliderSphere5);
	SAFE_DELETE(mpColliderSphere6);
	SAFE_DELETE(mpColliderSphere7);

	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpDamageCol2);
	SAFE_DELETE(mpDamageCol3);
	SAFE_DELETE(mpDamageCol4);
	SAFE_DELETE(mpDamageCol5);
	SAFE_DELETE(mpDamageCol6);
	SAFE_DELETE(mpDamageCol7);

	SAFE_DELETE(mpAttackCol);
	SAFE_DELETE(mpAttackCol2);
	SAFE_DELETE(mpAttackCol3);
	SAFE_DELETE(mpAttackCol4);
	SAFE_DELETE(mpAttackCol5);
	SAFE_DELETE(mpAttackCol6);
	SAFE_DELETE(mpAttackCol7);
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
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U��2
void CBeholder::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
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
		mState = EState::eIdle2;
	}
}

// �q�b�g
void CBeholder::UpdateHit()
{
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
		CEnemy::Death();
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
				mState = EState::eAttack3;
			}
			else if (Attack4)
			{
				mState = EState::eAttack4;
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

	if (mState == EState::eRun)
	{
		if (Position().Y() < 2.0f)
		{
			mMoveSpeed += CVector(0.0f, 0.5f, 0.0f);
		}

		if (Position().Y() >= 10.0f && Position().Y() < 12.0f)
		{
			mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
		}

		if (Position().Y() >= 20.0f)
		{
			mMoveSpeed -= CVector(0.0f, 1.0f, 0.0f);
		}
	}

	CDebugPrint::Print(" �U������: %d\n", mAttackTime);
	//CDebugPrint::Print(" HP: %d\n", mCharaStatus.hp);

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mpColliderSphere->Update();
	mpColliderSphere2->Update();
	mpColliderSphere3->Update();
	mpColliderSphere4->Update();
	mpColliderSphere5->Update();
	mpColliderSphere6->Update();
	mpColliderSphere7->Update();

	mpDamageCol->Update();
	mpDamageCol2->Update();
	mpDamageCol3->Update();
	mpDamageCol4->Update();
	mpDamageCol5->Update();
	mpDamageCol6->Update();
	mpDamageCol7->Update();

	mpAttackCol->Update();
	mpAttackCol2->Update();
	mpAttackCol3->Update();
	mpAttackCol4->Update();
	mpAttackCol5->Update();
	mpAttackCol6->Update();
	mpAttackCol7->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// �Փˏ���
void CBeholder::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackCol || self == mpAttackCol2 || self == mpAttackCol3 || self == mpAttackCol4 || self == mpAttackCol5
		|| self == mpAttackCol6 || self == mpAttackCol7 && mState != EState::eIdle && mState != EState::eIdle2)
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
	else if (self == mpColliderSphere || self == mpColliderSphere2 || self == mpColliderSphere3
		|| self == mpColliderSphere4 || self == mpColliderSphere5 || self == mpColliderSphere6 || self == mpColliderSphere7)
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
	mpAttackCol->SetEnable(true);
	mpAttackCol2->SetEnable(true);
	mpAttackCol3->SetEnable(true);
	mpAttackCol4->SetEnable(true);
	mpAttackCol5->SetEnable(true);
	mpAttackCol6->SetEnable(true);
	mpAttackCol7->SetEnable(true);
}

// �U���I��
void CBeholder::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
	mpAttackCol4->SetEnable(false);
	mpAttackCol5->SetEnable(false);
	mpAttackCol6->SetEnable(false);
	mpAttackCol7->SetEnable(false);
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
	mCharaMaxStatus = ENEMY4_STATUS[index];
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