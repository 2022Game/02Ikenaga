#include "CBoxer2.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"

// �{�N�T�[2�̃C���X�^���X
CBoxer2* CBoxer2::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 40.0f       // �͈͓�
#define MOVE_SPEED 0.08f         // �ړ����x
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f          // �d��
#define JUMP_END_Y 1.0f
#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 15.0f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f     // ��]����͈�

// �{�N�T�[2�̃A�j���[�V�����f�[�^�̃e�[�u��
const CBoxer2::AnimData CBoxer2::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Dance.x",	true,	170.0f	},	        // �_���X 85.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Idle.x",	true,	42.0f	},	        // �ҋ@ 21.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Attack.x",	true,	35.0f	},	    // �U�� 19.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Attack2.x",	true,	52.0f	},	    // �U��2 26.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2GetHit.x",	true,	36.0f	},	    // �q�b�g 17.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Defense.x",	true,	42.0f	},	    // �h�� 21.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2DefenseHit.x",	true,	42.0f	},  // �h�䒆�̃q�b�g 21.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Die.x",	true,	70.0f	},	        // ���� 24.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Dizzy.x",	true,	66.0f	},	        // �߂܂� 33.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2JumpStart.x",	true,	9.0f	},	    // �W�����v�̊J�n 9.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Jump.x",	true,	46.0f	},	        // �W�����v 23.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2JumpEnd.x",	true,	26.0f	},	    // �W�����v�̏I�� 26.0f
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Run.x",	true,	42.0f	},	        // ���� 21.0
	{ "Character\\Enemy\\Boxer\\animation2\\Boxer2Slide.x",	true,	72.0f	},	        // ���� 36.0
	//{ "Character\\Enemy\\Boxer\\animation2\\Boxer2DieRecover.x",	true,	48.0f	},	// �N���オ�� 27.0f
};

// �R���X�g���N�^
CBoxer2::CBoxer2()
	: mpRideObject(nullptr)
	, mAttackTime(0)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Boxer2");

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
		this, ELayer::eEnemy, 0.38f, false, 2.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere->Position(0.04f, 0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphere2 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.5f, false, 4.0f
	);
	mpColliderSphere2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere2->Position(0.07f, 0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(�E��)
	mpColliderSphere3 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.32f, false, 3.0f
	);
	mpColliderSphere3->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere3->Position(0.0f, -0.03f, 0.0f);

	// �L�����N�^�[�����߂�����(����)
	mpColliderSphere4 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.32f, false, 3.0f
	);
	mpColliderSphere4->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere4->Position(0.0f, -0.03f, 0.0f);

	// �L�����N�^�[�����߂�����(�E��)
	mpColliderSphere5 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.06f, false, 3.0f
	);
	mpColliderSphere5->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere5->Position(0.0f, 0.03f, 0.0f);

	// �L�����N�^�[�����߂�����(����)
	mpColliderSphere6 = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.06f, false, 3.0f
	);
	mpColliderSphere6->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere6->Position(0.0f, 0.03f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.38f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon,ETag::eFlame });
	//�_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageCol->Position(0.04f, 0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageCol2 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.5f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol2->SetCollisionTags({ ETag::eWeapon,ETag::eFlame });
	mpDamageCol2->Position(0.07f, 0.0f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��)
	mpDamageCol3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.32f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol3->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol3->Position(0.0f, -0.03f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageCol4 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.32f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol4->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol4->Position(0.0f, -0.03f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��)
	mpDamageCol5 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.1f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol5->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol5->Position(0.0f, 0.25f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageCol6 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.1f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol6->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol6->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol6->Position(0.0f, 0.25f, 0.0f);

	// �_���[�W��^����R���C�_�[(�E��)
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.32f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.0f, -0.03f, 0.0f);

	// �_���[�W��^����R���C�_�[(�E��)
	mpAttackCol2 = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.06f, false
	);
	mpAttackCol2->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol2->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol2->Position(0.0f, 0.03f, 0.0f);

	// �_���[�W��^����R���C�_�[(����)
	mpAttackCol3 = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.06f, false
	);
	mpAttackCol3->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol3->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol3->Position(0.0f, 0.03f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackCol4 = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.38f, false
	);
	mpAttackCol4->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol4->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol4->Position(0.04f, 0.1f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackCol5 = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.5f, false
	);
	mpAttackCol5->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol5->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol5->Position(0.07f, 0.0f, 0.0f);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_neck_01");
	mpColliderSphere->SetAttachMtx(headMty);
	mpDamageCol->SetAttachMtx(headMty);
	mpAttackCol4->SetAttachMtx(headMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_spine_02");
	mpColliderSphere2->SetAttachMtx(bodyMty);
	mpDamageCol2->SetAttachMtx(bodyMty);
	mpAttackCol5->SetAttachMtx(bodyMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̉E��̍s��ɃA�^�b�`
	const CMatrix* rightHandMty = GetFrameMtx("Armature_drill_r");
	mpColliderSphere3->SetAttachMtx(rightHandMty);
	mpDamageCol3->SetAttachMtx(rightHandMty);
	mpAttackCol->SetAttachMtx(rightHandMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[���{�N�T�[�̍���̍s��ɃA�^�b�`
	const CMatrix* leftHandMty = GetFrameMtx("Armature_drill_l");
	mpColliderSphere4->SetAttachMtx(leftHandMty);
	mpDamageCol4->SetAttachMtx(leftHandMty);

	// �_���[�W���󂯂�R���C�_�[���{�N�T�[�̉E���̍s��ɃA�^�b�`
	const CMatrix* rightFootMty = GetFrameMtx("Armature_thigh_r");
	mpDamageCol5->SetAttachMtx(rightFootMty);

	// �_���[�W���󂯂�R���C�_�[���{�N�T�[�̍����̍s��ɃA�^�b�`
	const CMatrix* leftFootMty = GetFrameMtx("Armature_thigh_l");
	mpDamageCol6->SetAttachMtx(leftFootMty);

	// �����߂��R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̉E���̍s��ɃA�^�b�`
	const CMatrix* rightFootMty2 = GetFrameMtx("Armature_foot_r");
	mpColliderSphere5->SetAttachMtx(rightFootMty2);
	mpAttackCol2->SetAttachMtx(rightFootMty2);

	// �����߂��R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̍����̍s��ɃA�^�b�`
	const CMatrix* leftFootMty2 = GetFrameMtx("Armature_foot_l");
	mpColliderSphere6->SetAttachMtx(leftFootMty2);
	mpAttackCol3->SetAttachMtx(leftFootMty2);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackCol->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
	mpAttackCol4->SetEnable(false);
	mpAttackCol5->SetEnable(false);
}

CBoxer2::~CBoxer2()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpColliderSphere2);
	SAFE_DELETE(mpColliderSphere3);
	SAFE_DELETE(mpColliderSphere4);
	SAFE_DELETE(mpColliderSphere5);
	SAFE_DELETE(mpColliderSphere6);

	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpDamageCol2);
	SAFE_DELETE(mpDamageCol3);
	SAFE_DELETE(mpDamageCol4);
	SAFE_DELETE(mpDamageCol5);
	SAFE_DELETE(mpDamageCol6);

	SAFE_DELETE(mpAttackCol);
	SAFE_DELETE(mpAttackCol2);
	SAFE_DELETE(mpAttackCol3);
	SAFE_DELETE(mpAttackCol4);
	SAFE_DELETE(mpAttackCol5);
}

