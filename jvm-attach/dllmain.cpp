#include "pch.h"
#include <windows.h>
#include "jni.h"
#include "utils.h"
#include "message.h"
#include <HookLib.h>


#pragma comment(lib, "HookLib.lib")

DWORD WINAPI MainThread(CONST LPVOID lpParam)
{
	HMODULE jvm = GetModuleHandlePeb(L"jvm.dll");

	MonitorNotify = (JVM_MonitorNotify)GetProcAddressPeb(jvm, "JVM_MonitorNotify");

}

typedef void(*JVM_MonitorNotify)(JNIEnv* env, jobject obj);

JVM_MonitorNotify MonitorNotify = NULL;

void MonitorNotify_Hook(JNIEnv* env, jobject obj) {
	MonitorNotify(env, obj);
	JNINativeMethod methods[] = { (PCHAR)"notify", (PCHAR)"()V", (PVOID)MonitorNotify };
	env->RegisterNatives(env->FindClass("java/lang/Object"), methods, sizeof(methods) / sizeof(JNINativeMethod));
	message(env, "Hello World");

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)unload, NULL, 0, NULL);
}

PVOID unload(PVOID arg) {
	HMODULE hm = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPWSTR)&unload, &hm);
	FreeLibraryAndExitThread(hm, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, NULL, 0, NULL);
		break;
	}

	return TRUE;
}