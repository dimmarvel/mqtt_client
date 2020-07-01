#pragma once

class ISubscriber
{
public:
	virtual ~ISubscriber() {};
	virtual void Update(std::string const &message, std::string topic) = 0;
};

class IPublisher
{
public:
	virtual ~IPublisher() {};
	virtual void Attach(ISubscriber * subscriber) = 0;
	virtual void Detach(ISubscriber * subscriber) = 0;
	virtual void Notify() = 0;
};