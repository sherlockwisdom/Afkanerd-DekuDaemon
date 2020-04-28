#!/bin/python3


# extracts files
# applies some custom parsing to it
# updat files


import os

HOME = os.environ['HOME'];
PATH = HOME + "/Deku/ISP/MTN";

# print(os.listdir( path ))

files = os.listdir( PATH );

for _file in files:
    print("> Working with file: ", _file);
    
    _file = open( PATH + "/" + _file, "r");
    _file = _file.read();

    print( _file );
    print();
