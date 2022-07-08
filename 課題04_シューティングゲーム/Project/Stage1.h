#pragma once

//ステージ１の情報を定義する

//敵の出現位置定義
float			gStg1EnemyStartPosX[] = {
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	
	4.0f,
	2.0f,
	0.0f,
	-2.0f,
	-4.0f,
	
	6.0f,
	6.0f,
	6.0f,
	6.0f,
	6.0f,
};
float			gStg1EnemyStartScroll[] = {
	100.0f,
	150.0f,
	200.0f,
	250.0f,
	300.0f,
	
	600.0f,
	650.0f,
	700.0f,
	750.0f,
	800.0f,
	
	1250.0f,
	1300.0f,
	1350.0f,
	1400.0f,
	1450.0f,
};
//スクロール方向切り替えタイミング
float			gStg1ChangeMode[] = {
	600.0f,
	1250.0f,
};

//敵の開始情報の構造体を作成する
ENEMYSTART gStg1EnemyStart = {
	15,
	gStg1EnemyStartPosX,
	gStg1EnemyStartScroll,
	2,
	gStg1ChangeMode,
};