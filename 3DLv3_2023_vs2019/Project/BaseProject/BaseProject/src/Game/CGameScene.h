#pragma once
#include "CSceneBase.h"
class CPlayer;
class CSound;

// �^�C�g���V�[��
class CGameScene :public CSceneBase
{
public:
	// �R���X�g���N�^
	CGameScene();
	// �f�X�g���N�^
	~CGameScene();
	// �V�[���ǂݍ���
	void Load();

	// �Q�[���N���A����
	bool IsClear();
	// �Q�[���N���A����
	void Clear();

	// �V�[���̍X�V����
	void Update();

private:
	// �v���C���[
	CPlayer* mpPlayer;
};
