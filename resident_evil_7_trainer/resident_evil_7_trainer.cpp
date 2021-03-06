#include "stdafx.h"

#include <iostream>
#include <vector>
#include <Windows.h>

#include "proc.h"
#include "mem.h"
#include "ascii.h"

bool bHealth = false, bO2 = false, bAmmo = false, bGrenades = false, bItems = false, bMoney = false;

void Menu()
{

	std::cout << "HOTKEY = FUNCTIONS = STATUS\n";
	std::cout << "===========================\n";
	std::cout << "NUM0 = Infinite Health = " << bHealth << "\n";
	std::cout << "NUM1 = Infinite Oxygen = " << bO2 << "\n";
	std::cout << "NUM2 = Increase Ammo = " << bAmmo << "\n";
	std::cout << "NUM3 = Increase Grenades = " << bGrenades << "\n";
	std::cout << "NUM4 = Add Items = " << bItems << "\n";
	std::cout << "NUM5 = Infinite Money = " << bMoney << "\n\n";
	std::cout << "INSERT = EXIT TRAINER\n";

}

int main()
{

	HANDLE hProcess = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 13);

	uintptr_t moduleBase = 0, playerPtr = 0, playerPtr2 = 0, playerPtr3 = 0, healthAddr = 0, o2Addr = 0, moneyAddr = 0;

	const float maxHealth = 9999, maxO2 = 100;
	const int maxMoney = 999999;

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
		playerPtr3 = moduleBase + 0x081F11C8;
		healthAddr = FindDMAAddy(hProcess, playerPtr, { 0x28,0x28,0x70,0x24 });
		o2Addr = FindDMAAddy(hProcess, playerPtr2, { 0xD8,0x20 });
		moneyAddr = FindDMAAddy(hProcess, playerPtr3, { 0x6C });

		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			bHealth = !bHealth;

			ClearScreen();
			Menu();
		}
		if (bHealth)
		{
			mem::PatchEx((BYTE*)healthAddr, (BYTE*)&maxHealth, sizeof(maxHealth), hProcess);
		}

		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bO2 = !bO2;
			
			ClearScreen();
			Menu();
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
				mem::PatchEx((BYTE*)(moduleBase + 0x41BA07), (BYTE*)"\x01\xC6", 2, hProcess);
				ClearScreen();
				Menu();
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x41BA07), (BYTE*)"\x29\xC6", 2, hProcess);
				ClearScreen();
				Menu();
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD5) & 1)
		{
			bMoney = !bMoney;

			ClearScreen();
			Menu();
		}
		if (bMoney)
		{
			mem::PatchEx((BYTE*)moneyAddr, (BYTE*)&maxMoney, sizeof(maxMoney), hProcess);
		}


		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			return(0);
		}

		Sleep(100);

	}

	ClearScreen();
	std::cout << "Resident Evil 7 process not found, press enter to exit\n";
	getchar();
	return(0);

}