#include <windows.h>
#include "jni.h"
#include "utils.h"
#include "pch.h"

void message(JNIEnv* jniEnv, LPCSTR message) {
	jclass jOptionPaneClss = jniEnv->FindClass("javax/swing/JOptionPane");
	jmethodID showMessageDialog = jniEnv->GetMethodID(jOptionPaneClss
		, "showMessageDialog"
		, "(Ljava/awt/Component;Ljava/lang/Object;)V");

	jniEnv->CallVoidMethod(jOptionPaneClss, showMessageDialog, jniEnv->NewStringUTF(message));
}