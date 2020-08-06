import os
import argparse

parser = argparse.ArgumentParser(description='to csv file')
parser.add_argument("-f", "--filename", help="File name")
args = parser.parse_args()

# After importing the database into your MySQL database,
from sqlalchemy import create_engine
import pymysql
import pandas as pd

db_connection_str = 'mysql+pymysql://your_username:your_password@localhost/__DEKU__' # "__DEKU__" is the imported database
db_connection = create_engine(db_connection_str)

df = pd.read_sql('SELECT * FROM MODEM_SMS_RECEIVED', con=db_connection)
print(df)

# converts datataframe to csv and stores in file inputed from the command line as argument
df.to_csv(os.path.join(r"path_to_storage_file", str(args.filename)), index = False, header=True)