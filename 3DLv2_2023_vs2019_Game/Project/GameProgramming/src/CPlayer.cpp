//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include"CTaskManager.h"
#include"CApplication.h"
#include"CCollider.h"
#include"CCharacter3.h"
#include "CCollisionManager.h"
#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //��]���x
#define ROTATION_YY CVector(0.0f,2.0f,0.0f) //��]���x
#define ROTATION_YO CVector(0.0f,0.3f,0.0f) 
#define ROTATION_Y CVector(0.0f,-0.1f,0.0f) //��]���x
#define ROTATION_X CVector(0.0f,-0.1f,0.0f) //��]���x
#define VELOCITY CVector(0.0f,0.0f,0.09f)//�ړ����x
#define  ROTATION_XV CVector(1.0f,0.0f,0.f) //��]���x
#define HP 1 //HP

bool CPlayer::IsDeath() const
{
	return sHp <= 0;
}

int CPlayer::sHp = 1;

int CPlayer::Hp()
{
	return sHp;
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

CPlayer* CPlayer::spInstance = nullptr;

//�Փˏ���
void CPlayer::Collision()
{
	mCollider.ChangePriority();
	//�Փˏ��������s
	//�R���C�_�̗D��x�ύX
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine3, COLLISIONRANGE);
}

void CPlayer::Collision(CCollider* m, CCollider* o) {
	//���g�̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ETRIANGLE://�����R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->Type() == CCollider::ETRIANGLE)
		{
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
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::CCollision(m, o))
		{
			sHp = 0;
			CTransform::Update();
		}
		break;
	}
}

CPlayer::CPlayer()
	:jump(0)
{
	ETag::EPLAYER;
	sHp = HP;

	mpModel = new CModel();
	mpModel->Load("res\\obama4.obj", "res\\obama4.mtl");
	//�C���X�^���X�̐ݒ�
	spInstance = this;
}

//CPayer(�ʒu,��],�X�P�[��)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
	:jump(0)
{
	CTransform::Update(pos, rot, scale); //�s��̍X�V
}

CPlayer::CPlayer(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	: mLine(this, &mMatrix, CVector(0.0f, 1.9f, 0.0f), CVector(0.0f, 0.0f, 0.0f))
	, mLine2(this, &mMatrix, CVector(0.3f, 1.0f, 1.0f), CVector(-0.3f, 1.0f, 1.0f))
	, mLine3(this, &mMatrix, CVector(0.0f, 1.0f, 0.5f), CVector(0.0f, 1.0f, -0.5f))
	, jump(0)
	, mCollider(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 0.5f)
{
	CTransform::Update(position, rotation, scale);
	//���f��,�ʒu,��],�g�k��ݒ肷��
	mpModel = model;  //���f���̐ݒ�
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	sHp = HP;
	spInstance = this;
}

//�X�V����
void CPlayer::Update()
{
	SetHidden(false);
	if (mInput.Key('Y')) 
	{
		SetHidden(true);
	}
	//�X�y�[�X�L�[���͂Œe����
	if (mInput.Key(VK_RBUTTON))//�E�N���b�N//LBUTTON���N���b�N
	{
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
		mPosition = mPosition + VELOCITY * mMatrixRotate;
		//mPosition = mPosition + ROTATION_X * mMatrixRotate;
	}
	jump++;
	if (jump > 50)
	{
		if (mInput.Key('J'))
		{
			//mPosition = mPosition + ROTATION_Y * mMatrixRotate;
			mPosition = mPosition + ROTATION_YY * mMatrixRotate;
			jump = 0;
		}
	}
	if (mInput.Key('K'))
	{
		//mPosition = mPosition + ROTATION_Y * mMatrixRotate;
		mPosition = mPosition + ROTATION_YO * mMatrixRotate;
	}
	mPosition = mPosition + ROTATION_Y * mMatrixRotate;
	//�ϊ��s��̍X�V
	CTransform::Update();

	//UI�ݒ�
	/*CApplication::Ui()->PosY(mPosition.Y());
	CApplication::Ui()->RotX(mRotation.X());
	CApplication::Ui()->RotY(mRotation.Y());*/
}