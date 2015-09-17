#include "PCClock.h"

PCClock::PCClock(void)
{
	//Frequenz holen 
	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&m_freq))
	{
	//	cout << "Performance Counter not supported." << endl;
	}
	m_curCount=0;
	m_lastCount=0;

}

PCClock::~PCClock(void)
{
}

void PCClock::start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_curCount); 
}

double PCClock::sinceStart()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_lastCount);
	double dTimeDiff = (((double)(m_lastCount-m_curCount))/((double)m_freq));
	return dTimeDiff;
}

double PCClock::stop()
{
	double dTimeDiff=sinceStart();
	m_curCount=0;
	m_lastCount=0;
	return dTimeDiff;
}