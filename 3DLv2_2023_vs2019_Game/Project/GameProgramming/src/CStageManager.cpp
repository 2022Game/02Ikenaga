#include "CStageManager.h"
#include "CStage1.h"
#include "CStage2.h"
#include "CStage3.h"
#include "CStage4.h"
#include "CStage5.h"

CStageManager* CStageManager::mpInstance = nullptr;

//�C���X�^���X���擾
CStageManager* CStageManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CStageManager();
	}
	return mpInstance;
}

//�R���X�g���N�^
CStageManager::CStageManager()
	:mpStage(nullptr)
{

}

//�f�X�g���N�^
CStageManager::~CStageManager()
{
	UnloadStage();
}

//�w�肳�ꂽ�X�e�[�W��ǂݍ���
void CStageManager::LoadStage(int no)
{
	//���ݓǂݍ���ł���X�e�[�W�͔j�����Ă���
	UnloadStage();

	switch (no)
	{
	case 0:mpStage = new CStage1(); break;
	case 1:mpStage = new CStage2(); break;
	case 2:mpStage = new CStage3(); break;
	case 3:mpStage = new CStage4(); break;
	case 4:mpStage = new CStage5(); break;
	}
}

//�ǂݍ��܂�Ă���X�e�[�W��j��
void CStageManager::UnloadStage()
{
	//�X�e�[�W���ǂݍ��܂�ĂȂ���΁A�������Ȃ�
	if (mpStage == nullptr)return;

	//�X�e�[�W�j��
	delete mpStage;
	mpStage = nullptr;
}