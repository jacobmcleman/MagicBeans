#pragma once
#include <..\Includes\GLM\glm.hpp>

template<typename VectorType>
float MagnitudeSquared(const VectorType& vector)
{
  return glm::dot(vector, vector);
}