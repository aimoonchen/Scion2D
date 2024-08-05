#pragma once
#include "Core/ECS/Components/AllComponents.h"
#include <Rendering/Core/Camera2D.h>
namespace SCION_CORE
{
bool EntityInView( const SCION_CORE::ECS::TransformComponent& transform, float width, float height,
				   const SCION_RENDERING::Camera2D& camera );
glm::mat4 RSTModel( const SCION_CORE::ECS::TransformComponent& transform, float width, float height );
} // namespace SCION_CORE