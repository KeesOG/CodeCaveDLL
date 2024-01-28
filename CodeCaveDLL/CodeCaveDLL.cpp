#include <Windows.h>


//Set base pointer and offsets:
DWORD* player_base; //Base
DWORD* game_base;   //Offset 1
DWORD* gold;        //Offset 2

//Return address is the original game instruction that resumes after the code cave execution.
DWORD ret_adress = 0x00CCAF90;

//Declare a function without any stackframe (no extra bytes)
__declspec(naked) void codecave() {

	//Run assembly code.
	__asm {
		pushad //Save all registers.
	}

	//Dereferencing base-pointer and offsets to reach plaers gold, and adding 500 to it.
	player_base = (DWORD*)0x17EF04C;
	game_base = (DWORD*)(*player_base + 0xA90);
	gold = (DWORD*)(*game_base + 0x4);
	*gold += 500;


	//Run assembly code.
	__asm {
		popad //Restore all registers.
		mov eax, dword ptr ds:[ecx] // original code.
		lea esi, dword ptr ds:[esi] // orignal code.
		jmp ret_adress // resumes rest orignal code.
	}
}

//Main DLL function that gets called on injecting DLL.
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	DWORD old_protect;
	unsigned char* hook_location = (unsigned char*)0x00CCAF8A;

	if (fdwReason == DLL_PROCESS_ATTACH) {
		MessageBox(0, L"Hack injected!\nright-click > terrain description\n\nby: KeesOG", L"Inject successful", 0); //MessageBox for debug/confirmation.
		VirtualProtect((void*)hook_location, 6, PAGE_EXECUTE_READWRITE, &old_protect); //Sets read/write for specific bytes to change (games opcode)
		*hook_location = 0xE9; //Location to start hook in games orignal code.
		*(DWORD*)(hook_location + 1) = (DWORD)&codecave - (DWORD)(hook_location + 5); //Getting codecave offset.
		*(hook_location + 5) = 0x90; //Add nop instruction to keep the same bytes in original code.
	}

	return true;
}