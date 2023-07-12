#pragma once
#ifndef CTASK_H
#define CTASK_H
class CTaskManager;
/*
�^�X�N�N���X
�^�X�N���X�g�̗v�f
*/

class CCollisionManager;
class CTask {
	friend CCollisionManager;
	friend CTaskManager;
public:
	//�Փˏ���	
	virtual void Collision() {}
	//�f�t�H���g�R���X�g���N�^
	CTask();
	//�f�X�g���N�^�@virtual�ɂ��Ȃ��Ǝq�N���X�̃f�X�g���N�^���Ă΂�Ȃ�
	virtual~CTask(){}
	//�X�V
	virtual void Update() {}
	//�`��
	virtual void Render(){}

	void SetHidden(bool isRender);
	//�`�悷�邩�ǂ���
	bool IsHidden()const;
protected:
	int mPriority; //�D��x
	bool mEnabled; //�L���t���O

	bool mIsHidden;//����ǉ�����
private:
	CTask* mpNext;//���̃|�C���^
	CTask* mpPrev;//�O�̃|�C���^
};

#endif 