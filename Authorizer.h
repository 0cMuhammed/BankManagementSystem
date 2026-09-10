#pragma once
#include "Validator.h"
#include "Hasher.h"
#include "User.h"
#include "Service.h"  


//a normal user cannot see all admins data they can only see the email + phone(Number censord) and full name 
//only admins can give full permissions and admins data are heavily censored when they are printed 
//a user cannot manage users except if they have that permission or they are an admin and specific permissions are allowed not all of them 
//hmm u need to add a pointer parameter to whoever user is currently signed in every CRUD operation so we can keep track of permissions



class Authorizer
{

private :

	constexpr static const char* ADMIN_USERNAME = "Admin";
	constexpr static const char* ADMIN_HASH = "$2a$12$nDz6obAV4aYZXJa0pismTeSG3uBvPhQ3gXOlnxYzFexR8hrMAehG6"; // for simplicity..

public : 

	enum class Permissions : int32_t
	{
		None = 0,
		ShowClientList = 1 << 0,
		AddClient = 1 << 1,
		DeleteClient = 1 << 2,
		UpdateClient = 1 << 3,
		FindClient = 1 << 4,
		Transactions = 1 << 5,
		ManageUsers = 1 << 6,

		ShowUserList = 1 << 7,
		AddUser = 1 << 8,
		DeleteUser = 1 << 9,
		UpdateUser = 1 << 10,
		FindUser = 1 << 11,

		AllPermissions = -1
	};
private:

	UserRepository& m_Ref;

	static bool _HasUsersAccess(const User& user, Permissions requiredPerm) {
		return user.GetPermissions() == static_cast<int32_t>(Permissions::AllPermissions) ? true : (user.GetPermissions() & static_cast<int32_t>(requiredPerm));

	}

	static void _Message(const std::string& msg) {
		std::cout << '\n' + msg;
	}

	static bool _IsAdmin(const User& user) {
	   return	user.GetUsername() == ADMIN_USERNAME && Hasher::VerifyUser(user.GetPassword(), std::string(ADMIN_HASH));
	}

	static void _SetPermission(int32_t &PermissionBits, Permissions permission) {
		PermissionBits |= static_cast<int32_t>(permission);
	}

	static void _SetPermission(int32_t &PermissionBits, int32_t Number) {
		PermissionBits = Number;
	}

	static int32_t _SetManagingPermissions(const User & CurrentUser) 
	{
		int32_t bits = static_cast<int32_t> (Permissions::None);
		
		std::cout << "\nDo you want to give access to : \n";

		if (_HasUsersAccess(CurrentUser, Permissions::ShowUserList) && Validator::GetConfirmation("\nShow List ? y/n : "))
		{
			bits |= static_cast<int32_t>(Permissions::ShowUserList);
		
			std::cout << "\n";
		}
		if (_HasUsersAccess(CurrentUser, Permissions::AddUser) &&Validator::GetConfirmation("\nAdd User? y/n : "))
		{
			bits |= static_cast<int32_t>(Permissions::AddUser);
			
			std::cout << "\n";
		}
		if (_HasUsersAccess(CurrentUser, Permissions::DeleteUser) && Validator::GetConfirmation("\nDelete User ? y/n : "))
		{
			bits |= static_cast<int32_t>(Permissions::DeleteUser);
		
			std::cout << "\n";
		}
		if (_HasUsersAccess(CurrentUser, Permissions::UpdateUser) && Validator::GetConfirmation("\nUpdate User ? y/n : "))
		{
			bits |= static_cast<int32_t>( Permissions::UpdateUser);
			
			std::cout << "\n";
		}
		if (_HasUsersAccess(CurrentUser, Permissions::FindUser) && Validator::GetConfirmation("\nFind User ? y/n : "))
		{
			bits |= static_cast<int32_t>(Permissions::FindUser);
			
			std::cout << "\n";
		}
		

		return bits;
	}

	static int32_t _SetUserPermissions(const User& CurrentUser) {

		int32_t bits = static_cast<int32_t> (Permissions::None);

		std::cout << "\nDo you want to give access to : \n";

		if (Validator::GetConfirmation("\nShow client list ? y/n : "))
		{
			bits |= static_cast<int32_t>(Permissions::ShowClientList);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nAdd Client? y/n : "))
		{
			bits |= static_cast<int32_t>( Permissions::AddClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nDelete Client ? y/n : "))
		{
			bits |= static_cast<int32_t>( Permissions::DeleteClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nUpdate Client ? y/n : "))
		{
			bits |= static_cast<int32_t>(Permissions::UpdateClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nFind Client ? y/n : "))
		{
			bits |= static_cast<int32_t>(Permissions::FindClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nTransactions Client ? y/n : "))
		{
			bits |= static_cast<int32_t>(Permissions::Transactions);
			std::cout << "\n";
		}
		if ((_IsAdmin(CurrentUser) || HasAccess(CurrentUser, Permissions::ManageUsers)) && Validator::GetConfirmation("\nManage Users ? y/n : "))
		{
			bits |= _SetManagingPermissions(CurrentUser);
			std::cout << "\n";
		}

		return bits;

	}


public:

	Authorizer(Service& Service_Ref) : m_Ref(Service_Ref.AccessUserServices().AccessRepository()) {};

	static bool HasAccess(const User& user, Permissions requiredPerm) {
		return user.GetPermissions() == static_cast<int32_t>(Permissions::AllPermissions) ? true : (user.GetPermissions() & static_cast<int32_t>(requiredPerm));

	}

	static int32_t ReadPermissions(const User& CurrentUser) {

		int32_t bits = static_cast<int32_t>(Permissions::None);

		bits = (_IsAdmin(CurrentUser) && Validator::GetConfirmation("\nDo you want to give full access?")) ? static_cast<int32_t>( Permissions::AllPermissions) : _SetUserPermissions(CurrentUser);

		return bits;

	}




};
