#pragma once
#include "FileHandler.h"
#include "Validator.h"
#include "Global.h"
#include<ctime>

class Logger
{
public:
	enum class Category : uint8_t
	{

		ShowClientList = 0,
		AddClient = 1,
		DeleteClient = 2,
		UpdateClient = 3,
		FindClient = 4,
		Deposit = 5,
		Withdraw = 6,
		Transfer = 7,
		ShowTotalBalance = 8,
		ManageUsers = 9,

		ShowUserList = 10,
		AddUser = 11,
		DeleteUser = 12,
		UpdateUser = 13,
		FindUser = 14,

		Login = 15,
		LoginAttempt = 16,
		NoAccsess = 17,
		ShowLogs = 18,


	};
	enum class Level : uint8_t
	{

		INFO = 0,
		WARN = 1,
		ERROR = 2,
		FATAL = 3,
	};
private:



	static std::string CategoryToString(Category category) {

		std::string table[] =
		{
			"Show Client List", "Add Clients", "Delete Client", "Update Client", "Find Client", "Deposit", "Withdraw", "Transfer", "Show Total Balance", "Manage Users", "Show User List", "Add User", "Delete User", "Update User", "Find User", "Login", "Login Attempt", "Operation Attempt", "Show Logs"
		};

		return table[static_cast<uint8_t>(category)];
	}
	static std::string LevelToString(Level level) {

		std::string table[] =
		{
			"INFO", "WARN", "ERROR", "FATAL"
		};

		return table[static_cast<uint8_t>(level)];
	}
	static std::string BuildPrefix(const std::string& Username, Category category, Level level) {
		time_t now = time(nullptr);
		char buf[32];

		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));

		return "[" + LevelToString(level) + " / " + buf + "]" + LOG_DELIMITER + CategoryToString(category) + LOG_DELIMITER + "USERNAME : " + Username;
	}


public:
	static void LogUser(const std::string& CurrentUsername, Category category, Level level, const std::string& op = "", const std::string& TargetUsername = "") {



		std::string Prefix = BuildPrefix(CurrentUsername, category, level);
		std::string Target = "TARGET USER : " + TargetUsername;
		std::string msg;

		switch (category)
		{
		case Category::Login:
		{
			msg = Prefix + LOG_DELIMITER + "Logged in successfully";

			break;
		}

		case Category::LoginAttempt:
		{
			msg = Prefix + LOG_DELIMITER + "Failed login attempt" + LOG_DELIMITER + Target;
			break;
		}
		case Category::NoAccsess:
		{
			msg = Prefix + LOG_DELIMITER + "No Access for " + op + LOG_DELIMITER;
			break;
		}
		case Category::ManageUsers:
		{
			msg = Prefix + LOG_DELIMITER + "Opened user management";
			break;
		}
		case Category::ShowLogs:
		{
			msg = Prefix + LOG_DELIMITER + "Viewed the activity logs";
			break;
		}

		case Category::ShowUserList:
		{
			msg = Prefix + LOG_DELIMITER + "Viewed the user list";
			break;
		}

		case Category::AddUser:
		{

			msg = Prefix + LOG_DELIMITER + "Created a new user" + LOG_DELIMITER + Target;
			break;
		}

		case Category::DeleteUser:
		{
			msg = Prefix + LOG_DELIMITER + "Deleted a user" + LOG_DELIMITER + Target;
			break;
		}

		case Category::UpdateUser:
		{
			msg = Prefix + LOG_DELIMITER + "Updated user data / permissions" + LOG_DELIMITER + Target;
			break;
		}

		case Category::FindUser:
		{
			msg = Prefix + LOG_DELIMITER + "Searched for a user" + LOG_DELIMITER + Target;
			break;
		}

		default:
		{
			return;
		}

		}

		FileHandler::SaveLog(msg);



	}

	static void LogClient(const std::string& CurrentUsername, Category category, Level level, const std::string& op = "", const std::string& TargetClientAccNum = "", double amount = 0)
	{

		std::string Prefix = BuildPrefix(CurrentUsername, category, level);
		std::string Target = "Account Number : " + TargetClientAccNum;
		std::string msg;

		switch (category)
		{
		case Category::ShowClientList:
		{
			msg = Prefix + LOG_DELIMITER + "Viewed the client list";
			break;
		}

		case Category::AddClient:
		{
			msg = Prefix + LOG_DELIMITER + "Added a new client" + LOG_DELIMITER + Target;
			break;
		}

		case Category::DeleteClient:
		{
			msg = Prefix + LOG_DELIMITER + "Deleted a client" + LOG_DELIMITER + Target;
			break;
		}

		case Category::UpdateClient:
		{
			msg = Prefix + LOG_DELIMITER + "Updated client data" + LOG_DELIMITER + Target;
			break;
		}

		case Category::FindClient:
		{
			msg = Prefix + LOG_DELIMITER + "Searched for a client" + LOG_DELIMITER + Target;
			break;
		}

		case Category::Deposit:
		{
			msg = Prefix + LOG_DELIMITER + "Deposited " + std::to_string(amount) + LOG_DELIMITER + Target;
			break;
		}

		case Category::Withdraw:
		{
			msg = Prefix + LOG_DELIMITER + "Withdrew " + std::to_string(amount) + LOG_DELIMITER + Target;
			break;
		}
		case Category::NoAccsess:
		{
			msg = Prefix + LOG_DELIMITER + "No Access for " + op + LOG_DELIMITER;
			break;
		}

		case Category::ShowTotalBalance:
		{
			msg = Prefix + LOG_DELIMITER + "Viewed total balances";
			break;
		}

		default:
		{
			return;
		}

		}
		FileHandler::SaveLog(msg);
	}

	static void LogTransfer(const std::string& CurrentUsername, Category category, Level level, const std::string& ClientFromAccNum, const std::string& ClientToAccNum, double FromBalance, double ToBalance, double FromNewBalance, double ToNewBalance, double amount = 0) {



		if (category != Category::Transfer)
		{
			return;
		}
		else
		{
			std::string Prefix = BuildPrefix(CurrentUsername, category, level);
			std::string TransferLog =
				"Transfer From: " + ClientFromAccNum +
				", To: " + ClientToAccNum +
				", Amount: " + std::to_string(amount) +
				", Balance: " + std::to_string(FromNewBalance) +
				" -> " + std::to_string(ToNewBalance);

			std::string msg = Prefix + LOG_DELIMITER + TransferLog;
			 
			FileHandler::SaveLogTransfer(msg);
		}


	}

};
