#pragma once

constexpr static const char* ADMIN_USERNAME = "Admin";
constexpr static int32_t  FullPermissions = -1;
constexpr static size_t FullPermisssionsInPositive = 8191;
constexpr static uint8_t FullPermssionsCount = 13;
constexpr static const char* ADMIN_HASH = "$2a$12$a2xKBx8rpmBVyebnDaxyh.ydpOGVnS.qqPHzz9gI7UGE5dEeUowmC"; // for simplicity..
static constexpr const char* CLIENTS_FILE = "ClientsData.txt";
static constexpr const char* USERS_FILE = "UsersData.txt";
static constexpr const char* LOGGER_FILE = "Logs.txt";
static constexpr const char* LOGGER_TRANSFERS_FILE = "AuditLogs.txt";

static constexpr const char* DELIMITER = "#//#";
static constexpr const char* LOG_DELIMITER = " - "; // FOR BOTH READABILITY AND LOADING IN VECTOR 