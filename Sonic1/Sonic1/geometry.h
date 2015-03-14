#pragma once
#include "imReadBasics.h"

class geometry: public imReadBasics
{
public:
	geometry(std::string);
	geometry(imReadBasics);
	~geometry(void);
};

