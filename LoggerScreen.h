#pragma once
#include <iostream>
#include<vector>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
#include "Validator.h"
#include "Session.h"
#include "Authorizer.h"
#include "FileHandler.h"
#include "Parser.h"
#include "Logger.h"
#include "Screen.h"

class LoggerScreen : public Screen
{
public:

    enum MenuComponents { Activity = 1, Audits = 2, Exit = 3 };

private:

    enum LogFields { Prefix = 0, Category = 1, Username = 2, Message = 3, Target = 4 };
    static constexpr const char* USERNAME_LABEL = "USERNAME : ";
    static constexpr const char* LEVEL_DATE_DELIMITER = " / ";
    static constexpr const char* TRANSFER_FROM_KEY = "Transfer From: ";
    static constexpr const char* TRANSFER_TO_KEY = ", To: ";
    static constexpr const char* TRANSFER_AMOUNT_KEY = ", Amount: ";
    static constexpr const char* TRANSFER_BALANCE_KEY = ", Balance: ";
    static constexpr const char* TRANSFER_BALANCE_DELIMITER = " -> ";

    static constexpr int ACTIVITY_W = 116;   // NEW: widths of the two tables (the old ACTIVITY_LINE_WIDTH / AUDIT_LINE_WIDTH were for the underscore lines)
    static constexpr int AUDIT_W = 112;
    static constexpr size_t PAGE = 18;       // NEW: rows per page (logs grow forever, so they are shown page by page)

    Session& m_Session;
    Service& m_ServiceRef;

    int m_LastIndex = 0;   // NEW: remembers the highlighted row

    // ------------------------------------------------ Menu

    void _MenuLayout() {

        _ClearScreen();
        term::Header("LOGS", "", m_Session.GetUser().GetUsername());   // NEW: replaces PrintHeader override + hand-printed menu box
        term::Section("MENU", std::string(term::g::Up) + term::g::Down + " move  " + term::g::Mid + "  Enter open  " + term::g::Mid + "  Esc back");
    }

    MenuComponents _NavigateUser()
    {
        static const std::vector<term::MenuItem> items = {
            { "ACTIVITY LOGS",  "everything users did, newest first" },
            { "AUDIT LOGS",     "every transfer with balances" },
            { "BACK",           "return to the main menu" }
        };

        int choice = term::Menu(items, m_LastIndex);

        if (choice < 0) return MenuComponents::Exit;           // Esc = go back

        m_LastIndex = choice;
        return static_cast<MenuComponents>(choice + 1);
    }

    void _Menu(bool& isInMainMenu, const User& CurrentUser) {

        do
        {
            _MenuLayout();

            const MenuComponents choice = _NavigateUser();

            if (choice != MenuComponents::Exit) _ClearScreen();

            switch (choice)
            {
            case MenuComponents::Activity:
            {
                _ShowLogs(CurrentUser);
                break;
            }
            case MenuComponents::Audits:
            {
                _ShowAuditLogs(CurrentUser);
                break;
            }
            case MenuComponents::Exit:
            {
                isInMainMenu = false;
                break;
            }
            default: //for later enum choices
            {
                break;
            }
            }

        } while (isInMainMenu);
    }

    // NEW: a message-only page (no logs / no access) waits for a key, a paged list does not (it ends with Esc)
    void _ShowLogs(const User& CurrentUser) {

        if(Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::ShowActivityLogs))
        {

            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::ShowLogs, Logger::Level::INFO);
            _ClearScreen();

            const std::vector<std::string> Logs = FileHandler::LoadLogs();

