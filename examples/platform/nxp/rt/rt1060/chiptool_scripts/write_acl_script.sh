#!/bin/bash

# this script is used for each device we want to bind to the SV controller
# it has the role of telling that specific device that it has permissions to receive commands from the SV controller
 
# for each SV controller device we want to bind to the current node, add a new variable that will store the corresponding node id given as a parameter
# example: SV_controller_node_id_2=$3
SV_controller_node_id=$1
node_id=$2

# when working with multiple boards, we want to bind each node to the rest, in this case all nodes except the current one are seen as SV controllers
# there should be a {"fabricIndex": n, "privilege": 3, "authMode": 2, "subjects": ['"$SV_controller_node_id_nth"'], "targets": null } part in the command for each SV controller
./chip-tool accesscontrol write acl '[{"fabricIndex": 1, "privilege": 5, "authMode": 2, "subjects": [112233], "targets": null },{"fabricIndex": 2, "privilege": 3, "authMode": 2, "subjects": ['"$SV_controller_node_id"'], "targets": null }]' "$node_id" 0
