// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
   $File: TEDebug\debug_memory.h $
   $Created: 01-01-2017 00h00m00s $
   $Modified: 06-02-2017 09h39m24s $
   $Revision: $
   $Creator : TOFFANIN Marc $
   $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(DEBUG_MEMORY_H)
#define DEBUG_MEMORY_H

#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <map>

#define _REDEFINED_NEW 1
#define _HOOK_MALLOC 1
#define _CHECK_HEAP_CORRUPTION 1
#define _CHECK_HEAP_CHAR 'A'
#define _CHECK_HEAP_LENGTH 10

namespace TE
{
	//NOTE(MArc) : Make a real memory manager with a memory arena ? Probably worth the hassle
	struct simple_file_locator
	{
		size_t Size;
		const char* File;
		int Line;
		bool IsArray;
	};
	struct simple_memory_manager
	{
		static simple_memory_manager& instance()
		{
			static simple_memory_manager SMM;
			return (SMM);
		}
		std::map<void*, simple_file_locator > MemoryMap;
		size_t MemoryUsed = 0;

		void* Alloc(size_t Size, const char* File, int Line, bool IsArray)
		{
			if (Size <= 0)
				throw std::runtime_error("Trying to malloc zero sized object");
			auto SizeOffset = 0;
#if _CHECK_HEAP_CORRUPTION
			SizeOffset = _CHECK_HEAP_LENGTH;
#endif
			void* Ptr = malloc(Size + SizeOffset);
			if (Ptr)
			{
#if _CHECK_HEAP_CORRUPTION
				memset((char*)Ptr + Size, _CHECK_HEAP_CHAR, _CHECK_HEAP_LENGTH);
#endif
				simple_file_locator SFL{ Size, File, Line, IsArray };
				MemoryUsed += Size;
				MemoryMap[Ptr] = SFL;
				return (Ptr);
			}
			return (nullptr);
		}

		void Release(void* Ptr, const char* File, int Line, bool IsArray)
		{
			if (!Ptr)
				throw std::runtime_error("Trying to free a nullptr");
			if (!MemoryMap.empty())
			{
				auto It = MemoryMap.find(Ptr);
				if (It == MemoryMap.end())
				{
					//std::cerr << "Delete called on heap variable/thirdparty pointer/non-existent pointer : " << Ptr << std::endl;
				}
				else
				{
					if (It->second.IsArray == IsArray)
					{
						/*std::cerr << "Delete file "
						  << It->second.File
						  << " line "
						  << It->second.Line << std::endl;
						 */}
					else
						if (IsArray)
							std::cerr << "Using delete[] on new" << std::endl;
						else
							std::cerr << "Using delete on new[]" << std::endl;
					MemoryUsed -= It->second.Size;
					MemoryMap.erase(It);					
				}
			}
			free(Ptr);
		}


#if _CHECK_HEAP_CORRUPTION

		void CheckHeapCorruption()
		{
			const char* Buffer[_CHECK_HEAP_LENGTH];
			memset(Buffer, _CHECK_HEAP_CHAR, _CHECK_HEAP_LENGTH);
			if (!MemoryMap.empty())
			{
				for (auto& Item : MemoryMap)
				{
					if (memcmp(Buffer, (char*)(Item.first) + Item.second.Size, _CHECK_HEAP_LENGTH))
					{
						std::cerr << "Written past object size in object declared in file "
							<< Item.second.File
							<< " line "
							<< Item.second.Line << std::endl;
					}
				}
			}
		}
#else
		void CheckHeapCorruption()
		{

		}
#endif


		~simple_memory_manager()
		{
			if (!MemoryMap.empty())
			{
				int TotalBytesLeaked = 0;
				std::cerr << std::endl
					<< std::endl
					<< "----------------------------------------" << std::endl
					<< " MEMORY LEAKS DETECTED : BAD DAY, HUH ? " << std::endl
					<< "----------------------------------------" << std::endl
					<< std::endl;
				for (auto& Item : MemoryMap)
				{
					std::cerr << " MemoryLeak from file "
						<< Item.second.File
						<< " line "
						<< Item.second.Line 
						<< " for "
						<< Item.second.Size
						<< " bytes "
						<< " at ptr : "
						<< Item.first
						<< std::endl;

					TotalBytesLeaked += Item.second.Size;
				}

				std::cerr << "Memory leaked for a total of "
					<< TotalBytesLeaked
					<< " bytes" << std::endl;
			}
			else
			{
				std::cerr << std::endl
					<< std::endl
					<< "----------------------------------------" << std::endl
					<< " NO MEMORY LEAKS : GOOD JOB ! " << std::endl
					<< "----------------------------------------" << std::endl
					<< std::endl;
			}
			MemoryMap.clear();
		}
	};
}

	inline 
void* operator new(size_t Size, const char* File, int Line)
{
	return (TE::simple_memory_manager::instance().Alloc(Size, File, Line, false));
}

	inline 
void* operator new[](size_t Size, const char* File, int Line)
{
	return (TE::simple_memory_manager::instance().Alloc(Size, File, Line, true));
}

	inline
void operator delete(void* Ptr, const char* File, int Line)
{
	TE::simple_memory_manager::instance().Release(Ptr, nullptr, 0, false);
}

	inline
void operator delete[](void* Ptr, const char* File, int Line)
{
	TE::simple_memory_manager::instance().Release(Ptr, nullptr, 0, true);
}



//Macros to hook new and delete keywords
#define DEBUG_NEW new(__FILE__, __LINE__)
#if _REDEFINED_NEW
#define new DEBUG_NEW	

	inline
void operator delete(void* Ptr)
{
	TE::simple_memory_manager::instance().Release(Ptr, nullptr, 0, false);
}

	inline
void operator delete[](void* Ptr)
{
	TE::simple_memory_manager::instance().Release(Ptr, nullptr, 0, true);
}
#endif

# ifdef _HOOK_MALLOC
#   ifdef new
#     define malloc(s) ((void*)(new char[s]))
#   else
#     define malloc(s) ((void*)(DEBUG_NEW char[s]))
#   endif
#   define free(p) delete[] (char*)(p)
# endif

#endif
