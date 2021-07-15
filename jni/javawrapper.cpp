#include "javawrapper.h"
#include "main.h"

extern JavaVM* javaVM;
extern utils::log client_log;

JNIEnv* CJavaWrapper::GetEnv()
{
	JNIEnv* env = nullptr;
	int getEnvStat = javaVM->GetEnv((void**)& env, JNI_VERSION_1_4);

	if (getEnvStat == JNI_EDETACHED)
	{
		client_log.push(LOG_CAT_TAG, "GetEnv: not attached");
		if (javaVM->AttachCurrentThread(&env, NULL) != 0)
		{
			client_log.push(LOG_CAT_TAG, "Failed to attach");
			return nullptr;
		}
	}
	if (getEnvStat == JNI_EVERSION)
	{
		client_log.push(LOG_CAT_TAG, "GetEnv: version not supported");
		return nullptr;
	}

	if (getEnvStat == JNI_ERR)
	{
		client_log.push(LOG_CAT_TAG, "GetEnv: JNI_ERR");
		return nullptr;
	}

	return env;
}

CJavaWrapper::CJavaWrapper(JNIEnv* env, jobject activity)
{
	this->activity = env->NewGlobalRef(activity);

	jclass nvEventClass = env->GetObjectClass(activity);
	if (!nvEventClass)
	{
		client_log.push(LOG_CAT_TAG, "nvEventClass null");
		return;
	}

	s_HideLayoutCut = env->GetMethodID(nvEventClass, "hideLayoutCut", "()V");
	s_HideNavBar = env->GetMethodID(nvEventClass, "hideNavBar", "()V");

	env->DeleteLocalRef(nvEventClass);
}

CJavaWrapper::~CJavaWrapper()
{
	JNIEnv* pEnv = GetEnv();
	if (pEnv) {
		pEnv->DeleteGlobalRef(this->activity);
	}
}

void CJavaWrapper::hideNavBar()
{
    JNIEnv* env = GetEnv();

	if (!env) {
        client_log.push(LOG_CAT_TAG, "env null");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_HideNavBar);

	EXCEPTION_CHECK(env);
}

void CJavaWrapper::hideLayoutCut()
{
    JNIEnv* env = GetEnv();

	if (!env) {
        client_log.push(LOG_CAT_TAG, "env null");
		return;
	}

	env->CallVoidMethod(this->activity, this->s_HideLayoutCut);

	EXCEPTION_CHECK(env);
}

CJavaWrapper* g_pJavaWrapper = nullptr;