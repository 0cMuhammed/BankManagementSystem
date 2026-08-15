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

	Person() = default;
	Person(const std::string& FirstName, const std::string& LastName, const std::string& Email, const std::string& PhoneNumber) : m_firstName(FirstName), m_lastName(LastName), m_email(Email), m_phoneNumber(PhoneNumber) 
	{

	};
	
	
	void setFirstName(const std::string & NewFirstName)  {
		m_firstName = NewFirstName;
	}
	void setLastName(const std::string& NewLastName)  {
		m_lastName = NewLastName;
	}
	void setEmail(const std::string& NewEmail)  {
		m_email = NewEmail;
	}
	void setPhoneNumber(const std::string& NewPhoneNumber)  {
		m_phoneNumber = NewPhoneNumber;
	}
	const std::string& getFirstName() const  noexcept  {
		return m_firstName;
	}
	const std::string& getLastName() const noexcept {
		return m_lastName;
	}
	const std::string& getEmail() const noexcept {
		return m_email;
	}
	const std::string& getPhoneNumber() const noexcept {
		return m_phoneNumber;
	}
	
	std::string getFullName() const  {
		return m_firstName + " " + m_lastName;
	}

	
};

