#pragma once
#pragma once
#include <string>
#include<iostream>
#include "Date.h"

	class Validator
	{
	public:

		static bool isNumberBetween(double Number, double from, double to) {
			return (Number >= from && Number <= to);
		}

		static double returnValidatedNumber(const std::string& invalidMessage = "Invalid Number", double from = 0, double to = 0)
		{

			double number = 0;
			bool hasRange = !(from == 0 && to == 0);

		
			std::cin >> number;

			while (std::cin.fail() || (hasRange && !isNumberBetween(number, from, to)))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\n" << invalidMessage << "\n";
				std::cin >> number;

			}

			return number;

		}
		static double returnNumber(const std::string& invalidMessage = "Invalid Number")
		{

			double number = 0;
		
			std::cin >> number;

			while (std::cin.fail() || number < 0 )
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\n" << invalidMessage << "\n";
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
			return (toupper(returnValidatedChar(msg + " y / n : ", "YyNn")) == 'Y');
		}

	};


