#include "CBoxer2.h"
#include "CPlayer.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CImpactEffect.h"

// �{�N�T�[2�̃C���X�^���X
CBoxer2* CBoxer2::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 60.0f       // �͈͓�
#define MOVE_SPEED 0.13f         // �ړ����x
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f          // �d��
#define JUMP_END_Y 1.0f
#define WALK_RANGE 150.0f        // �ǐՂ���͈�
#define STOP_RANGE 30.0f         // �ǐՂ����߂�͈�
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
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.4f, false, 2.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.04f, 0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.5f, false, 4.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.07f, 0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(�E��)
	mpColliderSphereHandR = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.27f, false, 3.0f
	);
	mpColliderSphereHandR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHandR->Position(0.0f, -0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(����)
	mpColliderSphereHandL = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.27f, false, 3.0f
	);
	mpColliderSphereHandL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHandL->Position(0.0f, -0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(�E��)
	mpColliderSphereFeetR = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.06f, false, 3.0f
	);
	mpColliderSphereFeetR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeetR->Position(0.0f, 0.03f, 0.0f);

	// �L�����N�^�[�����߂�����(����)
	mpColliderSphereFeetL = new CColliderSphere
	(
		this, ELayer::eEnemy, 0.06f, false, 3.0f
	);
	mpColliderSphereFeetL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeetL->Position(0.0f, 0.03f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.4f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon,ETag::eFlame });
	//�_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageColHead->Position(0.04f, 0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.5f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon,ETag::eFlame });
	mpDamageColBody->Position(0.07f, 0.0f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��)
	mpDamageColHandR = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.27f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHandR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHandR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHandR->Position(0.0f, -0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageColHandL = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.27f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHandL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHandL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHandL->Position(0.0f, -0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��)
	mpDamageColFeetR = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.1f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColFeetR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeetR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeetR->Position(0.0f, 0.25f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageColFeetL = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.1f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColFeetL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeetL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeetL->Position(0.0f, 0.25f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.4f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.04f, 0.1f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.5f, false
	);
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });
	mpAttackColBody->Position(0.07f, 0.0f, 0.0f);

	// �_���[�W��^����R���C�_�[(�E��)
	mpAttackColHandR = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.27f, false
	);
	mpAttackColHandR->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHandR->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHandR->Position(0.0f, -0.1f, 0.0f);

	// �_���[�W��^����R���C�_�[(�E��)
	mpAttackColFeetR = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.06f, false
	);
	mpAttackColFeetR->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColFeetR->SetCollisionTags({ ETag::ePlayer });
	mpAttackColFeetR->Position(0.0f, 0.03f, 0.0f);

	// �_���[�W��^����R���C�_�[(����)
	mpAttackColFeetL = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.06f, false
	);
	mpAttackColFeetL->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColFeetL->SetCollisionTags({ ETag::ePlayer });
	mpAttackColFeetL->Position(0.0f, 0.03f, 0.0f);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_neck_01");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpDamageColHead->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_spine_02");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);
	mpAttackColBody->SetAttachMtx(bodyMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̉E��̍s��ɃA�^�b�`
	const CMatrix* rightHandMty = GetFrameMtx("Armature_drill_r");
	mpColliderSphereHandR->SetAttachMtx(rightHandMty);
	mpDamageColHandR->SetAttachMtx(rightHandMty);
	mpAttackColHandR->SetAttachMtx(rightHandMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[���{�N�T�[�̍���̍s��ɃA�^�b�`
	const CMatrix* leftHandMty = GetFrameMtx("Armature_drill_l");
	mpColliderSphereHandL->SetAttachMtx(leftHandMty);
	mpDamageColHandL->SetAttachMtx(leftHandMty);

	// �_���[�W���󂯂�R���C�_�[���{�N�T�[�̉E���̍s��ɃA�^�b�`
	const CMatrix* rightFootMty = GetFrameMtx("Armature_thigh_r");
	mpDamageColFeetR->SetAttachMtx(rightFootMty);

	// �_���[�W���󂯂�R���C�_�[���{�N�T�[�̍����̍s��ɃA�^�b�`
	const CMatrix* leftFootMty = GetFrameMtx("Armature_thigh_l");
	mpDamageColFeetL->SetAttachMtx(leftFootMty);

	// �����߂��R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̉E���̍s��ɃA�^�b�`
	const CMatrix* rightFootMty2 = GetFrameMtx("Armature_foot_r");
	mpColliderSphereFeetR->SetAttachMtx(rightFootMty2);
	mpAttackColFeetR->SetAttachMtx(rightFootMty2);

	// �����߂��R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̍����̍s��ɃA�^�b�`
	const CMatrix* leftFootMty2 = GetFrameMtx("Armature_foot_l");
	mpColliderSphereFeetL->SetAttachMtx(leftFootMty2);
	mpAttackColFeetL->SetAttachMtx(leftFootMty2);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColHead->SetEnable(false);
	mpAttackColBody->SetEnable(false);
	mpAttackColHandR->SetEnable(false);
	mpAttackColFeetR->SetEnable(false);
	mpAttackColFeetL->SetEnable(false);

	const CMatrix* rightHandMty2 = GetFrameMtx("Armature_saw_r");
	mpImpact = new  CImpactEffect
	(
		this, rightHandMty2,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0, 90.f, 0.0f).Matrix()
	);
}

