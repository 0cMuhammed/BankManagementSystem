#pragma once
#include "Validator.h"
#include "Service.h"
#include "Authenticator.h"

// a normal user cannot see all admins data they can only see the email + phone(Number censord) and full name 
// only admins can give full permissions and admins data are heavily censored when they are printed 
// a user cannot manage users except if they have that permission or they are an admin and specific permissions are allowed not all of them 
//hmm u need to add a pointer parameter to whoever user is currently signed in every CRUD operation so we can keep track of permissions

class Authorizer
{

private :
	constexpr static const char* ADMIN_USERNAME = "ADMIN";
	constexpr static const char* ADMIN_HASH = "$2a$12$TgNn02Yblr6rSv1Cn9jEeOGT1NfBd7WV19pSHrDWfDbV9pPil5lka";

public : 

	enum class Permissions : int32_t
	{
		ShowList = 1 << 0, AddClient = 1 << 1, DeleteClient = 1 << 2, UpdateClient = 1 << 3, FindClient = 1 << 4, Transactions = 1 << 5, ManageUsers = 1 << 6, AllPermissions = -1
	};
	enum class ManageUsersPermissions : int32_t
	{
		ShowList = 1 << 7, AddUser = 1 << 8, DeleteUser = 1 << 9, UpdateUser = 1 << 10, FindUser = 1 << 11
	};

private:

	UserRepository& m_Ref;

	static bool _HasUsersAccess(const User& user, ManageUsersPermissions requiredPerm) {
		return user.GetPermissions() == static_cast<int32_t>(Permissions::AllPermissions) ? true : (user.GetPermissions() & static_cast<int32_t>(requiredPerm));

	}
	static void _Message(const std::string& msg) {
		std::cout << '\n' + msg;
	}
	static bool _IsAdmin(const User& user) {
	   return	user.GetUsername() == ADMIN_USERNAME && Hasher::VerifyUser(user.GetPassword(), std::string(ADMIN_HASH));
	}

	static void _SetPermission(int32_t &PermissionBits, Permission permission) {
		PermissionBits |= static_cast<int32_t>(permission);
	}
	static void _SetPermission(int32_t &PermissionBits, int32_t Number) {
		PermissionBits = Number;
	}
	static int32_t _SetManagingPermissions(const User & CurrentUser) 
	{
		int32_t Permissions = 0;
		
		std::cout << "\nDo you want to give access to : \n";

		if (_HasUsersAccess(CurrentUser, ManageUsersPermissions::ShowList) && Validator::GetConfirmation("\nShow List ? y/n : "))
		{
			Permissions |= static_cast<int32_t>(ManageUsersPermissions::ShowList);
		
			std::cout << "\n";
		}
		if (_HasUsersAccess(CurrentUser, ManageUsersPermissions::AddUser) &&Validator::GetConfirmation("\nAdd User? y/n : "))
		{
			Permissions |= static_cast<int32_t>(ManageUsersPermissions::AddUser);
			
			std::cout << "\n";
		}
		if (_HasUsersAccess(CurrentUser, ManageUsersPermissions::DeleteUser) && Validator::GetConfirmation("\nDelete User ? y/n : "))
		{
			Permissions |= static_cast<int32_t>(ManageUsersPermissions::DeleteUser);
		
			std::cout << "\n";
		}
		if (_HasUsersAccess(CurrentUser, ManageUsersPermissions::UpdateUser) && Validator::GetConfirmation("\nUpdate User ? y/n : "))
		{
			Permissions |= static_cast<int32_t>(ManageUsersPermissions::UpdateUser);
			
			std::cout << "\n";
		}
		if (_HasUsersAccess(CurrentUser, ManageUsersPermissions::FindUser) && Validator::GetConfirmation("\nFind User ? y/n : "))
		{
			Permissions |= static_cast<int32_t>(ManageUsersPermissions::FindUser);
			
			std::cout << "\n";
		}
		

		return Permissions;
	}

	static int32_t _SetUserPermissions(const User& CurrentUser) {

		int32_t Permissions = 0;

		std::cout << "\nDo you want to give access to : \n";

		if (Validator::GetConfirmation("\nShow client list ? y/n : "))
		{
			Permissions |= static_cast<int32_t>(Permission::ShowList);

			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nAdd Client? y/n : "))
		{
			Permissions |= static_cast<int32_t>(Permission::AddClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nDelete Client ? y/n : "))
		{
			Permissions |= static_cast<int32_t>(Permission::DeleteClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nUpdate Client ? y/n : "))
		{
			Permissions |= static_cast<int32_t>(Permission::UpdateClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nFind Client ? y/n : "))
		{
			Permissions |= static_cast<int32_t>(Permission::FindClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nTransactions Client ? y/n : "))
		{
			Permissions |= static_cast<int32_t>(Permission::Transactions);
			std::cout << "\n";
		}
		if ( ( _IsAdmin(CurrentUser) || HasAccess(CurrentUser,Permission::ManageUsers) ) && Validator::GetConfirmation("\nManage Users ? y/n : "))
		{
			Permissions |= _SetManagingPermissions(CurrentUser);
			std::cout << "\n";
		}

		return Permissions;
	}

	

public:

	Authorizer(Service& Service_Ref) : m_Ref(Service_Ref.AccessUserServices().AccessRepository()) {};

	static bool HasAccess(const User& user, Permissions requiredPerm) {
		return user.GetPermissions() == static_cast<int32_t>(Permissions::AllPermissions) ? true : (user.GetPermissions() & static_cast<int32_t>(requiredPerm));

	}

	static int32_t ReadPermissions(const User& user) {

		int32_t Permissions = 0;

		Permissions = ( _IsAdmin(user) && Validator::GetConfirmation("\nDo you want to give full access?") ) ? static_cast<int32_t>(Permission::AllPermissions) : _SetUserPermissions(user);

		return Permissions;

	}

};

