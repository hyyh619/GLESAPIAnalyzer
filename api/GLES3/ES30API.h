#ifndef _ES30_API_H
#define _ES30_API_H

#include <GLES3/gl31.h>
#include <GLES3/gl3ext.h>
#define GL_GLEXT_PROTOTYPES 1
#include <GLES2/gl2ext.h>
#include "ApiOESEngine.h"

#define CURRENT_GL_CONTEXT()            _GetThreadDataES3()

class CGLES3Context;

GLvoid*
GLES3CreateContext(
                   const stEngine * Eng
                   );

GLvoid
GLES3DestroyContext(
                    void * context
                    );

GLint
GLES3SetContext(
                void * context,
                const stEngine * Eng
                );

CGLES3Context*
_GetThreadDataES3(
               void
               );

#endif /* _ES30_API_H */