#include "CGear.h"
#include "CImage.h"

// �R���X�g���N
CGear::CGear(float x, float y)
{
	mpGear = new CImage("Gear");
	mpGear->SetPos(x, y);
	mpGear->SetSize(50.0f, 50.0f);
}

// �f�X�g���N�^
CGear::~CGear()
{

}
