#include "RailCamera.h"
#include <math.h>

using namespace MathUtility;

void RailCamera::Initialize(){
	//カメラの位置を設定する
	worldTransform_.translation_ = Vector3(0.0f, 0.0f, -50.0f);
	//カメラの回転を設定する
	worldTransform_.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.farZ=1000.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update(){
	//カメラを後ろに動かす
	worldTransform_.translation_.z -= 0.1f;
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
