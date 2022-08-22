#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& rotation, const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("black.jpg");

	//ワールド変換の初期化
	worldTransform_.rotation_ = rotation;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {
	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;
	worldTransform_.Transform();

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection){
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
