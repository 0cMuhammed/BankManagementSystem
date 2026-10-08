#pragma once
#include<vector>
#include<string>


class Client;
class User;


class FileHandler {

private :

	static  bool _isNotToBeSaved(const Client& client) noexcept;
	static  bool _isNotToBeSaved(const User& user) noexcept;

public :

	static std::vector<Client> LoadClients();
	static std::vector<User> LoadUsers();
	static std::vector<std::string> LoadLogs();
	static std::vector<std::string> LoadAuditLogs();

	static void SaveLog(const std::string &message);
	static void SaveLogTransfer(const std::string& message);
	static void SaveClients(const Client& client);
	static void SaveClients(const std::vector<Client> &Clients);

	static void SaveUsers(const User& user);
	static void SaveUsers(const std::vector<User> &Users);

};
