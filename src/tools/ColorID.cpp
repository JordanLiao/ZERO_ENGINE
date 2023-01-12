#include "ColorID.h"

int ColorID::counter = 3;

//need to add mutex support!
int ColorID::getNewId() {
    return counter++;
}
