#include <iostream>
#include <map>

class App {
private:
    map<string, ICommand*> commands;
public:
    App(map<string, ICommand*> commands);
    void run();
}
int initializeAndRun(int argc, char *argv[], bool is_exitable=false);