
#ifndef glesinit_h
#define glesinit_h

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 480


/// initializes the gl conext with either version 1 for GLES1 or 2 for GLES2
void* GLES_Init( int version );

/// swapbuffer
int GLES_SwapBuffers( void* );

/// shutdown gles
int GLES_Exit( void* );


#endif


