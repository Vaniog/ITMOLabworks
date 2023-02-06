#include "Sand_Heap/SandHeap.h"

int main(int argc, char** argv) {
    SandData sand_data;
    sand_data.Parse(argc, argv);
    SandHeap sand_heap(sand_data);
    sand_heap.Run();
    return 0;
}