#pragma once
#include "BaseScene.h"
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
#include "GameCamera.h"
#include "Wall.h"
#include <memory>
#include "Script/Player.h"
#include "collisionManager.h"
#include "Obstacles.h"

using namespace std;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Input* input, Audio* audio) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	void CheckAllCollision();

public://ゲッター
	SCENE GetNextScene() override { return NextScene; }
	//インスタンス
	Wall* GetWall() { return wall_.get(); }

	/// <summary>
	/// 敵の生成関数
	/// </summary>
	void CreateEnemy(const int& enemyType, const Vector3& position);
	void LoadEnemyPopData(const string& fileName);
	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	//入力
	Input* input_ = nullptr;
	//音
	Audio* audio_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//インスタンス
	unique_ptr<GameCamera> gameCamera_ = nullptr;
	unique_ptr<Wall> wall_ = nullptr;
	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//衝突マネージャ―
	unique_ptr<CollisionManager> collisionManager_ = nullptr;
	
	// 自キャラ
	unique_ptr<Player> player_ = nullptr;

	// 自キャラモデル
	unique_ptr<Model> playerModel_;
	vector<Model*> playerModels_;

	// 敵リスト
	list<unique_ptr<Enemy>> enemies_;

	// 敵モデル
	unique_ptr<Model> enemyModel1_;
	unique_ptr<Model> enemyModel2_;
	vector<Model*> enemyModels_;

	//障害物（レーザー）
	unique_ptr<Obstacles> obstacles_ = nullptr;

	// 敵発生コマンド
	stringstream enemyPopCommands_;

	// 敵発生待機時間
	bool isWaiting_ = false;
	int waitTimer_ = 0;
};
