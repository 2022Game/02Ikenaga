#include "CBoxer.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CHpGauge.h"
#include "CImpactEffect.h"
#include "CShieldRotate2.h"
#include "CGameEnemyUI.h"
#include "CHit.h"
#include "Maths.h"

// �{�N�T�[�̃C���X�^���X
CBoxer* CBoxer::spInstance = nullptr;

#define ENEMY_HEIGHT    1.0f   // �����R���C�_�[
#define WITHIN_RANGE   60.0f   // �͈͓�
#define MOVE_SPEED      0.8f   // �ړ����x
#define JUMP_SPEED      1.5f   // �W�����v�X�s�[�h
#define GRAVITY      0.0625f   // �d��
#define JUMP_END_Y      1.0f   // �W�����v�I��Y  
#define WALK_RANGE    100.0f   // �ǐՂ���͈�
#define STOP_RANGE     30.0f   // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f   // ��]����͈�

// �{�N�T�[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CBoxer::AnimData CBoxer::ANIM_DATA[] =
{
	{ "",										                true,	 0.0f,	 0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDance.x",	    true,	85.0f,	 0.5f},	 // �_���X
	{ "Character\\Enemy\\Boxer\\animation\\BoxerIdle.x",	    true,	21.0f,	 0.5f},  // �ҋ@
	{ "Character\\Enemy\\Boxer\\animation\\BoxerAttack.x",	    true,	19.0f,   0.5f},	 // �U��(�p���`)
	{ "Character\\Enemy\\Boxer\\animation\\BoxerAttack2.x",	    false,	26.0f,	 0.5f},	 // �U��2(�R��)
	{ "Character\\Enemy\\Boxer\\animation\\BoxerGetHit.x",	    false,	17.0f,   0.4f},	 // �q�b�g
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDefense.x",	    true,	21.0f,	 0.5f},	 // �h��
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDefenseHit.x",	false,	21.0f,	 0.5f},  // �h�䒆�̃q�b�g
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDie.x",	        false,	24.0f,	 0.2f},	 // ���� 24.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerDizzy.x",	    false,	24.0f,	 0.4f},	 // �߂܂� 24.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerJump.x",	    false,	23.0f,   0.5f},	 // �W�����v 23.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerJumpEnd.x",	    false,	26.0f,	 0.5f},	 // �W�����v�̏I�� 26.0f
	{ "Character\\Enemy\\Boxer\\animation\\BoxerRun.x",	        true,	21.0f, 	 0.5f},	 // ���� 21.0
	{ "Character\\Enemy\\Boxer\\animation\\BoxerSlide.x",	    false,	36.0f,   0.5f},	 // ���� 36.0
	//{ "Character\\Enemy\\Boxer\\animation\\BoxerDieRecover.x",	true,	48.0f	},	// �N���オ�� 24.0f
};

