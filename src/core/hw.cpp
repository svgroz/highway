#include "org_svgroz_HelloWorld.h"
#include <spdlog/spdlog.h>

JNIEXPORT void JNICALL Java_org_svgroz_HelloWorld_hello(JNIEnv *, jobject) {
  SPDLOG_INFO("Hello world");
}
