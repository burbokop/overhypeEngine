#include "engine.h"



std::string ats(int argc, char *argv[]) {
    std::string result;
    for(int i = 0; i < argc; i++)
        result += argv[i];
    return result;
}

int main(int argc, char *argv[])
{
    std::cout << COLOR_CYAN << "argc: " << argc << " argv: " << ats(argc, argv) << COLOR_RESET << "\n";
    std::ifstream input("./entry.js");
    std::string string((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    v8::Isolate *isolate = Engine::Init(argc, argv);
    Engine::RunEntryPoint(isolate, string.c_str());
    Engine::Quit(isolate);
    std::cout << COLOR_CYAN << "no errors, program ended" << COLOR_RESET << "\n";
}
