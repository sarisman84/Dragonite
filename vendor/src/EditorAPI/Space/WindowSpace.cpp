#include "WindowSpace.h"
#include "EditorAPI/EmberGUI.h"
GUISpace::GUISpace(const char* aName,EmberGUI* anEmberGUIInterface)
{
	myName = aName;
	myGUIInterface = anEmberGUIInterface;
	myDevice = myGUIInterface->GetDevice();
	myContext = myGUIInterface->GetContext();
}
