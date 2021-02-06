#include "Functions.h"
#include"Globals.h"
#include"csgo.h"
#include<iostream>
using namespace std;
#define PI 3.14159265359f


bool AttachProcess(const char* processName)
{
	PROCESSENTRY32 procEntry32;
	procEntry32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcSnap == INVALID_HANDLE_VALUE)
		return false;

	while (Process32Next(hProcSnap, &procEntry32))
	{
		if (!strcmp(processName, procEntry32.szExeFile))
		{
			HANDLE temp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
			if (temp == INVALID_HANDLE_VALUE) {
				CloseHandle(hProcSnap);
				return false;
			}

			pID = procEntry32.th32ProcessID;
			pHandle = temp;

			CloseHandle(hProcSnap);
			return true;
		}
	}

	CloseHandle(hProcSnap);
	return false;
}

DWORD GetModule(const char* moduleName)
{
	HANDLE hSnapShot;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);

	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(MODULEENTRY32);

	if (hSnapShot == INVALID_HANDLE_VALUE) {
		CloseHandle(hSnapShot);
		return 0;
	}

	if (Module32First(hSnapShot, &mEntry))
	{
		if (!strcmp(moduleName, mEntry.szModule))
		{
			CloseHandle(hSnapShot);
			return (DWORD)mEntry.modBaseAddr;
		}
	}

	while (Module32Next(hSnapShot, &mEntry))
	{
		if (!strcmp(moduleName, mEntry.szModule))
		{
			CloseHandle(hSnapShot);
			return (DWORD)mEntry.modBaseAddr;
		}
	}
	CloseHandle(hSnapShot);
	return false;
}

void bhop()
{
	int pFlag = rpm<int>(localPlayer + csgo::m_fFlags);
	vector3 pVel = rpm<vector3>(localPlayer + csgo::m_vecVelocity);
	int pMag = pVel.x + pVel.y + pVel.z;
	if (pMag != 0)
	{
		if (pFlag == 257 && GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			wpm<int>(5, client + csgo::dwForceJump);
		}
		else if (pFlag == 263 && GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			wpm<int>(5, client + csgo::dwForceJump);
		}
		else if (pFlag == 1281 && GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			wpm<int>(5, client + csgo::dwForceJump);
		}
		else if (pFlag == 1287 && GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			wpm<int>(5, client + csgo::dwForceJump);
		}
		else
		{
			wpm<int>(4, client + csgo::dwForceJump);
		}
	}
}

DWORD GetClosestTarget(float fov)
{
	D3DXVECTOR3 viewaAgles = rpm< D3DXVECTOR3>(clientstate + csgo::dwClientState_ViewAngles);
	D3DXVECTOR3 localEyePos = GetLocalEyePos();
	DWORD bestEntity = 0;
	for (int i = 1; i <= 64; i++)
	{
		DWORD cEnt = GetEntFromIndex(i);
		int entTeam = rpm<int>(cEnt + csgo::m_iTeamNum);
		if (cEnt != NULL && IsValid(cEnt) && entTeam != localTeam && cEnt != localPlayer && EntIsVisible(cEnt, localPlayer))
		{
			D3DXVECTOR3 angle = CalcAngle(localEyePos, GetBonePos(cEnt, 8));
			D3DXVECTOR3 Cangles = ClampAngles(angle - viewaAgles);
			float delta = sqrt(Cangles.x * Cangles.x + Cangles.y * Cangles.y);
			if (delta < fov)
			{
				fov = delta;
				bestEntity = cEnt;
			}
		}
	}
	return bestEntity;
}

D3DXVECTOR3 ClampAngles(D3DXVECTOR3 angle)
{
	while (angle.x < -180.0f)
		angle.x += 360.0f;

	while (angle.x > 180.0f)
		angle.x -= 360.0f;

	if (angle.x > 89.0f)
		angle.x = 89.0f;

	if (angle.x < -89.0f)
		angle.x = -89.0f;

	while (angle.y < -180.0f)
		angle.y += 360.0f;

	while (angle.y > 180.0f)
		angle.y -= 360.0f;

	angle.z = 0.0f;

	return angle;
}

