﻿#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>enemyBullet);

	int* GetTotalKill() { return &totalKill_;}
	int* GetTotalDamage() {return &totalDamage_;}

	void SetScene(int* scene) { sceneNum_ = scene; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
	 
	//BGM
	uint32_t BGMHandle_ = 0;
	uint32_t roop_ = 0;

	//3Dモデル
	Model* modelPlayer_ = nullptr;
	Model* modelPlayerBullet_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelEnemyBullet_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;
	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//天球
	Skydome* skydome_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//待機中フラグ
	bool isWait_ = false;
	int waitTimer_ = 0;
	bool isLast_ = false;

	//リザルト用
	int totalKill_ = 0;
	int totalDamage_ = 0;

	int* sceneNum_=nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生関数
	/// </summary>
	void PopEnemy(Vector3 position, int Destination);

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();
};
