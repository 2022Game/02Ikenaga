#ifndef CTRIANGLE_H
#define CTRIANGLE_H

#include"CVector.h"
/*
�O�p�`
*/
class CTriangle{
public:
	//Normal(�@���x�N�g��1,�@���x�N�g��2,�@���x�N�g��3)
	void Normal(const CVector& v0, const CVector& v1, const CVector& v2);
	void Vertex(const CVector &v0, const CVector &v1, const CVector &v2);
	//�@���ݒ�
	//Normal(�@���x�N�g��)
	void Normal(const CVector& n);
		//�`��
		void Render();
private:
	//�O�p�`�̉ϒ��z��
	CVector mV[3];//���_���W
	CVector mN[3];   //�@��
};
#endif