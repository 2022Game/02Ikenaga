//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include"CTaskManager.h"
#include"CApplication.h"
#include"CCharacter3.h"
#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //��]���x
#define ROTATION_YY CVector(0.0f,0.7f,0.0f) //��]���x
#define ROTATION_Y CVector(0.0f,-0.3f,0.0f) //��]���x
#define ROTATION_X CVector(0.0f,-0.1f,0.0f) //��]���x
#define VELOCITY CVector(0.0f,0.0f,0.09f)//�ړ����x
#define  ROTATION_XV CVector(1.0f,0.0f,0.f) //��]���x
//#define ZYUURYOKU (TIPSIZE/20.0f)
//#define JANPU (TIPSIZE /3.0f)

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

CPlayer* CPlayer::spInstance = nullptr;

//�Փˏ���
void CPlayer::Collision()
{
	
	//�R���C�_�̗D��x�ύX
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine3, COLLISIONRANGE);
}

void CPlayer::Collision(CCollider* m, CCollider* o) {
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->Type()) {
	case CCollider::ELINE://�����R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->Type() == CCollider::ETRIANGLE) {
			CVector adjust;//�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//�ʒu�̍X�V(mPosition+adjust)
				mPosition = mPosition + adjust;
				//�s��̍X�V
				CTransform::Update();
			}
		}
		break;
	}
}

CPlayer::CPlayer()
	:mLine(this, &mMatrix, CVector(0.0f, 1.0f,0.0f), CVector(0.0f, 0.0f, 0.0f))
	,mLine2(this, &mMatrix, CVector(0.5f, 1.0f, 1.0f), CVector(-0.5f, 1.0f, 1.0f))
	,mLine3(this, &mMatrix, CVector(0.0f, 1.0f, 0.5f), CVector(0.0f, 1.0f, -0.5f))
	//, mLine4(this, &mMatrix, CVector(0.0f, 1.0f, 2.5f), CVector(0.0f, 1.0f, -0.5f))
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;
	//mColliderMesh1.Set(this, &mMatrix, mpModel);
}

//CPayer(�ʒu,��],�X�P�[��)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale); //�s��̍X�V
}

//�X�V����
void CPlayer::Update()
{
	//�X�y�[�X�L�[���͂Œe����
	if (mInput.Key(VK_SPACE)) {
		CBullet* bullet = new CBullet();
		bullet->Set(0.1f, 1.5);
		bullet->Position(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
		bullet->Rotation(mRotation);
		bullet->Update();
		//CApplication::TaskManager()->Add(bullet);
	}
	//D�L�[���͂ŉ�]
	if (mInput.Key('D')) {
		//Y���̉�]�l������
		mRotation = mRotation - ROTATION_YV;
	}
	//A�L�[���͂ŉ�]
	if (mInput.Key('A')) {
		//Y���̉�]�l������
		mRotation = mRotation + ROTATION_YV;
	}
	//��L�[���͂őO�i
	if (mInput.Key(VK_UP)) {
		//Z�������̒l����]�����ړ�������
		mPosition = mPosition + VELOCITY * mMatrixRotate;
   }
	//S�L�[���͂ŏ����
	if (mInput.Key('S')) {
		//X���̉�]�l�����Z
		//mRotation = mRotation - ROTATION_XV;
		mPosition = mPosition - VELOCITY * mMatrixRotate;
	}
	//W�L�[���͂ŏ����
	if (mInput.Key('W')) {
		//X���̉�]�l�����Z
		//mRotation = mRotation + ROTATION_XV;
		mPosition = mPosition + VELOCITY * mMatrixRotate;
		//mPosition = mPosition + ROTATION_X * mMatrixRotate;
	}
	//if (mState != EState::EJUMP)
	//{
	//	if (mInput.Key('J'))
	//	{
	//		//mPosition = mPosition + ROTATION_YY * mMatrixRotate;
	//	}
	//}
	int jump = 0;
	if (jump < 0)
	{
		jump--;
	}
	if (jump < 1)
	{
		if (mInput.Key('J'))
		{
			//mPosition = mPosition + ROTATION_Y * mMatrixRotate;
			mPosition = mPosition + ROTATION_YY * mMatrixRotate;
		}
	}
	mPosition = mPosition + ROTATION_Y * mMatrixRotate;
	//�ϊ��s��̍X�V
	CTransform::Update();

	//UI�ݒ�
	CApplication::Ui()->PosY(mPosition.Y());
	CApplication::Ui()->RotX(mRotation.X());
	CApplication::Ui()->RotY(mRotation.Y());
}