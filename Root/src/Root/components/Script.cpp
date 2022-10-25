#include "Script.h"

bool Script::internal_started()
{
    return internal_called_start;
}

void Script::internal_start()
{
    internal_called_start = true;
}
