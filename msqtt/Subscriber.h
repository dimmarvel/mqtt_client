#pragma once

class Subscriber : public mosqpp::mosquittopp, public ISubscriber
{
private:

	std::string  _host;
	std::string  _id;
	int			 _port;
	int			 _howalive;

	static int	_number;
	int			_thisNumber;

	std::string					_message;
	std::vector<Publisher*>		_publishers;
	std::vector<std::string>	_subscribeTopics;

public:
	Subscriber();
	Subscriber(std::string id, std::string  topic, std::string host, int port, Publisher& publisher);
	~Subscriber();

	void Publisher_handler();
	//чекнуть работоспособность
	void Subscribe_On_Topic(std::string topic)
	{
		for (auto publishers_it : _publishers)
		{
			if (publishers_it->Check_Topic(topic))
			{
				subscribe(NULL, topic.c_str(), 1);
				_subscribeTopics.push_back(topic);
				std::cout << "Subscribe on topic " << topic << " was success." << std::endl;
				return;
			}
		}
		std::cout << "Topic " << topic << " not found." << std::endl;
	}
	void Out_Alive_Topics();
	void Create_Subscriber();
	void Subscribe_On_Publisher(Publisher* pub);
	bool Check_Subscribe(std::string topic);
	bool is_Sub_On_Publisher(Publisher* pub);

	void Update(std::string const &message, std::string topic) override;

	std::string get_id() { return _id; }
};
