#!/usr/bin/python3
# import numpy as np
import select
import socket
# import sys
import Queue
# import math


class MPU6050:

    def __init__(self, x, y, z, ax, ay, az):
        self.x = x
        self.y = y
        self.z = z
        self.ax = ax
        self.ay = ay
        self.az = az

    def set(self, x, y, z, ax, ay, az):
        self.x = x
        self.y = y
        self.z = z
        self.ax = ax
        self.ay = ay
        self.az = az

    def get(self):
        return [self.x, self.y, self.z, self.ax, self.ay, self.az]


def main():

    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setblocking(0)

    host = socket.gethostname()
    port = 9502

    print("starting tcp server on {}:{}".format(host, port))
    srv.bind((host, port))

    srv.listen(2)

    inputs = [srv]
    outputs = []
    messageQueues = {}
    while inputs:
        readable, writable, exceptional = select.select(inputs, outputs, inputs)
    for s in readable:
        if s is srv:
            connection, client_address = s.accept()
            connection.setblocking(0)
            inputs.append(connection)
            messageQueues[connection] = Queue.Queue()
        else:
            data = s.recv(1024)
            if data:
                messageQueues[s].put(data)
                if s not in outputs:
                    outputs.append(s)
            else:
                if s in outputs:
                    outputs.remove(s)
                inputs.remove(s)
                s.close()
                del messageQueues[s]

    for s in writable:
        try:
            next_msg = messageQueues[s].get_nowait()
        except Queue.Empty:
            outputs.remove(s)
        else:
            s.send(next_msg)

    for s in exceptional:
        inputs.remove(s)
        if s in outputs:
            outputs.remove(s)
        s.close()
        del messageQueues[s]


main()
