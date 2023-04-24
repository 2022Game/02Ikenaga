#pragma once
#include "CVector.h"
#include <vector>//vector�N���X�̃C���N���[�h(���I�z��)
#include "CMatrix.h"  //�}�g���N�X�N���X�̃C���N���[�h
class CModelX;  //CModelX�N���X�̐錾
class CModelXFrame;  //CModelXFrame�N���X�̐錾
#ifndef CMODELX_H //�C���N���[�h�K�[�h
#define CMODELX_H

#define MODEL_FILE "res\\sample.blend.x"  //���̓t�@�C����

//�̈������}�N����
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a; a=nullptr;}

class CMesh;  //CMesh�N���X�̐錾
class CMesh {
public:
	//�R���X�g���N�^
	CMesh();
	//�f�X�g���N�^
	~CMesh();
	//�ǂݍ��ݏ���
	void Init(CModelX* model);
private:
	//int mNormalNum;  //�@����
	//CVector* mpNormal;  //�@���x�N�g��
	int mFaceNum;  //�ʐ�
	int* mpVertexIndex;  //�ʂ��\�����钸�_�C���f�b�N�X
	int mVertexNum;  //���_��
	CVector* mpVertex;  //���_�f�[�^
};

//CModelXFrame�N���X�̒�`
class CModelXFrame {
	friend CModelX;
public:
	//�R���X�g���N�^
	CModelXFrame(CModelX* model);
	//�f�X�g���N�^
	~CModelXFrame();
private:
	CMesh* mpMesh;  //Mesh�f�[�^
	std::vector<CModelXFrame*> mChild;   //�q�t���[���̔z��
	CMatrix mTransformMatrix;  //�ϐ��s��
	char* mpName;  //�t���[�����O
	int mIndex;  //�t���[���ԍ�
};

/*
 CModelX
 X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX {
	friend CModelXFrame;
public:
	//�P��̎��o��
	char* GetToken();
	char* Token();
	~CModelX();
	//�m�[�h�̓ǂݔ�΂�
	void SkipNode();
	CModelX();
	//�t�@�C���ǂݍ���
	void Load(char* file);
private:
	std::vector<CModelXFrame*> mFrame;  //�t���[���̔z��
	//�P��̎��o��
	//char* GetToken();
	//c����؂蕶���Ȃ�true��Ԃ�
	bool IsDelimiter(char c);
	char* mpPointer;     //�ǂݍ��݈ʒu
	char mToken[1024];  //���o�����P��̗̈�
};

#endif
