#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct {
    unsigned char x;
    unsigned char y;
    unsigned char z;
} Coordinate;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    Coordinate coord = {253, 254, 255}; // 서울의 위도와 경도
    Coordinate coord2 = {1, 2, 3}; // 서울의 위도와 경도

    Coordinate coords[2];
    coords[0] = coord;
    coords[1] = coord2;

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n 소켓 생성 실패 \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // 서버 주소 변환
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\n 잘못된 주소/주소가 지원되지 않음 \n");
        return -1;
    }

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n 연결 실패 \n");
        return -1;
    }

    // 구조체 전송
    send(sock, &coord, sizeof(coord), 0);
    printf("좌표가 서버에 전송되었습니다.\n");

    // 소켓 종료
    close(sock);
    return 0;
}
