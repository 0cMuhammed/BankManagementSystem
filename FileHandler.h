#pragma once
#include<vector>


class Client;
class User;


class FileHandler {

private :
	static  bool _isNotToBeSaved(const Client& client) noexcept;
	static  bool _isNotToBeSaved(const User& user) noexcept;

	static constexpr const char * CLIENTS_FILE = "Clients.txt";
	static constexpr const char * USERS_FILE = "Users.txt";
	static constexpr const char*  DELIMITER = "#//#";

public :

	static std::vector<Client> LoadClients();
	static std::vector<User> LoadUsers();
	static void SaveClients(const Client& client);
	static void SaveClients(const std::vector<Client> &Clients);

	static void SaveUsers(const User& user);
	static void SaveUsers(const std::vector<User> &Users);

};
