#include "CTurtle.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "CShieldRotate2.h"
#include "CGameEnemyUI.h"
#include "CHit.h"
#include "Maths.h"

// �T�̃C���X�^���X
CTurtle* CTurtle::spInstance = nullptr;

#define ENEMY_HEIGHT    0.9f  // �����R���C�_�[
#define WITHIN_RANGE   40.0f  // �͈͓�
#define MOVE_SPEED      0.4f  // �ړ����x
#define GRAVITY         0.3f  // �d��
#define WALK_RANGE    100.0f  // �ǐՂ���͈�
#define STOP_RANGE     24.5f  // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f  // ��]����͈�

// �T�̃A�j���[�V�����f�[�^�̃e�[�u��
const CTurtle::AnimData CTurtle::ANIM_DATA[] =
{
	{ "",										                  true,	  0.0f,	 0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdleNormal.x",  true,	 51.0f,	 0.5f},  // �ҋ@
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdleBattle.x",  true,	 25.0f,	 0.5f},  // �ҋ@2
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdle.x",	      true,	 71.0f,	 0.5f},  // ���񂷑ҋ@
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdle2.x",       true,	 61.0f,	 0.5f},  // ���񂷑ҋ@2
	{ "Character\\Enemy\\Turtle\\animation\\TurtleAttack.x",	  false, 26.0f,	 0.5f},	 // �U��
	{ "Character\\Enemy\\Turtle\\animation\\TurtleAttack2.x",	  false, 26.0f,	 0.5f},	 // �U��2
	{ "Character\\Enemy\\Turtle\\animation\\TurtleGetHit.x",	  false, 26.0f,	 0.5f},	 // �q�b�g
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDefend.x",	  false, 18.0f,	 0.5f},	 // �h��
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDefendHit.x",	  false,  8.0f,	 0.3f},	 // �h�䒆�̃q�b�g
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDefendIdle.x",  true,	  8.0f,	 0.3f},	 // �h�䒆�̑ҋ@
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDie.x",	      false, 61.0f,	 0.3f},	 // ����
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDizzy.x",	      false, 41.0f,	 0.5f},	 // �߂܂�
	{ "Character\\Enemy\\Turtle\\animation\\TurtleRun.x",	      true,	 17.0f,	0.45f},  // �ړ�

};

// �R���X�g���N�^
CTurtle::CTurtle()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mDefenseTime(0)
	, mIsGrounded(false)
	, mMoveSpeed(CVector::zero)
	, mStateStep(0)
{
	mDefenseUp = false;

	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// �G�̎��
	mType = EEnemyType::eTurtle;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Turtle");

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

	// �L�����N�^�[�̐����R���C�_�[
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[�����߂�����(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.65f, false, 5.0f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy});
	mpColliderSphereBody->Position(0.3f, 0.5f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.65f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[��
	//�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eWeapon });
	// �_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageColBody->Position(0.3f, 0.5f, 0.0f);

	// �_���[�W��^����R���C�_�[(��)
	mpAttackColBody = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.65f, false
	);
	mpAttackColBody->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackColBody->SetCollisionTags({ ETag::ePlayer });
	mpAttackColBody->Position(0.3f, 0.5f, 0.0f);

	// �L�����N�^�[�����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[�ƍU���R���C�_�[���T�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMtx = GetFrameMtx("Armature_Body");
	mpColliderSphereBody->SetAttachMtx(bodyMtx);
	mpDamageColBody->SetAttachMtx(bodyMtx);
	mpAttackColBody->SetAttachMtx(bodyMtx);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackColBody->SetEnable(false);

	// �V�[���h�Ƃ̋���
	float ShieldDist = 20.0f;
	float height = 9.0f;
	// ��]����V�[���h
	mpShieldRotate = new CShieldRotate2(0.0f, ShieldDist, height);
	mpShieldRotate->Scale(2.0f, 2.0f, 2.0f);
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

	float Size = 18.0f;   // �T�C�Y
	float Height = 0.5f;  // ����
	// �q�b�g�G�t�F�N�g���쐬
	mpHitEffect = new CHit(Size, Height);
	mpHitEffect->SetOwner(this);
	mpHitEffect->Position(Position());
	mpHitEffect->SetShow(false);

	mpGameUI->SetUIoffSetPos(CVector(0.0f, 32.0f, 0.0f));

	// Lv.��ݒ�
	mpGameUI->SetLv();
	// ���x����ݒ�
	int level = 11;
	std::to_string(level);
	mpGameUI->SetEnemyLevel(level);
	// ���O��ݒ�
	std::string name = "�^�[�g��";
	mpGameUI->SetEnemyName(name);

	Scale(25.0f, 25.0f, 25.0f);
}

// �f�X�g���N�^
CTurtle::~CTurtle()
{
	// �L�����N�^�[�̐����R���C�_�[
	SAFE_DELETE(mpColliderLine);
	//�@�L�����N�^�[�̉����߂��R���C�_�[
	SAFE_DELETE(mpColliderSphereBody);
	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColBody);
	// �U���R���C�_�[
	SAFE_DELETE(mpAttackColBody);
	// ��]����V�[���h
	mpShieldRotate->Kill();
	mpShieldRotate2->Kill();
	mpShieldRotate3->Kill();
	mpShieldRotate4->Kill();
}

// �C���X�^���X
CTurtle* CTurtle::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CTurtle::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CTurtle::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
}

