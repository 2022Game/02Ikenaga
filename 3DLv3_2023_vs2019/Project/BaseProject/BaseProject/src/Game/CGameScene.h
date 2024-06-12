#pragma once
#include "CSceneBase.h"
class CPlayer;
class CSlime;
class CSlime2;
class CSlime3;
class CMushroom;
class CMushroom2;
class CMushroom3;
class CTurtle;
class CTurtle2;
class CTurtle3;
class CGameMenu;
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
	// ��������
	int SlimeReTime;      // SlimeRevivalTime(���b�h�X���C���̕�������)
	int Slime2ReTime;     // Slime2RevivalTime(�I�����W�X���C���̕�������)
	int Slime3ReTime;     // Slime3RevivalTime(�u���[�X���C���̕�������)
	int MushroomReTime;   // MushroomRevivalTime(�}�b�V�����[���̕�������)
	int Mushroom2ReTime;  // Mushroom2RevivalTime(�}�b�V�����[��2�̕�������)
	int Mushroom3ReTime;  // Mushroom3RevivalTime(�}�b�V�����[��3�̕�������)
	int TurtleReTime;     // TurtleRevivalTime(�T�̕�������)
	int Turtle2ReTime;    // Turtle2RevivalTime(�T2�̕�������)
	int Turtle3ReTime;    // Turtle3RevivalTime(�T3�̕�������)

	// �v���C���[
	CPlayer* mpPlayer;
	// �X���C��
	CSlime* mpSlime;
	CSlime2* mpSlime2;
	CSlime3* mpSlime3;
	// �}�b�V�����[��
	CMushroom* mpMushroom;
	CMushroom2* mpMushroom2;
	CMushroom3* mpMushroom3;
	// �T
	CTurtle* mpTurtle;
	CTurtle2* mpTurtle2;
	CTurtle3* mpTurtle3;

	CGameMenu* mpGameMenu;
};
