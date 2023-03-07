#pragma once

#include <stdio.h>
#include <sstream>
#include <cassert>

#include "SimpleMath.h"

/*
Convenience macro for releasing COM objects (also known as resources).
If it isn't null, release it and nullify it.
These are functions/data that have been created by some other system/process
and they take up memory and will not go away until properly "released"
This macro just saves typing and therefore reduces silly bugs
*/
#define ReleaseCOM(x) \
{    if(x){    x->Release();    x = 0;    } }                            

/*
Gives us a way to pass messages to the output window in Visual Studio
while the game is running. This means you don't have to halt the app
if you don't want to (with an assert).
*/
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s << "\n";                   \
   OutputDebugString( os_.str().c_str() );  \
}

#define WDBOUT(s)                \
{                                \
   std::wostringstream os_;        \
   os_ << s << L"\n";                       \
   OutputDebugStringW( os_.str().c_str() );  \
}