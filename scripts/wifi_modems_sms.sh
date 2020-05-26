#!/bin/bash
#!/bin/sh

# We found using mmcli wont work for Huawei E5573s-XXX wifi modems . A possible solution would be to use usb_modeswitch
# to make the modem work like a normal usb key

#get session token
curl -b session.txt -c session.txt http://192.168.8.1/html/index.html > /dev/null 2>&1

#get token
TOKEN=$(curl -s -b session.txt -c session.txt http://192.168.8.1/html/smsinbox.html)
TOKEN=$(echo $TOKEN | cut -d'"' -f 10)

echo $TOKEN > token.txt

#send sms

NUMBER=670471866
MESSAGE=hello

LENGTH=${#MESSAGE}
TIME=$(date +"%Y-%m-%d %T")
TOKEN=$(<token.txt)

SMS="<request><Index>-1</Index><Phones><Phone>$NUMBER</Phone></Phones><Sca/><Content>$MESSAGE</Content><Length>$LENGTH</Length><Reserved>1</Reserved><Date>$TIME</Date></request>"

echo $SMS

curl -v -b session.txt -c session.txt -H "X-Requested-With: XMLHttpRequest" --data "$SMS" http://192.168.8.1/api/sms/send-sms --header "__RequestVerificationToken: $TOKEN" --header "Content-Type:text/xml"
