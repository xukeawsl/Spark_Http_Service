#include <cstdio>
#include <sys/stat.h>

#include "ngx_http_service_interface.h"
#include "cJSON.h"
#include "sparkchain.h"

#define FILE_NAME "../config.json"

/* request format
{
    "appID": "",
    "apiKey": "",
    "apiSecret": "",
    "content": ""
}
*/

/* response format
{
    "code": xxx,
    "message": "",
    "content": ""
}
*/

struct request_data {
    char *appID = NULL;
    char *apiKey = NULL;
    char *apiSecret = NULL;
    char *content = NULL;
};


static void set_Response(ngx_json_response_t *resp, int code, const char *message, const char *content)
{
    cJSON *output = cJSON_CreateObject();
    if (output == NULL) {
        return;
    }

    cJSON_AddNumberToObject(output, "code", code);
    cJSON_AddStringToObject(output, "message", message);
    cJSON_AddStringToObject(output, "content", content);

    resp->data = cJSON_Print(output);

    cJSON_Delete(output);
}


static bool parse_Request(const ngx_json_request_t *rqst, struct request_data *data)
{
    cJSON *input = cJSON_Parse(rqst->data);
    if (input == NULL) {
        return false;
    }

    cJSON *appID = cJSON_GetObjectItem(input, "appID");
    if (cJSON_IsString(appID)) {
        data->appID = cJSON_GetStringValue(appID);
    }

    cJSON *apiKey = cJSON_GetObjectItem(input, "apiKey");
    if (cJSON_IsString(apiKey)) {
        data->apiKey = cJSON_GetStringValue(apiKey);
    }

    cJSON *apiSecret = cJSON_GetObjectItem(input, "apiSecret");
    if (cJSON_IsString(apiSecret)) {
        data->apiSecret = cJSON_GetStringValue(apiSecret);
    }

    cJSON *content = cJSON_GetObjectItem(input, "content");
    if (cJSON_IsString(content)) {
        data->content = cJSON_GetStringValue(content);
    }

    if (data->content == nullptr) {
        return false;
    }

    if (data->appID == nullptr && data->apiKey == nullptr && data->apiSecret == nullptr) {
        FILE *fp = fopen(FILE_NAME, "r");
        if (fp == nullptr) {
            return false;
        }

        struct stat statbuf;
        stat(FILE_NAME, &statbuf);
        size_t fileSize = statbuf.st_size;

        if (fileSize == 0) {
            return false;
        }

        char *file_buf = (char *)calloc(fileSize + 1, sizeof(char));
        if (file_buf == nullptr) {
            return false;
        }

        size_t readSize = fread(file_buf, sizeof(char), fileSize, fp);
        if (readSize == 0) {
            fclose(fp);
            free(file_buf);
            return false;
        }

        fclose(fp);

        cJSON *config_file = cJSON_Parse(file_buf);
        if (config_file == nullptr) {
            free(file_buf);
            
            return false;
        }

        appID = cJSON_GetObjectItem(config_file, "appID");
        if (cJSON_IsString(appID)) {
            data->appID = cJSON_GetStringValue(appID);
        }

        apiKey = cJSON_GetObjectItem(config_file, "apiKey");
        if (cJSON_IsString(apiKey)) {
            data->apiKey = cJSON_GetStringValue(apiKey);
        }

        apiSecret = cJSON_GetObjectItem(config_file, "apiSecret");
        if (cJSON_IsString(apiSecret)) {
            data->apiSecret = cJSON_GetStringValue(apiSecret);
        }

        free(file_buf);
    }

    
    return data->appID && data->apiKey && data->apiSecret;
}


extern "C"
void srv_spark(const ngx_json_request_t *rqst, ngx_json_response_t *resp)
{
    request_data data;

    if (!parse_Request(rqst, &data)) {
        set_Response(resp, SPARKCHAIN_ERR_PARAM_REQUIRED_MISSED, "必填参数缺失", "");
        return;
    }

    SparkChain::SparkChainConfig *config = SparkChain::SparkChainConfig::builder();
    config->appID(data.appID)
          ->apiKey(data.apiKey)
          ->apiSecret(data.apiSecret);
    
    int errCode = SparkChain::init(config);
    if (errCode) {
        set_Response(resp, errCode, "API 初始化失败", "");
        SparkChain::unInit();
        return;
    }

    SparkChain::LLM *llm = SparkChain::LLM::create();
    if (llm == nullptr) {
        set_Response(resp, SPARKCHAIN_ERR_GENERAL_FAILED, "LLM 构建失败", "");
        SparkChain::unInit();
        return;
    }

    SparkChain::LLMSyncOutput *result = llm->run(data.content);
    if (result == nullptr) {
        set_Response(resp, SPARKCHAIN_ERR_GENERAL_FAILED, "LLMSyncOutput 获取失败", "");
        SparkChain::LLM::destroy(llm);
        SparkChain::unInit();
        return;
    }

    set_Response(resp, result->getErrCode(), result->getErrMsg(), result->getContent());

    SparkChain::LLM::destroy(llm);
    SparkChain::unInit();
}