#include "CXEnemy.h"

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//��
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
	//��
	mColSphereBody.Matrix(&mpCombinedMatrix[8]);
}