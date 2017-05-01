#pragma once
#include <GLM\glm.hpp>

template<typename VectorType>
float MagnitudeSquared(const VectorType& vector)
{
  return glm::dot(vector, vector);
}