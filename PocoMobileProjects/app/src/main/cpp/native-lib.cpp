#include <jni.h>
#include <string>

#include "Logger/LoggerTest.h"
//#include "LoggerTest.h"

//#include "PocoTest/Logger/LoggerTest.h"
//#include "Net/NetTest.h"

void TestMain()
{
    LOGGER.WriteLog("Console", "Console");
}


extern "C" {
JNIEXPORT jstring JNICALL Java_com_seerlsab_pocoMobileSample_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Fill With Data";
    return env->NewStringUTF(hello.c_str());
}
}
