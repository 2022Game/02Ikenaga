#include "CBlock5.h"
#include"CApplication.h"
#include"CEnemy4.h"

#define TEXCOORD 315.0f, 290.0f, 289.0f, 319.0f//�e�N�X�`�����W

CBlock5::CBlock5(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK5;
}

void CBlock5::Collision(CCharacter* m, CCharacter* o)
{
	//�߂荞�ݒ����ϐ���錾����
	float x, y;
	switch (o->Tag())
	{
	case ETag::EBLOCK:
		//�܂�Ԃ��ɓ���������
		if (CRectangle::Collision(o, &x, &y))
		{
			//�߂荞�܂Ȃ��ʒu�܂Ŗ߂�
			X(X() + x);
			Y(Y() + y);
			//X�����x�𔽓]������
			mVx = -mVx;
			//-mVx <=mVx;
		}
		break;
	case ETag::EBLOCK5:
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			mEnabled = false;
			//�G�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
			CApplication::CharacterManager()->Add(
				new CEnemy4(X(),
					Y(),
					TIPSIZE, TIPSIZE, CApplication::Texture8()));
			if (o->State() == EState::EJUMP)
			{
				//mEnabled = false;
			}
		}
		break;
	}
}