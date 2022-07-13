#ifndef CLIENT_HEAD_H
#define CLIENT_FUNC_H

#define _XOPEN_SOURCE
#include <errno.h>
#include <arpa/inet.h>
#include <crypt.h>
#include <dirent.h>
#include <grp.h>
#include <fcntl.h>
// #include <mysql/mysql.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pwd.h>
#include <pthread.h>
#include <shadow.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <time.h>
#include <unistd.h>
#define ARGS_CHECK(argc, num)                 \
    {                                         \
        if (argc != num)                      \
        {                                     \
            fprintf(stderr, "args error!\n"); \
            return -1;                        \
        }                                     \
    }
#define ERROR_CHECK(ret, num, msg) \
    {                              \
        if (ret == num)            \
        {                          \
            perror(msg);           \
            return -1;             \
        }                          \
    }
#define PTHREAD_ERROR_CHECK(ret, msg)                     \
    {                                                     \
        if (ret != 0)                                     \
        {                                                 \
            fprintf(stderr, "%s:%s", msg, strerror(ret)); \
        }                                                 \
    }

#endif