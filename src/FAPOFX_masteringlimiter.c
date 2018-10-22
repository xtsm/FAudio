/* FAudio - XAudio Reimplementation for FNA
 *
 * Copyright (c) 2011-2018 Ethan Lee, Luigi Auriemma, and the MonoGame Team
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software in a
 * product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Ethan "flibitijibibo" Lee <flibitijibibo@flibitijibibo.com>
 *
 */

#include "FAPOFX.h"
#include "FAudio_internal.h"

/* FXMasteringLimiter FAPO Implementation */

const FAudioGUID FAPOFX_CLSID_FXMasteringLimiter =
{
	0xA90BC001,
	0xE897,
	0xE897,
	{
		0x74,
		0x39,
		0x43,
		0x55,
		0x00,
		0x00,
		0x00,
		0x01
	}
};

static FAPORegistrationProperties FXMasteringLimiterProperties =
{
	/* .clsid = */ {0},
	/* .FriendlyName = */
	{
		'F', 'X', 'M', 'a', 's', 't', 'e', 'r', 'i', 'n', 'g', 'L', 'i', 'm', 'i', 't', 'e', 'r', '\0'
	},
	/*.CopyrightInfo = */
	{
		'C', 'o', 'p', 'y', 'r', 'i', 'g', 'h', 't', ' ', '(', 'c', ')',
		'E', 't', 'h', 'a', 'n', ' ', 'L', 'e', 'e', '\0'
	},
	/*.MajorVersion = */ 0,
	/*.MinorVersion = */ 0,
	/*.Flags = */(
		FAPO_FLAG_FRAMERATE_MUST_MATCH |
		FAPO_FLAG_BITSPERSAMPLE_MUST_MATCH |
		FAPO_FLAG_BUFFERCOUNT_MUST_MATCH |
		FAPO_FLAG_INPLACE_SUPPORTED |
		FAPO_FLAG_INPLACE_REQUIRED
	),
	/*.MinInputBufferCount = */ 1,
	/*.MaxInputBufferCount = */  1,
	/*.MinOutputBufferCount = */ 1,
	/*.MaxOutputBufferCount =*/ 1
};

typedef struct FAPOFXMasteringLimiter
{
	FAPOBase base;

	/* TODO */
} FAPOFXMasteringLimiter;

void FAPOFXMasteringLimiter_Process(
	FAPOFXMasteringLimiter *fapo,
	uint32_t InputProcessParameterCount,
	const FAPOProcessBufferParameters* pInputProcessParameters,
	uint32_t OutputProcessParameterCount,
	FAPOProcessBufferParameters* pOutputProcessParameters,
	uint8_t IsEnabled
) {
	FAPOBase_BeginProcess(&fapo->base);

	/* TODO */

	FAPOBase_EndProcess(&fapo->base);
}

void FAPOFXMasteringLimiter_Free(void* fapo)
{
	FAPOFXMasteringLimiter *limiter = (FAPOFXMasteringLimiter*) fapo;
	limiter->base.pFree(limiter->base.m_pParameterBlocks);
	limiter->base.pFree(fapo);
}

/* Public API */

uint32_t FAPOFXCreateMasteringLimiter(
	FAPO **pEffect,
	const void *pInitData,
	uint32_t InitDataByteSize,
	FAudioMallocFunc customMalloc,
	FAudioFreeFunc customFree,
	FAudioReallocFunc customRealloc
) {
	/* Allocate... */
	FAPOFXMasteringLimiter *result = (FAPOFXMasteringLimiter*) customMalloc(
		sizeof(FAPOFXMasteringLimiter)
	);
	uint8_t *params = (uint8_t*) customMalloc(
		sizeof(FAPOFXMasteringLimiterParameters) * 3
	);
	if (pInitData == NULL)
	{
		FAudio_zero(params, sizeof(FAPOFXMasteringLimiterParameters) * 3);
	}
	else
	{
		FAudio_assert(InitDataByteSize == sizeof(FAPOFXMasteringLimiterParameters));
		FAudio_memcpy(params, pInitData, InitDataByteSize);
		FAudio_memcpy(params + InitDataByteSize, pInitData, InitDataByteSize);
		FAudio_memcpy(params + (InitDataByteSize * 2), pInitData, InitDataByteSize);
	}

	/* Initialize... */
	FAudio_memcpy(
		&FXMasteringLimiterProperties.clsid,
		&FAPOFX_CLSID_FXMasteringLimiter,
		sizeof(FAudioGUID)
	);
	CreateFAPOBaseWithCustomAllocatorEXT(
		&result->base,
		&FXMasteringLimiterProperties,
		params,
		sizeof(FAPOFXMasteringLimiterParameters),
		0,
		customMalloc,
		customFree,
		customRealloc
	);

	/* Function table... */
	result->base.base.Process = (ProcessFunc)
		FAPOFXMasteringLimiter_Process;
	result->base.Destructor = FAPOFXMasteringLimiter_Free;

	/* Finally. */
	*pEffect = &result->base.base;
	return 0;
}
