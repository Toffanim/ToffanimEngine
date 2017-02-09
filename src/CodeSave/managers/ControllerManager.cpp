// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
   $File: $
   $Date: $
   $Modified: 06-02-2017 07h50m20s $
   $Creator: Marc TOFFANIN $
   $Notice: Given as-is without warranty AT ALL that it's working$
   ======================================================================== */
#include "ControllerManager.h"
using namespace std;

ControllerManager::ControllerManager() : controllers( vector<Controller*>() )
{
}
