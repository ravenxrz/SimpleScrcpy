#!/usr/bin/python3

import socket

if __name__ == '__main__':
    s = socket.socket();
    s.connect(("127.0.0.1",27183))
    print("connect success")

