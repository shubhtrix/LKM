
Code is for character device to write to and from the kernel 
and user space. It is just an extension of /dev/null to read
and write data.

Bugs : 

1. While reading the device it will continuously read the device
    which should be exactly one character.
2. After rmmod the maor number not geeting removed from /proc/devices
    entry.
    
