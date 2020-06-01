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
LOGIN_REQ="<request><Username>admin</Username><Password>admin</Password><password_type>4</password_type></request>"

echo -e "COOKIE: \e[1;34m$COOKIE\e[0m\n"

echo -e "TOKEN : \e[1;34m$TOKEN\e[0m\n"

echo -e "LOGIN REQ: \e[1;34m$LOGIN_REQ\e[0m\n"

# Now lets actually login
curl -d $LOGIN_REQ "http://$MODEM_IP/api/user/login" \
    -H "Cookie: $COOKIE" \
    -H "__RequestVerificationToken: $TOKEN" \
    -H "Content-Type: text/xml" -H 'Connection: keep-alive'
#   --dump-header login_resp_hdr.txt

# Pull the important parts out

SESSION_ID=$(grep "SessionID=" login_resp_hdr.txt | cut -d ':' -f2 | cut -d ';' -f1)
ADM_TOKEN=$(grep "__RequestVerificationTokenone" login_resp_hdr.txt | cut -d ':' -f2)

echo -e "admin SESSION_ID is: \e[1;34m$SESSION_ID\e[0m\n"

echo -e "admin TOKEN is: \e[1.;34m$ADM_TOKEN\e[0m\n"

message_data="<request><Index>-1</Index><Phones><Phone>$phone</Phone></Phones><Sca></Sca><Content>hello</Content><Length>5</Length><Reserved>1</Reserved><Date>1</Date></request>"

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
