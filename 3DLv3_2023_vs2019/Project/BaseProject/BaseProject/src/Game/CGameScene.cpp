#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CSky.h"
#include "CNightSky.h"
#include "CPlayer.h"
#include "CSword.h"
#include "CShield.h"
#include "CEnemy.h"
#include "CSlime.h"
#include "CSlime2.h"
#include "CSlime3.h"
#include "CMushroom.h"
#include "CMushroom2.h"
#include "CMushroom3.h"
#include "CTurtle.h"
#include "CTurtle2.h"
#include "CTurtle3.h"
#include "CCactus.h"
#include "CCactus2.h"
#include "CCactus3.h"
#include "CBee.h"
#include "CBee2.h"
#include "CBee3.h"
#include "CRay.h"
#include "CRay2.h"
#include "CRay3.h"
#include "CBeholder.h"
#include "CChest.h"
#include "CChest2.h"
#include "CChest3.h"
#include "CBoxer.h"
#include "CBoxer2.h"
#include "CLich.h"
#include "CDragon.h"
#include "CPortionRed.h"
#include "CPortionGreen.h"
#include "CPortionBlue.h"
#include "CCamera.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CGear.h"
#include "Maths.h"

const CGameScene::SpawnData CGameScene::SPAWN_DATA[] =
{
	  // �G�̎�ށ@�@�@�@  ����   �I�t�Z�b�g�ʒu           �G�̑傫��  ��������
	{ EEnemyType::eSlime,  0.0f, CVector(0.0f,0.0f,0.0f),  30.0f,      0.0f },
};

// �R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpSpawnEnemy(nullptr)
	, SlimeReTime(0)
	, Slime2ReTime(0)
	, Slime3ReTime(0)
	, MushroomReTime(0)
	, Mushroom2ReTime(0)
	, Mushroom3ReTime(0)
	, TurtleReTime(0)
	, Turtle2ReTime(0)
	, Turtle3ReTime(0)  
	, mpGameMenu(nullptr)
{
}

// �f�X�g���N�^
CGameScene::~CGameScene()
{
}

