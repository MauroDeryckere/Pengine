#ifndef COMPONENT
#define COMPONENT

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		Component(GameObject* pGameObj);
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		GameObject* m_pGameObject;

	private:

	};

}
#endif

