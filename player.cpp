#include "player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	this->model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {
	Move();

	debugText_->SetPos(50, 70);
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
		worldTransform_.translation_.x -= move.x;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += move.x;
	}
	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += move.y;
	}
	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= move.y;
	}

	//行列更新
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	//ワールド行列
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	//ワールド行列に平行移動行列を掛ける
	worldTransform_.matWorld_ *= matTrans;
	//ワールド行列の転送
	worldTransform_.TransferMatrix();

}

void Player::Rotate(){
	const float kRotSpeed = 0.05f;

	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

}
