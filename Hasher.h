#pragma once
#include <string>
#include "bcrypt/BCrypt.hpp"
#define SALT_FACTOR 12

class Hasher
{

public :

	static std::string GetHash(const std::string& Password)
	{
		return BCrypt::generateHash(Password, SALT_FACTOR);

	}
	static bool VerifyUser(const std::string& password, const std::string& HashedPassword) {
		return BCrypt::validatePassword(password, HashedPassword);
	}

};

