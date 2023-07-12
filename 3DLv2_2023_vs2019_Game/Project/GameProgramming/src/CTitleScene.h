#pragma once
#include "CSceneBase.h"
#include "CSceneManager.h"
#include "CImage.h"
#include "CInput.h"

//�^�C�g���V�[��
class CTitleScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CTitleScene();
	//�f�X�g���N�^
	~CTitleScene();

	//�V�[���ǂݍ���
	void Load();
	void Update();
private:
	CInput mInput;
	CImage *mBgImage;
};