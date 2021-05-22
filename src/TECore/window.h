// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TECore\window.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 22-08-2017 01h17m31s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */

#pragma once

namespace TE {
namespace Core {
namespace Window {

struct window;

void Init(window&, const char*, const char*);
void Show(window&);

} // namespace Window
} // namespace Core
} // namespace TE