#include "Transform.h"
#include "GameObject.h"

#include <cassert>
#include <iostream>

dae::Transform::Transform(GameObject* pGameObj):
    m_position{}, m_pGameObj{ pGameObj }
{
    assert(m_pGameObj);
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

    m_pGameObj->SetPosDirty();
}

void dae::Transform::Rotate(float angle, const glm::vec3& axis, bool rotateAroundParent, const glm::vec3& rotPoint)
{
    glm::mat4 rotationMatrix;
    glm::vec4 rotatedOffset;

    assert(!(rotateAroundParent && rotPoint != glm::vec3{}));

    if (rotateAroundParent) 
    {
        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
        rotatedOffset = rotationMatrix * glm::vec4(m_position, 1.0f);
    }
    else
    {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -rotPoint);
        glm::mat4 reverseTranslationMatrix = glm::translate(glm::mat4(1.0f), rotPoint);

        rotationMatrix = reverseTranslationMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis) * translationMatrix;
        rotatedOffset = rotationMatrix * glm::vec4(m_position, 1.0f);
    }

    m_position = glm::vec3(rotatedOffset);
    m_pGameObj->SetPosDirty();
}