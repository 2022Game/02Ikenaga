#pragma once
#include "CUi.h"
#include "CPlayer2.h"
#include"CItem.h"

//�萔�̒�`
#define TEXTURE "�Q�[������̑f��02.png" //�e�N�X�`���̃t�@�C��
#define HITO "Image.png"
#define SURAIMU "Monster2.png"
#define KODOMO "117545.png"
#define GOAL "890568442.png"
#define UGOKUBLOCK "6fce1c1aa2540daee360283fa78defcd_t1.png"
#define AITEM "_7814-459000.png"
#define TEKI "chara081234.png"
#define TEKI2 "DqWQjAXWsAI-YEO1.png"
#define UPUP "117545011.png"

#define TIPSIZE 20.0f //�}�b�v�v�f�̃T�C�Y

/*
* CGame�N���X
* �Q�[���N���X
* �Q�[���̃}�b�v���쐬���A
* �L�����N�^��z�u����
*/
class CGame 
{
public:
	//void Collision(CCharacter* m, CCharacter* o);
	//static int Time(); //�o�ߎ���
	//�f�X�g���N�^
	~CGame();
	//�Q�[���N���A����
	bool IsClear();
	//�Q�[���N���A����
	void Clear();
	//�Q�[���I�[�o�[����
	bool IsOver();
	//�Q�[���I�[�o�[����
	void Over();
	//�X�^�[�g����
	void Start();
	CGame(); //�f�t�H���g�R���X�g���N�^�i���������j
	void Update(); //�X�V����
private:
	int mCdx, mCdy; //�J�����ƃv���C���[�̍��W�̍���
	void CameraSet(); //�J�����ݒ�
	CPlayer2 *mpPlayer; //�v���C���[�̃|�C���^
	CItem *mpItem;
	CUi *mpUi; //UI�N���X�̃|�C���^
    int mTime;
	//CItem* mpItem;
	//CGoal *mpGoal;
};
