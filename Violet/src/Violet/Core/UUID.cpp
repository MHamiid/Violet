#include "VIOPCH.h"
#include "UUID.h"
#include <random>

namespace Violet {
	static std::random_device s_randomDevice;
	static std::mt19937_64 s_engine(s_randomDevice());  //64-bit version
	static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

	UUID::UUID() : m_UUID(s_uniformDistribution(s_engine))
	{
	}
	UUID::UUID(uint64_t UUID) : m_UUID(UUID)
	{
	}
}