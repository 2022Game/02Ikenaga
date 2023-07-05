#pragma once
#include "CStageBase.h"

class CStageManager
{
public:
	//�C���X�^���X���擾
	static CStageManager* Instance();

	//�R���X�g���N�^
	CStageManager();
	//�f�X�g���N�^
	~CStageManager();

	//�w�肳�ꂽ�X�e�[�W��ǂݍ���
	void LoadStage(int no);
	//�ǂݍ��܂�Ă���X�e�[�W��j��
	void UnloadStage();

private:
	//CStageManager�̃C���X�^���X
	static CStageManager* mpInstance;
	//���ݓǂݍ���ł���X�e�[�W
	CStageBase* mpStage;
};