#include <vector>
#include "AlignedMemoryAllocator.h"

int main ( )
{
	using namespace custom_allocators;

	constexpr auto cache_line_size = size_t { 64 };
	using CacheAwareAllocator = AlignedMemoryAllocator<cache_line_size>;

	std::vector<int, CacheAwareAllocator::Type<int>> d;

	d.resize ( 12 );

	return 0;
}