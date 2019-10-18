// ConsoleApplication3.cpp : Defines the entry point for the console application.
//
#include<iostream>
#include<queue>
using namespace std;
struct Event
{
	string Type;
	double Time;
	bool IsPrivallage;
};
struct People
{
	double Arrival;
	double Depature;
	bool IsPrivallage;

};
double GenerateExpnential(double mean)
{
	double U = (double)rand() / RAND_MAX;

	double lamda = 1 / mean;
	double val = log(1 - U) / (-1 * lamda);
	if (val > 100000)
	
	{
		U = (double)rand() / RAND_MAX;
		val = log(1 - U) / (-1 * lamda);
	}

	
	return val;

}
bool CheckPrivallage()
{
	double U = (double)rand() / RAND_MAX;
	bool state;
	return state = (U > 0.5) ? true : false;

}


// Moves given minimum element to rear of  
// queue 
void insertMinToRear(queue<Event> &q, int min_index)
{
	Event min_val;
	int n = q.size();
	for (int i = 0; i < n; i++)
	{
		Event curr = q.front();
		q.pop();
		if (i != min_index)
			q.push(curr);
		else
			min_val = curr;
	}
	q.push(min_val);
}


void TakeInput(int &N, double &a, double &s)
{
	while (1)
	{
		cout << "Enter replications of the operation of the system (N): \n ";
		cin >> N;
		cout << "Enter a mean of an exponential distribution for Arrivals of people at a telescope  in Minutes (a): ";
		cin >> a;
		cout << "Enter a mean of an exponential distribution for a Service Time for a telescope  in Minutes (s): ";
		cin >> s;
		if (N > 0 && a > 0 && s > 0)
			break;
		cout << "Validation Error , Retry /n";
	}
}



class myComparator
{
public:
	int operator() (const Event& p1, const Event& p2)
	{
		return p1.Time > p2.Time;
	}
};
void CreateArrival(double Time, priority_queue <Event, vector<Event>, myComparator > Events)
{
	Event InitalState;
	InitalState.Type = "Arrival";
	double finshTime = Time + GenerateExpnential(1);
	InitalState.Time = finshTime;
	Events.push(InitalState);
	//In[InCounter++] = Time;
}