            if (Logs.size() == 0)
            {
                _Header(CurrentUser, "ACTIVITY LOGS", "0 RECORDS", ACTIVITY_W);
                term::Warn("No logs available in the system.", ACTIVITY_W);
                _GetBackToMenu("Press any key to go back to the Logs menu");
            }
            else
            {
                // newest first: row i shows Logs[size - 1 - i]
                term::Paginate(Logs.size(), PAGE, ACTIVITY_W,
                    [&](const std::string& info) { _Header(CurrentUser, "ACTIVITY LOGS", info, ACTIVITY_W); },
                    []() { _PrintLayout(); },
                    [&](size_t i) { _PrintFormattedLog(Logs[Logs.size() - 1 - i]); });
            }

        }
        else
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Activity Logs");
            _Header(CurrentUser, "ACTIVITY LOGS");
            NoAccessMsg();
            _GetBackToMenu("Press any key to go back to the Logs menu");
        }

    }

    void _ShowAuditLogs(const User& CurrentUser) {

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::ShowAuditLogs))
        {
            _ClearScreen();

            const std::vector<std::string> Logs = FileHandler::LoadAuditLogs();

            if (Logs.size() == 0)
            {
                _Header(CurrentUser, "AUDIT LOGS", "0 RECORDS", AUDIT_W);
                term::Warn("No audit logs available in the system.", AUDIT_W);
                _GetBackToMenu("Press any key to go back to the Logs menu");
            }
            else
            {
                term::Paginate(Logs.size(), PAGE, AUDIT_W,
                    [&](const std::string& info) { _Header(CurrentUser, "AUDIT LOGS", info, AUDIT_W); },
                    []() { _PrintAuditLayout(); },
                    [&](size_t i) { _PrintFormattedAuditLog(Logs[Logs.size() - 1 - i]); });
            }
        }
        else
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Audit Logs");
            _Header(CurrentUser, "AUDIT LOGS");
            NoAccessMsg();
            _GetBackToMenu("Press any key to go back to the Logs menu");
        }
    }

    void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

        bool isInMainMenu = true;

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::ShowLogs))
        {
            _Menu(isInMainMenu, CurrentUser);
        }
        else
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Show Logs");
            _Header(CurrentUser, "LOGS");
            NoAccessMsg();
            _GetBackToMenu();
        }

    }

    //exclusive  (the parsing helpers below are unchanged)
    static std::string _GetLevel(const std::string& Prefix) {

        const size_t pos = Prefix.find(LEVEL_DATE_DELIMITER);

        return Prefix.substr(1, pos - 1);
    }
    static std::string _GetDate(const std::string& Prefix) {

        const size_t pos = Prefix.find(LEVEL_DATE_DELIMITER);

        return Prefix.substr(pos + std::string(LEVEL_DATE_DELIMITER).length(), Prefix.length() - pos - std::string(LEVEL_DATE_DELIMITER).length() - 1);
    }
    static std::string _GetUsername(const std::string& Token) {

        const std::string Label = USERNAME_LABEL;

        return (Token.rfind(Label, 0) == 0) ? Token.substr(Label.length()) : Token;
    }
    static std::string _GetDetails(const std::vector<std::string>& Tokens) {

        std::string Details = Tokens[LogFields::Message];

        if (Tokens.size() > LogFields::Target)
        {
            const std::string& Target = Tokens[LogFields::Target];
            const size_t pos = Target.find(" : ");

            if (pos != std::string::npos && pos + 3 < Target.length())
            {
                Details += " (" + Target + ")";
            }
        }

        return Details;
    }

    static std::string _GetTransferField(const std::string& TransferMessage, const std::string& Key, const std::string& EndDelimiter) {

        const size_t start = TransferMessage.find(Key);

        if (start == std::string::npos)
        {
            return "-";
        }

        const size_t from = start + Key.length();
        const size_t to = (EndDelimiter.empty()) ? std::string::npos : TransferMessage.find(EndDelimiter, from);

        return TransferMessage.substr(from, (to == std::string::npos) ? std::string::npos : to - from);
    }
    static std::string _GetTransferFrom(const std::string& TransferMessage) {
        return _GetTransferField(TransferMessage, TRANSFER_FROM_KEY, ",");
    }
    static std::string _GetTransferTo(const std::string& TransferMessage) {
        return _GetTransferField(TransferMessage, TRANSFER_TO_KEY, ",");
    }
    static std::string _GetTransferAmount(const std::string& TransferMessage) {
        return _GetTransferField(TransferMessage, TRANSFER_AMOUNT_KEY, ",");
    }
    static std::string _GetFromNewBalance(const std::string& TransferMessage) {
        return _GetTransferField(TransferMessage, TRANSFER_BALANCE_KEY, TRANSFER_BALANCE_DELIMITER);
    }
    static std::string _GetToNewBalance(const std::string& TransferMessage) {
        return _GetTransferField(TransferMessage, TRANSFER_BALANCE_DELIMITER, "");
    }
    static std::string _FormatMoney(const std::string& Number) {

        try
        {
            return term::Money(std::stod(Number));   // NEW: "$1,250.00" like everywhere else (was fixed 2 decimals without $ or commas)
        }
        catch (const std::exception&)
        {
            return Number; // not a number, show it as it is
        }
    }

    // NEW: color by log level: INFO grey, WARN amber, ERROR red, FATAL magenta
    static const char* _LevelColor(const std::string& Level) {
        if (Level == "WARN")  return term::col::Amber;
        if (Level == "ERROR") return term::col::Red;
        if (Level == "FATAL") return term::col::Magenta;
        return term::col::Muted;
    }

    // ------------------------------------------------ Printing : Activity
    // widths: 20 + 7 + 14 + 20 + 44 details, with 2-space gaps and 2 on the left = 115

    static void _PrintFormattedLog(const std::string& Line)
    {
        const std::vector<std::string> Tokens = Parser::TokensToVec(Line);

        const std::string Level = _GetLevel(Tokens[LogFields::Prefix]);

        term::Print(std::string("  ") + term::col::Muted + term::Pad(_GetDate(Tokens[LogFields::Prefix]), 20)
                    + "  " + _LevelColor(Level) + term::Pad(Level, 7)
                    + term::col::Cyan + "  " + term::Pad(term::Fit(_GetUsername(Tokens[LogFields::Username]), 14), 14)
                    + term::col::Base + "  " + term::Pad(term::Fit(Tokens[LogFields::Category], 20), 20)
                    + term::col::Muted + "  " + term::Fit(_GetDetails(Tokens), 44), ACTIVITY_W);
    }
    static void _PrintLayout() {
        term::Print(std::string(term::col::Muted) + "  " + term::Pad("DATE & TIME", 20) + "  " + term::Pad("LEVEL", 7) + "  " + term::Pad("USER", 14)
                    + "  " + term::Pad("CATEGORY", 20) + "  " + "DETAILS", ACTIVITY_W);
        term::Rule(ACTIVITY_W);
    }

    // ------------------------------------------------ Printing : Audits
    // widths: 20 + 7 + 12 + 8 + 8 + 12 + 12 + 12 = 91, plus 7 gaps of 2 and 2 on the left = 107

    static void _PrintFormattedAuditLog(const std::string& Line)
    {
        const std::vector<std::string> Tokens = Parser::TokensToVec(Line);

        const std::string TransferMessage = (Tokens.size() > LogFields::Message) ? Tokens[LogFields::Message] : "";

        term::Print(std::string("  ") + term::col::Muted + term::Pad(_GetDate(Tokens[LogFields::Prefix]), 20)
                    + "  " + _LevelColor(_GetLevel(Tokens[LogFields::Prefix])) + term::Pad(_GetLevel(Tokens[LogFields::Prefix]), 7)
                    + term::col::Cyan + "  " + term::Pad(term::Fit(_GetUsername(Tokens[LogFields::Username]), 12), 12)
                    + term::col::Base + "  " + term::Pad(_GetTransferFrom(TransferMessage), 8)
                    + "  " + term::Pad(_GetTransferTo(TransferMessage), 8)
                    + term::col::White + "  " + term::PadLeft(_FormatMoney(_GetTransferAmount(TransferMessage)), 12)
                    + term::col::Amber + "  " + term::PadLeft(_FormatMoney(_GetFromNewBalance(TransferMessage)), 12)
                    + term::col::Green + "  " + term::PadLeft(_FormatMoney(_GetToNewBalance(TransferMessage)), 12), AUDIT_W);
    }
    static void _PrintAuditLayout() {
        term::Print(std::string(term::col::Muted) + "  " + term::Pad("DATE & TIME", 20) + "  " + term::Pad("LEVEL", 7) + "  " + term::Pad("USER", 12)
                    + "  " + term::Pad("FROM", 8) + "  " + term::Pad("TO", 8) + "  " + term::PadLeft("AMOUNT", 12)
                    + "  " + term::PadLeft("FROM BAL.", 12) + "  " + term::PadLeft("TO BAL.", 12), AUDIT_W);
        term::Rule(AUDIT_W);
    }

public:

    LoggerScreen(Service& Ref, Session& SessionRef) : Screen(Ref), m_Session(SessionRef), m_ServiceRef(Ref) {};

    void Start(const User& CurrentUser) override {

        PerformMenu(CurrentUser);

    }
};
