#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "cjson/cJSON.h"
#include "Actuator.h"

void error_handling(char *message);
void process_code(unsigned char code);
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

    
    while (1) {
        unsigned char code;
        str_len = read(sock, &code, sizeof(code));
        if (str_len == -1)
            error_handling("read() error");
        printf("Received code: %u\n", code);

        // 수신한 코드 처리
        process_code(code);
    }

    // 소켓 종료
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void process_code(unsigned char code) {
    unsigned char buzzer = (code & 0x08) >> 3; // A 비트
    unsigned char led_red = (code & 0x04) >> 2; // B 비트
    unsigned char led_green = (code & 0x02) >> 1; // C 비트
    unsigned char led_blue = (code & 0x01); // D 비트

    printf("buzzer: %d, led_red: %d, led_green: %d, led_blue: %d\n", buzzer, led_red, led_green, led_blue);
    
    RunActuator(buzzer, led_red, led_green, led_blue);
}