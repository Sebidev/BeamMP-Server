#pragma once

#include "Cryptography.h"
#include "commandline.h"
#include <atomic>
#include <fstream>
#include <functional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class TLuaEngine;

class TConsole {
public:
    TConsole();

    void Write(const std::string& str);
    void WriteRaw(const std::string& str);
    void InitializeLuaConsole(TLuaEngine& Engine);
    void BackupOldLog();
    void StartLoggingToFile();
    Commandline& Internal() { return mCommandline; }

private:
    void RunAsCommand(const std::string& cmd, bool IgnoreNotACommand = false);
    void ChangeToLuaConsole(const std::string& LuaStateId);
    void ChangeToRegularConsole();
    void HandleLuaInternalCommand(const std::string& cmd);

    void Command_Lua(const std::string& cmd, const std::vector<std::string>& args);
    void Command_Help(const std::string& cmd, const std::vector<std::string>& args);
    void Command_Kick(const std::string& cmd, const std::vector<std::string>& args);
    void Command_List(const std::string& cmd, const std::vector<std::string>& args);
    void Command_Status(const std::string& cmd, const std::vector<std::string>& args);
    void Command_Settings(const std::string& cmd, const std::vector<std::string>& args);
    void Command_Clear(const std::string&, const std::vector<std::string>& args);

    void Command_Say(const std::string& FullCommand);
    bool EnsureArgsCount(const std::vector<std::string>& args, size_t n);
    bool EnsureArgsCount(const std::vector<std::string>& args, size_t min, size_t max);

    static std::tuple<std::string, std::vector<std::string>> ParseCommand(const std::string& cmd);
    static std::string ConcatArgs(const std::vector<std::string>& args, char space = ' ');

    std::unordered_map<std::string, std::function<void(const std::string&, const std::vector<std::string>&)>> mCommandMap = {
        { "lua", [this](const auto& a, const auto& b) { Command_Lua(a, b); } },
        { "help", [this](const auto& a, const auto& b) { Command_Help(a, b); } },
        { "kick", [this](const auto& a, const auto& b) { Command_Kick(a, b); } },
        { "list", [this](const auto& a, const auto& b) { Command_List(a, b); } },
        { "status", [this](const auto& a, const auto& b) { Command_Status(a, b); } },
        { "settings", [this](const auto& a, const auto& b) { Command_Settings(a, b); } },
        { "clear", [this](const auto& a, const auto& b) { Command_Clear(a, b); } },
        { "say", [this](const auto&, const auto&) { Command_Say(""); } }, // shouldn't actually be called
    };

    Commandline mCommandline;
    std::vector<std::string> mCachedLuaHistory;
    std::vector<std::string> mCachedRegularHistory;
    TLuaEngine* mLuaEngine { nullptr };
    bool mIsLuaConsole { false };
    bool mFirstTime { true };
    std::string mStateId;
    const std::string mDefaultStateId = "BEAMMP_SERVER_CONSOLE";
    std::ofstream mLogFileStream;
    std::mutex mLogFileStreamMtx;
};
