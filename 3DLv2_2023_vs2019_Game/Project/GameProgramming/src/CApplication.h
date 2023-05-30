#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include "CCharacter.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include "CMiss.h"
#include <vector>
#include "CCharacterManager.h"
#include "CGame.h"
#include "CSound.h"
#include"CVector.h"
#include"CModel.h"
#include"CTriangle.h"
#include"CMatrix.h"
#include"CTransform.h"
#include"CCharacter3.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CColliderMesh.h"
#include"CEnemy3.h"
#include"CStage.h"
#include"CKokki.h"
#include"CHATA.h"

class CApplication
{
private:
	static CUi* spUi; //UI�N���X�̃|�C���^
	//���f������R���C�_�𐶐�
	CColliderMesh mColliderMesh;
	//�O�p�R���C�_�̍쐬
	CColliderTriangle mColliderTriangle;
	CColliderTriangle mColliderTriangle2;
	//���f���r���[�̋t�s��
	static CMatrix mModelViewInverse;
	CModel mStage;
	CModel mKokki;
	CModel mHata;
	CModel mEnemy1;
	//C5���f��
	CModel mModelC5;
	//static CTaskManager mTaskManager;
	CPlayer mPlayer;
	//CCharacter3 mCharacter;
	CModel mBackGround;//�w�i���f��
	//���f���N���X�̃C���X�^���X�쐬
	CModel mModel;
	//CModel mModel2;
	CTriangle* mTriangle;
	CVector mEye;
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[
	CGame* mpGame;
	static CCharacterManager mCharacterManager;
	enum class EState
	{
		ESTART,	//�Q�[���J�n
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	EState mState;
//	CCharacter mRectangle;
	//CPlayer* mpPlayer;
	static CTexture mTexture;
	//static CTexture mTexture1;
	CEnemy* mpEnemy;
//	CBullet* mpBullet;
	CInput mInput;
	CFont mFont;
	CMiss* mpMiss;
	//CCharacter�̃|�C���^�̉ϒ��z��
//	std::vector<CCharacter*> mCharacters;
public:
	~CApplication();
	static CUi* Ui();  //UI�N���X�̃C���X�^���X���擾
	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();
	//static CTaskManager* TaskManager();
	static CCharacterManager* CharacterManager();
	static CTexture* Texture();
	//static CTexture* Texture1();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};