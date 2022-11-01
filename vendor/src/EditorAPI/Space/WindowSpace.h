#pragma once

class EmberGUI;
struct GUISpace //Interface
{
public:
	virtual void Invoke() = 0;

public:
	unsigned int myID;
	bool myHoveredFlag, myFocusedFlag;
	const char* myName;
	EmberGUI* myGUIInterface;

};

template<typename SpaceDef>
struct EmberGUISpace : public GUISpace //Base Template
{
	EmberGUISpace(SpaceDef&& aCallback) : GUISpace(), mySpaceDefinition(aCallback) {}
	inline void Invoke() override
	{
		mySpaceDefinition(this);
	}
private:
	SpaceDef mySpaceDefinition;

};
