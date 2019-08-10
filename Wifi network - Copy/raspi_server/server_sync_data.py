#!/usr/bin/python3

from RPi.GPIO import *

import socket
import ifcfg
import random
import time

import threading
import urllib.request


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

sync_data = {'humi': 0, 'temp': 0}
def sync_thread():
  # Gửi dữ liệu lên server mỗi 5s
  last_sync = time.time()
  while True:
    if time.time() - last_sync > 5:
      print("> Send sensor data to server")
      contents = urllib.request.urlopen("http://www.insys.vn/cloud?temp={}&humi={}".format(sync_data['temp'], sync_data['humi'])).read()
      # print("> ", contents)
      last_sync = time.time()
    time.sleep(1)

try:
  print("> Server dang lang nghe ket noi tren {}:1234".format(
    ifcfg.interfaces()['wlan0']['inet']))
  while True:
    client_sock, client_addr = my_server.accept()
    print("> Client sock: {}".format(client_sock))
    print("> Client addr: {}".format(client_addr))

    try:
      syncThread = threading.Thread(target = sync_thread)
      syncThread.start()
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

        # Điều khiển thiết bị theo dữ kiện điều kiện môi trường từ máy trạm
        if temp > 30 and FanState is False:
          FanState = True
          output(FanPin, 1)
          print("> Nhiet do cao: bat quat")
        elif temp <= 30 and FanState is True:
          FanState = False
          output(FanPin, 0)
          print("> Nhiet do tot: tat quat")

        # Lưu dữ liệu để gửi lên server thông qua thread khác
        sync_data['temp'] = temp
        sync_data['humi'] = humi

    except Exception as e:
      client_sock.close()
      print("> Client disconnected: {}".format(e))
except:
  my_server.close()
  print("> Closed")
  pass
