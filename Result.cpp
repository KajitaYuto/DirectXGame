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
	//デバッグ用表示
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
