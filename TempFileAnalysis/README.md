# MYC-

Problem Faced

1.FILE_FLAG_DELETE_ON_CLOSE is a file attribute that does not allow to create another file handle of that file. So for working with that file 
we need to add FILE_SHARE_DELETE that gives the delete access to the operator.

2.For Flushing to disk i used FlushFileBuffers(HANDLE)

3.File mode is very sensitive because it does not give you an exception but return some data with garbage  that is not expected in c/c++.
Example: for reading binary file "rb" is okey. But if i use "rb+"(logically not true) that is not valid but does not give amy exception but
give some garbage data with some expected data. So be carefull


tempfileAnalysis function: generate a temporary file and read data from a given file and write that data to that temporary file.
fileRead function: read data from that temporary file
Stop:close file HANDLE

Outcome:
if any one want to create temporary file that will be deleted after the program exit even after program crashes then it will be usefull
