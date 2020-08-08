import os
import sys
import argparse

parser = argparse.ArgumentParser(description='to csv file')
parser.add_argument("-f", "--filename", help="File name")
parser.add_argument("--filter", dest="filter", help="To filter the sms data", action='store_true')
parser.add_argument("--phonenumber", dest="phonenumber", help="Phone number")
args = parser.parse_args()

# After importing the database into your MySQL database,
from sqlalchemy import create_engine
import pymysql
import pandas as pd

db_connection_str = 'mysql+pymysql://your_username:your_password@localhost/__DEKU__' # "__DEKU__" is the imported database
db_connection = create_engine(db_connection_str)

df = pd.read_sql('SELECT * FROM MODEM_SMS_RECEIVED', con=db_connection)
phoneNumber_df = df[df["PHONENUMBER"]==args.phonenumber]

filters = ["--phonenumber"]


# filters the dataframe based on the inputed command line argument
def use_filter(x):
    if args.filter == True:
        curr_filter = list(set(filters).intersection(x))
        if curr_filter[0] == "--phonenumber":
            print(phoneNumber_df)
            print(f"filtered by {args.phonenumber}")


# converts the dataframe to csv based on the -f command line argument. By default it saves an unfiltered csv file.
def to_csv():
    if "-f" in sys.argv:
        if "--phonenumber" in sys.argv:
            phoneNumber_df.to_csv(os.path.join(r"path_to_storage_file", str(args.filename)), index = False, header=True)
        else:
            df.to_csv(os.path.join(r"path_to_storage_file", str(args.filename)), index = False, header=True)


use_filter(sys.argv)
to_csv()