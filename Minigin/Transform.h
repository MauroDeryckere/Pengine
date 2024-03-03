#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dae
{
    class Transform final
    {
    public:
        const glm::vec3& GetPosition() const { return m_position; }
        void SetPosition(float x, float y, float z);

        void Rotate(float angle, const glm::vec3& axis);

    private:
        glm::vec3 m_position;
    };
}
