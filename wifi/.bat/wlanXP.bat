@ECHO OFF
netsh interface set interface "Wireless Network Connection" admin=enabled
sc config wzcsvc start= auto
net start wzcsvc