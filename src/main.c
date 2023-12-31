#include <psp2/kernel/modulemgr.h>
#include <taihen.h>

#define printf sceClibPrintf

static tai_hook_ref_t app_start_ref;

SceUID configHook = -1;

int fakeConfig(int arg1, int arg2) {
	*(unsigned char*)(arg1 + 804) = 'D';
	*(unsigned char*)(arg1 + 805) = 'e';
	*(unsigned char*)(arg1 + 806) = 'b';
	*(unsigned char*)(arg1 + 807) = 'u';
	*(unsigned char*)(arg1 + 808) = 'g';
	*(unsigned char*)(arg1 + 809) = 'M';
	*(unsigned char*)(arg1 + 810) = 'e';
	*(unsigned char*)(arg1 + 811) = 'n';
	*(unsigned char*)(arg1 + 812) = 'u';


	*(unsigned char*)(arg1 + 9) = 1;
	*(unsigned char*)(arg1 + 836) = 1;
	*(unsigned char*)(arg1 + 16) = 0;
	return arg1;
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args) {

	tai_module_info_t info;
	info.size = sizeof(tai_module_info_t);

	taiGetModuleInfo(TAI_MAIN_MODULE, &info);

	configHook = taiHookFunctionOffset(
		&app_start_ref,
		info.modid,
		0,			// Segment index
		0xD3A6A,  	// 0x810D3A6A
		1,			// ARM/THUMB
		fakeConfig
	);
	printf("configHook: %08x\n", configHook);
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args){
  if(configHook >= 0) {
    taiHookRelease(configHook, app_start_ref);
  }
  return SCE_KERNEL_STOP_SUCCESS;
}