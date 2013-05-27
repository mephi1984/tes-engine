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

	signalSender->Send();
	//ioService.post(boost::bind(&TSignalSender::Send, signalSender));
}



void TDataReadSignalMap::AddSlot(const std::string& nodeName, boost::function<void(boost::property_tree::ptree)> f)
{
	if (SignalMap.count(nodeName) == 0)
	{
		SignalMap[nodeName] = std::shared_ptr<boost::signal<void(boost::property_tree::ptree)>>(new boost::signal<void(boost::property_tree::ptree)>);
		SignalMap[nodeName]->connect(f);
	}
	else
	{
		SignalMap[nodeName]->connect(f);
	}
}

bool TDataReadSignalMap::SignalExists(const std::string& signalName)
{
	return SignalMap.count(signalName) != 0;
}

void TDataReadSignalMap::EmitSignal(const std::string& signalName, const boost::property_tree::ptree& pt)
{
	if (SignalMap.count(signalName) == 0)
	{
		throw ErrorToLog("Signal " + signalName + " does not exist!");
	}

	(*SignalMap[signalName])(pt);
}



void TDataReadSignalMap::Clear()
{
	SignalMap.clear();
}





TDataReader::TDataReader(boost::asio::ip::tcp::socket& socket)
	: Socket(socket)
{
}

void TDataReader::InnerStartRead()
{
	boost::asio::async_read(Socket, boost::asio::buffer(&DataSize, 4), boost::bind(&TDataReader::HandleReadDataSize, shared_from_this(), boost::asio::placeholders::error));
	
}

void TDataReader::StartReadOnce()
{
	InnerStartRead();
	Nonstop = false;
}

void TDataReader::StartReadNonstop()
{
	InnerStartRead();
	Nonstop = true;
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

		BOOST_FOREACH(auto i, propertyTree)
		{
			if (DataReadSignalMap.SignalExists(i.first))
			{
				DataReadSignalMap.EmitSignal(i.first, i.second);
			}
		}

		if (Nonstop)
		{
			InnerStartRead();
		}


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
	boost::shared_ptr<TDataReader> dataReader(new TDataReader(Socket));

	dataReader->DataReadSignalMap.AddSlot("OnHello", boost::bind(&TSimpleAuthorization::HandleGetData, this, _1));

	dataReader->ErrorSignal.connect(ErrorSignal);

	if (Login == "")
	{
		boost::property_tree::ptree pt;

		pt.put("Hello", "");

		SendPropertyTree(IoService, Socket, pt);

		dataReader->StartReadOnce();

	}
	else
	{
		boost::property_tree::ptree pt;

		pt.put("HelloAgain.Login", Login);
		pt.put("HelloAgain.Password", Password);

		SendPropertyTree(IoService, Socket, pt);

		dataReader->StartReadOnce();
	}

}


void TSimpleAuthorization::HandleGetData(boost::property_tree::ptree pTree)
{
	Login = pTree.get<std::string>("Login");
	Password = pTree.get<std::string>("Password");

	SaveLoginPasswordSignal(Login, Password);

	AuthorizedSignal();
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
    
	boost::asio::async_connect(Socket, iterator, boost::bind(&TClientSocket::HandleConnect, this, boost::asio::placeholders::error));
    
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

		ClientDataReader->DataReadSignalMap.Clear();

		std::shared_ptr<TSimpleAuthorization> authorization = boost::get<std::shared_ptr<TSimpleAuthorization>>(Authorization);

		authorization->AuthorizedSignal.disconnect_all_slots();
		authorization->SaveLoginPasswordSignal.disconnect_all_slots();
		authorization->ErrorSignal.disconnect_all_slots();


		OnDisconnectedSignal();

		OnAddressNotResolvedSignal.disconnect_all_slots();
		OnConnectedSignal.disconnect_all_slots();
		OnAutorizedSignal.disconnect_all_slots();
		OnDisconnectedSignal.disconnect_all_slots();

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
	ClientDataReader = boost::shared_ptr<TDataReader>(new TDataReader(Socket));
	OnAutorizedSignal();
	ClientDataReader->StartReadNonstop();
}

void TClientSocket::HandleAuthorizationError()
{

}

void TClientSocket::SendPropertyTree(boost::property_tree::ptree pTree)
{
	SE::SendPropertyTree(IoService, Socket, pTree);
}

} //namspace SE