
// g_framerate.cpp

#include "g_framerate.h"

void g_framerate::Init(float tfps)
{
  targetfps = tfps;
  QueryPerformanceCounter(&framedelay);
  QueryPerformanceFrequency(&tickspersecond);
}


void g_framerate::SetSpeedFactor()
{
  QueryPerformanceCounter(&currentticks);
  //This frame's length out of desired length
  speedfactor = (float)(currentticks.QuadPart-framedelay.QuadPart)/((float)tickspersecond.QuadPart/targetfps);
  fps = targetfps/speedfactor;
  if (speedfactor <= 0)
    speedfactor = 1;

  framedelay = currentticks;
}