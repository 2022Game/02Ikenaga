#include "CPlayerStatusUI.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CGameMenu.h"
#include "CText.h"
#include "CPlayer.h"
#include "CPlayerLevelUI.h"
#include "CPlayerMaxExpUI.h"
#include "CPlayerExpUI.h"
#include "CPlayerMaxHpUI.h"
#include "CPlayerHpUI.h"
#include "CPlayerMaxSpUI.h"
#include "CPlayerSpUI.h"
#include "CPlayerPowerUI.h"
#include "CPlayerDefenseUI.h"
#include "CPlayerScaleUI.h"

#define LV_POS CVector2(220.0f, 180.0f)
#define EXP_POS CVector2(210.0f, 220.0f)
#define HP_POS CVector2(220.0f, 260.0f)
#define SP_POS CVector2(220.0f, 300.0f)
#define ATTACK_POS CVector2(200.0f, 340.0f)
#define DEFENSE_POS CVector2(200.0f, 380.0f)
#define SIZE_POS CVector2(200.0f, 420.0f)
#define DETAIL_POS CVector2(200.0f, 500.0f)
#define SKILL_POS CVector2(650.0f, 240.0f)
#define SKILL_POS2 CVector2(650.0f, 420.0f)

#define SHADOW_COUNT 8     // �e�̃J�E���g
#define SHADOW_WIDTH 2.0f  // �e�̕�
#define MENU_ALPHA 0.75f

