#pragma once
#include<Windows.h>

struct vector3
{
	float x, y, z;
};

extern DWORD pID;
extern HANDLE pHandle;
extern DWORD client;
extern DWORD localPlayer;
extern DWORD engine;
extern int localTeam;
extern DWORD clientstate;
extern 	int myWeaponID;

struct Matrix3x4_t
{
	float Matrix[3][4];
};




