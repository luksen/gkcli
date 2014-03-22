# set a password

    echo somepass | gkcli set somekey

or, if you don't want somepass in your history just

   gkcli set somekey

and enter the password

# use with offlineimap

    [general]
    pythonfile = offlineimap.py
    
    remotepasseval = get_password("somekey")

# use with msmtp

    passwordeval	"gkcli get somekey; echo"
