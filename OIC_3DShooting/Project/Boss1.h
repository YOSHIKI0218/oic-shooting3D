#pragma once
#include "GameDefine.h"
#include "EnemyShot.h"

//ボス1
class CBoss1
{
private:
	CMeshContainer		m_Mesh;			//ボス本体のメッシュ
	CMeshContainer		m_ShieldMesh;	//シールドメッシュ
	Vector3				m_Pos;			//座標
	Vector3				m_Rot;			//回転角度（ラジアン）1radは約57.29deg
	CMatrix44			m_matWorld;		//全体のワールドマトリクス
	CMatrix44			m_matShield;	//シールドの行列(マトリクス)
	CMatrix44			m_matScale;		//スケール行列
	bool				m_bShow;		//表示状態

	int					m_MaxHP;		//最大体力
	int					m_HP;			//体力
	int					m_ShotWait;		//弾発射の待ち時間
	int					m_ShotInterval;//弾発射のインターバル
	Vector3				m_TargetPos;	//各回転アニメーション終了後に向く座標
	float				m_AnimTime;		//回転アニメーションの経過時間

	bool				m_bDestroyed;//破壊されたか

	int					m_CurrentShotPattern;//現在の弾発射パターン番号
	int					m_PrevShotPattern;//直前の発射パターン
	int					m_ExplosionInterval;//爆発の間隔

	float				m_TargetAngleP;//ターゲット角度(+)
	float				m_TargetAngleM;//ターゲット角度(-)

	float				m_ChangeTime;//パターン変更に使用する時間
	float				m_TimeElapsed;//開始後の経過時間
	bool				m_bDestroyedAndRemoved;//破壊されて消滅したか
	bool				m_bFlip;//回転中フラグ
	float				m_FlipTime;//回転に使用する時間
	bool				m_bUsedShield;//シールド展開したことがあるかどうか
	float				m_ShieldTime;//シードル展開残り時間
	float				m_MeshScale;//メッシュのスケール
	int					m_BossType;//ボスの種類。0でステージ2のボス。1でOiC
	float				m_PatternChangeInterval;//攻撃パターン変更間隔
	float				m_ExtremeAttackWaitTime;//強攻撃前の待ち時間
	CMeshContainer		m_AttackAreaMesh;//攻撃範囲のメッシュ
	int					m_AttackAreaTime;//攻撃範囲の点滅用
	bool				m_bShowAttackArea;//攻撃範囲の点滅用

	void UpdateShot(CEnemyShot* shot, int smax);
public:
	/**
	 * 読み込み
	 *
	 */
	bool Load(LPCMofChar modelName);


	/**
	 * 初期化
	 *
	 */
	void Initialize();

	/**
	 * 開始
	 *
	 */
	void Start();

	/**
	 * 更新
	 *
	 */
	void Update(CEnemyShot* shot, int smax);

	/**
	 * ダメージ処理
	 *
	 */
	void Damage(int dmg, const Vector3& bulletPos);

	/**
	 * 描画
	 *
	 */
	void Render();

	/**
	 * デバッグ描画
	 *
	 */
	void RenderDebug();

	/**
	 * 表示状態取得
	 *  戻り値がtrueなら表示中、falseなら非表示
	 */
	bool GetShow() { return m_bShow; }

	/**
	 * 衝突球を返す
	 *
	 */
	CSphere GetSphere();

	/**
	 * ターゲット座標の設定
	 *
	 */
	void SetTargetPos(const Vector3& t) { m_TargetPos = t; }

	/**
 * 解放
 *
 */
	void Release();

	/**
	 * 相手の方向へ回転する
	 *
	 */
	void RotateTarget();

	/// <summary>
	/// シールドの更新
	/// </summary>
	void UpdateShield();

	/**
	 * 全方位への弾発射
	 *
	 */
	void ShotAllDirShot(CEnemyShot* shot, int smax, int sCnt);

	/// <summary>
	/// 円形発射
	/// </summary>
	/// <param name="shot">敵弾の配列のポインタ</param>
	/// <param name="smax">弾の最大数</param>
	/// <param name="sCnt">発射数。sCnt×2個交互に発射する</param>
	void ShotCircleDir(CEnemyShot* shot, int smax, int sCnt);

	/// <summary>
	/// サイン発射
	/// </summary>
	/// <param name="shot">敵弾の配列のポインタ</param>
	/// <param name="smax">弾の最大数</param>
	/// <param name="sCnt">発射数。sCnt×2個の弾を発射する</param>
	void ShotSineWave(CEnemyShot* shot, int smax, int sCnt);

	/// <summary>
/// 並行発射(強)
/// </summary>
/// <param name="shot">敵弾の配列のポインタ</param>
/// <param name="smax">弾の最大数</param>
/// <param name="sCnt">0で一つだけ、1以上で1 + sCnt×2個の弾を発射する</param>
	void ShotExtremeParallel(CEnemyShot* shot, int smax, int sCnt);

	/// <summary>
	/// 並行発射
	/// </summary>
	/// <param name="shot">敵弾の配列のポインタ</param>
	/// <param name="smax">弾の最大数</param>
	/// <param name="sCnt">0で一つだけ、1以上で1 + sCnt×2個の弾を発射する</param>
	void ShotParallel(CEnemyShot* shot, int smax, int sCnt);

	/// <summary>
	/// 指定数発射
	/// </summary>
	/// <param name="shot">敵弾の配列のポインタ</param>
	/// <param name="smax">弾の最大数</param>
	/// <param name="sCnt">0で一つだけ、1以上で1 + sCnt×2個の弾を発射する</param>
	/// <param name="horming">ホーミングするかどうか</param>
	void Shot(CEnemyShot* shot, int smax, int sCnt, bool horming);

	/// <summary>
	/// 破壊され、消滅したかどうか
	/// </summary>
	/// <returns></returns>
	bool IsDestroyedAndRemoved() { return m_bDestroyedAndRemoved; }

	/// <summary>
	/// ボス1の現在のHPを取得する
	/// </summary>
	/// <returns>ボス1の現在のHP</returns>
	int GetHP() { return m_HP; }

	/// <summary>
	/// ボス1の最大HPを取得する
	/// </summary>
	/// <returns>ボス1の最大HP</returns>
	int GetMaxHP() { return m_MaxHP; }

	/// <summary>
	/// メッシュのスケールを設定する。
	/// </summary>
	/// <param name="value">ボスのスケール</param>
	void SetMeshScale(float value);

	/// <summary>
	/// ボスの種類を設定する
	/// </summary>
	/// <param name="value">値</param>
	void SetBossType(int value) { m_BossType = value; }

	/// <summary>
	/// ボスの種類を取得する。
	/// </summary>
	/// <returns>ボスの種類</returns>
	int GetBossType() { return m_BossType; }
};

