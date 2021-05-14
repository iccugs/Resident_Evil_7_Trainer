#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "mem.h"
#include "ascii.h"
bool bHealth = false, bO2= false, bAmmo = false, bGrenades = false;
void Menu()
{
	std::cout << "HOTKEY = FUNCTIONS = STATUS\n";
	std::cout << "===========================\n";
	std::cout << "NUM0 = INFINITE HEALTH = " << bHealth << "\n";
	std::cout << "NUM1 = INFINITE O2 = " << bO2 << "\n";
	std::cout << "NUM2 = INFINITE AMMO = " << bAmmo << "\n";
	std::cout << "NUM3 = INFINITE GRENADES = " << bGrenades << "\n\n";
	std::cout << "INSERT = EXIT TRAINER\n";
}
int main()
{
	HANDLE hProcess = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 13);
	uintptr_t moduleBase = 0;
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
		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			bHealth = !bHealth;
			if (bHealth)
			{
				mem::NopEx((BYTE*)(moduleBase + 0xB1EE1F), 5, hProcess);
				ClearScreen();
				Menu();
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0xB1EE1F), (BYTE*)"\xF3\x0F\x11\x52\x24", 5, hProcess);
				ClearScreen();
				Menu();
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bO2 = !bO2;
			if (bO2)
			{
				mem::NopEx((BYTE*)(moduleBase + 0x21B528D), 4, hProcess);
				ClearScreen();
				Menu();
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x21B528D), (BYTE*)"\xF3\x0F\x11\x12", 4, hProcess);
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bAmmo = !bAmmo;
			if (bAmmo)
			{
				mem::NopEx((BYTE*)(moduleBase + 0x5E4FFB), 3, hProcess);
				ClearScreen();
				Menu();
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x5E4FFB), (BYTE*)"\x89\x5F\x24", 3, hProcess);
				ClearScreen();
				Menu();
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bGrenades = !bGrenades;
			if (bGrenades)
			{
				mem::NopEx((BYTE*)(moduleBase + 0x16874E7), 6, hProcess);
				ClearScreen();
				Menu();
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x16874E7), (BYTE*)"\x89\xBE\x88\x00\x00\x00", 6, hProcess);
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