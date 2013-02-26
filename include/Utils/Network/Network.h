#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

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

const int CONST_CONNECTION_TIMEOUT_SECONDS = 300;


void SendPropertyTree(boost::asio::io_service& ioService, boost::asio::ip::tcp::socket& socket, boost::property_tree::ptree pTree);


class TDataReadSignalMap
{
protected:
	std::map<std::string, std::shared_ptr<boost::signal<void(boost::property_tree::ptree)>>> SignalMap;
public:

	void AddSlot(const std::string& nodeName, boost::function<void(boost::property_tree::ptree)> f);

	bool SignalExists(const std::string& signalName);
	
	void EmitSignal(const std::string& signalName, const boost::property_tree::ptree& pt);

	void Clear();

};


//Must be stored in shared_ptr only
class TDataReader : public boost::enable_shared_from_this<TDataReader>
{
protected:
	bool Nonstop;

	void InnerStartRead();
public:

	boost::asio::ip::tcp::socket& Socket;

	int DataSize;

	std::vector<char> Data;

	TDataReadSignalMap DataReadSignalMap;

	TDataReader(boost::asio::ip::tcp::socket& socket);

	void StartReadOnce();

	void StartReadNonstop();

	void HandleReadDataSize(const boost::system::error_code& error);

	void HandleReadData(const boost::system::error_code& error);
 
	boost::signal<void()> ErrorSignal;
};



class TAuthorizationInterface
{
public:
	virtual void Authorize() = 0;

	virtual ~TAuthorizationInterface() { }
};

class TSimpleAuthorization : public TAuthorizationInterface
{
public:

	boost::asio::io_service& IoService;

	boost::asio::ip::tcp::socket& Socket;

	std::string Login;
	std::string Password;


	TSimpleAuthorization(boost::asio::io_service& ioService, boost::asio::ip::tcp::socket& socket);

	virtual void Authorize();

	void HandleGetData(boost::property_tree::ptree pTree);

	boost::signal<void()> AuthorizedSignal;
	boost::signal<void(std::string, std::string)> SaveLoginPasswordSignal;
	boost::signal<void()> ErrorSignal;
};

typedef boost::variant<std::shared_ptr<TSimpleAuthorization>> TAuthorizationVariant;


class TClientSocket : public boost::enable_shared_from_this<TClientSocket>
{
protected:

	boost::asio::io_service IoService; //IoService must be declared before Socket
    
    boost::thread IoServiceThread;

	boost::asio::ip::tcp::socket Socket;
	
	int ReadDataLenLong;

	std::vector<char> ReadData;

	boost::shared_ptr<boost::asio::deadline_timer> ConnectionTimeoutTimer;

	bool Opened;

public:
	boost::signal<void()> OnAddressNotResolvedSignal;
	boost::signal<void()> OnConnectedSignal;
	boost::signal<void()> OnAutorizedSignal;
	boost::signal<void()> OnDisconnectedSignal;
	
	boost::shared_ptr<TDataReader> ClientDataReader;

	TAuthorizationVariant Authorization;

	TClientSocket();
	~TClientSocket();

	void IoServiceRun();

	void Open(const std::string address, const std::string& port);

	void Close();

	void HandleConnectTimeout(const boost::system::error_code& error);
	void HandleConnect(const boost::system::error_code& error);

	void HandleAuthorized();
	void HandleAuthorizationError();

	void SendPropertyTree(boost::property_tree::ptree pTree);
};


} //namespace SE


#endif