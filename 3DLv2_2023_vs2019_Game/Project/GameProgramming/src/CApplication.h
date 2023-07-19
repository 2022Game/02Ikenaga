#pragma once
#include "CTexture.h"
#include "CInput.h"
#include "CSound.h"
#include"CVector.h"
#include"CTaskManager.h"
#include"CUi.h"

class CApplication
{
private:
	//int mSTcount;
	static CUi* spUi; //UI�N���X�̃|�C���^
	//���f������R���C�_�𐶐�
	//CColliderMesh mColliderMesh;
	//���f���r���[�̋t�s��
	static CMatrix mModelViewInverse;
	//CPlayer mPlayer;
	//CCharacter3 mCharacter;
	//CModel* mpBackGround;//�w�i���f��
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[
	static CTexture mTexture;
	CInput mInput;
public:
	~CApplication();
	static CUi* Ui();  //UI�N���X�̃C���X�^���X���擾
	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};