#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {
	// 現在のシーンを設定
	NextScene = Game;
}

GameScene::~GameScene() {}

void GameScene::Initialize(Input* input, Audio* audio) {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = input;
	audio_ = audio;

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// スプライトテクスチャ

	// スプライトの生成

	// モデルテクスチャ

	// モデルの生成
	playerModel_.reset(Model::CreateFromOBJ("player"));
	playerModels_.push_back(playerModel_.get());
	enemyModel1_.reset(Model::CreateFromOBJ("boss_block"));
	enemyModels_.push_back(enemyModel1_.get());

	// インスタンスの生成
	gameCamera_ = std::make_unique<GameCamera>();
	wall_ = std::make_unique<Wall>();
	player_ = std::make_unique<Player>();
	obstacles_ = std::make_unique<Obstacles>();
	collisionManager_ = std::make_unique<CollisionManager>();

	// 他クラスの参照
	gameCamera_->SetParent(&player_->GetWorldTransform());
	obstacles_->SetGameScene(this);

	// インスタンス初期化
	gameCamera_->Initialize();
	wall_->Initialize();
	player_->Initialize(playerModels_);
	obstacles_->Initialize();

	gameCamera_->SetParent(&player_->GetWorldTransform());
	player_->SetWall(wall_.get());

	// enemyPopData読み込み
	LoadEnemyPopData("./csv/enemyPop.csv");

	// サウンドデータの読み込み
	gameSceneSoundDataHandle_ = audio_->LoadWave("./Sound/BGM/gameSceneBGM.wav");
	
	// 音声再生
	voiceHandle_ = audio_->PlayWave(gameSceneSoundDataHandle_, true);
	audio_->SetVolume(voiceHandle_, 0.2f);

	// カーソルを非表示
	ShowCursor(false);
}

void GameScene::Update() {
	// 音声が止まってたら再生
	if (audio_->IsPlaying(voiceHandle_)) {
		audio_->ResumeWave(voiceHandle_);
	}

	// 死亡した敵の削除
	enemies_.remove_if([](unique_ptr<Enemy>& enemy) {
		if (enemy->IsDead()) {
			enemy.reset();
			return true;
		}
		return false;
	});

	// 死亡した敵の弾の削除
	enemyBullets_.remove_if([](unique_ptr<EnemyBullet>& enemyBullet) {
		if (enemyBullet->IsDead()) {
			enemyBullet.reset();
			return true;
		}
		return false;
	});

	// プレイヤーの更新処理
	player_->Update(viewProjection_);

	// カメラの更新処理
	gameCamera_->Update();
	viewProjection_.matView = gameCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = gameCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の更新と転送
	viewProjection_.TransferMatrix();

	// オブジェクトの更新処理
	wall_->Update();
	obstacles_->Update();

	// 敵の生成処理
	UpdateEnemyPopCommands();

	// 敵の更新処理
	for (auto& enemy : enemies_) {
		enemy->Update();
	}

	// 敵の弾の更新処理
	for (auto& enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}

	// 当たり判定
	CheckAllCollision();


	switch (nowPhase) {
	case 0:
		// 休憩ポイントに着地したら休憩シーンへ
		if (wall_->GetIsLanding() && NextScene == Game) {
			// 音声停止
			audio_->StopWave(voiceHandle_);

			NextScene = Rest;
		}
		break;
	case 1:
		// 2回目の休憩ポイントに着地したらクリアシーンへ
		if (wall_->GetIsLanding() && NextScene == Game) {
			// 音声停止
			audio_->StopWave(voiceHandle_);

			NextScene = Clear;
		}
		break;
	default:
		break;
	}
	


#ifdef _DEBUG
	ImGui::Begin("GameSceneNow");

	ImGui::End();
#endif // _DEBUG
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

	// モデル
	wall_->Draw(viewProjection_);

	// 自キャラ
	player_->Draw(viewProjection_);

	// 敵
	for (auto& enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

	// 敵の弾
	for (auto& enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection_);
	}

	// レーザー
	obstacles_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	// 衝突マネージャーのクリア
	collisionManager_->Reset();
	// コライダーリストに登録
	collisionManager_->AddCollider(player_.get());
	collisionManager_->AddCollider(obstacles_.get());

	const list<PlayerBullet*>& playerBullets=player_->GetBullets();
	if (playerBullets.max_size() > 0) {
		for (auto& playerBullet : playerBullets) {
			collisionManager_->AddCollider(playerBullet);
		}
	}

	if (enemies_.max_size() > 0) {
		for (auto& enemy : enemies_) {
			collisionManager_->AddCollider(enemy.get());
		}
	}

	if (enemyBullets_.max_size() > 0) {
		for (auto& enemyBullet : enemyBullets_) {
			collisionManager_->AddCollider(enemyBullet.get());
		}
	}

	// 衝突判定処理
	collisionManager_->CheckAllCollisions();
}

void GameScene::LoadEnemyPopData(const string& fileName) {
	ifstream file;
	file.open(fileName);
	assert(file.is_open());

	enemyPopCommands_.clear();

	enemyPopCommands_ << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	// 待機処理
	if (isWaiting_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			isWaiting_ = false;
		}
		return;
	}

	std::string command;

	while (getline(enemyPopCommands_, command)) {
		std::istringstream line_stream(command);

		std::string word;
		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			Vector3 pos;
			int type;

			getline(line_stream, word, ',');
			pos.x = (float)stof(word.c_str());

			getline(line_stream, word, ',');
			pos.y = (float)stof(word.c_str());

			getline(line_stream, word, ',');
			pos.z = (float)stof(word.c_str());

			getline(line_stream, word, ',');
			type = atoi(word.c_str());

			CreateEnemy(type, pos);
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWaiting_ = true;
			waitTimer_ = waitTime;

			break;
		}
	}
}

void GameScene::CreateEnemy(const int& enemyType, const Vector3& position) {
	unique_ptr<Enemy> enemy = make_unique<Enemy>();

	enemy->Initialize(enemyModels_, enemyType, position);

	enemy->SetPlayer(player_.get());

	enemy->SetGameScene(this);

	enemies_.push_back(move(enemy));
}

void GameScene::CreateEnemyBullet(Model* model, const Vector3& pos, const Vector3& velocity) {
	unique_ptr<EnemyBullet> bullet = make_unique<EnemyBullet>();

	bullet->Initialize(model, pos, velocity);

	enemyBullets_.push_back(move(bullet));
}
