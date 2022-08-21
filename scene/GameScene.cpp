#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ファイル名を指定してテクスチャを読み込み
	pTextureHandle_ = TextureManager::Load("mario.jpg");
	eTextureHandle_ = TextureManager::Load("bakuhatsu4.png");
	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//スクリプトの読み込み
	LoadEnemyPopData();

	//自キャラの生成
	player_ = new Player;
	//自キャラの初期化
	player_->Initialize(model_, pTextureHandle_);

	//天球の生成
	skydome_ = new Skydome;
	//天球の初期化
	skydome_->Initialize(modelSkydome_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	//視点の移動ベクトル
	Vector3 move = { 0,0,0 };

	//視点の移動速さ
	const float kEyeSpeed = 0.2f;

	//視点移動(ベクトルの加算)
	viewProjection_.eye += move;

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_P)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
#endif

	//カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//転送
		viewProjection_.TransferMatrix();
	}
	else {
		//行列の再計算
		viewProjection_.UpdateMatrix();
		//転送
		viewProjection_.TransferMatrix();
	}

	//スクリプトの更新
	UpdateEnemyPopCommands();
	
	//キャラの更新
	player_->Update();
	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
	}
	//デスフラグの立った敵弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});
	//敵弾攻撃処理
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}

	//天球の更新
	skydome_->Update();

	CheckAllCollisions();
}

void GameScene::Draw() {

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
	player_->Draw(viewProjection_);
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullet();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		float dx = posA.x - posB.x;//(X2 - X1)
		float dy = posA.y - posB.y;//(Y2 - Y1)
		float dz = posA.z - posB.z;//(Z2 - Z1)
		//(X2 - X1)^2 + (Y2 - Y1)^2 + (Z2 - Z1)^2
		float d = dx * dx + dy * dy + dz * dz;
		//( R1 + R2) * (R1 + R2)
		float r = (Player::kRadius + EnemyBullet::kRadius) * (Player::kRadius + EnemyBullet::kRadius);
		//球と球の交差判定
		if (d <= r) {
			//自キャラの衝突時コールバック関数を呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバック関数を呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		//敵キャラの座標
		posA = enemy->GetWorldPosition();

		//敵キャラと自弾すべての当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//自弾の座標
			posB = bullet->GetWorldPosition();

			float dx = posA.x - posB.x;//(X2 - X1)
			float dy = posA.y - posB.y;//(Y2 - Y1)
			float dz = posA.z - posB.z;//(Z2 - Z1)
			//(X2 - X1)^2 + (Y2 - Y1)^2 + (Z2 - Z1)^2
			float d = dx * dx + dy * dy + dz * dz;
			//( R1 + R2) * (R1 + R2)
			float r = (Enemy::kRadius + PlayerBullet::kRadius) * (Enemy::kRadius + PlayerBullet::kRadius);
			//球と球の交差判定
			if (d <= r) {
				//敵キャラの衝突時コールバック関数を呼び出す
				enemy->OnCollision();
				//自弾の衝突時コールバック関数を呼び出す
				bullet->OnCollision();
			}
		}
	}
	
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	//自弾全てと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets) {
		//自弾の座標
		posA = playerBullet->GetWorldPosition();

		//自弾と敵弾すべての当たり判定
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
			//敵弾の座標
			posB = bullet->GetWorldPosition();

			float dx = posA.x - posB.x;//(X2 - X1)
			float dy = posA.y - posB.y;//(Y2 - Y1)
			float dz = posA.z - posB.z;//(Z2 - Z1)
			//(X2 - X1)^2 + (Y2 - Y1)^2 + (Z2 - Z1)^2
			float d = dx * dx + dy * dy + dz * dz;
			//( R1 + R2) * (R1 + R2)
			float r = (PlayerBullet::kRadius + EnemyBullet::kRadius) * (PlayerBullet::kRadius + EnemyBullet::kRadius);
			//球と球の交差判定
			if (d <= r) {
				//自弾の衝突時コールバック関数を呼び出す
				player_->OnCollision();
				//敵弾の衝突時コールバック関数を呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::LoadEnemyPopData(){
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::PopEnemy(Vector3 position){
	//敵キャラの生成
	std::unique_ptr<Enemy>newEnemy = std::make_unique<Enemy>();
	//敵キャラの初期化
	newEnemy->Initialize(model_,position, eTextureHandle_);
	//敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);
	//敵キャラにゲームシーンのアドレスを渡す
	newEnemy->SetGameScene(this);
	//敵キャラを登録する
	enemys_.push_back(std::move(newEnemy));
}

void GameScene::UpdateEnemyPopCommands(){
	if (isWait_) {
		waitTimer_--;
		if (waitTimer_ <= 0)isWait_ = false;
		return;
	}

	std::string line;

	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ',');
		
		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			PopEnemy(Vector3(x,y,z));
		}
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWait_ = true;
			waitTimer_ = waitTime;

			break;
		}
	}
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet) {
	enemyBullets_.push_back(std::move(enemyBullet));
}
