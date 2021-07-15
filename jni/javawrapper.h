#pragma once

#include <jni.h>

#include <string>

#define EXCEPTION_CHECK(env) \
	if ((env)->ExceptionCheck()) \ 
	{ \
		(env)->ExceptionDescribe(); \
		(env)->ExceptionClear(); \
		return; \
	}

class CJavaWrapper
{
	jobject activity;
	jmethodID s_HideLayoutCut, s_HideNavBar;
public:
	JNIEnv* GetEnv();

	CJavaWrapper(JNIEnv* env, jobject activity);
	~CJavaWrapper();
	
	void hideNavBar();
	void hideLayoutCut();
};

extern CJavaWrapper* g_pJavaWrapper;