#ifndef SIGNAL_SENDER_H_INCLUDED
#define SIGNAL_SENDER_H_INCLUDED

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/array.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/signal.hpp"
#include "boost/thread.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"

namespace SE
{
	//Xperimental -- need to optimize this

struct TSignalSender
	: public boost::enable_shared_from_this<TSignalSender>
{
	boost::asio::ip::tcp::socket& Socket;

	int Length;

	std::string Data;

	boost::signal<void()> ErrorSignal;

	TSignalSender(boost::asio::ip::tcp::socket& socket, int length, std::string data)
		: Socket(socket)
		, Length(length)
		, Data(data)
	{
	}

	void Send()
	{
		boost::asio::async_write(Socket, boost::asio::buffer(&Length, 4), boost::bind(&TSignalSender::Handle, shared_from_this(), boost::asio::placeholders::error));
		boost::asio::async_write(Socket, boost::asio::buffer(Data.c_str(), Data.size()), boost::bind(&TSignalSender::Handle, shared_from_this(), boost::asio::placeholders::error));
	}

	void Handle(const boost::system::error_code& error)
	{
		if (error)
		{
			ErrorSignal();
		}
	}
};

} //namespace SE

#endif
