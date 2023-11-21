#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CSword.h"
#include "CShield.h"
#include "CCamera.h"
#include "CSlime.h"
#include "CSlime2.h"
#include "CHpGauge.h"
#include "CEnemy2.h"
#include "CPortionRed.h"
#include "CPortionGreen.h"
#include "CPortionBlue.h"
#include "CExp.h"

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	,RevivalTime(0)
{
}

//�f�X�g���N�^
CGameScene::~CGameScene()
{
}

//�V�[���ǂݍ���
void CGameScene::Load()
{
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��

	new CField();

	CPlayer* player = new CPlayer();

	CCamera* mainCamera = new CCamera
	(
		DEFAULT_CAMERA_POS,
		player->Position()
	);
	mainCamera->SetFollowTargetTf(player);

	CSlime* enemy = new CSlime();
	enemy->Position(0.0f, 0.0f, -20.0f);
	enemy->Scale(25.0f, 25.0f, 25.0f);

	CSlime2* enemy2 = new CSlime2();
	enemy2->Position(-40.0f, 0.0f, -20.0f);
	enemy2->Scale(25.0f, 25.0f, 25.0f);

	//HP�Q�[�W���쐬
	/*CHpGauge* hpgauge = new CHpGauge();
	hpgauge->SetPos(10.0f, 190.0f);*/

	CEnemy2* enemy5 = new CEnemy2();
	enemy5->Position(60.0f, 0.0f, 20.0f);
	enemy5->Scale(20.0f, 20.0f, 20.0f);

	CPortionRed* portionred = new CPortionRed();
	portionred->Position(90.0f, 10.0f, 0.0f);
	portionred->Scale(70.0f, 70.0f, 70.0f);

	CPortionGreen* portiongreen = new CPortionGreen();
	portiongreen->Position(80.0f, 10.0f, 0.0f);
	portiongreen->Scale(70.0f, 70.0f, 70.0f);

	CPortionBlue* portionblue = new CPortionBlue();
	portionblue->Position(70.0f, 10.0f, 0.0f);
	portionblue->Scale(70.0f, 70.0f, 70.0f);

	CExp* exp = new CExp();
	exp->Position(50.0f, 5.0f, 0.0f);
	exp->Scale(4.0f, 4.0f, 4.0f);
}

//�V�[���̍X�V����
void CGameScene::Update()
{
	if (mpSlime->mHp <= 0)
	{
		(RevivalTime++/100);
		if (RevivalTime > 1000)
		{
			CSlime* enemy = new CSlime();
			enemy->Position(0.0f, 0.0f, -20.0f);
			enemy->Scale(25.0f, 25.0f, 25.0f);
			RevivalTime = 0;
		}
	}
	if (mpSlime2->mHp <= 0)
	{
		(RevivalTime++ / 100);
		if (RevivalTime > 1000)
		{
			CSlime2* enemy2 = new CSlime2();
			enemy2->Position(-40.0f, 0.0f, -20.0f);
			enemy2->Scale(25.0f, 25.0f, 25.0f);
			RevivalTime = 0;
		}
	}
	CDebugPrint::Print(" ��������: %d\n", RevivalTime / 100);
}
