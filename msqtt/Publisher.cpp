#pragma once
#include "definition.h"
#include"Notify.h"
#include"Publisher.h"

Publisher::Publisher() :
	_howalive(6000),
	_id(""),
	_currentTopic(""),
	_port(1883),
	_host("")
{
	mosqpp::lib_init();
}

Publisher::Publisher(std::string id, std::string  topic, std::string host, int port) :
	_howalive(6000),
	_id(id),
	_currentTopic(topic),
	_port(port),
	_host(host)
{

	mosqpp::lib_init();

	connect_handler(connect(_host.c_str(), _port, _howalive));
	loop_start();

	_allTopic.push_back(_currentTopic);
}

Publisher::~Publisher()
{
	loop_stop();
	disconnect_handler(mosqpp::lib_cleanup());

	_subs.clear();
}

void Publisher::Create_Publisher() 
{

	std::string id;
	std::string topic;
	std::string host;
	int port;
	std::cout << "Iput name: ";
	std::cin >> _id;
	std::cout << "Iput topic: ";
	std::cin >> _currentTopic;
	std::cout << "Iput host: ";
	std::cin >> _host;
	std::cout << "Iput port: ";
	std::cin >> _port;
	std::cout << "Iput how alive(sec): ";
	std::cin >> _howalive;

	connect_handler(connect(_host.c_str(), _port, _howalive));
	loop_start();

	_allTopic.push_back(_currentTopic);
}

void Publisher::disconnect_handler(int rc)
{
	std::cout << "Publisher - disconnect (" << rc << ")" << std::endl;
}

void Publisher::connect_handler(int mid)
{
	std::cout << "Publisher - message (" << mid << ") succes connect " << std::endl;
}

void Publisher::publish_handler(int mid)
{
	std::cout << "Publisher - message (" << mid << ") succes to be published " << std::endl;
}

void Publisher::Attach(ISubscriber * subscriber)
{
	_subs.push_back(subscriber);
}

void Publisher::Detach(ISubscriber * subscriber)
{
	_subs.remove(subscriber);
}

void Publisher::Create_Topic(std::string topic_name)
{
	_allTopic.push_back(topic_name);
}

void Publisher::Out_All_Topics()
{
	int i = 0;
	for (auto it : _allTopic)
	{
		std::cout << i++ << " Topic name: " << it << std::endl;
	}
}

//чекнуть работоспособность
void Publisher::Choice_Topic()
{
	std::cout << "Choice topic(input number)" << std::endl;
	Out_All_Topics();

	int id = 0;
	std::cin >> id;


	if (id < _allTopic.size())
	{
		_currentTopic = _allTopic.at(id);
	}
	else
	{
		std::cout << "Error: out of range, was choice another topic(" << 0 << ")\n";
		_currentTopic = _allTopic.at(0);
	}
	std::cout << "You choice topic: " << _allTopic[id] << std::endl;

}

std::string Publisher::getTopic()
{
	return _currentTopic;
}

//чекнуть работоспособность
bool Publisher::Check_Topic(std::string topic_name)
{
	for (auto it : _allTopic)
	{
		if (topic_name == it)
		{
			return true;
		}
	}
	return false;
}

void Publisher::Notify()
{

	auto it = _subs.begin();
	while (it != _subs.end())
	{
		(*it)->Update(_message, _currentTopic);
		++it;
	}
}

void Publisher::send_message(std::string message)
{
	int ret = publish(NULL, _currentTopic.c_str(), strlen(message.c_str()), message.c_str(), 1, true);
	_message = message;

	if (ret == MOSQ_ERR_SUCCESS)
	{
		publish_handler(ret);
	}
	else
	{
		std::cout << "Publisher - message (" << ret << ") succes to be published." << std::endl;
	}

	Notify();
}