#!/usr/bin/python3

from RPi.GPIO import *

import socket
import ifcfg
import random


### Thiết đặt lắng nghe kết nối trên port 1234
my_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Cài đặt kiểu kết nối là TCP
my_server.bind(("0.0.0.0", 1234)) # Gắn kết kết nối trên tới địa chỉ 0.0.0.0 tại cổng 1234
my_server.listen(4) # Lắng nghe kết nối đến địa chỉ và cổng trên (tối đa 4 kết nối cùng lúc)

### Cài đặt các pin GPIO để điều khiển thiết bị
FanPin = 4
FanState = False
setmode(BCM)
setwarnings(False)
setup(FanPin, 0)

try:
  print("> Server dang lang nghe ket noi tren {}:1234".format(
    ifcfg.interfaces()['wlan0']['inet']))
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

        data = data.decode('utf-8')
        try:
          temp, humi = data.split(',')
        except:
          pass
        temp = float(temp)
        humi = float(humi)

        if temp > 30 and FanState is False:
          FanState = True
          output(FanPin, 1)
          print("> Nhiet do cao: bat quat")
        elif temp <= 30 and FanState is True:
          FanState = False
          output(FanPin, 0)
          print("> Nhiet do tot: tat quat")

    except Exception as e:
      client_sock.close()
      print("> Client disconnected: {}".format(e))
except:
  my_server.close()
  print("> Closed")
  pass