// �R���X�g���N�^
CBoxer::CBoxer()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mDefenseTime(0)
	, mStateStep(0)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// �G�̎��
	mType = EEnemyType::eBoxer;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Boxer");

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
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereHead = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.4f, false, 2.0f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHead->Position(0.04f, 0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.5f, false, 4.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereBody->Position(0.07f, 0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(�E��)
	mpColliderSphereHandR = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.27f, false, 3.0f
	);
	mpColliderSphereHandR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHandR->Position(0.0f, -0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(����)
	mpColliderSphereHandL = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.27f, false, 3.0f
	);
	mpColliderSphereHandL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereHandL->Position(0.0f, -0.1f, 0.0f);

	// �L�����N�^�[�����߂�����(�E��)
	mpColliderSphereFeetR = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.06f, false, 3.0f
	);
	mpColliderSphereFeetR->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeetR->Position(0.0f, 0.03f, 0.0f);

	// �L�����N�^�[�����߂�����(����)
	mpColliderSphereFeetL = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.06f, false, 3.0f
	);
	mpColliderSphereFeetL->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphereFeetL->Position(0.0f, 0.03f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.4f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eWeapon,ETag::eFlame});
	//�_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageColHead->Position(0.04f, 0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.5f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon,ETag::eFlame });
	mpDamageColBody->Position(0.07f, 0.0f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��)
	mpDamageColHandR = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.27f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHandR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHandR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHandR->Position(0.0f, -0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageColHandL = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.27f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHandL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHandL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColHandL->Position(0.0f, -0.1f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�E�r)
	mpDamageColArmR = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColArmR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColArmR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColArmR->Position(0.0f, -0.04f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(���r)
	mpDamageColArmL = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.35f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColArmL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColArmL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColArmL->Position(0.0f, -0.04f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(�E��)
	mpDamageColFeetR = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.1f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColFeetR->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeetR->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeetR->Position(0.0f, 0.25f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(����)
	mpDamageColFeetL = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.1f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[�ƏՓ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColFeetL->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColFeetL->SetCollisionTags({ ETag::eWeapon });
	mpDamageColFeetL->Position(0.0f, 0.25f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColHead = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.4f, false
	);
	mpAttackColHead->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHead->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHead->Position(0.04f, 0.1f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.5f, false
	);
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });
	mpAttackColBody->Position(0.07f, 0.0f, 0.0f);

	// �_���[�W��^����R���C�_�[(�E��)
	mpAttackColHandR = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.27f, false
	);
	mpAttackColHandR->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColHandR->SetCollisionTags({ ETag::ePlayer });
	mpAttackColHandR->Position(0.0f, -0.1f, 0.0f);

	// �_���[�W��^����R���C�_�[(�E��)
	mpAttackColFeetR = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.06f, false
	);
	mpAttackColFeetR->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColFeetR->SetCollisionTags({ ETag::ePlayer });
	mpAttackColFeetR->Position(0.0f, 0.03f, 0.0f);

	// �_���[�W��^����R���C�_�[(����)
	mpAttackColFeetL = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.06f, false
	);
	mpAttackColFeetL->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColFeetL->SetCollisionTags({ ETag::ePlayer });
	mpAttackColFeetL->Position(0.0f, 0.03f, 0.0f);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[��
	// �U���R���C�_�[���{�N�T�[�̓��̍s��ɃA�^�b�`
	const CMatrix* headMtx = GetFrameMtx("Armature_neck_01");
	mpColliderSphereHead->SetAttachMtx(headMtx);
	mpDamageColHead->SetAttachMtx(headMtx);
	mpAttackColHead->SetAttachMtx(headMtx);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[��
	// �U���R���C�_�[���{�N�T�[�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMtx = GetFrameMtx("Armature_spine_02");
	mpColliderSphereBody->SetAttachMtx(bodyMtx);
	mpDamageColBody->SetAttachMtx(bodyMtx);
	mpAttackColBody->SetAttachMtx(bodyMtx);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[��
	// �U���R���C�_�[���{�N�T�[�̉E��̍s��ɃA�^�b�`
	const CMatrix* rightHandMtx = GetFrameMtx("Armature_drill_r");
	mpColliderSphereHandR->SetAttachMtx(rightHandMtx);
	mpDamageColHandR->SetAttachMtx(rightHandMtx);
	mpAttackColHandR->SetAttachMtx(rightHandMtx);

	// �����߂��R���C�_�[�ƃ_���[�W���󂯂�R���C�_�[���{�N�T�[�̍���̍s��ɃA�^�b�`
	const CMatrix* leftHandMtx = GetFrameMtx("Armature_drill_l");
	mpColliderSphereHandL->SetAttachMtx(leftHandMtx);
	mpDamageColHandL->SetAttachMtx(leftHandMtx);

	// �_���[�W���󂯂�R���C�_�[���{�N�T�[�̉E�r�̍s��ɃA�^�b�`
	const CMatrix* rightArmMtx = GetFrameMtx("Armature_lowerarm_r");
	mpDamageColArmR->SetAttachMtx(rightArmMtx);

	// �_���[�W���󂯂�R���C�_�[���{�N�T�[�̍��r�̍s��ɃA�^�b�`
	const CMatrix* leftArmMtx = GetFrameMtx("Armature_lowerarm_l");
	mpDamageColArmL->SetAttachMtx(leftArmMtx);

	// �_���[�W���󂯂�R���C�_�[���{�N�T�[�̉E���̍s��ɃA�^�b�`
	const CMatrix* rightFootMtx = GetFrameMtx("Armature_thigh_r");
	mpDamageColFeetR->SetAttachMtx(rightFootMtx);

	// �_���[�W���󂯂�R���C�_�[���{�N�T�[�̍����̍s��ɃA�^�b�`
	const CMatrix* leftFootMtx = GetFrameMtx("Armature_thigh_l");
	mpDamageColFeetL->SetAttachMtx(leftFootMtx);

	// �����߂��R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̉E���̍s��ɃA�^�b�`
	const CMatrix* rightFootMtx2 = GetFrameMtx("Armature_foot_r");
	mpColliderSphereFeetR->SetAttachMtx(rightFootMtx2);
	mpAttackColFeetR->SetAttachMtx(rightFootMtx2);

	// �����߂��R���C�_�[�ƍU���R���C�_�[���{�N�T�[�̍����̍s��ɃA�^�b�`
	const CMatrix* leftFootMtx2 = GetFrameMtx("Armature_foot_l");
	mpColliderSphereFeetL->SetAttachMtx(leftFootMtx2);
	mpAttackColFeetL->SetAttachMtx(leftFootMtx2);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColHead->SetEnable(false);
	mpAttackColBody->SetEnable(false);
	mpAttackColHandR->SetEnable(false);
	mpAttackColFeetR->SetEnable(false);
	mpAttackColFeetL->SetEnable(false);

	// �Ռ����쐬
	const CMatrix* rightHandMty2 = GetFrameMtx("Armature_saw_r");
	mpImpact = new  CImpactEffect
	(
		this, rightHandMty2,
		CVector(0.0f, 0.0f, 0.0f),
		CQuaternion(0.0, 90.f, 0.0f).Matrix()
	);

	// �V�[���h�Ƃ̋���
	float ShieldDist = 20.0f;
	float height = 15.0f;
	// ��]����V�[���h
	mpShieldRotate = new CShieldRotate2(0.0f, ShieldDist, height);
	mpShieldRotate->Scale(2.0f, 2.0f, 2.0f);
	mpShieldRotate->Position(0.0f, 10.0f, 0.0f);
	mpShieldRotate->SetOwner(this);

	// ��]����V�[���h2
	mpShieldRotate2 = new CShieldRotate2(180.0, ShieldDist, height);
	mpShieldRotate2->Scale(2.0f, 2.0f, 2.0f);
	mpShieldRotate2->SetOwner(this);

	// ��]����V�[���h3
	mpShieldRotate3 = new CShieldRotate2(-270.0f, ShieldDist, height);
	mpShieldRotate3->Scale(2.0f, 2.0f, 2.0f);
	mpShieldRotate3->SetOwner(this);

	// ��]����V�[���h4
	mpShieldRotate4 = new CShieldRotate2(270.0f, ShieldDist, height);
	mpShieldRotate4->Scale(2.0f, 2.0f, 2.0f);
	mpShieldRotate4->SetOwner(this);

	float Size = 20.0f;   // �T�C�Y
	float Height = 0.8f;  // ����
	// �q�b�g�G�t�F�N�g���쐬
	mpHitEffect = new CHit(Size, Height);
	mpHitEffect->SetOwner(this);
	mpHitEffect->Position(Position());
	mpHitEffect->SetShow(false);

	mpGameUI->SetUIoffSetPos(CVector(0.0f, 37.0f, 0.0f));

	// Lv.��ݒ�
	mpGameUI->SetLv();
	// ���x����ݒ�
	int level = 71;
	std::to_string(level);
	mpGameUI->SetEnemyLevel(level);
	// ���O��ݒ�
	std::string name = "�E���� �{�u";
	mpGameUI->SetEnemyName(name);

	Scale(20.0f, 20.0f, 20.0f);
}

// �f�X�g���N�^
CBoxer::~CBoxer()
{
	// �����R���C�_�[
	SAFE_DELETE(mpColliderLine);
	// �L�����N�^�[�̉����߂��R���C�_�[
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	SAFE_DELETE(mpColliderSphereHandR);
	SAFE_DELETE(mpColliderSphereHandL);
	SAFE_DELETE(mpColliderSphereFeetR);
	SAFE_DELETE(mpColliderSphereFeetL);
	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBody);
	SAFE_DELETE(mpDamageColHandR);
	SAFE_DELETE(mpDamageColHandL);
	SAFE_DELETE(mpDamageColArmR);
	SAFE_DELETE(mpDamageColArmL);
	SAFE_DELETE(mpDamageColFeetR);
	SAFE_DELETE(mpDamageColFeetL);
	// �U���R���C�_�[
	SAFE_DELETE(mpAttackColHead);
	SAFE_DELETE(mpAttackColBody);
	SAFE_DELETE(mpAttackColHandR);
	SAFE_DELETE(mpAttackColFeetR);
	SAFE_DELETE(mpAttackColFeetL);
	// ��]����V�[���h
	mpShieldRotate->Kill();
	mpShieldRotate2->Kill();
	mpShieldRotate3->Kill();
	mpShieldRotate4->Kill();
}

// �C���X�^���X
CBoxer* CBoxer::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CBoxer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CBoxer::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// �ҋ@���
void CBoxer::UpdateIdle()
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
void CBoxer::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >=10.0f && vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
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

// �U��(�p���`)
void CBoxer::UpdateAttack()
{
	SetAnimationSpeed(0.5f);
	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mAnimationFrame <= 30.0f)
	{
		// �v���C���[�̂�������֌���
		CVector dir = CPlayer::Instance()->Position() - Position();
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
		mStateStep++;
		break;
		// �X�e�b�v1 : �U���J�n�{�C���p�N�g����
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			AttackStart();
			if (vectorPos >= 40.0f)
			{
				if (!mpImpact->IsThrowing())
				{
					mpImpact->Start();
					mStateStep++;
				}
			}
			else
			{
				mStateStep++;
			}
		}
		break;
		// �X�e�b�v2 : �U���I��
	case 2:
		if (mAnimationFrame >= 15.0f)
		{
			AttackEnd();
			mStateStep++;
		}
		break;
		// �X�e�b�v3 : �U���A�j���[�V�����I���҂�
	case 3:
		if (mAnimationFrame >= 17.0f)
		{
			AttackEnd();
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U��2(�R��)
void CBoxer::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);
	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	if (mAnimationFrame <= 30.0f)
	{
		// �v���C���[�̂�������֌���
		CVector dir = CPlayer::Instance()->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (vectorPos >= 0.0f && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += newPos * 1.0f;
	}

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0 : �U��2�A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		mStateStep++;
		break;
		// �X�e�b�v1 : �U���J�n
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
		// �X�e�b�v2 : �U��2�A�j���[�V�����I���҂�
	case 2:
		if (mAnimationFrame >= 24.0f)
		{
			AttackEnd();
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U���I���҂�
void CBoxer::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpImpact->Stop();

		// �A���U�����邩�ǂ���
		bool continuousz = false;
		// �m��
		int percent = Math::Rand(0, 5);
		if (percent == 1) continuousz = true;
		if (continuousz)
		{
			ChangeState(EState::eAttack);
		}
		else
		{
			ChangeState(EState::eIdle2);
		}
	}
}

// �q�b�g
void CBoxer::UpdateHit()
{
	SetAnimationSpeed(0.4f);
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
			ChangeState(EState::eDizzy);
		}
		else
		{
			// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
			ChangeState(EState::eIdle2);
		}
	}
}

