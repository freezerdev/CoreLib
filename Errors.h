#pragma once

NS_BEGIN

constexpr auto FW_NO_ERROR = 0x0000;

constexpr auto FW_ERROR_ABORTED = 0xC000;
constexpr auto FW_ERROR_ACCESS_DENIED = 0xC001;
constexpr auto FW_ERROR_BUSY = 0xC002;
constexpr auto FW_ERROR_ENCRYPTION_FAILED = 0xC003;
constexpr auto FW_ERROR_EXISTS = 0xC004;
constexpr auto FW_ERROR_HANDLE_EOF = 0xC005;
constexpr auto FW_ERROR_INSUFFICIENT_BUFFER = 0xC006;
constexpr auto FW_ERROR_INTERRUPTED = 0xC007;
constexpr auto FW_ERROR_INVALID_HANDLE = 0xC008;
constexpr auto FW_ERROR_INVALID_PARAMETER = 0xC009;
constexpr auto FW_ERROR_INVALID_DATA = 0xC00A;
constexpr auto FW_ERROR_IO_PENDING = 0xC00B;
constexpr auto FW_ERROR_MORE_DATA = 0xC00C;
constexpr auto FW_ERROR_NO_MORE_DATA = 0xC00D;
constexpr auto FW_ERROR_NOT_ENOUGH_MEMORY = 0xC00E;
constexpr auto FW_ERROR_NOT_FOUND = 0xC00F;
constexpr auto FW_ERROR_QUOTA = 0xC010;
constexpr auto FW_ERROR_CRC = 0xC011;
constexpr auto FW_ERROR_NOT_SUPPORTED = 0xC012;
constexpr auto FW_ERROR_HASH = 0xC013;
constexpr auto FW_ERROR_TOO_LARGE = 0xC014;
constexpr auto FW_ERROR_LOW_LEVEL = 0xC015;
constexpr auto FW_ERROR_IS_DIR = 0xC016;
constexpr auto FW_ERROR_IS_NOT_DIR = 0xC017;
constexpr auto FW_ERROR_LINK = 0xC018;
constexpr auto FW_ERROR_HANDLES = 0xC019;
constexpr auto FW_ERROR_EMPTY = 0xC01A;
constexpr auto FW_ERROR_SIGN_FAIL = 0xC01B;
constexpr auto FW_ERROR_NOT_CONNECTED = 0xC01C;
constexpr auto FW_ERROR_SHUTTING_DOWN = 0xC01D;
constexpr auto FW_ERROR_ALREADY_CONNECTED = 0xC01E;
constexpr auto FW_ERROR_AUTHENTICATION_FAILED = 0xC01F;
constexpr auto FW_ERROR_NOT_IMPLEMENTED = 0xC020;
constexpr auto FW_ERROR_NETWORK_ISSUE = 0xC021;
constexpr auto FW_ERROR_SERVER_WEBCALL_FAILED = 0xC022;
constexpr auto FW_ERROR_SERVER_AUTH_FAILED = 0xC023;
constexpr auto FW_ERROR_SERVER_EXCEPTION = 0xC024;
constexpr auto FW_ERROR_DISABLED = 0xC025;
constexpr auto FW_ERROR_NOT_EMPTY = 0xC026;
constexpr auto FW_ERROR_SOME_FAILED = 0xC027;

constexpr auto FW_ERROR_UNKNOWN = 0xCFFF;

ERRCODE ConvertFromNativeErrorCode(const ERRCODE nNativeErrorCode) noexcept;

NS_END