// �V�[���ǂݍ���
void CGameScene::Load()
{
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��

	// �t�B�[���h�֘A
	CResourceManager::Load<CModel>("Field", "Field\\field.obj");
	CResourceManager::Load<CModel>("Sky", "Field\\Sky\\Sky.obj");
	CResourceManager::Load<CModel>("NightSky", "Field\\NightSky\\NightSky.obj");

	// �v���C���[�֘A
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\Dog.x");

	// �G�l�~�[�֘A
	CResourceManager::Load<CModelX>("Slime","Character\\Enemy\\Slime\\Slime.x");
	//CResourceManager::Load<CModelX>("Slime2", "Character\\Enemy\\Slime\\SlimeOrange.x");
	//CResourceManager::Load<CModelX>("Slime3", "Character\\Enemy\\Slime\\SlimeBlue.x");
	CResourceManager::Load<CModelX>("Mushroom", "Character\\Enemy\\Mushroom\\Mushroom.x");
	CResourceManager::Load<CModelX>("Turtle", "Character\\Enemy\\Turtle\\Turtle.x");
	CResourceManager::Load<CModelX>("Ray", "Character\\Enemy\\Ray\\Ray.x");
	CResourceManager::Load<CModelX>("Bee", "Character\\Enemy\\Bee\\Bee.x");
	CResourceManager::Load<CModelX>("Cactus", "Character\\Enemy\\Cactus\\Cactus.x");
	CResourceManager::Load<CModelX>("Chest", "Character\\Enemy\\Chest\\Chest.x");
	CResourceManager::Load<CModelX>("Beholder", "Character\\Enemy\\Beholder\\Beholder.x");
	CResourceManager::Load<CModelX>("Boxer", "Character\\Enemy\\Boxer\\Boxer.x");
	//CResourceManager::Load<CModelX>("Boxer2", "Character\\Enemy\\Boxer\\Boxer2.x");
	CResourceManager::Load<CModelX>("Lich", "Character\\Enemy\\Lich\\Lich.x");

	// �{�X�֘A
	//CResourceManager::Load<CModelX>("Dragon", "Character\\Enemy\\Dragon\\Dragon.x");

	// ����֘A
	CResourceManager::Load<CModel>("Sword", "Item\\Equipment\\Sword\\Sword.obj");
	CResourceManager::Load<CModel>("Shield", "Item\\Equipment\\Shield\\Shield.obj");
	CResourceManager::Load<CModel>("ShieldRotate", "Item\\Equipment\\Shield\\ShieldRotate.obj");

	// �A�C�e���֘A
	CResourceManager::Load<CModel>("Portion", "Item\\Portion\\PortionRed.obj");
	CResourceManager::Load<CModel>("Portion2", "Item\\Portion\\PortionGreen.obj");
	CResourceManager::Load<CModel>("Portion3", "Item\\Portion\\PortionBlue.obj");

	// UI�֘A
	CResourceManager::Load<CTexture>("Frame", "Character\\Player\\HP\\Frame.png");
	CResourceManager::Load<CTexture>("Gauge", "UI\\white.png");
	CResourceManager::Load<CTexture>("FrameEdge","Character\\Player\\HP\\FrameEdge.png");
	CResourceManager::Load<CTexture>("Gear", "UI\\Gear.png");

	// �G�t�F�N�g�֘A
	CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");
	CResourceManager::Load<CModel>("Slash", "Effect\\Slash\\Slash.obj");
	CResourceManager::Load<CModel>("Needle", "Effect\\Needle\\Needle.obj");
	CResourceManager::Load<CModel>("Coin", "Effect\\Coin\\Coin.obj");
	CResourceManager::Load<CModel>("Wave", "Effect\\Wave\\Wave.obj");
	CResourceManager::Load<CModel>("Roar", "Effect\\Roar.obj");
	CResourceManager::Load<CModel>("Tornado", "Effect\\Tornado\\Tornado.obj");
	CResourceManager::Load<CModel>("MagicCircle", "Effect\\MagicCircle\\MagicCircle.obj");
	CResourceManager::Load<CModel>("HealCircle", "Effect\\Circle\\Circle.obj");
	CResourceManager::Load<CModel>("BuffCircle", "Effect\\Circle2\\Circle2.obj");
	CResourceManager::Load<CTexture>("Current", "Effect\\Current\\lightning_bolt.png");

	// BGM�֘A
	CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");

	// �X���C���֘A�̌��ʉ�
	CResourceManager::Load<CSound>("SlimeRun", "Sound\\EnemySE\\Slime\\SlimeRun1.wav");
	CResourceManager::Load<CSound>("SlimeAttack", "Sound\\EnemySE\\Slime\\SlimeAttack.wav");
	CResourceManager::Load<CSound>("SlimeAttack2", "Sound\\EnemySE\\Slime\\SlimeAttack2.wav");
	CResourceManager::Load<CSound>("SlimeDizzy", "Sound\\EnemySE\\Slime\\SlimeDizzy.wav");
	CResourceManager::Load<CSound>("SlimeHit", "Sound\\EnemySE\\Slime\\SlimeHit.wav");
	CResourceManager::Load<CSound>("SlimeDie", "Sound\\EnemySE\\Slime\\SlimeDie.wav");

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eGame);

	CField* field = new CField();
	CField* field2 = new CField();
	field2->Position(0.0f, 0.0f, -1000.0f);
	CField* field3 = new CField();
	field3->Position(0.0f, 0.0f, 1000.0f);

	// ��
	CSky* sky = new CSky();
	sky->Scale(200.0f, 200.0f, 200.0f);
	sky->Position(0.0f, -1.5f, 0.0f);

	// ���
	CNightSky* nightSky = new CNightSky();
	nightSky->Scale(200.0f, 200.0f, 200.0f);
	nightSky->Position(0.0f, -1.5f, 0.0f);

	CPlayer* player = new CPlayer();

	/*CCamera* mainCamera = new CCamera
	(
		DEFAULT_CAMERA_POS,
		player->Position()
	);
	mainCamera->SetFollowTargetTf(player);*/

	// CGameCamera2�̃e�X�g
	CVector atPos = player->Position() + CVector(DEFAULT_CAMERA_POS);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(DEFAULT_CAMERA_POS),
		player->Position() + CVector(0.0f, 20.0f, 0.0f)
	);
	mainCamera->SetFollowTargetTf(player);
	mainCamera->AddCollider(field->GetCollider());
	mainCamera->AddCollider(field2->GetCollider());
	mainCamera->AddCollider(field3->GetCollider());
	mainCamera->SetHitColliderRatio(0.98f);
	player->Position(0.0f, 100.0f, 0.0f);

	// ���b�h�X���C��
	CSlime* enemy = new CSlime();
	enemy->Position(0.0f, 0.0f, -40.0f);
	enemy->Scale(25.0f, 25.0f, 25.0f);

	//// �I�����W�X���C��
	//CSlime2* enemy2 = new CSlime2();
	//enemy2->Position(-40.0f, 0.0f, -40.0f);
	//enemy2->Scale(25.0f, 25.0f, 25.0f);

	//// �u���[�X���C��
	//CSlime3* enemy3 = new CSlime3();
	//enemy3->Position(-20.0f, 0.0f, -90.0f);
	//enemy3->Scale(35.0f, 35.0f, 35.0f);

	// �}�b�V�����[��
	CMushroom* enemy4 = new CMushroom();
	enemy4->Position(150.0f, 0.0f, -150.0f);
	enemy4->Scale(25.0f, 25.0f, 25.0f);

	//// �}�b�V�����[��2
	//CMushroom2* enemy5 = new CMushroom2();
	//enemy5->Position(210.0f, 0.0f, -150.0f);
	//enemy5->Scale(25.0f, 25.0f, 25.0f);

	//// �}�b�V�����[��3
	//CMushroom3* enemy6 = new CMushroom3();
	//enemy6->Position(180.0f, 0.0f, -210.0f);
	//enemy6->Scale(35.0f, 35.0f, 35.0f);

	// �T
	CTurtle* enemy7 = new CTurtle();
	enemy7->Position(0.0f, -0.2f, -300.0f);
	enemy7->Scale(25.0f, 25.0f, 25.0f);

	//// �T2
	//CTurtle2* enemy8 = new CTurtle2();
	//enemy8->Position(-60.0f, -0.2f, -300.0f);
	//enemy8->Scale(25.0f, 25.0f, 25.0f);

	//// �T3
	//CTurtle3* enemy9 = new CTurtle3();
	//enemy9->Position(-30.0f, -0.2f, -360.0f);
	//enemy9->Scale(35.0f, 35.0f, 35.0f);

	// �G�C
	CRay* enemy10 = new CRay();
	enemy10->Position(150.0f, 0.0f, -450.0f);
	enemy10->Scale(35.0f, 35.0f, 35.0f);

	//// �G�C2
	//CRay2* enemy11 = new CRay2();
	//enemy11->Position(210.0f, 0.0f, -450.0f);
	//enemy11->Scale(35.0f, 35.0f, 35.0f);

	//// �G�C3
	//CRay3* enemy12 = new CRay3();
	//enemy12->Position(180.f,0.0f,-520.0f);
	//enemy12->Scale(45.0f, 45.0f, 45.0f);

	// �I
	CBee* enemy13 = new CBee();
	enemy13->Position(0.0f, 0.0f, -600.0f);
	enemy13->Scale(30.0f, 30.0f, 30.0f);

	//// �I2
	//CBee2* enemy14 = new CBee2();
	//enemy14->Position(-60.0f, 0.0f, -600.0f);
	//enemy14->Scale(30.0f, 30.0f, 30.0f);

	//// �I3
	//CBee3* enemy15 = new CBee3();
	//enemy15->Position(-30.0f, 0.0f, -660.0f);
	//enemy15->Scale(40.0f, 40.0f, 40.0f);

	// �T�{�e��
	CCactus* enemy16 = new CCactus();
	enemy16->Position(150.0f, 0.0f, -750.0f);
	enemy16->Scale(30.0f, 30.0f, 30.0f);

	//// �T�{�e��2
	//CCactus2* enemy17 = new CCactus2();
	//enemy17->Position(210.0f, 0.0f, -750.0f);
	//enemy17->Scale(30.0f, 30.0f, 30.0f);

	//// �T�{�e��3
	//CCactus3* enemy18 = new CCactus3();
	//enemy18->Position(180.0f, 0.0f, -810.0f);
	//enemy18->Scale(40.0f, 40.0f, 40.0f);

	// �`�F�X�g�����X�^�[
	CChest* enemy19 = new CChest();
	enemy19->Position(0.0f, 0.0f, -900.0f);
	enemy19->Scale(30.0f, 30.0f, 30.0f);

	//// �`�F�X�g�����X�^�[2
	//CChest2* enemy20 = new CChest2();
	//enemy20->Position(-60.0f, 0.0f, -900.0f);
	//enemy20->Scale(30.0f, 30.0f, 30.0f);

	//// �`�F�X�g�����X�^�[3
	//CChest3* enemy21 = new CChest3();
	//enemy21->Position(-30.0f, 0.0f, -960.0f);
	//enemy21->Scale(30.0f, 30.0f, 30.0f);

	// ���̂̃����X�^�[
	CBeholder* enemy22 = new CBeholder();
	enemy22->Position(150.0f, 0.0f, -1050.0f);
	enemy22->Scale(15.0f, 15.0f, 15.0f);

	// �{�N�T�[
	CBoxer* enemy25 = new CBoxer();
	//enemy25->Position(0.0f, 0.0f, -1200.0f);
	enemy25->Position(0.0f, 0.0f, 80.0f);
	enemy25->Scale(20.0f, 20.0f, 20.0f);

	//// �{�N�T�[2
	//CBoxer2* enemy27 = new CBoxer2();
	//enemy27->Position(370.0f, 0.0f, 20.0f);
	//enemy27->Scale(15.0f, 15.0f, 15.0f);

	// ���b�`
	CLich* enemy28 = new CLich();
	enemy28->Position(150.0f, 3.0f, -1350.0f);
	//enemy28->Position(0.0f, 3.0f, -200.0f);
	//enemy28->Rotate(0.0f,90.0f,-70.0f);
	enemy28->Scale(2.5f, 2.5f, 2.5f);

	//// �h���S��
	//CDragon* bossEnemy = new CDragon();
	//bossEnemy->Position(0.0f, 1.0f, -450.0f);
	//bossEnemy->Scale(15.0f, 15.0f, 15.0f);

	CGear* gear = new CGear(0.0f, 675.0f);

	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();
}

