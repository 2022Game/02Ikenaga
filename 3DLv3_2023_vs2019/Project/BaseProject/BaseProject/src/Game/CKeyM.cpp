#include "CKeyM.h"
#include "CImage.h"

// �R���X�g���N
CKeyM::CKeyM(float x, float y)
{
	mpKeyM = new CImage("KeyM");
	mpKeyM->SetPos(x, y);
	mpKeyM->SetSize(50.0f, 50.0f);
}

// �f�X�g���N�^
CKeyM::~CKeyM()
{
}