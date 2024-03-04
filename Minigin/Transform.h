#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dae
{
    class GameObject;

    class Transform final
    {
    public:
        Transform(GameObject* pGameObj);

        const glm::vec3& GetPosition() const { return m_position; }
        void SetPosition(float x, float y, float z);

        void Rotate(float angle, const glm::vec3& axis, bool rotateAroundParent = true, const glm::vec3& rotPoint = {});

    private:
        GameObject* m_pGameObj;

        glm::vec3 m_position;
    };
}
