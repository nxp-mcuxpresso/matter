#!/bin/bash

# this script is used to configure the on-board Wi-Fi and Bluetooth for the i.MX 8M Mini Matter controller
# replace the user_wifi_ssid and user_wifi_password fields with your own Wi-Fi network credentials

export SSID="user_wifi_ssid"
export PASSWORD="user_wifi_password"
wpa_passphrase ${SSID} ${PASSWORD} > wifiap.conf
ifconfig eth0 down 
modprobe moal mod_para=nxp/wifi_mod_para.conf 
wpa_supplicant -d -B -i mlan0 -c ./wifiap.conf 
sleep 5 
modprobe btnxpuart 
hciconfig hci0 up 
echo 1 > /proc/sys/net/ipv6/conf/all/forwarding 
echo 1 > /proc/sys/net/ipv4/ip_forward 
echo 2 > /proc/sys/net/ipv6/conf/all/accept_ra 
ln -sf /usr/sbin/xtables-nft-multi /usr/sbin/ip6tables 
ipset create -exist otbr-ingress-deny-src hash:net family inet6 
ipset create -exist otbr-ingress-deny-src-swap hash:net family inet6 
ipset create -exist otbr-ingress-allow-dst hash:net family inet6 
ipset create -exist otbr-ingress-allow-dst-swap hash:net family inet6 
sleep 1 
otbr-agent -I wpan0 -B mlan0  'spinel+hdlc+uart:///dev/ttyUSB0?uart-baudrate=1000000' -v -d 5 & 
sleep 2 
iptables -A FORWARD -i mlan0 -o wpan0 -j ACCEPT 
iptables -A FORWARD -i wpan0 -o mlan0 -j ACCEPT 
otbr-web & 