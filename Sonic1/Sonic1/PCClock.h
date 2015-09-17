//(C)opyright Christian Bendicks, Otto-von-Guericke University Magdeburg
#pragma once
#include <Windows.h>

//! Clock Class using Preformance Counter
//! The Performance Counter (available since Pentium) has a tact frequence
//! with 3,19 MHz,
//! so the accuracy is 0.313 micro-seconds.
class PCClock
{
public:

	//! Constructor
	PCClock(void);
	
	//!Deconstructor
	~PCClock(void);

	void start();//!< starts the measurement
	double stop();//!< stops the measurement, return time in s
	double sinceStart();//!<return time (in s) since start.

private:
	LONGLONG m_freq;//!< Frequency of Performance Counter
	LONGLONG m_curCount;//!< Current count
	LONGLONG m_lastCount;//!< Last count
};
