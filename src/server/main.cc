#include "../../include/SearchEngineServer.h"

int main()
{
    SearchEngineServer server("127.0.0.1",8888);
    server.start();
}