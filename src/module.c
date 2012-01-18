#include "fileflt.h"
/* ------------------------------------------------------------------------- */

int module_init(void)
{
	int rc = -1;

	if(TaskInit() == 0) {

#if (FLT_FRAMEWORK_TYPE_USED == FLT_FRAMEWORK_TYPE_DOUBLE_FCB)

	    g_FileFltContext.CacheManagerCallbacks.AcquireForLazyWrite  = SifsAcquireForLazyWrite;
	    g_FileFltContext.CacheManagerCallbacks.ReleaseFromLazyWrite = SifsReleaseFromLazyWrite;
	    g_FileFltContext.CacheManagerCallbacks.AcquireForReadAhead  = SifsAcquireForReadAhead;
	    g_FileFltContext.CacheManagerCallbacks.ReleaseFromReadAhead = SifsReleaseFromReadAhead;

	    g_FileFltContext.CacheManagerNoOpCallbacks.AcquireForLazyWrite  = SifsNoOpAcquire;
	    g_FileFltContext.CacheManagerNoOpCallbacks.ReleaseFromLazyWrite = SifsNoOpRelease;
	    g_FileFltContext.CacheManagerNoOpCallbacks.AcquireForReadAhead  = SifsNoOpAcquire;
	    g_FileFltContext.CacheManagerNoOpCallbacks.ReleaseFromReadAhead = SifsNoOpRelease;
	
	    switch ( MmQuerySystemSize() ) {

	    case MmSmallSystem:

	        g_FileFltContext.MaxDepth = 64;
	        break;

	    case MmMediumSystem:

	        g_FileFltContext.MaxDepth = 128;
	        break;

	    case MmLargeSystem:

	        g_FileFltContext.MaxDepth = 256;
	        break;
	    }
		
	    //
	    // Initialize the global data
	    //

	    ExInitializeNPagedLookasideList( &(g_FileFltContext.SifsIrpContextLookasideList),
	                                     NULL,
	                                     NULL,
	                                     0,
	                                     sizeof(SIFS_IRP_CONTEXT),
	                                     'PRIE',
	                                     0 );

	    ExInitializeNPagedLookasideList( &(g_FileFltContext.SifsFcbLookasideList),
	                                     NULL,
	                                     NULL,
	                                     0,
	                                     sizeof(SIFS_FCB),
	                                     'BCFE',
	                                     0 );

	    ExInitializePagedLookasideList( &(g_FileFltContext.SifsCcbLookasideList),
	                                    NULL,
	                                    NULL,
	                                    0,
	                                    sizeof(SIFS_CCB),
	                                    'BCCE',
	                                    0 );	    

	   ExInitializePagedLookasideList( &(g_FileFltContext.SifsMcbLookasideList),
                                    NULL,
                                    NULL,
                                    0,
                                    sizeof(SIFS_MCB),
                                    'BCME',
                                    0 );

#endif /* FLT_FRAMEWORK_TYPE_USED == FLT_FRAMEWORK_TYPE_DOUBLE_FCB */

	   rc = 0;
	}
	
	return rc;
}

void module_exit(void)
{
#if (FLT_FRAMEWORK_TYPE_USED == FLT_FRAMEWORK_TYPE_DOUBLE_FCB)

	ExDeletePagedLookasideList(&(g_FileFltContext.SifsMcbLookasideList));
	ExDeletePagedLookasideList(&(g_FileFltContext.SifsCcbLookasideList));
	ExDeleteNPagedLookasideList(&(g_FileFltContext.SifsFcbLookasideList));
	ExDeleteNPagedLookasideList(&(g_FileFltContext.SifsIrpContextLookasideList));
	
#endif /* FLT_FRAMEWORK_TYPE_USED == FLT_FRAMEWORK_TYPE_DOUBLE_FCB */

	TaskExit();
}