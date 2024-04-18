#include "CDragon.h"
#include "CPlayer.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "Maths.h"
#include "CFlamethrower.h"
#include "CRoarEffect.h"

// �h���S���̃C���X�^���X
CDragon* CDragon::spInstance = nullptr;

#define ENEMY_HEIGHT -2.9f
#define WITHIN_RANGE 40.0f       // �͈͓�
#define MOVE_SPEED 0.08f         // �ړ����x
#define GRAVITY 0.0625f          // �d��
#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 24.0f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  50.0f      // ��]����͈�

// �h���S���̃A�j���[�V�����f�[�^�̃e�[�u��
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{ "",										                true,	0.0f },// T�|�[�Y
	{ "Character\\Enemy\\Dragon\\animation\\DragonSleep.x",	    true,	81.0f,  0.5f },	    // �Q�� 81.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonIdle.x",	    true,	41.0f,  0.5f },	    // �ҋ@ 41.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonIdle2.x",	    true,	101.0f, 0.5f },	    // �ҋ@ 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonAttack.x",	false,	81.0f,  0.5f },	    // �U�� 81.0f
    { "Character\\Enemy\\Dragon\\animation\\DragonAttack2.x",	false,	91.0f,  0.5f },	    // �U��2 91.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonAttack3.x",	false,	36.0f,	0.5f },	    // �U��3 36.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyFlame.x",	true,	91.0f,	0.5f },	    // �t���C�t���[�� 91.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonGetHit.x",	true,	41.0f,	0.5f },	    // �q�b�g 41.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonDefend.x",	true,	61.0f,	0.5f },	    // �h�� 61.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonDie.x",	    true,	65.0f,	0.5f },	    // ���� 65.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonScream.x",	true,	101.0f,	0.5f },	    // �Y���� 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonTakeOff.x",	true,	121.0f, 0.5f },	    // ��s�J�n����O 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyIdle.x",	true,	31.0f,	0.5f },	    // �t���C�A�C�h�� 31.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonLand.x",	    true,	121.0f,	0.5f },	    // ���n 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonRun.x",	    true,	21.0f,	0.5f },	    // ���� 21.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyForward.x",	true,	62.0f	},	// �t���C�t�H���[�h 31.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyGlide.x",	true,	51.0f	},	// �t���C�O���C�h 51.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonWalk.x",	true,	82.0f	},	        // ���� 41.0f
};

