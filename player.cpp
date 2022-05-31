#include "player.h"

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULL�|�C���^�`�F�b�N
	assert(model);
	this->model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update(){


}

void Player::Draw(ViewProjection viewProjection){
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
