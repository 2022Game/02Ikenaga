#include "CExp.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

CExp::CExp()
	: CBillBoardImage("Item\\exp.png",ETag::eExp, ETaskPriority::eExp, 
		0, ETaskPauseType::eGame)
	,mMoveSpeed(0.0f,0.0f,0.0f)
	, mState(EState::Drop)
	, mElapsedTime(0.0f)
{
	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::eExp,
		0.5f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer, ELayer::eField });
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
		//else if(other->Layer() == ELayer::ePlayer)
		//{
		//	mpColliderSphere->SetEnable(true);
		//}
		//else
		//{
		//	// �z����Ԃł͒n�ʂƂ̓����蔻��͖���
		//	//mpColliderSphere->SetEnable(false);
		//}
	}
}

void CExp::Update()
{
	// �v���C���[�̏����擾
	CPlayer* player = CPlayer::Instance();
	// �v���C���[�̍��W
	CVector playerPos = CVector::zero;
	// �v���C���[�܂ł̃x�N�g��
	CVector playerVec = CVector::zero;
	// �v���C���[�܂ł̋���
	float dist = 9999.0f;
	if (player != nullptr)
	{
		playerPos = player->Position() + CVector(0.0f, 5.0f, 0.0f);
		playerVec = playerPos - Position();
		dist = playerVec.Length();
	}
	CDebugPrint::Print(" �o���l����:%f\n", dist);


	// ��Ԃɍ��킹�Ĉړ�������؂�ւ���
	switch (mState)
	{
		// �o���l�𗎂Ƃ������̏���
	    case EState::Drop:
			// �o���l���z������͈͓��������́A
			// ��莞�Ԃ��o�߂��Ă�����A�z����Ԃֈڍs
			if (mElapsedTime >= 1.0f && dist <= 20.0f
				|| mElapsedTime>=3.0f)
			{
				mState = EState::Absorb;
				mMoveSpeed = CVector::zero;
				// �z����Ԃł͒n�ʂƂ̓����蔻��͖���
				//mpColliderSphere->SetEnable(false);
			}
			else
			{
				// �d�͂����Z
				mMoveSpeed.Y(mMoveSpeed.Y() - 2.0f);
				// ���W���ړ����x���ړ�������
				Position(Position() + mMoveSpeed * Time::DeltaTime());
			}
			// �o�ߎ��Ԃ��v��
			mElapsedTime += Time::DeltaTime();
			break;
		// �o���l���z������鎞�̏���
		case EState::Absorb:
			// �����x
			float accelSpeed = 200.0f * Time::DeltaTime();
			// ���݂̈ړ����x + �����x �Ō��݂̈ړ����x���v�Z
			float moveSpeed = mMoveSpeed.Length() + accelSpeed;

			// �v���C���[�̕����ւ̈ړ��x�N�g�����v�Z
			mMoveSpeed = playerVec.Normalized() * moveSpeed;
			CVector currMoveSpeed = mMoveSpeed * Time::DeltaTime();
			// �v���C���[�܂ł̋������ړ��������Z���Ȃ��
			if (dist <= currMoveSpeed.Length())
			{
				// �o���l���擾
				
				// �o���l�͍폜
				//Kill();
			}
			else
			{
				// �v���C���[�̕����ֈړ�
				Position(Position() + currMoveSpeed);
			}
			break;
	}
}