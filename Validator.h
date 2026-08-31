#pragma once
#pragma once
#include <string>
#include<iostream>
#include "Date.h"
#include <limits>

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

			std::cout << "Enter your phone number : ";
			PhoneNumber = Validator::ReadString();


			while (!isAllDigits(PhoneNumber))
			{
				PhoneNumber.clear();
				std::cout << "\nInvalid phone number. Please enter exactly "
					<< PHONE_LENGTH << " digits: ";
				PhoneNumber = ReadString();
			}



			return PhoneNumber;


		}
		static std::string ReadPincode() {

			std::string pincode;
			pincode.resize(PINCODE_LENGTH);

			for (size_t i = 0; i < PINCODE_LENGTH; i++)
			{
				std::cout << "\nEnter your " + std::to_string(i + 1) + "th  digit : ";

				pincode[i] = static_cast<char>('0' + (size_t)Validator::returnValidatedNumber(0, 9)); // shifts form 48 (offset in ascii table) 

			}

			return pincode;


		}

		static bool isNumberBetween(double Number, double from, double to) {
			return (Number >= from && Number <= to);
		}


		static double returnValidatedNumber(double from = 0, double to = 0, const std::string& invalidMessage = "Invalid Number, Please enter a valid number : ")
		{

			double number = 0;
			bool hasRange = !(from == 0 && to == 0);

		
			std::cin >> number;

			while (std::cin.fail() || (hasRange && !isNumberBetween(number, from, to)))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\n" << invalidMessage;
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


