#include"CStage.h"

void CStage::Collision()
{

}

CStage::CStage(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	//: mCollider1(this, &mMatrix, CVector(0.0f, 5.0f, 0.0f), 0.8f)
	//, mCollider2(this, &mMatrix, CVector(0.0f, 5.0f, 20.0f), 0.8f)
	//, mCollider3(this, &mMatrix, CVector(0.0f, 5.0f, -20.0f), 0.8f)
{
	//���f��,�ʒu,��],�g�k��ݒ肷��
	mpModel = model;  //���f���̐ݒ�
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
}

//�X�V����
void CStage::Update()
{

}