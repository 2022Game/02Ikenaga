#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"

#define MODEL_SLIME "Character\\Slime\\Slime.x"

#define PLAYER_HEIGHT 2.0f

// エネミーのインスタンス
CEnemy* CEnemy::spInstance = nullptr;

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_SLIME);

	// CXCharacterの初期化
	Init(model);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
}

CEnemy::~CEnemy()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}

	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

CEnemy* CEnemy::Instance()
{
	return spInstance;
}

//更新処理
void CEnemy::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;
}

// 衝突処理
void CEnemy::Collision(CCollider* self, CCollider* other, const CVector& adjust)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
}

// 描画
void CEnemy::Render()
{
	CXCharacter::Render();
}
