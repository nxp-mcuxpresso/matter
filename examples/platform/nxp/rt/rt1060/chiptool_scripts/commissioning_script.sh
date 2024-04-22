#!/bin/bash

# this script is used for each device that wants to enter the Matter network
# it has the role of commissioning a Matter end node based on ble and wifi and creating a connection between it and a Matter controller
# each device is given a unique node_id based on which it will be controlled in the network

wifi_ssid=$1
wifi_passw=$2
node_id=$3

./chip-tool pairing ble-wifi "$node_id" "$wifi_ssid" "$wifi_passw" 20202021 3840
