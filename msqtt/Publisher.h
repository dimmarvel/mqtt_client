#pragma once

class Publisher : public mosqpp::mosquittopp, public IPublisher
{
private:

	std::string  _host;
	std::string  _id;
	std::string	 _currentTopic;
	int			 _port;
	int			 _howalive;

	std::string _message;
	std::list<ISubscriber*> _subs;
	std::vector<std::string> _allTopic;

	void connect_handler(int rc);
	void disconnect_handler(int rc);
	void publish_handler(int rc);


public:

	Publisher();
	Publisher(std::string id, std::string  topic, std::string host, int port);
	~Publisher();

	void send_message(std::string  message);

	std::string getTopic();

	void Create_Topic(std::string topic_name);
	bool Check_Topic(std::string topic_name);
	void Choice_Topic();
	void Out_All_Topics();
	void Create_Publisher();

	void Attach(ISubscriber * subscriber) override;
	void Detach(ISubscriber * subscriber) override;
	void Notify() override;

	std::string get_id() { return _id; }
};