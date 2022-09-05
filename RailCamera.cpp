#include "RailCamera.h"
#include <math.h>

using namespace MathUtility;

void RailCamera::Initialize() {
	//カメラの位置を設定する
	worldTransform_.translation_ = Vector3(0.0f, 0.0f, -50.0f);
	//カメラの回転を設定する
	worldTransform_.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.farZ = 1000.0f;
	viewProjection_.Initialize();

	input_ = Input::GetInstance();
}

void RailCamera::Update() {
	//カメラを後ろに動かす
	//worldTransform_.translation_.z += 0.1f;
	Move();
	worldTransform_.Transform();

	viewProjection_.eye = worldTransform_.translation_;
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = Vector3TransformNormal(forward, worldTransform_.matWorld_);
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映
	viewProjection_.up = Vector3TransformNormal(up, worldTransform_.matWorld_);

	//ビュープロジェクションの更新
	viewProjection_.UpdateMatrix();
	//定数バッファへの転送
	viewProjection_.TransferMatrix();
}

void RailCamera::Move() {
	float moveSpeed = 0.5f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= moveSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += moveSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += moveSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= moveSpeed;
	}

	//移動限界座標
	const float kMoveLimitX = 60;
	const float kMoveLimitY = 30;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}