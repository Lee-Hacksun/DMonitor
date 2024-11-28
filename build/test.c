#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/epoll.h>

typedef struct {
    unsigned char x;
    unsigned char y;
    unsigned char z;
} Coordinate;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    Coordinate coord;

    struct epoll_event* ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 소켓 옵션 설정
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("소켓 옵션 설정 실패");
        exit(EXIT_FAILURE);
    }

    // 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // 바인드
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("바인드 실패");
        exit(EXIT_FAILURE);
    }

    // 리슨
    if (listen(server_fd, 3) < 0) {
        perror("리슨 실패");
        exit(EXIT_FAILURE);
    }

    printf("서버가 클라이언트를 기다립니다...\n");
    
    epfd = epoll_create(5);
    ep_events = malloc(sizeof(struct epoll_event) * 5);

    event.events = EPOLLIN;
    event.data.fd = server_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &event);

    while(1)
    {
        event_cnt = epoll_wait(epfd, ep_events, 5, 1000);
        if(event_cnt == -1)
        {
            puts("error");
            break;
        }
        else if(event_cnt == 1)
        {
            for(int i = 0; i < event_cnt; i++)
            {
                if(ep_events[i].data.fd == server_fd)
                {
                    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                        perror("연결 수락 실패");
                        exit(EXIT_FAILURE);
                    }
                    else 
                    {
                        read(new_socket, &coord, sizeof(coord));
                        printf("수신된 좌표: 위도 = %d, 경도 = %d, %d\n%ld", coord.x, coord.y, coord.z, sizeof(coord));
                    }
                }
            }
        }
        else if(event_cnt == 0)
        {
            puts("대기중\n");
        }
    }

    // 클라이언트 연결 수락
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("연결 수락 실패");
        exit(EXIT_FAILURE);
    }

    // 구조체 수신
    read(new_socket, &coord, sizeof(coord));
    printf("수신된 좌표: 위도 = %d, 경도 = %d, %d\n%ld", coord.x, coord.y, coord.z, sizeof(coord));

    // 소켓 종료
    close(new_socket);
    close(server_fd);
    return 0;
}
