#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include <stdbool.h>

// Forward declare the Node to avoid cyclic dependency
struct Node;

typedef struct
{
    int x;
    int y;
    int r;
    char *name;
    bool isConnected;                  // Indicates if this input is connected
    struct Node *connectedNode;        // Pointer to the node this is connected to (if any)
    int connectedOutputConnectorIndex; // The index of the connected output (if connected)
} InputConnector;

typedef struct OutputConnector
{
    int x;
    int y;
    int r;
    char *name;
    bool isDragging; // Indicates if this output is being dragged
} OutputConnector;

InputConnector *inputConnectorNew(int r, char *name);
OutputConnector *outputConnectorNew(int r, char *name);
void inputConnectorFree(InputConnector *ic);
void outputConnectorFree(OutputConnector *oc);

#endif // __CONNECTOR_H__