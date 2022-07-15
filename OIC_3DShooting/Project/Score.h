#pragma once
class CScore
{
public:
	static void Save();

	static void Reset();

	static void AddScore(int value);

	/// <summary>
	/// スコアを取得する
	/// </summary>
	/// <returns>スコア</returns>
	static int GetScore() { return m_Score; }

	static void LoadSavedScore();
	static int GetSavedScore();
	static int GetHighScore() { return m_HighScore; }

private:
	static int	m_Score;
	static int m_HighScore;
};