bool AimAtPlayer(DWORD player, float smooth, int bone)
{
	if (player != NULL)
	{
		D3DXVECTOR3 localEyePos = GetLocalEyePos();
		D3DXVECTOR3 enemyPos =GetBonePos(player, bone);
		D3DXVECTOR3 aimAngles = enemyPos-localEyePos;
		VectorAngles(aimAngles, aimAngles);
		aimAngles -= rpm<D3DXVECTOR3>(localPlayer + csgo::m_aimPunchAngle) * 2.0f;
		D3DXVECTOR3 cAngles = rpm<D3DXVECTOR3>(clientstate + csgo::dwClientState_ViewAngles);
		D3DXVECTOR3 delta = aimAngles - cAngles;
		if (delta.x < -180.0f) delta.x += 360.0f;
		if (delta.x > 180.0f) delta.x -= 360.0f;
		if (delta.y < -180.0f) delta.y += 360.0f;
		if (delta.y > 180.0f) delta.y -= 360.0f;
		D3DXVECTOR3 sAngles = ClampAngles(cAngles + (delta / smooth));
		wpm<D3DXVECTOR3>(sAngles, clientstate + csgo::dwClientState_ViewAngles);
		return true;
	}
	return false;
}

bool pAimAtPlayer(DWORD player, float smooth, int bone)
{
	if (player != NULL)
	{
		D3DXVECTOR3 localEyePos = GetLocalEyePos();
		D3DXVECTOR3 enemyPos = GetBonePos(player, bone);
		D3DXVECTOR3 aimAngles = enemyPos - localEyePos;
		VectorAngles(aimAngles, aimAngles);
		aimAngles -= rpm<D3DXVECTOR3>(localPlayer + csgo::m_aimPunchAngle) * 1.0f;
		D3DXVECTOR3 cAngles = rpm<D3DXVECTOR3>(clientstate + csgo::dwClientState_ViewAngles);
		D3DXVECTOR3 delta = aimAngles - cAngles;
		if (delta.x < -180.0f) delta.x += 360.0f;
		if (delta.x > 180.0f) delta.x -= 360.0f;
		if (delta.y < -180.0f) delta.y += 360.0f;
		if (delta.y > 180.0f) delta.y -= 360.0f;
		D3DXVECTOR3 sAngles = ClampAngles(cAngles + (delta / smooth));
		wpm<D3DXVECTOR3>(sAngles, clientstate + csgo::dwClientState_ViewAngles);
		return true;
	}
	return false;
}

D3DXVECTOR3 GetLocalEyePos()
{
	D3DXVECTOR3 localPos = rpm<D3DXVECTOR3>(localPlayer + csgo::m_vecOrigin);
	localPos += rpm<D3DXVECTOR3>(localPlayer + csgo::m_vecViewOffset);
	return localPos;
}

DWORD GetEntFromIndex(int i)
{
	return rpm<DWORD>(client + csgo::dwEntityList + (i * 0x10));
}

bool IsValid(DWORD player)
{
	bool dormant = rpm<bool>(player + csgo::m_bDormant);
	int hp = rpm<int>(player + csgo::m_iHealth);
	bool alive = false;
	if (hp > 0) alive = true;
	if (!dormant && alive)
	{
		return true;
	}
	else return false;
}

D3DXVECTOR3 CalcAngle(const D3DXVECTOR3& src, const D3DXVECTOR3& dst)
{
	//square root func faster than normal func youd use
	const auto sqrtss = [](float in)
	{
		__m128 reg = _mm_load_ss(&in);
		return _mm_mul_ss(reg, _mm_rsqrt_ss(reg)).m128_f32[0];
	};


	D3DXVECTOR3 angles;

	//getting delta between source and destination vectors
	D3DXVECTOR3 delta = src - dst;

	//finding the hypoteneuse using pythagoras theorem a squared + b squared = c squared
	//this gives us the vector to our enemy
	float hyp = sqrtss(delta.x * delta.x + delta.y * delta.y);

	//now we need to find the angle needed to aim at the vector (aim angles)
	angles.x = asinf(delta.z / hyp) * (180 / PI);
	angles.y = atanf(delta.y / delta.x) * (180 / PI) + !((*(DWORD*)&delta.x) >> 31 & 1) * 180;

	angles.z = 0;

	return angles;
}

