#pragma once
#include "CSceneBase.h"

//�Q�[���V�[��
class CGameScene :public CSceneBase
{
public:

	//�R���X�g���N�^
	CGameScene();
	//�f�X�g���N�^
	~CGameScene();

	//�V�[���ǂݍ���
	void Load();
	void Update();
};
