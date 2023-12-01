#include "CExp.h"
#include "CCollisionManager.h"

CExp::CExp()
	: CBillBoardImage("Item\\exp.png",ETag::eExp, ETaskPriority::eExp, 
		0, ETaskPauseType::eGame)
	,mMoveSpeed(0.0f,0.0f,0.0f)
{
	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::eExp,
		0.5f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eField });
	//mpColliderSphere->Position(-0.03f, 0.1f, 0.05f);
}

CExp::~CExp()
{
	SAFE_DELETE(mpColliderSphere);
}


// �����ݒ�
void CExp::Setup(const CVector& pos, const CVector& dir)
{
	Position(pos);

	//�@�ړ������x�N�g����������]���ď�����֌�����
	CVector side = CVector::Cross(CVector::up, dir);
	CQuaternion q;
	q.RotationAxis(side, 75.0f);
	mMoveSpeed = q* dir * 75.0f;
}

void CExp::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderSphere)
	{
		// �v���C���[�ƏՓ˂����ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			Kill();
		}
		// �t�B�[���h�ƏՓ˂����ꍇ
		else if (other->Layer() == ELayer::eField)
		{
			//�����߂�
			Position(Position() + hit.adjust * hit.weight);

			// �ړ��x�N�g���𔽎˂�����
			CVector moveVec = mMoveSpeed;
			CVector n = hit.adjust.Normalized();
			float d = CVector::Dot(n, moveVec);
			CVector reflect = moveVec - (n * d * 2.0f);
			mMoveSpeed = reflect * 0.7f;

			// Y���̈ړ��ʂ�0�ɂ���
			//mMoveSpeed.Y(0.0f);
		}
	}
}

void CExp::Update()
{
	// �d�͂����Z
	mMoveSpeed.Y(mMoveSpeed.Y() - 2.0f);
	// ���W���ړ����x���ړ�������
	Position(Position() + mMoveSpeed * Time::DeltaTime());
}