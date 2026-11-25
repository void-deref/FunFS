import socket
import sys
from utils import bytes_to_hex, hex_to_bytes

# 1. UDP client machinery
# 2. hex-string to bytes converter
# 3. bytes to hex-string converter
# 4. a structure that resembles APDU protocol

udp_params = ("127.0.0.1", 8485)
udp_socket = None

def udp_connect():
	global udp_socket
	udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	udp_socket.connect(udp_params)
	
def udp_disconnect():
	udp_socket.close()

def udp_apdu_send(cdata:str, cmd_name:str = "", expected_sw:int = 0x9000):
	
	print(f"{cmd_name}\n>> " + cdata)

	udp_socket.send(bytearray(hex_to_bytes(cdata)))
	rdata     = udp_socket.recv(263)       # get array with responce
	
	ret_val   = rdata[:-2]                 # a list of bytes (except SW) to be returned to the user
	actual_sw = int.from_bytes(rdata[-2:]) # get SW bytes
	rdata     = rdata[:-2]                 # truncate SW bytes from incoming array
	rdata     = bytes_to_hex(rdata)  # convert byte array into string

	print(f"<< {rdata}  {actual_sw:X}")
	
	if actual_sw != expected_sw:
		print(f"ERROR: expected {expected_sw:X}, but got {actual_sw:X}")
		
		udp_disconnect()
		sys.exit(1)
	
	return ret_val