// �Q�[���N���A����
bool CGameScene::IsClear()
{
	return mpPlayer->mHp >= 10000;
}

// �Q�[���N���A����
void CGameScene::Clear()
{
	//CSceneManager::ClearInstance();
}

// �G�̃C���f�b�N�X�l���擾
int CGameScene::GetSpawnIndex() const
{
	int tableSize = ARRAY_SIZE(SPAWN_DATA);
	int index = Math::Rand(0, tableSize - 1);

	if (!(0 <= index && index < tableSize)) return -1;
	return index;
}

CEnemy* CGameScene::SpawnEnemy(EEnemyType type) const
{
	SpawnData data = SPAWN_DATA[mSpawnIndex];

	CEnemy* ret = nullptr;
	switch (type)
	{
	case EEnemyType::eBee:
		//ret = new CBee();
		break;
	case EEnemyType::eBeholder:
		//ret = new CBeholder();
		break;
	case EEnemyType::eBoxer:
		//ret = new CBoxer();
		break;
	case EEnemyType::eCactus:
		//ret = new CCactus();
		break;
	case EEnemyType::eChest:
		//ret = new CChest();
		break;
	case EEnemyType::eMushroom:
		//ret = new CMushroom();
		break;
	case EEnemyType::eRay:
		//ret = new CRay();
		break;
	case EEnemyType::eSlime:
	/*	if (data.SpawnTime >= 5.0f)
		{
			ret = new CSlime();
			data.SpawnTime = 0.0f;
		}*/
		break;
	case EEnemyType::eTurtle:
		//ret = new CTurtle();
		break;
	}
	return ret;
}

