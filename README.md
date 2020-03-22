# Covid19-MAR
The current code helps to fit and estimate the cumulative number of Covid-19 in Morocco using data published by the government.
Data fitted using function with 4 parameters:

                              a*exp(b*x+c) + d*x
The 5-sigma and 2-sigma intervals are plotted, and estimation for the next 4 days with its uncertainties  
# Data 
There are two files of data :
  - coronaMar.csv : contain the same data published by the government day-by-day
  - coronaMar2.csv: contain the data published by the government with the assumption that a day starts from 12:00 to 12:00 of the next day.
# How to run 
The code run inside the CERN Root Data Analysis Framework https://root.cern.ch/ using the following command :
 - root -l plot.cxx
 
# Enjoy and Stay @ Home 