CBoxer2* CBoxer2::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CBoxer2::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �ҋ@���
void CBoxer2::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �ҋ@���2
void CBoxer2::UpdateIdle2()
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
void CBoxer2::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U��2
void CBoxer2::UpdateAttack2()
{
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U���I���҂�
void CBoxer2::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();

		// �A���U�����邩�ǂ���
		bool continuousz = false;
		// �m��
		int percent = Math::Rand(0, 3);
		if (percent == 1) continuousz = true;
		if (continuousz)
		{
			mState = EState::eAttack;
		}
		else
		{
			mState = EState::eIdle2;
		}
	}
}

// �W�����v�J�n
void CBoxer2::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	if (IsAnimationFinished())
	{
		mState = EState::eJump;

		mMoveSpeed += CVector(0.0f, JUMP_SPEED, 0.0f);
		mIsGrounded = false;
	}
}

// �W�����v��
void CBoxer2::UpdateJump()
{
	ChangeAnimation(EAnimType::eJump);
	if (mMoveSpeed.Y() <= 0.0f || IsAnimationFinished())
	{
		mState = EState::eJumpEnd;
	}
}

// �W�����v�I��
void CBoxer2::UpdateJumpEnd()
{
	// �W�����v�A�j���[�V�������ҋ@��Ԃ֖߂�
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// �q�b�g
void CBoxer2::UpdateHit()
{
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

// �h��
void CBoxer2::UpdateDefense()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefense);

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= ROTATE_RANGE)
	{
		// �v���C���[�̂�������֌���
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (IsAnimationFinished())
	{
		if (mDefenseTime >= 300)
		{
			mState = EState::eAttack;
		}
		else
		{
			mState = EState::eDefense;
		}
	}
}

// �h�䒆�̃q�b�g
void CBoxer2::UpdateDefenseHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefenseHit);
	if (IsAnimationFinished())
	{
		mState = EState::eDefense;
	}
}

// ����
void CBoxer2::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
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
void CBoxer2::UpdateDizzy()
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
void CBoxer2::UpdateRun()
{
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();

	// �ǐՂ���߂Ď~�܂�
	if (vectorp <= STOP_RANGE && vectorp >= 21.0f)
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
	else if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
	}
	// �ǐՂ��~�܂������A�ҋ@���[�V������
	if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		if (mState != EState::eAttack2)
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::eIdle2;
			ChangeAnimation(EAnimType::eIdle2);
		}
	}
}

