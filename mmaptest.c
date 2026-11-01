#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// Định nghĩa các cờ (flag) nếu user/user.h chưa có
#define PROT_READ 1
#define PROT_WRITE 2
#define MAP_PRIVATE 2
#define MAP_ANONYMOUS 4

int main(int argc, char *argv[]) {
  printf("Starting mmap lazy allocation test...\n");

  // Mở một file có sẵn trong xv6 để ánh xạ
  /*int fd = open("README", O_RDWR);
  if(fd < 0){
    printf("Error: open file README failed\n");
    exit(1);
  }

  // Yêu cầu ánh xạ 1 trang (4096 bytes) với cờ MAP_PRIVATE
  void *addr = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  
  if((uint64)addr == 0xffffffffffffffff || addr == 0) {
    printf("Error: mmap system call failed\n");
    exit(1);
  }*/
  
  char *p = (char *)addr;
  printf("mmap allocated virtual address: %p\n", p);

  // KÍCH HOẠT PAGE FAULT TẠI ĐÂY
  // Vùng nhớ này chưa được cấp phát vật lý, lệnh gán này sẽ văng trap (scause 15 - store page fault)
  printf("Writing to memory to trigger scause 15...\n");
  p[0] = 'H'; 
  p[1] = 'a';
  p[2] = 'i';
  p[3] = '\0';

  // Lệnh đọc sẽ kiểm tra xem dữ liệu có thực sự được lưu vào trang vật lý mới hay không
  printf("Reading back memory: %s\n", p);

  if(p[0] == 'H' && p[1] == 'a' && p[2] == 'i') {
    printf("SUCCESS: Lazy allocation worked! Kernel handled page fault gracefully.\n");
  } else {
    printf("FAILED: Data mismatch.\n");
  }

  close(fd);
  exit(0);
}
