#include "CItem3.h"
#include "CApplication.h"

#define TEXCOORD1 325,280,150,190   //�|�C���g(�R�C��)

void CItem3::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CItem3::Collision(CCharacter* m, CCharacter* o)
{
	//�߂荞�ݒ����ϐ���錾����
	float x, y;
	switch (o->Tag())
	{
	case ETag::ETURN:
		//�܂�Ԃ��ɓ���������
		if (CRectangle::Collision(o, &x, &y))
		{
			//�߂荞�܂Ȃ��ʒu�܂Ŗ߂�
			X(X() + x);
			Y(Y() + y);
			//X�����x�𔽓]������
			mVx = -mVx;
		}
		break;
	case ETag::EENEMY:
		break;
	case ETag::EPLAYER:
		break;
	}
}
void CItem3::Update()
{
	//�ʏ�̉摜��ݒ�
	Texture(Texture(), TEXCOORD1);
}

CItem3::CItem3(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD1);
	mTag = ETag::EITEM;
}