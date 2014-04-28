#!/usr/bin/expect -f
# ./sshlogin.exp uptime
# set Variables
set user flavius.tirnacop
set password Warcraft16#
set ipaddr fep.grid.pub.ro
set arg1 [lrange $argv 0 0]
set timeout -1
# now connect to remote UNIX box (ipaddr) with given script to execute
spawn scp -r . $user@$ipaddr:Tema3
# match_max 100000 # Look for passwod prompt
expect "*?assword:*"
# Send password aka $password
send -- "$password\r"
# send blank line (\r) to make sure we get back to gui
send -- "\r"
send -- "ls -la"
expect eof
