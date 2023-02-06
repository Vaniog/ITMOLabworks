#include "parser/HafArgs.h"
#include "haf_lib/HafManager.h"

int main(int argc, char** argv) {
    HafArgs haf_args;
    if (!haf_args.Parse(argc, argv))
        return 0;
    HafManager haf_manager(haf_args);
    haf_manager.DoJob();
    return 0;
}