void CGameScene::UpdateSpawn()
{
	mSpawnIndex = GetSpawnIndex();
	SpawnData data = SPAWN_DATA[mSpawnIndex];
	// ��������G�𐶐�
	mpSpawnEnemy = SpawnEnemy(data.type);
	if (mpSpawnEnemy != nullptr)
	{
		mpSpawnEnemy->Scale(CVector::one * data.monsterScale);
	}
}

// �V�[���̍X�V����
void CGameScene::Update()
{
	// BGM�Đ����łȂ���΁ABGM���Đ�
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}

	mSpawnIndex = GetSpawnIndex();
	SpawnData data = SPAWN_DATA[mSpawnIndex];

	//// ���b�h�X���C���̕���
	//if (mpSlime->mHp <=0.0f)
	//{
	//	UpdateSpawn();
	//}

	//// �I�����W�X���C���̕���
	//if (mpSlime2->mHp <= 0)
	//{
	//	(Slime2ReTime++ / 100);
	//	if (Slime2ReTime > 500)
	//	{
	//		// �I�����W�X���C��
	//		CSlime2* enemy2 = new CSlime2();
	//		enemy2->Position(-40.0f, 0.0f, -40.0f);
	//		enemy2->Scale(25.0f, 25.0f, 25.0f);
	//		Slime2ReTime = 0;
	//	}
	//}

	////	�u���[�X���C���̕���
	//if (mpSlime3->mHp <= 0)
	//{
	//	(Slime3ReTime++ / 100);
	//	if (Slime3ReTime > 1000)
	//	{
	//		// �u���[�X���C��
	//		CSlime3* enemy3 = new CSlime3();
	//		enemy3->Position(-20.0f, 0.0f, -90.0f);
	//		enemy3->Scale(35.0f, 35.0f, 35.0f);
	//		Slime3ReTime = 0;
	//	}
	//}

	//// �}�b�V�����[���̕���
	//if (mpMushroom->mHp <= 0)
	//{
	//	(MushroomReTime++ / 100);
	//	if (MushroomReTime > 500)
	//	{
	//		// �}�b�V�����[��
	//		CMushroom* enemy4 = new CMushroom();
	//		enemy4->Position(150.0f, 0.0f, -150.0f);
	//		enemy4->Scale(25.0f, 25.0f, 25.0f);
	//		MushroomReTime = 0;
	//	}
	//}

	//// �}�b�V�����[��2�̕���
	//if (mpMushroom2->mHp <= 0)
	//{
	//	(Mushroom2ReTime++ / 100);
	//	if (Mushroom2ReTime > 500)
	//	{
	//		// �}�b�V�����[��2
	//		CMushroom2* enemy5 = new CMushroom2();
	//		enemy5->Position(210.0f, 0.0f, -150.0f);
	//		enemy5->Scale(25.0f, 25.0f, 25.0f);
	//		Mushroom2ReTime = 0;
	//	}
	//}

	//// �}�b�V�����[��3�̕���
	//if (mpMushroom3->mHp <= 0)
	//{
	//	(Mushroom3ReTime++ / 100);
	//	if (Mushroom3ReTime > 1000)
	//	{
	//		// �}�b�V�����[��3
	//		CMushroom3* enemy6 = new CMushroom3();
	//		enemy6->Position(180.0f, 0.0f, -210.0f);
	//		enemy6->Scale(35.0f, 35.0f, 35.0f);
	//		Mushroom3ReTime = 0;
	//	}
	//}

	//// �T�̕���
	//if (mpTurtle->mHp <=0)
	//{
	//	TurtleReTime++;
	//	if (TurtleReTime > 500)
	//	{
	//		// �T
	//		CTurtle* enemy7 = new CTurtle();
	//		enemy7->Position(0.0f, -0.2f, -300.0f);
	//		enemy7->Scale(25.0f, 25.0f, 25.0f);
	//		TurtleReTime = 0;
	//	}
	//}

	//// �T2�̕���
	//if (mpTurtle2->mHp <= 0)
	//{
	//	Turtle2ReTime++;
	//	if (Turtle2ReTime > 500)
	//	{
	//		// �T2
	//		CTurtle2* enemy8 = new CTurtle2();
	//		enemy8->Position(-60.0f, -0.2f, -300.0f);
	//		enemy8->Scale(25.0f, 25.0f, 25.0f);
	//		Turtle2ReTime = 0;
	//	}
	//}

	//// �T3�̕���
	//if (mpTurtle3->mHp <= 0)
	//{
	//	Turtle3ReTime++;
	//	if (Turtle3ReTime > 1000)
	//	{
	//		// �T3
	//		CTurtle3* enemy9 = new CTurtle3();
	//		enemy9->Position(-30.0f, -0.2f, -360.0f);
	//		enemy9->Scale(35.0f, 35.0f, 35.0f);
	//		Turtle3ReTime = 0;
	//	}
	//}
	//CDebugPrint::Print("���� %d\n", TurtleReTime);
	//CDebugPrint::Print("���� %d\n", Turtle2ReTime);
	//CDebugPrint::Print("���� %d\n", Turtle3ReTime);
}
