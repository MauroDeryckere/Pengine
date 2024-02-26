#ifndef COMPONENT
#define COMPONENT

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		Component(GameObject* pGameObj);

		GameObject* GetGameObj() const { return m_pGameObject; };

	private:
		GameObject* m_pGameObject;

	};

}
#endif

