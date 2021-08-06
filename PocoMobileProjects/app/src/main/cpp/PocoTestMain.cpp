#include <jni.h>

#include <string.h>

#include "Logger/LoggerTest.h"
//#include "Net/NetTest.h"

void TestMain()
{
    LOGGER.WriteLog("Console", "Console");
}

extern "C"
{
JNIEXPORT jstring JNICALL Java_com_seerlsab_pocoMobileSample_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject jObj) {
    std::string hello = "Fill With Data";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT jstring JNICALL Java_com_seerlsab_pocoMobileSample_MainActivity_ReceiveMessage(
        JNIEnv *env,
        jobject jObj) {

    std::string hello;
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void JNICALL Java_com_seerlsab_pocoMobileSample_MainActivity_SendMessage(
        JNIEnv *env, jobject jObj, jstring message) {
    const char* chMessage = env->GetStringUTFChars(message, 0);

}

JNIEXPORT void JNICALL Java_com_seerlsab_pocoMobileSample_MainActivity_LogMessage(
        JNIEnv *env, jobject jObj, jstring message) {
    const char* chMessage = env->GetStringUTFChars(message, 0);

    LOGGER.WriteLog("Formatting", chMessage, __FILE__, __LINE__);
}
}
