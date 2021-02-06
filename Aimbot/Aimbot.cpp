#include <iostream>
#include"csgo.h"
#include"Functions.h"
#include"Globals.h"
#include<thread>
#include<chrono>
#include<d3dx9.h>
using namespace std;
#define KeyDOWN -32768
#define KeyUP 0

int main()
{
	SetConsoleTitle("x64External coded by CODE");
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 10);
	float pfov, rfov1, rfov2;
	float psmooth, rsmooth1, rsmooth2;
	std::cout << "Enter pistol fov and smooth : \n";
	std::cin >> pfov >> psmooth;
	std::cout << "Enter rifle preaim fov and smooth : \n";
	std::cin >> rfov1 >> rsmooth1;
	std::cout << "Enter rifle afteraim fov and smooth : \n";
	std::cin >> rfov2 >> rsmooth2;
	if (AttachProcess("csgo.exe"))
	{
		std::cout << "Found Process csgo.exe\ninitializing cheat....\ncheat started....";
		client = GetModule("client.dll");
		engine = GetModule("engine.dll");
		clientstate = rpm<DWORD>(engine + csgo::dwClientState);
		bool wall = false;
		bool tbot = false;
		while (true)
		{
			localPlayer = rpm<DWORD>(client + csgo::dwLocalPlayer);
			localTeam = rpm<int>(localPlayer + csgo::m_iTeamNum);
			int pShotsFired = rpm<int>(localPlayer + csgo::m_iShotsFired);
			bhop();
			radar();
			cham();
			TBot();
			DWORD iCurWeaponAdress = rpm<DWORD>(localPlayer + csgo::m_hActiveWeapon) & 0xFFF;
			DWORD m_iBase = rpm<DWORD>(client + csgo::dwEntityList + (iCurWeaponAdress - 1) * 0x10);
			short id = rpm< short >(m_iBase + csgo::m_iItemDefinitionIndex);	
			if (GetAsyncKeyState(VK_F2) & 1)															//Wall F2 key & Q key toogle
			{
				wall = !wall;
			}
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)                                                            
			{
				glow();
			}
			if (wall)
			{
				glow();
			}
			if (pShotsFired<=1) //PREAIM
			{
				switch (id)
				{
					case 40: 
						if (checkIfScoped())
						{
							if (GetAsyncKeyState(VK_LBUTTON))
							{
								AimAtPlayer(GetClosestTarget(10), 1.f, 8);
							}
						}
						break;
					case 9:
						if (checkIfScoped())
						{
							if (GetAsyncKeyState(VK_LBUTTON))
							{
								AimAtPlayer(GetClosestTarget(10), 1.f, 5);
							}
						}
						break;
					case 1:
						if (GetAsyncKeyState(VK_LBUTTON))
						{
							pAimAtPlayer(GetClosestTarget(2) , 1.f, 8);
						}
						else
						{
							pAimAtPlayer(GetClosestTarget(3), 10.f, 8);
						}
						break;
					case 4:
					case 32:
					case 61:
					case 30:
					case 3:
					case 63:
						AimAtPlayer(GetClosestTarget(pfov),psmooth, 8);
						break;
					case 7:
					case 8:
					case 10:
					case 13:
					case 16:
					case 17:
					case 19:
					case 23:
					case 24:
					case 26:
					case 33:
					case 34:
					case 39:
					case 60:
						AimAtPlayer(GetClosestTarget(rfov1),rsmooth1, 8);
						break;
				}
			}
			else if (pShotsFired > 1) 														//AFTERAIM
			{
				switch (id)
				{
					case 7:
					case 8:
					case 10:
					case 13:
					case 16:
					case 17:
					case 19:
					case 23:
					case 24:
					case 26:
					case 33:
					case 34:
					case 39:
					case 60:																//ALL
						AimAtPlayer(GetClosestTarget(rfov2),rsmooth2, 8);
						break;
				}		
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	else
	{
		SetConsoleTextAttribute(h, 10);
		std:cout << "Process not found\n";
		system("pause");
	}
}