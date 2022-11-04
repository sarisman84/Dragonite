#pragma once
#include <vector>
#include <memory>
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

class EmberGUI;
struct GUISpace //Interface
{
public:
	GUISpace() = default;
	GUISpace(const char* aName, EmberGUI* anEmberGUIInterface);
	virtual ~GUISpace() = default;
	virtual void Invoke(void* someExtraData = nullptr) = 0;


	inline ID3D11Device* Device() { return myDevice; }
	inline ID3D11DeviceContext* Context() { return myContext; }

protected:
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
public:
	unsigned int myID;
	bool myHoveredFlag, myFocusedFlag;
	const char* myName;
	EmberGUI* myGUIInterface;

};

template<typename SpaceDef>
struct EmberGUISpace : public GUISpace //Base Template
{
	EmberGUISpace(EmberGUI* anInterface, const char* aName, SpaceDef&& aCallback) : GUISpace(aName, anInterface), mySpaceDefinition(aCallback) {}
	inline void Invoke(void* someExtraData = nullptr) override
	{
		mySpaceDefinition(this, someExtraData);
	}
private:
	SpaceDef& mySpaceDefinition;

};
