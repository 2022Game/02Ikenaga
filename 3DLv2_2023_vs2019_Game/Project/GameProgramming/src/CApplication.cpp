#include "CApplication.h"
//OpenGL
#include"glut.h"
#include "CRectangle.h"
#include"CVector.h"
#include"CBillBoard.h"
#include "CResourceManager.h"
#include "CStageManager.h"

//#define TEXTYA "AvatarHeadMale_Color_2K.jpeg"

#define SOUND_BGM "res\\mario.wav" //BGM�����t�@�C��
#define SOUND_OVER "res\\mdai.wav" //�Q�[���I�[�o�[�����t�@�C��

CApplication::~CApplication()
{
	delete spUi;  //�C���X�^���XUi�̍폜
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui() 
{
	return spUi;  //�C���X�^���X�̃|�C���^��Ԃ�
}

CMatrix  CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

void CApplication::Start()
{
	LOAD_MODEL("Stage", "res\\�̈��.obj", "res\\�̈��.mtl");
	LOAD_MODEL("Kokki", "res\\����.obj", "res\\����.mtl");
	LOAD_MODEL("Enemy1", "res\\�G1.obj", "res\\�G1.mtl");
	LOAD_MODEL("Police", "res\\�T�{��|���X.obj", "res\\�T�{��|���X.mtl");
	LOAD_MODEL("Transparent", "res\\�����̕�.obj", "res\\�����̕�.mtl");
	LOAD_MODEL("Frame", "res\\�g�ΐF.obj", "res\\�g�ΐF.mtl");
	LOAD_MODEL("Clear", "res\\�N���A�g.obj", "res\\�N���A�g.mtl");
	LOAD_MODEL("Enemy2", "res\\�e�����G.obj", "res\\�e�����G.mtl");
	LOAD_MODEL("Player", "res\\obama4.obj", "res\\obama4.mtl");
	LOAD_MODEL("BG", "res\\sky.obj", "res\\sky.mtl");

	spUi = new CUi();  //UI�N���X�̐���
	spUi->Hp(CPlayer::Hp());

	new CStage(GET_MODEL("Stage"), CVector(30.0f, -0.99f, 0.0f), CVector(), CVector(1.0f, 1.0f, 1.0f));
	CModel* kokki = GET_MODEL("Kokki");
	//new CKokki(&mKokki, CVector(-10.0f, 1.0f, 0.0f), CVector(0.0f, -90.0f, 0.0f), CVector(5.5f, 5.5f,0.5f));
	new CKokki(kokki, CVector(30.0f, 4.0f, -19.65f), CVector(0.0f, -90.0f, 0.0f), CVector(0.5f, 0.5f,0.5f));
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

	CModel*tp= GET_MODEL("Transparent");
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
	new CTransparent(tp, CVector(25.75f, 6.0, -16.4f), CVector(0.0f, 0.0f, 90.0f), CVector(53.0f,3.3f, 0.08));//�������猩�č��̃X�e�[�W���̃h�A�t��
	new CTransparent(tp, CVector(45.65f, 6.0, -16.4f), CVector(0.0f, 0.0f, 90.0f), CVector(53.0f, 3.3f, 0.08));//�������猩�ĉE�̃X�e�[�W�E�̃h�A�t��
	new CTransparent(tp, CVector(37.85f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(18.0f, 0.84f, 0.8f));
	new CTransparent(tp, CVector(42.0f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 5.0f, 0.8f));//�������猩�č��̃X�e�[�W���̃h�A�t�ߏ㑤
	new CTransparent(tp, CVector(18.0f, -2.648f, -18.1f), CVector(0.0f, 0.0f, 0.0f), CVector(19.0f, 5.0f, 0.8f));//�������猩�ĉE�̃X�e�[�W�E�̃h�A�t�ߏ㑤
	new CTransparent(tp, CVector(37.0f, 10.0, -16.25f), CVector(0.0f, 0.0f, 90.0f), CVector(33.0f, 4.0f, 0.04));//�������猩�Đ^�񒆂̃X�e�[�W
	new CTransparent(tp, CVector(30.0f, 0.4f, -17.1f), CVector(90.0f, 0.0f, 0.0f), CVector(11.3f,0.26f, 0.3f));//������	


	CModel* frameModel = GET_MODEL("Frame");
	new CFrame(frameModel, CVector(30.0f, -0.9f, 30.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	CModel* clearModel = GET_MODEL("Clear");
	new CClear(clearModel, CVector(30.0f, -0.9f, 30.0f), CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	//mEnemy2.Load(MODEL11);
	//new CEnemy2(&mEnemy2, CVector(-10.0f, -1.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f));
	//mHata.Load(MODEL5);
	//new CHATA(&mHata, CVector(30.0f, 100.0f, -19.65f), CVector(), CVector(5.5f, 5.5f, 0.5f));

	CModel* playerMode = GET_MODEL("Player");
	mPlayer.Model(playerMode);
	mPlayer.Position(CVector(30.0f, 3.0f, 1.0f));
	mPlayer.Rotation(CVector(0.0f, 180.0f, 0.0f));
	mPlayer.Scale(CVector(1.0f, 1.0f, 1.0f));//10.0f, 10.0f, 10.0f
	CMatrix matrix;
	matrix.print();
	mEye = CVector(1.0f, 2.0f, 3.0f);
	mpBackGround = GET_MODEL("BG");
	//�r���{�[�h�̍쐬
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͂Ȃ�
	mColliderMesh.Set(nullptr, nullptr, mpBackGround);

	CStageManager::Instance()->LoadStage(0);
	mState = EState::ESTART;
	//mpStage1 = new CStage1()
}

void CApplication::Update()
{
	switch (mState)
	{
	case EState::ESTART:	//��Ԃ��X�^�[�g
		mpStage1->Start();	//�X�^�[�g��ʕ\��
		//Enter�L�[�������ꂽ��
		if (mInput.Key(VK_RETURN))
		{	//��Ԃ��v���C���ɂ���
			mState = EState::EPLAY;
			//BGM���s�[�g�Đ�
			mSoundBgm.Repeat();
		}
		break;
	//case EState::EPLAY:
	//	//�Q�[���I�[�o�[������
	//	if (mpStage1->IsOver())
	//	{	//��Ԃ��Q�[���I�[�o�[�ɂ���
	//		mState = EState::EOVER;
	//	}
	//	//�Q�[���N���A������
	//	if (mpStage1->IsClear())
	//	{	//��Ԃ��Q�[���N���A�ɂ���
	//		mState = EState::ECLEAR;
	//	}
	//	break;
	//case EState::EOVER:
	//	//�Q�[���I�[�o�[����
	//	mpStage1->Over();
	//	//�G���^�[�L�[���͎�
	//	if (mInput.Key(VK_RETURN))
	//	{	//�Q�[���̃C���X�^���X�폜
	//		delete mpStage1;
	//		//�Q�[���̃C���X�^���X����
	//		mpStage1 = new CStage1();
	//		//��Ԃ��X�^�[�g�ɂ���
	//		mState = EState::ESTART;
	//	}
	//	break;
	//case EState::ECLEAR:
	//	//�Q�[���N���A����
	//	mpStage1->Clear();
	//	if (mInput.Key(VK_RETURN))
	//	{
	//		delete mpStage1;
	//		mpStage1 = new CStage1();
	//		mState = EState::ESTART;
	//	}
	//	break;
	}
	if (mInput.PullKey('1'))
	{
		CStageManager::Instance()->LoadStage(0);
	}
	else if (mInput.PullKey('2'))
	{
		CStageManager::Instance()->LoadStage(1);
	}
	spUi->Hp(CPlayer::Hp());
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::Instance()->Update();
	CTaskManager::Instance()->Collision();
	//�R���W�����}�l�[�W���̏Փˏ���
	//CCollisionManager::Instance()->Collision();
	//CTaskManager::Instance()->Collision();
	//���_1�A���_2�A���_3,�@���f�[�^�̍쐬
	CVector v0, v1, v2, n;
	//�@����������Őݒ肷��
	n.Set(0.0f, 1.0f, 0.0f);
	//���_1�̍��W��ݒ肷��
	v0.Set(0.0f, 0.0f, 0.5f);
	//���_2�̍��W��ݒ肷��
	v1.Set(1.0f, 0.0f, 0.0f);
	//���_3�̍��W��ݒ肷��
	v2.Set(0.0f, 0.0f, -0.5f);
	if (mInput.Key('J'))
	{
		mEye = mEye - CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('L'))
	{
		mEye = mEye + CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('I'))
	{
		mEye = mEye - CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('K'))
	{
		mEye = mEye + CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('O'))
	{
		mEye = mEye + CVector(0.0f, 0.1f, 0.0f);
	}
	if (mInput.Key('M'))
	{
		mEye = mEye - CVector(0.0f, 0.1f, 0.0f);
	}
	//���_�̐ݒ�
	//gluLookAt(���_X,���_Y,���_Z,���SX,���SY,���SZ,���X,���Y,���Z)
	//gluLookAt(mEye.X(), mEye.Y(), mEye.Z(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	CTransform trans;  //�ϊ��s��C���X�^���X�̍쐬
	trans.Position(CVector(1.0f, 4.0f, 2.0f));//�ʒu�̐ݒ�
	trans.Rotation(CVector(-10.0f, -20.0f, -30.0f));  //��]�̐ݒ�
	trans.Scale(CVector(1.0f, 1.0f, 1.0f));//�g��k���̐ݒ�

	trans.Update(); //�s��̍X�V
	//mPlayer.Update();
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;  //���_�A�����_�A�����
	//���_�����߂�
	e = mPlayer.Position() + CVector(-0.2f, 3.0f, -5.0f) * mPlayer.MatrixRotate();
		//�����_�����߂�
		c = mPlayer.Position();
	//����������߂�
		u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
		//�J�����̐ݒ�
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
		//���f���r���[�s��̎擾
		glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
		//�t�s��̎擾dd
		mModelViewInverse = mModelViewInverse.Transpose();
		mModelViewInverse.M(0, 3, 0);
		mModelViewInverse.M(1, 3, 0);
		mModelViewInverse.M(2, 3, 0);
	//mPlayer.Render();
	mpBackGround->Render();
	//�^�X�N���X�g�̍폜
	CTaskManager::Instance()->Delete();
	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::Instance()->Render();
	CCollisionManager::Instance()->Render();
	spUi->Render();  //UI�̕`��
}
