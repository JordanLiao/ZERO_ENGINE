#include "ColorID.h"

int ColorID::counter = 3;

int ColorID::getNewId()
{
    return counter++;
}
