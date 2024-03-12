#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "connector.h"

InputConnector *inputConnectorNew(int r, char *name)
{
    InputConnector *ic = malloc(sizeof(InputConnector));
    ic->x = 0;
    ic->y = 0;
    ic->r = r;
    ic->name = strdup(name);
    ic->isConnected = false;
    ic->connectedNode = NULL;
    ic->connectedOutputConnectorIndex = -1;

    return ic;
}

OutputConnector *outputConnectorNew(int r, char *name)
{
    OutputConnector *oc = malloc(sizeof(OutputConnector));
    oc->x = 0;
    oc->y = 0;
    oc->r = r;
    oc->name = strdup(name);
    oc->isDragging = false;

    return oc;
}

void inputConnectorFree(InputConnector *ic)
{
    // we dont want to free 'connectedNode' because it might be used elsewhere
    free(ic->name);
    free(ic);
}

void outputConnectorFree(OutputConnector *oc)
{
    free(oc->name);
    free(oc);
}