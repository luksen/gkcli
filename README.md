# use with offlineimap

    [general]
    pythonfile = offlineimap.py
    
    remotepasseval = get_password("somekey")

# use with msmtp

    passwordeval	"gkcli get somekey; echo"
