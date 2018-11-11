// xdagnetwalletCLI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <msclr\marshal_cppstd.h>

#include "XDagRuntime.h"
#include "XDagException.h"

#pragma unmanaged

#include "xdag_runtime.h"


#pragma managed


using namespace System;
using namespace System::Runtime::InteropServices;
using namespace XDagNetWalletCLI;


XDagRuntime::XDagRuntime(IXDagWallet^ wallet)
{
	if (wallet == nullptr)
	{
		throw gcnew System::ArgumentNullException();
	}

	this->xdagWallet = wallet;

	// Set Logger Callback
	InputPasswordDelegate^ func = gcnew InputPasswordDelegate(this, &XDagRuntime::InputPassword);
	gch = GCHandle::Alloc(func);
	IntPtr funcPtr = Marshal::GetFunctionPointerForDelegate(func);
	InputPasswordStd necb = static_cast<InputPasswordStd>(funcPtr.ToPointer());
	xdag_set_password_callback_wrap(necb);

	ShowStateDelegate^ func2 = gcnew ShowStateDelegate(this, &XDagRuntime::ShowState);
	gch = GCHandle::Alloc(func2);
	IntPtr funcPtr2 = Marshal::GetFunctionPointerForDelegate(func2);
	g_xdag_show_state = static_cast<ShowStateStd>(funcPtr2.ToPointer());

}

XDagRuntime::~XDagRuntime()
{

}

void XDagRuntime::Start()
{
	std::string exeFile = "xdag.exe";
	// std::string poolAddress = "feipool.xyz:13654";


	char *argv[] = { (char*)exeFile.c_str() };

	int result = xdag_init_wrap(1, argv, 1);

	if (result == 0)
	{
		return;
	}

	switch (result)
	{
	case -1:
		throw gcnew ArgumentNullException("Password is Empty");
		break;
	case -2:
	case -3:
		throw gcnew PasswordIncorrectException();
		break;
	default:
		break;
	}
}

bool XDagRuntime::HasExistingAccount()
{
	return xdag_dnet_crpt_found();
}

bool XDagRuntime::ValidateWalletAddress(String^ address)
{
	std::string addressStd = msclr::interop::marshal_as<std::string>(address);

	return xdag_is_valid_wallet_address(addressStd.c_str());
}

void XDagRuntime::TransferToAddress(String^ toAddress, double amount)
{
	std::string addressStd = msclr::interop::marshal_as<std::string>(toAddress);
	std::string amountStd = msclr::interop::marshal_as<std::string>(amount.ToString());

	int result = xdag_transfer_wrap(addressStd.c_str(), amountStd.c_str());

	if (result == 0)
	{
		// Success
		return;
	}

	switch (result)
	{
	case -3:
	case -4:
		throw gcnew InsufficientAmountException();
	case -5:
		throw gcnew WalletAddressFormatException();
	case -6:
		throw gcnew PasswordIncorrectException();
	default:
		throw gcnew InvalidOperationException("Failed to commit transfer. ErrorCode=[" + result.ToString() + "]");
		break;
	}

}

void XDagRuntime::DoTesting()
{
}

int XDagRuntime::InputPassword(const char *prompt, char *buf, unsigned size)
{
	if (this->xdagWallet == nullptr)
	{
		return -1;
	}

	String ^ promptString = ConvertFromConstChar(prompt);

	String ^ passwordString = this->xdagWallet->OnPromptInputPassword(promptString, (UINT)size);
	std::string passwordStd = msclr::interop::marshal_as<std::string>(passwordString);

	const char* passwordChars = passwordStd.c_str();
	if (strlen(passwordChars) == 0)
	{
		return -1;
	}

	strncpy_s(buf, size, passwordChars, size);

	return 0;
}

int XDagRuntime::ShowState(const char *state, const char *balance, const char *address)
{
	String^ stateString = ConvertFromConstChar(state);
	String^ balanceString = ConvertFromConstChar(balance);
	String^ addressString = ConvertFromConstChar(address);

	this->xdagWallet->OnUpdateState(stateString, balanceString, addressString);

	return 0;
}

String^ XDagRuntime::ConvertFromConstChar(const char* str)
{
	std::string promptStd(str);
	return msclr::interop::marshal_as<System::String^>(promptStd);
}

const char* XDagRuntime::ConvertFromString(String^ str)
{
	std::string stdString = msclr::interop::marshal_as<std::string>(str);

	int len = strlen(stdString.c_str());

	char * result = new char[len + 1];

	strcpy_s(result, sizeof(result), stdString.c_str());


	return const_cast<char *>(result);
}