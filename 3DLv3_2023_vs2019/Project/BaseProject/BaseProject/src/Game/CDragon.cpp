#include "CDragon.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "Maths.h"
#include "CFlamethrower.h"
#include "CRoarEffect.h"

// �h���S���̃C���X�^���X
CDragon* CDragon::spInstance = nullptr;

#define ENEMY_HEIGHT -3.0f
#define WITHIN_RANGE 40.0f       // �͈͓�
#define MOVE_SPEED 0.08f         // �ړ����x
#define GRAVITY 0.0625f          // �d��
#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 24.0f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  50.0f      // ��]����͈�

// �h���S���̃A�j���[�V�����f�[�^�̃e�[�u��
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	{ "Character\\Enemy\\Dragon\\animation\\DragonSleep.x",	true,	162.0f	},	    // �Q�� 81.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonIdle.x",	true,	82.0f	},	    // �ҋ@ 41.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonIdle2.x",	true,	202.0f	},	    // �ҋ@ 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonAttack.x",	true,	162.0f	},	    // �U�� 81.0f
    { "Character\\Enemy\\Dragon\\animation\\DragonAttack2.x",	true,	182.0f	},	    // �U��2 91.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonAttack3.x",	true,	72.0f	},	    // �U��3 36.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyFlame.x",	true,	182.0f	},	    // �t���C�t���[�� 91.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonGetHit.x",	true,	82.0f	},	    // �q�b�g 41.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonDefend.x",	true,	122.0f	},	    // �h�� 61.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonDie.x",	true,	150.0f	},	        // ���� 65.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonScream.x",	true,	202.0f	},	    // �Y���� 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonTakeOff.x",	true,	242.0f	},	    // ��s�J�n����O 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonFlyIdle.x",	true,	62.0f	},	    // �t���C�A�C�h�� 31.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonLand.x",	true,	242.0f	},	        // ���n 121.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonRun.x",	true,	42.0f	},	        // ���� 21.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyForward.x",	true,	62.0f	},	// �t���C�t�H���[�h 31.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyGlide.x",	true,	51.0f	},	// �t���C�O���C�h 51.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonWalk.x",	true,	82.0f	},	        // ���� 41.0f
};

