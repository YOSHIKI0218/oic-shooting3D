#pragma once

//�X�e�[�W�P�̏����`����

//�G�̏o���ʒu��`
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
//�X�N���[�������؂�ւ��^�C�~���O
float			gStg1ChangeMode[] = {
	600.0f,
	1250.0f,
};

//�G�̊J�n���̍\���̂��쐬����
ENEMYSTART gStg1EnemyStart = {
	15,
	gStg1EnemyStartPosX,
	gStg1EnemyStartScroll,
	2,
	gStg1ChangeMode,
};