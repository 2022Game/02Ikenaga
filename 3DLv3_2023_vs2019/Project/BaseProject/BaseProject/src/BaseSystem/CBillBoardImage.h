#pragma once
#include "CBillBoard.h"
#include "CRect.h"
class CCharaBase;

// ��ɃJ�����̕����������C���[�W
class CBillBoardImage : public CBillBoard
{
public:
	// �R���X�g���N�^
	CBillBoardImage(std::string path,
		ETag tag = ETag::eNone,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// �f�X�g���N�^
	~CBillBoardImage();

	/// <summary>
	/// UV�ݒ�
	/// </summary>
	/// <param name="uv"></param>
	void SetUV(const CRect& uv);

	/// <summary>
	/// �A�j���[�V�����f�[�^��ݒ�
	/// </summary>
	/// <param name="animData"></param>
	void SetAnimData(TexAnimData* animData);

	// �A�j���[�V�������I��������ǂ���
	bool IsEndAnim() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	// �G�t�F�N�g�̔��˃L�����N�^�[��ݒ�
	void SetOwner(CCharaBase* owner);

	// �G�t�F�N�g�̔��˃L�����N�^�[���擾
	CCharaBase* GetOwner() const;

private:
	// �A�j���[�V�������Đ��ł��邩�ǂ���
	bool IsEnableAnim() const;

	// �A�j���[�V�����f�[�^�̃��X�g
	TexAnimData* mpAnimData;
	// ���݂̃A�j���[�V�����ԍ�
	int mAnimNo;
	// �A�j���[�V�����p�̌o�ߎ��Ԍv���ϐ�
	float mElapsedTime;

protected:
	// �U�����q�b�g�����I�u�W�F�N�g��ǉ�
	void AddAttackHitObj(CObjectBase* obj);
	// ���ɍU�����q�b�g���Ă���I�u�W�F�N�g���ǂ���
	bool IsAttackHitObj(CObjectBase* obj) const;

	// �U�����q�b�g�ς݂̃I�u�W�F�N�g���X�g
	std::list<CObjectBase*> mAttackHitObjects;

	// �G�t�F�N�g�̏����L�����N�^�[
	CCharaBase* mOwner;
};