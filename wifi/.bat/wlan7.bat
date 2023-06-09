@ECHO OFF
netsh interface set interface "Wireless Network Connection" admin=enabled
net start Wlansvc
