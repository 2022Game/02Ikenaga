#include "CStageBase.h"

//�R���X�g���N�^
CStageBase::CStageBase()
{
}

//�f�X�g���N
CStageBase::~CStageBase()
{
	for (CCharacter3* chara : mCharaList)
	{
		delete chara;
	}
	mCharaList.clear();
}

//�L�����N�^�[�����X�g�ɒǉ�
void CStageBase::AddChara(CCharacter3* chara)
{
	mCharaList.push_back(chara);
}