// �R���X�g��
CPlayerStatusUI::CPlayerStatusUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mIsOpened(false)
{

	mpBackground = new CImage
	(
		"UI/menu_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(0.0f, 0.0f, 0.0f, MENU_ALPHA);

	mpStatus = new CImage
	(
		"UI/Text/status.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpStatus->SetPos(105.0f, 80.0f);
	mpStatus->SetSize(350.0f, 90.0f);

	mpBack = new CImage
	(
		"UI/back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBack->SetPos(140.0f, 600.0f);
	mpBack->SetSize(30.0f, 30.0f);

	mpShiftKey = new CImage
	(
		"UI/Key/shift.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpShiftKey->SetPos(185.0f, 590.0f);
	mpShiftKey->SetSize(55.0f, 55.0f);

	mpBuffSkill = new CImage
	(
		"UI/buffSkill.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBuffSkill->SetPos(820.0f, 140.0f);
	mpBuffSkill->SetSize(80.0f, 80.0f);

	mpSlashSkill = new CImage
	(
		"UI/slashSkill.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpSlashSkill->SetPos(820.0f, 320.0f);
	mpSlashSkill->SetSize(80.0f, 80.0f);

	CVector2 size = CVector2(WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5f);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// ���x���̃e�L�X�g���쐬
	mpLevelText = new CText
	(
		nullptr, 24,
		LV_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpLevelText->SetTextAlignH(textAlignH);
	mpLevelText->SetText("Lv.");

	// �o���l�̃e�L�X�g���쐬
	mpExpText = new CText
	(
		nullptr, 24,
		EXP_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpExpText->SetTextAlignH(textAlignH);
	mpExpText->SetText("Exp");

	// HP�̃e�L�X�g���쐬
	mpHpText = new CText
	(
		nullptr, 24,
		HP_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpHpText->SetTextAlignH(textAlignH);
	mpHpText->SetText("HP");

	// SP�̃e�L�X�g���쐬
	mpSpText = new CText
	(
		nullptr, 24,
		SP_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpSpText->SetTextAlignH(textAlignH);
	mpSpText->SetText("SP");

	// �U���͂̃e�L�X�g���쐬
	mpAttackText = new CText
	(
		nullptr, 24,
		ATTACK_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpAttackText->SetTextAlignH(textAlignH);
	mpAttackText->SetText("�U����");

	// �h��͂̃e�L�X�g���쐬
	mpDefenseText = new CText
	(
		nullptr, 24,
		DEFENSE_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpDefenseText->SetTextAlignH(textAlignH);
	mpDefenseText->SetText("�h���");

	// �傫���̃e�L�X�g���쐬
	mpSizeText = new CText
	(
		nullptr, 24,
		SIZE_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpSizeText->SetTextAlignH(textAlignH);
	mpSizeText->SetText("�傫��");

	// �����쐬
	mpLine = new CText
	(
		nullptr, 24,
		CVector2(410.0f, 220.0f),
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpLine->SetTextAlignH(textAlignH);
	mpLine->SetText("/");

	// ��2���쐬
	mpLine2 = new CText
	(
		nullptr, 24,
		CVector2(410.0f,260.0f),
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpLine2->SetTextAlignH(textAlignH);
	mpLine2->SetText("/");

	// ��3���쐬
	mpLine3 = new CText
	(
		nullptr, 24,
		CVector2(410.0f, 300.0f),
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpLine3->SetTextAlignH(textAlignH);
	mpLine3->SetText("/");

	// �ڍׂ̃e�L�X�g���쐬
	mpDetail = new CText
	(
		nullptr, 24,
		DETAIL_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpDetail->SetTextAlignH(textAlignH);
	mpDetail->SetText("���Ə��ŗl�X�ȍ�������z���X�L���Ŏ����������A�񕜂������̐g���ɂ߂Ă����B");

	// �X�L���̃e�L�X�g���쐬
	mpSkillText = new CText
	(
		nullptr, 24,
		SKILL_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpSkillText->SetTextAlignH(textAlignH);
	mpSkillText->SetText("�����_���ōU���̓A�b�v,�h��̓A�b�v,\nHP�񕜂�����B�iSP����-4�j");

	// �X�L��2�̃e�L�X�g���쐬
	mpSkillText2 = new CText
	(
		nullptr, 24,
		SKILL_POS2,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpSkillText2->SetTextAlignH(textAlignH);
	mpSkillText2->SetText("�O���Ɍ������Ďa������B�iSP����-5�j");

	// ���x��UI���쐬
	mpLevelUI = new CPlayerLevelUI(400.0f, 180.f);

	// �ő�o���l��UI���쐬
	mpMaxExpUI = new CPlayerMaxExpUI(455.0f, 220.f);

	// �o���l��UI���쐬
	mpExpUI = new CPlayerExpUI(350.0f, 220.f);

	// �ő�HP��UI���쐬
	mpMaxHpUI = new CPlayerMaxHpUI(445.0f, 260.f);

	// HP��UI���쐬
	mpHpUI = new CPlayerHpUI(340.0f, 260.f);

	// �ő�SP��UI���쐬
	mpMaxSpUI = new CPlayerMaxSpUI(455.0f, 300.f);

	// SP��UI���쐬
	mpSpUI = new CPlayerSpUI(350.0f, 300.f);

	// �U���͂�UI���쐬
	mpPowerUI = new CPlayerPowerUI(400.0f,340.0f);

	// �h��͂�UI���쐬
	mpDefenseUI = new CPlayerDefenseUI(400.0f, 380.0f);

	// �傫����UI���쐬
	mpScaleUI = new CPlayerScaleUI(360.0f, 420.0f);

	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CPlayerStatusUI::~CPlayerStatusUI()
{
	SAFE_DELETE(mpBackground);
	SAFE_DELETE(mpStatus);
	SAFE_DELETE(mpBack);
	SAFE_DELETE(mpShiftKey);
	SAFE_DELETE(mpBuffSkill);
	SAFE_DELETE(mpSlashSkill);
	SAFE_DELETE(mpLevelText);
	SAFE_DELETE(mpExpText);
	SAFE_DELETE(mpHpText);
	SAFE_DELETE(mpSpText);
	SAFE_DELETE(mpAttackText);
	SAFE_DELETE(mpDefenseText);
	SAFE_DELETE(mpSizeText);
	SAFE_DELETE(mpLine);
	SAFE_DELETE(mpLine2);
	SAFE_DELETE(mpLine3);
	SAFE_DELETE(mpDetail);
	SAFE_DELETE(mpSkillText);
	mpLevelUI->Kill();
	mpMaxExpUI->Kill();
	mpExpUI->Kill();
	mpMaxHpUI->Kill();
	mpHpUI->Kill();
	mpMaxSpUI->Kill();
	mpSpUI->Kill();
	mpPowerUI->Kill();
	mpDefenseUI->Kill();
	mpScaleUI->Kill();
}

// �\���ݒ�
void CPlayerStatusUI::SetShow(bool show)
{
	CTask::SetShow(show);

	mpLevelUI->SetShow(show);
	mpMaxExpUI->SetShow(show);
	mpExpUI->SetShow(show);
	mpMaxHpUI->SetShow(show);
	mpHpUI->SetShow(show);
	mpMaxSpUI->SetShow(show);
	mpSpUI->SetShow(show);
	mpPowerUI->SetShow(show);
	mpDefenseUI->SetShow(show);
	mpScaleUI->SetShow(show);
}

// ���݂̃v���C���[�̃X�e�[�^�X�𔽉f
void CPlayerStatusUI::ApplyPlayerStatus()
{
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr)return;

	const CharaStatus& status = player->Status();
	const CharaStatus& maxStatus = player->MaxStatus();

	// �ő�X�e�[�^�X�𔽉f
	SetMaxHp(maxStatus.hp);
	SetMaxSp(maxStatus.SpecialPoint);
	SetMaxExp(maxStatus.exp);

	// ���݂̃X�e�[�^�X�𔽉f
	SetHp(status.hp);
	SetSp(status.SpecialPoint);
	SetExp(status.exp);
	SetPower(status.power);
	SetDefense(status.defense);
	SetScale(status.volume);
	SetLevel(status.level);
}

// �I�[�v��
void CPlayerStatusUI::Open()
{
	SetEnable(true);
	SetShow(true);
	mIsOpened = true;

	ApplyPlayerStatus();
}

// �N���[�Y
void CPlayerStatusUI::Close()
{
	SetEnable(false);
	SetShow(false);

	CGameMenu::Instance()->Open();
}

// �I�[�v�����邩�ǂ���
bool CPlayerStatusUI::IsOpened() const
{
	return mIsOpened;
}

// ���߂�
void CPlayerStatusUI::Decide(int select)
{
	switch (select)
	{
	case 0:
		Close();
		break;
	case 1:
		break;
	}
}

// �v���C���[�̃��x����ݒ�
void CPlayerStatusUI::SetLevel(int level)
{
	mpLevelUI->SetLevel(level);
}

// �v���C���[�̍ő�o���l��ݒ�
void CPlayerStatusUI::SetMaxExp(int maxExp)
{
	mpMaxExpUI->SetMaxExp(maxExp);
}

// �v���C���[�̌o���l��ݒ�
void CPlayerStatusUI::SetExp(int exp)
{
	mpExpUI->SetExp(exp);
}

// �v���C���[�̍ő�HP��ݒ�
void CPlayerStatusUI::SetMaxHp(int maxHp)
{
	mpMaxHpUI->SetMaxHp(maxHp);
}

// �v���C���[��HP��ݒ�
void CPlayerStatusUI::SetHp(int hp)
{
	mpHpUI->SetHp(hp);
}

// �v���C���[�̍ő�SP��ݒ�
void CPlayerStatusUI::SetMaxSp(int maxSp)
{
	mpMaxSpUI->SetMaxSp(maxSp);
}

// �v���C���[��SP��ݒ�
void CPlayerStatusUI::SetSp(int sp)
{
	mpSpUI->SetSp(sp);
}

// �v���C���[�̍U���͂�ݒ�
void CPlayerStatusUI::SetPower (int power)
{
	mpPowerUI->SetPower(power);
}

// �v���C���[�̖h��͂�ݒ�
void CPlayerStatusUI::SetDefense(int defense)
{
	mpDefenseUI->SetDefense(defense);
}

// �v���C���[�̑傫����ݒ�
void CPlayerStatusUI::SetScale(float scale)
{
	mpScaleUI->SetScale(scale);
}

// �X�V
void CPlayerStatusUI::Update()
{
	mpBackground->Update();

	switch (mIsOpened)
	{
	case 0:
		Close();
		break;
	case 1:
		if (CInput::PushKey(VK_SHIFT))
		{
			mIsOpened = false;
		}
		break;
	}
}

// �`��
void CPlayerStatusUI::Render()
{
	mpBackground->Render();
	mpStatus->Render();
	mpBack->Render();
	mpShiftKey->Render();
	mpBuffSkill->Render();
	mpSlashSkill->Render();

	mpLevelText->SetColor(1.0f, 1.0f, 1.0f);
	mpExpText->SetColor(1.0f, 1.0f, 0.0f);
	mpHpText->SetColor(0.0f, 1.0f, 0.0f);
	mpSpText->SetColor(1.0f, 0.0f, 0.8f);
	mpAttackText->SetColor(1.0f, 0.0f, 0.0f);
	mpDefenseText->SetColor(0.0f, 1.0f, 1.0f);
	mpSizeText->SetColor(1.0f, 0.5f, 0.0f);
	mpLine->SetColor(1.0f, 1.0f, 1.0f);
	mpLine2->SetColor(1.0f, 1.0f, 1.0f);
	mpLine3->SetColor(1.0f, 1.0f, 1.0f);
	mpDetail->SetColor(1.0f, 1.0f, 1.0f);
	mpSkillText->SetColor(1.0f, 1.0f, 1.0f);
	mpSkillText2->SetColor(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		// ���x��
		mpLevelText->SetPos(LV_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpLevelText->Render();

		// �o���l
		mpExpText->SetPos(EXP_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpExpText->Render();

		// HP
		mpHpText->SetPos(HP_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpHpText->Render();

		// SP
		mpSpText->SetPos(SP_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpSpText->Render();

		// �U����
		mpAttackText->SetPos(ATTACK_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpAttackText->Render();

		// �h���
		mpDefenseText->SetPos(DEFENSE_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpDefenseText->Render();

		// �傫��
		mpSizeText->SetPos(SIZE_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpSizeText->Render();

		// ��
		mpLine->SetPos(CVector2(410.0f, 220.0f) + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpLine->Render();

		// ��2
		mpLine2->SetPos(CVector2(410.0f,260.0f) + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpLine2->Render();

		// ��3
		mpLine3->SetPos(CVector2(410.0f, 300.0f) + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpLine3->Render();

		// �ڍ�
		mpDetail->SetPos(DETAIL_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpDetail->Render();

		// �X�L��
		mpSkillText->SetPos(SKILL_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpSkillText->Render();

		// �X�L��2
		mpSkillText2->SetPos(SKILL_POS2 + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpSkillText2->Render();
	}
	mpLevelText->SetColor(0.0f, 0.0f, 0.0f);
	mpLevelText->SetPos(LV_POS);
	mpLevelText->Render();

	mpExpText->SetColor(0.0f, 0.0f, 0.0f);
	mpExpText->SetPos(EXP_POS);
	mpExpText->Render();

	mpHpText->SetColor(0.0f, 0.0f, 0.0f);
	mpHpText->SetPos(HP_POS);
	mpHpText->Render();

	mpSpText->SetColor(0.0f, 0.0f, 0.0f);
	mpSpText->SetPos(SP_POS);
	mpSpText->Render();

	mpAttackText->SetColor(0.0f, 0.0f, 0.0f);
	mpAttackText->SetPos(ATTACK_POS);
	mpAttackText->Render();

	mpDefenseText->SetColor(0.0f, 0.0f, 0.0f);
	mpDefenseText->SetPos(DEFENSE_POS);
	mpDefenseText->Render();

	mpSizeText->SetColor(0.0f, 0.0f, 0.0f);
	mpSizeText->SetPos(SIZE_POS);
	mpSizeText->Render();

	mpLine->SetColor(0.0f, 0.0f, 0.0f);
	mpLine->SetPos(CVector2(410.0f, 220.0f));
	mpLine->Render();

	mpLine2->SetColor(0.0f, 0.0f, 0.0f);
	mpLine2->SetPos(CVector2(410.0f, 260.0f));
	mpLine2->Render();

	mpLine3->SetColor(0.0f, 0.0f, 0.0f);
	mpLine3->SetPos(CVector2(410.0f, 300.0f));
	mpLine3->Render();

	mpDetail->SetColor(0.0f, 0.0f, 0.0f);
	mpDetail->SetPos(DETAIL_POS);
	mpDetail->Render();

	mpSkillText->SetColor(0.0f, 0.0f, 0.0f);
	mpSkillText->SetPos(SKILL_POS);
	mpSkillText->Render();

	mpSkillText2->SetColor(0.0f, 0.0f, 0.0f);
	mpSkillText2->SetPos(SKILL_POS2);
	mpSkillText2->Render();
}