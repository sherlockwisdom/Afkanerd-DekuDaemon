### Script to Extract Data From a MySQL Database and Dump Into a .csv File using Python

* Firstly, import the .sql database file into your MySQL database.
* In your Python file, import the necessary libraries:
    * `from sqlalchemy import create_engine`
    * `import pymysql`
    * `import pandas as pd`
* Create your database connection string using your MySQL username, password, sql server(localhost) and database name.
* Establish a database connection using the connection string and `create_engine()`. This will load the database into the script.
* Convert the connection into a pandas dataframe making sure to choose the desired table. The table used here is *MODEM_SMS_RECEIVED*.
* Print out the dataframe to view the data(optional).
* Store the dataframe into a .csv file using `pd.to_csv()` making sure to specify the path to the file and destination file as in .csv format as first argument. The *header* argument is set to *True* so as to include the headers in the csv file.

And that's pretty much it!
