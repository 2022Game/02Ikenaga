#include "CApplication.h"
//OpenGL
#include"glut.h"
#include"CVector.h"
#include"CBillBoard.h"
#include "CResourceManager.h"
#include "CStageManager.h"
#include "CSceneManager.h"
#include "CCollisionManager.h"
#include"CClear.h"
#include"CPlayer.h"

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
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
	spUi = new CUi();  //UI�N���X�̐���
	spUi->STCOUNT(CClear::STcount());
	//spUi->Hp(CPlayer::Hp());
}

void CApplication::Update()
{
	if (mInput.PullKey('1'))
	{
		CStageManager::Instance()->LoadStage(0);
	}
	else if (mInput.PullKey('2'))
	{
		CStageManager::Instance()->LoadStage(1);
	}
	else if (mInput.PullKey('3'))
	{
		CStageManager::Instance()->LoadStage(2);
	}
	else if (mInput.PullKey('4'))
	{
		CStageManager::Instance()->LoadStage(3);
	}
	else if (mInput.PullKey('5'))
	{
		CStageManager::Instance()->LoadStage(4);
	}
	spUi->STCOUNT(CClear::STcount());
	spUi->Hp(CPlayer::Hp());
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::Instance()->Update();
	CTaskManager::Instance()->Collision();
	//���_�̐ݒ�
	//gluLookAt(���_X,���_Y,���_Z,���SX,���SY,���SZ,���X,���Y,���Z)
	//gluLookAt(mEye.X(), mEye.Y(), mEye.Z(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	CTransform trans;  //�ϊ��s��C���X�^���X�̍쐬
	trans.Position(CVector(1.0f, 4.0f, 2.0f));//�ʒu�̐ݒ�
	trans.Rotation(CVector(-10.0f, -20.0f, -30.0f));  //��]�̐ݒ�
	trans.Scale(CVector(1.0f, 1.0f, 1.0f));//�g��k���̐ݒ�
	trans.Update(); //�s��̍X�V
	//mPlayer.Update();
	CSceneManager::Instance()->Update();
	
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//�t�s��̎擾dd
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);
	//�^�X�N���X�g�̍폜
	CTaskManager::Instance()->Delete();
	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::Instance()->Render();
	CCollisionManager::Instance()->Render();
	spUi->Render();  //UI�̕`��
}
