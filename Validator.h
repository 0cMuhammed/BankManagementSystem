#pragma once
#pragma once
#include <string>
#include<iostream>
#include "Date.h"
#include <limits>
#include "Global.h"
#include "Terminal.h"   // NEW: so prompts and error messages use the same look as the screens

#define PINCODE_LENGTH 4


#define PHONE_LENGTH 10
#define ZERO_ASCII 48
#define NINE_ASCII 57

	class Validator
	{
	public:
		static bool isAllDigits(const std::string& PhoneNumber) {
			
			if (PhoneNumber.length() != PHONE_LENGTH) {
				return false;
			}

			for (size_t i = 0; i < PHONE_LENGTH; i++)
			{
				if (!isNumberBetween(PhoneNumber[i], ZERO_ASCII, NINE_ASCII))
				{
					return false;
				}
			}

			return true;
		}


		static std::string  ReadPhoneNumber() {

			std::string PhoneNumber = "";
			PhoneNumber.reserve(PHONE_LENGTH);

			term::Prompt("PHONE");
			PhoneNumber = Validator::ReadString();


			while (!isAllDigits(PhoneNumber))
			{
				PhoneNumber.clear();
				term::Error("Invalid phone number. Please enter exactly " + std::to_string(PHONE_LENGTH) + " digits.");
				term::Prompt("PHONE");
				PhoneNumber = ReadString();
			}



			return PhoneNumber;


		}
		static std::string ReadPincode() {

			std::string pincode;
			pincode.resize(PINCODE_LENGTH);

			term::Info("Enter the pincode, one digit at a time.");
			for (size_t i = 0; i < PINCODE_LENGTH; i++)
			{
				term::Prompt("DIGIT " + std::to_string(i + 1));

				pincode[i] = static_cast<char>('0' + (size_t)Validator::returnValidatedNumber(0, 9)); // shifts form 48 (offset in ascii table) 

			}

			return pincode;


		}

		static bool isNumberBetween(double Number, double from, double to) {
			return (Number >= from && Number <= to);
		}


		static double returnValidatedNumber(double from = 0, double to = 0, const std::string& invalidMessage = "Invalid number. Please enter a valid number.")
		{

			double number = 0;
			bool hasRange = !(from == 0 && to == 0);

		
			std::cin >> number;

			while (std::cin.fail() || (hasRange && !isNumberBetween(number, from, to)))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				term::Error(invalidMessage);
				term::Prompt("TRY AGAIN");
				std::cin >> number;

			}

			return number;

		}
		static double returnNumber(const std::string& invalidMessage = "Invalid Number")
		{

			double number = 0;
		
			std::cin >> number;

			while (std::cin.fail() || number <= 0 )
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				term::Error(invalidMessage);
				term::Prompt("TRY AGAIN");
				std::cin >> number;

			}

			return number;

		}


		static bool isDateBetween(const Date& DateParameter, const Date& dateFrom, const Date& dateTo) {
			if (
				(Date::IsDate1AfterDate2(DateParameter, dateFrom) ||
					Date::IsDate1EqualDate2(DateParameter, dateFrom))
				&&
				(Date::IsDate1BeforeDate2(DateParameter, dateTo) ||
					Date::IsDate1EqualDate2(DateParameter, dateTo))
				)
			{
				return true;
			}

			return false;

		}
		static char returnValidatedChar(const std::string& message, const  std::string& validChars = "") {
			char charr;
			std::cout << message;
			std::cin >> charr;

			while (std::cin.fail() || (!validChars.empty() && validChars.find(charr) == std::string::npos))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\nInvalid Charactar, " << message;
				std::cin >> charr;

			}
			return charr;
		}

		static bool IsValidDate(const Date& Date)
		{
			return	Date::IsValidDate(Date);
		}

		static std::string ReadString()
		{
			std::string  S1 = "";
			std::getline(std::cin >> std::ws, S1);
			return S1;
		}

		static bool GetConfirmation(const std::string& msg) {
			// NEW: every old call site now gets the single-key Y/N question automatically.
			// Old callers pass messages like "\nDo you want to change your email ? y/n : " - we clean them:
			std::string m = msg;
			size_t first = m.find_first_not_of('\n');                      // cut leading newlines
			m = (first == std::string::npos) ? std::string() : m.substr(first);
			for (;;) {                                                     // cut trailing decoration: spaces, ':' and "y/n"
				while (!m.empty() && (m.back() == ' ' || m.back() == '\n')) m.pop_back();
				if (m.size() >= 3 && m.compare(m.size() - 3, 3, "y/n") == 0) { m.erase(m.size() - 3); continue; }
				if (!m.empty() && m.back() == ':') { m.pop_back(); continue; }
				break;
			}
			if (m.size() >= 2 && m.compare(m.size() - 2, 2, " ?") == 0) m.erase(m.size() - 2, 1);   // "name ?" -> "name?"
			term::Blank();
			return term::Confirm(m);
		}
		static bool IsInvalid(const std::string& any) {
			return any.find_first_of(DELIMITER) != std::string::npos; // so nothing can intervene with the delimiter in the file and cause a File I/O error
		}
	
		static std::string ReadNames(const std::string& Order) {

			term::Prompt(term::Upper(Order) + " NAME");
			std::string name = Validator::ReadString();

			while (IsInvalid(name)) {

				term::Error("Invalid " + Order + " name (it contains a reserved character).");
				term::Prompt(term::Upper(Order) + " NAME");

				name = Validator::ReadString();
			}

			return name;

		}

		static std::string  ReadEmails() {

			term::Prompt("EMAIL");
			std::string email = Validator::ReadString();

			while (IsInvalid(email)) {

				term::Error("Invalid email (it contains a reserved character).");
				term::Prompt("EMAIL");

				email = Validator::ReadString();
			}

			return email;

		}

	};


