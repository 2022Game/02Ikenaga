#include "CKeyQ.h"
#include "CImage.h"

// �R���X�g���N
CKeyQ::CKeyQ(float x, float y)
{
	mpKeyQ = new CImage("KeyQ");
	mpKeyQ->SetPos(x, y);
	mpKeyQ->SetSize(50.0f, 50.0f);
}

// �f�X�g���N�^
CKeyQ::~CKeyQ()
{
}