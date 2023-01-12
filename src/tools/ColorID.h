#ifndef _COLORID_H_
#define _COLORID_H_

//#include "../Core.h"

class ColorID {
private:
	static int counter;
public:
	//need to add mutex support!
	static int getNewId();
};

#endif