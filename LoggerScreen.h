#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include <limits>
#include <string>
#include <sstream>
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

    static constexpr size_t ACTIVITY_LINE_WIDTH = 116;
    static constexpr size_t AUDIT_LINE_WIDTH = 114;

    Session& m_Session;
    Service& m_ServiceRef;

    void PrintHeader(const User& CurrentUser, const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {

        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t\t  " << (((ScreenName != nullptr) ? ScreenName : "Logger Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";

        ShowUserAndDate(CurrentUser);
    }

    // ------------------------------------------------ Menu

    void _MenuLayout() {

        _ClearScreen();
        PrintHeader(m_Session.GetUser());

        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\t     Logger Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t[1] Activity Logs.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[2] Audit Logs.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[3] Main Menu.\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
    }

    static void _ExitMenu(bool& isInMainMenu, const char* message = "\nGetting Back to Main Menu...")
    {
        std::cout << message << "\n\n";

        isInMainMenu = false;
    }

    MenuComponents _NavigateUser(double from = 1, double to = 3)
    {
        _Message("Choose What do you want to do ? [1 to 3] : ");

        return  (MenuComponents)Validator::returnValidatedNumber(from, to);
    }

    void _Menu(bool& isInMainMenu, const User& CurrentUser) {

        do
        {
            _MenuLayout();

            switch (_NavigateUser())
            {
            case MenuComponents::Activity:
            {
                

                _ShowLogs(CurrentUser);
                _GetBackToMenu("Press Enter to go back to Logger Menu");
                break;
            }
            case MenuComponents::Audits:
            {
               

                _ShowAuditLogs(CurrentUser);
                _GetBackToMenu("Press Enter to go back to Logger Menu");
                break;
            }
            case MenuComponents::Exit:
            {
                _ExitMenu(isInMainMenu);
                break;
            }
            default: //for later enum choices
            {
                break;
            }
            }

        } while (isInMainMenu);
    }


    void _ShowLogs(const User& CurrentUser) {

        if(Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::ShowActivityLogs)) 
        {


            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::ShowLogs, Logger::Level::INFO);
            _ClearScreen();

            const std::vector<std::string> Logs = FileHandler::LoadLogs();

            if (Logs.size() == 0)
            {

                _Message("\t\t\t\tNo Logs Available In the System!.\n");

            }

            else
            {
                const std::string SubTitle = "\t    (" + std::to_string(Logs.size()) + ") Log(s), newest first.";

                PrintHeader(CurrentUser, "Activity Logs Screen", SubTitle.c_str());
                _PrintLayout();
                _PrintAll(Logs);

            }

        }
        else 
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Activity Logs");
            NoAccessMsg();

        }
       
    }



    void _ShowAuditLogs(const User& CurrentUser) {

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::ShowAuditLogs)) 
        {
            _ClearScreen();

            const std::vector<std::string> Logs = FileHandler::LoadAuditLogs();

            if (Logs.size() == 0)
            {

                _Message("\t\t\t\tNo Audit Logs Available In the System!.\n");

            }

            else
            {
                const std::string SubTitle = "\t    (" + std::to_string(Logs.size()) + ") Transfer(s), newest first.";

                PrintHeader(CurrentUser, "Audit Logs Screen (Transfers)", SubTitle.c_str());
                _PrintAuditLayout();
                _PrintAllAudits(Logs);

            }
        }
        else 
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Audit Logs");
            NoAccessMsg();
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
            NoAccessMsg();
        }

    }

    //exclusive
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
            std::ostringstream Out;
            Out << std::fixed << std::setprecision(2) << std::stod(Number);

            return Out.str();
        }
        catch (const std::exception&)
        {
            return Number; // not a number, show it as it is
        }
    }

   

    static void _PrintFormattedLog(const std::string& Line)
    {
        const std::vector<std::string> Tokens = Parser::TokensToVec(Line);

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(20) << std::left << _GetDate(Tokens[LogFields::Prefix]);
        std::cout << "| " << std::setw(7) << std::left << _GetLevel(Tokens[LogFields::Prefix]);
        std::cout << "| " << std::setw(15) << std::left << _GetUsername(Tokens[LogFields::Username]);
        std::cout << "| " << std::setw(20) << std::left << Tokens[LogFields::Category];
        std::cout << "| " << std::left << _GetDetails(Tokens);

    }
    static void _PrintLine(size_t Width = ACTIVITY_LINE_WIDTH) {
        std::cout << std::setw(8) << std::left << "" << "\n\t" << std::string(Width, '_') << "\n" << std::endl;
    }
    static void _PrintLayout() {

        _PrintLine();
        std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(20) << "Date & Time";
        std::cout << "| " << std::left << std::setw(7) << "Level";
        std::cout << "| " << std::left << std::setw(15) << "Username";
        std::cout << "| " << std::left << std::setw(20) << "Category";
        std::cout << "| " << std::left << "Details";
        _PrintLine();
    }
    static void _PrintAll(const std::vector<std::string>& Logs) {

        for (size_t i = Logs.size(); i > 0; i--)
        {

            _PrintFormattedLog(Logs[i - 1]);
            std::cout << std::endl;
        }
        _PrintLine();

    }

    // ------------------------------------------------ Printing : Audits

    static void _PrintFormattedAuditLog(const std::string& Line)
    {
        const std::vector<std::string> Tokens = Parser::TokensToVec(Line);

        const std::string TransferMessage = (Tokens.size() > LogFields::Message) ? Tokens[LogFields::Message] : "";

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(20) << std::left << _GetDate(Tokens[LogFields::Prefix]);
        std::cout << "| " << std::setw(7) << std::left << _GetLevel(Tokens[LogFields::Prefix]);
        std::cout << "| " << std::setw(15) << std::left << _GetUsername(Tokens[LogFields::Username]);
        std::cout << "| " << std::setw(8) << std::left << _GetTransferFrom(TransferMessage);
        std::cout << "| " << std::setw(8) << std::left << _GetTransferTo(TransferMessage);
        std::cout << "| " << std::setw(14) << std::left << _FormatMoney(_GetTransferAmount(TransferMessage));
        std::cout << "| " << std::setw(14) << std::left << _FormatMoney(_GetFromNewBalance(TransferMessage));
        std::cout << "| " << std::left << _FormatMoney(_GetToNewBalance(TransferMessage));

    }
    static void _PrintAuditLayout() {

        _PrintLine(AUDIT_LINE_WIDTH);
        std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(20) << "Date & Time";
        std::cout << "| " << std::left << std::setw(7) << "Level";
        std::cout << "| " << std::left << std::setw(15) << "Username";
        std::cout << "| " << std::left << std::setw(8) << "From";
        std::cout << "| " << std::left << std::setw(8) << "To";
        std::cout << "| " << std::left << std::setw(14) << "Amount";
        std::cout << "| " << std::left << std::setw(14) << "From Balance";
        std::cout << "| " << std::left << "To Balance";
        _PrintLine(AUDIT_LINE_WIDTH);
    }
    static void _PrintAllAudits(const std::vector<std::string>& Logs) {

        for (size_t i = Logs.size(); i > 0; i--)
        {

            _PrintFormattedAuditLog(Logs[i - 1]);
            std::cout << std::endl;
        }
        _PrintLine(AUDIT_LINE_WIDTH);

    }


public:


    LoggerScreen(Service& Ref, Session& SessionRef) : Screen(Ref), m_Session(SessionRef), m_ServiceRef(Ref) {};

    void Start(const User& CurrentUser) override {

        PerformMenu(CurrentUser);
        _GetBackToMenu();

    }
};

