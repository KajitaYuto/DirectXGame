#include "Audio.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "Result.h"
#include "Title.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	GameScene* gameScene = nullptr;
	Result* result = nullptr;
	Title* title = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow("GE2B_06_カジタ_ユウト_AL3_skyFighter");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize();

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	//タイトルの初期化
	title = new Title();
	title->Initialize();
	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();
	//リザルトの初期化
	result = new Result();
	result->Initialize(gameScene->GetTotalKill(), gameScene->GetTotalDamage());

	int scene = 0;
	gameScene->SetScene(&scene);
	title->SetScene(&scene);
	result->SetScene(&scene);

	title->SetKillandDamage(result->GetMostKill(), result->GetLeastDamage());

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// 入力関連の毎フレーム処理
		input->Update();
		// ゲームシーンの毎フレーム処理
		switch (scene) {
		case 0:
			title->Update();
			break;
		case 1:
			gameScene->Update();
			break;
		case 2:
			result->Update();
			break;
		case 3:
			gameScene = new GameScene();
			gameScene->Initialize();
			result->Initialize(gameScene->GetTotalKill(), gameScene->GetTotalDamage());
			gameScene->SetScene(&scene);
			scene = 0;
			break;
		}

		// 軸表示の更新
		axisIndicator->Update();

		// 描画開始
		dxCommon->PreDraw();
		switch (scene) {
		case 0:
			title->Draw();
			break;
		case 1:
			// ゲームシーンの描画
			gameScene->Draw();
			break;
		case 2:
			result->Draw();
			break;
		}
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	SafeDelete(gameScene);
	SafeDelete(result);
	audio->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}