CBoxer2::~CBoxer2()
{
	SAFE_DELETE(mpColliderLine);

	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereHandR);
	SAFE_DELETE(mpColliderSphereHandL);
	SAFE_DELETE(mpColliderSphereFeetR);
	SAFE_DELETE(mpColliderSphereFeetL);

	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColHandR);
	SAFE_DELETE(mpDamageColHandL);
	SAFE_DELETE(mpDamageColFeetR);
	SAFE_DELETE(mpDamageColFeetL);

	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColBody);
	SAFE_DELETE(mpAttackColHandR);
	SAFE_DELETE(mpAttackColFeetR);
	SAFE_DELETE(mpAttackColFeetL);
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
	if (mAnimationFrame >= 0.0f && mAnimationFrame < 5.0f)
	{
		AttackStart();
	}

	if (mAnimationFrame >= 8.0f)
	{
		if (!mpImpact->IsThrowing())
		{
			mpImpact->Start();
		}
	}
	if (IsAnimationFinished())
	{
		// �U���I���҂���Ԃֈڍs
		mState = EState::eAttackWait;
	}
}

// �U��2
void CBoxer2::UpdateAttack2()
{
	ChangeAnimation(EAnimType::eAttack2);
	if (mAnimationFrame >= 0.0f && mAnimationFrame < 5.0f)
	{
		AttackStart();
	}
	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= 0.0f && vectorp <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * 0.2f;
	}
	if (IsAnimationFinished())
	{
		// �U��2�I���҂���Ԃֈڍs
		mState = EState::eAttackWait;
	}
}

// �U���I���҂�
void CBoxer2::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpImpact->Stop();
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);

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
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
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
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eJump);
	if (mMoveSpeed.Y() <= 0.0f || IsAnimationFinished())
	{
		mState = EState::eJumpEnd;
	}
}

// �W�����v�I��
void CBoxer2::UpdateJumpEnd()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// �W�����v�A�j���[�V�������ҋ@��Ԃ֖߂�
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// �q�b�g
void CBoxer2::UpdateHit()
{
	mpImpact->Stop();
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
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::BoxerDeath();
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
	if (vectorp <= STOP_RANGE && vectorp >= 33.0f)
	{
		//mMoveSpeed.X(0.0f);
		//mMoveSpeed.Z(0.0f);

		// ��]����͈͂ł����
		if (vectorp <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
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
			ChangeAnimation(EAnimType::eIdle2);
			mState = EState::eIdle2;
		}
	}
}

// ����
void CBoxer2::UpdateSlide()
{
	ChangeAnimation(EAnimType::eSlide);
	if (mAnimationFrame >= 0.0f && mAnimationFrame < 5.0f)
	{
		AttackStart();
	}
	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= 0.0f && vectorp <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
	}
	if (IsAnimationFinished())
	{
		// ����I���҂���Ԃֈڍs
		mState = EState::eAttackWait;
	}
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
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eAttack && mState != EState::eAttack2
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

	//CDebugPrint::Print(" HP: %f\n", boxer->Position().Y());

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE || mState == EState::eAttack2 || mState == EState::eSlide)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (Position().Y() >= 0.1f)
	{
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereHandR->Update();
	mpColliderSphereHandL->Update();
	mpColliderSphereFeetR->Update();
	mpColliderSphereFeetL->Update();

	mpDamageColHead->Update();
	mpDamageColBody->Update();
	mpDamageColHandR->Update();
	mpDamageColHandL->Update();
	mpDamageColFeetR->Update();
	mpDamageColFeetL->Update();

	mpAttackColHead->Update();
	mpAttackColBody->Update();
	mpAttackColHandR->Update();
	mpAttackColFeetR->Update();
	mpAttackColFeetL->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// �Փˏ���
void CBoxer2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColHead || self == mpAttackColBody || self == mpAttackColHandR || self == mpAttackColFeetR
		|| self == mpAttackColFeetL && mState != EState::eIdle && mState != EState::eIdle2)
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
	else if (self == mpColliderSphereHead || self == mpColliderSphereBody || self == mpColliderSphereHandR
		|| self == mpColliderSphereHandL || self == mpColliderSphereFeetR || self == mpColliderSphereFeetL)
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
	mpAttackColHead->SetEnable(true);
	mpAttackColBody->SetEnable(true);
	mpAttackColHandR->SetEnable(true);
	mpAttackColFeetR->SetEnable(true);
	mpAttackColFeetL->SetEnable(true);
}

// �U���I��
void CBoxer2::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackColHead->SetEnable(false);
	mpAttackColBody->SetEnable(false);
	mpAttackColHandR->SetEnable(false);
	mpAttackColFeetR->SetEnable(false);
	mpAttackColFeetL->SetEnable(false);
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