void VectorAngles(D3DXVECTOR3 forward, D3DXVECTOR3& angles)
{
	float yaw;
	float pitch;

	if (forward.z == 0 && forward.x == 0)
	{
		yaw = 0;
		pitch = 270;
	}
	else
	{
		float tmp;
		yaw = (atan2(forward.y, forward.x) * 180 / PI);

		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward.x * forward.x + forward.y * forward.y);
		pitch = (atan2(-forward.z, tmp) * 180 / PI);

		if (pitch < 0)
			pitch += 360;
	}

	if (pitch > 180)
		pitch -= 360;
	else if (pitch < -180)
		pitch += 360;

	if (yaw > 180)
		yaw -= 360;
	else if (yaw < -180)
		yaw += 360;

	if (pitch > 89)
		pitch = 89;
	else if (pitch < -89)
		pitch = -89;

	if (yaw > 180)
		yaw = 180;
	else if (yaw < -180)
		yaw = -180;

	angles.x = pitch;
	angles.y = yaw;
	angles.z = 0;
}

D3DXVECTOR3 GetBonePos(DWORD player, int bone)
{
	Matrix3x4_t boneMatrix = rpm<Matrix3x4_t>(rpm<DWORD>(player + csgo::m_dwBoneMatrix) + bone * 0x30);
	return
	{
		boneMatrix.Matrix[0][3],
		boneMatrix.Matrix[1][3],
		boneMatrix.Matrix[2][3]
	};
}

bool EntIsVisible(DWORD ent, DWORD local)
{
	int mask = rpm<int>(ent + csgo::m_bSpottedByMask);
	int PBASE = rpm<int>(local + 0x64) - 1;
	return (mask & (1 << PBASE)) > 0;
}

void radar()
{

	for (short int i = 0; i < 64; i++)
	{
		DWORD entity = rpm<DWORD>(client + csgo::dwEntityList + i * 0x10);
		if (entity != NULL)
			wpm<bool>(true, entity + csgo::m_bSpotted);
	}
}

void glow()
{
	DWORD glowObject = rpm<DWORD>(client + csgo::dwGlowObjectManager);
	int myTeam = rpm<int>(localPlayer + csgo::m_iTeamNum);

	for (short int i = 0; i < 32; i++)
	{
		DWORD entity = rpm<DWORD>(client + csgo::dwEntityList + i * 0x10);
		if (entity != NULL)
		{
			int glowIndx = rpm<int>(entity + csgo::m_iGlowIndex);
			int entityTeam = rpm<int>(entity + csgo::m_iTeamNum);

			if (myTeam != entityTeam)
			{
				wpm<float>(255, glowObject + ((glowIndx * 0x38) + 0x4));      //r
				wpm<float>(255, glowObject + ((glowIndx * 0x38) + 0x8));      //g
				wpm<float>(0, glowObject + ((glowIndx * 0x38) + 0xC));    //b
				wpm<float>(0.3, glowObject + ((glowIndx * 0x38) + 0x10));   //opacity

			}
			wpm<bool>(true, glowObject + ((glowIndx * 0x38) + 0x24));
			wpm<bool>(false, glowObject + ((glowIndx * 0x38) + 0x25));
		}
	}

}

bool checkIfScoped()
{
	return rpm<bool>(localPlayer + csgo::m_bIsScoped);
}