// �h��
void CBoxer::UpdateDefense()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDefense);

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (vectorPos <= ROTATE_RANGE)
	{
		// �v���C���[�̂�������֌���
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (IsAnimationFinished())
	{
		if (mDefenseTime >= 200)
		{
			ChangeState(EState::eAttack);
		}
		else
		{
			ChangeState(EState::eDefense);
		}
	}
}

// �h�䒆�̃q�b�g
void CBoxer::UpdateDefenseHit()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDefenseHit);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eDefense);
	}
}

// ����
void CBoxer::UpdateDie()
{
	SetAnimationSpeed(0.2f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::BoxerDeath();
	}
}

// �߂܂�(����)
void CBoxer::UpdateDizzy()
{
	SetAnimationSpeed(0.4f);
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle2);
	}
}

// �W�����v��
void CBoxer::UpdateJump()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eJump);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eJumpEnd);
	}
}

// �W�����v�I��
void CBoxer::UpdateJumpEnd()
{
	// �W�����v�A�j���[�V�������ҋ@��Ԃ֖߂�
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// �ړ�
void CBoxer::UpdateRun()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// �͈͓��̎��A�ړ����ǐՂ���
	if (vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE)
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
	// �ǐՂ��~�܂������A�ҋ@���[�V������
	if (vectorPos <= STOP_RANGE || vectorPos >= WALK_RANGE)
	{
		if (mState != EState::eAttack2)
		{
			ChangeState(EState::eIdle2);
		}
	}
}

