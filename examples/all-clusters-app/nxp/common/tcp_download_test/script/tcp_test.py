"""
    Copyright 2022 NXP
    All rights reserved.

    SPDX-License-Identifier: BSD-3-Clause
"""

import argparse
import socket
import time


def main():
    """ Parse the provided parameters """
    parser = argparse.ArgumentParser()
    parser.add_argument('-ip', '--ip', required=True, type=str, help="Specify IP address")
    parser.add_argument('-p', '--port', required=True, type=str, help="Specify TCP port")
    parser.add_argument('-f', '--file', required=True, type=str, help="Specify file to send")
    parser.add_argument('-cs', '--chunk-size', required=True, type=str, help="Size of chunks the file will be read")
    args = parser.parse_args()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((args.ip, int(args.port)))

    with open(args.file, "rb") as f:
        file_hash = 0
        file_size = 0

        start_time = time.time()

        data = f.read(int(args.chunk_size))
        while data:
            for c in data:
                file_hash = (file_hash + c) % 256
                file_size += 1

            s.send(data)
            data = f.read(int(args.chunk_size))

            if (file_size % (1024 * 1024)) == 0:
                print("---> %d [MB]" % (file_size / (1024 * 1024)))

        end_time = time.time()
        dur_time = end_time - start_time + 0.001

        print("Transfer Size     = %.2f [KB]" % (file_size / 1024))
        print("Transfer Duration = %.2f [s]" % (dur_time))
        print("Transfer Speed    = %.2f [KB/s]" % ((file_size / 1024) / dur_time))

        print("Local file HASH   = " + str(file_hash))

    s.close()


if __name__ == '__main__':
    main()
