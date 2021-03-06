/**
\author		Andrey Korotkov aka DRON
\date		26.04.2012 (c)Andrey Korotkov

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#pragma once

#include "..\..\Common.h"

#ifndef NO_BUILTIN_RENDERER

#include <gl\glew.h>
#include <gl\wglew.h>

class CBaseRendererGL : public CInstancedObj, public IBaseRenderGL
{
	HGLRC _hRC;
	TWinDrawHandle _hDC;

public:
	
	CBaseRendererGL(uint uiInstIdx);

	bool Prepare(TCRendererInitResult &stResults);
	bool Initialize(TCRendererInitResult &stResults);
	bool Finalize();
	bool AdjustMode(TEngWindow &stNewWin);
	bool MakeCurrent();
	void Present();

};

#endif