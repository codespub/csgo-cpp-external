#pragma once
#include<Windows.h>
#include<TlHelp32.h>
#include"Globals.h"
#include<d3dx9.h>
#include"csgo.h"
#include<xmmintrin.h>
#include<thread>
#include<chrono>

template <class dataType>
void wpm(dataType value, DWORD addy)
{
	WriteProcessMemory(pHandle, (PVOID)addy, &value, sizeof(dataType), 0);
}
template <class dataType>
dataType rpm(DWORD addy)
{
	dataType data;
	bool state = ReadProcessMemory(pHandle, (PVOID)addy, &data, sizeof(dataType), 0);
	if (state)
		return data;
}

bool AttachProcess(const char* processName);
DWORD GetModule(const char* moduleName);
void bhop();
DWORD GetClosestTarget(float fov);
bool AimAtPlayer(DWORD player, float smooth, int bone);
bool pAimAtPlayer(DWORD player, float smooth, int bone);
D3DXVECTOR3 GetLocalEyePos();
DWORD GetEntFromIndex(int i);
bool IsValid(DWORD player);
D3DXVECTOR3 CalcAngle(const D3DXVECTOR3& src, const D3DXVECTOR3& dst);
void VectorAngles(D3DXVECTOR3 forward, D3DXVECTOR3& angles);
D3DXVECTOR3 GetBonePos(DWORD player, int bone);
bool EntIsVisible(DWORD ent, DWORD local);
void radar();
void glow();
bool checkIfScoped();
D3DXVECTOR3 ClampAngles(D3DXVECTOR3 angle);
void TBot();
void cham();
void autopistol();
void NoRecoil();