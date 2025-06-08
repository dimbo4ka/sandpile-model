#include "../lib/parser/parser.h"
#include "../lib/iterations/iterations.h"

using namespace std;

int main(int argc, char** argv) {
    std::optional<Arguments> args = ParseArguments(argc, argv);
    if (!args.has_value())
        return 1;

    Iterate(*args);
    return 0;
}
