#pragma once

#include <string>
#include <unordered_set>

class GameState
{
public:
    void SetFlag(const std::string& flag);
    bool HasFlag(const std::string& flag) const;

private:
    std::unordered_set<std::string> m_flags;
};