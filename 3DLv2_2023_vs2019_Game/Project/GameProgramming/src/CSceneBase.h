#pragma once
#include "CSceneType.h"
class CTask;

//�V�[���̃x�[�X�N���X
class CSceneBase
{
public:
	//�R���X�g���N�^
	CSceneBase(EScene scene);
	//�f�X�g���N�^
	virtual ~CSceneBase();

	virtual void Load() = 0;

	virtual void Update() = 0;

	EScene GetSceneType() const;

	void AddTask(CTask* task);

private:
	EScene mSceneType;
};