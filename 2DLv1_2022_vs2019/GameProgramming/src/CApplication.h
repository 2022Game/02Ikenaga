#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include "CCharacter.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include "CMiss.h"
#include <vector>
#include "CCharacterManager.h"
#include "CGame.h"
#include "CSound.h"
#include "CGoal.h"

class CApplication
{
private:
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[
	CGame* mpGame;
	static CCharacterManager mCharacterManager;
	enum class EState
	{
		ESTART,	//�Q�[���J�n
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	EState mState;
//	CCharacter mRectangle;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	static CTexture mTexture1;
	static CTexture mTexture2;
	static CTexture mTexture3;
	static CTexture mTexture4;
	static CTexture mTexture5;
	static CTexture mTexture6;
	static CTexture mTexture7;
	static CTexture mTexture8;
	static CTexture mTexture9;
	CEnemy* mpEnemy;
//	CBullet* mpBullet;
	CInput mInput;
	CFont mFont;
	CFont mFont1;
	CMiss* mpMiss;
	//CCharacter�̃|�C���^�̉ϒ��z��
//	std::vector<CCharacter*> mCharacters;
public:
	static CCharacterManager* CharacterManager();
	static CTexture* Texture();
	static CTexture* Texture1();
	static CTexture* Texture2();
	static CTexture* Texture3();
	static CTexture* Texture4();
	static CTexture* Texture5();
	static CTexture* Texture6();
	static CTexture* Texture7();
	static CTexture* Texture8();
	static CTexture* Texture9();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};