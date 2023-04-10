# 에코 서버 객체화 하기

☝🏻 멤버 변수 default 는 private

<br>

***

## 1. 서버 소켓 관리자
☝🏻 소켓 생성(socket) + 소켓과 IP 연결(address struct, bind, listen)

### 1-1. 멤버 변수
``` C++
#include <arpa/inet.h>
#include <sys/socket.h>

int                        server_socket
struct sockaddr_in         server_addr
```

### 1-2. 멤버 함수
``` C++
int     socket(int, int, int);
int     listen(int, int);
int     bind(int, const struct sockaddr *, socklen_t);
int     accept(int, struct sockaddr *, socklen_t *);
```

### 1-3. 생성자
``` C++
ServerSocket(std::string    protocol,
             int            port,
//             std::string     ip_address
            );
// 생성자에서 listen, bind 까지 하고 에러 핸들링
```

<br>

*** 

## 2. 서버 관리자

### 2-1. 멤버 변수
```C++
std::map<int, std::string>  clients
int 					    kq
// std::vector<struct kevent>  change_list
struct kevent 				event_list[8]
```

### 2-2. 멤버 함수
```C++
int		kqueue(void);

// 내부에서 EV_SET 및 change_list 에 kevent 를 push_back 함
void	setChangeList(std::vector<struct kevent> &change_list, 
                        uintptr_t                   ident,
                        int16_t                     filter,
                        uint16_t                    flags,
                        uint32_t                    fflags,
                        intptr_t                    data,
                        void                        *udata);

//   내부에서 kevent 실행
int     newEvents(int            kq,
                   struct const kevent   *changelist,
                   int            nchanges,
                   struct kevent         *eventlist,
                   int            nevents,
                   const timespec *timeout);

```
<br>

***

## 3. 이벤트 관리자 (kqueue)
* 서버를 상속 받은 이벤트 객체...?

<br>

***

server-socket 생성 및 ip 연결하는 클래스와 적용을 한 후, 나머지 부분 클래스화를 실행했다.
나머지 부분 구현에 대한 2가지 논의가 있었다.

첫번째.
1. kqueue 함수로 kq 파일디스크립터 생성
2. kq 에 서버 소켓 연결하여 클라이언트에서 보낸 이벤트 감지해서 kq 집합에 추가

두번째.
1. kqueue 함수로 kq 파일디스크립터 생성
2. kq 에 서버 소켓을 연결
3. 클라이언트에서 보낸 이벤트 감지해서 kq 집합에 추가