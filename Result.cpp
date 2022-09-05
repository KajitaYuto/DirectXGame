#include "Result.h"

Result::Result(){}

Result::~Result(){

}

void Result::Initialize(int* enemyKill, int* Damage){
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	enemyKillNum_ = enemyKill;
	totalDamage_ = Damage;
}

void Result::Update(){
	if (mostKillNum_ <= *enemyKillNum_) {
		mostKillNum_ = *enemyKillNum_;
		leastDamage_ = *totalDamage_;
	}
	debugText_->SetScale(2.0f);
	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 50);
	debugText_->Printf("Total Kill:%d, Total Damage:%d", *enemyKillNum_, *totalDamage_);
	debugText_->SetPos(50, 80);
	debugText_->Printf("Most  Kill:%d, Damage:%d", mostKillNum_, leastDamage_);
	debugText_->SetPos(500, 500);
	debugText_->Printf("Back to Enter");

	if (input_->PushKey(DIK_RETURN)) {
		*sceneNum_ = 3;
	}
}

void Result::Draw(){
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(commandList);

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	Model::PreDraw(commandList);

	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>
	//3D���f���`��
	
	////���C���`�悪�Q�Ƃ���r���[�v���W�F�N�V�������w�肷��(�A�h���X�n��)
	//PrimitiveDrawer::GetInstance()->DrawLine3d();
	// 3D�I�u�W�F�N�g�`��㏈��
	Model::PostDraw();
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(commandList);

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);
	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion
}
