#define USE_MEMD
#define MEMD_IMPLEMENTATION
#include "deps/memd.h"

#include <stdio.h>

#define MINIGFX_IMPLEMENTATION
#include "deps/minigfx.h"

#include "node.h"
#include "constants.h"

const int SCREEN_W = 1280;
const int SCREEN_H = 720;

int main(int argc, char *argv[])
{
    MgSurface *screen = mgWindow(SCREEN_W, SCREEN_H, "LogiMax", MG_FIXED);

    Node *node1 = nodeNew(10, 10, "Ic 1", mgRGB(100, 130, 100));
    Node *node2 = nodeNew(10, 120, "AND", mgRGB(130, 100, 100));
    nodeAddInput(node1, "Vcc");
    nodeAddInput(node1, "GND");
    nodeAddInput(node1, "CLK");
    nodeAddInput(node1, "ENBL");
    nodeAddInput(node1, "RESET");
    nodeAddOutput(node1, "O1");
    nodeAddOutput(node1, "O2");
    nodeAddOutput(node1, "O3");

    nodeAddInput(node2, "I1");
    nodeAddInput(node2, "I2");
    nodeAddOutput(node2, "O1");

    while (!mgClosed(screen))
    {
        mgClear(screen, mgRGB(0, 0, 0));
        mgRect(screen, 0, 0, SCREEN_W, SCREEN_H, mgRGB(45, 45, 45));

        nodeUpdate(node1, screen);
        nodeDraw(node1, screen);

        nodeUpdate(node2, screen);
        nodeDraw(node2, screen);

        mgFillRect(screen, -5, SCREEN_H - 80, SCREEN_W + 10, 85, mgRGB(45, 45, 45));

        mgUpdate(screen);
    }
    mgFree(screen);
    //
    // MEMD stuff
    char *report = memd_report();
    printf(report);
    memd_report_free(report);
    return 0;
}