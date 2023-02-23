#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("outputLab4.txt", O_WRONLY | O_CREAT, 0644);
    ssize_t n = 0;
    if (fd == -1) {
        perror("open");
        return 1;
    }

    const char buffer[] = "This is a test for opening, writing, and closing a file!";
    n = write(fd, buffer, (sizeof(buffer)-2));
    if (n == -1) {
        perror("write");
        return 1;
    }

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
