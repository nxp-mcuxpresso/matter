# Chip-tool commands scripts

The current folder contains bash scripts used for the commissioning and binding processes. They are based on chip-tool commands given in the command line of a Matter controller, such as a Raspberry Pi or I.MX MPU EVK.

**1. commissioning_script.sh**

Role: This script is used to commission each new device that wants to enter the Matter network. The commissioning takes place over ble and wifi. 
Usage: `./commissioning_script.sh` `wifi_ssid` `wifi_passw` `node_id`
Parameters:
- `wifi_ssid` and `wifi_passw`: are represented by the wifi credentials for the specific network
- `node_id`: unique integer for the current Matter device

**2. toggle_script.sh**

Role: This script is used to toggle a LED for a specific Matter device. 
Usage: `./toggle_script.sh` `node_id`
Parameters: 
- `node_id`: unique integer for the current Matter device

**3. set_device_name_script.sh**

Role: This script is used for each device we want to bind to the SV board represented as the new "controller". It has the role of naming a specific device so that it will be recognized by the SV controller when the user says different voice commands.
Usage: `./set_device_name_script.sh` `device_name` `node_id`
Parameters:
- `device_name`: it can be chosen from the options: "bedroom", "kitchen", "living", "bathroom" (for LED functionalities) and "central" (for shades functionalities)
- `node_id`: unique integer for the current Matter device

**4. write_acl_script.sh**

Role: This script is used for each device we want to bind to the SV board represented as the new "controller". It has the role of telling the specific device that it has permissions to receive commands from the SV controller.
Usage: `./write_acl_script.sh` `SV_controller_id` `node_id`
Parameters: 
- `SV_controller_id`: unique integer for the SV board considered as "controller"
- `node_id`: unique integer for the current Matter device

**5. write_binding_script.sh**

Role: This script is used for the SV controller. It has the role of creating the binding connections between the SV controller and all nodes specified in the json structure. Depending on how many nodes we want to bind, it will be needed to add new parameters, variables to store those parameters, as well as new json entries in the command's body for each node.
Usage: `./write_binding_script.sh` `SV_controller_id` `binded_node_id_1`
Parameters:
- `SV_controller_id`: unique integer for the SV board considered as "controller"
- `binded_node_id_1`: unique integer for the first device we want to bind to the SV controller
