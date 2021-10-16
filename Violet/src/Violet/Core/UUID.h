#pragma once
#include <xhash>

namespace Violet {
	/*UUID class uses C++ standard  random number generator*/
	class VIOLET_API UUID
	{
	public:
		UUID();
		UUID(uint64_t UUID);

		operator uint64_t() const { return m_UUID; }  //UUID to uint64_t conversion
	private:
		uint64_t m_UUID;  //Usually UUIDs are 128-bit not 64-bit, but it's fine for the use case of a game engine
	};

}


namespace std {

	/*Provide a hash function for UUID that unordered_map can use*/
	//Make a template specialization for the C++ standard hash struct for our UUID
	template<>
	struct hash<Violet::UUID>
	{
		/*Hash Function*/
		std::size_t operator()(const Violet::UUID& UUID) const
		{
			//Redirect it to the "uint64_t" template specialization (It's already defined in the C++ standards), note that UUID class is ultimately a uint64_t
			return hash<uint64_t>()(static_cast<uint64_t>(UUID));
		}
	};

}