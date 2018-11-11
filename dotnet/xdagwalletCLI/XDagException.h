#pragma once

using namespace System;

namespace XDagNetWalletCLI {

	public enum class XDagErrorCode : int
	{
		PASSWORD_INCORRECT,
		PASSWORD_EMPTY,
		INSUFFICIENT_AMOUNT,
		ADDRESS_FORMAT_ERROR,
	};

	public ref class XDagException : public Exception {
	public:
		XDagException(XDagErrorCode code, String^ message) : Exception(message)
		{
			this->ErrorCode = code;
		}
	protected:
		XDagErrorCode ErrorCode;
	};

	public ref class XDagUserException : public XDagException {
	public:
		XDagUserException(XDagErrorCode code, String^ message) : XDagException(code, message)
		{

		}
	};

	public ref class PasswordIncorrectException : public XDagUserException {
	public:
		PasswordIncorrectException() : XDagUserException(XDagErrorCode::PASSWORD_INCORRECT, "Password is Incorrect.")
		{

		}
	};

	public ref class WalletAddressFormatException : public XDagUserException {
	public:
		WalletAddressFormatException() : XDagUserException(XDagErrorCode::ADDRESS_FORMAT_ERROR, "Wallet address format is incorrect.")
		{

		}
	};

	public ref class InsufficientAmountException : public XDagUserException {
	public:
		InsufficientAmountException() : XDagUserException(XDagErrorCode::INSUFFICIENT_AMOUNT, "Insufficient amount.")
		{

		}
	};
}