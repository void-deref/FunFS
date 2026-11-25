#include "simu_udp.h"
#include "flash_emu.h"
#include "debug.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>

static struct sockaddr_in server_addr = {0};
static struct sockaddr_in client_addr = {0};

static SOCKET udp_socket = INVALID_SOCKET;
static int udp_port = 8485;

static socklen_t socket_len = sizeof(client_addr);
static volatile sig_atomic_t is_aborted = 0;

uint16_t
udp_server_init(void)
{
    is_aborted = 0;
#if defined(_WIN32)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", GET_SOCKET_ERRNO());
        exit(1);
    }
#endif /* _WIN32 */

    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (!IS_VALID_SOCKET(udp_socket))
    {
        printf("Failed to retrieve a UDP socket descriptor. Error: %d\n", GET_SOCKET_ERRNO());
        exit(1);
    }

    memset((uint8_t *)&server_addr, 0x00, sizeof(struct sockaddr_in));
    memset((uint8_t *)&client_addr, 0x00, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(udp_port);

    if (bind(udp_socket, (const struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0)
    {
        printf("Failed to assign the server address to the upd socket.\n");
        udp_server_close();
        exit(1);
    }

    return (uint16_t)udp_port;
}

void
udp_server_abort(void)
{
    is_aborted = 1;
    if (IS_VALID_SOCKET(udp_socket))
    {
        CLOSE_SOCKET(udp_socket);
        udp_socket = INVALID_SOCKET;
    }
}

void
udp_server_close(void)
{
    udp_server_abort();
#if defined(_WIN32)
    WSACleanup();
#endif
}

int16_t udp_receive_cdata(uint8_t *cdata)
{
    socket_len = sizeof(client_addr);

    int16_t bytes_read = recvfrom(
        udp_socket,
        (char *)cdata,
        APDU_BUFFER_LEN,
        0, // MSG_WAITALL,
        (struct sockaddr *)&client_addr,
        &socket_len);

    if (bytes_read < 0) {
        if (is_aborted) {
            return -1;
        }
        DBG_PRINT_VARG("\nUDP failed to retrieve a data from the client with the following reason: %d", GET_SOCKET_ERRNO())
        raise(SIGINT);
        return -1;
    }

    DBG_PRINT_VARG("%s", "<< ")
    DBG_PRINT_HEX(cdata, bytes_read)
    return bytes_read;
}

void udp_send_cdata(uint8_t *rdata, uint16_t len)
{
    if (is_aborted || !IS_VALID_SOCKET(udp_socket)) {
        return;
    }

    sendto(
        udp_socket,
        (const char *)rdata,
        len,
        MSG_CONFIRM,
        (struct sockaddr *)&client_addr,
        socket_len);

    DBG_PRINT_VARG("%s", ">> ")
    DBG_PRINT_HEX(rdata, len)
}
