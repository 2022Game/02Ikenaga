#include "CApplication.h"
//OpenGL
#include"glut.h"
#include "CRectangle.h"
#include"CVector.h"
#include"CBillBoard.h"
//#include"CTriangle.h"

//�N���X��static�ϐ�
CTexture CApplication::mTexture;
CCharacterManager CApplication::mCharacterManager;

#define SOUND_BGM "res\\mario.wav" //BGM�����t�@�C��
#define SOUND_OVER "res\\mdai.wav" //�Q�[���I�[�o�[�����t�@�C��
#define MODEL_OBJ "res\\f14.obj" ,"res\\f14.mtl"//���f���f�[�^�̎w��
#define MODEL_1 "res\\Smartphone.obj", "res\\Smartphone.mtl"
//�w�i���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"
//�G�A���@���f��
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"

//CTaskManager CApplication::mTaskManager;
//CTaskManager* CTaskManager::Instance()
//{
//	return mpInstance;
//}

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

CCharacterManager* CApplication::CharacterManager()
{
	return &mCharacterManager;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	spUi = new CUi();  //UI�N���X�̐���
	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͂Ȃ�
	//mColliderMesh.Set(nullptr, nullptr,&mBackGround);
	//�O�p�R���C�_�̊m�F
	//mColliderTriangle.Set(nullptr, nullptr, CVector(-50.0f, 0.0f, -50.0f), CVector(-50.0f, 0.0f, 50.0f), CVector(50.0f, 0.0f, -50.0f));
	//mColliderTriangle2.Set(nullptr, nullptr, CVector(50.0f, 0.0f, 50.0f), CVector(50.0f, 0.0f, -50.0f), CVector(-50.0f, 0.0f, 50.0f));
	new CEnemy3(CVector(-5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	new CEnemy3(CVector(5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	//�G�@�̃C���X�^���X�쐬
	new CEnemy(&mModelC5, CVector(30.0f, 10.0f, -130.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	//�G�@�̃C���X�^���X�쐬
	new CEnemy(&mModelC5, CVector(0.0f, 10.0f, -100.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	//C5���f���̓ǂݍ���
	mModelC5.Load(MODEL_C5);
	//mCharacter.Model(&mModel);
	//mCharacter.Scale(CVector(0.1f, 0.1f, 0.1f));
	mPlayer.Model(&mModel);
	mPlayer.Position(CVector(10.0f, 0.0f, -3.0f));
	mPlayer.Rotation(CVector(0.0f, 180.0f, 0.0f));
	mPlayer.Scale(CVector(10.0f, 10.0f, 10.0f));
	CMatrix matrix;
	matrix.print();
	mEye = CVector(1.0f, 2.0f, 3.0f);
	//���f���t�@�C���̓���
	//mModel.Load(MODEL_OBJ);
	mModel.Load(MODEL_1);
	mBackGround.Load(MODEL_BACKGROUND);
	//�r���{�[�h�̍쐬
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͂Ȃ�
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
}

void CApplication::Update()
{
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::Instance()->Update();
	//�R���W�����}�l�[�W���̏Փˏ���
	//CCollisionManager::Instance()->Collision();
	CTaskManager::Instance()->Collision();
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
	trans.Position(CVector(0.5f, 1.8f, 0.5f));//�ʒu�̐ݒ�
	trans.Rotation(CVector(-10.0f, -20.0f, -30.0f));  //��]�̐ݒ�
	trans.Scale(CVector(0.1f, 0.1f, 0.1f));//�g��k���̐ݒ�

	trans.Update(); //�s��̍X�V
	//mPlayer.Update();
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;  //���_�A�����_�A�����
	//���_�����߂�
	e = mPlayer.Position() + CVector(-0.2f, 1.0f, -3.0f) * mPlayer.MatrixRotate();
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
	mBackGround.Render();
	//�^�X�N���X�g�̍폜
	CTaskManager::Instance()->Delete();
	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::Instance()->Render();
	CCollisionManager::Instance()->Render();
	spUi->Render();  //UI�̕`��
	//mPlayer.bullet.Update();
	//mPlayer.bullet.Render();
	//mCharacter.Update();
	//mCharacter.Render();
	//mModel.Render(trans.Matrix());
	//CMatrix matrix, position, rotation, scale;
	//rotation.RotateY(180.0f);  //��]�s��ݒ�
	//mModel.Render(matrix);  //���f���̕`��
	//�`��J�n
	//glBegin(�`)
	//GL_TRIANGLES:�O�p�`
//	glBegin(GL_TRIANGLES);
//
//	//�@��(�ʂ̌���)�̐ݒ�
//	glNormal3f(n.X(), n.Y(), n.Z());
//	//���_���W�̐ݒ�
//	glVertex3f(v0.X(),v0.Y(),v0.Z());
//	glVertex3f(v1.X(),v1.Y(),v1.Z());
//	glVertex3f(v2.X(), v2.Y(), v2.Z());
//
//	//�@���ƒ��_�̐ݒ�
//	n.Set(0.0f, 0.0f, 1.0f);
//	v0.Set(0.5f, 0.0f, 0.0f);
//	v1.Set(0.0f, 1.0f, 0.0f);
//	v2.Set(-0.5f, 0.0f, 0.0f);
//	//�O�p�`2�̕`��
//	glNormal3f(n.X(), n.Y(), n.Z());
//	glVertex3f(v0.X(), v0.Y(), v0.Z());
//	glVertex3f(v1.X(), v1.Y(), v1.Z());
//	glVertex3f(v2.X(), v2.Y(), v2.Z());
//
//	//�@���ƒ��_�̐ݒ�
//	n.Set(1.0f, 0.0f, 0.0f);
//	v0.Set(0.0f, 0.5f, 0.0f);
//	v1.Set(0.0f, 0.0f, 1.0f);
//	v2.Set(0.0f, -0.5f, 0.0f);
//	//�O�p�`2�̕`��
//	glNormal3f(n.X(), n.Y(), n.Z());
//	glVertex3f(v0.X(), v0.Y(), v0.Z());
//	glVertex3f(v1.X(), v1.Y(), v1.Z());
//	glVertex3f(v2.X(), v2.Y(), v2.Z());
//
//	//�ʂ̌�����Z������
//	//glNormal3f(0.0f, 0.0f, 1.0f);
//	//glVertex3f(0.0f, 0.0f, 0.0f);
//	//glVertex3f(0.0f, 1.0f, 0.0f);
//	//glVertex3f(-0.5f, 0.0f, 0.0f);
//
//	//glNormal3f(1.0f, 0.0f, 0.0f);
//	//glVertex3f(0.0f, -0.5f, 0.0f);
//	//glVertex3f(0.0f, 0.0f, 0.0f);
//	//glVertex3f(0.0f, 0.0f, 1.0f);
//
//	//�O�p�`�N���X�̃C���X�^���X�쐬
//	CTriangle t0;
//	//�@���ƒ��_�̐ݒ�
//	t0.Vertex(CVector(1.0f, 0.0f, 0.5f), CVector(2.0f, 0.0f, 0.0f), CVector(1.0f, 0.0f, -0.5f));
//	t0.Normal(CVector(0.0f, 1.0f, 0.0f));
//	//�O�p�`�̕`��
//	t0.Render();
//
//	//�O�p�`�N���X�̃C���X�^���X�쐬
//	CTriangle t1;
//	//�@���ƒ��_�̐ݒ�
//	t1.Vertex(CVector(0.5f, 1.0f, 0.0f), CVector(0.0f, 2.0f, 0.0f), CVector(-0.5f, 1.0f, 0.0f));
//	t1.Normal(CVector(1.0f, 0.0f, 0.0f));
//	//�O�p�`�̕`��
//	t1.Render();
//
//	//�O�p�`�N���X�̃C���X�^���X�쐬
//	CTriangle t2;
//	//�@���ƒ��_�̐ݒ�
//	t2.Vertex(CVector(0.0f, 0.0f, 2.0f), CVector(0.0f, -0.5f, 1.0f), CVector(0.0f, 0.5f, 1.0f));
//	t2.Normal(CVector(1.0f, 0.0f, 0.0f));
//	//�O�p�`�̕`��
//	t2.Render();
//
//	//�`��I��
//	//glEnd();
}
