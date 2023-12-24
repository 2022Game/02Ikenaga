#pragma once
class CGameScene;

class CApplication
{
private:
	enum class EState
	{
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	CGameScene* mpGameScene;
	EState mState;
public:
	~CApplication();

	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�I�����Ɏ��s����v���O����
	void End();
	//�J��Ԃ����s����v���O����
	void Update();
};