// �R���X�g���N�^
CDragon::CDragon()
	: mpRideObject(nullptr)
	,mFlyingTime(0)
	,mDefenseTime(0)
	, mAttackTime(0)
	,mFlyingAttackTime(0)
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
	//mpColliderLine->Position(0.0f, 0.0f,0.0f);

	//// �L�����N�^�[�����߂�����
	//mpColliderSphere = new CColliderSphere
	//(
	//	this, ELayer::eEnemy,
	//	0.3f, false, 5.0f
	//);
	//mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	//mpColliderSphere->Position(0.0f, 0.2f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,0.6f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol->Position(0.1f, 0.0f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageCol2 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.4f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol2->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol2->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol2->Position(-0.2f, 0.1f, 0.15f);

	// �_���[�W���󂯂�R���C�_�[���쐬(���̐�[)
	mpDamageCol3 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.3f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol3->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol3->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol3->Position(-0.1f, 0.15f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageCol4 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.6f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol4->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol4->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol4->Position(0.1f, 0.0f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��2)
	mpDamageCol5 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.6f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol5->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol5->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol5->Position(0.1f, 0.0f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��3)
	mpDamageCol6 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.7f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol6->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol6->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol6->Position(0.1f, 0.0f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageCol7 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.9f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol7->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol7->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol7->Position(-0.2f, 0.0f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�Ғ�)
	mpDamageCol8 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 1.0f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol8->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol8->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol8->Position(-0.15f, 0.0f, 0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�Ғ�2)
	mpDamageCol9 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.8f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol9->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol9->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol9->Position(0.0f, -0.4f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�Ȃ�)
	mpDamageCol10 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.6f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol10->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol10->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol10->Position(0.0f, -0.4f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�Ȃ�2)
	mpDamageCol11 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.4f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol11->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol11->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol11->Position(0.1f, 0.0f, -0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�Ȃ�3)
	mpDamageCol12 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.35f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol12->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol12->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol12->Position(0.1f, 0.0f, -0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�Ȃ�4)
	mpDamageCol13 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.25f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol13->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol13->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol13->Position(0.2f, 0.0f, -0.1f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�Ȃ�5)
	mpDamageCol14 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.2f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol14->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol14->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol14->Position(0.2f, 0.1f, 0.0f);

	// �J�v�Z���R���C�_�[
	// �_���[�W���󂯂�R���C�_�[���쐬(�O�̍���)
	mpDamageCol15 = new CColliderCapsule
	(
		this, ELayer::eDamageCol,
		CVector(-2.0f,1.0f, 0.0f),CVector(2.0f, 1.0f, 0.0f),
		2.0f,false,1.0f
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol15->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol15->SetCollisionTags({ ETag::eWeapon });
	
	//mpDamageCol15->Position(0.1f, 0.0f, 0.15f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�O�̍���2)
	mpDamageCol16 = new CColliderSphere
	(
		this, ELayer::eDamageCol, 0.3f,false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol16->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol16->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol16->Position(0.1f, 0.0f, 0.15f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,0.6f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.1f, 0.0f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���h���S���̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_Head");
	mpDamageCol->SetAttachMtx(headMty);
	mpAttackCol->SetAttachMtx(headMty);

	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���h���S���̌��̍s��ɃA�^�b�`
	const CMatrix* mouthMty = GetFrameMtx("Armature_UpperMouth01");
	mpDamageCol2->SetAttachMtx(mouthMty);

	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���h���S���̌��̐�[�̍s��ɃA�^�b�`
	const CMatrix* mouthMty2 = GetFrameMtx("Armature_UpperMouth02");
	mpDamageCol3->SetAttachMtx(mouthMty2);

	// �_���[�W���󂯂�R���C�_�[���h���S���̎�̍s��ɃA�^�b�`
	const CMatrix* neckMty = GetFrameMtx("Armature_Neck03");
	mpDamageCol4->SetAttachMtx(neckMty);

	// �_���[�W���󂯂�R���C�_�[���h���S���̎�2�̍s��ɃA�^�b�`
	const CMatrix* neckMty2 = GetFrameMtx("Armature_Neck02");
	mpDamageCol5->SetAttachMtx(neckMty2);

	// �_���[�W���󂯂�R���C�_�[���h���S���̎�3�̍s��ɃA�^�b�`
	const CMatrix* neckMty3 = GetFrameMtx("Armature_Neck01");
	mpDamageCol6->SetAttachMtx(neckMty3);

	// �_���[�W���󂯂�R���C�_�[���h���S���̋��̍s��ɃA�^�b�`
	const CMatrix* chestMty = GetFrameMtx("Armature_Chest");
	mpDamageCol7->SetAttachMtx(chestMty);

	// �_���[�W���󂯂�R���C�_�[���h���S���̐Ғł̍s��ɃA�^�b�`
	const CMatrix* spineMty = GetFrameMtx("Armature_Spine02");
	mpDamageCol8->SetAttachMtx(spineMty);

	// �_���[�W���󂯂�R���C�_�[���h���S���̐Ғ�2�̍s��ɃA�^�b�`
	const CMatrix* spineMty2 = GetFrameMtx("Armature_Spine01");
	mpDamageCol9->SetAttachMtx(spineMty2);

	// �_���[�W���󂯂�R���C�_�[���h���S���̂Ȃ��̍s��ɃA�^�b�`
	const CMatrix* jointMty = GetFrameMtx("Armature_joint19");
	mpDamageCol10->SetAttachMtx(jointMty);

	// �_���[�W���󂯂�R���C�_�[���h���S���̂Ȃ�2�̍s��ɃA�^�b�`
	const CMatrix* jointMty2 = GetFrameMtx("Armature_joint20");
	mpDamageCol11->SetAttachMtx(jointMty2);

	// �_���[�W���󂯂�R���C�_�[���h���S���̂Ȃ�3�̍s��ɃA�^�b�`
	const CMatrix* jointMty3 = GetFrameMtx("Armature_joint21");
	mpDamageCol12->SetAttachMtx(jointMty3);

	// �_���[�W���󂯂�R���C�_�[���h���S���̂Ȃ�4�̍s��ɃA�^�b�`
	const CMatrix* jointMty4 = GetFrameMtx("Armature_joint22");
	mpDamageCol13->SetAttachMtx(jointMty4);

	// �_���[�W���󂯂�R���C�_�[���h���S���̂Ȃ�5�̍s��ɃA�^�b�`
	const CMatrix* jointMty5 = GetFrameMtx("Armature_joint23");
	mpDamageCol14->SetAttachMtx(jointMty5);

	// �_���[�W���󂯂�R���C�_�[���h���S���̑O�̍����̍s��ɃA�^�b�`
	const CMatrix* leftFootMty = GetFrameMtx("Armature_Hand_L");
	mpDamageCol15->SetAttachMtx(leftFootMty);

	// �_���[�W���󂯂�R���C�_�[���h���S���̑O�̍���2�̍s��ɃA�^�b�`
	const CMatrix* leftFootMty2 = GetFrameMtx("Armature_Middle02_L");
	mpDamageCol16->SetAttachMtx(leftFootMty2);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackCol->SetEnable(false);

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
	//SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpDamageCol2);
	SAFE_DELETE(mpDamageCol3);
	SAFE_DELETE(mpDamageCol4);
	SAFE_DELETE(mpDamageCol5);
	SAFE_DELETE(mpDamageCol6);
	SAFE_DELETE(mpDamageCol7);
	SAFE_DELETE(mpDamageCol8);
	SAFE_DELETE(mpDamageCol9);
	SAFE_DELETE(mpDamageCol10);
	SAFE_DELETE(mpDamageCol11);
	SAFE_DELETE(mpDamageCol12);
	SAFE_DELETE(mpDamageCol13);
	SAFE_DELETE(mpDamageCol14);
	SAFE_DELETE(mpDamageCol15);
	SAFE_DELETE(mpDamageCol16);

	SAFE_DELETE(mpAttackCol);
}

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
}

// �ҋ@���
void CDragon::UpdateIdle()
{ 
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
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

// �ҋ@���3
void CDragon::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// �U��
void CDragon::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
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
	if (mAnimationFrame >= 35.0f)
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
	}
	if (mAnimationFrame >=140.0f)
	{
		mpFlamethrower->Stop();
	}
	if (IsAnimationFinished())
	{
		//mpFlamethrower->Stop();
		// �U���I���҂���Ԃֈڍs
		mState = EState::eAttackWait;
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
	mState = EState::eAttackWait;
}


// �U��3
void CDragon::UpdateAttack3()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack3);
	AttackStart();
	// �U��3�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U���I���҂�
void CDragon::UpdateAttackWait()
{
	mpFlamethrower->Stop();
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle2;
	}
}

// ��s���̍U��
void CDragon::UpdateFlyingAttack()
{
	ChangeAnimation(EAnimType::eFlyingAttack);
	if (mAnimationFrame >= 30.0f)
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
	}
	if (IsAnimationFinished())
	{
		mpFlamethrower->Stop();
		// ��s���̍U���I���҂���Ԃֈڍs
		mState = EState::eFlyingAttackWait;
	}
}

