#pragma once
#include <memory>

class SceneBase;
class Pad;

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	void Init();
	void Update(const Pad& pad);
	void Draw();
	void End();
private:
	std::shared_ptr<SceneBase>m_pScene;
};

