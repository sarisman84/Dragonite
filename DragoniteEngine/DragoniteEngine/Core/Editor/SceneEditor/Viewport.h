#pragma once
#include "Core/Editor/GUIWindow.h"
#include "Core/Editor/RenderID.h"
#include "Core/Graphics/DirectX11/DXIncludes.h"
namespace Dragonite
{
	class SceneEditor;
	class Scene;
	class GraphicalInterface;
	class Mouse;

	class Viewport : public GUIWindow
	{
	public:
		Viewport();
		~Viewport() override;
		const bool TryGetObjectID(Mouse* aMouse, int& anId);
		inline void RegisterSceneEditor(SceneEditor* anEditor) { mySceneEditor = anEditor; }
		inline const bool IsBeingFocused() const noexcept { return myFocusFlag; }
		void DisplayMouseCoordinateInViewport(Mouse* aMouse);

	protected:
		void OnWindowInit() override;
		void OnWindowUpdate() override;
		void OnEnable() override;
		void OnDisable() override;
	private:
		int myFoundID;


		Vector2f myMousePos;
		Vector2f myMinRegion;
		Vector2f myCurrentResolution;
		Vector2f myPreviousResolution;
		float myAspectRatio;
		bool myFocusFlag;


		GraphicalInterface* myGraphicsInterface;
		Scene* myCurrentScene;
		RenderID myRenderID;

		RenderView myViewportTarget;
		ShaderResourceV myViewportResource;
		SceneEditor* mySceneEditor;

	};

}

