#pragma once
#include "CCharacter.h"
#include "CInput.h"
#include "CSound.h"
#include "CAttack.h"

class CPlayer2 : public CCharacter
{
public:
	static int Point();
	//HP���擾
	static int Hp();
	//�Փˏ���2
	void Collision();
	//�Փˏ���4
	void Collision(CCharacter* m, CCharacter* o);
	//CPlayer2(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CPlayer2(float x, float y, float w, float h, CTexture* pt);
	//�X�V����
	void Update();
private:
	CSound mSoundJump;	//�W�����vSE
	static int sHp;	//HP
	static int sPoint;
	int mInvincible; //���G�J�E���^
	int mnothing; //�Ȃɂ��Ȃ�
	CInput mInput;
	float mVy;//Y�����x
	float mVx;
};