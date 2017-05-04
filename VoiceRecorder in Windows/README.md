# MYC-

Problem Faced

1.Have no idea over windows programming.

2.Wav File Format Understanding

3.File mode is very sensitive because it does not give you an exception but return some data with garbage  that is not expected in c/c++.
Example: for reading binary file "rb" is okey. But if i use "rb+"(logically not true) that is not valid but does not give amy exception but
give some garbage data with some expected data. So be carefull

4. There can be problem in time based data saving means wait for a specific time for taking data and save to a file cause the unexpected repeatation 
of voice. That is why i used the alternating solution event based callback function. when the associated buffer will be full then a event occur 
and callback function callled with specifuc msg WIM_DONE the i save the data.

5. undefined c++ function??check std::

6.first time "myAudioFile->open("D:\\recor.wav", fstream::in | fstream::out| fstream::binary)" is not preferable it may not create the file.
you can write a "while loop test" for ensuring creation of file. else you can try first in only write mode and then truc mode 
	myAudioFile->open("D:\\recor.wav", std::fstream::out);
	myAudioFile->open("D:\\recor.wav", std::fstream::trunc);
	
	then write "myAudioFile->open("D:\\recor.wav", fstream::in | fstream::out| fstream::binary)"


startrecord function: start take voice data;
fileinit function: initialize wav file header;
CALLBACK VoiceRecordNotification function:it is called by OS when corresponding event occur take data and call savetofile funtion
savetofile funtion:saving the voice data in wav file

WAVEIN assotiated buffer is "short int waveIn[20000]" when this being full the event occur with passing msg "WIM_DONE"

Outcome:
Record voice data in wav file.
