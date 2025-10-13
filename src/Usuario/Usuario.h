#include "HTTPClient.h"
#include <WiFiClientSecure.h>
#define Usuario_h
#ifdef Usuario_h

String get_user_id(HTTPClient& client, WiFiClientSecure& wifi_client, const char* user_email);

#endif
