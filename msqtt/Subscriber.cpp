#pragma once
#include "definition.h"
#include"Notify.h"
#include"Publisher.h"
#include"Subscriber.h"

int Subscriber::_number = 0;

Subscriber::Subscriber() :
	_howalive(6000),
	_id(""),
	_port(1883),
	_host("")
{
	mosqpp::lib_init();
}


Subscriber::Subscriber( std::string id,  std::string  topic,  std::string host, int port, Publisher& publisher) :
	_howalive(6000),
	_id(id),
	_port(port),
	_host(host)
{

	mosqpp::lib_init();

	connect(_host.c_str(), _port, _howalive);
	loop_start();

	subscribe(NULL, topic.c_str(), 1);
	_subscribeTopics.push_back(topic);
	
	_thisNumber = ++Subscriber::_number;

	if (Check_Subscribe(topic))
	{
		_publishers.push_back(&publisher);
		publisher.Attach(this);
	}

}

Subscriber::~Subscriber() 
{
	_publishers.clear();
	std::cout << "deleted sub" << std::endl;
}


void Subscriber::Create_Subscriber() 
{

	std::string id;
	std::string topic;
	std::string host;
	int port;

	std::cout << "Iput name: ";
	std::cin >> id;
	std::cout << "Iput topic: ";
	std::cin >> topic;
	std::cout << "Iput host: ";
	std::cin >> host;
	std::cout << "Iput port: ";
	std::cin >> _port;
	std::cout << "Iput how alive(sec): ";
	std::cin >> _howalive;

	this->_host = host;
	this->_id = id;

	connect(_host.c_str(), _port, _howalive);
	loop_start();

	subscribe(NULL, topic.c_str(), 1);

	if (Check_Subscribe(topic))
	{
		_subscribeTopics.push_back(topic);
	}

	_thisNumber = ++Subscriber::_number;
}

void Subscriber::Update(std::string const &message, std::string topic)
{
	if (!Check_Subscribe(topic))
	{
		return; //not subscribe on topic echo
	}
	_message = message;
	Publisher_handler();
}


void Subscriber::Publisher_handler()
{
	std::cout << "Subscriber number " << _thisNumber <<" hears the publisher, his message - " << _message << std::endl;
}

bool Subscriber::Check_Subscribe(std::string topic) {
	for (auto it : _subscribeTopics)
	{
		if (it == topic)
		{
			return true;
		}
	}
	return false;
}

void Subscriber::Out_Alive_Topics()
{
	for (auto it : _publishers) 
	{
		it->Out_All_Topics();
	}
}

bool Subscriber::is_Sub_On_Publisher(Publisher* pub)
{
	for (auto it : _publishers) 
	{
		if (it->get_id() == pub->get_id())
		{
			return true;
		}
	}
	return false;
}
void Subscriber::Subscribe_On_Publisher(Publisher* pub) 
{
	_publishers.push_back(pub);
	pub->Attach(this);
}
