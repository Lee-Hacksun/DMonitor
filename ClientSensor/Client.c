#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "cjson/cJSON.h"
#include "Sensors.h"

void error_handling(char *message);

int main(int argc, char **argv)
{
    int sock;
    int str_len, i;
    struct sockaddr_in serv_addr;
    /*
        typedef struct{
            char clientId[20];
            DHT11_Data dht11;
            ColorData color;
            int flame;
            int gas;
            int light;
        } SensorData;

        typedef struct {
            float temperature;
            float humidity;
        } DHT11_Data;

        typedef struct {
            int red;
            int green;
            int blue;
        } ColorData;
    */
    char buf[100];
    char msg1[] = "Hello Everybody";
    char msg2[] = "This is test program";
    char message[10];

    if (argc != 3) {
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

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    #pragma region 클라이언트 정보 등록
    while (1) {
        cJSON* json = cJSON_CreateObject();
        if (json == NULL)
        {
            return -1;
        }

        SensorData sensorData = runSensors();

        cJSON_AddStringToObject(json, "clientID", sensorData.clientId);
        cJSON_AddStringToObject(json, "Species", "CherryBlossom");

        cJSON_AddNumberToObject(json, "Temperature", sensorData.dht11.temperature);
        cJSON_AddNumberToObject(json, "Humidity", sensorData.dht11.humidity);

        cJSON* color = cJSON_CreateObject();
        cJSON_AddNumberToObject(color, "Red", sensorData.color.red);
        cJSON_AddNumberToObject(color, "Green", sensorData.color.green);
        cJSON_AddNumberToObject(color, "Blue", sensorData.color.blue);

        cJSON_AddItemToObject(json, "Color", color);

        cJSON_AddNumberToObject(json, "Light", sensorData.light);
        
        cJSON_AddNumberToObject(json, "Flame", sensorData.flame);
        cJSON_AddNumberToObject(json, "Gas", sensorData.gas);

        char* jsonString = cJSON_Print(json);
        printf("jsonString: %s\n", jsonString);
        send(sock, jsonString, strlen(jsonString), 0);

        // free(json);
        cJSON_Delete(json);
        sleep(5); // 5초마다 반복
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