#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
 ��
*/
class CSky : public CObjectBase
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CSky* Instance();

	// �R���X�g���N
	CSky();
	//�@�f�X�g���N�^
	~CSky();

	// �R���C�_�[���擾
	CCollider* GetCollider() const;

	// �X�V
	void Update();
	// �`��
	void Render();

	static float mElapsedTime;	  // �o�ߎ���
private:
	// �C���X�^���X
	static CSky* spInstance;
	// ���f��
	CModel* mpModel;
	// �R���C�_�[���b�V��
	CColliderMesh* mpColliderMesh;
};