#include<iostream>
#include<fstream>
#include<cctype>
#include<string>
#include<filesystem>

std::string username;
std::string receivedUsername;
std::string receivedPass;
std::string password;
std::string encryptPassword;
std::string encryptUsername;
std::string response;

int random = (rand() % 7) + 12;
bool loggedIn = false;

enum ErrorList
{
	AllOK = 0,
	InvalidPass,
	InvalidUsername,
	EmptyUsername,
	EmptyPassword,
	InvalidAccount,
	AccountExists,
	NotUsingYN
};

std::string ErrorDetailStrings[] =
{
	"Everything is going normal.",
	"Invalid password.",
	"Invalid username.",
	"A username cannot be empty.",
	"A password cannot be empty.",
	"That account does not exist.",
	"That name has been already used.",
	"Please use Y/N only. Either uppercase or lowercase."
};

std::string encrypt(std::string& str, bool decrypt)
{
	if (not decrypt)
	{
		for (int i = 0; i < str.length(); i++)
		{
			str[i] += random;
		}
	}
	else
	{
		for (int i = 0; i < str.length(); i++)
		{
			str[i] -= random;
		}
	}
	return str;
}

std::string lowercase(std::string& str)
{
	for (int i = 0; i < str.length(); i++)
	{
		str[i] = std::tolower(str[i]);
	}
	return str;
}

int login()
{
	std::cout << "What is your username?" << std::endl;
	std::getline(std::cin, receivedUsername);
	if (receivedUsername == "")
	{
		return EmptyUsername;
	}

	std::ifstream credentialsRead(receivedUsername + ".ldf");

	if (credentialsRead.fail())
	{
		return InvalidAccount;
	}
	std::cout << "Hello, " << receivedUsername << ", now enter your password." << std::endl;
	std::getline(std::cin, receivedPass);

	std::string encryptedReceivedPass;

	encryptedReceivedPass = encrypt(receivedPass, false);

	std::string readPass;

	credentialsRead >> readPass;

	if (encryptedReceivedPass != readPass)
	{
		for (int e = 0; e < 10; e++)
		{
			return InvalidPass;
		}
	}
	else
	{
		std::cout << "Welcome back, " << receivedUsername << "." << std::endl;
		std::cout << "You may press any key to exit." << std::endl;
		std::cin.get();
	}
	return AllOK;
}

int createAcc()
{
	std::fstream credentialsCheck;
	std::cout << "What would you like your username to be?" << std::endl;
	std::getline(std::cin, username);
	credentialsCheck.open(username + ".ldf");
	if (credentialsCheck)
	{
		return AccountExists;
	}
	if (username == "")
	{
		return EmptyUsername;
	}
	std::cout << "Hello, " << username << ", now enter your password." << std::endl;
	std::getline(std::cin, password);
	if (password == "")
	{
		return EmptyUsername;
	}
	std::cout << "Your account has been created. The files for it are stored in the same directory as this executable." << std::endl;
	std::cout << "You may press any key to exit." << std::endl;
	std::cin.get();
	std::ofstream credentials(username + ".ldf");

	std::string encryptedPass = encrypt(password, false);

	credentials << encryptedPass << std::endl;

	credentials.close();
	return AllOK;
}

int main()
{
	while(!loggedIn)
	{
		std::cout << "Do you have an account already? Y/N" << std::endl;
		std::cin >> response;
		std::cin.ignore();
		if (lowercase(response) == "n")
		{
			int errorCode = createAcc();
			if (errorCode == AllOK)
			{
				loggedIn = true;
			}
			else
			{
				std::cout << ErrorDetailStrings[errorCode] << " Error Code: " << errorCode << std::endl;
			}
		}
		else if (lowercase(response) == "y")
		{
			int errorCode = login();
			if (errorCode == AllOK)
			{
				loggedIn = true;
			}
			else
			{
				std::cout << ErrorDetailStrings[errorCode] << " Error Code: " << errorCode << std::endl;
			}
		}
		else
		{
			std::cout << ErrorDetailStrings[NotUsingYN] << " Error Code: " << NotUsingYN << std::endl;
		}
	}
}

