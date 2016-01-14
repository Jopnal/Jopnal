#ifndef COMPONENT_H
#define COMPONENT_H


class Component
{
public:
	virtual ~Component();//virtual destructor


	virtual void SendMessage(Message *message);//use message system / -class, to send messages

	virtual void Update(float deltaTime);
	virtual void Init(void);
	virtual void Uninit(void);

	bool isActive(void) const;//Check if active, if not delete

private:
	//int compID; // each component type has unique ID

}









#endif