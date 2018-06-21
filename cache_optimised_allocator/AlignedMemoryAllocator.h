#pragma once
#include <cstddef>
#include <vector>
#include <mutex>

namespace custom_allocators
{
	template<std::size_t alignment>
	class AlignedMemoryAllocator
	{
	public:
		using size_type = size_t;

		template <typename T>
		class Type
		{
		public:
			using void_pointer = void*;
			using const_void_pointer = const void*;

			using pointer = T*;
			using const_pointer = const T*;

			using value_type = T;

			using difference_type = decltype ( static_cast< pointer >( nullptr ) - static_cast< pointer >( nullptr ) );

			static constexpr bool propagate_on_container_copy_assignment = true;
			static constexpr bool propagate_on_container_move_assignment = true;
			static constexpr bool propagate_on_container_swap = true;
			static constexpr bool is_always_equal = true;

			template<typename U>
			class rebind
			{
				using other = Type<U>;
			};

			//static constexpr size_type max_size ( ) noexcept
			//{
			//	return std::numeric_limits<size_type>::max ( );
			//}
			static pointer allocate ( const size_type size )
			{
				return static_cast< pointer > ( AlignedMemoryAllocator::allocate ( size * sizeof ( T ) ) );
			}
			static void deallocate ( const pointer memory_block, const size_type size ) noexcept
			{
				AlignedMemoryAllocator::deallocate ( memory_block, size * sizeof ( T ) );
			}

			Type ( ) = default;

			template <class U>
			constexpr Type ( const Type<U>& ) noexcept
			{
			}

			template <typename U>
			constexpr bool operator==( const Type<U>& ) const  noexcept
			{
				return true;
			}

			template <typename U>
			constexpr bool operator!=( const Type<U>& ) const noexcept
			{
				return false;
			}

			Type select_on_container_copy_construction ( )
			{
				return Type { };
			}
		};

	private:
		static auto& get_mutex ( ) noexcept
		{
			static std::mutex mutex;

			return mutex;
		}
		static auto& get_memory_pool ( ) noexcept
		{
			static std::vector<std::pair<void*, void*>> memory_pool;

			return memory_pool;
		}

		static void* allocate ( const size_type size )
		{
			const auto actual_size = size + alignment;

			std::lock_guard<std::mutex> guard { get_mutex ( ) };

			if ( const auto memory_block = std::malloc ( actual_size ) )
			{
				auto aligned_memory_block = memory_block;
				auto aligned_memory_block_size = actual_size;

				if ( std::align ( alignment, size, aligned_memory_block, aligned_memory_block_size ) )
				{
					get_memory_pool ( ).push_back ( { aligned_memory_block, memory_block } );

					return aligned_memory_block;
				}
			}

			throw std::bad_alloc ( );
		}
		static void deallocate ( void*const memory_block, const size_type ) noexcept
		{
			std::lock_guard<std::mutex> guard { get_mutex ( ) };
			auto& memory_pool = get_memory_pool ( );

			for ( auto it = memory_pool.begin ( ), end = memory_pool.end ( ); it != end; ++it )
			{
				const auto pair = *it;

				if ( pair.first == memory_block )
				{
					std::free ( pair.second );

					*it = memory_pool.back ( );
					memory_pool.pop_back ( );

					break;
				}
			}
		}
	};
}
