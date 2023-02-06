#pragma once
#include "Param.h"

#include <iostream>
#include <map>
#include <memory>

namespace omfl {
class Section : public Param {
    std::map<std::string, std::shared_ptr<Section>> sections_;
public:
    Section() = default;
    explicit Section(Param& param);
    [[nodiscard]] Section& Get(const std::string& name) const;
    bool AddParam(const std::string& name, const std::shared_ptr<Param>& param);
    void AddSection(const std::string& name);
private:
    static std::pair<std::string, std::string> SplitByFirstDot(const std::string& str);
};

}