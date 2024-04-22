#!/bin/bash

# this script is used for the SV controller 
# it has the role of creating the binding connection between the SV controller and all nodes specified in the json structure
 
# for each device we want to bind, add a new variable that will store the corresponding node id given as a parameter
# example: binded_node_id_2=$3
SV_controller_node_id=$1
binded_node_id_1=$2
 
# for each node we want to bind, there should be a { "node" : < nth_device_id > , "cluster" : "0x0006" , "endpoint" : “1” } part in the command
# example for 2 binded devices: '[{"node" : "'"$binded_node_id_1"'" , "cluster" : "0x0006" , "endpoint" : “1” }, { "node" : "'"$binded_node_id_2"'" , "cluster" : "0x0006" , "endpoint" : “1” }]' 
./chip-tool binding write binding '[{"node": "'"$binded_node_id_1"'", "cluster": "0x0006", "endpoint": "1"}]' "$SV_controller_node_id" 1
