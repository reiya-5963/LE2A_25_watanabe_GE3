#pragma once

//#include <memory>
#include <optional>

#include "BaseCharacter.h"

#include "input/Input.h"
#include "Wepon.h"

class LockOn;


/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseCharacter {
	// モデルの配列番号
	enum modelIndex {
		ModelIndexBody = 0,
		ModelIndexHead,
		ModelIndexL_Arm,
		ModelIndexR_Arm,
		ModelIndexWepon
	};

	// 状態
	enum class Behavior {
		kRoot,		// 通常
		kAttack,	// 攻撃
		kDash,		// ダッシュ
		kJump,		// ジャンプ中
	};

private:
	// ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用媒介変数
		uint32_t dashParameter_ = 0;
	};

	//
	struct WorkAttack {
		// 攻撃ギミックの媒介変数
		uint32_t attackParamater_ = 0;
		int32_t comboIndex = 0;
		int32_t inComboNext = 0;
		bool comboNext = false;
	};

	// 攻撃用定数
	struct ConstAttack {
		// 振りかぶりの字間<frame>
		uint32_t anticipationTime;
		// ための字間<frame>
		uint32_t changeTime;
		// 攻撃振りの字間<frame>
		uint32_t swingtime;
		// 硬直字間<frame>
		uint32_t recoveryTime;
		// 振りかぶりの移動速さ
		float anticipationSpeed;
		// ための移動速さ
		float changeSpeed;
		// 攻撃振りの移動速さ
		float swingSpeed;
	};
	
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(const std::vector<Model*>& models, const std::vector<Model*>& weponModels, Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection) override;


	void OnCollisionEnter(int object) override;
	//void OnCollisionExit() override;

	Vector3  GetWorldPosition() const override;
	
	/// <summary>
	/// 
	/// </summary>
	void SetViewProjection(const ViewProjection* viewProjection);
	
	void ApplyGlobalVariavles();

	void InitializeGlovalVariables();

	void WorldTransformInitialize();

	Wepon* GetWeponCollider() {
		if (behaviorRequest_ == Behavior::kAttack || !isAtkFinish_) {
			return wepon_.get();
		}
		else {
			return nullptr;
		}
	}

	bool IsRespown() { return isRespown_; }
	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

public: // メンバ関数
	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();

	// 浮遊ギミック更新
	void UpdateFloatingGimmick();

	// 腕ふりギミック初期化
	void InitializeArmGimmick();

	// 腕ふりギミック更新
	void UpdateArmGimmick();

	void InitializeAttackArmGimmick();

	void UpdateAttackArmGimmick();

	void InitializeAttackWeponGimmick();

	void UpdateAttackWeponGimmick();

public: // 通常状態
	void BehaviorRootInitialize();

	// 通常行動更新
	void BehaviorRootUpdate();

public: // 攻撃状態	
	void BehaviorAttackInitialize();

	// 攻撃行動更新
	void BehaviorAttackUpdate();

public: // ダッシュ状態	
	void BehaviorDashInitialize();

	// 攻撃行動更新
	void BehaviorDashUpdate();
public: // ダッシュ状態	
		void BehaviorJumpInitialize();

		// 攻撃行動更新
		void BehaviorJumpUpdate();
public:
	// コンボの数
	static const int ComboNum = 3;

private: // メンバ変数
	// コンボ定数表
	static const std::array<ConstAttack, ComboNum> kConstAttacks_;

	// ワールド変換データ
	//WorldTransform worldTransform_;
	WorldTransform worldTransform_body_;
	WorldTransform worldTransform_head_;
	WorldTransform worldTransform_l_arm_;
	WorldTransform worldTransform_r_arm_;
	std::unique_ptr<Wepon> wepon_;
	//WorldTransform worldTransform_wepon_;

	// キーボード入力
	Input* input_ = Input::GetInstance();;

	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動のサイクル<flame>
	float floatingPeriod_ = 60.0f;
	// 浮遊の振幅<m>
	float floatingAmplitude = 60.0f;

	// 腕ふりギミックの媒介変数
	float armParameter_ = 0.0f;
	// 腕ふりのサイクル<flame>
	float armPeriod_ = 60.0f;
	// 腕ふりの振幅<m>
	float armAmplitude = 60.0f;

	// 腕ふりギミックの媒介変数
	float weponParameter_ = 0.0f;
	// 腕ふりのサイクル<flame>
	float weponPeriod_ = 60.0f;
	// 腕ふりの振幅<m>
	float weponAmplitude = 60.0f;

	float jumpPower_ = 0.0f;
	float dash_ = 1.0f;
	int attackCount_ = 0;
	const int kMaxAtkCount = 40;

	bool isAttack_ = false;
	bool isAtkFinish_ = false;

	Behavior behavior_ = Behavior::kRoot;
	//
	std::optional<Behavior> behaviorRequest_ = std::nullopt;


	//bool isJump_ = false;
	bool isOnGround_ = false;

	WorkDash workDash_;

	WorkAttack workAttack_;
	

	bool isRespown_ = false;

	Vector3 velocity_ = {};

	const LockOn* lockOn_ = nullptr;

};