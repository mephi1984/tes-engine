#include "include/Utils/Utils.h"
#include "boost/random.hpp"


namespace SE
{
	std::vector<std::string> TSimpleAutorizator::UserPasswords;
	
	boost::mutex TSimpleAutorizator::UserPasswordsMutex;


	std::string TSimpleAutorizator::GenerateRandomPassword()
	{
		boost::random::mt19937 RandomGenerator;

		RandomGenerator.seed(static_cast<unsigned int>(std::time(0)));

		static const int passwordLength = 18;
		static const std::string symbols = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!()-=_+[]{};:,.?";

		std::string result;

		boost::random::uniform_int_distribution<> symbolRandom(0, symbols.size()-1);

		for (size_t i = 0; i < passwordLength; i++)
		{
			result += symbols[symbolRandom(RandomGenerator)];
		}

		return result;

	}


	TSimpleAutorizator::TSimpleAutorizator(boost::asio::io_service& ioService, boost::asio::ip::tcp::socket& socket)
		: IoService(ioService)
		, Socket(socket)
	{

	}


	void TSimpleAutorizator::StartListen()
	{
		boost::shared_ptr<TDataReader> dataReader(new TDataReader(Socket));

		dataReader->DataReadSignalMap.AddSlot("Hello", boost::bind(&TSimpleAutorizator::HandleGetRegisterRequest, this, _1));
		dataReader->DataReadSignalMap.AddSlot("HelloAgain", boost::bind(&TSimpleAutorizator::HandleGetLoginRequest, this, _1));
	
		//dataReader->ErrorSignal.connect(ErrorSignal);

		dataReader->StartReadOnce();

	}


	void TSimpleAutorizator::HandleGetRegisterRequest(boost::property_tree::ptree pTree)
	{

		CreateNewUser();
	}

	void TSimpleAutorizator::HandleGetLoginRequest(boost::property_tree::ptree pTree)
	{
		std::string login = pTree.get<std::string>("Login");
		std::string password = pTree.get<std::string>("Password");

		int loginNum = boost::lexical_cast<int>(login);

		bool loginIsValid = loginNum < UserPasswords.size();

		if (!loginIsValid)
		{
			CreateNewUser();
			return;
		}
		
		bool passwordIsValid = UserPasswords[loginNum] == password;

		if (!passwordIsValid)
		{
			CreateNewUser();
			return;
		}

		//Login and password are ok
		boost::property_tree::ptree pt;

		pt.put("OnHello.Login", login);
		pt.put("OnHello.Password", password);

		SendPropertyTree(IoService, Socket, pt);

		AllowedSignal(login);
		
	}

	void TSimpleAutorizator::CreateNewUser()
	{

		std::string login = boost::lexical_cast<std::string>(UserPasswords.size());
		std::string password = GenerateRandomPassword();

		UserPasswordsMutex.lock();
		UserPasswords.push_back(password);
		UserPasswordsMutex.unlock();

		boost::property_tree::ptree pt;

		pt.put("OnHello.Login", login);
		pt.put("OnHello.Password", password);

		SendPropertyTree(IoService, Socket, pt);

		AllowedSignal(login);
	}

	//================================


	TConnectedUser::TConnectedUser(TServerSocket& server)
		: Server(server)
		, Socket(server.IoService)
		, Autorizator(std::shared_ptr<TSimpleAutorizator>(new TSimpleAutorizator(server.IoService, Socket)))
	{
	}

	TConnectedUser::~TConnectedUser()
	{
		
	}

	void TConnectedUser::HandleAllowed(std::string login)
	{
		UserDataReader = boost::shared_ptr<TDataReader>(new TDataReader(Socket));

		UserDataReader->ErrorSignal.connect(boost::bind(&TServerSocket::DeleteUser, &Server, shared_from_this()));
		
		Login = login;

		Server.OnUserAuthorizedSignal(shared_from_this());
		
		UserDataReader->StartReadNonstop();
	}

	void TConnectedUser::SendPropertyTree(boost::property_tree::ptree pTree)
	{
		SE::SendPropertyTree(Server.IoService, Socket, pTree);
	}

	void TConnectedUser::DisconnectSlots()
	{
		UserDataReader->DataReadSignalMap.Clear();
		UserDataReader->ErrorSignal.disconnect_all_slots();

		std::shared_ptr<TSimpleAutorizator> authorizator = boost::get<std::shared_ptr<TSimpleAutorizator>>(Autorizator);

		authorizator->ErrorSignal.disconnect_all_slots();
		authorizator->AllowedSignal.disconnect_all_slots();
		//authorizator->DeniedSignal.disconnect_all_slots();

	}




	TServerSocket::TServerSocket()
	{
	}

	TServerSocket::~TServerSocket()
	{
	}

	void TServerSocket::Open(int port)
	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

		Acceptor = std::shared_ptr<boost::asio::ip::tcp::acceptor>(new boost::asio::ip::tcp::acceptor(IoService, endpoint));

		StartAccept();

		ServiceThread = boost::thread(boost::bind(&TServerSocket::UpdateInThread, this));

	}

	void TServerSocket::Close()
	{
	}

	void TServerSocket::UpdateInThread()
	{
		IoService.run();
		IoService.reset();
	}

	void TServerSocket::JoinServiceThread()
	{
		ServiceThread.join();
	}
	
	void TServerSocket::StartAccept()
	{
		boost::shared_ptr<TConnectedUser> newUser(new TConnectedUser(*this));
	
		Acceptor->async_accept(newUser->Socket, boost::bind(&TServerSocket::HandleAccept, this, newUser, boost::asio::placeholders::error));
	}


	void TServerSocket::HandleAccept(boost::shared_ptr<TConnectedUser> user, const boost::system::error_code& error)
	{
		if (!error)
		{
		
			UserArr.push_back(user);
			
			std::shared_ptr<TSimpleAutorizator> authorizator = boost::get<std::shared_ptr<TSimpleAutorizator>>(user->Autorizator);
			
			authorizator->AllowedSignal.connect(boost::bind(&TConnectedUser::HandleAllowed, user, _1));
			authorizator->ErrorSignal.connect(boost::bind(&TServerSocket::DeleteUser, this, user));

			boost::get<std::shared_ptr<TSimpleAutorizator>>(user->Autorizator)->StartListen();

		}

		StartAccept();
	}

	void TServerSocket::DeleteUser(boost::shared_ptr<TConnectedUser> user)
	{
		std::vector<boost::shared_ptr<TConnectedUser>>::iterator i = std::find(UserArr.begin(), UserArr.end(), user);

		user->DisconnectSlots();

		OnUserDisconnectedSignal(user);

		UserArr.erase(i);
	}


} //namspace SE