#include "User.h"
#include "FileHandler.h"
 void User::_SaveExistingObject() {
	std::vector<User> Users = FileHandler::LoadUsers();

	for (User& user : Users)
	{
		if (user.GetUsername() == this->m_username)
		{
			user = *this;
			break;

		}

	}

	FileHandler::SaveUsers(Users);
}
void User::_SaveNewObject() {
	this->m_mode = ObjectMode::ExistingMode;
	FileHandler::SaveUsers(*this);
}