// �R���X�g���N�^
CDragon::CDragon()
    : mState(EState::eIdle2)
	, mpRideObject(nullptr)
	, mFlyingTime(0)
	, mDefenseTime(0)
	, mAttackTime(0)
	, mFlyingAttackTime(0)
	, mStateStep(0)
	, mIsGrounded(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Dragon");

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
	ChangeAnimation(EAnimType::eIdle2);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine->Position(35.0f, 0.0f,-70.0f);

	// �L�����N�^�[�����߂�����(�O�̍���)
	mpColliderSphereFeet = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.7f, false, 20.0f
	);
	mpColliderSphereFeet->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeet->Position(0.0f, 0.0f, 0.2f);

	// �L�����N�^�[�����߂�����(�O�̉E��)
	mpColliderSphereFeet2 = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.7f, false, 20.0f
	);
	mpColliderSphereFeet2->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeet2->Position(0.0f, 0.0f, 0.2f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.6f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHead->Position(0.1f, 0.0f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColMouth = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.5f, false
	);
	mpDamageColMouth->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColMouth->SetCollisionTags({ ETag::eWeapon });
	mpDamageColMouth->Position(-0.2f, 0.1f, 0.15f);

	// �_���[�W���󂯂�R���C�_�[���쐬(���̐�[)
	mpDamageColTipMouth = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.4f, false
	);
	mpDamageColTipMouth->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColTipMouth->SetCollisionTags({ ETag::eWeapon });
	mpDamageColTipMouth->Position(-0.1f, 0.15f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColNeck = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.6f, false
	);
	mpDamageColNeck->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColNeck->SetCollisionTags({ ETag::eWeapon });
	mpDamageColNeck->Position(0.1f, 0.0f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColChest = new CColliderSphere
	(
		this, ELayer::eDamageCol, 3.0f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColChest->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColChest->SetCollisionTags({ ETag::eWeapon });
	mpDamageColChest->Position(-0.2f, 0.0f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�O�̍���)
	mpDamageColFeet = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.8f,false,1.0f
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColFeet->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet->Position(0.0f, 0.0f, 0.2f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�O�̉E��)
	mpDamageColFeet2 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.8f, false, 1.0f
	);
	mpDamageColFeet2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet2->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet2->Position(0.0f, 0.0f, 0.2f);

	// �_���[�W���󂯂�R���C�_�[���쐬(���̍���)
	mpDamageColFeet3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.8f, false, 1.0f
	);
	mpDamageColFeet3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet3->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet3->Position(0.0f, 0.0f, 0.2f);

	// �_���[�W���󂯂�R���C�_�[���쐬(���̉E��)
	mpDamageColFeet4= new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.8f, false, 1.0f
	);
	mpDamageColFeet4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeet4->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeet4->Position(0.0f, 0.0f, 0.2f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.6f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.1f, 0.0f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColMouth = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.5f, false
	);
	mpAttackColMouth->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColMouth->SetCollisionTags({ ETag::ePlayer });
	mpAttackColMouth->Position(-0.2f, 0.1f, 0.15f);

	// �_���[�W��^����R���C�_�[(���̐�[)
	mpAttackColTipMouth = new CColliderSphere
	(
		this, ELayer::eAttackCol, 0.4f, false
	);
	mpAttackColTipMouth->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColTipMouth->SetCollisionTags({ ETag::ePlayer });
	mpAttackColTipMouth->Position(-0.1f, 0.15f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���h���S���̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpDamageColHead->SetAttachMtx(headMty);
	mpAttackColHead->SetAttachMtx(headMty);

	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���h���S���̌��̍s��ɃA�^�b�`
	const CMatrix* mouthMty = GetFrameMtx("Armature_UpperMouth01");
	mpDamageColMouth->SetAttachMtx(mouthMty);
	mpAttackColMouth->SetAttachMtx(mouthMty);

	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���h���S���̌��̐�[�̍s��ɃA�^�b�`
	const CMatrix* mouthMty2 = GetFrameMtx("Armature_UpperMouth02");
	mpDamageColTipMouth->SetAttachMtx(mouthMty2);
	mpAttackColTipMouth->SetAttachMtx(mouthMty2);

	// �_���[�W���󂯂�R���C�_�[���h���S���̎�̍s��ɃA�^�b�`
	const CMatrix* neckMty = GetFrameMtx("Armature_Neck03");
	mpDamageColNeck->SetAttachMtx(neckMty);

	// �_���[�W���󂯂�R���C�_�[���h���S���̋��̍s��ɃA�^�b�`
	const CMatrix* chestMty = GetFrameMtx("Armature_Chest");
	mpDamageColChest->SetAttachMtx(chestMty);

	// �_���[�W���󂯂�R���C�_�[���h���S���̎�̍s��ɃA�^�b�`
	const CMatrix* handMty = GetFrameMtx("Armature_Hand_L");
	//mpColliderLine->SetAttachMtx(handMty);

	// �_���[�W���󂯂�R���C�_�[���h���S���̐Ғ�2�̍s��ɃA�^�b�`
	const CMatrix* spineMty2 = GetFrameMtx("Armature_Spine01");

	// �_���[�W���󂯂�R���C�_�[���h���S���̎�2�̍s��ɃA�^�b�`
	const CMatrix* neckMty2 = GetFrameMtx("Armature_Neck02");

	// �_���[�W���󂯂�R���C�_�[���h���S���̎�3�̍s��ɃA�^�b�`
	const CMatrix* neckMty3 = GetFrameMtx("Armature_Neck01");

	// �_���[�W���󂯂�R���C�_�[���h���S���̐Ғł̍s��ɃA�^�b�`
	const CMatrix* spineMty = GetFrameMtx("Armature_Spine02");

	// �_���[�W���󂯂�R���C�_�[���h���S���̂Ȃ��̍s��ɃA�^�b�`
	const CMatrix* jointMty = GetFrameMtx("Armature_joint19");
	
	// �_���[�W���󂯂�R���C�_�[���h���S���̂Ȃ�2�̍s��ɃA�^�b�`
	const CMatrix* jointMty2 = GetFrameMtx("Armature_joint20");

	// �_���[�W���󂯂�R���C�_�[���h���S���̂Ȃ�3�̍s��ɃA�^�b�`
	const CMatrix* jointMty3 = GetFrameMtx("Armature_joint21");

	// �_���[�W���󂯂�R���C�_�[���h���S���̂Ȃ�4�̍s��ɃA�^�b�`
	const CMatrix* jointMty4 = GetFrameMtx("Armature_joint22");

	// �_���[�W���󂯂�R���C�_�[���h���S���̂Ȃ�5�̍s��ɃA�^�b�`
	const CMatrix* jointMty5 = GetFrameMtx("Armature_joint23");

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[���h���S���̑O�̍����̍s��ɃA�^�b�`
	const CMatrix* leftFootMty = GetFrameMtx("Armature_Middle01_L");
	mpColliderSphereFeet->SetAttachMtx(leftFootMty);
	mpDamageColFeet->SetAttachMtx(leftFootMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[���h���S���̑O�̉E���̍s��ɃA�^�b�`
	const CMatrix* rightFootMty = GetFrameMtx("Armature_Middle01_R");
	mpColliderSphereFeet2->SetAttachMtx(rightFootMty);
	mpDamageColFeet2->SetAttachMtx(rightFootMty);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[���h���S���̌��̍����̍s��ɃA�^�b�`
	const CMatrix* leftFootMty2 = GetFrameMtx("Armature_MiddleToe01_L");
	mpDamageColFeet3->SetAttachMtx(leftFootMty2);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[���h���S���̌��̉E���̍s��ɃA�^�b�`
	const CMatrix* rightFootMty2 = GetFrameMtx("Armature_MiddleToe01_R");
	mpDamageColFeet4->SetAttachMtx(rightFootMty2);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColHead->SetEnable(false);
	mpAttackColMouth->SetEnable(false);
	mpAttackColTipMouth->SetEnable(false);

	const CMatrix* mtx = GetFrameMtx("Armature_Tongue01");
	mpFlamethrower = new CFlamethrower
	(
		this, mtx,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0,-90.f,0.0f).Matrix()
	); 

	mpRoar = new CRoarEffect
	(
		this,nullptr,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0, 0.f, 0.0f).Matrix()
	);
}

