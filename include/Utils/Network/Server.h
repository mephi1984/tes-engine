#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <string>
#include <map>
#include <vector>
#include "boost/shared_array.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/foreach.hpp"

#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/signal.hpp"
#include "boost/variant.hpp"

#include "include/Utils/Network/SignalSender.h"


namespace SE
{

class TAutorizatorInterface
{
protected:
public:
	virtual void StartListen() = 0;

	virtual ~TAutorizatorInterface() { }

};

class TSimpleAutorizator : public TAutorizatorInterface
{
protected:

	
	static std::vector<std::string> UserPasswords;
	static boost::mutex UserPasswordsMutex;

public:

	boost::asio::io_service& IoService;

	boost::asio::ip::tcp::socket& Socket;

	TSimpleAutorizator(boost::asio::io_service& ioService, boost::asio::ip::tcp::socket& socket);

	virtual void StartListen();

	void HandleGetData(boost::property_tree::ptree pTree);
	
	boost::signal<void()> ErrorSignal;
	boost::signal<void(std::string)> AllowedSignal;
	boost::signal<void()> DeniedSignal;
};

typedef boost::variant<std::shared_ptr<TSimpleAutorizator>> TAuthorizatorVariant;

class TServerSocket;

class TConnectedUser : public boost::enable_shared_from_this<TConnectedUser>
{
protected:

	TServerSocket& Server;
	std::string Login; //Need to generalize this

public:
	boost::asio::ip::tcp::socket Socket;

	TAuthorizatorVariant Autorizator;

	boost::shared_ptr<TDataReader> UserDataReader;

	TConnectedUser(TServerSocket& server);

	~TConnectedUser();

	std::string GetLogin() { return Login; } 

	void HandleAllowed(std::string login); //Need to generalize this

	void SendPropertyTree(boost::property_tree::ptree pTree);

	void DisconnectSlots();

};


class TServerSocket
{
protected:
	std::vector<boost::shared_ptr<TConnectedUser>> UserArr;

	boost::thread ServiceThread;

	std::shared_ptr<boost::asio::ip::tcp::acceptor> Acceptor;


public:

	boost::asio::io_service IoService;


	TServerSocket();
	~TServerSocket();

	void Open(int port);
	void Close();

	void UpdateInThread();
	void JoinServiceThread();
	
	void StartAccept();
	void HandleAccept(boost::shared_ptr<TConnectedUser> user, const boost::system::error_code& error);
	void DeleteUser(boost::shared_ptr<TConnectedUser> user);

	//Need to generalize this
	boost::signal<void(boost::shared_ptr<TConnectedUser>)> OnUserAuthorizedSignal;
	boost::signal<void(boost::shared_ptr<TConnectedUser>)> OnUserDisconnectedSignal;
	
};




} //namespace SE


#endif