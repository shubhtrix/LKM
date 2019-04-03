
Note : Article 10 needs to be done seperately as it is 
        about KGDB. So need to compile kernel from source
        and need to take care of KGDB configuration options.

Driver for USB

MUST READ :: Please remove already inserted usb_storage driver.
                As a root user, can use command
                # modprobe -r uas usb_storage
