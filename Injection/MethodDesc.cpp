#include "StdAfx.h"
#include "MethodDesc.h"
#include "Utility.h"
#include "SymbolAddressCache.h"
#include "Inspection.h"



MethodDesc::PFN_Reset MethodDesc::s_pfnReset = NULL;
MethodDesc::PFN_IsGenericMethodDefinition MethodDesc::s_pfnIsGenericMethodDefinition = NULL;
MethodDesc::PFN_GetNumGenericMethodArgs MethodDesc::s_pfnGetNumGenericMethodArgs = NULL;
MethodDesc::PFN_StripMethodInstantiation MethodDesc::s_pfnStripMethodInstantiation = NULL;
MethodDesc::PFN_HasClassOrMethodInstantiation MethodDesc::s_pfnHasClassOrMethodInstantiation = NULL;
MethodDesc::PFN_ContainsGenericVariables MethodDesc::s_pfnContainsGenericVariables = NULL;
MethodDesc::PFN_GetWrappedMethodDesc MethodDesc::s_pfnGetWrappedMethodDesc = NULL;
MethodDesc::PFN_GetDomain MethodDesc::s_pfnGetDomain = NULL;
MethodDesc::PFN_GetLoaderModule MethodDesc::s_pfnGetLoaderModule = NULL;