// �ҋ@���
void CTurtle::UpdateIdle()
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
void CTurtle::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);

	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();
	if (GetAnimationFrame() >= 10.0f && vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE && player->Position().Y() < 0.7f)
	{
		ChangeState(EState::eRun);
	}

	if (vectorPos <= 30.0f && player->Position().Y() >= 0.7f)
	{
		ChangeAnimation(EAnimType::eIdle2);
	}
}

// �ҋ@���3
void CTurtle::UpdateIdle3()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle3);

	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// �U��
void CTurtle::UpdateAttack()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// �U���I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);
}

// �U��2
void CTurtle::UpdateAttack2()
{
	SetAnimationSpeed(0.5f);

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0 : �U��2�A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack2);
		AttackStart();
		mStateStep++;
		break;
		// �X�e�b�v1 : �����߂��R���C�_���I�t
	case 1:
		if (mAnimationFrame >= 10.0f)
		{
			mpColliderSphereBody->SetEnable(false);
			mStateStep++;
		}
		break;
		// �X�e�b�v2 : �U���I���҂�
	case 2:
		if (mAnimationFrame >= 20.0f)
		{
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U���I���҂�
void CTurtle::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mpColliderSphereBody->SetEnable(true);
		ChangeState(EState::eIdle2);
	}
}

// �q�b�g
void CTurtle::UpdateHit()
{
	SetAnimationSpeed(0.5f);
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
			ChangeState(EState::eIdle2);
		}
	}
}

// �h��
void CTurtle::UpdateDefense()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDefense);
	mpAttackColBody->SetEnable(false);

	if (IsAnimationFinished())
	{
		ChangeState(EState::eDefenseIdle);
	}
}

// �h�䒆�̃q�b�g
void CTurtle::UpdateDefenseHit()
{
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eDefenseHit);
	mpAttackColBody->SetEnable(false);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eDefenseIdle);
	}
}

// �h�䒆�̑ҋ@
void CTurtle::UpdateDefenseIdle()
{
	SetAnimationSpeed(0.3f);
	ChangeAnimation(EAnimType::eDefenseIdle);
	mpAttackColBody->SetEnable(false);

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
		if (mDefenseTime >= 800)
		{
			ChangeState(EState::eAttack2);
		}
		else
		{
			ChangeState(EState::eDefenseIdle);
		}
	}
}

// ����
void CTurtle::UpdateDie()
{
	SetAnimationSpeed(0.3f);
	switch (mStateStep)
	{
	// �X�e�b�v0 : �A�j���[�V�����J�n
	case 0:
		ChangeAnimation(EAnimType::eDie);
		mStateStep++;
		break;
	// �X�e�b�v1 : �A�j���[�V�����I���҂�
	case 1:
		if (IsAnimationFinished())
		{
			Kill();
			// �G�l�~�[�̎��S����
			CEnemy::TurtleDeath();
		}
		break;
	}
}

// �߂܂�(����)
void CTurtle::UpdateDizzy()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eDizzy);

	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}
}

