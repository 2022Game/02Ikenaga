#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CBall.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CEnemy.h"

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
{
}

//�f�X�g���N�^
CGameScene::~CGameScene()
{
}

//�V�[���ǂݍ���
void CGameScene::Load()
{
	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��

	new CField();
	//new CBall();

	CBall* ball = new CBall();
	ball->Position(100.0f, 20.0f, 0.0f);
	ball->Scale(2.5f, 2.5f, 2.5f);

	CPlayer* player = new CPlayer();
	player->Scale(10.0f, 10.0f, 10.0f);

	CCamera* mainCamera = new CCamera
	(
		//CVector(5.0f, -15.0f, 180.0f),
		CVector(0.0f, 50.0f, 75.0f),
		player->Position()
	);
	mainCamera->SetFollowTargetTf(player);

	CEnemy* enemy = new CEnemy();
	enemy->Position(135.0f, 10.0f, 0.0f);
	enemy->Scale(10.0f, 10.0f, 10.0f);
}

//�V�[���̍X�V����
void CGameScene::Update()
{
}
