// Event_Handle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>

// using namespace sytem you must have to active common language cli from properties->c/c++
using namespace System;//															 ->general

public ref class ThresholdReachedEventArgs : public EventArgs
{	//as EventArgs Ref so extending class must be ref
	//as used ref so public varible willed preceded by "property"
	//for Ref class pointer will ^ not *
public:
	property int Threshold;
	property DateTime TimeReached;
};

public ref class Counter
{
private:
	int threshold;
	int total;

public:
	Counter() {};

	Counter(int passedThreshold)
	{
		threshold = passedThreshold;
	}

	void Add(int x)
	{
		total += x;
		if (total >= threshold) {
			ThresholdReachedEventArgs^ args = gcnew ThresholdReachedEventArgs();
			args->Threshold = threshold;
			args->TimeReached = DateTime::Now;
			
			//call for firing
			OnThresholdReached(args);
		}
	}

	//event
	event EventHandler<ThresholdReachedEventArgs^>^ ThresholdReached;

//event Firing
protected:
	virtual void OnThresholdReached(ThresholdReachedEventArgs^ e)
	{
		ThresholdReached(this, e);
	}
};

public ref class SampleHandler
{
public:
	static void c_ThresholdReached(Object^ sender, ThresholdReachedEventArgs^ e)
	{
		Console::WriteLine("The threshold of {0} was reached at {1}.",
			e->Threshold, e->TimeReached);
		//Environment::Exit(0);
	}
};

void main()
{
	Counter^ c = gcnew Counter((gcnew Random())->Next(10));
	c->ThresholdReached += gcnew EventHandler<ThresholdReachedEventArgs^>(SampleHandler::c_ThresholdReached);

	Console::WriteLine("press 'a' key to increase total");
	while (Console::ReadKey(true).KeyChar == 'a') {
		Console::WriteLine("adding one");
		c->Add(1);
	}
}

