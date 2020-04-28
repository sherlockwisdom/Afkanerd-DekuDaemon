#!/bin/python3


# extracts files
# applies some custom parsing to it
# updat files


import os

HOME = os.environ['HOME'];
PATH = HOME + "/Deku/ISP/MTN";

# print(os.listdir( path ))

def custom_parsing( split_message ):
    del split_message[0]

    return split_message

files = os.listdir( PATH );

for _file in files:
    print("> Working with file:", _file);
    
    _file = open( PATH + "/" + _file, "r");
    _file = _file.read();

    # print( _file );
    # print();

    # split_file = _file.split(',', 1)
    split_file = _file.split('"')
    message = split_file[1]
    split_message = message.split("\\n")
    
    split_message = custom_parsing( split_message )
    print(split_message)
    print()

    '''
    for split in split_file:
        equal_split = split.split('=');
        if( equal_split[0] == "message" ):
            # print(equal_split[1])
            message = equal_split[1].replace('"', '')

            split_message = message.split("\\n")

            split_message = custom_parsing( split_message )
            print(split_message)
            print()
    '''
