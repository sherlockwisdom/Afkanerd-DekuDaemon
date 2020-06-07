#!/bin/bash

# We found using mmcli wont work for Huawei E5573s-XXX wifi modems . A possible solution would be to use usb_modeswitch
# to make the modem work like a normal usb key

# This looks interesting : https://www.mrt-prodz.com/blog/view/2015/05/huawei-modem-api-and-data-plan-monitor
# Explains the error codes nicely https://godoc.org/github.com/knq/hilink
# https://blog.idorobots.org/entries/hacking-huawei-e3372-hilink..html

#------------
#DATA=$(curl http://192.168.8.1/api/webserver/SesTokInfo)
#SESSION_ID=$(echo "$DATA" | grep "SessionID=" | cut -b 10-147)
#TOKEN=$(echo "$DATA" | grep "TokInfo" | cut -b 10-41)
#curl http://192.168.8.1/api/sms/send-sms -H "Cookie: $SESSION_ID" -H "__RequestVerificationToken: $TOKEN" --data "<?xml version='1.0' encoding='UTF-8'?><request><Index>-1</Index><Phones><Phone>$1</Phone></Phones><Sca></Sca><Content>$2</Content><Length>-1</Length><Reserved>1</Reserved><Date>-1</Date></request>"
#------------

# Script updated from example here:https://github.com/arska/e3372/issues/1
# https://stackoverflow.com/questions/48728233/sending-sms-messages-via-bash-script-using-a-huawei-e8372-hilink

MODEM_IP="192.168.8.1"
message="Hello world"
phone="670471866"

# Get the Session token information from http://$MODEM_IP/api/webserver/SesTokInfo"
curl -s -X GET "http://$MODEM_IP/api/webserver/SesTokInfo" >ses_tok.xml

# Echo the ses_tok.xml file
# cat ses_tok.xml

# Now pull out the important stuff

echo -e "\n\e[1;32m"Pull the important stuff"\e[0m\n"

COOKIE=$(grep "SessionID=" ses_tok.xml | cut -b 10-147)
TOKEN=$(grep "TokInfo" ses_tok.xml | cut -b 10-41)

# we found the final password sent for login was a combination of the username ,sha-256 hash of the password and the session token
# then all hashed to sha-256 and converted back to base64 before sending . wandass
# Credits : https://github.com/gallinapassus/huawei-e5186

username="admin"
password_sha_256=$(printf "admin" | shasum -a 256 | cut -d ' ' -f 1 | tr -d '\n')

password_base64=$(printf "admin" | base64)

final_password_hash=$(printf "$username$password_sha_256$TOKEN" | shasum -a 256 | cut -d ' ' -f 1 | tr -d '\n' | base64)

LOGIN_REQ="<request><Username>$username</Username><Password>$final_password_hash</Password><password_type>4</password_type></request>"

echo -e "COOKIE: \e[1;34m$COOKIE\e[0m\n"

echo -e "TOKEN : \e[1;34m$TOKEN\e[0m\n"

echo -e "LOGIN REQ: \e[1;34m$LOGIN_REQ\e[0m\n"

echo -e "password_sha_256: \e[1;34m$password_sha_256\e[0m\n"

echo -e "final_password_hash: \e[1;34m$final_password_hash\e[0m\n"

# Now lets actually login
curl -v "http://$MODEM_IP/api/user/login" \
    -H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" \
    -H "Cookie: $COOKIE" \
    -H "__RequestVerificationToken: $TOKEN" \
    -H "token: $TOKEN" \
    -H 'Connection: keep-alive' \
    --data "<?xml version=""1.0"" encoding=""UTF-8""?> \
    <request> \
     <Username>$username</Username>\
     <Password>$final_password_hash</Password>\
     <password_type>4</password_type> \
    </request>" --compressed

#   --dump-header login_resp_hdr.txt

# Pull the important parts out

#SESSION_ID=$(grep "SessionID=" login_resp_hdr.txt | cut -d ':' -f2 | cut -d ';' -f1)
#ADM_TOKEN=$(grep "__RequestVerificationTokenone" login_resp_hdr.txt | cut -d ':' -f2)

#echo -e "admin SESSION_ID is: \e[1;34m$SESSION_ID\e[0m\n"

#echo -e "admin TOKEN is: \e[1.;34m$ADM_TOKEN\e[0m\n"

#message_data="<request><Index>-1</Index><Phones><Phone>$phone</Phone></Phones><Sca></Sca><Content>hello</Content><Length>5</Length><Reserved>1</Reserved><Date>1</Date></request>"

# Send an SMS
# curl -X POST -d $message_data "http://$MODEM_IP/api/sms/send-sms" -H "Cookie: $SESSION_ID" -H "__RequestVerificationToken: $ADM_TOKEN" -H "Content-Type: text/xml" #--dump-header send_result.txt

#curl -v http://192.168.8.1/api/sms/send-sms \
#    -H "Cookie: $SESSION_ID" \
#    -H "__RequestVerificationToken: $ADM_TOKEN" \
#    -H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" \
#    -H "Accept: */*" \
#    -H "Referer: http://192.168.8.1/html/smsinbox.html" \
#    -H "Content-Type: text/xml" \
#    -H "Connection: keep-alive" \
#    -H "Origin: http://192.168.8.1" \
#    --data $message_data \
#    --dump-header send_result.txt
