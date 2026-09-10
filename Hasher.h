#pragma once
#include <string>
#include "bcrypt/bcrypt.hpp"



class Hasher
{

private :

	constexpr static size_t SALT_FACTOR = 12;

public :

	static std::string GetHash(const std::string& Password)
	{
		return BCrypt::generateHash(Password, SALT_FACTOR);

	}
	static bool VerifyUser(const std::string& password, const std::string& HashedPassword) {
		return BCrypt::validatePassword(password, HashedPassword);
	}

};

