#pragma once
#include "Tube.h"
#include <vector>



class TubeSystem {

private:
	std::vector<std::vector<Tube*>> TubeMap;

	int width, height;

public:
	TubeSystem(int szerokosc, int wysokosc);
	//~TubeSystem(); // sprz¹ta pamiêæ

	Tube* getTube(int x, int y) const;
	void setTube(int x, int y, Tube* tube);

};
