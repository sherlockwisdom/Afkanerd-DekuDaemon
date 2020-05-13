#!/bin/bash
#!/bin/sh

#man-page says using the -K command would make the data more developer friends (key->value)
#SHOULD HAVE READ THIS FROM THE START... WOULD REFACTOR ALL THIS TO REFLECT THAT


#check for available modems
if [ "$1" == "list" ] ; then
	mmcli_list=$( mmcli -L | grep -oe "/[0-9]* " | cut -b 2- )
	ssh_list=$( nmcli device show 2>/dev/null | grep IP4.GATEWAY | grep -oe "192.168.[0-9*].[0-9*]" )
	printf "$mmcli_list"
	printf "\n"
	printf "$ssh_list"
elif [ "$1" == "extract" ] ; then
	modem_index=$2
	#if [ "$_command" == "e_id" ] ; then
	equipment_id=$( mmcli -m $modem_index 2>&1 | grep "equipment id" | grep -oe "[0-9]*" )
	#elif [ "$_command" == "sig_qual" ] ; then
	signal_quality=$( mmcli -m $modem_index 2>&1 | grep "signal quality" | grep -oe "[0-9]*" )
	#elif [ "$_command" == "op_name" ] ; then
	operator_name=$( mmcli -m $modem_index | grep "operator name" | grep -oe ": [a-zA-Z0-9#]*" | cut -b 3- )
	operator_id=$( mmcli -m $modem_index | grep "operator id" | grep -oe ": [0-9]*" | cut -b 3- )
	printf "equipment_id:$equipment_id\nsignal_quality:$signal_quality\noperator_name:$operator_name\noperator_id:$operator_id"

# Do not use regex here anywhwere cus service providers are not the same through out
elif [ "$1" == "ussd_initiate" ]; then
	#ussd_initiate 0 *155#
	modem_index=$2
	_command=$3
	ussd_respond=$( mmcli -K -m $modem_index --3gpp-ussd-initiate=$_command )
	printf "$ussd_respond"
	
elif [ "$1" == "ussd_respond" ]; then
	modem_index=$2
	_command=$3
	ussd_respond=$( mmcli -K -m $modem_index --3gpp-ussd-respond=$_command )
	printf "$ussd_respond"

elif [ "$1" == "ussd_status" ]; then
	#would need some parsing
	modem_index=$2
	ussd_respond=$( mmcli -K -m $modem_index --3gpp-ussd-status )
	printf "$ussd_respond"

elif [ "$1" == "ussd_cancel" ]; then
	modem_index=$2
	ussd_respond=$( mmcli -K -m $modem_index --3gpp-ussd-cancel )
	printf "$ussd_respond"

elif [ "$1" == "list_locked_request_files" ]; then
	locked_dir=$2
	files=$( ls -a $locked_dir | grep -oe "^\.\w*$" )
	printf "$files"

elif [ "$1" == "sms" ] ; then
	_type=$2
	if [ "$_type" == "send" ] ; then
		message=$3
		number=$4
		modem_index=$5
		
		output=$( mmcli -m $modem_index --messaging-create-sms="text=\"$message\",number=\"$number\",delivery-report-request='no'" --timeout=60 )
		sms_index=$( echo $output | grep -oe "[0-9]*$" )
		sending_output=$( mmcli -m $modem_index -s $sms_index --send )
		printf "$sending_output"
		#state=$( mmcli -m $modem_index -s $sms_index | grep state: | grep -oP ": [a-zA-Z]*" | cut -b 3- )
	elif [ "$_type" == "received" ] || [ "$_type" == "all" ] || [ "$_type" == "sent" ] || [ "$_type" == "unknown" ] ; then
		modem_index=$3
		output=
		if [ "$_type" == "all" ]; then
			output=$( mmcli -m $modem_index --messaging-list-sms | grep -oe "[0-9]* ([a-zA-Z]*)" | grep -oe "[0-9]*" )
		else
			output=$( mmcli -m $modem_index --messaging-list-sms | grep -oe "[0-9]* ($_type)" | grep -oe "[0-9]*" )
		fi
		printf "$output"
	elif [ "$_type" == "read_sms" ] ; then
		modem_index=$4
		message_index=$3

		message_number=$( mmcli -K -m $modem_index -s $message_index| grep number | grep -oe ": [a-Z0-9+]*" | cut -b 3- )
		message_text=$( mmcli -K -m $modem_index -s $message_index | grep text | grep -oP ": [a-zA-Z0-9\W :_<=?]*" | cut -b 3- )
		timestamp=$( mmcli -m $modem_index -s $message_index | grep timestamp: | grep -oP ": [a-zA-Z0-9\W]*" | cut -b 3- ) 
		printf "$message_number\n$message_text\n$timestamp"
		#print "$message_number\n$message_text"
	elif [ "$_type" == "delete" ]; then
		modem_index=$4
		message_index=$3

		output=$( mmcli -m $modem_index --messaging-delete-sms=$message_index )
		if [ ! -z "$output" ]; then
			printf "$output"
		fi

	fi
fi
