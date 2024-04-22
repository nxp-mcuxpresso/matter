#!/bin/bash

# this script can be used to toggle the LED of a specific Matter device based on its node_id
# the "toggle" command can be changed to either "on" or "off"

node_id=$1

./chip-tool onoff toggle "$node_id" 1
