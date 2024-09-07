#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

GameScene::GameScene() {
	//現在のシーンを設定
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
	

	// インスタンスの生成
	gameCamera_ = std::make_unique<GameCamera>();
	wall_ = std::make_unique<Wall>();
	player_ = std::make_unique<Player>();

	// インスタンス初期化
	gameCamera_->Initialize();
	wall_->Initialize();
	//player_->Initialize(playerModels_);
	


}

void GameScene::Update() {
	// カメラの更新処理
	gameCamera_->Update();
	viewProjection_.matView = gameCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = gameCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の更新と転送
	viewProjection_.TransferMatrix();

	//オブジェクトの更新処理
	wall_->Update();















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

	//モデル
	wall_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::LoadEnemyPopData(const string& fileName) {
	ifstream file;
	file.open(fileName);
	assert(file.is_open());

	enemyPopCommands.clear();

	enemyPopCommands << file.rdbuf();

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

	while (getline(enemyPopCommands, command)) {
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

	enemies_.push_back(move(enemy));
}
