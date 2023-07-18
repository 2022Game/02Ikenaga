#include "CGameScene.h"
#include "CResourceManager.h"
#include "CStageManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CStage.h"
#include "CKokki.h"
#include "CTransparent.h"
#include "CFrame.h"
#include "CClear.h"
#include "CSceneManager.h"
//#include "CStageManager.h"

//�R���X�g���N�^
CGameScene::CGameScene()
	:CSceneBase(EScene::eGame)
{
}

//�f�X�g���N�^
CGameScene::~CGameScene()
{
}

//�V�[���ǂݍ���
void CGameScene::Load()
{
	//�����Ń^�C�g����ʂɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��
	LOAD_MODEL("Stage", "res\\�̈��.obj", "res\\�̈��.mtl");
	LOAD_MODEL("Kokki", "res\\����.obj", "res\\����.mtl");
	LOAD_MODEL("Enemy1", "res\\�G1.obj", "res\\�G1.mtl");
	LOAD_MODEL("Police", "res\\�T�{��|���X.obj", "res\\�T�{��|���X.mtl");
	LOAD_MODEL("Transparent", "res\\�����̕�.obj", "res\\�����̕�.mtl");
	LOAD_MODEL("Frame", "res\\�g�ΐF.obj", "res\\�g�ΐF.mtl");
	LOAD_MODEL("Clear", "res\\�N���A�g.obj", "res\\�N���A�g.mtl");
	LOAD_MODEL("Enemy2", "res\\�e�����G.obj", "res\\�e�����G.mtl");
	LOAD_MODEL("Player", "res\\obama4.obj", "res\\obama4.mtl");
	//LOAD_MODEL("BG", "res\\sky.obj", "res\\sky.mtl");

	AddTask(new CField());

	new CStage(GET_MODEL("Stage"), CVector(30.0f, -0.99f, 0.0f), CVector(), CVector(1.0f, 1.0f, 1.0f));
	CModel* kokki = GET_MODEL("Kokki");
	//new CKokki(&mKokki, CVector(-10.0f, 1.0f, 0.0f), CVector(0.0f, -90.0f, 0.0f), CVector(5.5f, 5.5f,0.5f));
	new CKokki(kokki, CVector(30.0f, 4.0f, -19.65f), CVector(0.0f, -90.0f, 0.0f), CVector(0.5f, 0.5f, 0.5f));
	//new CKokki(&mKokki, CVector(30.0f, -1.2f, -17.6f), CVector(0.0f, 0.0f, 0.0f), CVector(48.4f, 0.84f, 0.95f));
	new CKokki(kokki, CVector(23.55f, -0.81f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));//�����̓������猩�č��̃X�e�[�W���̊K�i
	new CKokki(kokki, CVector(23.25f, -0.99f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(22.95f, -1.14f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(22.65f, -1.31f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(22.35f, -1.48f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(23.25f, -0.2f, -15.38f), CVector(90.0f, 0.0f, 0.0f), CVector(8.0f, 0.0554f, 0.1f));//���̃X�e�[�W���̊K�i�̑O�̔�

	new CKokki(kokki, CVector(36.45f, -0.81f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));//�E�̃X�e�[�W���̊K�i
	new CKokki(kokki, CVector(36.75f, -0.99f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(37.05f, -1.14f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(37.35f, -1.31f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(37.65f, -1.48f, -15.8f), CVector(0.0f, 0.0f, 90.0f), CVector(5.0f, 0.1f, 0.2f));
	new CKokki(kokki, CVector(36.77, -0.654f, -15.38f), CVector(90.0f, 0.0f, 0.0f), CVector(8.0f, 0.0554f, 0.3f)); //�E�̃X�e�[�W���̊K�i�̑O�̔�

	CModel* tp = GET_MODEL("Transparent");
	new CTransparent(tp, CVector(30.0f, -2.648f, -17.6f), CVector(0.0f, 0.0f, 0.0f), CVector(48.4f, 0.84f, 0.95f));//-2.649
	new CTransparent(tp, CVector(22.15f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(18.0f, 0.84f, 0.8f));
	new CTransparent(tp, CVector(18.0f, -2.648f, -18.5f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 0.84f, 0.6f));//�������猩�č��̃X�e�[�W���̃h�A�t�ߏ㑤
	new CTransparent(tp, CVector(43.64f, 0.65f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(16.0f, 0.3f, 7.0f));//�����̕�(�E)
	new CTransparent(tp, CVector(17.39f, 0.65f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(16.0f, 0.3f, 7.0f)); //���̕�
	new CTransparent(tp, CVector(38.1f, 6.0f, 15.83f), CVector(90.0f, 0.0f, 0.0f), CVector(49.0f, 1.27f, 3.0f));//�����̉��̕�(�E)
	new CTransparent(tp, CVector(45.7f, 5.0f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(26.0f, 0.5f, 7.0f));//��K�̏��̕�(�E)
	new CTransparent(tp, CVector(44.75f, 2.4f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(5.5f, 0.8f, 7.0f));//��K�̏�(�E)
	new CTransparent(tp, CVector(15.7f, 5.0f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(26.0f, 0.5f, 7.0f));//��K�̏��̕�(��)
	new CTransparent(tp, CVector(18.0f, 2.4f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(5.5f, 0.8f, 7.0f));//��K�̏�(��)
	new CTransparent(tp, CVector(46.75f, 10.0f, 0.0f), CVector(0.0f, 0.0f, 90.0f), CVector(20.0f, 10.0f, 9.0f));//����
	new CTransparent(tp, CVector(21.9f, 6.0f, 15.83f), CVector(90.0f, 0.0f, 0.0f), CVector(49.0f, 1.27f, 3.0f));//�����̉��̕�(��)
	new CTransparent(tp, CVector(30.0f, 7.9f, 15.8f), CVector(90.0f, 0.0f, 0.0f), CVector(15.0f, 1.27f, 3.0f));//�����̉��̕�(�^��)	
	new CTransparent(tp, CVector(30.0f, 6.0f, -21.3f), CVector(90.0f, 0.0f, 0.0f), CVector(80.0f, 0.5f, 3.0f));//�X�e�[�W��
	new CTransparent(tp, CVector(25.75f, 6.0, -16.4f), CVector(0.0f, 0.0f, 90.0f), CVector(53.0f, 3.3f, 0.08));//�������猩�č��̃X�e�[�W���̃h�A�t��
	new CTransparent(tp, CVector(45.65f, 6.0, -16.4f), CVector(0.0f, 0.0f, 90.0f), CVector(53.0f, 3.3f, 0.08));//�������猩�ĉE�̃X�e�[�W�E�̃h�A�t��
	new CTransparent(tp, CVector(37.85f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(18.0f, 0.84f, 0.8f));
	new CTransparent(tp, CVector(42.0f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 5.0f, 0.8f));//�������猩�č��̃X�e�[�W���̃h�A�t�ߏ㑤
	new CTransparent(tp, CVector(18.0f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 5.0f, 0.8f));//�������猩�ĉE�̃X�e�[�W�E�̃h�A�t�ߏ㑤
	new CTransparent(tp, CVector(37.0f, 10.0, -16.25f), CVector(0.0f, 0.0f, 90.0f), CVector(33.0f, 4.0f, 0.04));//�������猩�Đ^�񒆂̃X�e�[�W
	new CTransparent(tp, CVector(30.0f, 0.4f, -17.1f), CVector(90.0f, 0.0f, 0.0f), CVector(11.3f, 0.26f, 0.3f));//������	

	//mEnemy2.Load(MODEL11);
	//new CEnemy2(&mEnemy2, CVector(-10.0f, -1.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f));
	//mHata.Load(MODEL5);
	//new CHATA(&mHata, CVector(30.0f, 100.0f, -19.65f), CVector(), CVector(5.5f, 5.5f, 0.5f));


	//mpBackGround = GET_MODEL("BG");
	//�r���{�[�h�̍쐬
	//new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͂Ȃ�
	//mColliderMesh.Set(nullptr, nullptr, mpBackGround);

	CStageManager::Instance()->LoadStage(0);
}

void CGameScene::Update()
{
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return;
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;  //���_�A�����_�A�����
	//���_�����߂�
	e = player->Position() + CVector(-0.2f, 3.0f, -5.0f) * player->MatrixRotate();
	//�����_�����߂�
	c = player->Position();
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f) * player->MatrixRotate();
	//�J�����̐ݒ�
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());

	if (player->IsDeath())
	{
		//CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}
}