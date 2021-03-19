#include <time.h>

long double lastUpdateError;
clock_t currentTime;
clock_t startTime;
clock_t lastUpdateTime;
clock_t timeSinceLastCharRender;

long double getTotalSeconds();
long double getSecondsSinceLastUpdate();
