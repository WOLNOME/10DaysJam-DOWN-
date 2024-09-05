#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "sstream"
#include "fstream"
#include <cassert>
#include "enemy.h"

using namespace std;

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
	/// 敵の生成関数
	/// </summary>
	void CreateEnemy(const int& enemyType, const Vector3& position);
	void LoadEnemyPopData(const string& fileName);
	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// 敵リスト
	list<unique_ptr<Enemy>> enemies_;

	// 敵モデル
	unique_ptr<Model> enemyModel1_;
	unique_ptr<Model> enemyModel2_;
	vector<Model*> enemyModels_;

	// 敵発生コマンド
	stringstream enemyPopCommands;

	// 敵発生待機時間
	bool isWaiting_ = false;
	int waitTimer_ = 0;
};
