#pragma once
#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class Title {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Title();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Title();

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

	void SetKillandDamage(int* mostKill, int* leastDamage) {
		mostKillNum_ = mostKill;
		leastDamage_ = leastDamage;
	};

	void SetScene(int* scene) { sceneNum_ = scene; }


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	//スプライト
	std::unique_ptr<Sprite>title_;
	std::unique_ptr<Sprite>title2_;
	//天球
	Skydome* skydome_ = nullptr;

	//スコア用
	int* mostKillNum_ = nullptr;
	int* leastDamage_ = nullptr;

	const float kRotSpeed = 0.5f;

	int* sceneNum_ = nullptr;

	//待機中フラグ
	bool isWait_ = true;
	const int kWaitTimer = 60;
	int waitTimer_ = kWaitTimer;

	Vector3 domeRot_ = { 0.0f,0.0f,0.0f };
};
