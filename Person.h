#pragma once
#include<string>
class Person
{
private:
	std::string m_firstName = "";
	std::string m_lastName = "";
	std::string m_email = "";
	std::string m_phoneNumber = ""; //Rule of Zero since all string members handle their own memory managment, a destructor/move/copy and their operator overloads are all handeled by the compiler for u 

public:


	Person(std::string FirstName, std::string LastName, std::string Email, std::string PhoneNumber) : m_firstName(std::move(FirstName)), m_lastName(std::move(LastName)), m_email(std::move(Email)), m_phoneNumber(std::move(PhoneNumber))
	{

	};
	
	
	void SetFirstName(const std::string & NewFirstName)  {
		m_firstName = NewFirstName;
	}
	void SetLastName(const std::string& NewLastName)  {
		m_lastName = NewLastName;
	}
	void SetEmail(const std::string& NewEmail)  {
		m_email = NewEmail;
	}
	void SetPhoneNumber(const std::string& NewPhoneNumber)  {
		m_phoneNumber = NewPhoneNumber;
	}
	const std::string& GetFirstName() const  noexcept  {
		return m_firstName;
	}
	const std::string& GetLastName() const noexcept {
		return m_lastName;
	}
	const std::string& GetEmail() const noexcept {
		return m_email;
	}
	const std::string& GetPhoneNumber() const noexcept {
		return m_phoneNumber;
	}
	
	std::string GetFullName() const  {
		return m_firstName + " " + m_lastName;
	}

	
};

