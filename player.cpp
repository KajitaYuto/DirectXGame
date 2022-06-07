#include "player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	this->model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update() {
	Move();

	debugText_->SetPos(50, 70);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x
		, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection) {
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Player::Move() {
	Vector3 move = { 0,0,0 };
	float moveSpeed = 5.0f;

	//�����������ňړ��x�N�g����ύX
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

	//�s��X�V
	//���s�ړ��s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	//���[���h�s��
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	//���[���h�s��ɕ��s�ړ��s����|����
	worldTransform_.matWorld_ *= matTrans;
	//���[���h�s��̓]��
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
