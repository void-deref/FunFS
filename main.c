#include "simulator/simu_udp.h"
#include "iso7816_cmd.h"
#include "simulator/debug.h"

#include <signal.h>
#include <stdlib.h>

static volatile sig_atomic_t keep_running = 1;

static void
interrupt_handler(int num)
{
    (void)num;
    signal(SIGINT, SIG_IGN);
    keep_running = 0;
    udp_server_abort(); // Мгновенно разбудит recvfrom() на Windows и Linux

    // signal(SIGINT, SIG_IGN);
    // udp_server_close();
    // mm_save_image();
    // DBG_PRINT_VARG("\n%s\n", "Done.");
    // exit(0);
}

int
main(int argc, char *argv[])
{
    ISO_SW sw = SW_UNKNOWN;
    Apdu apdu;

    if (signal(SIGINT, interrupt_handler) == SIG_ERR)
    {
        DBG_PRINT_VARG("%s", "ERROR: can't catch SIGINT\n");
    }
    udp_server_init();

    if ((sw = iso_initialize()) != SW_OK)
    {
        DBG_PRINT_VARG("ERROR: 'iso_initialize()' function failed with SW %04X (%s)\n", sw, DBG_SW_TO_STRING(sw));
        raise(SIGINT);
    }

    while (keep_running)
    {
        // accept next incoming APDU
        int len = apdu_receive_cdata(&apdu);
        if (len <= 0 || !keep_running) {
            break;
        }

        // process incoming command
        sw = apdu_process(&apdu);

        // return data (if any) and SW
        apdu_send_rdata(apdu.buffer, apdu.resp_len, sw);
    }

    udp_server_close();
    mm_save_image();
    DBG_PRINT_VARG("\n%s\n", "Done.");
    return 0;
}