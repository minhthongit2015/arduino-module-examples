#!/usr/bin/python3

import socket
import ifcfg
import random


### Thiết đặt lắng nghe kết nối trên port 1234
my_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Cài đặt kiểu kết nối là TCP
my_server.bind(("0.0.0.0", 1234)) # Gắn kết kết nối trên tới địa chỉ 0.0.0.0 tại cổng 1234
my_server.listen(4) # Lắng nghe kết nối đến địa chỉ và cổng trên (tối đa 4 kết nối cùng lúc)

def getIPv4():
  try:
    interface1 = "Wireless LAN adapter Wi-Fi"
    interface2 = "wlan0"
    interface = ""
    if interface1 in ifcfg.interfaces():
      interface = interface1
    elif interface2 in ifcfg.interfaces():
      interface = interface2
    return ifcfg.interfaces()[interface]['inet']
  except:
    return (([ip for ip in socket.gethostbyname_ex(socket.gethostname())[2] if not ip.startswith("127.")] or [[(s.connect(("8.8.8.8", 53)), s.getsockname()[0], s.close()) for s in [socket.socket(socket.AF_INET, socket.SOCK_DGRAM)]][0][1]]) + ["no IP found"])[0]


try:
  print("> Server dang lang nghe ket noi tren {}:1234".format(getIPv4()))
  while True:
    client_sock, client_addr = my_server.accept()
    print("> Client sock: {}".format(client_sock))
    print("> Client addr: {}".format(client_addr))
    
    # Tao thread moi de? xu ly cho client

    try:
      temp, humi = 0,0
      while True:
        data = client_sock.recv(1024)
        print("> recv: {} from {}:{}".format(data, client_addr[0], client_addr[1]))
        client_sock.send(data+b"\n")
    except Exception as e:
      client_sock.close()
      print("> Client disconnected: {}".format(e))
except Exception as e:
  my_server.close()
  print("> Closed", e)
  pass
