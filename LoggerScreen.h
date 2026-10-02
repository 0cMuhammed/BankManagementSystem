#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include <limits>
#include "Session.h"
#include "Authorizer.h"
#include "FileHandler.h"
#include "Parser.h"
#include "Logger.h"
#include "Screen.h"


class LoggerScreen : public Screen
{
private :

    enum LogFields { Prefix = 0, Category = 1, Username = 2, Message = 3, Target = 4 }; // positions of the tokens inside one log line

    static constexpr const char* USERNAME_LABEL = "USERNAME : ";
    static constexpr const char* LEVEL_DATE_DELIMITER = " / ";

    void PrintHeader(const User &CurrentUser, const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {

        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t\t  " << (((ScreenName != nullptr) ? ScreenName : "Activity Logs Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";

        ShowUserAndDate(CurrentUser);
    }

    void _ShowLogs(const User &CurrentUser) {

        _ClearScreen();

        const std::vector<std::string> Logs = FileHandler::LoadLogs();

        if (Logs.size() == 0)
        {

            _Message("\t\t\t\tNo Logs Available In the System!.\n");

        }

        else
        {
            const std::string SubTitle = "\t    (" + std::to_string(Logs.size()) + ") Log(s), newest first.";

            PrintHeader(CurrentUser, nullptr, SubTitle.c_str());
            _PrintLayout();
            _PrintAll(Logs);

        }
    }

    void PerformMenu(const User &CurrentUser, const char* Message = nullptr) override {

        if (Authorizer::IsAdmin(CurrentUser)) // Only Admins
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::ShowLogs, Logger::Level::INFO);
            _ShowLogs(CurrentUser);
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

    static void _PrintFormattedLog(const std::string& Line)
    {
        const std::vector<std::string> Tokens = Parser::TokensToVec(Line); 

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(20) << std::left << _GetDate(Tokens[LogFields::Prefix]);
        std::cout << "| " << std::setw(7) << std::left << _GetLevel(Tokens[LogFields::Prefix]);
        std::cout << "| " << std::setw(15) << std::left << _GetUsername(Tokens[LogFields::Username]);
        std::cout << "| " << std::setw(20) << std::left << Tokens[LogFields::Category];
        std::cout << "| " << std::left << _GetDetails(Tokens);

    }
    static void _PrintLine() {
        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_____________________________________________________________\n" << std::endl;
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

            _PrintFormattedLog(Logs[i-1]);
            std::cout << std::endl;
        }
        _PrintLine();

    }


public:


    LoggerScreen(Service& Ref) : Screen(Ref) {};

    void Start(const User &CurrentUser) override {

        PerformMenu(CurrentUser);
        _GetBackToMenu();

    }
};