int main()
{


	while (1)
	{
		
		int N;
		double a, s;
		TakeInput(N, a, s);
		int Iterator = N;
		double AvgWaitingTimeAllDay = 0;
		double AvgWaitingTimePirvallageDay = 0;
		double AvgWaitingTimeNotDay = 0;

		double sumAllDay = 0;
		double sumPrDay = 0;
		double sumNotDay = 0;

		std::cout << "Simulating.. , it may take few seconds please wait. \n";
		while (Iterator)
		{
			//DATA for Simulation 
			bool ServerIsBusy = false; // represnting Telescope Service
			double Time = 0; //Represnting Actual Time Stamp
			queue<People> NormalQueue;//Representing a Normal Queue of people to see a telescope with normal ticket
			queue<People> PrivilegeQueue;//Representing a Queue of people who has  Privilege ticket
			queue<People> Stats;//Representing a statistics for people who Enter for simulation analysis
			priority_queue <Event, vector<Event>, myComparator > Events;//Representing a min heap to Order The execution of events
			Event InitalState; // representing an intial visitor state

			InitalState.Type = "Arrival";
			InitalState.Time = Time;
			Events.push(InitalState);
			while (1)// Simulating until the day is off
			{
				if (Time < 360 || !Events.empty())// checking for both day finsh and There is no one in queue waiting for an event
				{
					Event mEvent;
					mEvent = Events.top();
					Events.pop();
					Time = mEvent.Time; // simulate at Event time
					if (mEvent.Type == "Arrival")
					{
						People person;
						person.Arrival = Time; //someone arrive
						if (ServerIsBusy)
						{
							//Taking a Decision of Privilege Ticket
							if (!NormalQueue.empty())
							{
								bool IsPrivallage = CheckPrivallage();
								if (IsPrivallage)
								{
									person.IsPrivallage = true;
									PrivilegeQueue.push(person);
								}
								else
								{
									NormalQueue.push(person);
								}
							}
							else
							{
								NormalQueue.push(person);
							}
						}
						else
						{
							ServerIsBusy = true;
							Event mEvent;
							mEvent.Time = Time + GenerateExpnential(s);
							mEvent.Type = "Departure";
							Events.push(mEvent);
							person.Depature = mEvent.Time;
							Stats.push(person);
						}
						if (Time < 360) {
							// while the Day on , generate events to wait/Enter queue
							Event GenerateEvent;
							GenerateEvent.Type = "Arrival";
							GenerateEvent.Time = Time + GenerateExpnential(a);
							Events.push(GenerateEvent);
						}
					}
					else if (mEvent.Type == "Departure")
					{
						// Serve from PrivilegeQueue unitl it become empty
						if (!PrivilegeQueue.empty())
						{
							//Someone Entering a Telescope Service
							People Person = PrivilegeQueue.front();
							PrivilegeQueue.pop();
							Event mEvent;
							mEvent.Time = Time + GenerateExpnential(s);
							mEvent.Type = "Departure";
							Events.push(mEvent);

							//for Simulation analysis
							Person.Depature = mEvent.Time;
							Person.IsPrivallage = true;
							Stats.push(Person);

						}
						else
						{
							//Take from a normal queue
							if (!NormalQueue.empty())
							{
								People Person = NormalQueue.front();
								NormalQueue.pop();
								Event mEvent;
								mEvent.Time = Time + GenerateExpnential(s);
								mEvent.Type = "Departure";
								Events.push(mEvent);
								//For Simulation Analysis
								Person.IsPrivallage = false;
								Person.Depature = mEvent.Time;
								Stats.push(Person);

							}
							else
							{
								// There is noone in any queue => Telescope Service is Avaliable
								ServerIsBusy = false;
							}

						}
					}
				}
				else
				{
					break;
				}
			}

			//Simulation analysis for a single Day 
			double AvgWaitingTimeAll = 0;
			double AvgWaitingTimePirvallage = 0;
			double AvgWaitingTimeNot = 0;

			double sumAll = 0;
			int cAll = 0;
			double sumPr = 0;
			int cPr = 0;
			double sumNot = 0;
			int cNot = 0;
			while (!Stats.empty())
			{
				People Person = Stats.front();
				sumAll += (Person.Depature - Person.Arrival);
				Stats.pop();
				cAll++;

				if (Person.IsPrivallage == true)
				{
					cPr++;
					sumPr += (Person.Depature - Person.Arrival);
				}
				else {
					cNot++;
					sumNot += (Person.Depature - Person.Arrival);
				}
			}

			AvgWaitingTimeAll = sumAll / cAll;
			AvgWaitingTimePirvallage = sumPr / cPr;
			AvgWaitingTimeNot = sumNot / cNot;

			sumAllDay += AvgWaitingTimeAll;
			sumPrDay += AvgWaitingTimePirvallage;
			sumNotDay += AvgWaitingTimeNot;
			Iterator--;

		}
		system("CLS");

		//Simulation Analysis for N Days 
		AvgWaitingTimeAllDay = sumAllDay / N;
		AvgWaitingTimePirvallageDay = sumPrDay / N;
		AvgWaitingTimeNotDay = sumNotDay / N;
		cout << "Simulation is Done\n";
		cout << "--------------------------------------------------------\n";
		cout << "n=" << N << ",s=" << s << "mins,a=" << a << "mins\n";
		cout << "Av.waiting for all = " << AvgWaitingTimeAllDay <<"mins\n";
		cout << "Av.waiting for privilege =" << AvgWaitingTimePirvallageDay <<"mins\n";
		cout << "Av. waiting for without-privilege= " << AvgWaitingTimeNotDay <<"mins\n";



		cout << "Do you want Simulate Again with Different Paramters? y/n \n";
		char Choice;
		cin >> Choice;
		if (Choice == 'n')
			break;
	}

	return 0;
}

