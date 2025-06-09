#ifndef DNA_H
#define DNA_H

#include <vector>
#include <list>

class DNA
{
	public:
		DNA() = default;
		
	private:
		std::vector<std::vector<double>> distanceMatrix;
		std::list<double> lines;
};

#endif