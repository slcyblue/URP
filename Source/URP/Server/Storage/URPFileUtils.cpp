#include "Server/Storage/URPFileUtils.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

bool UURPFileUtils::EnsureDirectory(const FString& Directory)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    return PlatformFile.CreateDirectoryTree(*Directory);
}
