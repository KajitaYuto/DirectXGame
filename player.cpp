#include "player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	this->model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {
	Move();

	debugText_->SetPos(50, 50);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x
		, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection) {
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Player::Move() {
	Vector3 move = { 0,0,0 };
	float moveSpeed = 5.0f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		move = { -moveSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_D)) {
		move = { moveSpeed, 0, 0 };
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

}
