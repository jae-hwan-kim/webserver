#include "../class/Response.hpp"
#include "../class/Transaction.hpp"
#include "../include/include.hpp"

//----- send/recv --------------------------------------------------------------
int ft::safeSend(int fd, Response &response) {
  // std::cout << GRY << "Debug: safeSend\n" << DFT;
  int send_len;

  signal(SIGPIPE, SIG_IGN);
  if ((send_len = send(fd, response.getResponseMsg(),
                       response.getResponseMsgSize(), 0)) == -1) {
    std::cerr << RED << "Error: Transaction: send() error\n" << DFT;
  }
  signal(SIGPIPE, SIG_DFL);
  return send_len;
}

int ft::safeRecv(int fd, char *buf, int size) {
  // std::cout << GRY << "Debug: safeRecv\n" << DFT;
  int recv_len;

  signal(SIGPIPE, SIG_IGN);
  if ((recv_len = recv(fd, buf, size, 0)) == -1) {
    std::cerr << RED << "Error: Transaction: recv() error\n" << DFT;
  }
  signal(SIGPIPE, SIG_DFL);
  return recv_len;
}

//----- read/write -------------------------------------------------------------
ssize_t ft::safeRead(int fd, char *buf, int size) {
  // std::cout << GRY << "Debug: safeRead\n" << DFT;
  signal(SIGPIPE, SIG_IGN);
  ssize_t read_len = read(fd, buf, size);
  if (read_len == -1) {
    throw Transaction::ErrorPage500Exception();
  }
  signal(SIGPIPE, SIG_DFL);
  return read_len;
}

ssize_t ft::safeWrite(int fd, char *buf, int size) {
  // std::cout << GRY << "Debug: safeWrite\n" << DFT;
  signal(SIGPIPE, SIG_IGN);
  ssize_t write_len = write(fd, buf, size);
  if (write_len == -1) {
    throw Transaction::ErrorPage500Exception();
  }
  signal(SIGPIPE, SIG_DFL);
  return write_len;
}

//----- fread/fwrite -----------------------------------------------------------
size_t ft::safeFread(char *buf, int size, int count, FILE *file_ptr) {
  // std::cout << GRY << "Debug: safeFread\n" << DFT;

  signal(SIGPIPE, SIG_IGN);
  size_t read_len = std::fread(buf, size, count, file_ptr);
  if (std::ferror(file_ptr)) {
    std::cerr << RED << "Error: Transaction: file fread() error\n" << DFT;
  }
  signal(SIGPIPE, SIG_DFL);
  return read_len;
}

size_t ft::safeFwrite(const char *buf, int size, int count, FILE *file_ptr) {
  // std::cout << GRY << "Debug: safeFwrite\n" << DFT;

  signal(SIGPIPE, SIG_IGN);
  size_t write_len = std::fwrite(buf, size, count, file_ptr);
  if (std::ferror(file_ptr)) {
    std::cerr << RED << "Error: Transaction: file fwrite() error\n" << DFT;
  }
  signal(SIGPIPE, SIG_DFL);
  return write_len;
}

//----- open/close -------------------------------------------------------------
int ft::safeClose(int fd) {
  // std::cout << GRY << "Debug: safeClose\n" << DFT;
  if (close(fd) != 0) {
    std::cerr << RED << "Error: Transaction: file close() error\n" << DFT;
  }
  return 0;
}

//----- fopen/fclose -----------------------------------------------------------
std::FILE *ft::safeFopen(const char *filename, const char *mode) {
  // std::cout << GRY << "Debug: safeFopen\n" << DFT;
  std::FILE *fp;

  // fopen 으로 읽으면 fp 의 stream 버퍼를 1024 로 설정한다.
  if ((fp = std::fopen(filename, mode)) == NULL) {
    std::cerr << RED << "Error: Transaction: file fopen() error\n" << DFT;
  }
  // 1024 로 설정된 fp 의 stream 을 F_STREAM_SIZE 크기로 설정한다.
  std::setvbuf(fp, 0, _IONBF, F_STREAM_SIZE);
  return fp;
}

int ft::safeFclose(FILE *fp) {
  // std::cout << GRY << "Debug: safeFclose\n" << DFT;
  if (std::fclose(fp) != 0) {
    std::cerr << RED << "Error: Transaction: file fclose() error\n" << DFT;
  }
  return 0;
}

//----- fork/pipe --------------------------------------------------------------
pid_t ft::safeFork() {
  // std::cout << GRY << "Debug: safeFork\n" << DFT;
  pid_t pid;

  pid = fork();
  if (pid == -1) {
    throw Transaction::ErrorPage500Exception();
  }
  return pid;
}

void ft::safePipe(int *fd) {
  // std::cout << GRY << "Debug: safePipe\n" << DFT;
  if (pipe(fd) == -1) {
    throw Transaction::ErrorPage500Exception();
  }
}