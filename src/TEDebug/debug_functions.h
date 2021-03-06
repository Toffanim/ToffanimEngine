// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TEDebug\debug_functions.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m24s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(DEBUG_FUNCTIONS_H)
#define DEBUG_FUNCTIONS_H

#include <stdlib.h>
#include <vector>

//Partially inspired by HandmadeHero's code

struct debug_event
{
	const char* GUID;
	size_t CyclesCount = 0;
	size_t CyclesBegin = __rdtsc();
};

typedef std::vector<debug_event> debug_event_list;
static debug_event_list GlobalDebugEventList;

void RecordBeginEvent(const char* GUID)
{
	debug_event Event;
	Event.GUID = GUID;
	GlobalDebugEventList.push_back(Event);
}

void RecordEndEvent(const char* GUID)
{
	for (auto& DebugEvent : GlobalDebugEventList)
	{
		if (GUID == DebugEvent.GUID)
		{
			auto Cycles = __rdtsc();
			auto Diff = Cycles - DebugEvent.CyclesBegin;
			DebugEvent.CyclesCount = Diff;
		}
	}
}

#define UniqueFileCounterString__(A, B, C, D) A "|" #B "|" #C "|" D
#define UniqueFileCounterString_(A, B, C, D) UniqueFileCounterString__(A, B, C, D)
#define DEBUG_NAME(Name) UniqueFileCounterString_(__FILE__, __LINE__, __COUNTER__, Name)


#define TIMED_BLOCK__(GUID, Number, ...) timed_block TimedBlock_##Number(GUID, ## __VA_ARGS__)
#define TIMED_BLOCK_(GUID, Number, ...) TIMED_BLOCK__(GUID, Number, ## __VA_ARGS__)
#define TIMED_BLOCK(Name, ...) TIMED_BLOCK_(DEBUG_NAME(Name), __COUNTER__, ## __VA_ARGS__)
#define TIMED_FUNCTION(...) TIMED_BLOCK_(DEBUG_NAME(__FUNCTION__), ## __VA_ARGS__)

#define BEGIN_BLOCK_(GUID) { RecordBeginEvent(GUID);/* Record event begin block */ }
#define END_BLOCK_(GUID) { RecordEndEvent(GUID);/* Record event end block */  }

#define BEGIN_FRAME() { GlobalDebugEventList.clear(); }

#define BEGIN_BLOCK(Name) BEGIN_BLOCK_(DEBUG_NAME(Name))
#define END_BLOCK() END_BLOCK_(DEBUG_NAME("END_BLOCK_"))

struct timed_block
{
	const char* _GUID;
	timed_block(char *GUID, unsigned int HitCountInit = 1)
	{
		_GUID = GUID;
		BEGIN_BLOCK_(_GUID);
	}

	~timed_block()
	{
		END_BLOCK_(_GUID);
	}
};

#endif
