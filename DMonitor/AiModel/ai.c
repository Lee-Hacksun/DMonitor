#include <stdio.h>
#include <string.h>
#include <time.h>
#include "model/eml_common.h"
#include "model/eml_trees.h"

#include "ai.h"

// 충북
#include "model/chungbuk_blossom_model.h"
#include "model/chungbuk_maple_model.h"

// 충남
#include "model/chungnam_blossom_model.h"
#include "model/chungnam_maple_model.h"

// 강원
#include "model/gangwon_blossom_model.h"
#include "model/gangwon_maple_model.h"

// 광주
#include "model/gwangju_blossom_model.h"
#include "model/gwangju_maple_model.h"

// 경북
#include "model/gyeongbuk_blossom_model.h"
#include "model/gyeongbuk_maple_model.h"

// 경기
#include "model/gyeonggi_blossom_model.h"
#include "model/gyeonggi_maple_model.h"

// 경남
#include "model/gyeongnam_blossom_model.h"
#include "model/gyeongnam_maple_model.h"

// 전북
#include "model/jeonbuk_blossom_model.h"
#include "model/jeonbuk_maple_model.h"

// 전남
#include "model/jeonnam_blossom_model.h"
#include "model/jeonnam_maple_model.h"

// 제주
#include "model/jeju_blossom_model.h"
#include "model/jeju_maple_model.h"

// 서울
#include "model/seoul_blossom_model.h"
#include "model/seoul_maple_model.h"

// 부산
#include "model/busan_blossom_model.h"
#include "model/busan_maple_model.h"

// 대구
#include "model/daegu_blossom_model.h"
#include "model/daegu_maple_model.h"

// 대전
#include "model/daejeon_blossom_model.h"
#include "model/daejeon_maple_model.h"

// 인천
#include "model/incheon_blossom_model.h"
#include "model/incheon_maple_model.h"

// 울산
#include "model/ulsan_blossom_model.h"
#include "model/ulsan_maple_model.h"

char* PredictPeakDay(char* RegionCode, char* Species, int16_t temperature, int16_t humidity, int16_t light) {
    char regionCode[5];
    char species[32];

    strcpy(regionCode, RegionCode);
    strcpy(species, Species);

    int16_t features[3];
    features[0] = temperature; 
    features[1] = humidity;    
    features[2] = light;       

    int32_t features_length = sizeof(features) / sizeof(int16_t);
    float dayOfYear;

    switch (atoi(regionCode)) {
        case 11:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = seoul_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = seoul_maple_model_predict(features, features_length);
            }
            break;
        case 21:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = busan_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = busan_maple_model_predict(features, features_length);
            }
            break;
        case 22:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = daegu_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = daegu_maple_model_predict(features, features_length);
            }
            break;
        case 23:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = incheon_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = incheon_maple_model_predict(features, features_length);
            }
            break;
        case 24:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = gwangju_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = gwangju_maple_model_predict(features, features_length);
            }
            break;
        case 25:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = daejeon_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = daejeon_maple_model_predict(features, features_length);
            }
            break;
        case 26:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = ulsan_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = ulsan_maple_model_predict(features, features_length);
            }
            break;
        case 31:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = gyeonggi_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = gyeonggi_maple_model_predict(features, features_length);
            }
            break;
        case 32:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = gangwon_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = gangwon_maple_model_predict(features, features_length);
            }
            break;
        case 33:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = chungbuk_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = chungbuk_maple_model_predict(features, features_length);
            }
            break;
        case 34:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = chungnam_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = chungnam_maple_model_predict(features, features_length);
            }
            break;
        case 35:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = jeonbuk_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = jeonbuk_maple_model_predict(features, features_length);
            }
            break;
        case 36:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = jeonnam_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = jeonnam_maple_model_predict(features, features_length);
            }
            break;
        case 37:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = gyeongbuk_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = gyeongbuk_maple_model_predict(features, features_length);
            }
            break;
        case 38:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = gyeongnam_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = gyeongnam_maple_model_predict(features, features_length);
            }
            break;
        case 39:
            if (strcmp(species, "CherryBlossom") == 0) {
                dayOfYear = jeju_blossom_model_predict(features, features_length);
            } else if (strcmp(species, "Maple") == 0) {
                dayOfYear = jeju_maple_model_predict(features, features_length);
            }
            break;
    }

    // 현재 연도 가져오기
    time_t t = time(NULL);
    struct tm *currentTime = localtime(&t);
    int currentYear = currentTime->tm_year + 1900; // 현재 연도

    // 현재 연도와 dayOfYear를 기반으로 날짜 계산
    struct tm timeinfo = {0};
    timeinfo.tm_year = currentYear - 1900; // 현재 연도
    timeinfo.tm_mday = (int)dayOfYear; // dayOfYear를 정수로 변환하여 일로 설정
    mktime(&timeinfo); // 날짜 계산

    // 결과 출력
    char* date_string = malloc(sizeof(char) *  DATE_LENGHT); 
    snprintf(date_string, DATE_LENGHT, "Predict Date: %04d-%02d-%02d", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
    return date_string;
}