void TBot()
{
	if ((GetAsyncKeyState(VK_MENU) & 0x8000))
	{
		int crosshair = rpm<int>(localPlayer + csgo::m_iCrosshairId);
		if (crosshair != 0 && crosshair < 64)
		{
			DWORD entity = rpm<DWORD>(client + csgo::dwEntityList + ((crosshair - 1) * 0x10));
			int eTeam = rpm<int>(entity + csgo::m_iTeamNum);
			int eHealth = rpm<int>(entity + csgo::m_iHealth);
			DWORD iCurWeaponAdress = rpm<DWORD>(localPlayer + csgo::m_hActiveWeapon) & 0xFFF;
			DWORD m_iBase = rpm<DWORD>(client + csgo::dwEntityList + (iCurWeaponAdress - 1) * 0x10);
			short id = rpm<short>(m_iBase + csgo::m_iItemDefinitionIndex);
			if (eTeam != localTeam && eHealth > 0 && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			{
				if (id == 40 || id == 9)
				{
					if (checkIfScoped())
					{
						wpm<int>(5, client + csgo::dwForceAttack);
						std::this_thread::sleep_for(std::chrono::milliseconds(20));
						wpm<int>(4, client + csgo::dwForceAttack);
					}
				}
				else
				{
					wpm<int>(5, client + csgo::dwForceAttack);
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
					wpm<int>(4, client + csgo::dwForceAttack);
				}
			}

		}
	}
}

void cham()
{
		float brightness = 10;
		DWORD glowObject = rpm<DWORD>(client + csgo::dwGlowObjectManager);
		int myTeam = rpm<int>(localPlayer + csgo::m_iTeamNum);
		for (short int i = 0; i < 64; i++)
		{
			
			DWORD entity = rpm<DWORD>(client + csgo::dwEntityList + i * 0x10);
			if (entity != NULL)
			{
				int glowIndx = rpm<int>(entity + csgo::m_iGlowIndex);
				int entityTeam = rpm<int>(entity + csgo::m_iTeamNum);
				int health = rpm<int>(entity + csgo::m_iHealth);

				if (myTeam != entityTeam && entity>1 && health>66)                                //green
				{
					wpm<int>((BYTE)0,entity + csgo::m_clrRender);
					wpm<int>((BYTE)255,entity + csgo::m_clrRender + 0x1);
					wpm<int>((BYTE)0,entity + csgo::m_clrRender + 0x2);
					wpm<int>((BYTE)255,entity + csgo::m_clrRender + 0x3);
				}
				else if (myTeam != entityTeam && entity > 1 && health <= 66 && health>=33)			//yellow
				{
					wpm<int>((BYTE)255, entity + csgo::m_clrRender);
					wpm<int>((BYTE)255, entity + csgo::m_clrRender + 0x1);
					wpm<int>((BYTE)0, entity + csgo::m_clrRender + 0x2);
					wpm<int>((BYTE)255, entity + csgo::m_clrRender + 0x3);
				}
				else if (myTeam != entityTeam && entity > 1 && health < 33)													//red
				{
					wpm<int>((BYTE)255, entity + csgo::m_clrRender);
					wpm<int>((BYTE)0, entity + csgo::m_clrRender + 0x1);
					wpm<int>((BYTE)0, entity + csgo::m_clrRender + 0x2);
					wpm<int>((BYTE)255, entity + csgo::m_clrRender + 0x3);
				}
				else
				{
					wpm<int>((BYTE)-1, entity + csgo::m_clrRender);
					wpm<int>((BYTE)-1, entity + csgo::m_clrRender + 0x1);
					wpm<int>((BYTE)-1, entity + csgo::m_clrRender + 0x2);
					wpm<int>((BYTE)-1, entity + csgo::m_clrRender + 0x3);
				}
				wpm<bool>(true, glowObject + ((glowIndx * 0x38) + 0x24));
			}
		}
		DWORD thisPtr = (int)(engine + csgo::model_ambient_min - 0x2c);
		DWORD xored = *(DWORD*)&brightness ^ thisPtr;
		wpm<int>(xored,engine +csgo::model_ambient_min);
}

