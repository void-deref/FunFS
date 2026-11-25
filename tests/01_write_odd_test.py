from udp_client import *
from utils import lv_hex
def main():

    udp_connect() #("127.0.0.1", 8485)
    print("Connecting to IP 127.0.0.1 at port 8485")

    udp_apdu_send("00e0 0000" + lv_hex("62" + lv_hex("820138 83023F00 8A0101 8D024003") + "8C" + lv_hex("6FFFFFFFFFFFFF")), "create MF")
    udp_apdu_send("00e0 0000" + lv_hex("62" + lv_hex("8002000C 82020100 83023F01 8A0101") + "8C" + lv_hex("6BFFFFFF1111")), "create EF 3F01")
    udp_apdu_send("00e0 0000" + lv_hex("62" + lv_hex("8002000C 82020100 83023F02 8A0101") + "8C" + lv_hex("6BFFFFFF1111")), "create EF 3F02")

    udp_apdu_send("00a4 0800" + lv_hex("3F00 3F01"),      "select 3F01")
    udp_apdu_send("00d0 0000" + lv_hex("010203"),         "write 010203")
    udp_apdu_send("00d0 0004" + lv_hex("0405060708090A"), "write 0405060708090A")

    udp_apdu_send("00b0 0000 0C", "read 3F01")
    udp_apdu_send("80fe 0000",    "terminate card usage")
    
    udp_disconnect()

main()