#include <vector>

class Object
{
public:
    void Awake();
    void Start();
    void Update();
    void LateUpdate();
};

class Scene
{
public:
    static const bool CreateEmptyScene(Scene &aNewScene); //NO camera and dir light
    static const bool CreateScene(Scene &aNewScene); //With camera and dir light
    static const bool LoadScene(Scene& aLoadedScene, const char* aPath);

    void Play();
    void Pause();
    void Stop();


private:
    Scene* myExecutedScene;
    std::vector<Object> myObjects;
};