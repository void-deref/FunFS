#ifndef FUNFS_SIMU_UDP_H
#define FUNFS_SIMU_UDP_H

#if defined(_WIN32)
#ifndef _WIN32_WININT
#define _WIN32_WININT 0x0600
#endif /* _WIN32_WININT */
#ifndef MSG_CONFIRM
#define MSG_CONFIRM 0
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
// #   pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif /* _WIN32 */

#if defined(_WIN32)
#define IS_VALID_SOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSE_SOCKET(s) closesocket(s)
#define GET_SOCKET_ERRNO() (WSAGetLastError())
#else
#define IS_VALID_SOCKET(s) ((s) >= 0)
#define CLOSE_SOCKET(s) close(s)
#define SOCKET int
#define GET_SOCKET_ERRNO() (errno)
#endif /* _WIN32 */

// #include <stdio.h>
// #include <string.h>
#include <stdint.h>

uint16_t udp_server_init(void);
void udp_server_close(void);

void udp_receive_cdata(uint8_t *cdata);
void udp_send_cdata(uint8_t *rdata, uint16_t len);

#endif /* FUNFS_SIMU_UDP_H */