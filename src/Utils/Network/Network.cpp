#include "include/Utils/Utils.h"


namespace SE
{

void SendPropertyTree(boost::asio::io_service& ioService, boost::asio::ip::tcp::socket& socket, boost::property_tree::ptree pTree)
{
	//Xperimental -- need to optimize this

	std::stringstream o_stream;

	boost::property_tree::write_xml(o_stream, pTree);

	std::string data = o_stream.str();

	int len = data.size();

	boost::shared_ptr<TSignalSender> signalSender(new TSignalSender(socket, len, data));

	ioService.post(boost::bind(&TSignalSender::Send, signalSender));
}

TDataReader::TDataReader(boost::asio::ip::tcp::socket& socket)
	: Socket(socket)
{
}

void TDataReader::StartRead()
{
	boost::asio::async_read(Socket, boost::asio::buffer(&DataSize, 4), boost::bind(&TDataReader::HandleReadDataSize, shared_from_this(), boost::asio::placeholders::error));
}

void TDataReader::HandleReadDataSize(const boost::system::error_code& error)
{
	if (error)
	{
		ErrorSignal();
		return;
	}


	if (DataSize > 65536 || DataSize <= 0)
	{
		// len>65536 is something unbelievable. Prevent this just in case;
		ErrorSignal();
		return;
	}

	Data.resize(DataSize);

	boost::asio::async_read(Socket, boost::asio::buffer(Data), boost::bind(&TDataReader::HandleReadData, shared_from_this(), boost::asio::placeholders::error));

}

void TDataReader::HandleReadData(const boost::system::error_code& error)
{
	if (error)
	{
		ErrorSignal();
		return;
	}

	try
	{
		//Xperimental - Might be optimized a lot:

		std::string xmlCode = std::string(&Data[0], &Data[0] + Data.size());

		std::stringstream stream(xmlCode);

		boost::property_tree::ptree propertyTree;

		boost::property_tree::read_xml(stream, propertyTree);

		DataReadSignal(propertyTree);

	}
	catch(boost::property_tree::ptree_error)
	{
		ErrorSignal();
	}
}


TSimpleAuthorization::TSimpleAuthorization(boost::asio::io_service& ioService, boost::asio::ip::tcp::socket& socket)
	: Socket(socket)
	, IoService(ioService)
{

}


void TSimpleAuthorization::Authorize()
{
	std::shared_ptr<TDataReader> dataReader(new TDataReader(Socket));

	dataReader->DataReadSignal.connect(boost::bind(&TSimpleAuthorization::HandleGetData, this, _1));
	
	dataReader->ErrorSignal.connect(ErrorSignal);

	boost::property_tree::ptree pt;

	pt.put("Hello", "");

	SendPropertyTree(IoService, Socket, pt);

	dataReader->StartRead();


}


void TSimpleAuthorization::HandleGetData(boost::property_tree::ptree pTree)
{
	if (pTree.find("OnHello") != pTree.not_found())
	{
		Login = p.get<std::string>("OnHello.Login");
		Password = p.get<std::string>("OnHello.Password");

		SaveLoginPasswordSignal(Login, Password);
		AuthorizedSignal();
		
		return;
	}

	ErrorSignal();
}

TClientSocket::TClientSocket()
	: Socket(IoService)
	, ReadDataLenLong(0)
	, Opened(false)
	, Authorization(std::shared_ptr<TSimpleAuthorization>(new TSimpleAuthorization(IoService, Socket)))
{

}

TClientSocket::~TClientSocket()
{
}

void TClientSocket::IoServiceRun()
{
	//Need try-catch here!


	//To be run in separated thread

	IoService.run();

	IoService.reset();

}


void TClientSocket::Open(const std::string address, const std::string& port)
{
	boost::asio::ip::tcp::resolver resolver(IoService);
	
	boost::asio::ip::tcp::resolver::query query(address.c_str(), port.c_str());
	
    boost::system::error_code ec;
    
	boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query, ec);
    
    boost::asio::ip::tcp::resolver::iterator end;
    
    if (ec)
    {
        OnAddressNotResolvedSignal();
        return;
    }
    
	boost::asio::async_connect(Socket, iterator, boost::bind(&TClientSocket::HandleConnect, shared_from_this(), boost::asio::placeholders::error));
    
    ConnectionTimeoutTimer = boost::shared_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer(IoService, boost::posix_time::seconds(CONST_CONNECTION_TIMEOUT_SECONDS)));
    
    ConnectionTimeoutTimer->async_wait(boost::bind(&TClientSocket::HandleConnectTimeout, this, boost::asio::placeholders::error));

	IoServiceThread = boost::thread(boost::bind(&TClientSocket::IoServiceRun, this));
}

void TClientSocket::Close()
{
	if (Opened)
	{

		Opened = false;

		ConnectionTimeoutTimer->cancel();

		Socket.close();

		OnDisconnectedSignal();
	}
}


void TClientSocket::HandleConnectTimeout(const boost::system::error_code& error)
{
	if (!error)
	{
    
		Socket.cancel();
   
		IoService.stop();
	}
}

void TClientSocket::HandleConnect(const boost::system::error_code& error)
{
	if (error)
	{
		Socket.close();
		return;
	}

	Opened = true;

	OnConnectedSignal();

	std::shared_ptr<TSimpleAuthorization> authorization = boost::get<std::shared_ptr<TSimpleAuthorization>>(Authorization);

	authorization->AuthorizedSignal.connect(boost::bind(&TClientSocket::HandleAuthorized, this));
	authorization->ErrorSignal.connect(boost::bind(&TClientSocket::HandleAuthorizationError, this));

	authorization->Authorize();
	
}

void TClientSocket::HandleAuthorized()
{
	OnAutorizedSignal();
}

void TClientSocket::HandleAuthorizationError()
{

}

} //namspace SE