// ����
void CBoxer::UpdateSlide()
{
	SetAnimationSpeed(0.5f);
	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	if (mAnimationFrame <= 30.0f)
	{
		// �v���C���[�̂�������֌���
		CVector dir = CPlayer::Instance()->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (vectorPos >= 0.0f && vectorPos <= WALK_RANGE)
	{
		mMoveSpeed += newPos * 1.0f;
	}

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0 : ����A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eSlide);
		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 5.0f)
		{
			AttackStart();
			mStateStep++;
		}
		break;
	case 2:
		if (mAnimationFrame >= 34.0f)
		{
			// ����I���҂���Ԃֈڍs
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �X�V����
void CBoxer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
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
		// �W�����v��
	case EState::eJump:
		UpdateJump();
		break;
		// �W�����v�I��
	case EState::eJumpEnd:
		UpdateJumpEnd();
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

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eHit)
	{
		if (vectorPos <= 50.0f)
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
				ChangeState(EState::eAttack2);
			}
			else if (Defense)
			{
				ChangeState(EState::eDefense);
			}
			else if (Jump)
			{
				ChangeState(EState::eJump);
			}
			else if (Slide)
			{
				ChangeState(EState::eSlide);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eSlide
			|| mState == EState::eJump || mState == EState::eDefense || vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
		
	}

	// �h�䎞�Ԍv��
	if (mState == EState::eDefense || mState == EState::eDefenseHit)
	{
		mDefenseTime++;
		mDefenseUp = true;
	}
	else
	{
		mDefenseUp = false;
		mDefenseTime = 0;
	}

	if (vectorPos >= STOP_RANGE && vectorPos <= WALK_RANGE ||mState == EState::eAttack2 ||mState ==EState::eSlide)
	{
		Position(Position() + mMoveSpeed);
	}

	if (Position().Y() >= 0.1f)
	{
		Position(Position().X(), Position().Y() - 0.5f, Position().Z());
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �L�����N�^�[�̉����߂��R���C�_�[
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();
	mpColliderSphereHandR->Update();
	mpColliderSphereHandL->Update();
	mpColliderSphereFeetR->Update();
	mpColliderSphereFeetL->Update();
	// �_���[�W���󂯂�R���C�_�[
	mpDamageColHead->Update();
	mpDamageColBody->Update();
	mpDamageColHandR->Update();
	mpDamageColHandL->Update();
	mpDamageColArmR->Update();
	mpDamageColArmL->Update();
	mpDamageColFeetR->Update();
	mpDamageColFeetL->Update();
	// �U���R���C�_�[
	mpAttackColHead->Update();
	mpAttackColBody->Update();
	mpAttackColHandR->Update();
	mpAttackColFeetR->Update();
	mpAttackColFeetL->Update();

	mIsGrounded = false;
	
	CEnemy::Update();

	if (mState == EState::eIdle)
	{
		CHpGauge* hpGauge = mpGameUI->GetHpGauge();
		hpGauge->SetShow(false);
		CLevelUI* Lv = mpGameUI->GetLv();
		Lv->SetShow(false);
		CEnemyLevelUI* Level = mpGameUI->GetLevel();
		Level->SetShow(false);
		CEnemyNameUI* Name = mpGameUI->GetName();
		Name->SetShow(false);
	}
}

// �Փˏ���
void CBoxer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
				int damage = CalcDamage(1.0f,this, chara);

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
void CBoxer::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	if (mState == EState::eAttack)
	{
		mpAttackColHandR->SetEnable(true);
	}

	if (mState == EState::eAttack2)
	{
		mpAttackColFeetR->SetEnable(true);
		mpAttackColFeetL->SetEnable(true);
	}

	if (mState == EState::eSlide)
	{
		mpAttackColHead->SetEnable(true);
		mpAttackColBody->SetEnable(true);
	}
}

// �U���I��
void CBoxer::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackColHead->SetEnable(false);
	mpAttackColBody->SetEnable(false);
	mpAttackColHandR->SetEnable(false);
	mpAttackColFeetR->SetEnable(false);
	mpAttackColFeetL->SetEnable(false);
}

