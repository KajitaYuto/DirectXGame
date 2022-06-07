#include "playerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("black.jpg");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {
	Matrix4 matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0]=worldTransform_.scale_.x;
	matScale.m[1][1]=worldTransform_.scale_.y;
	matScale.m[2][2]=worldTransform_.scale_.z;

	Matrix4 matRotZ = MathUtility::Matrix4Identity();
	matRotZ.m[0][0] = cosf(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sinf(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sinf(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cosf(worldTransform_.rotation_.z);

	Matrix4 matRotX = MathUtility::Matrix4Identity();
	matRotX.m[1][1] = cosf(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sinf(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sinf(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cosf(worldTransform_.rotation_.x);

	Matrix4 matRotY = MathUtility::Matrix4Identity();
	matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
	matRotY.m[0][2] = -sinf(worldTransform_.rotation_.y);
	matRotY.m[2][0] = sinf(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);

	Matrix4 matTrans = MathUtility::Matrix4Identity();
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	//���[���h�s��
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	//���[���h�s��ɕ��s�ړ��s����|����
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRotZ;
	worldTransform_.matWorld_ *= matRotX;
	worldTransform_.matWorld_ *= matRotY;
	worldTransform_.matWorld_ *= matTrans;
	//���[���h�s��̓]��
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection){
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
