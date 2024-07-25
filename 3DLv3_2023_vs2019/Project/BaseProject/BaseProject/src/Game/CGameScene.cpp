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
#include "CTurtle.h"
#include "CCactus.h"
#include "CBee.h"
#include "CRay.h"
#include "CBeholder.h"
#include "CChest.h"
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
#include "CMenu.h"
#include "CKeyM.h"
#include "CKeyQ.h"
#include "CKeyE.h"
#include "CBuffSkill.h"
#include "CSlashSkill.h"
#include "CLockWall.h"
#include "CLockWall2.h"
#include "CEnemyManager.h"
#include "CLock.h"
#include "CLock2.h"
#include "Maths.h"

// �R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
{
	CEnemyManager::Create();
}

// �f�X�g���N�^
CGameScene::~CGameScene()
{
	CGameMenu::ClearInstance();

	CEnemyManager::Delete();
}

// �V�[���ǂݍ���
void CGameScene::Load()
{
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);
	CInput::ShowCursor(false);

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
	CResourceManager::Load<CModelX>("Slime2", "Character\\Enemy\\Slime\\SlimeOrange.x");
	CResourceManager::Load<CModelX>("Slime3", "Character\\Enemy\\Slime\\SlimeBlue.x");
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
	CResourceManager::Load<CModelX>("Dragon", "Character\\Enemy\\Dragon\\Dragon.x");

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
	CResourceManager::Load<CTexture>("Menu", "UI\\menu.png");
	CResourceManager::Load<CTexture>("KeyM", "UI\\Key\\mKey.png");
	CResourceManager::Load<CTexture>("KeyQ", "UI\\Key\\qKey.png");
	CResourceManager::Load<CTexture>("KeyE", "UI\\Key\\eKey.png");
	CResourceManager::Load<CTexture>("BuffSkill", "UI\\buffSkill.png");
	CResourceManager::Load<CTexture>("SlashSkill", "UI\\slashSkill.png");
	CResourceManager::Load<CTexture>("Wall", "UI\\black.png");
	CResourceManager::Load<CTexture>("Lock", "UI\\Lock.png");

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

	// �o�t�X�L�����쐬
	CBuffSkill* buhhSkill = new CBuffSkill(990.0f, 619.0f);
	// �X���b�V���X�L���쐬
	CSlashSkill* slashSkill = new CSlashSkill(1120.0f, 619.0f);
	// ���b�N���̕�
	CLockWall* lockWall = new CLockWall(990.0f, 619.0f);
	// ���b�N���̕�2
	CLockWall2* lockWall2 = new CLockWall2(1120.0f, 619.0f);
	// ���b�N
	CLock* lock = new CLock(1010.0f, 630.0f);
	// ���b�N
	CLock2* lock2 = new CLock2(1140.0f, 630.0f);

	CField* field = new CField();

	// ��
	CSky* sky = new CSky();
	sky->Scale(400.0f, 400.0f, 400.0f);
	sky->Position(0.0f, -1.5f, 0.0f);

	// ���
	CNightSky* nightSky = new CNightSky();
	nightSky->Scale(400.0f, 400.0f, 400.0f);
	nightSky->Position(0.0f, -1.5f, 0.0f);

	CPlayer* player = new CPlayer();

	// CGameCamera2�̃e�X�g
	CVector atPos = player->Position() + CVector(DEFAULT_CAMERA_POS);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(DEFAULT_CAMERA_POS),
		player->Position() + CVector(0.0f, 20.0f, 0.0f)
	);
	mainCamera->SetFollowTargetTf(player);
	mainCamera->AddCollider(field->GetCollider());
	mainCamera->SetHitColliderRatio(0.98f);
	player->Position(0.0f, 100.0f, 0.0f);

	// ���b�h�X���C��
	CSlime* enemy = new CSlime();
	enemy->Position(0.0f, 0.0f, -40.0f);

	// �I�����W�X���C��
	CSlime2* enemy2 = new CSlime2();
	enemy2->Position(-40.0f, 0.0f, -40.0f);
	enemy2->Scale(25.0f, 25.0f, 25.0f);

	// �u���[�X���C��
	CSlime3* enemy3 = new CSlime3();
	enemy3->Position(-20.0f, 0.0f, -90.0f);

	// �}�b�V�����[��
	CMushroom* enemy4 = new CMushroom();
	enemy4->Position(150.0f, 0.0f, -150.0f);

	// �T
	CTurtle* enemy7 = new CTurtle();
	enemy7->Position(0.0f, -0.5f, -300.0f);

	// �G�C
	CRay* enemy10 = new CRay();
	enemy10->Position(150.0f, 0.0f, -450.0f);

	// �I
	CBee* enemy13 = new CBee();
	enemy13->Position(0.0f, 0.0f, -600.0f);

	// �T�{�e��
	CCactus* enemy16 = new CCactus();
	enemy16->Position(150.0f, 0.0f, -750.0f);

	// �`�F�X�g�����X�^�[
	CChest* enemy19 = new CChest();
	enemy19->Position(0.0f, 0.0f, -900.0f);

	// ���̂̃����X�^�[
	CBeholder* enemy22 = new CBeholder();
	enemy22->Position(150.0f, 0.0f, -1050.0f);

	// �{�N�T�[
	CBoxer* enemy25 = new CBoxer();
	enemy25->Position(0.0f, 0.0f, -1200.0f);

	//// �{�N�T�[2
	//CBoxer2* enemy27 = new CBoxer2();
	//enemy27->Position(370.0f, 0.0f, 20.0f);
	//enemy27->Scale(15.0f, 15.0f, 15.0f);

	// ���b�`
	CLich* enemy28 = new CLich();
	enemy28->Position(75.0f, 3.0f, -1350.0f);

	// �h���S��
	CDragon* bossEnemy = new CDragon();
	bossEnemy->Position(75.0f, 1.0f, -1700.0f);

	// ���j���[�A�C�R�����쐬
	CMenu* menu = new CMenu(10.0f, 680.0f);
	//M�L�[���쐬
	CKeyM* keyM = new CKeyM(40.0f, 670.0f);
	// Q�L�[���쐬
	CKeyQ* keyQ = new CKeyQ(1014.0f, 560.0f);
	// E�L�[���쐬
	CKeyE* keyE = new CKeyE(1144.0f, 560.0f);

	// �Q�[�����j���[���쐬
	CGameMenu::Instance();
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

// �V�[���̍X�V����
void CGameScene::Update()
{
	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	CGameMenu* menu = CGameMenu::Instance();
	if (!menu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			menu->Open();
		}
	}
}
