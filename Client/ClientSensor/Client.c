#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "cjson/cJSON.h"
#include "Sensors.h"

typedef struct _clientDate
{
    char dlientId[20];
    char regionCode[5];
    char species[20];
    int red;
    int blue;
    int green;
} clientDate;

void error_handling(char *message);

int main(int argc, char **argv)
{
    int sock;
    struct sockaddr_in serv_addr;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

#pragma region 클라이언트 정보 등록
    cJSON *json = cJSON_CreateObject();
    if (json == NULL)
    {
        return -1;
    }
    char clientId[20] = "SENSOR01";
    char regionCode[5];
    char species[20];
    int red = 0;
    int green =0;
    int blue = 0;
    int num = -1;

    printf("Enter regionCode: ");
    scanf("%4s", regionCode);

    while (num != 1 && num != 2)
    {
        printf("choose species\n");
        printf("1. CherryBlossom\n");
        printf("2. Maple\n");
        scanf("%d", &num);

        if (num == 1)
            strcpy(species, "CherryBlossom");
        else if (num == 2)
            strcpy(species, "Maple");
        else
            printf("Invalid species\n");
    }

    printf("Client ID: %s\n", clientId);
    printf("Region Code: %s\n", regionCode);
    printf("Species: %s\n", species);
    cJSON_AddStringToObject(json, "clientID", clientId);
    cJSON_AddStringToObject(json, "RegionCode", regionCode);
    cJSON_AddStringToObject(json, "Species", species);

    cJSON* color = cJSON_CreateObject();
    while (1)
    {
        printf("Do you want to set the color? (y/n): ");
        char answer;
        scanf(" %c", &answer);

        if (answer == 'y')
        {
            printf("Enter red: ");
            scanf(" %d", &red);
            printf("Enter green: ");
            scanf(" %d", &green);
            printf("Enter blue: ");
            scanf(" %d", &blue);

            
            cJSON_AddNumberToObject(color, "Red", red);
            cJSON_AddNumberToObject(color, "Green", green);
            cJSON_AddNumberToObject(color, "Blue", blue);

            break;
        } else if (answer == 'n'){
            cJSON_AddNumberToObject(color, "Red", 0);
            cJSON_AddNumberToObject(color, "Green", 0);
            cJSON_AddNumberToObject(color, "Blue", 0);

            break;
        } else {
            printf("Invalid answer\n");
        }
    }

	cJSON_AddItemToObject(json, "Color", color);

	char* jsonString = cJSON_Print(json);
	if (jsonString == NULL)
    {
        cJSON_Delete(json);
        return -1;
    }

	send(sock, jsonString, strlen(jsonString), 0);
	cJSON_Delete(json);
	free(jsonString);
	sleep(1);

#pragma region 클라이언트 정보 등록
    while (1)
    {
        cJSON* root = cJSON_CreateObject();
        cJSON* sensorData = cJSON_CreateObject();
        cJSON_AddItemToObject(root, "SensorData", sensorData);
        if (root == NULL)
        {
            return -1;
        }

        SensorData sensorDatas = runSensors();

        cJSON_AddNumberToObject(sensorData, "Flame", sensorDatas.flame);
        cJSON_AddNumberToObject(sensorData, "Gas", sensorDatas.gas);
        cJSON_AddNumberToObject(sensorData, "Humidity", sensorDatas.dht11.humidity);
        cJSON_AddNumberToObject(sensorData, "Light", sensorDatas.light);
        cJSON_AddNumberToObject(sensorData, "Temperature", sensorDatas.dht11.temperature);
    

        cJSON *color = cJSON_CreateObject();
        cJSON_AddItemToObject(sensorData, "Color", color);
        cJSON_AddNumberToObject(color, "Red", sensorDatas.color.red);
        cJSON_AddNumberToObject(color, "Green", sensorDatas.color.green);
        cJSON_AddNumberToObject(color, "Blue", sensorDatas.color.blue);

        char *jsonString = cJSON_Print(root);
        printf("jsonString: %s\n", jsonString);
        int sendResult = send(sock, jsonString, strlen(jsonString), 0);
        if (sendResult == -1)
        {
            error_handling("send() error");
            break; // 서버와의 연결이 끊겼을 경우 루프를 종료
        }

        cJSON_Delete(root);
        free(jsonString);
        sleep(10); // 10초마다 반복
    }
#pragma endregion

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}