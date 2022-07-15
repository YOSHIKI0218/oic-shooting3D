#pragma once

#include "CStage.h"

float gStg1EnemyStartPosX[] = {
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,

	6.0f,
	6.0f,
	6.0f,
	6.0f,
	6.0f,
	6.0f,
	6.0f,
	6.0f,
	6.0f,
	6.0f,
};

float gStg1EnemyStartScroll[] = {
	100.0f, 
	100.0f + 50.0f,
	100.0f + 50.0f + 50.0f,
	100.0f + 50.0f + 50.0f + 50.0f,
	100.0f + 50.0f + 50.0f + 50.0f + 50.0f,
	100.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f,
	100.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f,
	100.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f,
	100.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f,
	100.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f,

	600.0f,
	600.0f + 50.0f,
	600.0f + 50.0f + 50.0f,
	600.0f + 50.0f + 50.0f + 50.0f,
	600.0f + 50.0f + 50.0f + 50.0f + 50.0f,
	600.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f,
	600.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f,
	600.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f,
	600.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f,
	600.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f + 50.0f,
};

float gStgEnemyStartPosX[] = {
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,
	-6.0f,

	6.0f,
	6.0f,
	6.0f,
	6.0f,
	6.0f
};

float gStgEnemyStartScroll[] = {
	100.0f,
	100.0f + 50.0f,
	100.0f + 50.0f + 50.0f,
	100.0f + 50.0f + 50.0f + 50.0f,
	100.0f + 50.0f + 50.0f + 50.0f + 50.0f,

	600.0f,
	600.0f + 50.0f,
	600.0f + 50.0f + 50.0f,
	600.0f + 50.0f + 50.0f + 50.0f,
	600.0f + 50.0f + 50.0f + 50.0f + 50.0f
};

EnemyStart gStg2EnemyStart = {
	20,
	gStg1EnemyStartPosX,
	gStg1EnemyStartScroll,
};

EnemyStart gStgEnemyStart = {
	10,
	gStgEnemyStartPosX,
	gStgEnemyStartScroll,
};
