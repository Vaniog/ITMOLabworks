#pragma once
#include "../parser/HafArgs.h"
#include "HafArchive.h"
#include "HafStream.h"

struct HafManager {
    HafArgs haf_args;
    explicit HafManager(HafArgs& haf_args) : haf_args(haf_args) {
    }

    void DoJob();

    void Create();
    void Extract();
    void Append();
    void Delete();
    void Concatenate();
    void List() const;
};