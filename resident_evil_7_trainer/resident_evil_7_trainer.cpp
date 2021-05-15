#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "mem.h"
#include "ascii.h"
bool bHealth = false, bO2= false, bAmmo = false, bGrenades = false, bItems = false;
void Menu()
{
	std::cout << "HOTKEY = FUNCTIONS = STATUS\n";
	std::cout << "===========================\n";
	std::cout << "NUM0 = INFINITE HEALTH = " << bHealth << "\n";
	std::cout << "NUM1 = INFINITE O2 = " << bO2 << "\n";
	std::cout << "NUM2 = INFINITE AMMO = " << bAmmo << "\n";
	std::cout << "NUM3 = INFINITE GRENADES = " << bGrenades << "\n";
	std::cout << "NUM4 = INFINITE ITEMS = " << bItems << "\n\n";
	std::cout << "INSERT = EXIT TRAINER\n";
}
int main()
{
	HANDLE hProcess = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 13);
	uintptr_t moduleBase = 0, playerPtr = 0, playerPtr2 = 0, healthAddr = 0, o2Addr = 0;
	const float maxHealth = 9999, maxO2 = 100;
	DWORD procId = GetProcId(L"re7.exe");
	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		moduleBase = GetModuleBaseAddress(procId, L"re7.exe");
		AsciiArt();
		getchar();
		ClearScreen();
		Menu();
	}
	else
	{
		std::cout << "Resident Evil 7 process not found, press enter to exit\n";
		getchar();
		return(0);
	}
	DWORD dwExit = 0;
	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		playerPtr = moduleBase + 0x081EA150;
		playerPtr2 = moduleBase + 0x081F01C0;
		healthAddr = FindDMAAddy(hProcess, playerPtr, { 0x28,0x28,0x70,0x24 });
		o2Addr = FindDMAAddy(hProcess, playerPtr2, { 0xD8,0x20 });

		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			bHealth = !bHealth;
			if (bHealth)
			{
				ClearScreen();
				Menu();
			}
			else
			{
				ClearScreen();
				Menu();
			}
		}
		if (bHealth)
		{
			mem::PatchEx((BYTE*)healthAddr, (BYTE*)&maxHealth, sizeof(maxHealth), hProcess);
		}
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bO2 = !bO2;
			if (bO2)
			{
				ClearScreen();
				Menu();
			}
			else
			{
				ClearScreen();
				Menu();
			}
		}
		if (bO2)
		{
			mem::PatchEx((BYTE*)o2Addr, (BYTE*)&maxO2, sizeof(maxO2), hProcess);
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bAmmo = !bAmmo;
			if (bAmmo)
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x608DCC), (BYTE*)"\x41\xFF\xC0", 3, hProcess);
				ClearScreen();
				Menu();
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x608DCC), (BYTE*)"\x41\xFF\xC8", 3, hProcess);
				ClearScreen();
				Menu();
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bGrenades = !bGrenades;
			if (bGrenades)
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x16874D5), (BYTE*)"\xFF\xC0", 2, hProcess);
				ClearScreen();
				Menu();
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x16874D5), (BYTE*)"\xFF\xC8", 2, hProcess);
				ClearScreen();
				Menu();
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD4) & 1)
		{
			bItems = !bItems;
			if (bItems)
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x41B9EA), (BYTE*)"\x89\xB7\x88\x00\x00\x00", 6, hProcess);
				ClearScreen();
				Menu();
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x41B9EA), (BYTE*)"\x8B\xB7\x88\x00\x00\x00", 6, hProcess);
				ClearScreen();
				Menu();
			}
		}
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			return(0);
		}
		Sleep(10);
	}
	ClearScreen();
	std::cout << "Resident Evil 7 process not found, press enter to exit\n";
	getchar();
	return(0);
}