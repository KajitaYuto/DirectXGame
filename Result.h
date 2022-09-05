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
class Result {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Result();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Result();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(int* enemyKill,int* Damage);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	int* GetMostKill() { return &mostKillNum_; }
	int* GetLeastDamage() { return &leastDamage_; }

	void SetScene(int* scene) { sceneNum_ = scene; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//リザルト用
	int* enemyKillNum_ = nullptr;
	int* totalDamage_ = nullptr;

	int mostKillNum_ = 0;
	int leastDamage_ = 0;

	int* sceneNum_ = nullptr;

};
