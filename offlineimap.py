#!/usb/bin/python
from subprocess import check_output

def get_password(name):
    try:
        return check_output(["gkcli", "get", name])
    except:
        return None