CDragon::~CDragon()
{
	SAFE_DELETE(mpColliderLine);
	// �L���������߂��R���C�_�[���폜
	SAFE_DELETE(mpColliderSphereFeet);
	SAFE_DELETE(mpColliderSphereFeet2);
	// �_���[�W���󂯂�R���C�_�[���폜
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColMouth);
	SAFE_DELETE(mpDamageColTipMouth);
	SAFE_DELETE(mpDamageColNeck);
	SAFE_DELETE(mpDamageColChest);
	SAFE_DELETE(mpDamageColFeet);
	SAFE_DELETE(mpDamageColFeet2);
	SAFE_DELETE(mpDamageColFeet3);
	SAFE_DELETE(mpDamageColFeet4);
	// �U���R���C�_�[���폜
	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColMouth);
	SAFE_DELETE(mpAttackColTipMouth);
}

// �h���S���̃C���X�^���X
CDragon* CDragon::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CDragon::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
	SetAnimationSpeed(data.animSpeed);
}

// ��Ԃ̐؂�ւ�
void CDragon::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// �ҋ@���
void CDragon::UpdateIdle()
{ 
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// �ҋ@���2
void CDragon::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		ChangeState(EState::eRun);
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle2);
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle2);
		}
	}
}

// �ҋ@���3
void CDragon::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// �U��
void CDragon::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
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

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
	// �X�e�b�v0 : �U���A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack);
		AttackStart();
		mStateStep++;
		break;
	// �X�e�b�v1�@: �Ή����ˊJ�n
	case 1:
		if (mAnimationFrame >= 17.5f)
		{
			if (!mpFlamethrower->IsThrowing())
			{
				mpFlamethrower->Start();
				SetAnimationSpeed(0.25f);
				mStateStep++;
			}
		}
		break;
	// �X�e�b�v2 : �Ή����ˏI���҂�
	case 2:
		if (mAnimationFrame >= 70.0f)
		{
			mpFlamethrower->Stop();
			SetAnimationSpeed(0.5f);
			// �U���I���҂���Ԃֈڍs
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U��2
void CDragon::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);
}


// �U��3
void CDragon::UpdateAttack3()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack3);
	AttackStart();
	// �U��3�I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);
}

// �U���I���҂�
void CDragon::UpdateAttackWait()
{
	mpFlamethrower->Stop();
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle2);
	}
}

