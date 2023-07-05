#pragma once
#include <vector>
#include "CCharacter3.h"

class CStageBase
{
public:
	//�R���X�g���N�^
	CStageBase();
	//�f�X�g���N�^
	virtual ~CStageBase();
protected:
	//�L�����N�^�[�����X�g�ɒǉ�
	void AddChara(CCharacter3* chara);
private:
	//���������L�����N�^�[�̃��X�g
	std::vector<CCharacter3*> mCharaList;
};
