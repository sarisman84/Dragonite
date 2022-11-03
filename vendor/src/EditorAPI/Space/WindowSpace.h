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
	GUISpace(ID3D11Device* aDevice, ID3D11DeviceContext* aContext)
	{
		myDevice = aDevice;
		myContext = aContext;
	}
	virtual void Invoke(std::vector<std::shared_ptr<GUISpace>>& someElements, ID3D11RenderTargetView* aBufferToRenderTo) = 0;


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
	EmberGUISpace(ID3D11Device* aDevice, ID3D11DeviceContext* aContext, SpaceDef&& aCallback) : GUISpace(aDevice, aContext), mySpaceDefinition(aCallback) {}
	inline void Invoke(std::vector<std::shared_ptr<GUISpace>>& someElements, ID3D11RenderTargetView* aBufferToRenderTo) override
	{
		mySpaceDefinition(this, someElements, aBufferToRenderTo);
	}
private:
	SpaceDef& mySpaceDefinition;

};
