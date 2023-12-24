#pragma once
#include "CSceneBase.h"
class CPlayer;
class CSlime;
class CSlime2;

// �^�C�g���V�[��
class CGameScene :public CSceneBase
{
public:
	// �R���X�g���N�^
	CGameScene();
	// �f�X�g���N�^
	~CGameScene();
	// �V�[���ǂݍ���
	void Load();

	// �Q�[���N���A����
	bool IsClear();
	// �Q�[���N���A����
	void Clear();
	// �Q�[���I�[�o�[����
	bool IsOver();
	// �Q�[���I�[�o�[����
	void Over();
	// �V�[���̍X�V����
	void Update();

private:
	// ��������
	int RevivalTime;
	int RevivalTime2;
	CPlayer* mpPlayer;
	CSlime* mpSlime;
	CSlime2* mpSlime2;
};
