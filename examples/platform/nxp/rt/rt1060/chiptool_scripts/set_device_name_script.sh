#!/bin/bash

# this script is used for each device we want to bind to the SV controller
# it has the role of "naming" a specific device, so that it will be recognized by the SV controller when the user says different voice commands

device_name=$1
node_id=$2

./chip-tool basicinformation write node-label ""$device_name"" "$node_id" 0
