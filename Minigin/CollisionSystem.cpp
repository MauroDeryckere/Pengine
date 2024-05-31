#include "CollisionSystem.h"

#include "ECS.h"

#include "TransformComponent.h"
#include "RectColliderComponent.h"
#include "BodyComponent.h"

#include "CollisionEvent.h"

#include <functional>

namespace Pengin
{
	CollisionSystem::CollisionSystem(ECS& ecs, bool CollisionResolution):
		m_ECS{ ecs },
		m_CollisionResolution{ CollisionResolution }
	{ }

	void CollisionSystem::Step()
	{
		auto rectCollComps = m_ECS.GetComponents<RectColliderComponent>();

		for (auto collRectAIt = rectCollComps.begin(); collRectAIt != rectCollComps.end(); ++collRectAIt)
		{
			const EntityId entityA = rectCollComps.GetIdFromIterator(collRectAIt);
			const auto& transA{ m_ECS.GetComponent<TransformComponent>(entityA) };
			const auto& rectCollA = (*collRectAIt);

			BodyComponent* bodyAPtr{ m_ECS.HasComponent<BodyComponent>(entityA) ? 
																		&m_ECS.GetComponent<BodyComponent>(entityA) 
																		: nullptr };

			const UtilStructs::Rectf rectA = bodyAPtr ? 
											CalcCollRect(bodyAPtr, transA, rectCollA)
											: CalcCollRect(transA, rectCollA);

			for (auto collRectBIt = std::next(collRectAIt); collRectBIt != rectCollComps.end(); ++collRectBIt)
			{
				const EntityId entityB = rectCollComps.GetIdFromIterator(collRectBIt);
				const auto& transB{ m_ECS.GetComponent<TransformComponent>(entityB) };
				const auto& rectCollB = (*collRectBIt);

				BodyComponent* bodyBPtr{ m_ECS.HasComponent<BodyComponent>(entityB) ?
																			&m_ECS.GetComponent<BodyComponent>(entityB)
																			: nullptr };

				const UtilStructs::Rectf rectB = bodyBPtr ?
												CalcCollRect(bodyBPtr, transB, rectCollB)
												: CalcCollRect(transB, rectCollB);

				if (!UtilFuncs::IsCollidingAABB(rectA, rectB))
				{
					//No collision
					continue;
				}

				if (bodyAPtr && bodyBPtr)
				{
					if (bodyAPtr->collType == BodyComponent::CollType::Static && bodyBPtr->collType == BodyComponent::CollType::Static)
					{
						//No collision to deal with, 2 static bodies
						continue;
					}
					else if (bodyAPtr->collType == BodyComponent::CollType::Trigger || bodyBPtr->collType == BodyComponent::CollType::Trigger)
					{
						DEBUG_OUT("trigger COLL\n\n\n");
						const CollPair pair{ entityA, entityB };
						m_FrameCollisions.insert(pair);
						continue;
					}
				}

				//Add to the set of unqiue collisions to later dispatch an event
				{
					const CollPair pair { entityA, entityB };
					m_FrameCollisions.insert(pair);
				}

				if (bodyAPtr && bodyBPtr)
				{
					const glm::vec3 normal{ CalcCollNormal(rectA, rectB) };
					const float penetrationDepth{ CalcPenetrationDeth(normal, rectA, rectB) };

					SeparateBodies(bodyAPtr, bodyBPtr, normal, penetrationDepth);

					if (m_CollisionResolution)
					{
						const glm::vec3 relativeVelocity = bodyBPtr->velocity - bodyAPtr->velocity;
						const float relativeVelocityAlongNormal = glm::dot(relativeVelocity, normal);

						constexpr float restitutionFactor = -1.f; //For now, we do not support elasticity (it does work but not added as a parameter in body and still need to dd physics for friction,..)
						const float impulseMagnitude = -(1.0f + restitutionFactor) * relativeVelocityAlongNormal;

						if (bodyAPtr->collType == BodyComponent::CollType::Static)
						{
							bodyBPtr->velocity += normal * impulseMagnitude;
						}
						else if (bodyBPtr->collType == BodyComponent::CollType::Static)
						{
							bodyAPtr->velocity -= normal * impulseMagnitude;
						}
						else
						{
							bodyAPtr->velocity -= normal * impulseMagnitude;
							bodyBPtr->velocity += normal * impulseMagnitude;
						}
					}
				}
			}
		}
	}

