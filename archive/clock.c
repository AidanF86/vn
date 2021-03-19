long double getTotalSeconds()
{
    return (long double)currentTime / CLOCKS_PER_SEC;
}

long double getSecondsSinceLastUpdate()
{
    return (long double)(currentTime - lastUpdateTime) / CLOCKS_PER_SEC;
}
