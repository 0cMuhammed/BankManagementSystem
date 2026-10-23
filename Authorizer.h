#pragma once
#include "Validator.h"
#include "Hasher.h"
#include "User.h"
#include "Service.h"  
#include "Global.h"



class Authorizer
{


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

		ShowLogs = 1 << 6,
		ShowActivityLogs = 1 << 7,
		ShowAuditLogs = 1 << 8,

		ManageUsers = 1 << 9,
		ShowUserList = 1 << 10,
		AddUser = 1 << 11,
		DeleteUser = 1 << 12,
		UpdateUser = 1 << 13,
		FindUser = 1 << 14,

		AllPermissions = -1
	};
private:

	UserRepository& m_Ref;


	static bool _isEligibileUser(const User &CurrentUser) {
		 return HasAccess(CurrentUser, Permissions::ShowUserList) || HasAccess(CurrentUser, Permissions::AddUser) || HasAccess(CurrentUser, Permissions::DeleteUser) || HasAccess(CurrentUser, Permissions::UpdateUser)|| HasAccess(CurrentUser, Permissions::FindUser);
	}
	static bool _isEligibileClientPerm(const User& CurrentUser) {
		return HasAccess(CurrentUser, Permissions::ShowClientList) || HasAccess(CurrentUser, Permissions::AddClient) || HasAccess(CurrentUser, Permissions::DeleteClient) || HasAccess(CurrentUser, Permissions::UpdateClient) || HasAccess(CurrentUser, Permissions::FindClient) || HasAccess(CurrentUser, Permissions::Transactions);
	}
	static bool _isEligibileLogs(const User& CurrentUser) {
		return HasAccess(CurrentUser, Permissions::ShowActivityLogs) || HasAccess(CurrentUser, Permissions::ShowAuditLogs);
	}



	static void _SetPermission(int32_t &PermissionBits, Permissions permission) {

		if (PermissionBits == static_cast<int32_t>(Permissions::AllPermissions))
			return;


		PermissionBits |= static_cast<int32_t>(permission);
	}

	static void _SetPermission(int32_t &PermissionBits, int32_t Number) {

		if (PermissionBits == static_cast<int32_t>(Permissions::AllPermissions))
			return;

		PermissionBits = Number;
	}

	static int32_t _SetUserPermissions(const User & CurrentUser) 
	{
		if (!_isEligibileUser(CurrentUser)) 
		{
			term::Warn("You have no permissions available to grant.");
			return static_cast<int32_t>(Permissions::None);
		}

		int32_t bits = static_cast<int32_t> (Permissions::None);
		int8_t count = 0;
		
		do {
			term::Blank();
			term::Info("Give access to:");

			if (HasAccess(CurrentUser, Permissions::ShowUserList) && term::Confirm("Show List?"))
			{
				bits |= static_cast<int32_t>(Permissions::ShowUserList);
				
				count++;
			}
			if (HasAccess(CurrentUser, Permissions::AddUser) && term::Confirm("Add User?"))
			{
				bits |= static_cast<int32_t>(Permissions::AddUser);
				
				count++;
			}
			if (HasAccess(CurrentUser, Permissions::DeleteUser) && term::Confirm("Delete User?"))
			{
				bits |= static_cast<int32_t>(Permissions::DeleteUser);
				
			    count++;
			}
			if (HasAccess(CurrentUser, Permissions::UpdateUser) && term::Confirm("Update User?"))
			{
				bits |= static_cast<int32_t>(Permissions::UpdateUser);
				
				count++;
			}
		
			if (HasAccess(CurrentUser, Permissions::FindUser) && term::Confirm("Find User?"))
			{
				bits |= static_cast<int32_t>(Permissions::FindUser);
				
				count++;
			}
		

			if (count < 1)
			{
				term::Error("At least ONE permission is needed, try again.");
				
				bits = 0;
			}

		} while (count < 1);

		return bits;
	}

	static int32_t _SetClientPermissions(const User &CurrentUser) {


		if (!_isEligibileClientPerm(CurrentUser))
		{
			term::Warn("You have no permissions available to grant.");
			return static_cast<int32_t>(Permissions::None);
		}


		
		int32_t bits = static_cast<int32_t>(Permissions::None);
		int8_t count = 0;
		

		do {

			term::Blank();
			term::Info("Give access to:");

			if (HasAccess(CurrentUser, Permissions::ShowClientList) && term::Confirm("Show client list?"))
			{
				bits |= static_cast<int32_t>(Permissions::ShowClientList);
				count++;
			}
			if (HasAccess(CurrentUser, Permissions::AddClient) && term::Confirm("Add Client?"))
			{
				bits |= static_cast<int32_t>(Permissions::AddClient);
				count++;
			}
			if (HasAccess(CurrentUser, Permissions::DeleteClient) && term::Confirm("Delete Client?"))
			{
				bits |= static_cast<int32_t>(Permissions::DeleteClient);
				count++;
			}
			if (HasAccess(CurrentUser, Permissions::UpdateClient) && term::Confirm("Update Client?"))
			{
				bits |= static_cast<int32_t>(Permissions::UpdateClient);
				count++;
			}
			if (HasAccess(CurrentUser, Permissions::FindClient) && term::Confirm("Find Client?"))
			{
				bits |= static_cast<int32_t>(Permissions::FindClient);
				count++;
			}
			if ( HasAccess(CurrentUser, Permissions::Transactions) && term::Confirm("Transactions Client?"))
			{
				bits |= static_cast<int32_t>(Permissions::Transactions);
				count++;
			}


			if (count < 1)
			{
				term::Error("At least ONE permission is needed, try again.");
				bits = 0;
			}

		} while (count < 1);


		return bits;

	}

	static int32_t _SetLogPermissions(const User &CurrentUser) {


		if (!_isEligibileLogs(CurrentUser))
		{
			term::Warn("You have no permissions available to grant.");
			return static_cast<int32_t>(Permissions::None);
		}

		int32_t bits = static_cast<int32_t> (Permissions::None);
		uint8_t count = 0;

		do {
			term::Blank();
			term::Info("Give access to:");

			if (HasAccess(CurrentUser, Permissions::ShowActivityLogs) && term::Confirm("Show Activity Logs?"))
			{
				bits |= static_cast<int32_t>(Permissions::ShowActivityLogs);

				count++;
			}
			if (HasAccess(CurrentUser, Permissions::ShowAuditLogs) && term::Confirm("Show Audit Logs?"))
			{
				bits |= static_cast<int32_t>(Permissions::ShowAuditLogs);

				count++;
			}

			if (count < 1)
			{
				term::Error("At least ONE permission is needed, try again.");

				bits = 0;
			}

		} while (count < 1);

		return bits;


	}
	static int32_t _SetAllPermissions(const User& CurrentUser) {

		int32_t bits = static_cast<int32_t>(Permissions::None);
	
		bits = _SetClientPermissions(CurrentUser);

		if ( (IsAdmin(CurrentUser) || HasAccess(CurrentUser, Permissions::ManageUsers) ) && term::Confirm("Manage Users?"))
		{
			bits |= static_cast<int32_t>(Permissions::ManageUsers);
			

			bits |= _SetUserPermissions(CurrentUser);
		}

		if ((IsAdmin(CurrentUser) || HasAccess(CurrentUser, Permissions::ShowLogs)) && term::Confirm("Show Logs?"))
		{
			bits |= static_cast<int32_t>(Permissions::ShowLogs);


			bits |= _SetLogPermissions(CurrentUser);
		}


		
		return (bits == FullPermisssionsInPositive ? FullPermissions : bits);

	}


public:

	Authorizer(Service& Service_Ref) : m_Ref(Service_Ref.AccessUserServices().AccessRepository()) {};

	static bool HasAccess(const User& user, Permissions requiredPerm) {
		return  IsAdmin(user) || user.GetPermissions() == static_cast<int32_t>(Permissions::AllPermissions) ? true : (user.GetPermissions() & static_cast<int32_t>(requiredPerm));

	}
	static bool IsAdmin(const User& user) {

		return	user.GetUsername() == ADMIN_USERNAME && user.GetPassword() == std::string(ADMIN_HASH);
	}

	static int32_t ReadPermissions(const User& CurrentUser) {

		int32_t bits = static_cast<int32_t>(Permissions::None);

		bits = (IsAdmin(CurrentUser) && term::Confirm("Do you want to give full access?")) ? static_cast<int32_t>(Permissions::AllPermissions) : _SetAllPermissions(CurrentUser);

		return bits;

	}




};
