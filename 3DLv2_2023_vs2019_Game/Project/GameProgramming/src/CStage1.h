#pragma once
#include "CStageBase.h"
#include "CPlayer.h"
#include "CUi.h"

class CStage1:public CStageBase
{
public:
	//�Q�[���N���A����
	bool IsClear();
	//�Q�[���N���A����
	void Clear();
	//�Q�[���I�[�o�[����
	bool IsOver();
	//�Q�[���I�[�o�[����
	void Over();
	void Start();
	void Update();
	//�R���X�g���N�^
	CStage1();
	//�f�X�g���N�^
	~CStage1();
private:
	CPlayer* mpPlayer;
	CUi* mpUi;
};