// ��s���̍U��
void CDragon::UpdateFlyingAttack()
{
	ChangeAnimation(EAnimType::eFlyingAttack);
	if (mAnimationFrame >= 35.0f)
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
	}
	if (mAnimationFrame >= 140.0f)
	{
		mpFlamethrower->Stop();
	}
	if (IsAnimationFinished())
	{
		mpFlamethrower->Stop();
		// ��s���̍U���I���҂���Ԃֈڍs
		ChangeState(EState::eFlyingAttackWait);
	}
}

// ��s���̍U���I���҂�
void CDragon::UpdateFlyingAttackWait()
{
	mpFlamethrower->Stop();
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eFlyingIdle);
	}
}

// �q�b�g
void CDragon::UpdateHit()
{
	mpFlamethrower->Stop();
	// �q�b�g�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle2);
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// �h��
void CDragon::UpdateDefense()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefense);

	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// ����
void CDragon::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S������
		CEnemy::Death();
	}
}

// �Y����
void CDragon::UpdateRoar()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eRoar);
	if (mAnimationFrame >= 1.0f)
	{
		if (!mpRoar->IsThrowing())
		{
			mpRoar->Start();
		}
	}
	if (IsAnimationFinished())
	{
		mpRoar->Stop();
		ChangeState(EState::eFlyingStart);
	}
}

// ��s�J�n
void CDragon::UpdateFlyingStart()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eFlyingStart);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eFlyingIdle);
	}
}

// ��s���̑ҋ@
void CDragon::UpdateFlyingIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eFlyingIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eFlyingIdle);
	}
}

// ��s�I��
void CDragon::UpdateFlyingEnd()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eFlyingEnd);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// �ړ�
void CDragon::UpdateRun()
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
		ChangeState(EState::eIdle2);
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// �X�V����
void CDragon::Update()
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
		// ��s���̍U��
	case EState::eFlyingAttack:
		UpdateFlyingAttack();
		break;
		// ��s���̍U���I���҂�
	case EState::eFlyingAttackWait:
		UpdateFlyingAttackWait();
		break;
		// �q�b�g
	case EState::eHit:
		UpdateHit();
		break;
		// �h��
	case EState::eDefense:
		UpdateDefense();
		break;
		// ����
	case EState::eDie:
		UpdateDie();
		break;
		// �Y����
	case EState::eRoar:
		UpdateRoar();
		break;
		// ��s�J�n
	case EState::eFlyingStart:
		UpdateFlyingStart();
		break;
		// ��s�̑ҋ@
	case EState::eFlyingIdle:
		UpdateFlyingIdle();
		break;
		// ��s�I��
	case EState::eFlyingEnd:
		UpdateFlyingEnd();
		break;
		// �ړ�
	case EState::eRun:
		UpdateRun();
		break;
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eIdle3
		&& mState != EState::eAttack && mState != EState::eAttack2 && mState != EState::eAttack3 && mState != EState::eAttackWait
		&& mState != EState::eHit && mState != EState::eDefense && mState != EState::eDie && mState != EState::eRoar
		&& mState != EState::eFlyingStart && mState != EState::eFlyingIdle && mState != EState::eFlyingAttack && mState != EState::eFlyingAttackWait
		&& mState != EState::eFlyingEnd && mState != EState::eRun)
	{
		ChangeState(EState::eIdle2);
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
			// �U��
			bool Attack = false;
			// �h��
			bool Defense = false;
			// �m�����ŏ���2�ő�5
			int probability2 = Math::Rand(2, 4);
			// �m�����ŏ���5�ő�7
			int probability3 = Math::Rand(5, 7);
			// �m�����ŏ���8�ő�10
			int probability4 = Math::Rand(8, 10);

			if (probability2 == 2)Attack2 = true;
			if (probability3 == 5)Attack = true;
			if (probability4 == 8)Defense = true;
			if (Attack2)
			{
				ChangeState(EState::eAttack2);
			}
			else if (Attack)
			{
				ChangeState(EState::eAttack);
			}
			else if (Defense)
			{
				ChangeState(EState::eDefense);
			}
			else
			{
				ChangeState(EState::eAttack3);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3
			|| mState == EState::eDefense || mState == EState::eFlyingIdle)
		{
			mAttackTime = 0;
		}

		if (mCharaStatus.hp <= mCharaMaxStatus.hp * 0.5 && mFlyingTime >= 1)
		{
			mFlyingTime++;
		}
		if (mState == EState::eFlyingStart)
		{
			mFlyingTime = 0;
		}
	}

	// �h�䎞�Ԍv��
	if (mState == EState::eDefense)
	{
		mDefenseTime++;
	}
	else
	{
		mDefenseTime = 0;
	}

	if (mState != EState::eFlyingStart && mState != EState::eFlyingIdle && mState != EState::eFlyingAttack
		&& mState != EState::eFlyingAttackWait && mState != EState::eFlyingEnd)
	{
		if (mCharaStatus.hp <= mCharaMaxStatus.hp * 0.5 && mFlyingTime ==0)
		{
			ChangeState(EState::eRoar);
		}
		if (mCharaStatus.hp <= mCharaMaxStatus.hp * 0.5 && mFlyingTime >=400)
		{
			ChangeState(EState::eFlyingStart);
		}
	}

	if (mState == EState::eFlyingIdle)
	{
		mFlyingTime++;
		mFlyingAttackTime++;

		if (mFlyingAttackTime > 400)
		{
			ChangeState(EState::eFlyingAttack);
		}
	}
	if (mState == EState::eFlyingAttack || mState == EState::eFlyingEnd)
	{
		mFlyingAttackTime = 0;
	}

	if (mFlyingTime >= 600)
	{
		ChangeState(EState::eFlyingEnd);
	}
	if (mState == EState::eFlyingEnd || mState == EState::eFlyingStart && mFlyingTime >=400)
	{
		mFlyingTime = 1;
	}

	// HP��0�ȉ��ɂȂ�����A
	if (mCharaStatus.hp <= 0)
	{
		// ���S����
		Death();
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	CDebugPrint::Print(" (�n)�U������: %d\n", mAttackTime);
	CDebugPrint::Print(" �U������: %d\n",mFlyingAttackTime);
	CDebugPrint::Print(" �󒆎���: %d\n", mFlyingTime);
	CDebugPrint::Print(" HP: %d\n", mCharaStatus.hp);

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	 // �L���������߂��R���C�_�[
	mpColliderSphereFeet->Update();
	mpColliderSphereFeet2->Update();

	// �_���[�W���󂯂�R���C�_�[
	mpDamageColHead->Update();
	mpDamageColMouth->Update();
	mpDamageColTipMouth->Update();
	mpDamageColNeck->Update();
	mpDamageColChest->Update();
	mpDamageColFeet->Update();
	mpDamageColFeet2->Update();
	mpDamageColFeet3->Update();
	mpDamageColFeet4->Update();

	// �U���R���C�_�[
	mpAttackColHead->Update();
	mpAttackColMouth->Update();
	mpAttackColTipMouth->Update();

	mIsGrounded = false;

	if (CInput::PushKey(('Q')))
	{
		ChangeState(EState::eAttack2);
	}

	// �uE�v�L�[�ŉ��̔��˂��I���I�t����
	if (CInput::PushKey('Z'))
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
		else
		{
			mpFlamethrower->Stop();
		}
	}
	if (CInput::PushKey('C'))
	{
		if (!mpRoar->IsThrowing())
		{
			mpRoar->Start();
		}
		else
		{
			mpRoar->Stop();
		}
	}
}