// ����
void CBoxer2::UpdateSlide()
{
	ChangeAnimation(EAnimType::eSlide);
	AttackStart();
	// ����I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �X�V����
void CBoxer2::Update()
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
		// �W�����v�J�n
	case EState::eJumpStart:
		UpdateJumpStart();
		break;
		// �W�����v��
	case EState::eJump:
		UpdateJump();
		break;
		// �W�����v�I��
	case EState::eJumpEnd:
		UpdateJumpEnd();
		break;
		// �q�b�g
	case EState::eHit:
		UpdateHit();
		break;
		// �h��
	case EState::eDefense:
		UpdateDefense();
		break;
		// �h�䒆�̃q�b�g
	case EState::eDefenseHit:
		UpdateDefenseHit();
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
		// ����
	case EState::eSlide:
		UpdateSlide();
		break;
	}

	// HP�Q�[�W�̍��W���X�V(�G�̍��W�̏�����̍��W)
	CVector gaugePos = Position() + CVector(0.0f, 30.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eAttack
		&& mState != EState::eAttackWait && mState != EState::eHit && mState != EState::eDefense && mState != EState::eDefenseHit
		&& mState != EState::eDie && mState != EState::eDizzy && mState != EState::eJumpStart && mState != EState::eJump
		&& mState != EState::eJumpEnd && mState != EState::eRun && mState != EState::eSlide)
	{
		mState = EState::eIdle2;
	}

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle)
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
			// �h��
			bool Defense = false;
			// �W�����v
			bool Jump = false;
			// ����
			bool Slide = false;

			// �m�����ŏ���2�ő�4
			int probability2 = Math::Rand(2, 4);
			// �m�����ŏ���5�ő�7
			int probability3 = Math::Rand(5, 7);
			// �m�����ŏ���8�ő�10
			int probability4 = Math::Rand(8, 10);
			//�m�����ŏ���11�ő�13
			int probability5 = Math::Rand(11, 13);

			if (probability2 == 2)Attack2 = true;
			if (probability3 == 5)Defense = true;
			if (probability4 == 8)Jump = true;
			if (probability5 == 11)Slide = true;
			if (Attack2)
			{
				mState = EState::eAttack2;
			}
			else if (Defense)
			{
				mState = EState::eDefense;
			}
			else if (Jump)
			{
				mState = EState::eJump;
			}
			else if (Slide)
			{
				mState = EState::eSlide;
			}
			else
			{
				mState = EState::eAttack;
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eSlide
			|| mState == EState::eJumpStart || mState == EState::eDefense)
		{
			mAttackTime = 0;
		}
	}

	// �h�䎞�Ԍv��
	if (mState == EState::eDefense || mState == EState::eDefenseHit)
	{
		mDefenseTime++;
	}
	else
	{
		mDefenseTime = 0;
	}

	CDebugPrint::Print(" �U������: %d\n", mAttackTime);
	//CDebugPrint::Print(" HP: %f\n", boxer->Position().Y());

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mpColliderSphere->Update();
	mpColliderSphere2->Update();
	mpColliderSphere3->Update();
	mpColliderSphere4->Update();
	mpColliderSphere5->Update();
	mpColliderSphere6->Update();

	mpDamageCol->Update();
	mpDamageCol2->Update();
	mpDamageCol3->Update();
	mpDamageCol4->Update();
	mpDamageCol5->Update();
	mpDamageCol6->Update();

	mpAttackCol->Update();
	mpAttackCol2->Update();
	mpAttackCol3->Update();
	mpAttackCol4->Update();
	mpAttackCol5->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// �Փˏ���
void CBoxer2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackCol || self == mpAttackCol2 || self == mpAttackCol3 || self == mpAttackCol4
		|| self == mpAttackCol5 && mState != EState::eIdle && mState != EState::eIdle2)
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
		|| self == mpColliderSphere4 || self == mpColliderSphere5 || self == mpColliderSphere6)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CBoxer2::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
	mpAttackCol2->SetEnable(true);
	mpAttackCol3->SetEnable(true);
	mpAttackCol4->SetEnable(true);
	mpAttackCol5->SetEnable(true);
}

// �U���I��
void CBoxer2::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
	mpAttackCol4->SetEnable(false);
	mpAttackCol5->SetEnable(false);
}

// �`��
void CBoxer2::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CBoxer2::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CBoxer2::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY9_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CBoxer2::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HP����_���[�W������
	if (mCharaStatus.hp -= damage)
	{
		if (mState == EState::eDefense)
		{
			mState = EState::eDefenseHit;
		}
		else
		{
			mState = EState::eHit;
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

// �h��͂̋����������擾
float CBoxer2::GetDefBuff(const CVector& attackDir)const
{
	// �h���Ԃł���΁A�h��2�{
	if (mState == EState::eDefense) return 2.0f;

	// �ʏ펞�̖h��̊���
	return mBaseDefenseBuffRatio;
}

// ���S����
void CBoxer2::Death()
{
	// ���S��Ԃֈڍs
	mState = EState::eDie;
}