	constexpr UtilStructs::Rectf CollisionSystem::CalcCollRect(const TransformComponent& transform, const RectColliderComponent& rColl) const noexcept
	{
		static_assert(std::is_move_constructible_v<UtilStructs::Rectf>);
		static_assert(std::is_move_assignable_v<UtilStructs::Rectf>);

		return UtilStructs::Rectf
		{ 
			(transform.worldPos.x + rColl.collRect.x * transform.scale.x),
			(transform.worldPos.y + rColl.collRect.y * transform.scale.y),
			(transform.scale.x * rColl.collRect.width),
			(transform.scale.y * rColl.collRect.height) 
		};
	}

	constexpr UtilStructs::Rectf CollisionSystem::CalcCollRect(const BodyComponent* body, const TransformComponent& transform, const RectColliderComponent& rColl) const noexcept
	{
		static_assert(std::is_move_constructible_v<UtilStructs::Rectf>);
		static_assert(std::is_move_assignable_v<UtilStructs::Rectf>);

		return UtilStructs::Rectf
		{
			(body->currentPosition.x + rColl.collRect.x * transform.scale.x),
			(body->currentPosition.y + rColl.collRect.y * transform.scale.y),
			(rColl.collRect.width * transform.scale.x),
			(rColl.collRect.height * transform.scale.y)
		};
	}

	const glm::vec3 CollisionSystem::CalcCollNormal(const UtilStructs::Rectf& rectA, const UtilStructs::Rectf& rectB) const noexcept
	{
		const float dx = (rectB.x + rectB.width / 2.0f) - (rectA.x + rectA.width / 2.0f);
		const float dy = (rectB.y + rectB.height / 2.0f) - (rectA.y + rectA.height / 2.0f);

		const float combinedHalfWidths = (rectA.width + rectB.width) / 2.0f;
		const float combinedHalfHeights = (rectA.height + rectB.height) / 2.0f;

		const float overlapX = combinedHalfWidths - fabs(dx);
		const float overlapY = combinedHalfHeights - fabs(dy);

		if (overlapX < overlapY)
		{
			return glm::vec3{ (dx < 0.f) ? -1.f : 1.f, 0.f, 0.f };
		}
		else
		{
			return glm::vec3{ 0.f, (dy < 0.f) ? -1.f : 1.f, 0.f };
		}
	}

	const float CollisionSystem::CalcPenetrationDeth(const glm::vec3& normal, const UtilStructs::Rectf& rectA, const UtilStructs::Rectf& rectB) const noexcept
	{
		if (normal.x != 0.f)
		{
			if (normal.x > 0.f)
			{
				return (rectA.x + rectA.width) - rectB.x;
			}

			return (rectB.x + rectB.width) - rectA.x;
		}

		else if (normal.y != 0.f)
		{
			if (normal.y > 0.f)
			{
				return (rectA.y + rectA.height) - rectB.y;
			}

			return (rectB.y + rectB.height) - rectA.y;
		}

		return 0.f;
	}

	void CollisionSystem::SeparateBodies(BodyComponent* bodyA, BodyComponent* bodyB, const glm::vec3& normal, const float penDepth) noexcept
	{
		if (bodyA->collType == BodyComponent::CollType::Static)
		{
			bodyB->currentPosition += penDepth * normal;
		}
		else if (bodyB->collType == BodyComponent::CollType::Static)
		{
			bodyA->currentPosition -= penDepth * normal;
		}
		else
		{
			bodyA->currentPosition -= penDepth/2.f * normal;
			bodyB->currentPosition += penDepth/2.f * normal;
		}
	}

	void CollisionSystem::BroadCastCollisionsEvents() noexcept
	{
		for (const auto& pair : m_FrameCollisions)
		{;
			EventManager::GetInstance().BroadcoastEvent(std::make_unique<CollisionEvent>(pair.entityA, pair.entityB));
		}

		m_FrameCollisions.clear();
	}
}