// 1���x���A�b�v
void CBoxer::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CBoxer::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY9_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpGameUI->SetMaxHp(mCharaMaxStatus.hp);
	mpGameUI->SetHp(mCharaStatus.hp);
}

// ��_���[�W����
void CBoxer::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HP����_���[�W������
	if (mCharaStatus.hp -= damage)
	{
		if (mState == EState::eDefense)
		{
			if (mState != EState::eDie)
			{
				mpHitEffect->StartHitEffect();
			}
			ChangeState(EState::eDefenseHit);
		}
		else
		{
			if (mState != EState::eDie)
			{
				mpHitEffect->StartHitEffect();
			}
			ChangeState(EState::eHit);
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
		Position(Position() - dir * Scale().X() * 0.1f);
	}
}

// �h��͂̋����������擾
float CBoxer::GetDefBuff(const CVector& attackDir)const
{
	// �h���Ԃł���΁A�h��2�{
	if (mState == EState::eDefense) return 2.0f;

	// �ʏ펞�̖h��̊���
	return mBaseDefenseBuffRatio;
}

// ���S����
void CBoxer::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}

// �����_���Ɉʒu���擾
CVector CBoxer::GetRandomSpawnPos()
{
	CVector pos = CVector::zero;
	pos.X(Math::Rand(-200.0f, 0.0f));
	pos.Z(Math::Rand(-1250.0f, -1100.0f));

	return CVector(pos);
}

// �`��
void CBoxer::Render()
{
	CXCharacter::Render();
}