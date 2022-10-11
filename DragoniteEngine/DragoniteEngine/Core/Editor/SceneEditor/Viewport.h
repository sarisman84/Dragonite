#pragma once
#include "Core/Editor/GUIWindow.h"
#include "Core/Editor/RenderID.h"
#include "Core/Graphics/DirectX11/DXIncludes.h"

#include "Core/Graphics/CameraInterface.h"
namespace Dragonite
{
	class SceneEditor;
	class Scene;
	class GraphicalInterface;
	class Mouse;
	class Object;


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

		int myFoundID;


		Vector2f myMousePos;
		Vector2f myMinRegion;
		Vector2f myCurrentResolution;
		Vector2f myPreviousResolution;

		int myLastValidElement;
		bool myIsManipulatingFlag;
		float myAspectRatio;

		PerspectiveProfile myPerspectiveProfile;
		CameraInterface myEditorCameraInterface;
		GraphicalInterface* myGraphicsInterface;
		Scene* myCurrentScene;
		RenderID myRenderID;

		RenderView myViewportTarget;
		ShaderResourceV myViewportResource;
		SceneEditor* mySceneEditor;
		TextureFactory* myTextureFactory;
		

	};

}

