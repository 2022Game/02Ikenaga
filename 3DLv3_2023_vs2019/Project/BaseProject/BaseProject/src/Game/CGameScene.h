#pragma once
#include "CSceneBase.h"
class CSlime;
class CSlime2;

//�^�C�g���V�[��
class CGameScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CGameScene();
	//�f�X�g���N�^
	~CGameScene();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	// ��������
	int RevivalTime;
	int RevivalTime2;
	CSlime* mpSlime;
	CSlime2* mpSlime2;
};
