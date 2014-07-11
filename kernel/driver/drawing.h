#include "frameBuffer.h"

extern void setForeColour(u16 colour);
extern void setGraphicsAddress(struct FrameBufferDescription* address);
extern void drawPixel(int x, int y);
extern void drawLine(int x0, int y0, int x1, int y1);
extern void drawCharacter(char character, u32 x, u32 y);
extern void drawString(char* string, u32 length, u32 x, u32 y);
