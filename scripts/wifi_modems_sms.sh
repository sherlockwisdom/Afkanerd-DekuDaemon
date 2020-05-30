#!/bin/bash
#!/bin/sh

# We found using mmcli wont work for Huawei E5573s-XXX wifi modems . A possible solution would be to use usb_modeswitch
# to make the modem work like a normal usb key

# This looks interesting : https://www.mrt-prodz.com/blog/view/2015/05/huawei-modem-api-and-data-plan-monitor
# Explains the error codes nicely https://godoc.org/github.com/knq/hilink
# https://blog.idorobots.org/entries/hacking-huawei-e3372-hilink..html

#!/bin/bash

DATA=$(curl http://192.168.8.1/api/webserver/SesTokInfo)
SESSION_ID=$(echo "$DATA" | grep "SessionID=" | cut -b 10-147)
TOKEN=$(echo "$DATA" | grep "TokInfo" | cut -b 10-41)

curl http://192.168.8.1/api/sms/send-sms -H "Cookie: $SESSION_ID" -H "__RequestVerificationToken: $TOKEN" --data "<?xml version='1.0' encoding='UTF-8'?><request><Index>-1</Index><Phones><Phone>$1</Phone></Phones><Sca></Sca><Content>$2</Content><Length>-1</Length><Reserved>1</Reserved><Date>-1</Date></request>"