// �ړ�
void CTurtle::UpdateRun()
{
	SetAnimationSpeed(0.45f);
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector newPos = (player->Position() - Position()).Normalized();
	float vectorPos = (player->Position() - Position()).Length();

	// �͈͓��̎��A�ړ����ǐՂ���
	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE && player->Position().Y() < 0.7f)
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
	if (vectorPos <= 30.0f && player->Position().Y() >= 0.7f)
	{
		ChangeState(EState::eIdle2);
	}
	// �ǐՂ��~�܂������A�U���p�̑ҋ@���[�V������
	else if (vectorPos <= STOP_RANGE || vectorPos > WALK_RANGE)
	{
		ChangeState(EState::eIdle2);
	}
}

// �X�V����
void CTurtle::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
	mMoveSpeed.Z(0.0f);

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@
	case EState::eIdle:
		UpdateIdle();
		break;
		// �ҋ@2
	case EState::eIdle2:
		UpdateIdle2();
		break;
		// �ҋ@3
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
		// �h�䒆�̃q�b�g
	case EState::eDefenseIdle:
		UpdateDefenseIdle();
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

	CPlayer* player = CPlayer::Instance();
	float vectorPos = (player->Position() - Position()).Length();

	if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eDefenseIdle
		|| mState == EState::eHit)
	{
		mAttackTime++;

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
			// �U��2�̊m�����ŏ���2�ő�5
			int probability2 = Math::Rand(2, 4);
			// �h��̊m�����ŏ���5�ő�7
			int probability3 = Math::Rand(4, 6);

			if (probability2 == 2)Attack2 = true;
			if (probability3 == 4)Defense = true;

			if (Attack2)
			{
				ChangeState(EState::eAttack2);
			}
			else if (Defense && mState != EState::eDefenseIdle)
			{
				ChangeState(EState::eDefense);
			}
			else
			{
				ChangeState(EState::eAttack);
			}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eDefense)
		{
			mAttackTime = 0;
		}
		if (vectorPos >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
	}

	if (vectorPos > STOP_RANGE && vectorPos <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed);
	}

	// �h�䎞�Ԍv��
	if (mState == EState::eDefense || mState == EState::eDefenseHit || mState == EState::eDefenseIdle)
	{
		mDefenseTime++;
		mDefenseUp = true;
	}
	else
	{
		mDefenseTime = 0;
		mDefenseUp = false;
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �L�����̉����߂��R���C�_�[
	mpColliderSphereBody->Update();
	// �_���[�W���󂯂�R���C�_�[
	mpDamageColBody->Update();
	// �U���R���C�_�[
	mpAttackColBody->Update();

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
void CTurtle::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackColBody && mState != EState::eIdle && mState != EState::eIdle2 &&
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
	else if (self == mpColliderSphereBody)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CTurtle::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	if (mState == EState::eAttack || mState == EState::eAttack2)
	{
		mpAttackColBody->SetEnable(true);
	}
}

// �U���I��
void CTurtle::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackColBody->SetEnable(false);
}

// 1���x���A�b�v
void CTurtle::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CTurtle::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY3_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpGameUI->SetMaxHp(mCharaMaxStatus.hp);
	mpGameUI->SetHp(mCharaStatus.hp);
}

// ��_���[�W����
void CTurtle::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HP����_���[�W������
	if (mCharaStatus.hp -= damage)
	{
		if (mState == EState::eDefense || mState == EState::eDefenseIdle)
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
		Position(Position() - dir * Scale().X() * 0.01f);
	}
}

// �h��͂̋����������擾
float CTurtle::GetDefBuff(const CVector& attackDir)const
{
	// �h���Ԃł���΁A�h��2�{
	if (mState == EState::eDefense|| mState == EState::eDefenseIdle) return 2.0f;

	// �ʏ펞�̖h��̊���
	return mBaseDefenseBuffRatio;
}

// ���S����
void CTurtle::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}

// �����_���Ɉʒu���擾
CVector CTurtle::GetRandomSpawnPos()
{
	CVector pos = CVector::zero;
	pos.X(Math::Rand(-200.0f, 0.0f));
	pos.Y(-0.2f);
	pos.Z(Math::Rand(-350.0f, -200.0f));

	return CVector(pos);
}

// �`��
void CTurtle::Render()
{
	CXCharacter::Render();
}