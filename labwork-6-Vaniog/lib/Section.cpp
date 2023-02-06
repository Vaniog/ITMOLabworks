#include "Section.h"
#include "IstreamFuncs.h"

#include <sstream>

namespace omfl {
Section::Section(Param& param) : Param(param) {
}

Section& omfl::Section::Get(const std::string& name) const { // NOLINT (recursive)
    auto split = SplitByFirstDot(name);
    if (split.second.empty()) {
        return *sections_.find(name)->second;
    }
    return sections_.find(split.first)->second->Get(split.second);
}

bool Section::AddParam(const std::string& name, const std::shared_ptr<Param>& param) {
    if (sections_.find(name) != sections_.end()) {
        return false;
    }
    std::shared_ptr<Section> section = std::make_shared<Section>(*param);
    sections_.insert(std::make_pair(name, section));
    return true;
}

void Section::AddSection(const std::string& name) { // NOLINT (recursive)
    auto split = SplitByFirstDot(name);
    if (split.second.empty()) {
        sections_.insert(std::make_pair(name, std::make_shared<Section>()));
        return;
    }
    if (sections_.find(split.first) == sections_.end())
        sections_.insert(std::make_pair(split.first, std::make_shared<Section>()));
    sections_.find(split.first)->second->AddSection(split.second);
}

std::pair<std::string, std::string> Section::SplitByFirstDot(const std::string& str) {
    std::stringstream iss(str);
    std::string str_1 = isf::GetWhile(iss, [](uint8_t c){return c != '.';});
    iss.ignore(); // ignore dot
    std::string str_2;
    if (iss)
        str_2 = iss.str().substr(iss.tellg());
    return std::make_pair(str_1, str_2);
}

}