#include "Score.h"
#include "Mof.h"
#include <iostream>
#include <fstream>
int CScore::m_Score;
int CScore::m_HighScore;
using namespace std;

void CScore::Save()
{
	if (m_HighScore > m_Score)
	{
		return;
	}
	m_HighScore = m_Score;
	ofstream file;
	file.open("score.dat", ios::out|ios::binary);

	if (!file)
	{
		MOF_PRINTLOG("Failed to save score\n");
		return;
	}

	file.write((char*)&m_Score, sizeof(int));
	file.close();
	MOF_PRINTLOG("score saved\n");
}

void CScore::Reset()
{
	m_Score = 0;
	//m_HighScore = 0;
}

void CScore::AddScore(int value)
{
	m_Score += value;
}

void CScore::LoadSavedScore()
{
	std::ifstream file;
	file.open("score.dat", ios::in|ios::binary);

	if (!file)
	{
		Reset();
		return;
	}
	while (!file.eof())
	{
		file.read((char*)&m_HighScore, sizeof(int));
	}
	file.close();
}

int CScore::GetSavedScore()
{
	std::ifstream file;
	file.open("score.dat", ios::in | ios::binary);

	if (!file)
		return 0;
	int temp = 0;
	while (!file.eof())
	{
		file.read((char*)&temp, sizeof(int));
	}

	file.close();
	return temp;
}
