#include "Title.h"

Title::Title(){}

Title::~Title(){
	delete modelSkydome_;
}

void Title::Initialize(){
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	model_ = Model::CreateFromOBJ("player", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//�V���̐���
	skydome_ = new Skydome;
	//�V���̏�����
	skydome_->Initialize(modelSkydome_);
	skydome_->SetTranslation(Vector3(0.0f, 0.0f, 0.0f));

	uint32_t texture_ = TextureManager::Load("Title.png");
	title_.reset(Sprite::Create(texture_, { 640,150 }, { 1,1,1,1 }, { 0.5f,0.5f }));
	texture_ = TextureManager::Load("Title2.png");
	title2_.reset(Sprite::Create(texture_, { 640,600 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.rotation_.y = 270.0f* 3.141592654f/180;
	worldTransform_.Initialize();

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 1000.0f;
	viewProjection_.eye = { -10.0f,5.0f,-20.0f };
	viewProjection_.target = worldTransform_.translation_;
	viewProjection_.Initialize();
}

void Title::Update(){
	waitTimer_--;
	if (waitTimer_ < 0) {
		waitTimer_ = kWaitTimer;
		isWait_ = !isWait_;
	}
	debugText_->SetScale(1.5f);
	//�f�o�b�O�p�\��
	debugText_->SetPos(20,680);
	debugText_->Printf("No1 Kill:%d taken Damage:%d", *mostKillNum_, *leastDamage_);

	//�V���̍X�V
	domeRot_.y += 0.0005f;
	if (domeRot_.y >= 360.0f * 3.141592654f / 180)domeRot_.y = 0;
	skydome_->SetRotation(domeRot_);
	skydome_->Update();

	//�s��̍Čv�Z
	viewProjection_.UpdateMatrix();

	worldTransform_.translation_ = { viewProjection_.target.x+10.0f,viewProjection_.target.y - 5.0f,viewProjection_.target.z + 2.0f };
	worldTransform_.Transform();

	if (input_->PushKey(DIK_SPACE)) {
		*sceneNum_=1;
	}
}

void Title::Draw(){
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
	model_->Draw(worldTransform_, viewProjection_);
	skydome_->Draw(viewProjection_);

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
	title_->Draw();
	if(isWait_)title2_->Draw();

	// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);
	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion
}
