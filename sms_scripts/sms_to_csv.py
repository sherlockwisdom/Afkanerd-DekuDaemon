# After importing the database into your MySQL database,

from sqlalchemy import create_engine
import pymysql
import pandas as pd

db_connection_str = 'mysql+pymysql://your_username:your_password@localhost/__DEKU__' # "__DEKU__" is the imported database
db_connection = create_engine(db_connection_str)

df = pd.read_sql('SELECT * FROM MODEM_SMS_RECEIVED', con=db_connection)
print(df)

#converts datataframe to csv and stores in file called 'modem_sms_received.csv'
df.to_csv(r"path_to_storage_file\modem_sms_received.csv", index = False, header=True)