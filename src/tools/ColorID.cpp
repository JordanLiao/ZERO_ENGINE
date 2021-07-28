#include "ColorID.h"

int ColorID::counter = 1;

int ColorID::getNewId()
{
    return counter++;
}
