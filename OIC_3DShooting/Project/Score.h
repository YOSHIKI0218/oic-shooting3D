#pragma once
class CScore
{
public:
	static void Save();

	static void Reset();

	static void AddScore(int value);

	/// <summary>
	/// �X�R�A���擾����
	/// </summary>
	/// <returns>�X�R�A</returns>
	static int GetScore() { return m_Score; }

	static void LoadSavedScore();
	static int GetSavedScore();
	static int GetHighScore() { return m_HighScore; }

private:
	static int	m_Score;
	static int m_HighScore;
};

