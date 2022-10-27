#pragma once
#include "Core/Editor/GUIWindow.h"
#include "Core/Editor/RenderID.h"
#include "Core/Graphics/DirectX11/DXIncludes.h"

#include "Core/Graphics/CameraInterface.h"
#include <memory>
namespace Dragonite
{
	class SceneEditor;
	class Scene;
	class GraphicalInterface;
	class Mouse;
	class Object;
	class TextureFactory;
	class Texture;



	class TextureFactory;

	class Viewport : public GUIWindow
	{
	public:
		Viewport();
		~Viewport() override;
		const bool TryGetObjectID(Mouse* aMouse, int& anId);
		inline void RegisterSceneEditor(SceneEditor* anEditor) { mySceneEditor = anEditor; }
		void DisplayDebugInfo(Mouse* aMouse);

		const Vector2f GetLocalMousePos(Mouse* aMouse);
		void ManipulateObject(Dragonite::Scene* aScene, Dragonite::Object* anObject);
	protected:
		void OnWindowInit() override;
		void OnWindowUpdate() override;
		void OnEnable() override;
		void OnDisable() override;
	private:


		void RenderViewport();
		void DetectAssetDrop();
		void RenderTopBar();

	private:
		//Primitive Data
		int myFoundID;
		bool myIsInPlayFlag;
		int myLastValidElement;
		bool myIsManipulatingFlag;
		float myAspectRatio;

		//Transform data
		Vector2f myMousePos;
		Vector2f myMinRegion;
		Vector2f myCurrentResolution;
		Vector2f myPreviousResolution;

	
		//Camera data
		PerspectiveProfile myPerspectiveProfile;
		OrthographicProfile myOrthographicProfile;
		CameraInterface myEditorCameraInterface;
		GraphicalInterface* myGraphicsInterface;

		//Scene data
		Scene* myScene;
		RenderID myRenderID;
		RenderView myViewportTarget;

		//Editor References
		ShaderResourceV myViewportResource;
		SceneEditor* mySceneEditor;
		TextureFactory* myTextureFactory;

		//Icons
		std::shared_ptr<Texture>
			myPlayIcon,
			myStopIcon,
			mySaveIcon;


	};

}

