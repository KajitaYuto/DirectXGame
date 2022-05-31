#include "player.h"

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULLポインタチェック
	assert(model);
	this->model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update(){


}

void Player::Draw(ViewProjection viewProjection){
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
