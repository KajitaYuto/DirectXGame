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

	//天球の生成
	skydome_ = new Skydome;
	//天球の初期化
	skydome_->Initialize(modelSkydome_);
	skydome_->SetTranslation(Vector3(0.0f, 0.0f, 0.0f));

	uint32_t texture_ = TextureManager::Load("Title.png");
	title_.reset(Sprite::Create(texture_, { 640,150 }, { 1,1,1,1 }, { 0.5f,0.5f }));
	texture_ = TextureManager::Load("Title2.png");
	title2_.reset(Sprite::Create(texture_, { 640,600 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.rotation_.y = 270.0f* 3.141592654f/180;
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
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
	//デバッグ用表示
	debugText_->SetPos(20,680);
	debugText_->Printf("No1 Kill:%d taken Damage:%d", *mostKillNum_, *leastDamage_);

	//天球の更新
	domeRot_.y += 0.0005f;
	if (domeRot_.y >= 360.0f * 3.141592654f / 180)domeRot_.y = 0;
	skydome_->SetRotation(domeRot_);
	skydome_->Update();

	//行列の再計算
	viewProjection_.UpdateMatrix();

	worldTransform_.translation_ = { viewProjection_.target.x+10.0f,viewProjection_.target.y - 5.0f,viewProjection_.target.z + 2.0f };
	worldTransform_.Transform();

	if (input_->PushKey(DIK_SPACE)) {
		*sceneNum_=1;
	}
}

void Title::Draw(){
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_);
	skydome_->Draw(viewProjection_);

	////ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->DrawLine3d();
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	title_->Draw();
	if(isWait_)title2_->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
