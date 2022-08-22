#include "WorldTransform.h"

void WorldTransform::Transform(){
	Matrix4 matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = scale_.x;
	matScale.m[1][1] = scale_.y;
	matScale.m[2][2] = scale_.z;

	Matrix4 matRotZ = MathUtility::Matrix4Identity();
	matRotZ.m[0][0] = cosf(rotation_.z);
	matRotZ.m[0][1] = sinf(rotation_.z);
	matRotZ.m[1][0] = -sinf(rotation_.z);
	matRotZ.m[1][1] = cosf(rotation_.z);

	Matrix4 matRotX = MathUtility::Matrix4Identity();
	matRotX.m[1][1] = cosf(rotation_.x);
	matRotX.m[1][2] = sinf(rotation_.x);
	matRotX.m[2][1] = -sinf(rotation_.x);
	matRotX.m[2][2] = cosf(rotation_.x);

	Matrix4 matRotY = MathUtility::Matrix4Identity();
	matRotY.m[0][0] = cosf(rotation_.y);
	matRotY.m[0][2] = -sinf(rotation_.y);
	matRotY.m[2][0] = sinf(rotation_.y);
	matRotY.m[2][2] = cosf(rotation_.y);

	Matrix4 matTrans = MathUtility::Matrix4Identity();
	matTrans.m[3][0] = translation_.x;
	matTrans.m[3][1] = translation_.y;
	matTrans.m[3][2] = translation_.z;
	//ワールド行列
	matWorld_ = MathUtility::Matrix4Identity();
	//ワールド行列に平行移動行列を掛ける
	matWorld_ *= matScale;
	matWorld_ *= matRotZ;
	matWorld_ *= matRotX;
	matWorld_ *= matRotY;
	matWorld_ *= matTrans;
	if (parent_ != nullptr)matWorld_ *= parent_->matWorld_;
	//ワールド行列の転送
	TransferMatrix();
}