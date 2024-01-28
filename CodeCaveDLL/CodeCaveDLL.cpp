#include <Windows.h>


DWORD ret_adress = 0x00CCAF90;

DWORD* player_base;
DWORD* game_base;
DWORD* gold;


__declspec(naked) void codecave() {
	__asm {
		pushad
	}

	player_base = (DWORD*)0x17EF04C;
	game_base = (DWORD*)(*player_base + 0xA90);
	gold = (DWORD*)(*game_base + 0x4);
	*gold += 500;

	__asm {
		popad
		mov eax, dword ptr ds:[ecx]
		lea esi, dword ptr ds:[esi]
		jmp ret_adress
	}
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	DWORD old_protect;
	unsigned char* hook_location = (unsigned char*)0x00CCAF8A;
	if (fdwReason == DLL_PROCESS_ATTACH) {
		MessageBox(0, 0, 0, 0);
		VirtualProtect((void*)hook_location, 6, PAGE_EXECUTE_READWRITE, &old_protect);
		*hook_location = 0xE9;
		*(DWORD*)(hook_location + 1) = (DWORD)&codecave - (DWORD)(hook_location + 5);
		*(hook_location + 5) = 0x90;
	}

	return true;
}