// ��s���̍U���I���҂�
void CDragon::UpdateFlyingAttackWait()
{
	mpFlamethrower->Stop();
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eFlyingIdle;
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
		mState = EState::eIdle2;
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
		mState = EState::eIdle2;
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
		// �G�l�~�[�̎��S����
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
		mState = EState::eFlyingStart;
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
		mState = EState::eFlyingIdle;
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
		mState = EState::eFlyingIdle;
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
		mState = EState::eIdle2;
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
		mState = EState::eIdle2;
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
		mState = EState::eIdle2;
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
				mState = EState::eAttack2;
			}
			else if (Attack)
			{
				mState = EState::eAttack;
			}
			else if (Defense)
			{
				mState = EState::eDefense;
			}
			else
			{
				mState = EState::eAttack3;
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
			mState = EState::eRoar;
		}
		if (mCharaStatus.hp <= mCharaMaxStatus.hp * 0.5 && mFlyingTime >=400)
		{
			mState = EState::eFlyingStart;
		}
	}

	if (mState == EState::eFlyingIdle)
	{
		mFlyingTime++;
		mFlyingAttackTime++;

		if (mFlyingAttackTime > 400)
		{
			mState = EState::eFlyingAttack;
		}
	}
	if (mState == EState::eFlyingAttack || mState == EState::eFlyingEnd)
	{
		mFlyingAttackTime = 0;
	}

	if (mFlyingTime >= 600)
	{
		mState = EState::eFlyingEnd;
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

	mpDamageCol->Update();
	mpDamageCol2->Update();
	mpDamageCol3->Update();
	mpDamageCol4->Update();
	mpDamageCol5->Update();
	mpDamageCol6->Update();
	mpDamageCol7->Update();
	mpDamageCol8->Update();
	mpDamageCol9->Update();
	mpDamageCol10->Update();
	mpDamageCol11->Update();
	mpDamageCol12->Update();
	mpDamageCol13->Update();
	mpDamageCol14->Update();
	mpDamageCol15->Update();
	mpDamageCol16->Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	if (CInput::PushKey(('Q')))
	{
		mState = EState::eAttack;
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
	if (self == mpAttackCol && mState != EState::eIdle && mState != EState::eIdle2 &&
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
			//mMoveSpeed.Y(0.0f);

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// �L�����N�^�[���m�̏Փˏ���
	/*else if (self == mpColliderSphere)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}*/
}

// �U���J�n
void CDragon::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
}

// �U���I��
void CDragon::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
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
	mCharaMaxStatus = ENEMY4_STATUS[index];
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
		mState = EState::eHit;
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
	mState = EState::eDie;
}