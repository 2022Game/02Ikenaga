#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include "CModelX.h"
#include "CMaterial.h"
#include "CCharaBase.h"
#include "CharaStatus.h"
#include "CharaStatus2.h"

class CXCharacter : public CCharaBase
{
public:
	//�R���X�g���N�^
	CXCharacter(ETag tag, ETaskPriority prio);
	//�f�X�g���N�^
	virtual ~CXCharacter();

	//����������
	void Init(CModelX* model);
	//�A�j���[�V�����̕ύX
	void ChangeAnimation(int index, bool loop, float framesize);
	//�X�V����
	void Update(const CMatrix& m);
	void Update();
	//�`�揈��
	void Render();
	//�A�j���[�V�����̍Đ��I������
	//true:�I�� false:�Đ���
	bool IsAnimationFinished();

	int AnimationIndex();	//�A�j���[�V�����̔ԍ��̎擾

	const CMatrix* GetFrameMtx(std::string name) const;

	//�v���C���[�̍ő�X�e�[�^�X���擾
	const CharaStatus& MaxStatus() const;
	//�v���C���[�̃X�e�[�^�X���擾
	const CharaStatus& Status() const;

	//�G�l�~�[�̍ő�X�e�[�^�X���擾
	const CharaStatus2& MaxStatus2() const;
	//�G�l�~�[�̃X�e�[�^�X���擾
	const CharaStatus2& Status2() const;
protected:
	CModelX* mpModel;			//���f���f�[�^
	CMatrix* mpCombinedMatrix;	//�����s��ޔ�

	bool mAnimationLoopFlg;		//true:�A�j���[�V�������J��Ԃ�

	int mAnimationIndex;		//�A�j���[�V�����ԍ�

	float mAnimationFrame;		//�A�j���[�V�����̍Đ��t���[��
	float mAnimationFrameSize;	//�A�j���[�V�����̍Đ��t���[����

	CharaStatus mCharaMaxStatus;  //�v���C���[�̍ő�X�e�[�^�X
	CharaStatus mCharaStatus;  //�v���C���[�̃X�e�[�^�X

	CharaStatus2 mCharaMaxStatus2;  //�G�l�~�[�̍ő�X�e�[�^�X
	CharaStatus2 mCharaStatus2;  //�G�l�~�[�̃X�e�[�^�X
};
#endif