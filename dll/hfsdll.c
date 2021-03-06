﻿#include "xpwn_common.h"

#include "abstractfile.h"
#include "hfs/hfslib.h"

#include "hfsdll.h"

HfsContext* hfslib_open(const char* fileName) {
	BOOL success = FALSE;
	HfsContext* ctx = (HfsContext*)malloc(sizeof(HfsContext));
	if (ctx == NULL) {
		return NULL;
	}
	memset (ctx, 0, sizeof(*ctx));
	do {
		ctx->io = openFlatFile(fileName);
		if(ctx->io == NULL) {
			//fprintf(stderr, "error: Cannot open image-file.\n");
			break;
		}
	
		ctx->volume = openVolume(ctx->io); 
		if(ctx->volume == NULL) {
			//fprintf(stderr, "error: Cannot open volume.\n");
			break;
		}
		success = TRUE;
	} while (FALSE);
	if (!success) {
		hfslib_close(ctx);
		ctx = NULL;
	}
	return ctx;
}

uint64_t hfslib_getsize(HfsContext* ctx) {
	HFSPlusVolumeHeader* h = ctx->volume->volumeHeader;
	return ((uint64_t)h->totalBlocks) * h->blockSize;
}

void hfslib_close(HfsContext* ctx) {
	if (ctx == NULL)
		return;
	if (ctx->volume != NULL) {
		closeVolume(ctx->volume);
	}
	if (ctx->io != NULL) {
		CLOSE(ctx->io);
	}
	free(ctx);
}

BOOL hfslib_extend(HfsContext* ctx, uint64_t newSize) {
	return grow_hfs(ctx->volume, newSize);
}

BOOL hfslib_untar(HfsContext* ctx, const char* tarFile) {
	AbstractFile *inFile = createAbstractFileFromFile(fopen(tarFile, "rb"));
	BOOL result = FALSE;
	if(inFile == NULL) {
		printf("file to untar not found");
	} else {
		result = hfs_untar(ctx->volume, inFile);
	}

	if (inFile != NULL) {
		inFile->close(inFile);
	}
	return result;
}

