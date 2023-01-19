#include"CTaskManager.h"

//�^�X�N�}�l�[�W���̃C���X�^���X
CTaskManager* CTaskManager::mpInstance = nullptr;

//�C���X�^���X�̎擾
CTaskManager* CTaskManager::Instance()
{
	//�C���X�^���X���Ȃ����
	if (mpInstance == nullptr)
	{    //�C���X�^���X���쐬
		mpInstance = new CTaskManager();
	}
	return mpInstance;
}

void CTaskManager::Delete() {
	//�擪����Ō�܂ŌJ��Ԃ�
	CTask* task = mHead.mpNext;
	while (task->mpNext){
		CTask* del = task;
		//����
		task = task->mpNext;
		//mEnabled��false�Ȃ�폜
		if (del->mEnabled == false) {
			delete del;
		}
	}
}


void CTaskManager::Remove(CTask* task) {
	//�^�X�N�̑O�̎����A�^�X�N�̎��ɂ���
	task->mpPrev->mpNext = task->mpNext;
	//�^�X�N�̎��̑O���A�^�X�N�̑O�ɂ���
	task->mpNext->mpPrev = task->mpPrev;
}

//�f�t�H���g�R���X�g���N�^
CTaskManager::CTaskManager()
{
	mHead.mpNext = &mTail;
	mTail.mpPrev = &mHead;
}
CTaskManager::~CTaskManager() {
}
//���X�g�ɒǉ�
//Add(�^�X�N�̃|�C���^)
void CTaskManager::Add(CTask* addTask) {
	//mHead�̎����猟��
	CTask* task = mHead.mpNext;

	//�D��x�̑傫�����ɑ}������
	//�}���ʒu�̌���(�D��x���������傫���Ȃ����O)
	// mPriority>=0�̂���
	while (addTask->mPriority<task->mPriority)
	{
		task = task->mpNext;
	}
	
	//mTail�̑O�ɒǉ�
	//CTask* task = &mTail;
	//addtask�̎���task
	addTask->mpNext = task;
	//addtask�̑O��task�̑O��
	addTask->mpPrev = task->mpPrev;
	//addtask�̑O�̎���addtask��
	addTask->mpPrev->mpNext = addTask;
	//task�̑O��addtask��
	task->mpPrev = addTask;
}
//�X�V
void CTaskManager::Update() {
	//�擪����Ō�܂ŌJ��Ԃ�
	CTask* task = mHead.mpNext;
	while (task->mpNext){
		//�X�V�������Ă�
		task->Update();
		//����
		task = task->mpNext;
	}
}
//�`��
void CTaskManager::Render() {
	//�擪����Ō�܂ŌJ��Ԃ�
	CTask* task = mTail.mpPrev;
	while (task->mpPrev != nullptr)
	{
		//�`�揈�����Ă�
		task->Render();
		//�O��
		task = task->mpPrev;
		//return;
	}
}