// �Փˏ���
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColHead || self == mpAttackColMouth || self == mpAttackColTipMouth
		&& mState != EState::eIdle && mState != EState::eIdle2 &&
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
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// �L�����N�^�[���m�̏Փˏ���
	else if (self == mpColliderSphereFeet || self == mpColliderSphereFeet2)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CDragon::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	if (mState == EState::eAttack2)
	{

	}
	else if (mState == EState::eAttack3)
	{
		mpAttackColHead->SetEnable(true);
		mpAttackColMouth->SetEnable(true);
		mpAttackColTipMouth->SetEnable(true);
	}
}

// �U���I��
void CDragon::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackColHead->SetEnable(false);
	mpAttackColMouth->SetEnable(false);
	mpAttackColTipMouth->SetEnable(false);
}

// �`��
void CDragon::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CDragon::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CDragon::ChangeLevel(int level)
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
void CDragon::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HP����_���[�W������
	if (mCharaStatus.hp -= damage)
	{
		mpFlamethrower->Stop();
		ChangeState(EState::eHit);
	}
	// HP��0�ȉ��ɂȂ�����A
	if (mCharaStatus.hp <= 0)
	{
		// ���S����
		Death();
	}
}

// �h��͂̋����������擾
float CDragon::GetDefBuff(const CVector& attackDir)const
{
	// �h���Ԃł���΁A�h��2�{
	if (mState == EState::eDefense) return 2.0f;

	// �ʏ펞�̖h��̊���
	return mBaseDefenseBuffRatio;
}

// ���S����
void CDragon::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}